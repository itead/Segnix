
OBJS := iteadboot.o

.PHONY: all
all: $(OBJS)
	
.PHONY: clean
clean:
	rm -rf $(OBJS)

%.o : %.cpp
	$(SDK_CPP_COMPILE) -c -o $@ $<

