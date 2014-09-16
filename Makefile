#####################################################################
# File		:	Makefile
# Desc		:	Manage all source files in SDK.
# Author	: 	Wu Pengfei
# Company	:	ITEAD Intelligent Systems Co.,Ltd.(http://imall.iteadstudio.com/)
# Date		:	2014/3/11
# History	:	Version		Modified by		Date		What
#               v1.0        Wu Pengfei      2014/3/11   Create
#				v2.0		Wu Pengfei		2014/5/20	Change for extension
#####################################################################

###############################################################################
# Global variables in Makefile tree
###############################################################################
SDK_C_COMPILE 		:= gcc
SDK_CPP_COMPILE 	:= g++
SDK_LINKER 	    	:= ld
SDK_LIB_OPTIMIZE_OPTION	:= -O2

export SDK_C_COMPILE SDK_CPP_COMPILE SDK_LIB_OPTIMIZE_OPTION SDK_LINKER

###############################################################################
# SUBDIRS: directories to go into and make all or clean
###############################################################################
SUBDIRS := lib
SUBDIRS += bootloader
SUBDIRS += libraries

SUBDIRS_CLEAN := $(addsuffix .clean,$(SUBDIRS))


###############################################################################
# Host system path to install SDK components into.
###############################################################################
SYS_USR_LIB 			:= /usr/lib
SYS_USR_LOCAL_LIB 		:= /usr/local/lib
SYS_USR_BIN 			:= /usr/bin
SYS_USR_LOCAL_BIN 		:= /usr/local/bin
SYS_UDEV_RULES			:= /etc/udev/rules.d
SYS_USR_INCLUDE 		:= /usr/include
SYS_USR_LOCAL_INCLUDE 	:= /usr/local/include

###############################################################################
# SDK components needed to be copied into SYS_USR_LIB
###############################################################################
INSTALL_USR_LIB := bootloader/iteadboot.o
INSTALL_USR_LIB += lib/libiteadc.so
INSTALL_USR_LIB += lib/libiteadcpp.so
INSTALL_USR_LIB += libraries/libiteadmodule.so

###############################################################################
# SDK components needed to be copied into SYS_USR_LOCAL_LIB
###############################################################################
INSTALL_USR_LOCAL_LIB := $(INSTALL_USR_LIB)

###############################################################################
# SDK components needed to be copied into SYS_USR_BIN
###############################################################################
INSTALL_USR_BIN := tools/iteadcompile

###############################################################################
# SDK components needed to be copied into SYS_USR_LOCAL_BIN
###############################################################################
INSTALL_USR_LOCAL_BIN := $(INSTALL_USR_BIN)

###############################################################################
# SDK components needed to be copied into SYS_UDEV_RULES
###############################################################################
INSTALL_UDEV_RULES := tools/final-itead.rules


###############################################################################
# headers of core lib
###############################################################################
HEADERS_include := itead.h
HEADERS_include += itead_config.h
HEADERS_include += itead_debug.h
HEADERS_include += itead_delay.h
HEADERS_include += itead_global.h
HEADERS_include += itead_gpio.h
HEADERS_include += itead_gpio_pin_map.h
HEADERS_include += itead_print.h
HEADERS_include += itead_serial.h
HEADERS_include += itead_spi.h
HEADERS_include += itead_utility.h
HEADERS_include += itead_wire.h
HEADERS_include += itead_interrupt.h

HEADERS_include := $(addprefix include/,$(HEADERS_include))


###############################################################################
# headers of libraries for function shields
###############################################################################

HEADERS_itead_GSM-GPRS-GPS 	:= call.h gps.h GSM.h inetGSM.h LOG.h SIM900.h sms.h Streaming.h WideTextFinder.h
HEADERS_itead_GSM-GPRS-GPS	:= $(addprefix libraries/itead_GSM-GPRS-GPS/,$(HEADERS_itead_GSM-GPRS-GPS))

HEADERS_itead_GFX 			:= itead_GFX.h lcd_font.h
HEADERS_itead_GFX			:= $(addprefix libraries/itead_GFX/,$(HEADERS_itead_GFX))

HEADERS_itead_LiquidCrystal := itead_LiquidCrystal.h
HEADERS_itead_LiquidCrystal	:= $(addprefix libraries/itead_LiquidCrystal/,$(HEADERS_itead_LiquidCrystal))

HEADERS_itead_SSD1306 		:= itead_SSD1306.h
HEADERS_itead_SSD1306		:= $(addprefix libraries/itead_SSD1306/,$(HEADERS_itead_SSD1306))

HEADERS_itead_TinyGPS 		:= itead_TinyGPS.h
HEADERS_itead_TinyGPS		:= $(addprefix libraries/itead_TinyGPS/,$(HEADERS_itead_TinyGPS))

HEADERS_itead_PN532_SPI 	:= PN532.h
HEADERS_itead_PN532_SPI		:= $(addprefix libraries/itead_PN532_SPI/,$(HEADERS_itead_PN532_SPI))

