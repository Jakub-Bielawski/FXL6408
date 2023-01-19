#include "FXL6408.hpp"

FXL6408::FXL6408(){

}

FXL6408::~FXL6408(){
}

void FXL6408 :: init(){
    copySettings();

    uint8_t IODirection = 0x00;
    uint8_t InterruptMask = 0x00;
    uint8_t InputDefaultState = 0x00;

    for(int i = 0; i < 8; i++){
        confPinModeReg(&IODirection,settings.GPIOAddress[i],settings.GPIOMode[i]);
        confInterruptMaskReg(&InterruptMask,settings.GPIOAddress[i], settings.GPIOInt[i]);
        confInputDefaultState(&InputDefaultState, settings.GPIOAddress[i],settings.GPIOState[i]);
    }
    writeRegister(FXL6408_IO_DIRECTION, IODirection);
    writeRegister(FXL6408_INTERRUPT_MASK, InterruptMask);
    writeRegister(FXL6408_INPUT_DEFAULT_STATE, InputDefaultState);
    writeRegister(FXL6408_OUTOUT_HIGH_Z, 0x00);    // neccesary to controll output IO
    
    uint8_t regVal = 0x00;
    FXL6408 :: readRegister(&regVal, FXL6408_INTERRUPT_MASK);
    Serial.println(regVal,2);
    regVal=0x00;
    FXL6408 :: readRegister(&regVal, FXL6408_INPUT_DEFAULT_STATE);
    Serial.println(regVal,2);
}

fxl6408_status_t FXL6408 :: pinWrite(uint8_t pin, bool val){
    uint8_t regVal = 0x00;
    fxl6408_status_t returnStatus = FXL6408_OK;
    FXL6408 :: readRegister(&regVal, FXL6408_OUTPUT_STATE);

    if (val){ // setting pin HIGH
        regVal |= pin;
    }else{ // setting pin LOW
        regVal &= ~pin;
    }
    FXL6408 :: writeRegister(FXL6408_OUTPUT_STATE, regVal);

    // check if correct
    uint8_t checkRegVal = 0x00; 
    FXL6408 :: readRegister(&checkRegVal, FXL6408_OUTPUT_STATE);


    bool bitCheck = checkRegVal&pin;
    if (bitCheck ^ val){
        returnStatus = FXL6408_NOK;

    }
    return returnStatus;
}

bool FXL6408 :: pinRead(int pin){
    uint8_t regVal;
    FXL6408 :: readRegister(&regVal, FXL6408_INPUT_STATUS);

    return (bool)(regVal & pin);
}

void FXL6408 :: getInterruptStatus( uint8_t *output){
    FXL6408 :: readRegister(output, FXL6408_INTERRUPT_STATUS);
    uint8_t regVal=0x00;
    FXL6408 :: writeRegister(FXL6408_INTERRUPT_STATUS, regVal);
}

bool FXL6408 :: checkIntOnPin(uint8_t *regVal, int pin){
    return (bool)(*regVal & pin);;
}

void FXL6408 :: reset(){
    uint8_t regVal = 0x00;

    FXL6408 :: readRegister(&regVal, FXL6408_DEVICE_ID_AND_CTRL);
    regVal &= ~0x01;
    FXL6408 :: writeRegister(FXL6408_DEVICE_ID_AND_CTRL, regVal);
}

void FXL6408 :: copySettings(){
    settings.GPIOMode[0] = gpio0.mode;
    settings.GPIOMode[1] = gpio1.mode;
    settings.GPIOMode[2] = gpio2.mode;
    settings.GPIOMode[3] = gpio3.mode;
    settings.GPIOMode[4] = gpio4.mode;
    settings.GPIOMode[5] = gpio5.mode;
    settings.GPIOMode[6] = gpio6.mode;
    settings.GPIOMode[7] = gpio7.mode;

    settings.GPIOState[0] = gpio0.state;
    settings.GPIOState[1] = gpio1.state;
    settings.GPIOState[2] = gpio2.state;
    settings.GPIOState[3] = gpio3.state;
    settings.GPIOState[4] = gpio4.state;
    settings.GPIOState[5] = gpio5.state;
    settings.GPIOState[6] = gpio6.state;
    settings.GPIOState[7] = gpio7.state;

    settings.GPIOInt[0] = gpio0.INTEnable;
    settings.GPIOInt[1] = gpio1.INTEnable;
    settings.GPIOInt[2] = gpio2.INTEnable;
    settings.GPIOInt[3] = gpio3.INTEnable;
    settings.GPIOInt[4] = gpio4.INTEnable;
    settings.GPIOInt[5] = gpio5.INTEnable;
    settings.GPIOInt[6] = gpio6.INTEnable;
    settings.GPIOInt[7] = gpio7.INTEnable;

}

void FXL6408 :: confPinModeReg(uint8_t* data, uint8_t pin, int mode){
    confRegister(data,pin,mode);
}

void FXL6408 :: confInterruptMaskReg(uint8_t* data, uint8_t pin, int mode){
    confRegister(data,pin,mode);
}

void FXL6408 :: confInputDefaultState(uint8_t* data, uint8_t pin, int mode){
    confRegister(data,pin,mode);
}

void FXL6408 :: confRegister(uint8_t* data, uint8_t pin, int mode){
    switch (mode){
        case 1:{ // INPUT, HIGH
            *data |= pin;
            break;
        }
        case 0:{ // OUTPUT, LOW
            *data &= ~pin;
            break;
        }
    }
}
fxl6408_status_t FXL6408 :: readRegister(uint8_t* outputPointer, uint8_t offset){
    uint8_t result = 0;
	uint8_t numBytes = 1;
	fxl6408_status_t returnError = FXL6408_OK;
    Wire.beginTransmission(FXL6408_I2C_ADDRESS);
    Wire.write(offset);
    if( Wire.endTransmission() != 0 ){
        returnError = FXL6408_NOK;
    }

    Wire.requestFrom(FXL6408_I2C_ADDRESS, numBytes);
    while ( Wire.available() ){ // slave may send less than requested
        result = Wire.read(); // receive a byte as a proper uint8_t
	}
    *outputPointer = result;
    return returnError;
}

fxl6408_status_t FXL6408 :: writeRegister(uint8_t offset, uint8_t dataToWrite){
	fxl6408_status_t returnError = FXL6408_OK;

    Wire.beginTransmission(FXL6408_I2C_ADDRESS);
    Wire.write(offset);
    Wire.write(dataToWrite);

    if( Wire.endTransmission() != 0 )
    {
        returnError = FXL6408_NOK;
    }

    return returnError;
}
