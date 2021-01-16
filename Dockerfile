# Build environment
FROM alpine AS build-env
RUN apk add --no-cache build-base clang cmake
ENV CC=clang CXX=clang++
#RUN apt-get update && apt-get install -y --no-install-recommends build-essential cmake
ADD . /src/
RUN mkdir -p /src/build && \
  cd /src/build && \
  cmake -DCMAKE_BUILD_TYPE=Release .. && \
  make

# Runtime environment
FROM alpine
LABEL maintainer="Sylvain VISTE <viste.sylvain@gmail.com>"
COPY --from=build-env /src/build/main/tinybooter /src/build/healthcheck/healthcheck /bin/
RUN mkdir -p /etc/tinybooter.d/
HEALTHCHECK --interval=5s CMD ["healthcheck"]
CMD ["tinybooter", "/etc/tinybooter.d/"]
