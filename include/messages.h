/**
 * @file messages.h
 *
 * File containing classes representing messages of the single byte protocol
 * for the 3DM-GX3-25
 *
 * @author Jan Sommer
 *  Created on: Apr 25, 2013
 *
 */


#ifndef MESSAGES_H
#define MESSAGES_H

#include<stdint.h>
#include<stdexcept>
#include<SerialPort.h>
#include<iostream>
#include<iomanip>

#include "vector.h"

namespace USU
{

// Command protocol constants

const uint8_t RAW_ACC_ANG                     = 0xC1; /*!< Raw Accelerometer and Angular Rate Sensor Outputs */
const uint8_t ACC_ANG                         = 0xC2; /*!< Acceleration & Angular Rate */
const uint8_t DELTA_ANGLE_VEL                 = 0xC3; /*!< DeltaAngle & DeltaVelocity */
const uint8_t SET_CONTINUOUS_MODE             = 0xC4; /*!< Set Continuous Mode */
const uint8_t ORIENTATION_MATRIX              = 0xC5; /*!< Orientation Matrix */
const uint8_t ORIENTATION_UPDATE_MAT          = 0xC6; /*!< Orientation Update Matrix */
const uint8_t MAG_VEC                         = 0xC7; /*!< Magnetometer Vector */
const uint8_t ACC_ANG_ORIENTATION_MAT         = 0xC8; /*!< Acceleration, Angular Rate & Orientation Matrix */
const uint8_t WRITE_ACC_BIAS_CORRECTION       = 0xC9; /*!< Write Accel Bias Correction */
const uint8_t WRITE_GYRO_BIAS_CORRECTION      = 0xCA; /*!< Write Gyro Bias Correction */
const uint8_t ACC_ANG_MAG_VEC                 = 0xCB; /*!< Acceleration, Angular Rate & Magnetometer Vector */
const uint8_t ACC_ANG_MAG_VEC_ORIENTATION_MAT = 0xCC; /*!< Acceleration, Angular Rate & Magnetometer Vectors & Orientation Matrix */
const uint8_t CAPTURE_GYRO_BIAS               = 0xCD; /*!< Capture Gyro Bias */
const uint8_t EULER_ANGLES                    = 0xCE; /*!< Euler Angles */
const uint8_t EULER_ANGLES_ANG_RATES          = 0xCF; /*!< Euler Angles and Angular Rates */
const uint8_t TRANSFER_TO_NONVOL_MEM          = 0xD0; /*!< Transfer Quantity to Non-Volatile Memory */
const uint8_t TEMPERATURES                    = 0xD1; /*!< Temperatures */
const uint8_t GYRO_STABIL_ACC_ANG_MAG         = 0xD2; /*!< Gyro Stabilized Acceleration, Angular Rate & Magnetometer */
const uint8_t DELTA_ANGLE_VEL_MAG_VEC         = 0xD3; /*!< DeltaAngle & DeltaVelocity & Magnetometer Vectors */
const uint8_t MODE                            = 0xD4; /*!< Mode */
const uint8_t MODE_PRESET                     = 0xD5; /*!< Mode Preset */
const uint8_t CONTINUOUS_PRESET               = 0xD6; /*!< Continuous Preset */
const uint8_t TIMER                           = 0xD7; /*!< Timer */
const uint8_t COMM_SETTINGS                   = 0xD9; /*!< Communications Settings */
const uint8_t STATIONARY_TEST                 = 0xDA; /*!< Stationary Test */
const uint8_t SAMPLING_SETTINGS               = 0xDB; /*!< Sampling Settings */
const uint8_t REALIGN_UP_NORTH                = 0xDD; /*!< Realign Up and North */
const uint8_t QUATERNION                      = 0xDF; /*!< Quaternion */
const uint8_t WRITE_WORD_EEPROM               = 0xE4; /*!< Write Word to EEPROM */
const uint8_t READ_WORD_EEPROM                = 0xE5; /*!< Read Word from EEPROM */
const uint8_t READ_FIRMWARE_VER               = 0xE9; /*!< Read Firmware Version Number */
const uint8_t READ_DEVICE_ID                  = 0xEA; /*!< Read Device ID String */
const uint8_t STOP_CONTINUOUS                 = 0xFA; /*!< Stop Continuous Mode (no reply) */
const uint8_t FIRMWARE_UPDATE                 = 0xFD; /*!< Firmware Update (no reply) */
const uint8_t DEVICE_RESET                    = 0xFE; /*!< Device Reset (no reply) */

/*!
\addtogroup 3dm
@{
*/

/*!
 \brief Abstract base class for received packets

 The class provides some useful function available to all derived classes
 such as checksum calculation and creation of vectors and matrizes from
 the received binary data.

*/
class GX3Packet
{
public:
     /*!
      \brief Read the information for the structure from the SerialPort

      \param serialPort serialPort object from libserial
      \return bool true if reading (and checksum) was successful, false otherwise
     */
     virtual bool readFromSerial(SerialPort &serialPort) = 0;

