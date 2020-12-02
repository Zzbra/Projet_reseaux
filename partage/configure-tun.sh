#!/bin/bash
# configure-tun.sh

sudo ip addr add fc00:1234:ffff::1/64 dev tun0
sudo ip link set tun0 up
ip addr show dev tun0