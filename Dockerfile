FROM alpine:latest
COPY ./Xerxes /opt/Xerxes/
COPY ./useragents /opt/Xerxes/
WORKDIR /opt/Xerxes
ENTRYPOINT ["/bin/sh"]