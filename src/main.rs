/*use signal_hook::consts::signal::*;
use signal_hook::low_level;

use std::io::Error;

#[cfg(feature = "extended-siginfo")]
type Signals =
  signal_hook::iterator::SignalsInfo<signal_hook::iterator::exfiltrator::origin::WithOrigin>;

#[cfg(not(feature = "extended-siginfo"))]
use signal_hook::iterator::Signals;

fn main() -> Result<(), Error> {
  const SIGNALS: &[i32] = &[
    SIGTERM, SIGQUIT, SIGINT, SIGTSTP, SIGWINCH, SIGHUP, SIGCHLD, SIGCONT,
  ];
  let mut sigs = Signals::new(SIGNALS)?;
  for signal in &mut sigs {
    match signal {
      SIGINT => {
        println!("\nGot SIGINT");
      }
      SIGTERM => {
        println!("\nGot SIGTERM");
      }
      term_sig => {
        println!("\nGot {:?}", term_sig);
      }
    }
    #[cfg(feature = "extended-siginfo")]
    let signal = signal.signal;
    // After printing it, do whatever the signal was supposed to do in the first place
    low_level::emulate_default_handler(signal)?;
  }
  Ok(())
}*/

//mod healthcheck;

#![feature(proc_macro_hygiene, decl_macro)]

#[macro_use]
extern crate serde_derive;
extern crate serde_yaml;

#[derive(Debug, PartialEq, Serialize, Deserialize)]
struct TinybooterConfig {
  healthcheck_port: u16,
}

#[macro_use]
extern crate rocket;
use rocket::config::{Config, Environment};
use rocket::http::Status;

extern crate clap;
use clap::{App, Arg};

static ready: bool = false;
static started: bool = true;
static live: bool = true;

#[get("/readiness")]
fn status_readiness() -> Status {
  if ready {
    Status::Accepted
  } else {
    Status::NotAcceptable
  }
}

#[get("/liveness")]
fn status_liveness() -> Status {
  if live {
    Status::Accepted
  } else {
    Status::NotAcceptable
  }
}

#[get("/startup")]
fn status_startup() -> Status {
  if started {
    Status::Accepted
  } else {
    Status::NotAcceptable
  }
}

fn read_config(configfile: &str) -> Result<TinybooterConfig, Box<dyn std::error::Error>> {
  let f = std::fs::File::open(configfile)?;
  let config: TinybooterConfig = serde_yaml::from_reader(f)?;
  Ok(config)
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
  let matches = App::new("Tinybooter")
    .version("1.0.0")
    .author("Sylvain VISTE <viste.sylvain@protonmail.com>")
    .about("Process launcher for Docker and Kubernetes")
    .arg(
      Arg::with_name("config")
        .short("c")
        .long("config")
        .value_name("FILE"),
    )
    .get_matches();

  let configfile = matches
    .value_of("config")
    .unwrap_or("/etc/tinybooter/tinybooter.yaml");
  println!("Value for config: {}", configfile);

  let config = read_config(configfile)?;

  let rocket_config = Config::build(Environment::Staging)
    .port(config.healthcheck_port)
    .finalize()?;

  rocket::custom(rocket_config)
    .mount(
      "/",
      routes![status_startup, status_liveness, status_readiness],
    )
    .launch();

  Ok(())
}
