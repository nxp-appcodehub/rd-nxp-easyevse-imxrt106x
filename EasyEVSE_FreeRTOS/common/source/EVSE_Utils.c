/*
 * Copyright 2023-2026 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply
 * with and are bound by, such license terms. If you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#if (EASYEVSE_EV == 1)
#include <time.h>
#include <stdint.h>
#endif

#include "board.h"
#include "pin_mux.h"

#include "FreeRTOS.h"
#include "semphr.h"

#include "fsl_trng.h"
#include "fsl_lpi2c.h"

#include "EVSE_Utils.h"

LPI2C_Type const *LPI2C_base_pointer[]   = LPI2C_BASE_PTRS;
LPSPI_Type const *LPSPI_base_pointer[]   = LPSPI_BASE_PTRS;
LPUART_Type const *LPUART_base_pointer[] = LPUART_BASE_PTRS;

#define LPI2C_INSTANCES  (sizeof(LPI2C_base_pointer) / sizeof(LPI2C_base_pointer[0]))
#define LPSPI_INSTANCES  (sizeof(LPSPI_base_pointer) / sizeof(LPSPI_base_pointer[0]))
#define LPUART_INSTANCES (sizeof(LPUART_base_pointer) / sizeof(LPUART_base_pointer[0]))

static SemaphoreHandle_t s_lpi2cMutex[LPI2C_INSTANCES];
static SemaphoreHandle_t s_lpspiMutex[LPSPI_INSTANCES];
static SemaphoreHandle_t s_lpuartMutex[LPUART_INSTANCES];

#if (EASYEVSE_EV == 1)
#define SECONDS_IN_DAY 86400
#define SECONDS_IN_HOUR 3600
#define SECONDS_IN_MINUTE 60
/* Timestamp set to 1 January 2026 */
static time_t ev_timestamp = 1767225600UL;
static const int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
#endif

uint32_t EVSE_GetMsSinceBoot()
{
    uint32_t offsetTick = 0;
    uint32_t fromISR    = __get_IPSR();

    if (fromISR)
    {
        offsetTick = xTaskGetTickCountFromISR();
    }
    else
    {
        offsetTick = xTaskGetTickCount();
    }

    return offsetTick * (1000 / configTICK_RATE_HZ);
}

uint32_t EVSE_GetSecondsSinceBoot()
{
    uint32_t offsetTick = 0;
    offsetTick          = EVSE_GetMsSinceBoot();

    return offsetTick / 1000;
}

const char *convertSecToFormat(uint32_t seconds, timestamp_formats_t time_format)
{
    uint32_t hours                                    = 0;
    uint8_t min                                       = 0;
    uint8_t sec                                       = 0;
    static char string_format[MAX_TIME_FORMAT_LENGTH] = {0};

    hours = seconds / HOURS_DIVIDER;
    min   = (seconds % HOURS_DIVIDER) / MINUTES_DIVIDER;
    sec   = seconds - (hours * HOURS_DIVIDER) - (min * MINUTES_DIVIDER);

    if (time_format == HH_MM_SS)
    {
        sprintf(string_format, "%02d:%02d:%02d", hours, min, sec);
    }
    else if (time_format == HH_MM)
    {
        sprintf(string_format, "%02d:%02d", hours, min);
    }
    else
    {
        sprintf(string_format, "%02d:%02d", min, sec);
    }

    return string_format;
}

evse_peripheral_status_t EVSE_LPUART_InitMutex(uint8_t instance)
{
    evse_peripheral_status_t status = EVSE_Peripheral_Success;

    if (instance >= LPUART_INSTANCES)
    {
        configPRINTF(("Error %s LPUART asked for instance %d", __FUNCTION__, instance));
        while (1)
            ;
    }

    if (s_lpuartMutex[instance] == NULL)
    {
        s_lpuartMutex[instance] = xSemaphoreCreateMutex();

        if (s_lpuartMutex[instance] == NULL)
        {
            configPRINTF(("Error %s Failed to create Mutex %d", __FUNCTION__, instance));
            while (1)
                ;
        }
        else
        {
            xSemaphoreGive(s_lpuartMutex[instance]);
        }
    }
    else
    {
        status = EVSE_Peripheral_AlreadyInit;
    }

    return status;
}

