FROM ubuntu:latest
COPY ./Xerxes /opt/Xerxes/
COPY ./useragents /opt/Xerxes/
RUN apt-get update && apt-get install -y libssl-dev
WORKDIR /opt/Xerxes
ENTRYPOINT ["/bin/sh"]