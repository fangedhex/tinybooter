FROM alpine:3.8
ADD . /src/
RUN chmod +x /src/install.sh && /src/install.sh
HEALTHCHECK --interval=1s CMD ["healthcheck"]
CMD ["tinybooter", "/etc/tinybooter.d/"]
