/**
 * Struct containing the tinybooter global config
 */
#[derive(Debug, PartialEq, Serialize, Deserialize)]
pub struct TinybooterConfig {
  pub healthcheck_port: u16,
  pub jobs: String,
}

/**
 * Enum for job kind config
 */
#[derive(Debug, PartialEq, Serialize, Deserialize)]
#[serde(rename_all = "lowercase")]
pub enum JobKind {
  Init,
  Service,
  TearDown,
}

/**
 * Struct containing config about the job to run
 */
#[derive(Debug, PartialEq, Serialize, Deserialize)]
pub struct JobConfig {
  pub kind: JobKind,
  pub cmd: String,
  pub args: Vec<String>,
}

pub type Jobs = Vec<JobConfig>;

/**
 * Reads global config from path
 */
pub fn read_config(configfile: String) -> Result<TinybooterConfig, Box<dyn std::error::Error>> {
  let f = std::fs::File::open(configfile)?;
  let config: TinybooterConfig = serde_yaml::from_reader(f)?;
  Ok(config)
}

/**
 * Reads jobs config from glob pattern
 */
pub fn read_jobs_config(pattern: String) -> Result<Jobs, Box<dyn std::error::Error>> {
  use glob::glob;

  let mut jobs = Vec::new();

  for entry in glob(&pattern).expect("Failed to read glob pattern") {
    match entry {
      Ok(path) => {
        let f = std::fs::File::open(&path)?;
        let config: JobConfig = serde_yaml::from_reader(f)?;
        jobs.push(config)
      }
      Err(e) => println!("{:?}", e),
    }
  }

  Ok(jobs)
}
