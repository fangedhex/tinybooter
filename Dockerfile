# Build environment
FROM rhub/fedora-clang-devel AS build-env
ADD . /src/
RUN cd /src/ && make

# Runtime environment
FROM debian:buster-slim
LABEL maintainer="Sylvain VISTE <viste.sylvain@gmail.com>"
COPY --from=build-env /src/tinybooter /src/healthcheck /bin/
RUN mkdir -p /etc/tinybooter.d/
HEALTHCHECK --interval=5s CMD ["healthcheck"]
CMD ["tinybooter", "/etc/tinybooter.d/"]
