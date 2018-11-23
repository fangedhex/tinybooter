FROM debian:buster-slim
LABEL maintainer="Sylvain VISTE <viste.sylvain@gmail.com>"
ADD . /src/
RUN mkdir -p /etc/tinybooter.d/ \
&& apt-get update && apt-get install -y build-essential make clang \
&& cd /src/ \
&& make \
&& cp tinybooter /usr/bin/ \
&& cp healthcheck /usr/bin/ \
&& apt-get remove -y build-essential make clang && rm -rf /var/lib/apt/lists/* \
&& rm -rf /src
HEALTHCHECK --interval=5s CMD ["healthcheck"]
CMD ["tinybooter", "/etc/tinybooter.d/"]
