/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright 2024-2025 NXP
 */

#ifndef PKCS11_NXP_H
#define PKCS11_NXP_H

#define CK_PTR *

#define CK_DECLARE_FUNCTION(returnType, name) returnType name

#define CK_DECLARE_FUNCTION_POINTER(returnType, name) returnType(*name)

#define CK_CALLBACK_FUNCTION(returnType, name) returnType(*name)

#ifndef NULL_PTR
#define NULL_PTR 0
#endif

#include "pkcs11.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define PKCS11_MAX_SESSIONS 10U
#define PKCS11_MAX_MECHANISMS 256U

/**
 * Constants representing PKCS11 slots for cryptographic operations.
 *
 * In the application, cryptographic operations may leverage two different
 * PKCS11 tokens and/or slots due to the limitations of secure element SE05x
 * The SE05x can only cipher a limited amount of data, and do not serve as
 * cryptographic accelerators, which results in slower operation speeds.
 * They are primarily utilized for TLS authentication with long-life credentials
 * stored within. To accommodate operations that exceed the SE's data handling
 * capabilities or require faster processing, these tasks are offloaded to
 * either a software (SW) PKCS11 provider or a more capable hardware (HW) token.
 * Accordingly, two constants are defined to indicate the specific slot that
 * should execute a given operation, aligning with PKCS11 proxy configuration:
 *
 * - PKCS11_SLOT1: Configured to execute ECC operations (ECDSA, ECDH) on SE05x.
 * - PKCS11_SLOT0: Configured to execute the reamining operations on the
 * Trusted Execution Environment (TEE).
 *
 * It is also possible to align both constants to the same token/slot if a single
 * token can efficiently handle all operations required by the application.
 *
 * Additionally, for instances requiring enhanced processing speed over security,
 * the slot index may be configured to point to SoftHSMv2.
 *
 *                     +--------------+
 *                     |PKCS11 Wrapper|
 *                     +------+-------+
 *                            |
 *                     +------+-------+
 *         +-----------+ p11-kit-proxy+-----------+
 *         |           +------+-------+           |
 *         |                  |                   |
 * +-------+--------+ +-------+---------+ +-------+--------+
 * |      TEE       | |      SE05x      | |     SW/CPU     |
 * +----------------+ +-----------------+ +----------------+
 *   libckteec.so.0    libsss_pkcs11.so     libsofthsm2.so
 */
#define PKCS11_SLOT0 0U /* Set to slot index of TEE */
#define PKCS11_SLOT1 1U /* Set to slot index of SE05x */

#define PKCS11_OBJECT_PREFIX "sss:"
#define PKCS11_OBJECT_PREFIX_LEN 4U
#define PKCS11_OBJECT_MAX_LABEL_LEN 16U

#define ECDSA_MAX_PUBLIC_KEY_SIZE 133U
#define ECDSA_MAX_SIGNATURE_RAW_SIZE 132U
#define ECDSA_MAX_SIGNATURE_DER_SIZE 220U

#define PKCS11_MAX_DIGEST_SIZE 64U
#define PKCS11_MAX_DATA_HASH_SIZE (4U * 1024U)

#define PKCS11_MAX_OID_LENGTH 32U

/* Function prototypes */
const char* CKR_TO_STR(CK_RV rvReturn);
CK_RV PKCS11_Initialize(const char* pchLibraryPath);
void PKCS11Functions_FinalizeOperations(void);
CK_SESSION_HANDLE PKCS11_GetSession(CK_SLOT_ID slotId);
CK_RV PKCS11_OpenSession(CK_SLOT_ID slotId, CK_SESSION_HANDLE_PTR phSession);
void PKCS11_CloseSession(CK_SESSION_HANDLE hSession);
void PKCS11_CloseSessions();
CK_SESSION_HANDLE PKCS11_GetSession(CK_SLOT_ID slotId);
void PKCS11_ReleaseSession(CK_SESSION_HANDLE hSession);
CK_RV PKCS11_ListSlots(bool bTokenPresent, CK_SLOT_ID_PTR pSlots,
                       CK_ULONG_PTR pulSlotCount);
