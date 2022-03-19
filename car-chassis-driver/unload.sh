#!/bin/bash

module="car-chassis-driver"
device="car-chassis"

rmmod ${module}
rm /dev/${device}