TARGET ?= producer


obj-m += $(TARGET).o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

load-producer:
	sudo insmod $(TARGET).ko

remove-module:
	sudo rmmod $(TARGET).ko

test:
	sudo dmesg