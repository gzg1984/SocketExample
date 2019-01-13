FROM ubuntu:latest
MAINTAINER Maxpain <g.zg1984@gmail.com>
RUN sed -i s@/archive.ubuntu.com/@/mirrors.aliyun.com/@g /etc/apt/sources.list
RUN apt update
RUN apt-get install -y git
