KERN_DIR = /home/wupf/work/kernel/linux-sunxi-wupf


all:
	make -C $(KERN_DIR) M=`pwd` modules 

clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules.order

obj-m	+= itead_a10_a20_core_gpio.o
