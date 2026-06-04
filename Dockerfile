FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    cmake \
    g++ \
    libgtest-dev \
    git \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

RUN rm -rf build
RUN mkdir -p build && cd build && cmake .. -DBUILD_GUI=OFF && make tests calculator

CMD ["./build/calculator"]
