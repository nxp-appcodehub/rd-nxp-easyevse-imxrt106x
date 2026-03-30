/*
 * Copyright 2025 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply
 * with and are bound by, such license terms. If you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#ifndef MESSAGE_BUILDER_METROLOGY_H
#define MESSAGE_BUILDER_METROLOGY_H

#include "metrology_defines.pb.h"
#include "message_handler.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum MetrologyFields
{
    HAS_VOLTAGE         = 1 << 1,
    HAS_CURRENT         = 1 << 2,
    HAS_ACTIVE_POWER    = 1 << 3,
    HAS_APPARENT_POWER  = 1 << 4,
    HAS_REACTIVE_POWER  = 1 << 5,
    HAS_POWER_DIRECTION = 1 << 6,
    HAS_METROLOGY_INDEX = 1 << 7,
    HAS_ALL             = HAS_VOLTAGE | HAS_CURRENT | HAS_ACTIVE_POWER | HAS_APPARENT_POWER | HAS_REACTIVE_POWER |
              HAS_POWER_DIRECTION | HAS_METROLOGY_INDEX,
} MetrologyFields_t;

/**
* @brief Build a human-readable string representation of metrology data
*        using compact notation (VPh1, VPh2, IPh1, IPh2, etc.)
*
* @param metrology Pointer to metrology_schema_t structure
* @param buffer Output buffer for the string
* @param buffer_size Size of the output buffer
* @return int Number of characters written, or -1 on error
*/
int metrology_to_string(const metrology_schema_t *metrology, char *buffer, size_t buffer_size);

/**
 * @brief Copies metrology values that are marked as present in the source structure.
 *
 * This function checks each `has_data_phaseX` flag in @p src and copies the
 * corresponding `data_phaseX` field to @p dst only if the flag is set.
 * No fields are cleared in @p dst; only flagged fields are overwritten.
 *
 * @param[out] dst  Pointer to the destination metrology_values_t structure.
 * @param[in]  src  Pointer to the source metrology_values_t structure.
 *
 * @return MessageHandler_Success on success.
 * @return MessageHandler_InvalidParameter if either @p dst or @p src is NULL.
 */

MessageStatus metrology_values_copy_present(metrology_values_t *dst, const metrology_values_t *src);

/**
 * @brief Creates a metrology_values_t structure with optional phase data.
 *
 * @param values Pointer to the output structure.
 * @param has_phase1 Whether phase1 data is valid.
 * @param phase1 Value for phase1.
 * @param has_phase2 Whether phase2 data is valid.
 * @param phase2 Value for phase2.
 * @param has_phase3 Whether phase3 data is valid.
 * @param phase3 Value for phase3.
 * @return MessageStatus indicating success or error.
 */
MessageStatus create_metrology_values(metrology_values_t *values,
                                      bool has_phase1,
                                      float phase1,
                                      bool has_phase2,
                                      float phase2,
                                      bool has_phase3,
                                      float phase3);

/**
 * @brief Creates a present_voltage_t structure.
 *
 * @param voltage Pointer to the output structure.
 * @param has_data Whether the data field is valid.
 * @param data Pointer to the metrology values.
 * @return MessageStatus indicating success or error.
 */
MessageStatus create_present_voltage(present_voltage_t *voltage, bool has_data, metrology_values_t *data);

/**
 * @brief Creates a present_current_t structure.
 */
MessageStatus create_present_current(present_current_t *current, bool has_data, metrology_values_t *data);

/**
 * @brief Creates a present_active_power_t structure.
 */
MessageStatus create_present_active_power(present_active_power_t *power, bool has_data, metrology_values_t *data);

/**
 * @brief Creates a present_reactive_power_t structure.
 */
MessageStatus create_present_reactive_power(present_reactive_power_t *power, bool has_data, metrology_values_t *data);

/**
 * @brief Creates a present_apparent_power_t structure.
 */
MessageStatus create_present_apparent_power(present_apparent_power_t *power, bool has_data, metrology_values_t *data);

/**
 * @brief Creates a present_power_t structure from its components.
 */
MessageStatus create_present_power(present_power_t *present_power,
                                   bool has_active,
                                   present_active_power_t *active,
                                   bool has_reactive,
                                   present_reactive_power_t *reactive,
                                   bool has_apparent,
                                   present_apparent_power_t *apparent);

/**
 * @brief Populates a metrology_all_t structure with optional components.
 */
MessageStatus create_metrology_all_data(metrology_schema_t *metrology_schema,
                                        bool has_current,
                                        const present_current_t *current,
                                        bool has_voltage,
                                        const present_voltage_t *voltage,
                                        bool has_power,
                                        const present_power_t *power,
                                        bool has_power_direction,
                                        const power_direction_t direction);

/**
 * @brief Creates a full metrology_all message inside an emeter_respond_t.
 *
 * @param emeter_metrology_all Pointer to the emeter_respond_t message to populate.
 * @param has_current Whether current data is present.
 * @param current Pointer to the current structure.
 * @param has_voltage Whether voltage data is present.
 * @param voltage Pointer to the voltage structure.
 * @param has_power Whether power data is present.
 * @param power Pointer to the power structure.
 * @param has_direction Whether power direction is present.
 * @param direction The power direction value.
 * @return MessageStatus indicating success or error.
 */
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
                                           power_direction_t direction);

#ifdef __cplusplus
}
#endif

#endif // MESSAGE_BUILDER_METROLOGY_H
