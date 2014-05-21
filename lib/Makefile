
.PHONY: all
all: libiteadc.so libiteadcpp.so
	
.PHONY: libiteadc.so
libiteadc.so: 
	$(MAKE) -C c $@
	mv c/$@ ./

.PHONY: libiteadcpp.so
libiteadcpp.so: 
	$(MAKE) -C cpp $@
	mv cpp/$@ ./

.PHONY: clean
clean:
	rm -rf libiteadc.so libiteadcpp.so
