#ifndef __ITEAD_DOXYGEN_H__
#define __ITEAD_DOXYGEN_H__



/**
 * @defgroup delay Time
 * Details about micros, millis, delay and delayMicroseconds.
 */

/**
 * @defgroup gpio GPIO
 * Details about pinMode, digitalWrite, digitalRead, analogWrite, digitalRead8, and digitalWrite8 etc.
 */

/**
 * @defgroup interrupt Interrupt
 * Details about attachInterrupt, detachInterrupt, interrupt, and noInterrupt etc. 
 * 
 * @warning 
 *  You need to load itead_gpio_int.ko for using GPIO interrupt functions. 
 * 
 * @warning 
 *  For Iteaduino Plus, you can't use Serial2(UART2) or SPI0, when you've loaded itead_gpio_int.ko
 *  to enable GPIO interrupt functions, such as attachInterrupt(), detachInterrupt() 
 *  and so on.
 *
 * @warning 
 *  For ITEAD CORE EVB, you can't use SPI0, when you've loaded itead_gpio_int.ko
 *  to enable GPIO interrupt functions, such as attachInterrupt(), detachInterrupt() 
 *  and so on.
 *
 */

/**
 * @defgroup serial Serial Port
 * How to use objects of Serial (another name is UART)
 */

/**
 * @defgroup i2c I2C Bus
 * How to use objects of I2C (also WIRE)
 */

/**
 * @defgroup spi SPI Bus
 * How to use objects of SPI
 */

/**
 * @defgroup global Global Stuff
 * Global Macro, Datatype, Variable and so forth.
 */

    /**
     * @ingroup global
     * @defgroup global_macro Global Macro
     * Global Macros user can use directly.
     */

    /**
     * @ingroup global
     * @defgroup global_datatype Global Datatype
     * Global datatypes user can use directly.
     */

#endif /* #ifdef __ITEAD_DOXYGEN_H__ */
