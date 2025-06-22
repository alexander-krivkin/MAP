FROM ubuntu:latest
COPY . /usr/src/Test2.2
WORKDIR /usr/src/Test2.2
RUN apt update
RUN apt install -y clang
RUN clang -lstdc++ -o Test2.2 main.cpp
CMD ["./Test2.2"]