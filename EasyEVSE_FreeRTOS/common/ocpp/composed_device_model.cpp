// SPDX-License-Identifier: Apache-2.0
// Copyright Pionix GmbH and Contributors to EVerest

#include "composed_device_model.hpp"
#include <ocpp/v2/ctrlr_component_variables.hpp>

//TODO: should be enum
#define SUPPORTED_OCPP_VERSIONS_IDX 0
#define PLACEHOLDER_VAR_CONNECTOR_IDX 1

static constexpr auto VARIABLE_SOURCE_OCPP = "OCPP";

//ocpp::v2::DeviceModelAttributeMap EVSE_OCPP_AttributeMap;
std::vector<ocpp::v2::VariableAttribute> variableAttributeList;

namespace module::device_model {

static ocpp::v2::DeviceModelMap builDeviceModelMap()
{
	ocpp::v2::DeviceModelMap myMap;
	ocpp::v2::VariableMetaData ctrlrMetaData, metaData, metaData2;
	ctrlrMetaData.source = VARIABLE_SOURCE_OCPP;

	ocpp::v2::VariableAttribute attribute;
    attribute.type = ocpp::v2::AttributeEnum::Actual;
    attribute.value = ocpp::CiString<25>("ocpp2.1,ocpp2.0.1");
	attribute.mutability = ocpp::v2::MutabilityEnum::ReadOnly;

	//ocpp::v2::Component{"InternalCtrlr"};
	//ocpp::v2::Variable myVar = ocpp::v2::ControllerComponentVariables::SupportedOcppVersions.variable.value();
	myMap[ocpp::v2::ControllerComponents::InternalCtrlr][ocpp::v2::Variable{"SupportedOcppVersions"}] = ctrlrMetaData;
	variableAttributeList.push_back(attribute);

	ocpp::v2::Component evse_component;
    ocpp::v2::EVSE evse;
    evse.id = 1;
    evse_component.evse = evse;
    evse_component.name = "EVSE";
    ocpp::v2::VariableCharacteristics power_char = {.dataType = ocpp::v2::DataEnum::decimal, .supportsMonitoring = true, .maxLimit = 510.5};
    metaData.characteristics = power_char;
    myMap[evse_component][ocpp::v2::EvseComponentVariables::Power] = metaData;

    ocpp::v2::Component connector;
    connector.name = "Connector";
    ocpp::v2::EVSE evse_conn;
    evse_conn.id = 1;
    evse_conn.connectorId = 1;
    connector.evse = evse_conn;

	ocpp::v2::VariableAttribute conn_attribute;
	conn_attribute.type = ocpp::v2::AttributeEnum::Actual;
	conn_attribute.value = "ocpp2.1,ocpp2.0.1"; //ocpp::CiString<25>("ocpp2.1,ocpp2.0.1");
    conn_attribute.mutability = ocpp::v2::MutabilityEnum::ReadOnly;

	myMap[connector][ocpp::v2::Variable{"PlaceholderVar"}] = metaData2;
	variableAttributeList.push_back(conn_attribute);

	return myMap;
}

ComposedDeviceModelStorage::ComposedDeviceModelStorage(const std::string& libocpp_device_model_storage_address,
                                                       const bool libocpp_initialize_device_model,
                                                       const std::string& device_model_migration_path,
                                                       const std::string& device_model_config_path) :
                                                    		   device_model_map(builDeviceModelMap())
{

}
ocpp::v2::DeviceModelMap ComposedDeviceModelStorage::get_device_model() {
#if 0
    ocpp::v2::DeviceModelMap everest_dm = everest_device_model_storage->get_device_model();
    ocpp::v2::DeviceModelMap libocpp_dm = libocpp_device_model_storage->get_device_model();
    everest_dm.merge(libocpp_dm);
    return everest_dm;
#endif

    configPRINTF(("Device model map size is %d", this->device_model_map.size()));
    return this->device_model_map;
}

std::optional<ocpp::v2::VariableAttribute>
ComposedDeviceModelStorage::get_variable_attribute(const ocpp::v2::Component& component_id,
                                                   const ocpp::v2::Variable& variable_id,
                                                   const ocpp::v2::AttributeEnum& attribute_enum) {
    if (get_variable_source(component_id, variable_id) == VARIABLE_SOURCE_OCPP) {
    	//return EVSE_OCPP_AttributeMap[component_id][variable_id];
    	if (variable_id.name == "SupportedOcppVersions"){
    	       return variableAttributeList[SUPPORTED_OCPP_VERSIONS_IDX];
    	}
#if 0
        return libocpp_device_model_storage->get_variable_attribute(component_id, variable_id, attribute_enum);
#endif
    }
    if (variable_id.name == "PlaceholderVar")
    {
    	return variableAttributeList[PLACEHOLDER_VAR_CONNECTOR_IDX];
    }
#if 0
    return everest_device_model_storage->get_variable_attribute(component_id, variable_id, attribute_enum);
#endif
    return std::nullopt;
}

std::vector<ocpp::v2::VariableAttribute>
ComposedDeviceModelStorage::get_variable_attributes(const ocpp::v2::Component& component_id,
                                                    const ocpp::v2::Variable& variable_id,
                                                    const std::optional<ocpp::v2::AttributeEnum>& attribute_enum) {
#if 0
	if (get_variable_source(component_id, variable_id) == VARIABLE_SOURCE_OCPP) {
        return libocpp_device_model_storage->get_variable_attributes(component_id, variable_id, attribute_enum);
    }

    return everest_device_model_storage->get_variable_attributes(component_id, variable_id, attribute_enum);
#endif
    std::vector<ocpp::v2::VariableAttribute> variableAttributeList;
    //todo:: extract attributes for variable_id from the device_model_map and push_back to variableAttributeList
    return variableAttributeList;
}

bool ComposedDeviceModelStorage::set_variable_attribute_value(const ocpp::v2::Component& component_id,
                                                              const ocpp::v2::Variable& variable_id,
                                                              const ocpp::v2::AttributeEnum& attribute_enum,
                                                              const std::string& value, const std::string& source) {
    if (get_variable_source(component_id, variable_id) == VARIABLE_SOURCE_OCPP) {
#if 0
        return libocpp_device_model_storage->set_variable_attribute_value(component_id, variable_id, attribute_enum,
                                                                          value, source);
#endif
        configPRINTF(("set_variable_attribute_value called but not implemented yet"));
        return true;
    }
#if 0
    return everest_device_model_storage->set_variable_attribute_value(component_id, variable_id, attribute_enum, value,
                                                                      source);
#endif
    configPRINTF(("set_variable_attribute_value called but not implemented yet"));
    return true;
}

std::optional<ocpp::v2::VariableMonitoringMeta>
ComposedDeviceModelStorage::set_monitoring_data(const ocpp::v2::SetMonitoringData& data,
                                                const ocpp::v2::VariableMonitorType type) {
#if 0
    return libocpp_device_model_storage->set_monitoring_data(data, type);
#endif
    return std::nullopt;
}

bool ComposedDeviceModelStorage::update_monitoring_reference(const int32_t monitor_id,
                                                             const std::string& reference_value) {
#if 0
    return libocpp_device_model_storage->update_monitoring_reference(monitor_id, reference_value);
#endif
    return true;
}

std::vector<ocpp::v2::VariableMonitoringMeta>
ComposedDeviceModelStorage::get_monitoring_data(const std::vector<ocpp::v2::MonitoringCriterionEnum>& criteria,
                                                const ocpp::v2::Component& component_id,
                                                const ocpp::v2::Variable& variable_id) {
#if 0
    if (get_variable_source(component_id, variable_id) == VARIABLE_SOURCE_OCPP) {
        return libocpp_device_model_storage->get_monitoring_data(criteria, component_id, variable_id);
    }

    return everest_device_model_storage->get_monitoring_data(criteria, component_id, variable_id);
#endif

    std::vector<ocpp::v2::VariableMonitoringMeta> monitoringMetaList;
    //todo extract monitoring data for variable with id variable_id from the device_model_map and push_back to monitoringMetaList
    return monitoringMetaList;

}

ocpp::v2::ClearMonitoringStatusEnum ComposedDeviceModelStorage::clear_variable_monitor(int monitor_id,
                                                                                       bool allow_protected) {
#if 0
	return libocpp_device_model_storage->clear_variable_monitor(monitor_id, allow_protected);
#endif
    return ocpp::v2::ClearMonitoringStatusEnum::Accepted;
}

int32_t ComposedDeviceModelStorage::clear_custom_variable_monitors() {
#if 0
    return libocpp_device_model_storage->clear_custom_variable_monitors();
#endif
    return 0;
}

void ComposedDeviceModelStorage::check_integrity() {
#if 0
    everest_device_model_storage->check_integrity();
    libocpp_device_model_storage->check_integrity();
#endif
    return;
}

std::string module::device_model::ComposedDeviceModelStorage::get_variable_source(const ocpp::v2::Component& component,
                                                                                  const ocpp::v2::Variable& variable) {
    std::optional<std::string> variable_source = device_model_map[component][variable].source;
    if (variable_source.has_value() && variable_source.value() != VARIABLE_SOURCE_OCPP) {
        // For now, this just throws because we only have the libocpp source. When the config service is
        // implemented, this should not throw.
        throw ocpp::v2::DeviceModelError("Source is not 'OCPP', not sure what to do");
    }

    return VARIABLE_SOURCE_OCPP;
}

} // namespace module::device_model





