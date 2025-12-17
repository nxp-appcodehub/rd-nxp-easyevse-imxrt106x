/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#ifndef FREERTOS_RECURSIVE_MUTEX_HPP_
#define FREERTOS_RECURSIVE_MUTEX_HPP_


#include "FreeRTOS.h"
#include "semphr.h"


class FreeRTOSRecursiveMutex {
public:
	FreeRTOSRecursiveMutex()
	{
		xMutex = xSemaphoreCreateRecursiveMutex();
	}

	~FreeRTOSRecursiveMutex()
	{
		if (xMutex)
		{
			vSemaphoreDelete(xMutex);
	    }
	}

	void lock()
	{
		xSemaphoreTakeRecursive(xMutex, portMAX_DELAY);
	}

	void unlock()
	{
		xSemaphoreGiveRecursive(xMutex);
	}

	bool try_lock()
	{
		return xSemaphoreTakeRecursive(xMutex, 0) == pdTRUE;
    }

private:
	SemaphoreHandle_t xMutex;
};

class FreeRTOSRecursiveLockGuard {
public:
	FreeRTOSRecursiveLockGuard(FreeRTOSRecursiveMutex& m) : mutex(m)
	{
	    mutex.lock();
	}

    ~FreeRTOSRecursiveLockGuard()
    {
        mutex.unlock();
    }

private:
    FreeRTOSRecursiveMutex& mutex;
};

class FreeRTOSRecursiveUniqueLock {
public:
	FreeRTOSRecursiveUniqueLock(FreeRTOSRecursiveMutex& m, bool lock_later = false) : mutex(m), owns_lock(false)
	{
		if (!lock_later)
		{
			lock();
		}
	}

    ~FreeRTOSRecursiveUniqueLock()
    {
    	if (owns_lock)
    	{
    		unlock();
    	}
    }

    void lock()
    {
    	if (!owns_lock)
    	{
    		mutex.lock();
    		owns_lock = true;
    	}
    }

    void unlock()
    {
    	if (owns_lock)
    	{
    		mutex.unlock();
    		owns_lock = false;
    	}
    }

    bool owns_lock_f() const
	{
    	return owns_lock;
	}

private:
    FreeRTOSRecursiveMutex& mutex;
    bool owns_lock;
};


#endif /* FREERTOS_RECURSIVE_MUTEX_HPP_ */
