# /*

SDKCCOMPILE 	:= gcc
SDKCPPCOMPILE 	:= g++

export SDKCCOMPILE SDKCPPCOMPILE

SUBDIRS := lib
SUBDIRS += bootloader
SUBDIRS += libraries


SUBDIRS_CLEAN := $(addsuffix .clean,$(SUBDIRS))

all: $(SUBDIRS)
	@echo "all done"

$(SUBDIRS):
	$(MAKE) -C $@ all


clean: $(SUBDIRS_CLEAN)
	@echo "clean done"

$(SUBDIRS_CLEAN):
	$(MAKE) -C $(subst .clean,,$@) clean

update: clean uninstall all install
	@echo "update done"


install:
	chmod 755 ./* -R
	
	cp lib/libiteadc.so /usr/lib
	cp lib/libiteadc.so /usr/local/lib

	cp lib/libiteadcpp.so /usr/lib
	cp lib/libiteadcpp.so /usr/local/lib
	
	cp libraries/libiteadmodule.so /usr/lib
	cp libraries/libiteadmodule.so /usr/local/lib

	cp include/itead*.h /usr/include
	cp include/itead*.h /usr/local/include
	
	cp bootloader/iteadboot.o /usr/lib
	cp bootloader/iteadboot.o /usr/local/lib
	
	cp tools/iteadcompile /usr/bin
	cp tools/iteadcompile /usr/local/bin
	
	cp tools/20-itead.rules /etc/udev/rules.d/

	cp libraries/itead_LCD4Bit_mod/itead_LCD4Bit_mod.h /usr/include
	cp libraries/itead_LCD4Bit_mod/itead_LCD4Bit_mod.h /usr/local/include

	cp libraries/itead_GFX/itead_GFX.h /usr/include
	cp libraries/itead_GFX/itead_GFX.h /usr/local/include

	cp libraries/itead_SSD1306/itead_SSD1306.h /usr/include
	cp libraries/itead_SSD1306/itead_SSD1306.h /usr/local/include
	
	@echo "install done"

uninstall:
	rm -rf /usr/lib/libiteadc.so 
	rm -rf /usr/local/lib/libiteadc.so
	
	rm -rf /usr/lib/libiteadcpp.so 
	rm -rf /usr/local/lib/libiteadcpp.so
	
	rm -rf /usr/lib/libiteadmodule.so
	rm -rf /usr/local/lib/libiteadmodule.so
	
	rm -rf /usr/include/itead*.h
	rm -rf /usr/local/include/itead*.h
	
	rm -rf /usr/lib/iteadboot.o
	rm -rf /usr/local/lib/iteadboot.o
	
	rm -rf /usr/bin/iteadcompile
	rm -rf /usr/local/bin/iteadcompile
	
	rm -rf /etc/udev/rules.d/20-itead.rules
	
	rm -rf /usr/include/itead_LCD4Bit_mod.h
	rm -rf /usr/local/include/itead_LCD4Bit_mod.h

	rm -rf /usr/include/itead_GFX.h
	rm -rf /usr/local/include/itead_GFX.h

	rm -rf /usr/include/itead_SSD1306.h
	rm -rf /usr/local/include/itead_SSD1306.h
	
	@echo "uninstall done"



.PHONY: all install clean uninstall update $(SUBDIRS_CLEAN) $(SUBDIRS)