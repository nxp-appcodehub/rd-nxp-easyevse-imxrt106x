/*
 * Copyright 2020, 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>

#include "FreeRTOS.h"
#include "task.h"

/**
 * @brief Warn if stack overflow is detected.
 *
 * If configCHECK_FOR_STACK_OVERFLOW is set to 1,
 * this hook provides a location for applications to
 * define a response to a stack overflow.
 *
 * Use this hook to help identify that a stack overflow
 * has occurred.
 *
 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    (void)pcTaskName;
    (void)xTask;

    for (;;)
        ;
}

/**
 * @brief Warn user if pvPortMalloc fails.
 *
 * Called if a call to pvPortMalloc() fails because there is insufficient
 * free memory available in the FreeRTOS heap.  pvPortMalloc() is called
 * internally by FreeRTOS API functions that create tasks, queues, software
 * timers, and semaphores.  The size of the FreeRTOS heap is set by the
 * configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h.
 *
 */
void vApplicationMallocFailedHook()
{
    for (;;)
        ;
}

/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
 * implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
 * used by the Idle task. */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize)
{
    /* If the buffers to be provided to the Idle task are declared inside this
     * function then they must be declared static - otherwise they will be allocated on
     * the stack and so not exists after this function exits. */
    static StaticTask_t xIdleTaskTCB;
    __attribute__((section(
        ".ocram_non_cacheable_data ,\"aw\", %nobits @"))) static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle
     * task's state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
 * application must provide an implementation of vApplicationGetTimerTaskMemory()
 * to provide the memory that is used by the Timer service task. */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize)
{
    /* If the buffers to be provided to the Timer task are declared inside this
     * function then they must be declared static - otherwise they will be allocated on
     * the stack and so not exists after this function exits. */
    static StaticTask_t xTimerTaskTCB;
    __attribute__((section(".ocram_non_cacheable_data ,\"aw\", %nobits @"))) static StackType_t
        uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH];

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
     * task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configTIMER_TASK_STACK_DEPTH is specified in words, not bytes. */
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

#if defined __has_include
#if __has_include("wm_os.h")
#include "wm_os.h"
#define WIFI_FILE_FOUND 1
#endif /* __has_include ("wm_os.h") */
#endif /* defined __has_include */

extern void vApplicationTickHookSE(void);
extern void vApplicationTickHookUI(void);
extern void TrgtHW_TimerISR(void);

/* If the wifi files are present, use wifi abstraction layer*/
#if (WIFI_FILE_FOUND == 0U)

void vApplicationIdleHook(void)
{

}

/*!
 * @brief FreeRTOS tick hook.
 */
void vApplicationTickHook(void)
{
#if (ENABLE_LCD)
    vApplicationTickHookUI();
#endif /* ENABLE_LCD */

#if (ENABLE_SE)
    vApplicationTickHookSE();
#endif /* ENABLE_SE */

#if (ENABLE_ISO15118 == 1)
    TrgtHW_TimerISR();
#endif /* ENABLE_ISO15118 */
}

#endif /* ENABLE_WIFI */

void vApplicationTickHookRegister(void)
{
#if (WIFI_FILE_FOUND == 1U)

#if (ENABLE_LCD)
    os_setup_tick_function(vApplicationTickHookUI);
#endif /* ENABLE_LCD */

#if (ENABLE_SE)
    os_setup_tick_function(vApplicationTickHookSE);
#endif /* ENABLE_SE */

#if (ENABLE_ISO15118 == 1)
    os_setup_tick_function(TrgtHW_TimerISR);
#endif

#endif /* ENABLE_WIFI */
}
