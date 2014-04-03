
OBJS := iteadboot.o

all: $(OBJS)
	
clean:
	rm -rf $(OBJS)

%.o : %.cpp
	$(SDKCPPCOMPILE) -c -o $@ $<

.PHONY: all clean
