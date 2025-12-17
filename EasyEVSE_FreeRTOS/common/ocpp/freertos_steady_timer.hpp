/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#ifndef FREERTOS_STEADY_TIMER_HPP_
#define FREERTOS_STEADY_TIMER_HPP_

#include "FreeRTOS.h"
#include "timers.h"
#include <functional>
#include <chrono>

#include "OcppTimepoint.hpp"

//replaces Everest::SteadyTimer

class FreeRTOSSteadyTimer {
public:
	using Callback = std::function<void()>;
    std::chrono::seconds interval_seconds;

	FreeRTOSSteadyTimer() : timerHandle(NULL), timeoutTicks(0) {}

	~FreeRTOSSteadyTimer()
	{
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
			if (timeoutTicks == 0)
			{
				timeoutTicks = 1;
			}
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

	void timeout(uint32_t interval_s)
	{
		// Convert seconds to milliseconds, then to ticks
		uint32_t interval_ms = interval_s * 1000;
		timeoutTicks = pdMS_TO_TICKS(interval_ms);
		if (timeoutTicks == 0)
		{
			timeoutTicks = 1;
		}

		// Stop existing timer if running
		if (timerHandle) {
			xTimerStop(timerHandle, 0);
			xTimerDelete(timerHandle, 0);
			timerHandle = NULL;
		}

		// Create and start new timer
		timerHandle = xTimerCreate(
				"SteadyTimer",      // Timer name
				timeoutTicks,       // Timer period in ticks
				pdFALSE,           // One-shot timer (not auto-reload)
				this,              // Timer ID (pass 'this' pointer)
				timerCallback      // Callback function
		);

		if (timerHandle != NULL)
		{
			BaseType_t result = xTimerStart(timerHandle, 0);
			if (result != pdPASS)
			{
				configPRINTF(("Failed to start timer"));
			}
		}
		else
		{
			configPRINTF(("Failed to create timer"));
		}
	}

	template<typename Rep, typename Period>
	void timeout(const std::function<void()>& callback, const std::chrono::duration<Rep, Period>& interval_s)
	{
		interval_seconds = std::chrono::duration_cast<std::chrono::seconds>(interval_s);

		configPRINTF(("FreeRTOSSteadyTimer: timeout requested, interval is %d seconds", static_cast<uint32_t>(interval_seconds.count())));

		tCallback = callback;

		// Convert seconds to milliseconds, then to ticks
		auto interval_ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(interval_s);
		timeoutTicks = pdMS_TO_TICKS(static_cast<uint32_t>(interval_ms_duration.count()));
		if (timeoutTicks == 0)
		{
			timeoutTicks = 1;
		}

	    // Stop existing timer if running
	    if (timerHandle)
	    {
	    	xTimerStop(timerHandle, 0);
	    	xTimerDelete(timerHandle, 0);
	    	timerHandle = NULL;
	    }

	    // Create and start new timer
	    timerHandle = xTimerCreate(
	    		"SteadyTimer",      // Timer name
				timeoutTicks,       // Timer period in ticks
				pdFALSE,            // One-shot timer (not auto-reload)
				this,               // Timer ID (pass 'this' pointer)
				timerCallback       // Callback function
	    );

	    if (timerHandle != NULL)
	    {
	    	BaseType_t result = xTimerStart(timerHandle, 0);
	    	if (result != pdPASS)
	    	{
	    		configPRINTF(("Failed to start timer"));
	    	}
	    }
	    else
	    {
	    	configPRINTF(("Failed to create timer"));
	    }

	}

	void timeout(const std::function<void()>& callback, uint32_t interval_s)
	{
		configPRINTF(("FreeRTOSSteadyTimer: timeout requested, interval is %d seconds", interval_s));

		tCallback = callback;

	    // Convert seconds to milliseconds, then to ticks
	    uint32_t interval_ms = interval_s * 1000;
	    timeoutTicks = pdMS_TO_TICKS(interval_ms);
		if (timeoutTicks == 0)
		{
			timeoutTicks = 1;
		}

	    // Stop existing timer if running
	    if (timerHandle)
	    {
	    	xTimerStop(timerHandle, 0);
	    	xTimerDelete(timerHandle, 0);
	    	timerHandle = NULL;
	    }

	    // Create and start new timer
	    timerHandle = xTimerCreate(
	    		"SteadyTimer",      // Timer name
				timeoutTicks,       // Timer period in ticks
				pdFALSE,            // One-shot timer (not auto-reload)
				this,               // Timer ID (pass 'this' pointer)
				timerCallback       // Callback function
	    );

	    if (timerHandle != NULL)
	    {
	    	BaseType_t result = xTimerStart(timerHandle, 0);
	    	if (result != pdPASS)
	    	{
	    		configPRINTF(("Failed to start timer"));
	    	}
	    }
	    else
	    {
	    	configPRINTF(("Failed to create timer"));
	    }
	}

	//const std::chrono::time_point<Clock, Duration>& time_point
	void at(const std::function<void()>& callback, uint32_t time_point)
	{
		configPRINTF(("[at] FreeRTOSSteadyTimer: at requested"));
        //TODO;
	}

	void at(const std::function<void()>& callback, OcppTimepoint time_point)
	{
		configPRINTF(("[at] FreeRTOSSteadyTimer: at requested"));
		unsigned long int timestamp_s = time_point.to_time_point_s();

		OcppTimepoint now_time_point;
		unsigned long int now_timestamp_s = now_time_point.to_time_point_s();

		int32_t diff_s = timestamp_s - now_timestamp_s;
		if (diff_s <= 0)
		{
			configPRINTF(("invalid timepoint, will use 0 seconds as timer interval"));
			configPRINTF(("received timepoint is %lu", timestamp_s));
			configPRINTF(("now timepoint is %lu", now_timestamp_s));

			diff_s = 1;
		}

		this->timeout(callback, (uint32_t)diff_s);
	}

	template<typename Rep, typename Period>
	void interval(const std::function<void()>& callback, const std::chrono::duration<Rep, Period>& interval_s)
	{
		tCallback = callback;
		interval_seconds = std::chrono::duration_cast<std::chrono::seconds>(interval_s);

		configPRINTF(("FreeRTOSSteadyTimer: interval requested, interval is %d seconds", static_cast<uint32_t>(interval_seconds.count())));

		// Convert to milliseconds, then to ticks for FreeRTOS timer
		auto interval_ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(interval_s);
		timeoutTicks = pdMS_TO_TICKS(static_cast<uint32_t>(interval_ms_duration.count()));
		if (timeoutTicks == 0)
		{
			timeoutTicks = 1;
		}

		// Stop existing timer if running
		if (timerHandle)
		{
			xTimerStop(timerHandle, 0);
			xTimerDelete(timerHandle, 0);
			timerHandle = NULL;
		}

		// Create and start new timer
		timerHandle = xTimerCreate(
				"SteadyTimer",      // Timer name
				timeoutTicks,       // Timer period in ticks
				pdTRUE,             // auto-reload
				this,               // Timer ID (pass 'this' pointer)
				timerCallback       // Callback function
		);

		if (timerHandle != NULL)
		{
			BaseType_t result = xTimerStart(timerHandle, 0);
			if (result != pdPASS)
			{
				configPRINTF(("Failed to start timer"));
			}
		}
		else
		{
			configPRINTF(("Failed to create timer"));
		}
	}

	template <class Rep, class Period> void interval(const std::chrono::duration<Rep, Period>& interval)
	{
		// Stop existing timer if running
		if (timerHandle)
		{
			xTimerStop(timerHandle, 0);
			xTimerDelete(timerHandle, 0);
			timerHandle = NULL;
		}

		interval_seconds = std::chrono::duration_cast<std::chrono::seconds>(interval);
		// Convert to milliseconds, then to ticks for FreeRTOS timer
		auto interval_ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(interval);
		timeoutTicks = pdMS_TO_TICKS(static_cast<uint32_t>(interval_ms_duration.count()));
		if (timeoutTicks == 0)
		{
			timeoutTicks = 1;
		}

		// Create and start new timer
		timerHandle = xTimerCreate(
				"SteadyTimer",      // Timer name
				timeoutTicks,       // Timer period in ticks
				pdTRUE,             // auto-reload
				this,               // Timer ID (pass 'this' pointer)
				timerCallback       // Callback function
		);

		if (timerHandle != NULL)
		{
			BaseType_t result = xTimerStart(timerHandle, 0);
			if (result != pdPASS)
			{
				configPRINTF(("Failed to start timer"));
			}
		}
		else
		{
			configPRINTF(("Failed to create timer"));
		}
	}

private:
    TimerHandle_t timerHandle;
    TickType_t timeoutTicks;

    Callback tCallback;

    static void timerCallback(TimerHandle_t xTimer)
    {
    	FreeRTOSSteadyTimer* timer = static_cast<FreeRTOSSteadyTimer*>(pvTimerGetTimerID(xTimer));

    	if (timer && timer->tCallback)
    	{
    		configPRINTF(("Timer expired, executing callback"));
    		timer->tCallback();
    	}
    	else
    	{
    		configPRINTF(("Timer expired but no callback set"));
    	}
    }
};

#endif /* FREERTOS_STEADY_TIMER_HPP_ */
