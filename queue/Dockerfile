FROM alpine:3.4

RUN apk add --update bash file file-dev autoconf automake libtool build-base && rm -rf /var/cache/apk/* && apk add --update flex flex-dev bison zlib zlib-dev && rm -rf /var/cache/apk/* && apk add --update curl-dev libmicrohttpd-dev boost-dev && rm -rf /var/cache/apk/*

WORKDIR /tmp/crawler
COPY ["configure.ac", "Makefile.am", "COPYING", "INSTALL", "NEWS", "README", "AUTHORS", "ChangeLog", "./"]
#COPY ["data", "data"]
COPY ["src", "src"]
COPY ["m4", "m4"]

RUN mkdir -p m4 && \
    autoreconf -iv -I m4 && \
    ./configure --disable-debug && \
    make all install

#VOLUME /crawl
#VOLUME /binary

##WORKDIR /crawl
#RUN rm -rf /tmp/crawler
#ENTRYPOINT ["crawler"]
#CMD ["--help"]
