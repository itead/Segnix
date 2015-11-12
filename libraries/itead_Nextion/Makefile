
OBJS := NexButton.o
OBJS += NexCrop.o
OBJS += NexGauge.o
OBJS += NexHardware.o
OBJS += NexHotspot.o
OBJS += NexObject.o
OBJS += NexPage.o
OBJS += NexPicture.o
OBJS += NexProgressBar.o
OBJS += NexSlider.o
OBJS += NexText.o
OBJS += NexTouch.o
OBJS += NexWaveform.o
OBJS += NexDualStateButton.o
OBJS += NexNumber.o

itead_Nextion.o: $(OBJS)
	$(SDK_LINKER) -r -o $@ $^
	rm -rf $^

%.o:%.cpp
	$(SDK_CPP_COMPILE) $(SDK_LIB_OPTIMIZE_OPTION) -fpic -c -o $@ $< -I../../include





