SDKCCOMPILE 	:= gcc
SDKCPPCOMPILE 	:= g++

SDK_LIB_OPTIMIZE_OPTION	:= -O2

export SDKCCOMPILE SDKCPPCOMPILE SDK_LIB_OPTIMIZE_OPTION

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

distclean: clean
	rm -rf include/itead_config.h
	@echo "distclean done"

update: uninstall clean all install
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
	
	cp libraries/itead_TinyGPS/itead_TinyGPS.h /usr/include
	cp libraries/itead_TinyGPS/itead_TinyGPS.h /usr/local/include
	
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
	
	rm -rf /usr/include/itead_TinyGPS.h
	rm -rf /usr/local/include/itead_TinyGPS.h
	
	@echo "uninstall done"

.PHONY: all install clean distclean uninstall update $(SUBDIRS_CLEAN) $(SUBDIRS) 


.PHONY: Iteaduino_Plus
Iteaduino_Plus:
	@ echo "#ifndef __ITEAD_CONFIG_H__" > include/itead_config.h
	@ echo "#define __ITEAD_CONFIG_H__" >> include/itead_config.h
	@ echo "#define BOARD_ITEADUINO_PLUS" >> include/itead_config.h
	@ echo "#endif" >> include/itead_config.h
	@ echo "config board Iteaduino_Plus finished"

.PHONY: Raspberry_Rv2
Raspberry_Rv2:
	@ echo "#ifndef __ITEAD_CONFIG_H__" > include/itead_config.h
	@ echo "#define __ITEAD_CONFIG_H__" >> include/itead_config.h
	@ echo "#define BOARD_RASPBERRY_RV2" >> include/itead_config.h
	@ echo "#endif" >> include/itead_config.h
	@ echo "config board Raspberry_Rv2 finished"