HEADERS_itead_nRF24L01-lite 		:= nRF24L01P.h
HEADERS_itead_nRF24L01-lite		:= $(addprefix libraries/itead_nRF24L01-lite/,$(HEADERS_itead_nRF24L01-lite))

HEADERS_itead_NRF24L01P		:= NRF24L01P.h
HEADERS_itead_NRF24L01P		:= $(addprefix libraries/itead_NRF24L01P/,$(HEADERS_itead_NRF24L01P))

HEADERS_itead_SHT1x		:= SHT1x.h
HEADERS_itead_SHT1x		:= $(addprefix libraries/itead_SHT1x/,$(HEADERS_itead_SHT1x))

#
# include config.mk for optional libraries.
#
ifeq (config.mk,$(wildcard config.mk))
include config.mk

#
# itead_SSD1306 depends on itead_GFX
#
ifeq ($(INSTALL_LIBRARIES_itead_SSD1306),y)
INSTALL_LIBRARIES_itead_GFX 			:= y
endif

#
# export for other programs.
#
export INSTALL_LIBRARIES_itead_GSM-GPRS-GPS
export INSTALL_LIBRARIES_itead_LiquidCrystal
export INSTALL_LIBRARIES_itead_SSD1306
export INSTALL_LIBRARIES_itead_GFX
export INSTALL_LIBRARIES_itead_TinyGPS
export INSTALL_LIBRARIES_itead_PN532_SPI
export INSTALL_LIBRARIES_itead_nRF24L01-lite
export INSTALL_LIBRARIES_itead_NRF24L01P
export INSTALL_LIBRARIES_itead_SHT1x

endif  # ifeq (config.mk,$(wildcard config.mk))



HEADERS_libraries :=

ifeq ($(INSTALL_LIBRARIES_itead_GSM-GPRS-GPS),y)
HEADERS_libraries += $(HEADERS_itead_GSM-GPRS-GPS)
endif

ifeq ($(INSTALL_LIBRARIES_itead_LiquidCrystal),y)
HEADERS_libraries += $(HEADERS_itead_LiquidCrystal)
endif

ifeq ($(INSTALL_LIBRARIES_itead_SSD1306),y)
HEADERS_libraries += $(HEADERS_itead_SSD1306)
endif

ifeq ($(INSTALL_LIBRARIES_itead_GFX),y)
HEADERS_libraries += $(HEADERS_itead_GFX)
endif

ifeq ($(INSTALL_LIBRARIES_itead_TinyGPS),y)
HEADERS_libraries += $(HEADERS_itead_TinyGPS)
endif

ifeq ($(INSTALL_LIBRARIES_itead_PN532_SPI),y)
HEADERS_libraries += $(HEADERS_itead_PN532_SPI)
endif

ifeq ($(INSTALL_LIBRARIES_itead_nRF24L01-lite),y)
HEADERS_libraries += $(HEADERS_itead_nRF24L01-lite)
endif

ifeq ($(INSTALL_LIBRARIES_itead_NRF24L01P),y)
HEADERS_libraries += $(HEADERS_itead_NRF24L01P)
endif

ifeq ($(INSTALL_LIBRARIES_itead_SHT1x),y)
HEADERS_libraries += $(HEADERS_itead_SHT1x)
endif


###############################################################################
# SDK components needed to be copied into SYS_USR_INCLUDE
###############################################################################
INSTALL_USER_INCLUDE := $(HEADERS_include)
INSTALL_USER_INCLUDE += $(HEADERS_libraries)

###############################################################################
# SDK components needed to be copied into SYS_USR_LOCAL_INCLUDE
###############################################################################
INSTALL_USER_LOCAL_INCLUDE := $(INSTALL_USER_INCLUDE)




###############################################################################
# The first target : all
# command "make" or "make all"
###############################################################################
.PHONY: all
all: $(SUBDIRS)
	@echo "all done"

.PHONY: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@ all

