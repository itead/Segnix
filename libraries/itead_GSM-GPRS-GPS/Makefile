
OBJS := call.o
OBJS += gps.o
OBJS += GSM.o
OBJS += inetGSM.o
OBJS += LOG.o
OBJS += SIM900.o
OBJS += sms.o
OBJS += WideTextFinder.o


itead_GSM-GPRS-GPS.o: $(OBJS)
	$(SDKLINKER) -r -o $@ $^
	rm -rf $(OBJS)

%.o:%.cpp
	$(SDKCPPCOMPILE) $(SDK_LIB_OPTIMIZE_OPTION) -fpic -c -o $@ $< -I../../include

