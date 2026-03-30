// SPDX-License-Identifier: Apache-2.0
// Copyright 2020 - 2023 Pionix GmbH and Contributors to EVerest
// Copyright 2026 NXP

#include <ocpp/common/charging_station_base.hpp>

namespace ocpp {
ChargingStationBase::ChargingStationBase(const EvseSecurity& evse_security,
                                         const std::optional<SecurityConfiguration> security_configuration) {

	configPRINTF(("No implementation of evse_security to create the charging station base"));
    return;
}

ChargingStationBase::ChargingStationBase(const std::shared_ptr<EvseSecurity> evse_security,
                             const std::optional<SecurityConfiguration> security_configuration)
{
#if 1
    if (evse_security != nullptr) {
        this->evse_security = evse_security;
    } else {
        if (!security_configuration.has_value()) {
        	configPRINTF(("No implementation of EvseSecurity interface and no SecurityConfiguration provided to chargepoint constructor. One of options must be set"));
            //return;
        	throw std::runtime_error("No implementation of EvseSecurity interface and no SecurityConfiguration provided to chargepoint constructor. One of options must be set");
        }
        this->evse_security = std::make_shared<EvseSecurityImpl>(security_configuration.value());
    }
#endif
#if 0
    this->work = boost::make_shared<boost::asio::executor_work_guard<boost::asio::io_context::executor_type>>(
        boost::asio::make_work_guard(this->io_context));
#endif
    this->io_context_thread = std::thread([this]() { this->io_context.run(); });
}

ChargingStationBase::~ChargingStationBase() {
#if 0
    work->get_executor().context().stop();
#endif
    io_context.stop();
    io_context_thread.join();
}

} // namespace ocpp
