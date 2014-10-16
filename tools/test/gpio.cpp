#include <itead.h>

int main(int argc, char **argv)
{
    uint16_t pin;
    char cmd_read;
    char cmd_write;
    uint8_t value;
    
    if(argc != 3 && argc != 4)
    {
return_help:

        printf("Usage:\n");
        printf("  ./gpio r pin\n");
        printf("  ./gpio w pin value\n");
        printf("\n");
        printf("Example:\n");
        printf("  ./gpio r 47\n");
        printf("  ./gpio w 47 1\n");
        printf("  ./gpio w 47 0\n");        
        printf("\n");
        
        return 0;
    }
    
    /* Read Pin */
    if(argc == 3)
    {
        cmd_read = argv[1][0];
        pin = atoi(argv[2]);
        
        if(cmd_read != 'r')
        {
            goto return_help;
        }
        pinMode(pin, INPUT);
        printf("Read: pin%u = %u\n", pin, digitalRead(pin));
        return 0;
    }

    /* Write Pin */
    if(argc == 4)
    {
        cmd_read = argv[1][0];
        pin = atoi(argv[2]);
        value = atoi(argv[3]);
        
        if(cmd_read != 'w')
        {
            goto return_help;
        }
        pinMode(pin, OUTPUT);
        if(digitalWrite(pin, value) == 0)
        {
            printf("Write: pin%u = %u\n", pin, value);
            return 0;
        } 
        else
        {
            return -1;
        }
        
    }
    
}