.PHONY: clean
clean: $(SUBDIRS_CLEAN)
	rm -f libraries/*.o
	@echo "clean done"

.PHONY: $(SUBDIRS_CLEAN)
$(SUBDIRS_CLEAN):
	$(MAKE) -C $(subst .clean,,$@) clean

.PHONY: distclean
distclean: clean
	rm -f include/itead_config.h
	rm -f config.mk
	@echo "distclean done"

.PHONY: update
update: uninstall clean all install
	@echo "update done"

.PHONY: install
install:
#	chmod 755 \
#	$(INSTALL_USR_LIB) \
#	$(INSTALL_USR_LOCAL_LIB) \
#	$(INSTALL_USR_BIN) \
#	$(INSTALL_USR_LOCAL_BIN) \
#	$(INSTALL_UDEV_RULES) \
#	$(INSTALL_USER_INCLUDE) \
#	$(INSTALL_USER_LOCAL_INCLUDE)
	
# install into 	SYS_USR_LIB
	cp -f $(INSTALL_USR_LIB) $(SYS_USR_LIB)
	cd $(SYS_USR_LIB) && chmod 755 $(notdir $(INSTALL_USR_LIB))
# install into 	SYS_USR_LOCAL_LIB
	cp -f $(INSTALL_USR_LOCAL_LIB) $(SYS_USR_LOCAL_LIB)
	cd $(SYS_USR_LOCAL_LIB) && chmod 755 $(notdir $(INSTALL_USR_LOCAL_LIB))
# install into 	SYS_USR_BIN
	cp -f $(INSTALL_USR_BIN) $(SYS_USR_BIN)
	cd $(SYS_USR_BIN) && chmod 755 $(notdir $(INSTALL_USR_BIN))
# install into 	SYS_USR_LOCAL_BIN
	cp -f $(INSTALL_USR_LOCAL_BIN) $(SYS_USR_LOCAL_BIN)
	cd $(SYS_USR_LOCAL_BIN) && chmod 755 $(notdir $(INSTALL_USR_LOCAL_BIN))
# install into 	SYS_UDEV_RULES
	cp -f $(INSTALL_UDEV_RULES) $(SYS_UDEV_RULES)
	cd $(SYS_UDEV_RULES) && chmod 755 $(notdir $(INSTALL_UDEV_RULES))
# install into 	SYS_USR_INCLUDE
	cp -f $(INSTALL_USER_INCLUDE) $(SYS_USR_INCLUDE)
	cd $(SYS_USR_INCLUDE) && chmod 755 $(notdir $(INSTALL_USER_INCLUDE))
# install into 	SYS_USR_LOCAL_INCLUDE
	cp -f $(INSTALL_USER_LOCAL_INCLUDE) $(SYS_USR_LOCAL_INCLUDE)
	cd $(SYS_USR_LOCAL_INCLUDE) && chmod 755 $(notdir $(INSTALL_USER_LOCAL_INCLUDE))
	
	@echo "install done"

.PHONY: uninstall
uninstall:
# uninstall from	SYS_USR_LIB
	cd $(SYS_USR_LIB) && rm -f $(notdir $(INSTALL_USR_LIB))
# uninstall from 	SYS_USR_LOCAL_LIB
	cd $(SYS_USR_LOCAL_LIB) && rm -f $(notdir $(INSTALL_USR_LOCAL_LIB))
# uninstall from 	SYS_USR_BIN
	cd $(SYS_USR_BIN) && rm -f $(notdir $(INSTALL_USR_BIN))
# uninstall from 	SYS_USR_LOCAL_BIN
	cd $(SYS_USR_LOCAL_BIN) && rm -f $(notdir $(INSTALL_USR_LOCAL_BIN))
# uninstall from 	SYS_UDEV_RULES
	cd $(SYS_UDEV_RULES) && rm -f $(notdir $(INSTALL_UDEV_RULES))
# uninstall from 	SYS_USR_INCLUDE
	cd $(SYS_USR_INCLUDE) && rm -f $(notdir $(INSTALL_USER_INCLUDE))
# uninstall from 	SYS_USR_LOCAL_INCLUDE
	cd $(SYS_USR_LOCAL_INCLUDE) && rm -f $(notdir $(INSTALL_USER_LOCAL_INCLUDE))
	
	@echo "uninstall done"


###############################################################################
# make BoardName to generate include/itead_config.h 
# to compile with your board supported by ITEAD-SDK.
# BoardName: Iteaduino_Plus,Raspberry_Rv2
###############################################################################
.PHONY: Iteaduino_Plus
Iteaduino_Plus:
	@ echo "#ifndef __ITEAD_CONFIG_H__" > include/itead_config.h
	@ echo "#define __ITEAD_CONFIG_H__" >> include/itead_config.h
	@ echo "#define BOARD_ITEADUINO_PLUS" >> include/itead_config.h
	@ echo "#endif" >> include/itead_config.h
	@ cp tools/config.mk ./
	@ echo "config board Iteaduino_Plus finished"

.PHONY: Raspberry_Rv2
Raspberry_Rv2:
	@ echo "#ifndef __ITEAD_CONFIG_H__" > include/itead_config.h
	@ echo "#define __ITEAD_CONFIG_H__" >> include/itead_config.h
	@ echo "#define BOARD_RASPBERRY_RV2" >> include/itead_config.h
	@ echo "#endif" >> include/itead_config.h
	@ cp tools/config.mk ./
	@ echo "config board Raspberry_Rv2 finished"

.PHONY: Raspberry_Pi_Model_BPlus
Raspberry_Pi_Model_BPlus:
	@ echo "#ifndef __ITEAD_CONFIG_H__" > include/itead_config.h
	@ echo "#define __ITEAD_CONFIG_H__" >> include/itead_config.h
	@ echo "#define BOARD_RASPBERRY_PI_MODEL_BPLUS" >> include/itead_config.h
	@ echo "#endif" >> include/itead_config.h
	@ cp tools/config.mk ./
	@ echo "config board Raspberry_Pi_Model_BPlus finished"


###############################################################################
# Top Level Makefile End !
###############################################################################
