// SPDX-License-Identifier: Apache-2.0
// Copyright 2020 - 2023 Pionix GmbH and Contributors to EVerest
// Copyright 2026 NXP

#include <ocpp/common/call_types.hpp>

#if EASYEVSE
extern void EVSE_OCPP_GenerateUUID(char* buffer, size_t buffer_size);
#endif

namespace ocpp {

MessageId create_message_id() {
#if EASYEVSE
	char buffer[40] = {0,};
	EVSE_OCPP_GenerateUUID(buffer, sizeof(buffer));
    return MessageId(std::string(buffer));
#else
    return MessageId(std::string("uuid-method-not-implemented"));
#endif
}

bool operator<(const MessageId& lhs, const MessageId& rhs) {
    return lhs.get() < rhs.get();
}

void to_json(json& j, const MessageId& k) {
    j = json(k.get());
}

void from_json(const json& j, MessageId& k) {
    k.set(j);
}

} // namespace ocpp
