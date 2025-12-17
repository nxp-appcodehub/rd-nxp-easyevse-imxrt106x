/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef FREERTOS_CONDITION_VARIABLE_HPP_
#define FREERTOS_CONDITION_VARIABLE_HPP_

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include <vector>

class FreeRTOSConditionVariable {
public:
	FreeRTOSConditionVariable() {
        semaphore = xSemaphoreCreateBinary();
        mutex = xSemaphoreCreateMutex();
    }

    ~FreeRTOSConditionVariable() {
        if (semaphore)
        	vSemaphoreDelete(semaphore);

        if (mutex)
        	vSemaphoreDelete(mutex);
    }

    // Wait indefinitely until notified
    void wait() {
        xSemaphoreTake(semaphore, portMAX_DELAY);
    }

    void wait_until(FreeRTOSMutex lock, uint32_t deadline) {
    	//todo
    	return;
    }

    // Wait with timeout (in milliseconds)
    bool wait_for(uint32_t timeout_ms) {
        TickType_t ticks = pdMS_TO_TICKS(timeout_ms);
        return xSemaphoreTake(semaphore, ticks) == pdTRUE;
    }

    // Wait with timeout (in milliseconds)
    bool wait_for(FreeRTOSMutex lock, uint32_t timeout_ms) {
        //todo
    	return false;
    }

    // Notify one waiting task
    void notify_one() {
        xSemaphoreGive(semaphore);
    }

    // Notify all waiting tasks
    void notify_all() {
    	//needs to be modified to handle a list of sempahores for each task
        xSemaphoreGive(semaphore);
    }

    // Lock/unlock for external use (optional)
    void lock() {
        xSemaphoreTake(mutex, portMAX_DELAY);
    }

    void unlock() {
        xSemaphoreGive(mutex);
    }

private:
    SemaphoreHandle_t semaphore;
    SemaphoreHandle_t mutex;
};

class FreeRTOSConditionVariableAny {
public:
    FreeRTOSConditionVariableAny() {
        // No global semaphore needed
    }

    // Register the current task and wait for a notification
    void wait() {
        TaskHandle_t task = xTaskGetCurrentTaskHandle();
        register_waiter(task);
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);  // Wait for notification
    }

    // Wait with timeout (in milliseconds)
    bool wait_for(uint32_t timeout_ms) {
        TaskHandle_t task = xTaskGetCurrentTaskHandle();
        register_waiter(task);
        TickType_t ticks = pdMS_TO_TICKS(timeout_ms);
        return ulTaskNotifyTake(pdTRUE, ticks) > 0;
    }

    // Notify one waiting task
    void notify_one() {
        taskENTER_CRITICAL();
        if (!waiters.empty()) {
            TaskHandle_t task = waiters.back();
            waiters.pop_back();
            xTaskNotifyGive(task);
        }
        taskEXIT_CRITICAL();
    }

    // Notify all waiting tasks
    void notify_all() {
        taskENTER_CRITICAL();
        for (TaskHandle_t task : waiters) {
            xTaskNotifyGive(task);
        }
        waiters.clear();
        taskEXIT_CRITICAL();
    }

private:
    std::vector<TaskHandle_t> waiters;

    void register_waiter(TaskHandle_t task) {
        taskENTER_CRITICAL();
        waiters.push_back(task);
        taskEXIT_CRITICAL();
    }
};

#endif /* FREERTOS_CONDITION_VARIABLE_HPP_ */
