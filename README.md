# Server

## Requirement

``` shell
apt install -y uuid-dev libssl-dev zlib1g-dev git
```

## Build

```shell
git submodule sync --recursive && git submodule update --init --recursive
cd include/uWebSockets && make && make install
cd ../..

mkdir build && cd build
cmake ..
make
```

## Library

* [crow](https://github.com/ipkn/crow)
* [nlohmann/json](https://github.com/nlohmann/json)