#-----------------#
# MSP430 Emulator #
#-----------------#
FROM ubuntu:19.10

# Meta
LABEL PROJECT_NAME="MSP430 Emulator"
LABEL PROJECT_CODE="MSPEMU"
LABEL VENDOR="Public"
LABEL ARTEFACT="MSP430 emulator"
LABEL DESCRIPTION="MSP430 emulator forked from github.com/RudolsGeosits/MSP430-Emulator and improved for use in CI"
LABEL LICENSE="GPLv3"

# Variables (not part of the container ENV)
ARG USER_NAME=msp
ARG HOME_DIR=/home/${USER_NAME}
ARG WORK_DIR=${HOME_DIR}/msp430-emulator

# Basic utilities
RUN apt-get update && apt-get install -y \
    g++ \
    gcc-msp430 \
    libreadline-dev \
    libssl-dev \
    libwebsockets-dev \
    make \
    msp430mcu \
    msp430-libc

# Directory setup
ENV HOME=${HOME_DIR}
RUN mkdir -p ${HOME_DIR} && \
    mkdir -p ${WORK_DIR}

ENV PATH=${PATH}:${WORK_DIR}

WORKDIR ${WORK_DIR}

# Sources (not moved into src folder to preserve compatibility with the parent repository)
# COPY dir behaves as cp dir/*, therefore directories need to be copied separately
# 430x2_firm is not needed
COPY debugger ${WORK_DIR}/debugger
COPY devices ${WORK_DIR}/devices
COPY test ${WORK_DIR}/test
COPY Makefile \
     main.cpp \
     main.h \
     ${WORK_DIR}/

# Build
RUN make -C ${WORK_DIR} all

# Test
RUN make -C ${WORK_DIR} test