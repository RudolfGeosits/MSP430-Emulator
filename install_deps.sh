#!/bin/bash

echo "$(whoami)"
[ "$UID" -eq 0 ] || exec sudo "$0" "$@"

apt update;
apt install gcc-msp430 msp430mcu msp430-libc;
apt install libreadline-dev libssl-dev;

echo "Done Attempting Install"
