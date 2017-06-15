FROM alpine:3.6 as BUILD

WORKDIR /code

RUN apk add --no-cache \
    g++ cmake make util-linux-dev openssl-dev zlib-dev git

COPY . /code

RUN git submodule sync --recursive && git submodule update --init --recursive && \
    cd include/uWebSockets && make && make install && \
    cd ../.. && \
    mkdir build && cd build && cmake .. && make


FROM alpine:3.6

RUN apk add --no-cache \
    libstdc++ libgcc util-linux-dev openssl-dev zlib-dev make
WORKDIR /app/build
COPY --from=BUILD /code /app
RUN cd /app/include/uWebSockets && make install

ENV PORT 4000

EXPOSE $PORT/tcp

CMD ["./server"]
