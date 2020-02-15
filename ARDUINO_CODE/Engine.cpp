#include "Engine.h"



ENGINE_DATA::ENGINE_DATA() {

}

void ENGINE_DATA::readFrame(can_frame *f) {
    this->engineOn = true;
    if (f->can_id == 0x0218) {
        this->torque_converter_state = f->data[3] & 0b00000111;
    } else if (f->can_id == 0x0418) {
        this->transmission_temp = uint8_t(f->data[3]) - 40;
        this->actualGear = (f->data[4]) & 0b00001111;
        this->targetGear = ((f->data[4]) & 0b11110000) >> 4;
    }
}

const char* ENGINE_DATA::getTransmissionTemp() {
    if (this->engineOn == false) {
        return ENGINE_OFF;
    } else if (this->transmission_temp == 0xFF) {
        return UNKNOWN_VAL;
    } else {
        memset(buffer, 0x00, sizeof(buffer));
        sprintf(buffer, "%d C", transmission_temp);
        return buffer;
    }
}

const char* ENGINE_DATA::getTorqueConverterStatus() {
    if (this->engineOn == false) {
        return ENGINE_OFF;
    }
    switch (this->torque_converter_state)
    {
    case 0x01:
        return TORQUE_C_OPEN;
    case 0x02:
        return TORQUE_C_SLIP;
    case 0x03:
        return TORQUE_C_CLOSED;
    default:
        memset(buffer, 0x00, sizeof(buffer));
        sprintf(buffer, "%d", torque_converter_state);
        return buffer;
    }
}

const char* ENGINE_DATA::getGearing() {
    if (this->engineOn == false) {
        return ENGINE_OFF;
    }
    if (this->actualGear == 0x00 || this->targetGear == 0x00) {
        return GEAR_NEUTRAL;
    } else if (this->targetGear == 11 || this->actualGear == 11) {
        return GEAR_REVERSE;
    } else if (this->targetGear == 12 || this->actualGear == 12) {
        return GEAR_REVERSE_2;
    } else if (this->targetGear == 13 || this->actualGear == 13) {
        return GEAR_PARK;
    } else {
        memset(buffer, 0x00, sizeof(buffer));
        if (targetGear == actualGear) {
            sprintf(buffer, "%d", actualGear);
        } else if (targetGear > actualGear){
            sprintf(buffer, "%d -> %d", actualGear, targetGear);
        } else {
            sprintf(buffer, "%d <- %d", targetGear, actualGear);
        }
        return buffer;
    }
}


