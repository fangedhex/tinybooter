#!/bin/sh
mkdir -p /etc/tinybooter.d/
apk add --no-cache tzdata libstdc++ build-base make clang
cd /src/
make
cp tinybooter /usr/bin/
cp healthcheck /usr/bin/
apk del build-base make clang
rm -rf /src
