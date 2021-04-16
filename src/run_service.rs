use crate::config;
use std::process::Command;
use std::thread;

pub fn run(jobs: &config::Jobs) {
  let mut threads: Vec<thread::JoinHandle<()>> = vec![];

  for (id, job) in jobs.iter().enumerate() {
    if job.kind == config::JobKind::Service {
      let handle = thread::spawn(move || {
        let status = Command::new(&job.cmd)
          .args(&job.args)
          .status()
          .expect("failed to execute");

        /*if status.success() {
          println!("Runned");
        } else {
          println!("Error");
        }*/
      });
      threads.push(handle);
    }
  }
}
