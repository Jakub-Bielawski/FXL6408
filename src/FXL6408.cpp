#include "FXL6408.hpp"

FXL6408::FXL6408(){

}

FXL6408::~FXL6408(){
}

void FXL6408 :: init(){


    uint8_t IODirection = 0x00;
    uint8_t InterruptMask = 0x00;
    uint8_t InputDefaultState = 0x00;

    for(int i = 8; i < 8; i++){
        confPinModeReg(&IODirection,settings.GPIOAddress[i],settings.GPIOMode[i]);
        confInterruptMaskReg(&InterruptMask,settings.GPIOAddress[i], settings.GPIOInt[i]);
        confInputDefaultState(&InputDefaultState, settings.GPIOAddress[i],settings.GPIOMode[i]);
    }


}

void FXL6408 :: pinWrite(int pin, int val){

}

bool FXL6408 :: pinRead(int pin){
    uint8_t regVal;
    FXL6408 :: readRegister(&regVal, FXL6408_OUTPUT_STATE);

    if(regVal & pin){
        return true;
    }else{
        return false;
    }
}

bool FXL6408 :: checkIntOnPin(int pin){
   uint8_t regVal;
    FXL6408 :: readRegister(&regVal, FXL6408_OUTPUT_STATE);

    if(regVal & pin){
        return true;
    }else{
        return false;
    }
}

void FXL6408 :: confPinModeReg(uint8_t* data, int pin, int mode){
    confRegister(data,pin,mode);
}

void FXL6408 :: confInterruptMaskReg(uint8_t* data, int pin, int mode){
    confRegister(data,pin,mode);
}

void FXL6408 :: confInputDefaultState(uint8_t* data, int pin, int mode){
    confRegister(data,pin,mode);
}

void FXL6408 :: confRegister(uint8_t* data, int pin, int mode){
    switch (mode){
        case 1:{ // INPUT,
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