#include <itead.h>

int main(int argc, char **argv)
{
	uint8_t rx = 0;
	uint8_t i=0;
	int index = 0;
	
	SPI1.begin();
	SPI1.setDataMode(SPI_MODE0);
	SPI1.setBitOrder(MSBFIRST);
	for(index=0;index<256;index++)
	{
        rx = SPI1.transfer(index);
        printf("Read 0x%x \n", rx);
	}
	SPI1.end();
	
	return 0;
}
