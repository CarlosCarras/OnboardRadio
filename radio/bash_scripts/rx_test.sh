#!/usr/bin/env bash

BUS=2
ADDR=0x25
REG=0x2A 	# RSSI register

value=i2cget -y $BUS $ADDR $REG w

echo "Raw RSSI Value: "
echo $value
echo -e "\nRSSI Output Voltage: "
echo "scale=2 ; $value * 3/ 4096" | bc