evse_peripheral_status_t EVSE_LPUART_GetMutex(uint8_t instance)
{
    evse_peripheral_status_t error = EVSE_Peripheral_Success;

    if (instance >= LPUART_INSTANCES)
    {
        configPRINTF(("Error LPUART asked for instance %d", instance));
        error = EVSE_Peripheral_Fail;
    }
    else if (s_lpuartMutex[instance] == NULL)
    {
        configPRINTF(("Error %s Asked for non initialized mutex lpuart instance %d", __FUNCTION__, instance));
        error = EVSE_Peripheral_Fail;
    }
    else if (xSemaphoreTake(s_lpuartMutex[instance], portMAX_DELAY) != pdTRUE)
    {
        configPRINTF(("Error %s Failed to get mutex lpuart instance ", __FUNCTION__));
        error = EVSE_Peripheral_Fail;
    }

    return error;
}

evse_peripheral_status_t EVSE_LPUART_PostMutex(uint8_t instance)
{
    evse_peripheral_status_t error = EVSE_Peripheral_Success;

    if (instance >= LPUART_INSTANCES)
    {
        configPRINTF(("Error LPUART asked for instance %d", instance));
        error = EVSE_Peripheral_Fail;
    }
    else if (s_lpuartMutex[instance] == NULL)
    {
        configPRINTF(("Error %s Asked for non initialized mutex lpuart instance %d", __FUNCTION__, instance));
        error = EVSE_Peripheral_Fail;
    }
    else if (xSemaphoreGive(s_lpuartMutex[instance]) != pdTRUE)
    {
        configPRINTF(("Error %s Failed to give mutex lpuart instance", __FUNCTION__));
        error = EVSE_Peripheral_Fail;
    }

    return error;
}

uint8_t EVSE_LPUART_GetInstance(LPUART_Type *base)
{
    uint8_t instance = 0;

    for (instance = 0; instance < LPUART_INSTANCES; instance++)
    {
        if (base == LPUART_base_pointer[instance])
        {
            return instance;
        }
    }

    return 0;
}

evse_peripheral_status_t EVSE_LPSPI_InitMutex(uint8_t instance)
{
    evse_peripheral_status_t status = EVSE_Peripheral_Success;

    if (instance >= LPSPI_INSTANCES)
    {
        configPRINTF(("Error %s LPSPI asked for instance %d", __FUNCTION__, instance));
        while (1)
            ;
    }

    if (s_lpspiMutex[instance] == NULL)
    {
        s_lpspiMutex[instance] = xSemaphoreCreateMutex();

        if (s_lpspiMutex[instance] == NULL)
        {
            configPRINTF(("Error %s Failed to create Mutex %d", __FUNCTION__, instance));
            while (1)
                ;
        }
        else
        {
            xSemaphoreGive(s_lpspiMutex[instance]);
        }
    }
    else
    {
        status = EVSE_Peripheral_AlreadyInit;
    }

    return status;
}

evse_peripheral_status_t EVSE_LPSPI_GetMutex(uint8_t instance)
{
    evse_peripheral_status_t error = EVSE_Peripheral_Success;

    if (instance >= LPSPI_INSTANCES)
    {
        configPRINTF(("Error LPSPI asked for instance %d", instance));
        error = EVSE_Peripheral_Fail;
    }
    else if (s_lpspiMutex[instance] == NULL)
    {
        configPRINTF(("Error %s Asked for non initialized mutex lpspi instance %d", __FUNCTION__, instance));
        error = EVSE_Peripheral_Fail;
    }
    else if (xSemaphoreGetMutexHolder(s_lpspiMutex[instance]) == xTaskGetCurrentTaskHandle())
    {
        /* Do nothing */
    }
    else if (xSemaphoreTake(s_lpspiMutex[instance], portMAX_DELAY) != pdTRUE)
    {
        configPRINTF(("Error %s Failed to get mutex lpspi instance ", __FUNCTION__));
        error = EVSE_Peripheral_Fail;
    }

    return error;
}

