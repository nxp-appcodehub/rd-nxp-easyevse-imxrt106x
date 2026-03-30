/*
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "metrology_defines.pb.h"
#include "message_handler.h"
#include "message_builder_metrology.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

/* Helper function to append formatted string safely */
static int safe_append(char *buffer, size_t buffer_size, size_t *offset, const char *format, ...)
{
    if (*offset >= buffer_size - 1) {
        return -1;  // Buffer full
    }

    va_list args;
    va_start(args, format);
    int written = vsnprintf(buffer + *offset, buffer_size - *offset, format, args);
    va_end(args);

    if (written > 0) {
        *offset += written;
    }

    return written;
}

/* Helper function to format metrology_values_t with compact notation */
static void format_metrology_values_compact(char *buffer, size_t buffer_size, size_t *offset,
                                           const metrology_values_t *values,
                                           const char *prefix)
{
    if (values->has_data_phase1) {
        safe_append(buffer, buffer_size, offset, "  %sPh1: %.2f,",
                   prefix, values->data_phase1);
    }

    if (values->has_data_phase2) {
        safe_append(buffer, buffer_size, offset, "  %sPh2: %.2f,",
                   prefix, values->data_phase2);
    }

    if (values->has_data_phase3) {
        safe_append(buffer, buffer_size, offset, "  %sPh3: %.2f,",
                   prefix, values->data_phase3);
    }
}

int metrology_to_string(const metrology_schema_t *metrology, char *buffer, size_t buffer_size)
{
    if (!metrology || !buffer || buffer_size == 0) {
        return -1;
    }

    size_t offset = 0;

    // Header
    // ========== VOLTAGE ==========
    if (metrology->has_voltage && metrology->voltage.has_data) {
        safe_append(buffer, buffer_size, &offset, "Voltage[V]: ");
        format_metrology_values_compact(buffer, buffer_size, &offset,
                                       &metrology->voltage.data, "V");
    }

    // ========== CURRENT ==========
    if (metrology->has_current && metrology->current.has_data) {
        safe_append(buffer, buffer_size, &offset, "\n\rCurrent[A]: ");
        format_metrology_values_compact(buffer, buffer_size, &offset,
                                       &metrology->current.data, "I");
    }

    // ========== POWER ==========
    if (metrology->has_power) {
        // Active Power (W)
        if (metrology->power.has_active_power &&
            metrology->power.active_power.has_data) {
            safe_append(buffer, buffer_size, &offset, "\n\rActivePowers[W]:");
            format_metrology_values_compact(buffer, buffer_size, &offset,
                                           &metrology->power.active_power.data, "P");
        }

        // Reactive Power (VAR)
        if (metrology->power.has_reactive_power &&
            metrology->power.reactive_power.has_data) {
            safe_append(buffer, buffer_size, &offset, "\n\rReactivePower[VAR]:");
            format_metrology_values_compact(buffer, buffer_size, &offset,
                                           &metrology->power.reactive_power.data, "Q");
        }

        // Apparent Power (VA)
        if (metrology->power.has_apparent_power &&
            metrology->power.apparent_power.has_data) {
            safe_append(buffer, buffer_size, &offset, "\n\rApparentPower[VA]:");
            format_metrology_values_compact(buffer, buffer_size, &offset,
                                           &metrology->power.apparent_power.data, "S");
        }
    }

    // ========== POWER DIRECTION ==========
    if (metrology->has_power_direction) {
        safe_append(buffer, buffer_size, &offset, "\n\rDirection:");
        switch (metrology->power_direction) {
            case POWER_DIRECTION_GRID2_VEHICLE:
                safe_append(buffer, buffer_size, &offset, "  G2V");
                break;
            case POWER_DIRECTION_VEHICLE2_GRID:
                safe_append(buffer, buffer_size, &offset, "  V2G");
                break;
            default:
                safe_append(buffer, buffer_size, &offset, "  Unknown (%u)",
                           metrology->power_direction);
                break;
        }
    }

    // ========== ENERGY METER ==========
    if (metrology->has_energy_meter_index) {
        safe_append(buffer, buffer_size, &offset, "\n\rEnergy:");
        safe_append(buffer, buffer_size, &offset, "  Active: %u Wh, ",
                   metrology->energy_meter_index.wh_t);

        safe_append(buffer, buffer_size, &offset, "  Reactive: %u VARh",
                   metrology->energy_meter_index.varh_t);

    }

    safe_append(buffer, buffer_size, &offset, "\n");
    return (int)offset;
}

MessageStatus create_metrology_values(metrology_values_t *values,
                                      bool has_phase1,
                                      float phase1,
                                      bool has_phase2,
                                      float phase2,
                                      bool has_phase3,
                                      float phase3)
{
    if (values == NULL)
    {
        return MessageHandler_InvalidParameter;
    }

    *values = (metrology_values_t)METROLOGY_VALUES_INIT_DEFAULT;

    values->has_data_phase1 = has_phase1;
    if (values->has_data_phase1)
        values->data_phase1 = phase1;

    values->has_data_phase2 = has_phase2;
    if (values->has_data_phase2)
        values->data_phase2 = phase2;

    values->has_data_phase3 = has_phase3;
    if (values->has_data_phase3)
        values->data_phase3 = phase3;

    return MessageHandler_Success;
}

