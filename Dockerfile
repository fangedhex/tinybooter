# Build/test environment
FROM debian:buster-slim AS build-env
RUN apt-get update && apt-get install -y --no-install-recommends build-essential cmake libgtest-dev libgmock-dev
ADD . /src/
RUN mkdir -p /src/build && \
cd /src/build && \
cmake -DCMAKE_BUILD_TYPE=Release .. && \
make && /src/build/main/tinybooter_test

# Runtime environment
FROM debian:buster-slim
LABEL maintainer="Sylvain VISTE <viste.sylvain@gmail.com>"
COPY --from=build-env /src/build/main/tinybooter /src/build/healthcheck/healthcheck /bin/
RUN mkdir -p /etc/tinybooter.d/
HEALTHCHECK --interval=5s CMD ["healthcheck"]
CMD ["tinybooter", "/etc/tinybooter.d/"]
