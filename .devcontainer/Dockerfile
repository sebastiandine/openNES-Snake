FROM  debian:bullseye

RUN apt update && apt install -y build-essential git doxygen graphviz
RUN git clone https://github.com/cc65/cc65.git && cd cc65 && make && make avail
