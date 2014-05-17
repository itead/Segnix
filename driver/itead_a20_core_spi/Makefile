KERN_DIR = /home/work/sw/awlinux/linux-sunxi


all:
	make -C $(KERN_DIR) M=`pwd` modules 

clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules.order

obj-m	+= spi-sun7i.o
