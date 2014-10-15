#include <itead.h>

static inline bool pinEachOther(uint16_t pin1, uint16_t pin2)
{
    pinMode(pin1,OUTPUT);
    pinMode(pin2,INPUT);
    digitalWrite(pin1,HIGH);
    if (digitalRead(pin2) != HIGH)
    {
        printf("ERROR1:pin1=%d, pin2=%d\n",pin1,pin2);
        return false;
    }
    digitalWrite(pin1,LOW);
    if (digitalRead(pin2) != LOW)
    {
        printf("ERROR2:pin1=%d, pin2=%d\n",pin1,pin2);
        return false;
    }

    pinMode(pin2,OUTPUT);
    pinMode(pin1,INPUT);
    digitalWrite(pin2,HIGH);
    if (digitalRead(pin1) != HIGH)
    {
        printf("ERROR3:pin1=%d, pin2=%d\n",pin1,pin2);
        return false;
    }
    digitalWrite(pin2,LOW);
    if (digitalRead(pin1) != LOW)
    {
        printf("ERROR4:pin1=%d, pin2=%d\n",pin1,pin2);
        return false;
    }
    return true;
}

#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))

typedef struct 
{
    uint16_t pin1;
    uint16_t pin2;
} PinPair;

static const PinPair pin_pairs[] = 
{
    {PD26, PD27},
    {PG10, PG11},
    {PB19, PB18},
    {PI11, PI13},
    {PI12, PI10},
    {PB5,   PB6},
    {PB7,   PB8},
    {PB19, PB18},
    {PI15, PI16},
    {PG10, PG11},
    {PI17, PI18},
    {PI19, PI20},
    {PI11, PI13},
    {PI12, PI10},
    {PI14, PI21},
    {PB21, PB20},
    {PG0,   PG1},
    {PG2,   PG3},
    {PG4,   PG5},
    {PG6,   PG7},
    {PG8,   PG9},
    {PD0 ,  PD1},
    {PD2 ,  PD3},
    {PD4 ,  PD5},
    {PD6 ,  PD7},
    {PD8 ,  PD9},
    {PD10, PD11},
    {PD12, PD13},
    {PD14, PD15},
    {PD16, PD17},
    {PD18, PD19},
    {PD20, PD21},
    {PD22, PD23},
    {PD24, PD25},
    {PD26, PD27},
};

static bool test_all_pin()
{
    int i;
    int pin_ok_cnt = 0;
    
    for(i=0; i<ARRAY_SIZE(pin_pairs); i++)
    {
        if(pinEachOther(pin_pairs[i].pin1, pin_pairs[i].pin2))
        {
            pin_ok_cnt++;
        }
    }
    
    return pin_ok_cnt == ARRAY_SIZE(pin_pairs) ? true : false;
}

int main(int argc, char **argv)
{
    if (test_all_pin())
    {
        printf("GPIO_PASS\n");
        return 0;
    }
    else 
    {
        printf("GPIO_ERROR\n");
        return 0;
    }
}
