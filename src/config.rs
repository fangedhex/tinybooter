#[derive(Debug, PartialEq, Serialize, Deserialize)]
pub struct TinybooterConfig {
  pub healthcheck_port: u16,
  pub pods: String,
}

#[derive(Debug, PartialEq, Serialize, Deserialize)]
#[serde(rename_all = "lowercase")]
pub enum PodKind {
  Init,
  Service,
  TearDown,
}

#[derive(Debug, PartialEq, Serialize, Deserialize)]
pub struct PodConfig {
  pub kind: PodKind,
}
