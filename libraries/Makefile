# /*
OBJS := itead_LCD4Bit_mod.o
OBJS += itead_GFX.o
OBJS += itead_SSD1306.o
OBJS += itead_TinyGPS.o

all: libiteadmodule.so

libiteadmodule.so: $(OBJS)
	$(SDKCPPCOMPILE) $(SDK_LIB_OPTIMIZE_OPTION) -fpic -shared -o $@ $^

$(OBJS):
	$(MAKE) -C $(subst .o,,$@) $@
	mv $(subst .o,,$@)/$@ ./

clean:
	rm -rf  $(OBJS) libiteadmodule.so

.PHONY: all $(OBJS)