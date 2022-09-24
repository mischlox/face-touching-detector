# Face Touching Detector

Face Touching Detector is an application that prevents me from touching my face by detecting the overlap of hands and faces.

## Contributions

* **YOLOv5**m model trained on [Human-Parts Dataset](https://github.com/xiaojie1017/Human-Parts.git)
* **Libtorch C++** implementation of YOLOv5 inference
* **Docker Container Image** for easy deployment
* Simple **QT5** GUI for visualisation of results
* **Detector Interface** that allows to implement other detector-applications

## Demo video

<div style="text-align: center;">
<a href="https://www.youtube.com/watch?feature=player_embedded&v=oIkSHqMhEus" target="_blank">
 <img src="https://img.youtube.com/vi/oIkSHqMhEus/0.jpg" alt="Watch the video" width="480" />
</a>
</div>

## Getting started

### Environment

This application was tested in the following environment.

* GPU: GeForce GTX 1660 SUPER
* Ubuntu 20.04
* CUDA 11.3
* OpenCV 4.2.0
* Libtorch 1.12.1
* Spdlog 1.10.0
* Boost Program Options 1.71.0
* QT 5.12.8

### Installation
#### Download Libtorch

Select version compatible to your environment from [here](https://pytorch.org/get-started/locally/), e.g. for CUDA 11.3:

    wget https://download.pytorch.org/libtorch/cu113/libtorch-cxx11-abi-shared-with-deps-1.12.1%2Bcu113.zip
    unzip libtorch-cxx11-abi-shared-with-deps-1.12.1+cu113.zip -d deps/

#### Build

    mkdir build 
    cd build
    cmake ..
    make

### Build and run from Docker container

Install Nvidia Container Toolkit for GPU support

    sudo apt-get update
    sudo apt-get install nvidia-container-toolkit

Build container

    ./docker_build.sh

Run docker container interactively with the provided script ```./docker_run.sh``` and build app

## Train and deploy custom model

todo

## Acknowledgements

* [YOLOv5](https://github.com/ultralytics/yolov5)
* [YOLOv5 in Libtorch](https://github.com/Nebula4869/YOLOv5-LibTorch/) 
* [Human-Parts Dataset](https://github.com/xiaojie1017/Human-Parts.git)