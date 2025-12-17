/*
 * Copyright 2025 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply
 * with and are bound by, such license terms. If you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#pragma once

#include <ocpp/common/types.hpp>
#include <chrono>
#include <time.h>

#include "evse_timer.hpp"

using namespace std::chrono_literals;

namespace ocpp {
class ClockAlignedTimer : private EvseTimer {
public:
	using system_time_point = OcppTimepoint;
private:
	static ClockAlignedTimer* instance; // singleton instance
	static bool instance_created;

	TaskHandle_t taskHandle;
	std::chrono::seconds call_interval;
	std::function<void()> callback;

    // Actual task logic
	void taskFunction() {
		EventBits_t timerBits = 0U;
		while (true)
		{
			if (xEvseTimerEventGroup)
			{
				timerBits = xEventGroupWaitBits(
						xEvseTimerEventGroup,            // The event group handle
						EVSE_TIMER_TRIGGERED_EVENT,      // The bit to wait for
						pdTRUE,                          // Clear the bit on exit
						pdFALSE,                         // Wait for any bit (not all)
						portMAX_DELAY                    // Wait forever
				);

				if ((timerBits & EVSE_TIMER_TRIGGERED_EVENT) == EVSE_TIMER_TRIGGERED_EVENT)
				{
					if (this->callback)
					{
						this->callback();
					}
				}
			}
			else
			{
				configPRINTF(("No xEvseTimerEventGroup"));
				vTaskDelay(1000);
			}
		}

    }


    // Static wrapper to call the member function
    static void taskFunctionWrapper(void *pvParameters) {
    	ClockAlignedTimer *instance = static_cast<ClockAlignedTimer*>(pvParameters);
        instance->taskFunction();
    }

	ClockAlignedTimer()
	{
        xTaskCreate(
            taskFunctionWrapper,             // Static wrapper
            "AlignedTimerTask",              // Task name
            2048*2,                          // Stack size
            this,                            // Pass 'this' pointer
            4,                               // Priority
            &taskHandle                      // Store handle
        );
	}

public:
    static ClockAlignedTimer& getInstance() {
        // Disable interrupts for thread safety
        //taskENTER_CRITICAL();
        if (!instance_created) {
            instance = new ClockAlignedTimer();
            instance_created = true;
        }
        //taskEXIT_CRITICAL();
        return *instance;
    }

	system_time_point interval_starting_from(const std::function<void()>& callback,
	                                             const std::chrono::seconds interval,
	                                             system_time_point start_point) {
	        this->callback = callback;
	        return this->interval_starting_from(interval, start_point);
	    }

    system_time_point interval_starting_from(const std::chrono::seconds interval,
                                             system_time_point start_point) {
        this->call_interval = interval;
        this->start_timer(interval);

        return start_point; //just to keep method signature
    }

    // Stop timer from executing its callback
    using EvseTimer::stop;
};
}
