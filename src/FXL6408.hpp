#ifndef FXL6408__H_
#define FXL6408__H_
#include "Arduino.h"
#include "Wire.h"
/*
Based on https://www.mouser.pl/datasheet/2/308/1/FXL6408_D-2314184.pdf

*/
/************** Device I2C Address ************/

#define FXL6408_I2C_ADDRESS         0x43

/**************** Device Register *************/
#define FXL6408_DEVICE_ID_AND_CTRL  0x01
#define FXL6408_IO_DIRECTION        0x03
#define FXL6408_OUTPUT_STATE        0x05
#define FXL6408_OUTOUT_HIGH_Z       0x07
#define FXL6408_INPUT_DEFAULT_STATE 0x09
#define FXL6408_PULL_ENABLE         0x0B
#define FXL6408_PULL_DOWN_PULL_UP   0x0D
#define FXL6408_INPUT_STATUS        0x0F
#define FXL6408_INTERRUPT_MASK      0x11
#define FXL6408_INTERRUPT_STATUS    0x13

/*************** IO DIRECTION ****************/
#define FXL6408_GPIO_INPUT          0
#define FXL6408_GPIO_OUTPUT         1

/***************** IO STATE ******************/
#define FXL6408_GPIO_LOW            0
#define FXL6408_GPIO_HIGH           1

/***************** INTERRUPTS  ****************/

#define FXL6408_GPIO_INTERRUPT_ACTIVE 0
#define FXL6408_GPIO_INTERRUPT_NOT_ACTIVE 1

/*************** GPIO ADDRESSES **************/

#define FXL6408_GPIO_0              0x01
#define FXL6408_GPIO_1              0x02
#define FXL6408_GPIO_2              0x04
#define FXL6408_GPIO_3              0x08
#define FXL6408_GPIO_4              0x10
#define FXL6408_GPIO_5              0x20
#define FXL6408_GPIO_6              0x40
#define FXL6408_GPIO_7              0x80


typedef enum{
    FXL6408_OK,
    FXL6408_NOK
} fxl6408_status_t;

struct FXL6408_GPIO{
public:
    bool mode = FXL6408_GPIO_INPUT;
    bool INTEnable = FXL6408_GPIO_INTERRUPT_NOT_ACTIVE;
    bool state = FXL6408_GPIO_LOW;
    int address;
};
struct FXL6408Settings{
public:

    bool GPIOMode[8];
    bool GPIOInt[8];
    bool GPIOState[8];

    int GPIOAddress[8] = {
        FXL6408_GPIO_0,
        FXL6408_GPIO_1,
        FXL6408_GPIO_2,
        FXL6408_GPIO_3,
        FXL6408_GPIO_4,
        FXL6408_GPIO_5,
        FXL6408_GPIO_6,
        FXL6408_GPIO_7,
    };
};

/// @brief 
class FXL6408{
public:
    FXL6408();
    ~FXL6408();
    // basic
    void init();
    fxl6408_status_t pinWrite(uint8_t pin, bool val);
    bool pinRead(int pin);

    // additional
    void getInterruptStatus(uint8_t *output);
    bool checkIntOnPin(uint8_t *regVal, int pin);
    void reset();

    FXL6408_GPIO gpio0;
    FXL6408_GPIO gpio1;
    FXL6408_GPIO gpio2;
    FXL6408_GPIO gpio3;
    FXL6408_GPIO gpio4;
    FXL6408_GPIO gpio5;
    FXL6408_GPIO gpio6;
    FXL6408_GPIO gpio7;

private:

    void copySettings();
    void confPinModeReg(uint8_t* data, uint8_t pin, int mode);
    void confInterruptMaskReg(uint8_t* data, uint8_t pin, int mode);
    void confInputDefaultState(uint8_t* data, uint8_t pin, int mode);
    void confRegister(uint8_t* data, uint8_t pin, int mode);

    fxl6408_status_t readRegister(uint8_t* outputPointer, uint8_t offset);
    fxl6408_status_t writeRegister(uint8_t offset, uint8_t dataToWrite);

    // only for setup
    FXL6408Settings settings;

};

#endif // FXL6408__H_

