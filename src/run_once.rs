use crate::config;
use std::process::Command;

pub fn run_once(jobs: &config::Jobs, kind: config::JobKind) {
  for (id, job) in jobs.iter().enumerate() {
    if job.kind == kind {
      let status = Command::new(&job.cmd)
        .args(&job.args)
        .status()
        .expect("failed to execute");

      if status.success() {
        println!("Runned");
      } else {
        println!("Error");
      }
    }
  }
}
