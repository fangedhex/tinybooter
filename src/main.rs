#![feature(proc_macro_hygiene, decl_macro)]

#[macro_use]
extern crate serde_derive;
extern crate serde_yaml;

extern crate clap;
use clap::{App, Arg};

mod config;
mod run_once;
mod run_service;

fn main() -> Result<(), Box<dyn std::error::Error>> {
  // Setup CLI lib for parsing
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

  // Load general and jobs configurations
  let config_data = config::read_config(configfile.to_string())?;
  let jobs = config::read_jobs_config(config_data.jobs)?;

  // Run init jobs
  run_once::run_once(&jobs, config::JobKind::Init);

  // Run service jobs
  run_service::run(&jobs);

  // Run "cleanup" jobs
  run_once::run_once(&jobs, config::JobKind::TearDown);

  Ok(())
}
