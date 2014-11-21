OBJS := Dev_Light.o
OBJS += Dev_Switch.o
OBJS += Dev_THSensor.o

PYTHON_DIST_DIR := /usr/lib/python2.7/dist-packages

INSTALL_IOTGO_MODULE_PY_CMD := \
	rm -rf $(PYTHON_DIST_DIR)/IoTgo_module.py* \
	&& cp -rf IoTgo_module.py $(PYTHON_DIST_DIR)/

INSTALL_IOTGO_MODULE_PY := $(shell $(INSTALL_IOTGO_MODULE_PY_CMD))

itead_IoTgo.o: $(OBJS) IoTgo.o
	$(SDK_LINKER) -r -o $@ $^
	rm -rf $^

PYTHON27_CFLAGS := $(shell python2.7-config --cflags)
PYTHON27_CFLAGS := $(subst -Wstrict-prototypes,,$(PYTHON27_CFLAGS)) 
PYTHON27_INCLUDES := $(shell python2.7-config --includes)

.PHONY: IoTgo.o
IoTgo.o: IoTgo.cpp IoTgo.h
	$(SDK_CPP_COMPILE) $(SDK_LIB_OPTIMIZE_OPTION) $(PYTHON27_CFLAGS) -fpic -c -o $@ $< -I../../include

%.o:%.cpp
	$(SDK_CPP_COMPILE) $(SDK_LIB_OPTIMIZE_OPTION) -fpic -c -o $@ $< -I../../include $(PYTHON27_INCLUDES)
