#!/bin/sh

apt install -y g++ cmake uuid-dev libssl-dev zlib1g-dev git
git submodule sync --recursive && git submodule update --init --recursive
cd include/uWebSockets && make && make install
cd ../..