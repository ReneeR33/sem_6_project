#!/bin/bash

module="camera-driver"
device="camera-device"

insmod build/${module}.ko || exit -1

major=$(awk "\$2==\"$module\" {print \$1}" /proc/devices)

rm -f /dev/${device}
mknod /dev/${device} c $major 0