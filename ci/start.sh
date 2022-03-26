# run docker

IMAGE=""
PROXY="http://10.100.116.132:1087"

docker run -it --device /dev/nvidiactl:/dev/nvidiactl \
--device /dev/nvidia-uvm:/dev/nvidia-uvm \
--device /dev/nvidia0:/dev/nvidia0 \
-v /usr/cuda_files:/usr/cuda_files \
-v /usr/bin/nvidia-smi:/usr/bin/nvidia-smi \
$(IMAGE) bash


#############
# in container
#############
export LD_LIBRARY_PATH=/usr/local/nvidia/lib:/usr/local/nvidia/lib64:/usr/cuda_files/
http_proxy=$PROXY https_proxy=$PROXY python3 train.py

