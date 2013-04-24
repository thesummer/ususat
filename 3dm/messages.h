#ifndef MESSAGES_H
#define MESSAGES_H

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

#endif // MESSAGES_H
