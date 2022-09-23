#!/bin/bash

docker run  --name ft-detector -it --rm \
            --net host \
            --privileged \
            --device=/dev/video0 \
            --gpus=all \
            -v $PWD/:/app \
            -v $HOME/.Xauthority:/root/.Xauthority:rw \
            -v $XSOCK:$XSOCK \
            -v /run/user/1000/pulse/native:/run/user/1000/pulse/native \
            -e PULSE_SERVER=/run/user/1000/pulse/native \
            -e DISPLAY=$DISPLAY \
            ftdet-img bash