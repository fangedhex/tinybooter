use actix_web::{get, web, App, HttpServer, Responder};

#[get("/{id}/{name}")]
async fn index(web::Path((id, name)): web::Path<(u32, String)>) -> impl Responder {
  format!("Hello {}! id:{}", name, id)
}

pub async fn healthcheck() -> std::io::Result<()> {
  HttpServer::new(|| App::new().service(index))
    .bind("127.0.0.1:8080")?
    .run()
    .await
}
