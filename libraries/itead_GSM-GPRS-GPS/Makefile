
OBJS := call.o
OBJS += gps.o
OBJS += GSM.o
OBJS += inetGSM.o
OBJS += LOG.o
OBJS += SIM900.o
OBJS += sms.o
OBJS += WideTextFinder.o


itead_GSM-GPRS-GPS.o: $(OBJS)
	$(SDK_LINKER) -r -o $@ $^
	rm -rf $^

%.o:%.cpp
	$(SDK_CPP_COMPILE) $(SDK_LIB_OPTIMIZE_OPTION) -fpic -c -o $@ $< -I../../include