     /*!
      \brief Print the information of the GX3Packet to an ostream object

      Enables convenient data recording of all different GX3Packet classes.
      Uses csv format; every packet is a single line (without std::endl).

      \param os
     */
     virtual void print(std::ostream &os) = 0;

     /*!
      \brief Implementation of the <<-operator for printinf for all GX3Packets

      Calls the print function of the derived class.

      \param os
      \param packet
      \return std::ostream &operator
     */
     std::ostream & operator << (std::ostream & os, const GX3Packet packet)
     {
        packet.print(os);
        return os;
     }

    /*!
     \brief Calculates the checksum of a received byte array

     \param buffer pointer to the byte array
     \param length length of the byte array
     \return bool  true: checksum matches, false: checksum does not match
    */
    static bool calculateChecksum(uint8_t * buffer, unsigned int length)
    {
        uint16_t sum = 0;
        for(unsigned int i = 0; i<length-2; i++)
        {
            sum += buffer[i];
        }
        uint16_t temp = (buffer[length-2] << 8)  + buffer[length-1];
        return (sum == temp );
    }

protected:
    /*!
     \brief Creates a Eigen::Vector3f consisting of 3 floats from 12 sucessive bytes

     NOTE: Make sure that the endianess of the host system and the 3DM match.
           The endianess of the sent floats can be set with the SamplingSettings command.

     \param buffer Pointer to the byte array
     \return vector vector created from the byte array
    */
    static inline vector createVector(uint8_t * buffer)
    {
        return vector(*(float*) &buffer[0],
                      *(float*) &buffer[4],
                      *(float*) &buffer[8]);
    }

    /*!
     \brief Creates an unsigned integer from 4 successive bytes

     \param buffer Pointer to the byte array
     \return unsigned int created unsigned integer
    */
    static inline unsigned int createUInt(uint8_t *buffer)
    {
        return ( (buffer[0]<<24) + (buffer[1]<<16) + (buffer[2]<<8) + buffer[3] );
    }

    /*!
     \brief Creates a Eigen::Matrix3f from byte array

     NOTE: Make sure that the endianess of the host system and the 3DM match.
           The endianess of the sent floats can be set with the SamplingSettings command.

     \param buffer Pointer to the byte array
     \param mat reference to a matrix which will be filled with the data from the byte array
    */
    static void createMatrix(uint8_t * buffer, matrix& mat)
    {
        mat << *(float*) &buffer[0],  *(float*) &buffer[4],  *(float*) &buffer[8],
               *(float*) &buffer[12], *(float*) &buffer[16], *(float*) &buffer[20],
               *(float*) &buffer[24], *(float*) &buffer[28], *(float*) &buffer[32];
    }
};

/*!
 \brief Representation for receiving (raw) acceleration & angular rate packets

 This class can be used with the commands for raw acceleration and angular rates
 and acceleration and angular rate.
 For the latter the units are:
    - acceleration: g
    - angular rate: rad/s
For the units of the raw values see the protocol data sheet.
*/
class RawAccAng : public GX3Packet
{
public:
   /*!
    \brief Creates an empty packet object
    */
    RawAccAng() {}

    bool readFromSerial(SerialPort &serialPort)
    {
        uint8_t buffer[size];
        buffer[0] = serialPort.ReadByte();
        if(buffer[0] != RAW_ACC_ANG && buffer[0] != ACC_ANG) return false;

        serialPort.ReadRaw(&buffer[1], size-1);
        if(GX3Packet::calculateChecksum(buffer, size) == false)
        {
            using namespace std;
            return false;
        }

        acc  = createVector(&buffer[1]);
        gyro = createVector(&buffer[13]);

        timer = createUInt(&buffer[25]);

        return true;
    }

