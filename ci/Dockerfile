FROM nvidia/cuda:11.4.1-cudnn8-devel-ubuntu20.04

RUN apt-get update && apt-get install -y python3 \
    python3-pip \
    vim \
    git \
    wget \
    zip

RUN wget https://github.com/coder/code-server/releases/download/v4.1.0/code-server_4.1.0_amd64.deb \
    --no-verbose \
    --show-progress \
    --progress=bar:force:noscroll && \
    dpkg -i code-server_4.1.0_amd64.deb && \
    rm -rf code-server_4.1.0_amd64.deb

RUN pip3 install tensorflow \
    tflite-model-maker \
    pycocotools \
    tflite-support \
    flatbuffers==1.12 \
    gym \
    ray==1.11.0 \
    pandas \
    numpy -i  https://pypi.tuna.tsinghua.edu.cn/simple

EXPOSE 8080

ENTRYPOINT ["code-server" ,"--bind-addr=0.0.0.0:8080", "--auth=none"]