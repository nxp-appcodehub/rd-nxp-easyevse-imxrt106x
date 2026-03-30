// SPDX-License-Identifier: Apache-2.0
// Copyright Pionix GmbH and Contributors to EVerest
// Copyright 2026 NXP

#include <evse_security/evse_security.hpp>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <optional>
#include <set>
#include <stdio.h>

#include <evse_security/certificate/x509_bundle.hpp>
#include <evse_security/certificate/x509_hierarchy.hpp>
#include <evse_security/certificate/x509_wrapper.hpp>
#include <evse_security/utils/evse_filesystem.hpp>

namespace evse_security {

static InstallCertificateResult to_install_certificate_result(CertificateValidationResult error) {
    switch (error) {
    case CertificateValidationResult::Valid:
        return InstallCertificateResult::Accepted;
    case CertificateValidationResult::Expired:
        return InstallCertificateResult::Expired;
    case CertificateValidationResult::InvalidSignature:
        return InstallCertificateResult::InvalidSignature;
    case CertificateValidationResult::InvalidChain:
        return InstallCertificateResult::InvalidCertificateChain;
    case CertificateValidationResult::InvalidLeafSignature:
        return InstallCertificateResult::InvalidSignature;
    case CertificateValidationResult::IssuerNotFound:
        return InstallCertificateResult::NoRootCertificateInstalled;
    default:
        return InstallCertificateResult::InvalidFormat;
    }
}

static std::vector<CaCertificateType> get_ca_certificate_types(const std::vector<CertificateType> certificate_types) {
    std::vector<CaCertificateType> ca_certificate_types;
    for (const auto& certificate_type : certificate_types) {
        if (certificate_type == CertificateType::V2GRootCertificate) {
            ca_certificate_types.push_back(CaCertificateType::V2G);
        }
        if (certificate_type == CertificateType::MORootCertificate) {
            ca_certificate_types.push_back(CaCertificateType::MO);
        }
        if (certificate_type == CertificateType::CSMSRootCertificate) {
            ca_certificate_types.push_back(CaCertificateType::CSMS);
        }
        if (certificate_type == CertificateType::MFRootCertificate) {
            ca_certificate_types.push_back(CaCertificateType::MF);
        }
    }
    return ca_certificate_types;
}

static CertificateType get_certificate_type(const CaCertificateType ca_certificate_type) {
    switch (ca_certificate_type) {
    case CaCertificateType::V2G:
        return CertificateType::V2GRootCertificate;
    case CaCertificateType::MO:
        return CertificateType::MORootCertificate;
    case CaCertificateType::CSMS:
        return CertificateType::CSMSRootCertificate;
    case CaCertificateType::MF:
        return CertificateType::MFRootCertificate;
    default:
        throw std::runtime_error("Could not convert CaCertificateType to CertificateType");
    }
}


std::mutex EvseSecurity::security_mutex;

EvseSecurity::EvseSecurity(const FilePaths& file_paths, const std::optional<std::string>& private_key_password,
                           const std::optional<std::uintmax_t>& max_fs_usage_bytes,
                           const std::optional<std::uintmax_t>& max_fs_certificate_store_entries,
                           const std::optional<std::chrono::seconds>& csr_expiry,
                           const std::optional<std::chrono::seconds>& garbage_collect_time) :
    private_key_password(private_key_password) {
    static_assert(sizeof(std::uint8_t) == 1, "uint8_t not equal to 1 byte!");

    std::vector<fs::path> dirs = {
        file_paths.directories.csms_leaf_cert_directory,
        file_paths.directories.csms_leaf_key_directory,
        file_paths.directories.secc_leaf_cert_directory,
        file_paths.directories.secc_leaf_key_directory,
    };

#if 0
    for (const auto& path : dirs) {
        if (!fs::exists(path)) {
            EVLOG_warning << "Could not find configured leaf directory at: " << path.string()
                          << " creating default dir!";
            if (!fs::create_directories(path)) {
                EVLOG_error << "Could not create default dir for path: " << path.string();
            }
        } else if (!fs::is_directory(path)) {
            throw std::runtime_error(path.string() + " is not a directory.");
        }
    }
#endif
    this->ca_bundle_path_map[CaCertificateType::CSMS] = file_paths.csms_ca_bundle;
    this->ca_bundle_path_map[CaCertificateType::MF] = file_paths.mf_ca_bundle;
    this->ca_bundle_path_map[CaCertificateType::MO] = file_paths.mo_ca_bundle;
    this->ca_bundle_path_map[CaCertificateType::V2G] = file_paths.v2g_ca_bundle;
#if 0
    for (const auto& pair : this->ca_bundle_path_map) {
        if (!fs::exists(pair.second)) {
            EVLOG_warning << "Could not find configured " << conversions::ca_certificate_type_to_string(pair.first)
                          << " bundle file at: " + pair.second.string() << ", creating default!";
            if (!filesystem_utils::create_file_if_nonexistent(pair.second)) {
                EVLOG_error << "Could not create default bundle for path: " << pair.second;
            }
        }
    }

    // Check that the leafs directory is not related to the bundle directory because
    // on garbage collect that can delete relevant CA certificates instead of leaf ones
    for (const auto& leaf_dir : dirs) {
        for (auto const& [certificate_type, ca_bundle_path] : ca_bundle_path_map) {
            if (ca_bundle_path == leaf_dir) {
                throw std::runtime_error(leaf_dir.string() +
                                         " leaf directory can not overlap CA directory: " + ca_bundle_path.string());
            }
        }
    }

    this->directories = file_paths.directories;
    this->links = file_paths.links;

    this->max_fs_usage_bytes = max_fs_usage_bytes.value_or(DEFAULT_MAX_FILESYSTEM_SIZE);
    this->max_fs_certificate_store_entries = max_fs_certificate_store_entries.value_or(DEFAULT_MAX_CERTIFICATE_ENTRIES);
    this->csr_expiry = csr_expiry.value_or(DEFAULT_CSR_EXPIRY);
    this->garbage_collect_time = garbage_collect_time.value_or(DEFAULT_GARBAGE_COLLECT_TIME);

    // Start GC timer
    garbage_collect_timer.interval([this]() { this->garbage_collect(); }, this->garbage_collect_time);
#endif
}

EvseSecurity::~EvseSecurity() {
}

InstallCertificateResult EvseSecurity::install_ca_certificate(const std::string& certificate,
                                                              CaCertificateType certificate_type) {
    std::lock_guard<std::mutex> guard(EvseSecurity::security_mutex);
    return InstallCertificateResult::Accepted;
}

DeleteCertificateResult EvseSecurity::delete_certificate(const CertificateHashData& certificate_hash_data) {
    std::lock_guard<std::mutex> guard(EvseSecurity::security_mutex);
    return DeleteCertificateResult::Accepted;

}

InstallCertificateResult EvseSecurity::update_leaf_certificate(const std::string& certificate_chain,
                                                               LeafCertificateType certificate_type) {

    std::lock_guard<std::mutex> guard(EvseSecurity::security_mutex);
    return InstallCertificateResult::Accepted;
}

GetInstalledCertificatesResult EvseSecurity::get_installed_certificate(CertificateType certificate_type) {
    return get_installed_certificates({certificate_type});
}

GetInstalledCertificatesResult
EvseSecurity::get_installed_certificates(const std::vector<CertificateType>& certificate_types) {
	GetInstalledCertificatesResult result;
    std::lock_guard<std::mutex> guard(EvseSecurity::security_mutex);
    return result;
}

int EvseSecurity::get_count_of_installed_certificates(const std::vector<CertificateType>& certificate_types) {
    std::lock_guard<std::mutex> guard(EvseSecurity::security_mutex);
    return 0;
}

OCSPRequestDataList EvseSecurity::get_v2g_ocsp_request_data() {
    std::lock_guard<std::mutex> guard(EvseSecurity::security_mutex);
    return OCSPRequestDataList();
}

OCSPRequestDataList EvseSecurity::get_mo_ocsp_request_data(const std::string& certificate_chain) {
    std::lock_guard<std::mutex> guard(EvseSecurity::security_mutex);

    // Test for both MO and V2G roots
    return generate_ocsp_request_data_internal({CaCertificateType::V2G, CaCertificateType::MO}, certificate_chain);
}

OCSPRequestDataList EvseSecurity::generate_ocsp_request_data_internal(const std::set<CaCertificateType>& possible_roots,
                                                                      const std::string& leaf_chain) {
    OCSPRequestDataList response;
    return response;
}

void EvseSecurity::update_ocsp_cache(const CertificateHashData& certificate_hash_data,
                                     const std::string& ocsp_response) {
     return;
}

std::optional<fs::path> EvseSecurity::retrieve_ocsp_cache(const CertificateHashData& certificate_hash_data) {
    std::lock_guard<std::mutex> guard(EvseSecurity::security_mutex);
    return retrieve_ocsp_cache_internal(certificate_hash_data);
}

std::optional<fs::path> EvseSecurity::retrieve_ocsp_cache_internal(const CertificateHashData& certificate_hash_data) {
    return std::nullopt;
}

bool EvseSecurity::is_ca_certificate_installed(CaCertificateType certificate_type) {
#if !EASYEVSE
    std::lock_guard<std::mutex> guard(EvseSecurity::security_mutex);
#endif
    return is_ca_certificate_installed_internal(certificate_type);
}

bool EvseSecurity::is_ca_certificate_installed_internal(CaCertificateType certificate_type) {
#if EASYEVSE
	if (certificate_type == CaCertificateType::CSMS)
	{
		return true; // ca certificate is installed
	}
	return false;
#else
    return false;
#endif
}

void EvseSecurity::certificate_signing_request_failed(const std::string& csr, LeafCertificateType certificate_type) {
    // TODO(ioan): delete the pairing key of the CSR
}

GetCertificateSignRequestResult
EvseSecurity::generate_certificate_signing_request_internal(LeafCertificateType certificate_type,
                                                            const CertificateSigningRequestInfo& info) {
    GetCertificateSignRequestResult result{};
    return result;
}

GetCertificateSignRequestResult EvseSecurity::generate_certificate_signing_request(LeafCertificateType certificate_type,
                                                                                   const std::string& country,
                                                                                   const std::string& organization,
                                                                                   const std::string& common,
                                                                                   bool use_custom_provider) {
	std::lock_guard<std::mutex> guard(EvseSecurity::security_mutex);
	GetCertificateSignRequestResult result{};
	result.status = GetCertificateSignRequestStatus::InvalidRequestedType;

	return result;
}

GetCertificateSignRequestResult EvseSecurity::generate_certificate_signing_request(LeafCertificateType certificate_type,
                                                                                   const std::string& country,
                                                                                   const std::string& organization,
                                                                                   const std::string& common) {
    return generate_certificate_signing_request(certificate_type, country, organization, common, false);
}

GetCertificateFullInfoResult EvseSecurity::get_all_valid_certificates_info(LeafCertificateType certificate_type,
                                                                           EncodingFormat encoding, bool include_ocsp) {
    std::lock_guard<std::mutex> guard(EvseSecurity::security_mutex);

    GetCertificateFullInfoResult result =
        get_full_leaf_certificate_info_internal({certificate_type, encoding, include_ocsp, true, true});

        return result;
}

GetCertificateInfoResult EvseSecurity::get_leaf_certificate_info(LeafCertificateType certificate_type,
                                                                 EncodingFormat encoding, bool include_ocsp) {
    std::lock_guard<std::mutex> guard(EvseSecurity::security_mutex);

    return get_leaf_certificate_info_internal(certificate_type, encoding, include_ocsp);
}

GetCertificateInfoResult EvseSecurity::get_leaf_certificate_info_internal(LeafCertificateType certificate_type,
                                                                          EncodingFormat encoding, bool include_ocsp) {
    GetCertificateFullInfoResult result =
        get_full_leaf_certificate_info_internal({certificate_type, encoding, include_ocsp, false, false});
    GetCertificateInfoResult internal_result;

    internal_result.status = result.status;
    if (!result.info.empty()) {
        internal_result.info = std::move(result.info.at(0));
    }

    return internal_result;
}

GetCertificateFullInfoResult
EvseSecurity::get_full_leaf_certificate_info_internal(const CertificateQueryParams& params) {
    auto certificate_type = params.certificate_type;

    GetCertificateFullInfoResult result;
    result.status = GetCertificateInfoStatus::NotFound;
    return result;
}

bool EvseSecurity::update_certificate_links(LeafCertificateType certificate_type) {
    bool changed = false;
    return changed;
}

GetCertificateInfoResult EvseSecurity::get_ca_certificate_info_internal(CaCertificateType certificate_type) {
    GetCertificateInfoResult result{};
    result.status = GetCertificateInfoStatus::NotFound;
    return result;
}

GetCertificateInfoResult EvseSecurity::get_ca_certificate_info(CaCertificateType certificate_type) {
    std::lock_guard<std::mutex> guard(EvseSecurity::security_mutex);

    return get_ca_certificate_info_internal(certificate_type);
}

std::string EvseSecurity::get_verify_file(CaCertificateType certificate_type) {
    std::lock_guard<std::mutex> guard(EvseSecurity::security_mutex);
    return {};
}

std::string EvseSecurity::get_verify_location(CaCertificateType certificate_type) {

    std::lock_guard<std::mutex> guard(EvseSecurity::security_mutex);
    return {};
}

int EvseSecurity::get_leaf_expiry_days_count(LeafCertificateType certificate_type) {
    std::lock_guard<std::mutex> guard(EvseSecurity::security_mutex);
    return 0;
}

bool EvseSecurity::verify_file_signature(const fs::path& path, const std::string& signing_certificate,
                                         const std::string signature) {
    std::lock_guard<std::mutex> guard(EvseSecurity::security_mutex);
    return false;
}

std::vector<std::uint8_t> EvseSecurity::base64_decode_to_bytes(const std::string& base64_string) {
    std::vector<std::uint8_t> decoded_bytes;
    return decoded_bytes;
}

std::string EvseSecurity::base64_decode_to_string(const std::string& base64_string) {
    std::string decoded_string;
    return decoded_string;
}

std::string EvseSecurity::base64_encode_from_bytes(const std::vector<std::uint8_t>& bytes) {
    std::string encoded_string;
    return encoded_string;
}

std::string EvseSecurity::base64_encode_from_string(const std::string& string) {
    std::string encoded_string;
    return encoded_string;
}

CertificateValidationResult EvseSecurity::verify_certificate(const std::string& certificate_chain,
                                                             LeafCertificateType certificate_type) {
    std::lock_guard<std::mutex> guard(EvseSecurity::security_mutex);
    return verify_certificate_internal(certificate_chain, {certificate_type});
}

CertificateValidationResult
EvseSecurity::verify_certificate(const std::string& certificate_chain,
                                 const std::vector<LeafCertificateType>& certificate_types) {
    std::lock_guard<std::mutex> guard(EvseSecurity::security_mutex);
    return verify_certificate_internal(certificate_chain, certificate_types);
}

CertificateValidationResult
EvseSecurity::verify_certificate_internal(const std::string& certificate_chain,
                                          const std::vector<LeafCertificateType>& certificate_types) {

        return CertificateValidationResult::Unknown;

}

void EvseSecurity::garbage_collect() {
    std::lock_guard<std::mutex> guard(EvseSecurity::security_mutex);
    return;
}

bool EvseSecurity::is_filesystem_full() {
	return false;
}

} // namespace evse_security