CK_RV PKCS11_FindSlotByTokenName(const char* pchTokenName, CK_SLOT_ID* pSlotId);
CK_RV PKCS11_GetMechanisms(unsigned long ulIndex, CK_SLOT_ID slotId);
CK_BBOOL PKCS11_IsMechanismSupported(unsigned long ulIndex, CK_MECHANISM_TYPE mech);
/* Message-digesting operations */
CK_RV PKCS11_DigestInit(CK_SESSION_HANDLE hSession,
                        CK_MECHANISM_TYPE ckMechanismType);
CK_RV PKCS11_DigestUpdate(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pchPart,
                          CK_ULONG ulPartLen);
CK_RV PKCS11_DigestFinal(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pchDigest,
                         CK_ULONG_PTR pulDigestLen);
CK_RV PKCS11_Digest(CK_SESSION_HANDLE hSession,
                    CK_BYTE_PTR pchData, CK_ULONG ulDataLen,
                    CK_BYTE_PTR pchDigest, CK_ULONG_PTR pulDigestLen);
/* HMAC operations */
CK_RV PKCS11_HMAC_ImportKey(CK_SESSION_HANDLE hSession,
                            const uint8_t aucKey[], uint32_t uKeySize,
                            CK_OBJECT_HANDLE_PTR phKey);
CK_RV PKCS11_HMAC_Compute(CK_SESSION_HANDLE hSession,
                          CK_MECHANISM_TYPE ckMechanismType,
                          CK_OBJECT_HANDLE hKey, CK_BYTE_PTR pData,
                          CK_ULONG ulDataLen, CK_BYTE_PTR pSignature,
                          CK_ULONG_PTR pulSignatureSize);
CK_RV PKCS11_HMAC_Init(CK_SESSION_HANDLE hSession,
                       CK_MECHANISM_TYPE ckMechanismType,
                       CK_OBJECT_HANDLE hKey);
CK_RV PKCS11_HMAC_Update(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pPart,
                         CK_ULONG ulPartLen);
CK_RV PKCS11_HMAC_Final(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pSignature,
                        CK_ULONG_PTR pulSignatureSize);
/* RNG operations */
CK_RV PKCS11_RNG_Seed(CK_SESSION_HANDLE hSession, const uint8_t aucSeed[],
                      uint32_t ulSize);
CK_RV PKCS11_RNG_Read(CK_SESSION_HANDLE hSession, uint8_t ucMinData,
                      uint8_t aucData[], uint8_t* pucSize);

/* KDF operations */
CK_RV PKCS11_KDF_SingleStep(CK_SESSION_HANDLE hSession, CK_MECHANISM_TYPE hashAlgo,
                            CK_BYTE_PTR pSharedSecret, CK_ULONG ulSharedSecretLen,
                            CK_ULONG ulKeyDataLen, CK_BYTE_PTR pOtherInfo, CK_ULONG ulOtherInfoLen,
                            CK_BYTE_PTR pDerivedKey, CK_ULONG ulDerivedKeyLen);
CK_RV PKCS11_PBKDF1(CK_SESSION_HANDLE hSession, CK_MECHANISM_TYPE eHashMechanism,
                    CK_BYTE_PTR pPassword, CK_ULONG ulPasswordLen,
                    CK_BYTE_PTR pSalt, CK_ULONG ulSaltLen, CK_ULONG ulIterations,
                    CK_BYTE_PTR pDerivedKey, CK_ULONG_PTR pulDerivedKeyLen);
CK_RV PKCS11_HKDF_Extract(CK_SESSION_HANDLE hSession,
                          CK_MECHANISM_TYPE ckMechanismType,
                          CK_OBJECT_HANDLE hIKM,
                          CK_BYTE_PTR pSalt, CK_ULONG ulSaltLen,
                          CK_BYTE_PTR aucLabel,
                          CK_OBJECT_HANDLE_PTR phPRK);
