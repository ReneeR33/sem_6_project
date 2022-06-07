#!/bin/bash

module="camera-driver"
device="camera-device"

rmmod ${module}
rm /dev/${device}