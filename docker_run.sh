docker run  --name ft-detector -it --rm \
            --net host \
            --privileged \
            --device=/dev/video0 \
            --gpus=all \
            -v $HOME/.Xauthority:/root/.Xauthority:rw \
            -v $XSOCK:$XSOCK \
            -v /run/user/1000/pulse/native:/run/user/1000/pulse/native \
            -e PULSE_SERVER=/run/user/1000/pulse/native \
            -e DISPLAY=$DISPLAY \
            -u $UID:$UID \
            ftdet:v1 ./build_docker/src/ft-detector-app/ft-detector-app