CK_RV PKCS11_HKDF_Expand(CK_SESSION_HANDLE hSession,
                         CK_MECHANISM_TYPE ckMechanismType,
                         CK_OBJECT_HANDLE hPRK,
                         CK_BYTE_PTR pInfo, CK_ULONG ulInfoLen,
                         CK_BYTE_PTR aucLabel, CK_OBJECT_HANDLE_PTR phOutKey);
CK_RV PKCS11_HKDF_Derive(CK_SESSION_HANDLE hSession,
                         CK_MECHANISM_TYPE ckMechanismType,
                         CK_OBJECT_HANDLE hBaseKey,
                         CK_BYTE_PTR pSalt, CK_ULONG ulSaltLen,
                         CK_BYTE_PTR pInfo, CK_ULONG ulInfoLen,
                         CK_BYTE_PTR aucLabel, CK_OBJECT_HANDLE_PTR phOutKey);
/* Blockcipher */
CK_RV PKCS11_AES_ImportKey(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pchKey,
                           CK_ULONG ulKeyLength, CK_OBJECT_HANDLE_PTR phKey);
CK_RV PKCS11_AES_EncryptBlock(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE hKey,
                              const CK_BYTE_PTR pData, CK_ULONG dataLen,
                              CK_BYTE_PTR pEncryptedData,
                              CK_ULONG_PTR pEncryptedDataLen);
CK_RV PKCS11_AES_DecryptBlock(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE hKey,
                              const CK_BYTE_PTR pEncryptedData,
                              CK_ULONG encryptedDataLen, CK_BYTE_PTR pData,
                              CK_ULONG_PTR pDataLen);
CK_RV PKCS11_AES_CBC_EncryptInit(CK_SESSION_HANDLE hSession,
                                 CK_OBJECT_HANDLE hKey, CK_BYTE_PTR pIV,
                                 CK_ULONG ulIVLen, CK_BBOOL generateIV);
CK_RV PKCS11_AES_CBC_Encrypt(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pData,
                             CK_ULONG ulDataLen, CK_BYTE_PTR pEncryptedData,
                             CK_ULONG_PTR pulEncryptedDataLen);
CK_RV PKCS11_AES_CBC_DecryptInit(CK_SESSION_HANDLE hSession,
                                 CK_OBJECT_HANDLE hKey, CK_BYTE_PTR pIV,
                                 CK_ULONG ulIVLen);
CK_RV PKCS11_AES_CBC_Decrypt(CK_SESSION_HANDLE hSession,
                             CK_BYTE_PTR pEncryptedData,
                             CK_ULONG ulEncryptedDataLen,
                             CK_BYTE_PTR pDecryptedData,
                             CK_ULONG_PTR pulDecryptedDataLen);

/* ECC */
void PKCS11_EC_GetCurveInfoByOid(const uint8_t* pchOid, uint32_t ulOidLength,
                                 const char** ppchCurveName,
                                 uint32_t* ulCurveSizeBytes);
CK_RV PKCS11_EC_GenKeyPair(CK_SESSION_HANDLE hSession, const char* pchCurveName,
                           CK_BYTE_PTR aucLabel, CK_OBJECT_HANDLE_PTR phPublicKey,
			   CK_OBJECT_HANDLE_PTR phPrivateKey);
CK_RV PKCS11_EC_GetPublicKey(CK_SESSION_HANDLE hSession,
                             CK_OBJECT_HANDLE hPublicKey, uint8_t* pchValue,
                             uint32_t* pulValueLen);
CK_RV PKCS11_EC_ImportPublicKey(CK_SESSION_HANDLE hSession,
                                const char* pchCurveName,
                                const CK_BYTE_PTR pchPublicKey,
                                CK_ULONG ulPublicKeySize,
                                CK_OBJECT_HANDLE_PTR phPublicKey);
