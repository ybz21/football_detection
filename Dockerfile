FROM ubuntu:20.04

RUN apt-get update && apt-get install -y python3 \
    python3-pip \
    vim \
    git \
    wget

RUN pip3 install tensorflow \
    tflite-model-maker \
    pycocotools \
    numpy -i  https://pypi.tuna.tsinghua.edu.cn/simple