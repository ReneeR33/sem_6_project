ccflags-y := -std=gnu99 -Wno-declaration-after-statement
obj-m += car-chassis-driver.o
car-chassis-driver-objs := driver.o gpio.o wheels.o

BUILD_DIR=$(PWD)/build
BUILD_MAKEFILE=$(BUILD_DIR)/Makefile

all: $(BUILD_MAKEFILE)
	make -C /lib/modules/$(shell uname -r)/build M=$(BUILD_DIR) src=$(PWD) modules

$(BUILD_MAKEFILE):
	mkdir -p $(BUILD_DIR)
	touch "$@"

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(BUILD_DIR) src=$(PWD) clean