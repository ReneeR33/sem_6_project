#!/bin/bash

module="car-chassis-driver"
device="car-chassis"

insmod build/${module}.ko || exit -1

major=$(awk "\$2==\"$module\" {print \$1}" /proc/devices)

rm -f /dev/${device}
mknod /dev/${device} c $major 0