evse_peripheral_status_t EVSE_LPSPI_PostMutex(uint8_t instance)
{
    evse_peripheral_status_t error = EVSE_Peripheral_Success;

    if (instance >= LPSPI_INSTANCES)
    {
        configPRINTF(("Error LPSPI asked for instance %d", instance));
        error = EVSE_Peripheral_Fail;
    }
    else if (s_lpspiMutex[instance] == NULL)
    {
        configPRINTF(("Error %s Asked for non initialized mutex lpspi instance %d", __FUNCTION__, instance));
        error = EVSE_Peripheral_Fail;
    }
    else if (xSemaphoreGive(s_lpspiMutex[instance]) != pdTRUE)
    {
        configPRINTF(("Error %s Failed to give mutex lpspi instance", __FUNCTION__));
        error = EVSE_Peripheral_Fail;
    }

    return error;
}

evse_peripheral_status_t EVSE_LPI2C_InitMutex(uint8_t instance)
{
    evse_peripheral_status_t status = EVSE_Peripheral_Success;

    if (instance >= LPI2C_INSTANCES)
    {
        configPRINTF((error("Error %s LPI2C asked for instance %d"), __FUNCTION__, instance));
        while (1)
            ;
    }

    if (s_lpi2cMutex[instance] == NULL)
    {
        s_lpi2cMutex[instance] = xSemaphoreCreateMutex();

        if (s_lpi2cMutex[instance] == NULL)
        {
            configPRINTF((error("Error %s Failed to create Mutex %d"), __FUNCTION__, instance));
            while (1)
                ;
        }
        else
        {
            xSemaphoreGive(s_lpi2cMutex[instance]);
        }
    }
    else
    {
        status = EVSE_Peripheral_AlreadyInit;
    }

    return status;
}

void EVSE_LPI2C_GetMutex(uint8_t instance)
{
    uint8_t error = 0;
    if (instance >= LPI2C_INSTANCES)
    {
        configPRINTF((error("Error LPI2C asked for instance %d"), instance));
        error = true;
    }
    else if (s_lpi2cMutex[instance] == NULL)
    {
        configPRINTF((error("Error %s Asked for non initialized mutex lpi2c instance %d"), __FUNCTION__, instance));
        error = true;
    }
    else if (xSemaphoreTake(s_lpi2cMutex[instance], portMAX_DELAY) != pdTRUE)
    {
        configPRINTF((error("Error %s Failed to get mutex lpi2c instance "), __FUNCTION__));
        error = true;
    }

    if (error)
    {
        while (1)
        {
            vTaskDelay(10);
        }
    }
}

void EVSE_LPI2C_PostMutex(uint8_t instance)
{
    uint8_t error = 0;

    if (instance >= LPI2C_INSTANCES)
    {
        configPRINTF((error("Error LPI2C asked for instance %d"), instance));
        error = true;
    }
    else if (s_lpi2cMutex[instance] == NULL)
    {
        configPRINTF((error("Error %s Asked for non initialized mutex lpi2c instance %d"), __FUNCTION__, instance));
        error = true;
    }
    else if (xSemaphoreGive(s_lpi2cMutex[instance]) != pdTRUE)
    {
        configPRINTF((error("Error %s Failed to give mutex lpi2c instance"), __FUNCTION__));
        error = true;
    }

    if (error)
    {
        while (1)
        {
            vTaskDelay(10);
        }
    }
}

/* TODO - Integrate with SE */
uint32_t EVSE_Random()
{
    uint32_t output = 0;
    TRNG_GetRandomData(TRNG, &output, sizeof(uint32_t));
    return output;
}


