/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "evse_timer.hpp"

void EvseTimer::start_timer(const std::chrono::seconds interval) {
	if (timer_started)
	{
		return;
	}

	interval_ms = std::chrono::milliseconds(interval.count() * 1000); //std::chrono::duration_cast<std::chrono::milliseconds>(interval);
	if (interval_ms.count() == 0)
	{
		return;
	}

	timer_handle = xTimerCreate("EvseTimer", pdMS_TO_TICKS(interval_ms.count()), pdTRUE, this, evse_timer_callback);
	if (timer_handle) {
		xTimerStart(timer_handle, 0);
		configPRINTF(("EvseTimer created and started"));
	}

	timer_started = true;
}

void EvseTimer::stop() {
	configPRINTF(("EvseTimer stopped"));
	if (timer_handle != nullptr) {
		xTimerStop(timer_handle, 0);
		xTimerDelete(timer_handle, 0);
		timer_handle = nullptr;
		timer_started = false;
	}
}

