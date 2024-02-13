FROM ubuntu:22.04

RUN apt-get -y update && DEBIAN_FRONTEND=noninteractive apt-get -y install \
    git cmake python3 python3-pip build-essential

RUN pip3 install aqtinstall

ARG QT=6.6.0
ARG QT_MODULES=
ARG QT_HOST=linux
ARG QT_TARGET=desktop
ARG QT_ARCH=gcc_64

RUN aqt install-qt ${QT_HOST} ${QT_TARGET} ${QT} ${QT_ARCH} --outputdir /opt/qt -m ${QT_MODULES}

ENV PATH /opt/qt/${QT}/gcc_64/bin:$PATH
ENV QT_PLUGIN_PATH /opt/qt/${QT}/gcc_64/plugins/
ENV QML_IMPORT_PATH /opt/qt/${QT}/gcc_64/qml/
ENV QML2_IMPORT_PATH /opt/qt/${QT}/gcc_64/qml/


RUN git clone https://github.com/catchorg/Catch2.git \
    && cd Catch2 \
    && cmake -Bbuild -H. -DBUILD_TESTING=OFF \
    && cmake --build build/ --target install

WORKDIR ./tests/matfHub/test

COPY . /tests

CMD ["./docker_testovi.sh"]
