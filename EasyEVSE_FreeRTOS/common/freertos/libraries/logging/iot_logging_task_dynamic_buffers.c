/*
 * FreeRTOS Common V1.1.3
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://aws.amazon.com/freertos
 * http://www.FreeRTOS.org
 */

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* Logging includes. */
#include "iot_logging_task.h"
#include "logging_levels.h"

/* Standard includes. */
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "fsl_debug_console.h"

/* Sanity check all the definitions required by this file are set. */
#ifndef configPRINT_STRING
    #error configPRINT_STRING( x ) must be defined in FreeRTOSConfig.h to use this logging file.  Set configPRINT_STRING( x ) to a function that outputs a string, where X is the string.  For example, #define configPRINT_STRING( x ) MyUARTWriteString( X )
#endif

#ifndef configLOGGING_MAX_MESSAGE_LENGTH
    #error configLOGGING_MAX_MESSAGE_LENGTH must be defined in FreeRTOSConfig.h to use this logging file.  configLOGGING_MAX_MESSAGE_LENGTH sets the size of the buffer into which formatted text is written, so also sets the maximum log message length.
#endif

#ifndef configLOGGING_INCLUDE_TIME_AND_TASK_NAME
    #error configLOGGING_INCLUDE_TIME_AND_TASK_NAME must be defined in FreeRTOSConfig.h to use this logging file.  Set configLOGGING_INCLUDE_TIME_AND_TASK_NAME to 1 to prepend a time stamp, message number and the name of the calling task to each logged message.  Otherwise set to 0.
#endif

/* A block time of 0 just means don't block. */
#define loggingDONT_BLOCK    0

/*-----------------------------------------------------------*/

/*
 * The task that actually performs the print output.  Using a separate task
 * enables the use of slow output, such as as a UART, without the task that is
 * outputting the log message having to wait for the message to be completely
 * written.  Using a separate task also serializes access to the output port.
 *
 * The structure of this task is very simple; it blocks on a queue to wait for
 * a pointer to a string, sending any received strings to a macro that performs
 * the actual output.  The macro is port specific, so implemented outside of
 * this file.  This version uses dynamic memory, so the buffer that contained
 * the log message is freed after it has been output.
 */
static void prvLoggingTask( void * pvParameters );

/*-----------------------------------------------------------*/

/*
 * The queue used to pass pointers to log messages from the task that created
 * the message to the task that will performs the output.
 */
static QueueHandle_t xQueue = NULL;

/*-----------------------------------------------------------*/

/*
 * Core ID.
 */
extern const char *g_coreName;

/*-----------------------------------------------------------*/

BaseType_t xLoggingTaskInitialize( uint16_t usStackSize,
                                   UBaseType_t uxPriority,
                                   UBaseType_t uxQueueLength )
{
    BaseType_t xReturn = pdFAIL;

    /* Ensure the logging task has not been created already. */
    if( xQueue == NULL )
    {
        /* Create the queue used to pass pointers to strings to the logging task. */
        xQueue = xQueueCreate( uxQueueLength, sizeof( char ** ) );

        if( xQueue != NULL )
        {
            if( xTaskCreate( prvLoggingTask, "Logging", usStackSize, NULL, uxPriority, NULL ) == pdPASS )
            {
                xReturn = pdPASS;
            }
            else
            {
                /* Could not create the task, so delete the queue again. */
                vQueueDelete( xQueue );
            }
        }
    }

    return xReturn;
}
/*-----------------------------------------------------------*/

static void prvLoggingTask( void * pvParameters )
{
    /* Disable unused parameter warning. */
    ( void ) pvParameters;

    char * pcReceivedString = NULL;

    for( ; ; )
    {
        /* Block to wait for the next string to print. */
        if( xQueueReceive( xQueue, &pcReceivedString, portMAX_DELAY ) == pdPASS )
        {
            configPRINT_STRING( pcReceivedString );

            vPortFree( ( void * ) pcReceivedString );
        }
    }
}

/*-----------------------------------------------------------*/

