#
# Change KERN_DIR to your linux-sunxi-3.4 source tree
# make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-
#
KERN_DIR = /your-linux-sunxi-3.4


all:
	make -C $(KERN_DIR) M=`pwd` modules 

clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules.order

obj-m	+= spi-sun7i.o
