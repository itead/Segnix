
OBJS :=

ifeq ($(INSTALL_LIBRARIES_itead_GSM-GPRS-GPS),y)
OBJS += itead_GSM-GPRS-GPS.o
endif

ifeq ($(INSTALL_LIBRARIES_itead_LiquidCrystal),y)
OBJS += itead_LiquidCrystal.o
endif

ifeq ($(INSTALL_LIBRARIES_itead_SSD1306),y)
OBJS += itead_SSD1306.o
endif

ifeq ($(INSTALL_LIBRARIES_itead_GFX),y)
OBJS += itead_GFX.o
endif

ifeq ($(INSTALL_LIBRARIES_itead_TinyGPS),y)
OBJS += itead_TinyGPS.o
endif

ifeq ($(INSTALL_LIBRARIES_itead_PN532_SPI),y)
OBJS += itead_PN532_SPI.o
endif


.PHONY: all
all: libiteadmodule.so

libiteadmodule.so: $(OBJS)
	$(SDK_CPP_COMPILE) $(SDK_LIB_OPTIMIZE_OPTION) -fpic -shared -o $@ $^

.PHONY: $(OBJS)
$(OBJS):
	$(MAKE) -C $(subst .o,,$@) $@
	mv $(subst .o,,$@)/$@ ./

.PHONY: clean
clean:
	rm -rf  $(OBJS) libiteadmodule.so