static void prvLoggingPrintfCommon( uint8_t usLoggingLevel,
                                    const char * pcFile,
                                    size_t fileLineNo,
                                    const char * pcFormat,
                                    va_list args )
{
    size_t xLength = 0;
    int32_t xLength2 = 0;
    char * pcPrintString = NULL;

    configASSERT( usLoggingLevel <= LOG_DEBUG );
    configASSERT( pcFormat != NULL );
    configASSERT( configLOGGING_MAX_MESSAGE_LENGTH > 0 );

    /* The queue is created by xLoggingTaskInitialize().  Check
     * xLoggingTaskInitialize() has been called. */
    configASSERT( xQueue );

    /* Allocate a buffer to hold the log message. */
    pcPrintString = pvPortMalloc( configLOGGING_MAX_MESSAGE_LENGTH );

    if( pcPrintString != NULL )
    {
        const char *pcLevelString = "";
        size_t ulFormatLen = 0UL;

        /* Choose the string for the log level metadata for the log message. */
        switch( usLoggingLevel )
        {
            case LOG_ERROR:
                pcLevelString = "E";
                break;

            case LOG_WARN:
                pcLevelString = "W";
                break;

            case LOG_INFO:
                pcLevelString = "I";
                break;

            case LOG_DEBUG:
                pcLevelString = "D";
        }

        /* Add the chosen log level information as prefix for the message. */
        /*if( pcLevelString != NULL )
        {
            xLength += snprintf( pcPrintString + xLength, configLOGGING_MAX_MESSAGE_LENGTH - xLength, "[%s] ", pcLevelString );
            configASSERT( xLength > 0 );
        }*/

        /* Add metadata of task name and tick time for a new log message. */
        if( strcmp( pcFormat, "\n" ) != 0 )
        {
            /* Add metadata of task name and tick count if config is enabled. */
            #if ( configLOGGING_INCLUDE_TIME_AND_TASK_NAME == 1 )
                {
                    const char * pcTaskName;
                    const char * pcNoTask = "None";
                    static BaseType_t xMessageNumber = 0;
                    uint32_t time = portTICK_PERIOD_MS * xTaskGetTickCount();

                    /* Add a time stamp and the name of the calling task to the
                     * start of the log. */
                    if( xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED )
                    {
                        pcTaskName = pcTaskGetName( NULL );
                    }
                    else
                    {
                        pcTaskName = pcNoTask;
                    }

                    xLength += snprintf( pcPrintString, configLOGGING_MAX_MESSAGE_LENGTH, "[%s] [%3lu] [%5lu.%3lu] [%20s]  ",
                                         pcLevelString,
                                         ( unsigned long ) xMessageNumber++,
                                         (time / 1000),
										 (time % 1000),
                                         pcTaskName );
                }
            #endif /* if ( configLOGGING_INCLUDE_TIME_AND_TASK_NAME == 1 ) */
        }

        /* If provided, add the source file and line number metadata in the message. */
        if( pcFile != NULL )
        {
            /* If a file path is provided, extract only the file name from the string
             * by looking for '/' or '\' directory seperator. */
            const char * pcFileName = NULL;

            /* Check if file path contains "\" as the directory separator. */
            if( strrchr( pcFile, '\\' ) != NULL )
            {
                pcFileName = strrchr( pcFile, '\\' ) + 1;
            }
            /* Check if file path contains "/" as the directory separator. */
            else if( strrchr( pcFile, '/' ) != NULL )
            {
                pcFileName = strrchr( pcFile, '/' ) + 1;
            }
            else
            {
                /* File path contains only file name. */
                pcFileName = pcFile;
            }

            xLength += snprintf( pcPrintString + xLength, configLOGGING_MAX_MESSAGE_LENGTH - xLength, "[%s:%d] ", pcFileName, fileLineNo );
            configASSERT( xLength > 0 );
        }

        xLength2 = vsnprintf( pcPrintString + xLength, configLOGGING_MAX_MESSAGE_LENGTH - xLength, pcFormat, args );

        if( xLength2 < 0 )
        {
            /* vsnprintf() failed. Restore the terminating NULL
             * character of the first part. Note that the first
             * part of the buffer may be empty if the value of
             * configLOGGING_INCLUDE_TIME_AND_TASK_NAME is not
             * 1 and as a result, the whole buffer may be empty.
             * That's the reason we have a check for xLength > 0
             * before sending the buffer to the logging task.
             */
            xLength2 = 0;
            pcPrintString[ xLength ] = '\0';
        }

        xLength += ( size_t ) xLength2;

        /* Add newline characters if the message does not end with them.*/
        ulFormatLen = strlen( pcFormat );

        if( ( ulFormatLen >= 2 ) && ( strncmp( pcFormat + ulFormatLen, "\r\n", 2 ) != 0 ) )
        {
            xLength += snprintf( pcPrintString + xLength, configLOGGING_MAX_MESSAGE_LENGTH - xLength, "%s", "\r\n" );
            configASSERT( xLength > 0 );
        }

        /* Only send the buffer to the logging task if it is
         * not empty. */
        if( xLength > 0 )
        {
            /* Send the string to the logging task for IO. */
            if( xQueueSend( xQueue, &pcPrintString, loggingDONT_BLOCK ) != pdPASS )
            {
                /* The buffer was not sent so must be freed again. */
                vPortFree( ( void * ) pcPrintString );
            }
        }
        else
        {
            /* The buffer was not sent, so it must be
             * freed. */
            vPortFree( ( void * ) pcPrintString );
        }
    }
}

