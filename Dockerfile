FROM gzg1984/dev_ubuntu:latest
MAINTAINER Maxpain <g.zg1984@gmail.com>
RUN git clone https://github.com/gzg1984/SocketExample.git
WORKDIR SocketExample
RUN make
EXPOSE 8888
CMD ["/start.sh"]
CMD ["/SocketExample/ServerApp","&"]

