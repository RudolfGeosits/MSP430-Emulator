#!/bin/bash

echo "$(whoami)"
[ "$UID" -eq 0 ] || exec sudo "$0" "$@"

apt update;
apt install -y gcc-msp430 msp430mcu msp430-libc;
apt install -y libreadline-dev libssl-dev libwebsockets-dev;

echo "Done Installing Deps"
