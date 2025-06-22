FROM ubuntu:latest
COPY . /usr/src/Test
WORKDIR /usr/src/Test
RUN apt update
RUN apt install -y nano
RUN apt install -y g++
RUN g++ -o Test main.cpp
CMD ["./Test"]