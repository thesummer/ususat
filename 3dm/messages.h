#ifndef MESSAGES_H
#define MESSAGES_H

// Command protocol constants

const unsigned int RAW_ACC_ANG                     = 0xC1; /*!< Raw Accelerometer and Angular Rate Sensor Outputs */
const unsigned int ACC_ANG                         = 0xC2; /*!< Acceleration & Angular Rate */
const unsigned int DELTA_ANGLE_VEL                 = 0xC3; /*!< DeltaAngle & DeltaVelocity */
const unsigned int SET_CONTINUOUS_MODE             = 0xC4; /*!< Set Continuous Mode */
const unsigned int ORIENTATION_MATRIX              = 0xC5; /*!< Orientation Matrix */
const unsigned int ORIENTATION_UPDATE_MAT          = 0xC6; /*!< Orientation Update Matrix */
const unsigned int MAG_VEC                         = 0xC7; /*!< Magnetometer Vector */
const unsigned int ACC_ANG_ORIENTATION_MAT         = 0xC8; /*!< Acceleration, Angular Rate & Orientation Matrix */
const unsigned int WRITE_ACC_BIAS_CORRECTION       = 0xC9; /*!< Write Accel Bias Correction */
const unsigned int WRITE_GYRO_BIAS_CORRECTION      = 0xCA; /*!< Write Gyro Bias Correction */
const unsigned int ACC_ANG_MAG_VEC                 = 0xCB; /*!< Acceleration, Angular Rate & Magnetometer Vector */
const unsigned int ACC_ANG_MAG_VEC_ORIENTATION_MAT = 0xCC; /*!< Acceleration, Angular Rate & Magnetometer Vectors & Orientation Matrix */
const unsigned int CAPTURE_GYRO_BIAS               = 0xCD; /*!< Capture Gyro Bias */
const unsigned int EULER_ANGLES                    = 0xCE; /*!< Euler Angles */
const unsigned int EULER_ANGLES_ANG_RATES          = 0xCF; /*!< Euler Angles and Angular Rates */
const unsigned int TRANSFER_TO_NONVOL_MEM          = 0xD0; /*!< Transfer Quantity to Non-Volatile Memory */
const unsigned int TEMPERATURES                    = 0xD1; /*!< Temperatures */
const unsigned int GYRO_STABIL_ACC_ANG_MAG         = 0xD2; /*!< Gyro Stabilized Acceleration, Angular Rate & Magnetometer */
const unsigned int DELTA_ANGLE_VEL_MAG_VEC         = 0xD3; /*!< DeltaAngle & DeltaVelocity & Magnetometer Vectors */
const unsigned int MODE                            = 0xD4; /*!< Mode */
const unsigned int MODE_PRESET                     = 0xD5; /*!< Mode Preset */
const unsigned int CONTINUOUS_PRESET               = 0xD6; /*!< Continuous Preset */
const unsigned int TIMER                           = 0xD7; /*!< Timer */
const unsigned int COMM_SETTINGS                   = 0xD9; /*!< Communications Settings */
const unsigned int STATIONARY_TEST                 = 0xDA; /*!< Stationary Test */
const unsigned int SAMPLING_SETTINGS               = 0xDB; /*!< Sampling Settings */
const unsigned int REALIGN_UP_NORTH                = 0xDD; /*!< Realign Up and North */
const unsigned int QUATERNION                      = 0xDF; /*!< Quaternion */
const unsigned int WRITE_WORD_EEPROM               = 0xE4; /*!< Write Word to EEPROM */
const unsigned int READ_WORD_EEPROM                = 0xE5; /*!< Read Word from EEPROM */
const unsigned int READ_FIRMWARE_VER               = 0xE9; /*!< Read Firmware Version Number */
const unsigned int READ_DEVICE_ID                  = 0xEA; /*!< Read Device ID String */
const unsigned int STOP_CONTINUOUS                 = 0xFA; /*!< Stop Continuous Mode (no reply) */
const unsigned int FIRMWARE_UPDATE                 = 0xFD; /*!< Firmware Update (no reply) */
const unsigned int DEVICE_RESET                    = 0xFE; /*!< Device Reset (no reply) */

#include "../minimu/vector.h"
#include<stdint.h>

