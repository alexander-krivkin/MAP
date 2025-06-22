FROM ubuntu:latest
COPY . /usr/src/Test2.1
WORKDIR /usr/src/Test2.1
RUN apt update
RUN apt install -y g++
RUN g++ -o Test2.1 main.cpp
CMD ["./Test2.1"]