CK_RV PKCS11_EC_ImportPrivateKey(CK_SESSION_HANDLE hSession,
                                 const char* pchCurveName,
                                 CK_BYTE_PTR pchPrivateKey,
                                 CK_ULONG ulPrivateKeySize,
                                 CK_OBJECT_HANDLE* phPrivateKey);
CK_RV PKCS11_ECDH_DeriveKey(CK_SESSION_HANDLE hSession,
                            CK_OBJECT_HANDLE hPrivateKey,
                            CK_BYTE_PTR pPublicData, CK_ULONG ulPublicDataLen,
                            CK_ULONG ulSecretLen, CK_BYTE_PTR aucSecretLabel,
                            CK_OBJECT_HANDLE* phSharedSecret);
CK_RV PKCS11_EC_GetCurveOID(CK_SESSION_HANDLE hSession,
                            CK_OBJECT_HANDLE hPrivateKey, uint8_t* pchOid,
                            uint32_t* pulOidLength);

/* DH */
CK_RV PKCS11_DH_GenKeyPair(CK_SESSION_HANDLE hSession,
                           CK_OBJECT_HANDLE* phPrivKey,
                           CK_OBJECT_HANDLE* phPubKey, CK_BYTE* pchPrime,
                           CK_ULONG ulPrimeLen, CK_BYTE* pchGenerator,
                           CK_ULONG ulGeneratorLen);
CK_RV PKCS11_DH_DeriveKey(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE hPrivKey,
                          CK_BYTE_PTR pPublicData, CK_ULONG ulPublicDataLen,
                          CK_ULONG ulSecretLen, CK_OBJECT_HANDLE* phSecret);
CK_RV PKCS11_DH_ImportKey(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pchPrime,
                          CK_ULONG ulPrimeLen, CK_BYTE_PTR pchBase,
                          CK_ULONG ulBaseLen, CK_BYTE_PTR pchPrivKey,
                          CK_ULONG ulPrivKeyLen, CK_OBJECT_HANDLE* hPrivKey);

/* Signing */
CK_RV PKCS11_Signature_Generate(CK_SESSION_HANDLE hSession,
                                CK_OBJECT_HANDLE hPrivateKey,
                                CK_MECHANISM_TYPE ckMechanismType,
                                CK_BYTE_PTR pchData, CK_ULONG ulDataLen,
                                CK_BYTE_PTR pchSignature,
                                CK_ULONG_PTR pulSignatureLen);
CK_RV PKCS11_Signature_Verify(CK_SESSION_HANDLE hSession,
                              CK_OBJECT_HANDLE hPublicKey,
                              CK_MECHANISM_TYPE ckMechanismType,
                              CK_BYTE_PTR pchData, CK_ULONG ulDataLen,
                              CK_BYTE_PTR pchSignature,
                              CK_ULONG ulSignatureLen);
/* Utils */
CK_RV PKCS11_Object_Find(CK_SESSION_HANDLE hSession, CK_OBJECT_CLASS ckClass,
                         char const* aucKeyLabel, CK_ULONG ulSizeKeyLabelSize,
                         CK_OBJECT_HANDLE_PTR phKey);
CK_RV PKCS11_Object_GenerateLabel(char acOutputLabel[], uint32_t ulOutputLabelMaxSize);
CK_RV PKCS11_Object_SetLabel(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE_PTR hObject, char* aucLabel);
CK_RV PKCS11_Object_GetValue(CK_SESSION_HANDLE hSession,
                             CK_OBJECT_HANDLE hObject, uint8_t* pchValue,
                             uint32_t* pulValueLen);
CK_RV PKCS11_Object_Destroy(CK_SESSION_HANDLE hSession,
                            CK_OBJECT_HANDLE hObject);

bool nxpPKCS11_Init();
#endif // PKCS11_NXP_H