class GX3Packet
{
public:
    virtual unsigned int size() = 0;
    static bool calculateChecksum(uint8_t * buffer, unsigned int length)
    {
        uint16_t sum = 0;
        for(unsigned int i = 0; i<length-2; i++)
        {
            sum += buffer[i];
        }
        return (sum == (*(uint16_t) &buffer[length-2]) );
    }

    static inline vector createVector(uint8_t * buffer)
    {
        return vector(*(float*) &buffer[0],
                      *(float*) &buffer[4],
                      *(float*) &buffer[8]);
    }

    static void createMatrix(uint8_t * buffer, matrix& mat)
    {
        mat << *(float*) &buffer[0],  *(float*) &buffer[4],  *(float*) &buffer[8],
               *(float*) &buffer[12], *(float*) &buffer[16], *(float*) &buffer[20],
               *(float*) &buffer[24], *(float*) &buffer[28], *(float*) &buffer[32];
    }
};

class RawAccAng : public GX3Packet
{
public:
    RawAccAng(uint8_t* buffer)
    {
        acc  = createVector(&buffer[1]);
        gyro = createVector(&buffer[13]);

        timer = *(unsigned int*) &buffer[25];
    }

    virtual unsigned int size() {return mSize;}

    vector acc;
    vector gyro;

    unsigned int timer;

private:
    const static int mSize = 31;
};


class AccAngMagOrientationMat : public GX3Packet
{
public:
    AccAngMagOrientationMat(uint8_t* buffer)
    {
        acc  = createVector(&buffer[1]);
        gyro = createVector(&buffer[13]);
        mag  = createVector(&buffer[25]);

        createMatrix(&buffer[37], orientation);
        timer = *(unsigned int*) &buffer[73];
    }

    virtual unsigned int size() {return mSize;}

    vector acc;
    vector gyro;
    vector mag;

    matrix orientation;
    unsigned int timer;

private:
    const static int mSize = 79;
};

class GX3Command
{
public:
    virtual unsigned int size() = 0;

};

class SamplingSettings : public GX3Command
{
public:
    enum FunctionSelector{
        ReturnOnly=0, Change=1,
        ChangeAndSave=2, ChangeWithoutReply=3
    };
    enum DataConditioning{
        CalcOrientation      = 0x01,
        EnableConingSculling = 0x02,
        FloatLittleEndian    = 0x10,
        SuppressNaN          = 0x20,
        FiniteSizeCorrection = 0x40,
        DisableMag           = 0x100,
        DisableMagNorthComp  = 0x400,
        DisableGravComp      = 0x800,
        EnableQuaternion     = 0x1000

    };

    SamplingSettings(FunctionSelector funSel, uint16_t samplingPeriod_ms = 10,
                     uint16_t dataCondFlags = 0x03, uint8_t gyroAccFilter = 15,
                     uint8_t magFilter = 17, uint16_t upCompensation = 10,
                     uint16_t northCompensation = 10, uint8_t magPower = 0)
    {
        uint16_t *temp16;
        mCommand[0] = SAMPLING_SETTINGS;
        mCommand[1] = 0xA8;
        mCommand[2] = 0xB9;
        mCommand[3] = (uint8_t) funSel;
        temp16 = (uint16_t*) &mCommand[4];
        *temp16 = samplingPeriod_ms;
        temp16 = (uint16_t*) &mCommand[6];
        *temp16 = dataCondFlags;
        mCommand[8]  = gyroAccFilter;
        mCommand[9]  = magFilter;
        temp16 = (uint16_t*) &mCommand[10];
        *temp16 = upCompensation;
        temp16 = (uint16_t*) &mCommand[12];
        *temp16 = northCompensation;
        mCommand[14] = magPower;
        mCommand[15] = 0;
        mCommand[16] = 0;
        mCommand[17] = 0;
        mCommand[18] = 0;
        mCommand[19] = 0;

    }

    virtual unsigned int size() {return mSize;}
    bool checkResponse(uint8_t *buffer, unsigned int length)
    {
        if (length != 19) return false;

        if(GX3Packet::calculateChecksum(buffer, length) == false)
            return false;

        if(buffer[0] != SAMPLING_SETTINGS) return false;
        for(int i=1; i<11; i++)
        {
            if(buffer[i] != mCommand[i+3]) return false;
        }

        return true;
    }

    uint8_t mCommand[mSize];

private:
    const static int mSize = 20;
};


#endif // MESSAGES_H