    /*!
     \brief Print the stored information to ostream object

     Format:
        timestamp,accX,accY,accZ,gyroX,gyroY,gyroZ

     \param os
    */
    virtual void print(std::ostream &os)
    {
        os << timer << "," << acc[0]  << "," << acc[1] << ","  << acc[2]
                    << "," << gyro[0] << "," << gyro[1] << "," << gyro[2];
    }

    vector acc; /*!< Vector containing the accelerometer data */
    vector gyro; /*!< Vector containing the gyroscope (angular rate) data */

    unsigned int timer; /*!< The value of the timestamp for the package */

    enum{size = 31}; /*!< Size of the package (enum to avoid complications with static consts) */
};

/*!
 \brief Representation for receiving acceleration, angular rate and magnetometer packets

 This class can be used with the commands which return 3 Vectors.
 The units are:
    - acceleration: g
    - angular rate: rad/s
    - magnetic field: gauß

*/
class AccAngMag : public GX3Packet
{
public:
    /*!
     \brief Creates an empty packet object
     */
    AccAngMag() {}

    bool readFromSerial(SerialPort &serialPort)
    {
        uint8_t buffer[size];
        buffer[0] = serialPort.ReadByte();
        if(buffer[0] != ACC_ANG_MAG_VEC)
        {
            return false; //throw std::runtime_error("Wrong package identifier");
        }

        serialPort.ReadRaw(&buffer[1], size-1);
        if(GX3Packet::calculateChecksum(buffer, size) == false)
        {
            using namespace std;
            return false;
        }

        acc  = createVector(&buffer[1]);
        gyro = createVector(&buffer[13]);
        mag  = createVector(&buffer[25]);

        timer = createUInt(&buffer[37]);

        return true;
    }

    /*!
     \brief Print the stored information to ostream object

     Format:
        timestamp,accX,accY,accZ,magX,magY,magZ,gyroX,gyroY,gyroZ

     \param os
    */
    virtual void print(std::ostream &os)
    {
        os << timer << "," << acc[0]  << "," << acc[1]  << "," << acc[2]
                    << "," << mag[0]  << "," << mag[1]  << "," << mag[2]
                    << "," << gyro[0] << "," << gyro[1] << "," << gyro[2];
    }

    vector acc; /*!< Vector containing the accelerometer data */
    vector gyro; /*!< Vector containing the gyroscope (angular rate) data */
    vector mag; /*!< Vector containing the magnetometer data */

    unsigned int timer; /*!< The value of the timestamp for the package */

    enum{size = 43}; /*!< Size of the package (enum to avoid complications with static consts) */
};

/*!
 \brief Representation for receiving the Quaternion representation from the IMU

 The class will return a Quaternion from the Eigen library

*/
class Quaternion : public GX3Packet
{
public:
    /*!
     \brief Creates an empty packet object
     */
    Quaternion() {}

    bool readFromSerial(SerialPort &serialPort)
    {
        uint8_t buffer[size];
        buffer[0] = serialPort.ReadByte();
        if(buffer[0] != QUATERNION)
        {
            return false; //throw std::runtime_error("Wrong package identifier");
        }
        serialPort.ReadRaw(&buffer[1], size-1);
        if(GX3Packet::calculateChecksum(buffer, size) == false)
        {
            return false;
        }

        quat = quaternion( (float*) &buffer[1]);
        timer = createUInt(&buffer[17]);

        return true;
    }

    /*!
     \brief Print the stored information to ostream object

        quaternion = w + i*x + j*y + k*z

        Format:
            timestamp,w,x,y,z

     \param os
    */
    virtual void print(std::ostream &os)
    {
        os << timer << "," << quat[0]  << "," << quat[1] << "," << quat[2] << "," << quat[3];
    }

    quaternion quat; /*!< Eigen::Quaternionf representing the Orientation of the IMU*/

    unsigned int timer; /*!< The value of the timestamp for the package */

    enum{size = 23}; /*!< Size of the package (enum to avoid complications with static consts) */
};


/*!
 \brief Representation for packets containing the 3 sensor vectors and orientation matrix
  This class can be used with the commands which return 3 Vectors and a 3x3 Matrix.
  The units are:
    - acceleration: g
    - angular rate: rad/s
    - magnetic field: gauß
*/
class AccAngMagOrientationMat : public GX3Packet
{
public:
    /*!
     \brief Creates an empty packet object
     */
    AccAngMagOrientationMat() {}

