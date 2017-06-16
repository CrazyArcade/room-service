FROM alpine:3.6 as BUILD
LABEL maintainer "i@giuem.com"

WORKDIR /code
COPY include /code/include

RUN apk add --no-cache \
    g++ cmake make util-linux-dev openssl-dev zlib-dev && \
    cd include/uWebSockets && make && make install && cd ../..

COPY . /code

RUN mkdir build && cd build && cmake -DCMAKE_BUILD_TYPE=Release .. && make


FROM alpine:3.6
LABEL maintainer "i@giuem.com"

RUN apk add --no-cache \
    libstdc++ libgcc util-linux-dev openssl-dev zlib-dev
WORKDIR /app/build
COPY --from=BUILD /code /app
RUN cd /app/include/uWebSockets && make install

ENV PORT 4000

EXPOSE $PORT/tcp

CMD ["./server"]
