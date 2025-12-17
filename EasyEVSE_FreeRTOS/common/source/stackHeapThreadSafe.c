/*
 * Copyright 2019, 2025 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply
 * with and are bound by, such license terms. If you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"

extern void _pvHeapStart(void); // get the address of the heap start inserted by the linker
extern void _pvHeapLimit(void);  // address heap end
extern void _HeapSize(void);  // make sure to define these symbols in linker command file

int s_heapBytesRemaining = (int)_HeapSize;
int s_totalBytesProvidedBySBRK = 0;

static void MallocFailedHook(void){

//if the device has reached his heap limit, place the code here for repercussion
	while(1);
}

char *sbrk(int WantedMemory) {
    static char *s_pCurrentHeapEnd = (char*)_pvHeapStart;
    vPortEnterCritical(); // Note: safe to use before FreeRTOS scheduler started

    char *previousHeapEnd = s_pCurrentHeapEnd;
    if (s_pCurrentHeapEnd + WantedMemory > ((char*)_pvHeapLimit)) {

        MallocFailedHook();
        return (char *)-1;
    }

    s_pCurrentHeapEnd += WantedMemory;
    s_heapBytesRemaining -= WantedMemory;
    s_totalBytesProvidedBySBRK += WantedMemory;

    vPortExitCritical();
    return (char *) previousHeapEnd;
}

char * _sbrk(int WantedMemory) { return sbrk(WantedMemory); };

// Standard __malloc_lock and __malloc_unlock were empty functions so they did no thread protection for the malloc or free function
void __malloc_lock()
{
    vPortEnterCritical();
};
void __malloc_unlock()
{
    (void)vPortExitCritical();
};

void __env_lock()
{
    vPortEnterCritical();
};
void __env_unlock()
{
    (void)vPortExitCritical();
};
