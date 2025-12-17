/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef FREERTOS_MUTEX_HPP_
#define FREERTOS_MUTEX_HPP_

#include "FreeRTOS.h"
#include "semphr.h"

class FreeRTOSMutex {
public:
	FreeRTOSMutex()
	{
		xMutex = xSemaphoreCreateMutex();
	}

	~FreeRTOSMutex()
	{
		if (xMutex)
		{
			vSemaphoreDelete(xMutex);
	    }
	}

	void lock()
	{
		xSemaphoreTake(xMutex, portMAX_DELAY);
	}

	void unlock()
	{
		xSemaphoreGive(xMutex);
	}

	bool try_lock()
	{
		return xSemaphoreTake(xMutex, 0) == pdTRUE;
    }

private:
	SemaphoreHandle_t xMutex;
};

class FreeRTOSLockGuard{
public:
	FreeRTOSLockGuard(FreeRTOSMutex& m) : mutex(m)
	{
	    mutex.lock();
	}

	~FreeRTOSLockGuard()
	{
		mutex.unlock();
	}

private:
	FreeRTOSMutex& mutex;
};

class FreeRTOSTimedMutex{
public:
	    FreeRTOSTimedMutex(uint32_t timeout = 100) : timeout(100)
		{
			xMutex = xSemaphoreCreateMutex();
		}

		~FreeRTOSTimedMutex()
		{
			if (xMutex)
			{
				vSemaphoreDelete(xMutex);
		    }
		}

		void lock()
		{
			xSemaphoreTake(xMutex, pdMS_TO_TICKS(timeout));
		}

		void unlock()
		{
			xSemaphoreGive(xMutex);
		}

		bool try_lock()
		{
			return xSemaphoreTake(xMutex, pdMS_TO_TICKS(timeout)) == pdTRUE;
	    }

	private:
		SemaphoreHandle_t xMutex;
		uint32_t timeout;

};
#endif /* FREERTOS_MUTEX_HPP_ */
