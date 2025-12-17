// SPDX-License-Identifier: Apache-2.0
// Copyright Pionix GmbH and Contributors to EVerest
// Copyright 2025 NXP

#include <conversions.hpp>
#include "types.hpp"
#include "ocpp_enums.hpp"


namespace module {
namespace conversions {
ocpp::v2::SampledValue to_ocpp_sampled_value(const ocpp::v2::ReadingContextEnum& reading_context,
                                             const ocpp::v2::MeasurandEnum& measurand, const std::string& unit,
                                             const std::optional<ocpp::v2::PhaseEnum> phase,
                                             ocpp::v2::LocationEnum location) {
    ocpp::v2::SampledValue sampled_value;
    ocpp::v2::UnitOfMeasure unit_of_measure;
    sampled_value.context = reading_context;
    sampled_value.location = location;
    sampled_value.measurand = measurand;
    unit_of_measure.unit = unit;
    sampled_value.unitOfMeasure = unit_of_measure;
    sampled_value.phase = phase;
    return sampled_value;
}

#if 0
ocpp::v2::SignedMeterValue to_ocpp_signed_meter_value(const types::units_signed::SignedMeterValue& signed_meter_value) {
    ocpp::v2::SignedMeterValue ocpp_signed_meter_value;
    ocpp_signed_meter_value.signedMeterData = signed_meter_value.signed_meter_data;
    ocpp_signed_meter_value.signingMethod = signed_meter_value.signing_method;
    ocpp_signed_meter_value.encodingMethod = signed_meter_value.encoding_method;
    ocpp_signed_meter_value.publicKey = signed_meter_value.public_key.value_or("");

    return ocpp_signed_meter_value;
}
#else
ocpp::v2::SignedMeterValue to_ocpp_signed_meter_value(const ocpp::v2::SignedMeterValue& signed_meter_value)
{
	return signed_meter_value;
}
#endif

ocpp::DateTime to_ocpp_datetime_or_now(const std::string& datetime_string) {
//    std::optional<ocpp::DateTime> timestamp;
//    try {
//        return ocpp::DateTime(datetime_string);
//    } catch (const ocpp::TimePointParseException& e) {
//        configPRINTF(("Could not parse datetime string. Using current DateTime instead"));
//    }
    return ocpp::DateTime();
}

ocpp::v2::MeterValue
to_ocpp_meter_value(const ocpp::Powermeter& power_meter,
                    const ocpp::v2::ReadingContextEnum& reading_context,
                    const std::optional<ocpp::v2::SignedMeterValue> signed_meter_value) {
    ocpp::v2::MeterValue meter_value;
    meter_value.timestamp = to_ocpp_datetime_or_now(power_meter.timestamp);

    bool energy_Wh_import_signed_total_added = false;
    // individual signed meter values can be provided by the power_meter itself

    ocpp::v2::SampledValue sampled_value = to_ocpp_sampled_value(
        reading_context, ocpp::v2::MeasurandEnum::Energy_Active_Import_Register, "Wh", std::nullopt);

    // Energy.Active.Import.Register
#if EVSE_OCPP_SIGNED_METER_VALUES
    if (power_meter.energy_Wh_import_signed.has_value()) {
        sampled_value.value = power_meter.energy_Wh_import.total;
        const auto& energy_Wh_import_signed = power_meter.energy_Wh_import_signed.value();
        if (energy_Wh_import_signed.total.has_value()) {
            sampled_value.signedMeterValue = to_ocpp_signed_meter_value(energy_Wh_import_signed.total.value());
            energy_Wh_import_signed_total_added = true;
        }
        meter_value.sampledValue.push_back(sampled_value);
    }
#endif

    if (not energy_Wh_import_signed_total_added) {
        // No signed meter value for Energy.Active.Import.Register added, either no signed meter values are available or
        // just one global signed_meter_value is present signed_meter_value is intended for OCMF style blobs of signed
        // meter value reports during transaction start or end
        // This is interpreted as Energy.Active.Import.Register
        sampled_value = to_ocpp_sampled_value(reading_context, ocpp::v2::MeasurandEnum::Energy_Active_Import_Register,
                                              "Wh", std::nullopt);
        sampled_value.value = power_meter.energy_Wh_import.total;
        // add signedMeterValue if present
        if (signed_meter_value.has_value()) {
            sampled_value.signedMeterValue = to_ocpp_signed_meter_value(signed_meter_value.value());
        }
        meter_value.sampledValue.push_back(sampled_value);
    }

    if (power_meter.energy_Wh_import.L1.has_value()) {
        sampled_value = to_ocpp_sampled_value(reading_context, ocpp::v2::MeasurandEnum::Energy_Active_Import_Register,
                                              "Wh", ocpp::v2::PhaseEnum::L1);
        sampled_value.value = power_meter.energy_Wh_import.L1.value();
#if EVSE_OCPP_SIGNED_METER_VALUES
        if (power_meter.energy_Wh_import_signed.has_value()) {
            const auto& energy_Wh_import_signed = power_meter.energy_Wh_import_signed.value();
            if (energy_Wh_import_signed.L1.has_value()) {
                sampled_value.signedMeterValue = to_ocpp_signed_meter_value(energy_Wh_import_signed.L1.value());
            }
        }
#endif
        meter_value.sampledValue.push_back(sampled_value);
    }
    if (power_meter.energy_Wh_import.L2.has_value()) {
        sampled_value = to_ocpp_sampled_value(reading_context, ocpp::v2::MeasurandEnum::Energy_Active_Import_Register,
                                              "Wh", ocpp::v2::PhaseEnum::L2);
        sampled_value.value = power_meter.energy_Wh_import.L2.value();
#if EVSE_OCPP_SIGNED_METER_VALUES
        if (power_meter.energy_Wh_import_signed.has_value()) {
            const auto& energy_Wh_import_signed = power_meter.energy_Wh_import_signed.value();
            if (energy_Wh_import_signed.L2.has_value()) {
                sampled_value.signedMeterValue = to_ocpp_signed_meter_value(energy_Wh_import_signed.L2.value());
            }
        }
#endif
        meter_value.sampledValue.push_back(sampled_value);
    }
    if (power_meter.energy_Wh_import.L3.has_value()) {
        sampled_value = to_ocpp_sampled_value(reading_context, ocpp::v2::MeasurandEnum::Energy_Active_Import_Register,
                                              "Wh", ocpp::v2::PhaseEnum::L3);
        sampled_value.value = power_meter.energy_Wh_import.L3.value();
#if EVSE_OCPP_SIGNED_METER_VALUES
        if (power_meter.energy_Wh_import_signed.has_value()) {
            const auto& energy_Wh_import_signed = power_meter.energy_Wh_import_signed.value();
            if (energy_Wh_import_signed.L3.has_value()) {
                sampled_value.signedMeterValue = to_ocpp_signed_meter_value(energy_Wh_import_signed.L3.value());
            }
        }
#endif
        meter_value.sampledValue.push_back(sampled_value);
    }

    // Energy.Active.Export.Register
    if (power_meter.energy_Wh_export.has_value()) {
        auto sampled_value = to_ocpp_sampled_value(
            reading_context, ocpp::v2::MeasurandEnum::Energy_Active_Export_Register, "Wh", std::nullopt);
        sampled_value.value = power_meter.energy_Wh_export.value().total;
#if EVSE_OCPP_SIGNED_METER_VALUES
        if (power_meter.energy_Wh_export_signed.has_value()) {
            const auto& energy_Wh_export_signed = power_meter.energy_Wh_export_signed.value();
            if (energy_Wh_export_signed.total.has_value()) {
                sampled_value.signedMeterValue = to_ocpp_signed_meter_value(energy_Wh_export_signed.total.value());
            }
        }
#endif
        meter_value.sampledValue.push_back(sampled_value);
        if (power_meter.energy_Wh_export.value().L1.has_value()) {
            sampled_value = to_ocpp_sampled_value(
                reading_context, ocpp::v2::MeasurandEnum::Energy_Active_Export_Register, "Wh", ocpp::v2::PhaseEnum::L1);
            sampled_value.value = power_meter.energy_Wh_export.value().L1.value();
#if EVSE_OCPP_SIGNED_METER_VALUES
            if (power_meter.energy_Wh_export_signed.has_value()) {
                const auto& energy_Wh_export_signed = power_meter.energy_Wh_export_signed.value();
                if (energy_Wh_export_signed.L1.has_value()) {
                    sampled_value.signedMeterValue = to_ocpp_signed_meter_value(energy_Wh_export_signed.L1.value());
                }
            }
#endif
            meter_value.sampledValue.push_back(sampled_value);
        }
        if (power_meter.energy_Wh_export.value().L2.has_value()) {
            sampled_value = to_ocpp_sampled_value(
                reading_context, ocpp::v2::MeasurandEnum::Energy_Active_Export_Register, "Wh", ocpp::v2::PhaseEnum::L2);
            sampled_value.value = power_meter.energy_Wh_export.value().L2.value();
#if EVSE_OCPP_SIGNED_METER_VALUES
            if (power_meter.energy_Wh_export_signed.has_value()) {
                const auto& energy_Wh_export_signed = power_meter.energy_Wh_export_signed.value();
                if (energy_Wh_export_signed.L2.has_value()) {
                    sampled_value.signedMeterValue = to_ocpp_signed_meter_value(energy_Wh_export_signed.L2.value());
                }
            }
#endif
            meter_value.sampledValue.push_back(sampled_value);
        }
        if (power_meter.energy_Wh_export.value().L3.has_value()) {
            sampled_value = to_ocpp_sampled_value(
                reading_context, ocpp::v2::MeasurandEnum::Energy_Active_Export_Register, "Wh", ocpp::v2::PhaseEnum::L3);
            sampled_value.value = power_meter.energy_Wh_export.value().L3.value();
#if EVSE_OCPP_SIGNED_METER_VALUES
            if (power_meter.energy_Wh_export_signed.has_value()) {
                const auto& energy_Wh_export_signed = power_meter.energy_Wh_export_signed.value();
                if (energy_Wh_export_signed.L3.has_value()) {
                    sampled_value.signedMeterValue = to_ocpp_signed_meter_value(energy_Wh_export_signed.L3.value());
                }
            }
#endif
            meter_value.sampledValue.push_back(sampled_value);
        }
    }

    // Power.Active.Import
    if (power_meter.power_W.has_value()) {
        auto sampled_value =
            to_ocpp_sampled_value(reading_context, ocpp::v2::MeasurandEnum::Power_Active_Import, "W", std::nullopt);
        sampled_value.value = power_meter.power_W.value().total;
#if EVSE_OCPP_SIGNED_METER_VALUES
        if (power_meter.power_W_signed.has_value()) {
            const auto& power_W_signed = power_meter.power_W_signed.value();
            if (power_W_signed.total.has_value()) {
                sampled_value.signedMeterValue = to_ocpp_signed_meter_value(power_W_signed.total.value());
            }
        }
#endif
        meter_value.sampledValue.push_back(sampled_value);
        if (power_meter.power_W.value().L1.has_value()) {
            sampled_value = to_ocpp_sampled_value(reading_context, ocpp::v2::MeasurandEnum::Power_Active_Import, "W",
                                                  ocpp::v2::PhaseEnum::L1);
            sampled_value.value = power_meter.power_W.value().L1.value();
#if EVSE_OCPP_SIGNED_METER_VALUES
            if (power_meter.power_W_signed.has_value()) {
                const auto& power_W_signed = power_meter.power_W_signed.value();
                if (power_W_signed.L1.has_value()) {
                    sampled_value.signedMeterValue = to_ocpp_signed_meter_value(power_W_signed.L1.value());
                }
            }
#endif
            meter_value.sampledValue.push_back(sampled_value);
        }
        if (power_meter.power_W.value().L2.has_value()) {
            sampled_value = to_ocpp_sampled_value(reading_context, ocpp::v2::MeasurandEnum::Power_Active_Import, "W",
                                                  ocpp::v2::PhaseEnum::L2);
            sampled_value.value = power_meter.power_W.value().L2.value();
#if EVSE_OCPP_SIGNED_METER_VALUES
            if (power_meter.power_W_signed.has_value()) {
                const auto& power_W_signed = power_meter.power_W_signed.value();
                if (power_W_signed.L2.has_value()) {
                    sampled_value.signedMeterValue = to_ocpp_signed_meter_value(power_W_signed.L2.value());
                }
            }
#endif
            meter_value.sampledValue.push_back(sampled_value);
        }
        if (power_meter.power_W.value().L3.has_value()) {
            sampled_value = to_ocpp_sampled_value(reading_context, ocpp::v2::MeasurandEnum::Power_Active_Import, "W",
                                                  ocpp::v2::PhaseEnum::L3);
            sampled_value.value = power_meter.power_W.value().L3.value();
#if EVSE_OCPP_SIGNED_METER_VALUES
            if (power_meter.power_W_signed.has_value()) {
                const auto& power_W_signed = power_meter.power_W_signed.value();
                if (power_W_signed.L3.has_value()) {
                    sampled_value.signedMeterValue = to_ocpp_signed_meter_value(power_W_signed.L3.value());
                }
            }
#endif
            meter_value.sampledValue.push_back(sampled_value);
        }
    }

    // Power.Reactive.Import
    if (power_meter.VAR.has_value()) {
        auto sampled_value =
            to_ocpp_sampled_value(reading_context, ocpp::v2::MeasurandEnum::Power_Reactive_Import, "var", std::nullopt);
        sampled_value.value = power_meter.VAR.value().total;
#if EVSE_OCPP_SIGNED_METER_VALUES
        if (power_meter.VAR_signed.has_value()) {
            const auto& VAR_signed = power_meter.VAR_signed.value();
            if (VAR_signed.total.has_value()) {
                sampled_value.signedMeterValue = to_ocpp_signed_meter_value(VAR_signed.total.value());
            }
        }
#endif
        meter_value.sampledValue.push_back(sampled_value);
        if (power_meter.VAR.value().VARphA.has_value()) {
            sampled_value = to_ocpp_sampled_value(reading_context, ocpp::v2::MeasurandEnum::Power_Reactive_Import,
                                                  "var", ocpp::v2::PhaseEnum::L1);
            sampled_value.value = power_meter.VAR.value().VARphA.value();
#if EVSE_OCPP_SIGNED_METER_VALUES
            if (power_meter.VAR_signed.has_value()) {
                const auto& VAR_signed = power_meter.VAR_signed.value();
                if (VAR_signed.L1.has_value()) {
                    sampled_value.signedMeterValue = to_ocpp_signed_meter_value(VAR_signed.L1.value());
                }
            }
#endif
            meter_value.sampledValue.push_back(sampled_value);
        }
        if (power_meter.VAR.value().VARphB.has_value()) {
            sampled_value = to_ocpp_sampled_value(reading_context, ocpp::v2::MeasurandEnum::Power_Reactive_Import,
                                                  "var", ocpp::v2::PhaseEnum::L2);
            sampled_value.value = power_meter.VAR.value().VARphB.value();
#if EVSE_OCPP_SIGNED_METER_VALUES
            if (power_meter.VAR_signed.has_value()) {
                const auto& VAR_signed = power_meter.VAR_signed.value();
                if (VAR_signed.L2.has_value()) {
                    sampled_value.signedMeterValue = to_ocpp_signed_meter_value(VAR_signed.L2.value());
                }
            }
#endif
            meter_value.sampledValue.push_back(sampled_value);
        }
        if (power_meter.VAR.value().VARphC.has_value()) {
            sampled_value = to_ocpp_sampled_value(reading_context, ocpp::v2::MeasurandEnum::Power_Reactive_Import,
                                                  "var", ocpp::v2::PhaseEnum::L3);
            sampled_value.value = power_meter.VAR.value().VARphC.value();
#if EVSE_OCPP_SIGNED_METER_VALUES
            if (power_meter.VAR_signed.has_value()) {
                const auto& VAR_signed = power_meter.VAR_signed.value();
                if (VAR_signed.L3.has_value()) {
                    sampled_value.signedMeterValue = to_ocpp_signed_meter_value(VAR_signed.L3.value());
                }
            }
#endif
            meter_value.sampledValue.push_back(sampled_value);
        }
    }

    // Current.Import
    if (power_meter.current_A.has_value()) {
        auto sampled_value =
            to_ocpp_sampled_value(reading_context, ocpp::v2::MeasurandEnum::Current_Import, "A", std::nullopt);
        if (power_meter.current_A.value().L1.has_value()) {
            sampled_value = to_ocpp_sampled_value(reading_context, ocpp::v2::MeasurandEnum::Current_Import, "A",
                                                  ocpp::v2::PhaseEnum::L1);
            sampled_value.value = power_meter.current_A.value().L1.value();
#if EVSE_OCPP_SIGNED_METER_VALUES
            if (power_meter.current_A_signed.has_value()) {
                const auto& current_A_signed = power_meter.current_A_signed.value();
                if (current_A_signed.L1.has_value()) {
                    sampled_value.signedMeterValue = to_ocpp_signed_meter_value(current_A_signed.L1.value());
                }
            }
#endif
            meter_value.sampledValue.push_back(sampled_value);
        }
        if (power_meter.current_A.value().L2.has_value()) {
            sampled_value = to_ocpp_sampled_value(reading_context, ocpp::v2::MeasurandEnum::Current_Import, "A",
                                                  ocpp::v2::PhaseEnum::L2);
            sampled_value.value = power_meter.current_A.value().L2.value();
#if EVSE_OCPP_SIGNED_METER_VALUES
            if (power_meter.current_A_signed.has_value()) {
                const auto& current_A_signed = power_meter.current_A_signed.value();
                if (current_A_signed.L2.has_value()) {
                    sampled_value.signedMeterValue = to_ocpp_signed_meter_value(current_A_signed.L2.value());
                }
            }
#endif
            meter_value.sampledValue.push_back(sampled_value);
        }
        if (power_meter.current_A.value().L3.has_value()) {
            sampled_value = to_ocpp_sampled_value(reading_context, ocpp::v2::MeasurandEnum::Current_Import, "A",
                                                  ocpp::v2::PhaseEnum::L3);
            sampled_value.value = power_meter.current_A.value().L3.value();
#if EVSE_OCPP_SIGNED_METER_VALUES
            if (power_meter.current_A_signed.has_value()) {
                const auto& current_A_signed = power_meter.current_A_signed.value();
                if (current_A_signed.L3.has_value()) {
                    sampled_value.signedMeterValue = to_ocpp_signed_meter_value(current_A_signed.L3.value());
                }
            }
#endif
            meter_value.sampledValue.push_back(sampled_value);
        }
        if (power_meter.current_A.value().DC.has_value()) {
            sampled_value =
                to_ocpp_sampled_value(reading_context, ocpp::v2::MeasurandEnum::Current_Import, "A", std::nullopt);
            sampled_value.value = power_meter.current_A.value().DC.value();
#if EVSE_OCPP_SIGNED_METER_VALUES
            if (power_meter.current_A_signed.has_value()) {
                const auto& current_A_signed = power_meter.current_A_signed.value();
                if (current_A_signed.DC.has_value()) {
                    sampled_value.signedMeterValue = to_ocpp_signed_meter_value(current_A_signed.DC.value());
                }
            }
#endif
            meter_value.sampledValue.push_back(sampled_value);
        }
        if (power_meter.current_A.value().N.has_value()) {
            sampled_value = to_ocpp_sampled_value(reading_context, ocpp::v2::MeasurandEnum::Current_Import, "A",
                                                  ocpp::v2::PhaseEnum::N);
            sampled_value.value = power_meter.current_A.value().N.value();
#if EVSE_OCPP_SIGNED_METER_VALUES
            if (power_meter.current_A_signed.has_value()) {
                const auto& current_A_signed = power_meter.current_A_signed.value();
                if (current_A_signed.N.has_value()) {
                    sampled_value.signedMeterValue = to_ocpp_signed_meter_value(current_A_signed.N.value());
                }
            }
#endif
            meter_value.sampledValue.push_back(sampled_value);
        }
    }

    // Voltage
    if (power_meter.voltage_V.has_value()) {
        if (power_meter.voltage_V.value().L1.has_value()) {
            sampled_value = to_ocpp_sampled_value(reading_context, ocpp::v2::MeasurandEnum::Voltage, "V",
                                                  ocpp::v2::PhaseEnum::L1_N);
            sampled_value.value = power_meter.voltage_V.value().L1.value();
#if EVSE_OCPP_SIGNED_METER_VALUES
            if (power_meter.voltage_V_signed.has_value()) {
                const auto& voltage_V_signed = power_meter.voltage_V_signed.value();
                if (voltage_V_signed.L1.has_value()) {
                    sampled_value.signedMeterValue = to_ocpp_signed_meter_value(voltage_V_signed.L1.value());
                }
            }
#endif
            meter_value.sampledValue.push_back(sampled_value);
        }
        if (power_meter.voltage_V.value().L2.has_value()) {
            sampled_value = to_ocpp_sampled_value(reading_context, ocpp::v2::MeasurandEnum::Voltage, "V",
                                                  ocpp::v2::PhaseEnum::L2_N);
            sampled_value.value = power_meter.voltage_V.value().L2.value();
#if EVSE_OCPP_SIGNED_METER_VALUES
            if (power_meter.voltage_V_signed.has_value()) {
                const auto& voltage_V_signed = power_meter.voltage_V_signed.value();
                if (voltage_V_signed.L2.has_value()) {
                    sampled_value.signedMeterValue = to_ocpp_signed_meter_value(voltage_V_signed.L2.value());
                }
            }
#endif
            meter_value.sampledValue.push_back(sampled_value);
        }
        if (power_meter.voltage_V.value().L3.has_value()) {
            sampled_value = to_ocpp_sampled_value(reading_context, ocpp::v2::MeasurandEnum::Voltage, "V",
                                                  ocpp::v2::PhaseEnum::L3_N);
            sampled_value.value = power_meter.voltage_V.value().L3.value();
#if EVSE_OCPP_SIGNED_METER_VALUES
            if (power_meter.voltage_V_signed.has_value()) {
                const auto& voltage_V_signed = power_meter.voltage_V_signed.value();
                if (voltage_V_signed.L3.has_value()) {
                    sampled_value.signedMeterValue = to_ocpp_signed_meter_value(voltage_V_signed.L3.value());
                }
            }
#endif
            meter_value.sampledValue.push_back(sampled_value);
        }
        if (power_meter.voltage_V.value().DC.has_value()) {
            sampled_value = to_ocpp_sampled_value(reading_context, ocpp::v2::MeasurandEnum::Voltage, "V", std::nullopt);
            sampled_value.value = power_meter.voltage_V.value().DC.value();
#if EVSE_OCPP_SIGNED_METER_VALUES
            if (power_meter.voltage_V_signed.has_value()) {
                const auto& voltage_V_signed = power_meter.voltage_V_signed.value();
                if (voltage_V_signed.DC.has_value()) {
                    sampled_value.signedMeterValue = to_ocpp_signed_meter_value(voltage_V_signed.DC.value());
                }
            }
#endif
            meter_value.sampledValue.push_back(sampled_value);
        }
    }
    return meter_value;
}
#if 0
ocpp::v2::ReasonEnum to_ocpp_reason(types::evse_manager::StopTransactionReason reason) {
    switch (reason) {
    case types::evse_manager::StopTransactionReason::DeAuthorized:
        return ocpp::v2::ReasonEnum::DeAuthorized;
    case types::evse_manager::StopTransactionReason::EmergencyStop:
        return ocpp::v2::ReasonEnum::EmergencyStop;
    case types::evse_manager::StopTransactionReason::EnergyLimitReached:
        return ocpp::v2::ReasonEnum::EnergyLimitReached;
    case types::evse_manager::StopTransactionReason::EVDisconnected:
        return ocpp::v2::ReasonEnum::EVDisconnected;
    case types::evse_manager::StopTransactionReason::GroundFault:
        return ocpp::v2::ReasonEnum::GroundFault;
    case types::evse_manager::StopTransactionReason::HardReset:
        return ocpp::v2::ReasonEnum::ImmediateReset;
    case types::evse_manager::StopTransactionReason::Local:
        return ocpp::v2::ReasonEnum::Local;
    case types::evse_manager::StopTransactionReason::LocalOutOfCredit:
        return ocpp::v2::ReasonEnum::LocalOutOfCredit;
    case types::evse_manager::StopTransactionReason::MasterPass:
        return ocpp::v2::ReasonEnum::MasterPass;
    case types::evse_manager::StopTransactionReason::Other:
        return ocpp::v2::ReasonEnum::Other;
    case types::evse_manager::StopTransactionReason::OvercurrentFault:
        return ocpp::v2::ReasonEnum::OvercurrentFault;
    case types::evse_manager::StopTransactionReason::PowerLoss:
        return ocpp::v2::ReasonEnum::PowerLoss;
    case types::evse_manager::StopTransactionReason::PowerQuality:
        return ocpp::v2::ReasonEnum::PowerQuality;
    case types::evse_manager::StopTransactionReason::Reboot:
        return ocpp::v2::ReasonEnum::Reboot;
    case types::evse_manager::StopTransactionReason::Remote:
        return ocpp::v2::ReasonEnum::Remote;
    case types::evse_manager::StopTransactionReason::SOCLimitReached:
        return ocpp::v2::ReasonEnum::SOCLimitReached;
    case types::evse_manager::StopTransactionReason::StoppedByEV:
        return ocpp::v2::ReasonEnum::StoppedByEV;
    case types::evse_manager::StopTransactionReason::TimeLimitReached:
        return ocpp::v2::ReasonEnum::TimeLimitReached;
    case types::evse_manager::StopTransactionReason::Timeout:
        return ocpp::v2::ReasonEnum::Timeout;
    case types::evse_manager::StopTransactionReason::ReqEnergyTransferRejected:
        return ocpp::v2::ReasonEnum::ReqEnergyTransferRejected;
    case types::evse_manager::StopTransactionReason::SoftReset:
    case types::evse_manager::StopTransactionReason::UnlockCommand:
        return ocpp::v2::ReasonEnum::Other;
    }
    return ocpp::v2::ReasonEnum::Other;
}

ocpp::v2::IdToken to_ocpp_id_token(const types::authorization::IdToken& id_token) {
    ocpp::v2::IdToken ocpp_id_token = {id_token.value, types::authorization::id_token_type_to_string(id_token.type)};
    if (id_token.additional_info.has_value()) {
        std::vector<ocpp::v2::AdditionalInfo> ocpp_additional_info;
        const auto& additional_info = id_token.additional_info.value();
        for (const auto& entry : additional_info) {
            ocpp_additional_info.push_back({entry.value, entry.type});
        }
        ocpp_id_token.additionalInfo = ocpp_additional_info;
    }
    return ocpp_id_token;
}

types::evse_manager::StopTransactionReason to_everest_stop_transaction_reason(const ocpp::v2::ReasonEnum& stop_reason) {
    switch (stop_reason) {
    case ocpp::v2::ReasonEnum::DeAuthorized:
        return types::evse_manager::StopTransactionReason::DeAuthorized;
    case ocpp::v2::ReasonEnum::EmergencyStop:
        return types::evse_manager::StopTransactionReason::EmergencyStop;
    case ocpp::v2::ReasonEnum::EnergyLimitReached:
        return types::evse_manager::StopTransactionReason::EnergyLimitReached;
    case ocpp::v2::ReasonEnum::EVDisconnected:
        return types::evse_manager::StopTransactionReason::EVDisconnected;
    case ocpp::v2::ReasonEnum::GroundFault:
        return types::evse_manager::StopTransactionReason::GroundFault;
    case ocpp::v2::ReasonEnum::ImmediateReset:
        return types::evse_manager::StopTransactionReason::HardReset;
    case ocpp::v2::ReasonEnum::Local:
        return types::evse_manager::StopTransactionReason::Local;
    case ocpp::v2::ReasonEnum::LocalOutOfCredit:
        return types::evse_manager::StopTransactionReason::LocalOutOfCredit;
    case ocpp::v2::ReasonEnum::MasterPass:
        return types::evse_manager::StopTransactionReason::MasterPass;
    case ocpp::v2::ReasonEnum::Other:
        return types::evse_manager::StopTransactionReason::Other;
    case ocpp::v2::ReasonEnum::OvercurrentFault:
        return types::evse_manager::StopTransactionReason::OvercurrentFault;
    case ocpp::v2::ReasonEnum::PowerLoss:
        return types::evse_manager::StopTransactionReason::PowerLoss;
    case ocpp::v2::ReasonEnum::PowerQuality:
        return types::evse_manager::StopTransactionReason::PowerQuality;
    case ocpp::v2::ReasonEnum::Reboot:
        return types::evse_manager::StopTransactionReason::Reboot;
    case ocpp::v2::ReasonEnum::Remote:
        return types::evse_manager::StopTransactionReason::Remote;
    case ocpp::v2::ReasonEnum::SOCLimitReached:
        return types::evse_manager::StopTransactionReason::SOCLimitReached;
    case ocpp::v2::ReasonEnum::StoppedByEV:
        return types::evse_manager::StopTransactionReason::StoppedByEV;
    case ocpp::v2::ReasonEnum::TimeLimitReached:
        return types::evse_manager::StopTransactionReason::TimeLimitReached;
    case ocpp::v2::ReasonEnum::Timeout:
        return types::evse_manager::StopTransactionReason::Timeout;
    case ocpp::v2::ReasonEnum::ReqEnergyTransferRejected:
        return types::evse_manager::StopTransactionReason::ReqEnergyTransferRejected;
    }
    return types::evse_manager::StopTransactionReason::Other;
}

std::vector<ocpp::v2::GetVariableData>
to_ocpp_get_variable_data_vector(const std::vector<types::ocpp::GetVariableRequest>& get_variable_request_vector) {
    std::vector<ocpp::v2::GetVariableData> ocpp_get_variable_data_vector;
    for (const auto& get_variable_request : get_variable_request_vector) {
        ocpp::v2::GetVariableData get_variable_data;
        get_variable_data.component = to_ocpp_component(get_variable_request.component_variable.component);
        get_variable_data.variable = to_ocpp_variable(get_variable_request.component_variable.variable);
        if (get_variable_request.attribute_type.has_value()) {
            get_variable_data.attributeType = to_ocpp_attribute_enum(get_variable_request.attribute_type.value());
        }
        ocpp_get_variable_data_vector.push_back(get_variable_data);
    }
    return ocpp_get_variable_data_vector;
}

std::vector<ocpp::v2::SetVariableData>
to_ocpp_set_variable_data_vector(const std::vector<types::ocpp::SetVariableRequest>& set_variable_request_vector) {
    std::vector<ocpp::v2::SetVariableData> ocpp_set_variable_data_vector;
    for (const auto& set_variable_request : set_variable_request_vector) {
        ocpp::v2::SetVariableData set_variable_data;
        set_variable_data.component = to_ocpp_component(set_variable_request.component_variable.component);
        set_variable_data.variable = to_ocpp_variable(set_variable_request.component_variable.variable);
        if (set_variable_request.attribute_type.has_value()) {
            set_variable_data.attributeType = to_ocpp_attribute_enum(set_variable_request.attribute_type.value());
        }
        try {
            set_variable_data.attributeValue = set_variable_request.value;
        } catch (std::runtime_error& e) {
            EVLOG_error << "Could not convert attributeValue to CiString";
            continue;
        }
        ocpp_set_variable_data_vector.push_back(set_variable_data);
    }
    return ocpp_set_variable_data_vector;
}

ocpp::v2::Component to_ocpp_component(const types::ocpp::Component& component) {
    ocpp::v2::Component _component;
    _component.name = component.name;
    if (component.evse.has_value()) {
        _component.evse = to_ocpp_evse(component.evse.value());
    }
    if (component.instance.has_value()) {
        _component.instance = component.instance.value();
    }
    return _component;
}

ocpp::v2::Variable to_ocpp_variable(const types::ocpp::Variable& variable) {
    ocpp::v2::Variable _variable;
    _variable.name = variable.name;
    if (variable.instance.has_value()) {
        _variable.instance = variable.instance.value();
    }
    return _variable;
}

ocpp::v2::EVSE to_ocpp_evse(const types::ocpp::EVSE& evse) {
    ocpp::v2::EVSE _evse;
    _evse.id = evse.id;
    if (evse.connector_id.has_value()) {
        _evse.connectorId = evse.connector_id.value();
    }
    return _evse;
}

ocpp::v2::AttributeEnum to_ocpp_attribute_enum(const types::ocpp::AttributeEnum attribute_enum) {
    switch (attribute_enum) {
    case types::ocpp::AttributeEnum::Actual:
        return ocpp::v2::AttributeEnum::Actual;
    case types::ocpp::AttributeEnum::Target:
        return ocpp::v2::AttributeEnum::Target;
    case types::ocpp::AttributeEnum::MinSet:
        return ocpp::v2::AttributeEnum::MinSet;
    case types::ocpp::AttributeEnum::MaxSet:
        return ocpp::v2::AttributeEnum::MaxSet;
    }
    throw std::out_of_range("Could not convert AttributeEnum");
}

ocpp::v2::EnergyTransferModeEnum to_ocpp_energy_transfer_mode(const types::iso15118::EnergyTransferMode transfer_mode) {
    switch (transfer_mode) {
    case types::iso15118::EnergyTransferMode::AC_single_phase_core:
        return ocpp::v2::EnergyTransferModeEnum::AC_single_phase;
    case types::iso15118::EnergyTransferMode::AC_two_phase:
        return ocpp::v2::EnergyTransferModeEnum::AC_two_phase;
    case types::iso15118::EnergyTransferMode::AC_three_phase_core:
        return ocpp::v2::EnergyTransferModeEnum::AC_three_phase;
    case types::iso15118::EnergyTransferMode::DC:
    case types::iso15118::EnergyTransferMode::DC_core:
    case types::iso15118::EnergyTransferMode::DC_extended:
    case types::iso15118::EnergyTransferMode::DC_combo_core:
    case types::iso15118::EnergyTransferMode::DC_unique:
        return ocpp::v2::EnergyTransferModeEnum::DC;

    case types::iso15118::EnergyTransferMode::AC_BPT:
    case types::iso15118::EnergyTransferMode::AC_BPT_DER:
    case types::iso15118::EnergyTransferMode::AC_DER:
    case types::iso15118::EnergyTransferMode::DC_BPT:
    case types::iso15118::EnergyTransferMode::DC_ACDP:
    case types::iso15118::EnergyTransferMode::DC_ACDP_BPT:
    case types::iso15118::EnergyTransferMode::WPT:
        throw std::out_of_range("Could not convert EnergyTransferModeEnum: OCPP does not know this type");
    }

    throw std::out_of_range("Could not convert EnergyTransferMode");
}

ocpp::v2::NotifyEVChargingNeedsRequest
to_ocpp_notify_ev_charging_needs_request(const types::iso15118::ChargingNeeds& charging_needs) {
    // The evseId is calculated outside of this function in the required OCPP201 module
    ocpp::v2::NotifyEVChargingNeedsRequest _request;
    ocpp::v2::ChargingNeeds& _charging_needs = _request.chargingNeeds;

    _charging_needs.requestedEnergyTransfer = to_ocpp_energy_transfer_mode(charging_needs.requested_energy_transfer);

    // TODO(ioan): add v2x params
    if (charging_needs.ac_charging_parameters.has_value()) {
        const auto& ac = charging_needs.ac_charging_parameters.value();
        auto& ac_charging_parameters = _charging_needs.acChargingParameters.emplace();

        ac_charging_parameters.energyAmount = ac.energy_amount;
        ac_charging_parameters.evMinCurrent = ac.ev_min_current;
        ac_charging_parameters.evMaxCurrent = ac.ev_max_current;
        ac_charging_parameters.evMaxVoltage = ac.ev_max_voltage;
    } else if (charging_needs.dc_charging_parameters.has_value()) {
        const auto& dc = charging_needs.dc_charging_parameters.value();
        auto& dc_charging_parameters = _charging_needs.dcChargingParameters.emplace();

        dc_charging_parameters.evMaxCurrent = dc.ev_max_current;
        dc_charging_parameters.evMaxVoltage = dc.ev_max_voltage;
        dc_charging_parameters.energyAmount = dc.energy_amount;
        dc_charging_parameters.evMaxPower = dc.ev_max_power;
        dc_charging_parameters.stateOfCharge = dc.state_of_charge;
        dc_charging_parameters.evEnergyCapacity = dc.ev_energy_capacity;
        dc_charging_parameters.fullSoC = dc.full_soc;
        dc_charging_parameters.bulkSoC = dc.bulk_soc;
    }

    return _request;
}


types::authorization::AuthorizationStatus
to_everest_authorization_status(const ocpp::v2::AuthorizationStatusEnum status) {
    switch (status) {
    case ocpp::v2::AuthorizationStatusEnum::Accepted:
        return types::authorization::AuthorizationStatus::Accepted;
    case ocpp::v2::AuthorizationStatusEnum::Blocked:
        return types::authorization::AuthorizationStatus::Blocked;
    case ocpp::v2::AuthorizationStatusEnum::ConcurrentTx:
        return types::authorization::AuthorizationStatus::ConcurrentTx;
    case ocpp::v2::AuthorizationStatusEnum::Expired:
        return types::authorization::AuthorizationStatus::Expired;
    case ocpp::v2::AuthorizationStatusEnum::Invalid:
        return types::authorization::AuthorizationStatus::Invalid;
    case ocpp::v2::AuthorizationStatusEnum::NoCredit:
        return types::authorization::AuthorizationStatus::NoCredit;
    case ocpp::v2::AuthorizationStatusEnum::NotAllowedTypeEVSE:
        return types::authorization::AuthorizationStatus::NotAllowedTypeEVSE;
    case ocpp::v2::AuthorizationStatusEnum::NotAtThisLocation:
        return types::authorization::AuthorizationStatus::NotAtThisLocation;
    case ocpp::v2::AuthorizationStatusEnum::NotAtThisTime:
        return types::authorization::AuthorizationStatus::NotAtThisTime;
    case ocpp::v2::AuthorizationStatusEnum::Unknown:
        return types::authorization::AuthorizationStatus::Unknown;
    }
    throw std::out_of_range(
        "Could not convert ocpp::v2::AuthorizationStatusEnum to types::authorization::AuthorizationStatus");
}

types::authorization::IdToken to_everest_id_token(const ocpp::v2::IdToken& id_token) {
    types::authorization::IdToken _id_token;
    _id_token.value = id_token.idToken.get();
    _id_token.type = types::authorization::string_to_id_token_type(id_token.type);
    return _id_token;
}


types::ocpp::OcppTransactionEvent
to_everest_ocpp_transaction_event(const ocpp::v2::TransactionEventRequest& transaction_event) {
    types::ocpp::OcppTransactionEvent ocpp_transaction_event;
    switch (transaction_event.eventType) {
    case ocpp::v2::TransactionEventEnum::Started:
        ocpp_transaction_event.transaction_event = types::ocpp::TransactionEvent::Started;
        break;
    case ocpp::v2::TransactionEventEnum::Updated:
        ocpp_transaction_event.transaction_event = types::ocpp::TransactionEvent::Updated;
        break;
    case ocpp::v2::TransactionEventEnum::Ended:
        ocpp_transaction_event.transaction_event = types::ocpp::TransactionEvent::Ended;
        break;
    }

    if (transaction_event.evse.has_value()) {
        ocpp_transaction_event.evse = to_everest_evse(transaction_event.evse.value());
    }
    ocpp_transaction_event.session_id =
        transaction_event.transactionInfo.transactionId; // session_id == transaction_id for OCPP2.0.1
    ocpp_transaction_event.transaction_id = transaction_event.transactionInfo.transactionId;
    return ocpp_transaction_event;
}

types::ocpp::OcppTransactionEventResponse
to_everest_transaction_event_response(const ocpp::v2::TransactionEventResponse& transaction_event_response) {
    types::ocpp::OcppTransactionEventResponse everest_transaction_event_response;

    everest_transaction_event_response.total_cost = transaction_event_response.totalCost;
    everest_transaction_event_response.charging_priority = transaction_event_response.chargingPriority;
    if (transaction_event_response.updatedPersonalMessage.has_value()) {
        everest_transaction_event_response.personal_message =
            to_everest_message_content(transaction_event_response.updatedPersonalMessage.value());
    }

    return everest_transaction_event_response;
}

//types::ocpp::BootNotificationResponse
//to_everest_boot_notification_response(const ocpp::v2::BootNotificationResponse& boot_notification_response) {
//    types::ocpp::BootNotificationResponse everest_boot_notification_response;
//    everest_boot_notification_response.status = to_everest_registration_status(boot_notification_response.status);
//    everest_boot_notification_response.current_time = boot_notification_response.currentTime.to_rfc3339();
//    everest_boot_notification_response.interval = boot_notification_response.interval;
//    if (boot_notification_response.statusInfo.has_value()) {
//        everest_boot_notification_response.status_info =
//            to_everest_status_info_type(boot_notification_response.statusInfo.value());
//    }
//    return everest_boot_notification_response;
//}

//types::ocpp::RegistrationStatus
//to_everest_registration_status(const ocpp::v2::RegistrationStatusEnum& registration_status) {
//    switch (registration_status) {
//    case ocpp::v2::RegistrationStatusEnum::Accepted:
//        return types::ocpp::RegistrationStatus::Accepted;
//    case ocpp::v2::RegistrationStatusEnum::Pending:
//        return types::ocpp::RegistrationStatus::Pending;
//    case ocpp::v2::RegistrationStatusEnum::Rejected:
//        return types::ocpp::RegistrationStatus::Rejected;
//    }
//    throw std::out_of_range("Could not convert ocpp::v2::RegistrationStatusEnum to types::ocpp::RegistrationStatus");
//}

#endif

} // namespace conversions
} // namespace module
