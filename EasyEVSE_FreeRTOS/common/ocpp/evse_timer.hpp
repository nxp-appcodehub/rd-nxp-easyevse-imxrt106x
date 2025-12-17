#ifndef EVEREST_TIMER_FREERTOS_HPP
#define EVEREST_TIMER_FREERTOS_HPP

#include <functional>
#include <chrono>

#ifdef __cplusplus
extern "C" {
#endif
#include "FreeRTOS.h"
#include "timers.h"
#include "event_groups.h"
#include "task.h"
#ifdef __cplusplus
}
#endif

#define EVSE_TIMER_TRIGGERED_EVENT   (1 << 0)

class EvseTimer {
public:
	static EventGroupHandle_t xEvseTimerEventGroup;
private:
    TimerHandle_t timer_handle = nullptr;
    std::chrono::milliseconds interval_ms;
    bool timer_started = false;

    static void evse_timer_callback(TimerHandle_t xTimer) {
        if (xEvseTimerEventGroup)
        {
             xEventGroupSetBits(xEvseTimerEventGroup, EVSE_TIMER_TRIGGERED_EVENT);
        }
    }

public:
    EvseTimer(){;}

    ~EvseTimer() {
        stop();
    }

    void start_timer(const std::chrono::seconds interval);

    void stop();
};

#endif // EVEREST_TIMER_FREERTOS_HPP
