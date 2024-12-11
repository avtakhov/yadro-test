#!/bin/bash

apt-get update && apt-get install -y \
    gcc g++ \
    cmake \
    wget

wget -qO- https://boostorg.jfrog.io/artifactory/main/release/1.86.0/source/boost_1_86_0.tar.gz | tar xz

cd boost_1_86_0
./bootstrap.sh gcc
./b2 --with-json install
