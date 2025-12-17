// SPDX-License-Identifier: Apache-2.0
// Copyright Pionix GmbH and Contributors to EVerest
// Copyright 2025 NXP

#ifndef OCPP_V2_CONVERSIONS_HPP
#define OCPP_V2_CONVERSIONS_HPP

#if 0
#include <generated/types/display_message.hpp>
#include <generated/types/evse_manager.hpp>
#include <generated/types/iso15118.hpp>
#include <generated/types/ocpp.hpp>
#include <generated/types/reservation.hpp>
#include <generated/types/system.hpp>
#endif

#include <ocpp/v2/messages/Authorize.hpp>
#include <ocpp/v2/messages/BootNotification.hpp>
#include <ocpp/v2/messages/ClearDisplayMessage.hpp>
#include <ocpp/v2/messages/DataTransfer.hpp>
#include <ocpp/v2/messages/FirmwareStatusNotification.hpp>
#include <ocpp/v2/messages/Get15118EVCertificate.hpp>
#include <ocpp/v2/messages/GetDisplayMessages.hpp>
#include <ocpp/v2/messages/GetLog.hpp>
#include <ocpp/v2/messages/NotifyEVChargingNeeds.hpp>
#include <ocpp/v2/messages/SetDisplayMessage.hpp>
#include <ocpp/v2/messages/TransactionEvent.hpp>
#include <ocpp/v2/messages/UpdateFirmware.hpp>

namespace module {
namespace conversions {

/// \brief Converts the provided parameters to an ocpp::v2::SampledValue.
ocpp::v2::SampledValue to_ocpp_sampled_value(const ocpp::v2::ReadingContextEnum& reading_context,
                                             const ocpp::v2::MeasurandEnum& measurand, const std::string& unit,
                                             const std::optional<ocpp::v2::PhaseEnum> phase,
                                             ocpp::v2::LocationEnum location = ocpp::v2::LocationEnum::Outlet);

/// \brief Converts the given types::units_signed::SignedMeterValue \p signed_meter_value  to an
/// ocpp::v2::SignedMeterValue.
ocpp::v2::SignedMeterValue to_ocpp_signed_meter_value(const ocpp::v2::SignedMeterValue& signed_meter_value);

/// \brief Converts the provided parameters to an ocpp::v2::MeterValue.
ocpp::v2::MeterValue to_ocpp_meter_value(const ocpp::Powermeter& power_meter,
                                         const ocpp::v2::ReadingContextEnum& reading_context,
                                         const std::optional<ocpp::v2::SignedMeterValue> signed_meter_value);

ocpp::DateTime to_ocpp_datetime_or_now(const std::string& datetime_string);
} // namespace conversions
} // namespace module

#endif // OCPP_V2_CONVERSIONS_HPP
