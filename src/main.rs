use signal_hook::consts::signal::*;
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
}