    bool readFromSerial(SerialPort &serialPort)
    {
        uint8_t buffer[size];
        buffer[0] = serialPort.ReadByte();
        if(buffer[0] != ACC_ANG_MAG_VEC_ORIENTATION_MAT) return false;

        serialPort.ReadRaw(&buffer[1], size-1);
        if(GX3Packet::calculateChecksum(buffer, size) == false) return false;

        acc  = createVector(&buffer[1]);
        gyro = createVector(&buffer[13]);
        mag  = createVector(&buffer[25]);

        createMatrix(&buffer[37], orientation);
        timer = createUInt(&buffer[73]);

        return true;
    }


    /*!
     \brief Print the stored information to ostream object

     Format:
        timestamp,accX,accY,accZ,magX,magY,magZ,gyroX,gyroY,gyroZ,mat(0,[0..2]),mat(1,[0..2]),mat(2,[0..2])

     \param os
    */
    virtual void print(std::ostream &os)
    {
        os << timer << "," << acc[0]  << "," << acc[1]  << "," << acc[2]
                    << "," << mag[0]  << "," << mag[1]  << "," << mag[2]
                    << "," << gyro[0] << "," << gyro[1] << "," << gyro[2]
                    << "," << orientation(0,0) << "," << orientation(0,1) << "," << orientation(0,1)
                    << "," << orientation(1,0) << "," << orientation(1,1) << "," << orientation(1,2)
                    << "," << orientation(2,0) << "," << orientation(2,1) << "," << orientation(2,2);

    }

    vector acc; /*!< Vector containing the accelerometer data */
    vector gyro; /*!< Vector containing the gyroscope (angular rate) data */
    vector mag; /*!< Vector containing the magnetometer data */

    matrix orientation; /*!< 3x3 Matrix containing the orientation */
    unsigned int timer; /*!< The value of the timestamp for the package */

    enum {size = 79}; /*!< Size of the package (enum to avoid complications with static consts) */
};

////////////////////////////////////////////////////////////////////////////////////////////////////////

/*!
 \brief Base class for commands send to the 3DM-GX3-25

 Just an empty base class, so that all commands share the same base class.

 TODO: Implement sendCommand in base class instead of in each class separately?
*/
class GX3Command
{
public:
    virtual bool sendCommand(SerialPort &serialPort) = 0;
    virtual bool checkResponse(uint8_t *buffer) = 0;
};

/*!
 \brief Represents the "Set continuous mode" command
*/
class SetCountinuousMode : public GX3Command
{
public:
   /*!
    \brief Creates the command

    Allocates a buffer for the byte commands.
    Sets the static bytes and fills the settings bytes
    based on the passed parameters.

     \param CommandByte Command code of the command which is to be executed periodically (Default stop continuous mode)
    */
    SetCountinuousMode(uint8_t CommandByte = 0)
    {
        mCommand[0] = SET_CONTINUOUS_MODE;
        mCommand[1] = 0xC1;
        mCommand[2] = 0x29;
        mCommand[3] = CommandByte;
    }

    bool sendCommand(SerialPort &serialPort)
    {
        serialPort.WriteRaw(mCommand, size);
        uint8_t buffer[responseSize];
        buffer[0] = serialPort.ReadByte();
        if(buffer[0] != SET_CONTINUOUS_MODE) return false;

        serialPort.ReadRaw(&buffer[1], responseSize-1);
        return checkResponse(buffer);
    }

    /*!
     \brief Checks if the response to this command has the correct setup

     \param buffer pointer to the byte array containing the response from the 3DM
     \return bool  true if the response is correct, false if it suggests an error
    */
    bool checkResponse(uint8_t *buffer)
    {

        if(buffer[0] != SET_CONTINUOUS_MODE) return false;

        if(GX3Packet::calculateChecksum(buffer, responseSize) == false)
            return false;

        if(buffer[1] != mCommand[3]) return false;

        return true;
    }

