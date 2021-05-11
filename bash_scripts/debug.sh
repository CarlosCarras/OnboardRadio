#!/usr/bin/env bash

BUS=2
ADDR=0x25
REG=0x0D

read -p 'LED to Turn On: ' led

case "$led" in
  "0")
    echo "Turning on LED 0."
    cmd=0x01
    ;;
  "1")
    echo "Turning on LED 1."
    cmd=0x02
    ;;
  "2")
    echo "Turning on LEDs 0 and 1."
    cmd=0x03
    ;;
  *)
    echo "Invalid LED."
    exit 1
    ;;
esac

i2cset -y $BUS $ADDR $REG $cmd
