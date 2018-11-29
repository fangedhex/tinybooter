FROM debian:buster-slim
LABEL maintainer="Sylvain VISTE <viste.sylvain@gmail.com>"
RUN apt-get update && apt-get install -y --no-install-recommends build-essential make clang
ADD . /src/
RUN mkdir -p /etc/tinybooter.d/ \
&& cd /src/ \
&& make \
&& cp tinybooter /usr/bin/ \
&& cp healthcheck /usr/bin/ \
&& apt-get remove -y build-essential make clang && rm -rf /var/lib/apt/lists/* \
&& rm -rf /src
HEALTHCHECK --interval=5s CMD ["healthcheck"]
CMD ["tinybooter", "/etc/tinybooter.d/"]