    enum {size = 4, responseSize = 8 }; /*!< Size of the command (enum to avoid complications with static consts) */
    uint8_t mCommand[size]; /*!< Buffer which contains the byte array for the command */
};

/*!
 \brief Represents the "Sampling Settings" command
*/
class SamplingSettings : public GX3Command
{

public:
    /*!
     \brief Sets the function Selector

     The function selector has 4 states:
        - ReturnOnly: Does not change the Sampling Settings, only returns the current state
        - Change: Set new Sampling settings, but do not store them in non-volatile memory (will be reset after shutdown)
        - ChangeAndSave: Set new Sampling Settings and store them in non-volatile memory (will be permanent)
        - ChangeWithoutReply: As Change but no response is sent

    */
    enum FunctionSelector{
        ReturnOnly=0, Change=1,
        ChangeAndSave=2, ChangeWithoutReply=3
    };
    /*!
     \brief Flags for the Data conditioning

     Sets the bits for Data conditioning bytes.
     Combine multiple flags using the "or" operator ("|")

    */
    enum DataConditioning{
        FlagCalcOrientation      = 0x01,
        FlagEnableConingSculling = 0x02,
        FlagDefault              = 0x03,
        FlagFloatLittleEndian    = 0x10,
        FlagSuppressNaN          = 0x20,
        FlagFiniteSizeCorrection = 0x40,
        FlagDisableMag           = 0x100,
        FlagDisableMagNorthComp  = 0x400,
        FlagDisableGravComp      = 0x800,
        FlagEnableQuaternion     = 0x1000

    };

    /*!
     \brief Creates the command

     Allocates a buffer for the byte commands.
     Sets the static bytes and fills the settings bytes
     based on the passed parameters.

     \param funSel  Sets the functions selector
     \param samplingPeriod_ms  Sets the sampling period in ms (1 to 1000)
     \param dataCondFlags Sets general behaviour of the 3DM; use DataConditioning-flags
     \param gyroAccFilter Sets the filter value for the gyro and accelerometer
     \param magFilter Sets the filter value for the magnetometer
     \param upCompensation Sets the time for up compensation
     \param northCompensation Sets the time for north compensation
     \param magPower Sets the Power state
    */
    SamplingSettings(FunctionSelector funSel, uint16_t samplingPeriod_ms = 10,
                     uint16_t dataCondFlags = SamplingSettings::FlagDefault,
                     uint8_t gyroAccFilter = 15, uint8_t magFilter = 17,
                     uint16_t upCompensation = 10, uint16_t northCompensation = 10,
                     uint8_t magPower = 0)
    {
        mCommand[0] = SAMPLING_SETTINGS;
        mCommand[1] = 0xA8;
        mCommand[2] = 0xB9;
        mCommand[3] = (uint8_t) funSel;
        mCommand[4] = (samplingPeriod_ms >> 8);
        mCommand[5] = (samplingPeriod_ms & 0x00FF);
        mCommand[6] = (dataCondFlags >> 8);
        mCommand[7] = (dataCondFlags& 0x00FF);
        mCommand[8] = gyroAccFilter;
        mCommand[9] = magFilter;
        mCommand[10]= (upCompensation >> 8);
        mCommand[11]= (upCompensation & 0x00FF);
        mCommand[12]= (northCompensation >> 8);
        mCommand[13]= (northCompensation & 0x00FF);
        mCommand[14]= magPower;
        mCommand[15]= 0;
        mCommand[16]= 0;
        mCommand[17]= 0;
        mCommand[18]= 0;
        mCommand[19]= 0;

    }

    bool sendCommand(SerialPort &serialPort)
    {
        serialPort.WriteRaw(mCommand, size);
        uint8_t buffer[responseSize];
        buffer[0] = serialPort.ReadByte();
        if(buffer[0] != SAMPLING_SETTINGS) return false;

        serialPort.ReadRaw(&buffer[1], responseSize-1);
        return checkResponse(buffer);
    }

    /*!
     \brief Checks if the response to this command has the correct setup

     \param buffer pointer to the byte array containing the response from the 3DM
     \return bool  true if the response is correct, false if it suggests an error
    */
    bool checkResponse(uint8_t *buffer)
    {
        if(GX3Packet::calculateChecksum(buffer, responseSize) == false)
            return false;

        if(buffer[0] != SAMPLING_SETTINGS) return false;

        for(int i=1; i<11; i++)
        {
            if(buffer[i] != mCommand[i+3]) return false;
        }

        return true;
    }

    enum {size = 20, responseSize = 19}; /*!< Size of the command (enum to avoid complications with static consts) */
    uint8_t mCommand[size]; /*!< Buffer which contains the byte array for the command */

};

/*! @}*/

}
#endif // MESSAGES_H
