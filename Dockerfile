# Build/test environment
FROM archlinux/base AS build-env
RUN pacman -Sy --noconfirm clang make gtest gmock
ADD . /src/
RUN cd /src/ && make test

# Runtime environment
FROM debian:buster-slim
LABEL maintainer="Sylvain VISTE <viste.sylvain@gmail.com>"
COPY --from=build-env /src/tinybooter /src/healthcheck /bin/
RUN mkdir -p /etc/tinybooter.d/
HEALTHCHECK --interval=5s CMD ["healthcheck"]
CMD ["tinybooter", "/etc/tinybooter.d/"]
