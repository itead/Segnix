
all: libiteadc.so libiteadcpp.so
	
libiteadc.so: 
	$(MAKE) -C c $@
	mv c/$@ ./

libiteadcpp.so: 
	$(MAKE) -C cpp $@
	mv cpp/$@ ./

clean:
	rm -rf libiteadc.so libiteadcpp.so

.PHONY: all clean libiteadc.so libiteadcpp.so