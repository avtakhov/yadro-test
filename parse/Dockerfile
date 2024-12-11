FROM ubuntu:22.04

WORKDIR /app
COPY install.sh /app/install.sh
RUN ./install.sh

COPY . /app
CMD ["./start.sh", "/tmp/example/a.txt", "/tmp/example/a.bin", "/tmp/example/a.json"]