MessageStatus create_present_voltage(present_voltage_t *voltage, bool has_data, metrology_values_t *data)
{
    if ((voltage == NULL) || ((has_data == true) && (data == NULL)))
    {
        return MessageHandler_InvalidParameter;
    }

    voltage->has_data = has_data;
    if (voltage->has_data)
    {
        voltage->data = *data;
    }

    return MessageHandler_Success;
}

MessageStatus create_present_current(present_current_t *current, bool has_data, metrology_values_t *data)
{
    if ((current == NULL) || ((has_data == true) && (data == NULL)))
    {
        return MessageHandler_InvalidParameter;
    }

    //  *current = (present_current_t)PRESENT_CURRENT_INIT_DEFAULT;

    current->has_data = has_data;
    if (current->has_data)
    {
        current->data = *data;
    }

    return MessageHandler_Success;
}

MessageStatus create_present_active_power(present_active_power_t *power, bool has_data, metrology_values_t *data)
{
    if ((power == NULL) || ((has_data == true) && (data == NULL)))
    {
        return MessageHandler_InvalidParameter;
    }

    // *power = (present_active_power_t)PRESENT_ACTIVE_POWER_INIT_DEFAULT;

    power->has_data = has_data;
    if (power->has_data)
        power->data = *data;

    return MessageHandler_Success;
}

MessageStatus create_present_reactive_power(present_reactive_power_t *power, bool has_data, metrology_values_t *data)
{
    if ((power == NULL) || ((has_data == true) && (data == NULL)))
    {
        return MessageHandler_InvalidParameter;
    }

    // *power = (present_reactive_power_t)PRESENT_REACTIVE_POWER_INIT_DEFAULT;
    power->has_data = has_data;
    if (power->has_data)
        power->data = *data;

    return MessageHandler_Success;
}

MessageStatus create_present_apparent_power(present_apparent_power_t *power, bool has_data, metrology_values_t *data)
{
    if ((power == NULL) || ((has_data == true) && (data == NULL)))
    {
        return MessageHandler_InvalidParameter;
    }

    // *power = (present_apparent_power_t)PRESENT_APPARENT_POWER_INIT_DEFAULT;
    power->has_data = has_data;
    if (power->has_data)
        power->data = *data;

    return MessageHandler_Success;
}

MessageStatus create_present_power(present_power_t *present_power,
                                   bool has_active,
                                   present_active_power_t *active,
                                   bool has_reactive,
                                   present_reactive_power_t *reactive,
                                   bool has_apparent,
                                   present_apparent_power_t *apparent)
{
    if (!present_power)
        return MessageHandler_InvalidParameter;
    if (has_active && !active)
        return MessageHandler_InvalidParameter;
    if (has_reactive && !reactive)
        return MessageHandler_InvalidParameter;
    if (has_apparent && !apparent)
        return MessageHandler_InvalidParameter;

    // Assign values safely
    present_power->has_active_power = has_active;
    if (has_active)
    {
        present_power->active_power = *active;
    }

    present_power->has_reactive_power = has_reactive;
    if (has_reactive)
    {
        present_power->reactive_power = *reactive;
    }

    present_power->has_apparent_power = has_apparent;
    if (has_apparent)
    {
        present_power->apparent_power = *apparent;
    }

    return MessageHandler_Success;
}

MessageStatus create_metrology_all_data(metrology_schema_t *metrology_schema,
                                        bool has_current,
                                        const present_current_t *current,
                                        bool has_voltage,
                                        const present_voltage_t *voltage,
                                        bool has_power,
                                        const present_power_t *power,
                                        bool has_power_direction,
                                        const power_direction_t direction)
{
    if (metrology_schema == NULL)
        return MessageHandler_InvalidParameter;
    if (has_current && current == NULL)
        return MessageHandler_InvalidParameter;
    if (has_voltage && voltage == NULL)
        return MessageHandler_InvalidParameter;
    if (has_power && power == NULL)
        return MessageHandler_InvalidParameter;

    metrology_schema->has_current = has_current;
    if (has_current)
        metrology_schema->current = *current;

    metrology_schema->has_voltage = has_voltage;
    if (has_voltage)
        metrology_schema->voltage = *voltage;

    metrology_schema->has_power = has_power;
    if (has_power)
        metrology_schema->power = *power;

    metrology_schema->has_power_direction = has_power_direction;
    if (has_power_direction)
        metrology_schema->power_direction = direction;

    return MessageHandler_Success;
}

MessageStatus create_metrology_all_message(metrology_schema_t *metrology_schema,
                                           bool isRespose,
                                           pb_size_t *which_type,
                                           bool has_current,
                                           const present_current_t *current,
                                           bool has_voltage,
                                           const present_voltage_t *voltage,
                                           bool has_power,
                                           const present_power_t *power,
                                           bool has_direction,
                                           power_direction_t direction)
{
    if ((metrology_schema == NULL || which_type == NULL))
        return MessageHandler_InvalidParameter;

    if (isRespose == true)
    {
        *which_type = RESPOND_METROLOGY_TAG;
    }
    else
    {
        *which_type = REQUEST_METROLOGY_TAG;
    }

    return create_metrology_all_data(metrology_schema, has_current, current, has_voltage, voltage, has_power, power,
                                     has_direction, direction);
}
