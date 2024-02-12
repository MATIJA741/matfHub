#!/bin/bash

sudo apt-get install -y liblapack-dev
sudo apt-get install -y libblas-dev
sudo apt-get install -y libboost-dev
sudo apt-get install -y libarmadillo-dev
sudo apt-get install -y gnuplot


DEPENDENCIES_DIR="./matfHub/dependencies"
VALID_COMMIT_HASH="f1ed26f"


if [! -d ${DEPENDENCIES_DIR}]; 
then
	mkdir -p ${DEPENDENCIES_DIR}
fi

if [[ ! -e ${DEPENDENCIES_DIR}/.git ]]; 
then
    # Directory is not a Git repository and is not empty, delete everything
    rm -rf ${DEPENDENCIES_DIR}
    mkdir ${DEPENDENCIES_DIR}

    # Perform the clone
    git clone https://github.com/Waqar144/QSourceHighlite.git "$DEPENDENCIES_DIR"
    cd ${DEPENDENCIES_DIR}
    git reset --hard ${VALID_COMMIT_HASH}
    rm -rf .git
    rm main.cpp
    rm mainwindow.cpp
    rm mainwindow.h
    rm mainwindow.ui
    rm -rf screenshot
    rm -rf test_files
fi
