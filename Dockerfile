FROM nvidia/cuda:11.4.1-cudnn8-devel-ubuntu20.04

RUN apt-get update && apt-get install -y python3 \
    python3-pip \
    vim \
    git \
    wget \
    zip

RUN pip3 install tensorflow \
    tflite-model-maker \
    pycocotools \
    tflite-support \
    flatbuffers==1.12 \
    numpy -i  https://pypi.tuna.tsinghua.edu.cn/simple
# export LD_LIBRARY_PATH=/usr/local/nvidia/lib:/usr/local/nvidia/lib64:/usr/cuda_files/