/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef FREERTOS_SYSTEM_TIMER_HPP_
#define FREERTOS_SYSTEM_TIMER_HPP_

#include "FreeRTOS.h"
#include "timers.h"
#include <functional>

//replaces Everest::SystemTimer
class FreeRTOSSystemTimer {
public:
	using Callback = std::function<void()>;
    uint32_t interval;

	FreeRTOSSystemTimer() : timerHandle(NULL), timeoutTicks(0) {}

	~FreeRTOSSystemTimer(){
		if (timerHandle)
	    {
			xTimerDelete(timerHandle, 0);
	    }
	}

	void async_wait()
	{
		//TODO;
	}

	void expires_after(uint32_t timeout_ms)
	{
		timeoutTicks = pdMS_TO_TICKS(timeout_ms);
		if (timerHandle)
		{
			xTimerChangePeriod(timerHandle, timeoutTicks, 0);
		}
		else
		{
            timerHandle = xTimerCreate("FreeRTOSSTeadyTimer", timeoutTicks, pdFALSE, this, timerCallback);
		}
	}

	void stop()
	{
		if (timerHandle)
		{
			xTimerStop(timerHandle, 0);
		}
	}

	//const std::chrono::duration<Rep, Period>& interval
	void timeout(const std::function<void()>& callback, uint32_t interval)
	{
		//TODO;
	}

	//const std::chrono::time_point<Clock, Duration>& time_point
	void at(const std::function<void()>& callback, uint32_t time_point)
	{
        //TODO;
	}

private:
    TimerHandle_t timerHandle;
    TickType_t timeoutTicks;

    Callback tCallback;

    static void timerCallback(TimerHandle_t xTimer)
    {
    	//TODO;
    }
};


#endif /* FREERTOS_SYSTEM_TIMER_HPP_ */
