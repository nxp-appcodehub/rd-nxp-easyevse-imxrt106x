/*
 * Copyright 2023-2025 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply
 * with and are bound by, such license terms. If you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#ifndef EVSE_UTILS_H_
#define EVSE_UTILS_H_

#include "fsl_common.h"

#define HOURS_DIVIDER          3600
#define MINUTES_DIVIDER        60
#define MAX_TIME_FORMAT_H      7
#define MAX_TIME_FORMAT_M      2
#define MAX_TIME_FORMAT_S      2
#define MAX_TIME_FORMAT_LENGTH (MAX_TIME_FORMAT_H + MAX_TIME_FORMAT_M + MAX_TIME_FORMAT_S + 3)

/**
 * @brief Used by the Peripheral functions
 *
 */
typedef enum
{
    EVSE_Peripheral_Success,
    EVSE_Peripheral_Fail,
    EVSE_Peripheral_AlreadyInit, /* The interface has already been inited */
} evse_peripheral_status_t;

typedef enum _timestamp_formats
{
    HH_MM_SS,
    HH_MM,
    MM_SS
} timestamp_formats_t;

typedef enum _evse_return_status
{
    SUCCESS,
    FAIL,
    NOT_IMPLEMENTED,
}evse_return_status_t;

/**
 * Get the the number of miliseconds since the scheduler started
 * @return number of miliseconds
 */
uint32_t EVSE_GetMsSinceBoot();

/**
 * Get the the number of seconds since the scheduler started
 * @return number of seconds
 */
uint32_t EVSE_GetSecondsSinceBoot();

/**
 * @brief [Peripheral]Creates a mutex that should be used for LPUART exclusive access
 *
 * @param instance LPUART instance
 * @return evse_peripheral_status_t
    EVSE_Peripheral_Success if the initialisation was successful
    EVSE_Peripheral_Fail if the initialisation failed
    EVSE_Peripheral_AlreadyInit if already initialized
 */
evse_peripheral_status_t EVSE_LPUART_InitMutex(uint8_t instance);

/**
 * @brief [Peripheral]Get the mutex for a specific LPUART instance. EVSE_LPUART_InitMutex should be called before this
 * get.
 *
 * @param instance LPUART instance for which to get the mutex
 * @return evse_peripheral_status_t EVSE_Peripheral_Success/EVSE_Peripheral_Fail
 */
evse_peripheral_status_t EVSE_LPUART_GetMutex(uint8_t instance);

/**
 * @brief [Peripheral]Post the mutex for a specific LPUART instance.
 * EVSE_LPUART_GetMutex should be called before otherwise this will return fail
 *
 * @param instance LPUART instance for which to post the mutex
 * @return evse_peripheral_status_t EVSE_Peripheral_Fail
 */
evse_peripheral_status_t EVSE_LPUART_PostMutex(uint8_t instance);

/**
 * @brief Get the instance number from a base address
 *
 * @param base The LPUART base address
 * @return uint8_t Return an instance number
 */
uint8_t EVSE_LPUART_GetInstance(LPUART_Type *base);

/**
 * @brief [Peripheral]Creates a mutex that should be used for LPSPI exclusive access
 *
 * @param instance SPI LPSPI instance
 * @return evse_peripheral_status_t
    EVSE_Peripheral_Success if the initialisation was successful
    EVSE_Peripheral_Fail if the initialisation failed
    EVSE_Peripheral_AlreadyInit if already initialized
 */
evse_peripheral_status_t EVSE_LPSPI_InitMutex(uint8_t instance);

/**
 * @brief [Peripheral]Get the mutex for a specific LPSPI instance. EVSE_LPSPI_InitMutex should be called before this
 * get.
 *
 * @param instance LPSPI instance for which to get the mutex
 * @return evse_peripheral_status_t EVSE_Peripheral_Success/EVSE_Peripheral_Fail
 */
evse_peripheral_status_t EVSE_LPSPI_GetMutex(uint8_t instance);

/**
 * @brief [Peripheral]Post the mutex for a specific LPSPI instance.
 * EVSE_LPSPI_GetMutex should be called before otherwise this will return fail
 *
 * @param instance LPSPI instance for which to post the mutex
 * @return evse_peripheral_status_t EVSE_Peripheral_Fail
 */
evse_peripheral_status_t EVSE_LPSPI_PostMutex(uint8_t instance);

/**
 * Creates a mutex that should be used for LPI2C exclusive access
 * @param instance LPI2C instance
 */
evse_peripheral_status_t EVSE_LPI2C_InitMutex(uint8_t instance);

/**
 * Get the mutex. EVSE_LPI2C_InitMutex should be called before this get.
 * @param instance LPI2C instance
 */
void EVSE_LPI2C_GetMutex(uint8_t instance);

/**
 * Post the mutex
 * @param instance LPI2C instance
 */
void EVSE_LPI2C_PostMutex(uint8_t instance);

/**
 * Return a random generated number
 * @return
 */
uint32_t EVSE_Random();

/**
 * Return a string that displays time (given in seconds) in a specified format
 * @param seconds     period of time that we want to convert (given in seconds)
 * @param time_format the desired time format, chosen from timestamp_formats_t enum
 */
const char *convertSecToFormat(uint32_t seconds, timestamp_formats_t time_format);

#endif /* EVSE_UTILS_H_ */
