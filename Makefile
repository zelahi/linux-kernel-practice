build:
	docker build -t my-test:v1.0 .

run:
	docker run --privileged -it my-test:v1.0

obj-m += test.o
all:
	make -C /lib/modules/$(uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(uname -r)/build M=$(PWD) clean

load-module:
	sudo insmod network.ko