#if (EASYEVSE_EV == 1)

/* Check if year is a leap year */
static int is_leap_year(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

/* Calculate days since epoch (1970-01-01) */
static int64_t days_since_epoch(int year, int month, int day)
{
    int64_t days = 0;
    int i;

    /* Add days for complete years */
    for (i = 1970; i < year; i++) {
        days += is_leap_year(i) ? 366 : 365;
    }

    /* Add days for complete months in current year */
    for (i = 0; i < month - 1; i++) {
        days += days_in_month[i];
        /* Add leap day if February and leap year */
        if (i == 1 && is_leap_year(year)) {
            days++;
        }
    }

    /* Add remaining days */
    days += day - 1;

    return days;
}

int rfc3339_to_utc_timestamp(const char *rfc3339_str, time_t *timestamp)
{
    int year = 0;
    int month = 0;
    int day = 0;
    int hour = 0;
    int min = 0;
    int sec = 0;
    int millisec = 0;
    char tz_char;
    int tz_hour = 0, tz_min = 0;
    int64_t days, seconds;

    if (!rfc3339_str || !timestamp) {
        return -1;
    }

    /* Parse the date/time components */
    int parsed = sscanf(rfc3339_str, "%d-%d-%dT%d:%d:%d",
                        &year, &month, &day, &hour, &min, &sec);

    if (parsed != 6) {
        return -1;
    }

    /* Validate ranges */
    if (year < 1970 || month < 1 || month > 12 || day < 1 || day > 31 ||
        hour < 0 || hour > 23 || min < 0 || min > 59 || sec < 0 || sec > 60) {
        return -1;
    }

    /* Calculate timestamp */
    days = days_since_epoch(year, month, day);
    seconds = days * 86400LL + hour * 3600LL + min * 60LL + sec;

    *timestamp = (time_t)seconds;

    return 0;
}

int utc_timestamp_to_rfc3339(time_t timestamp, char* rfc3339_buff, size_t len)
{
	// Start from Unix epoch
	int year = 1970;
    int month = 0;
    int day = 0;
    int hours = 0;
    int minutes = 0;
    int seconds = 0;

    if (!rfc3339_buff)
    {
    	return -1;
    }

	unsigned long _day = timestamp / SECONDS_IN_DAY;
	unsigned long seconds_remaining = timestamp % SECONDS_IN_DAY;

	// Calculate year
	while (1) {
		unsigned int days_in_year = 365;
		if (is_leap_year(year))
		{
			days_in_year = 366;
		}

		if (_day >= days_in_year)
		{
			_day -= days_in_year;
			year++;
		} else {
			break;
		}
	}

	// Calculate month
	unsigned int _month = 0;
	while (_day > days_in_month[_month]) {
		_day -= days_in_month[_month];
		if ((month == 1) && (is_leap_year(year)))
		{
			_day -= 1;
		}
		_month++;
	}
	_month++;
	_day = _day + 1;

	day = _day;
	month = _month;
	hours = seconds_remaining / SECONDS_IN_HOUR;
	seconds_remaining %= SECONDS_IN_HOUR;
	minutes = seconds_remaining / SECONDS_IN_MINUTE;
	seconds = seconds_remaining % SECONDS_IN_MINUTE;

	int n = snprintf(rfc3339_buff, len,
					"%04d-%02d-%02dT%02d:%02d:%02dZ",
					year,
					month,
					day,
					hours,
					minutes,
					seconds);

	// Safety check: ensure snprintf didn't overflow
	if (n < 0 || n >= len) {
		configPRINTF(("to_rfc3339: formatting error or overflow, n return code is %d", n));
		return -1;
	}

	return 0;
}

void EV_SetTimestamp(time_t new_timestamp)
{
	if (new_timestamp > 0)
	{
		ev_timestamp = new_timestamp;
	}
}

time_t EV_GetTimestamp(void)
{
	return ev_timestamp;
}

#endif /* (EASYEVSE_EV == 1) */