/*-----------------------------------------------------------*/

void vLoggingPrintfError( const char * pcFormat,
                          ... )
{
    va_list args;

    va_start( args, pcFormat );
    prvLoggingPrintfCommon( LOG_ERROR, NULL, 0, pcFormat, args );

    va_end( args );
}

/*-----------------------------------------------------------*/

void vLoggingPrintfWarn( const char * pcFormat,
                         ... )
{
    va_list args;

    va_start( args, pcFormat );
    prvLoggingPrintfCommon( LOG_WARN, NULL, 0, pcFormat, args );

    va_end( args );
}

/*-----------------------------------------------------------*/

void vLoggingPrintfInfo( const char * pcFormat,
                         ... )
{
    va_list args;

    va_start( args, pcFormat );
    prvLoggingPrintfCommon( LOG_INFO, NULL, 0, pcFormat, args );

    va_end( args );
}

/*-----------------------------------------------------------*/

void vLoggingPrintfDebug( const char * pcFormat,
                          ... )
{
    va_list args;

    va_start( args, pcFormat );
    prvLoggingPrintfCommon( LOG_DEBUG, NULL, 0, pcFormat, args );

    va_end( args );
}

/*-----------------------------------------------------------*/

void vLoggingPrintfWithFileAndLine( const char * pcFile,
                                    size_t fileLineNo,
                                    const char * pcFormat,
                                    ... )
{
    configASSERT( pcFile != NULL );

    va_list args;

    va_start( args, pcFormat );
    prvLoggingPrintfCommon( LOG_NONE, pcFile, fileLineNo, pcFormat, args );

    va_end( args );
}

/*-----------------------------------------------------------*/

/*!
 * \brief Formats a string to be printed and sends it
 * to the print queue.
 *
 * Appends the message number, time (in ticks), and task
 * that called vLoggingPrintf to the beginning of each
 * print statement.
 *
 */
void vLoggingPrintf( const char * pcFormat,
                     ... )
{
    va_list args;

    va_start( args, pcFormat );
    prvLoggingPrintfCommon( LOG_NONE, NULL, 0, pcFormat, args );

    va_end( args );
}

/*-----------------------------------------------------------*/

void vLoggingPrint( const char * pcMessage )
{
    char * pcPrintString = NULL;
    size_t xLength = 0;

    /* The queue is created by xLoggingTaskInitialize().  Check
     * xLoggingTaskInitialize() has been called. */
    configASSERT( xQueue );

    xLength = strlen( pcMessage ) + 1;
    pcPrintString = pvPortMalloc( xLength );

    if( pcPrintString != NULL )
    {
        strncpy( pcPrintString, pcMessage, xLength );

        /* Send the string to the logging task for IO. */
        if( xQueueSend( xQueue, &pcPrintString, loggingDONT_BLOCK ) != pdPASS )
        {
            /* The buffer was not sent so must be freed again. */
            vPortFree( ( void * ) pcPrintString );
        }
    }
}

