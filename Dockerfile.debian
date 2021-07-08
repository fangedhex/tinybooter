FROM debian:sid-slim AS builder
WORKDIR /src
ADD . .
RUN apt-get update && apt-get install -y build-essential cmake ninja-build git \
&& cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release \
&& cd build && ninja
FROM debian:sid-slim
COPY --from=builder /src/build/main/tinybooter /usr/bin/tinybooter
