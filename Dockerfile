FROM alpine:3.8
LABEL maintainer="Sylvain VISTE <viste.sylvain@gmail.com>"
ADD . /src/
RUN mkdir -p /etc/tinybooter.d/ \
&& apk add --no-cache tzdata libstdc++ build-base make clang \
&& cd /src/ \
&& make \
&& cp tinybooter /usr/bin/ \
&& cp healthcheck /usr/bin/ \
&& apk del build-base make clang \
&& rm -rf /src
HEALTHCHECK --interval=5s CMD ["healthcheck"]
CMD ["tinybooter", "/etc/tinybooter.d/"]
