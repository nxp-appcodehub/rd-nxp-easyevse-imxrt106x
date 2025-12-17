// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright 2024-2025 NXP
 */
#if ENABLE_ISO15118
#if (PKCS11_SUPPORTED == 1)

#include "features_stx.h"

#if (DEBUG_STX != 0)
#define STX_MOD_ID eSTXMOD_ID_MODULES
#define MOD_DBG_LVL MODULES_DBG_LVL
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug_stx.h"
#include "pkcs11_nxp.h"
#include "runtimelib.h"
#include "stxtypes.h"

static CK_SESSION_HANDLE g_ahSession[PKCS11_MAX_SESSIONS] = {CK_INVALID_HANDLE};

static CK_MECHANISM_TYPE* g_aMechanisms[PKCS11_MAX_SESSIONS][PKCS11_MAX_MECHANISMS];
static CK_ULONG g_ulMechCount[PKCS11_MAX_SESSIONS] = {0};

/* Pointer to the PKCS#11 function list */
CK_FUNCTION_LIST_PTR g_pCKFunctionList = NULL;
/* User's PIN */
static const char* g_pchUserPin = NULL;
/* Name of the cryptographic token */
static const char* g_pchTokenName = NULL;

static const uint8_t* EC_CRUVE_NAME_2_OID(const char* pchCurveName, uint32_t* pulOidLen);

/**
 * Converts a PKCS#11 return value to its string representation.
 *
 * This function takes a PKCS#11 return value (CK_RV) and converts it into its string representation.
 * It's a utility function that can be helpful for debugging and error reporting purposes.
 *
 * @param rvReturn The PKCS#11 return value to be converted to a string.
 *
 * @return const char* A pointer to a constant character string representing the string representation
 *                    of the provided PKCS#11 return value.
 *
 * The function returns a pointer to a constant string corresponding to the provided PKCS#11 return value.
 * The string representation can be used for logging or displaying error messages.
 */
const char* CKR_TO_STR(CK_RV rvReturn)
{
    switch (rvReturn)
    {
    case CKR_OK:
        return "CKR_OK";
    case CKR_CANCEL:
        return "CKR_CANCEL";
    case CKR_HOST_MEMORY:
        return "CKR_HOST_MEMORY";
    case CKR_SLOT_ID_INVALID:
        return "CKR_SLOT_ID_INVALID";
    case CKR_GENERAL_ERROR:
        return "CKR_GENERAL_ERROR";
    case CKR_FUNCTION_FAILED:
        return "CKR_FUNCTION_FAILED";
    case CKR_ARGUMENTS_BAD:
        return "CKR_ARGUMENTS_BAD";
    case CKR_NO_EVENT:
        return "CKR_NO_EVENT";
    case CKR_NEED_TO_CREATE_THREADS:
        return "CKR_NEED_TO_CREATE_THREADS";
    case CKR_CANT_LOCK:
        return "CKR_CANT_LOCK";
    case CKR_ATTRIBUTE_READ_ONLY:
        return "CKR_ATTRIBUTE_READ_ONLY";
    case CKR_ATTRIBUTE_SENSITIVE:
        return "CKR_ATTRIBUTE_SENSITIVE";
    case CKR_ATTRIBUTE_TYPE_INVALID:
        return "CKR_ATTRIBUTE_TYPE_INVALID";
    case CKR_ATTRIBUTE_VALUE_INVALID:
        return "CKR_ATTRIBUTE_VALUE_INVALID";
    case CKR_ACTION_PROHIBITED:
        return "CKR_ACTION_PROHIBITED";
    case CKR_DATA_INVALID:
        return "CKR_DATA_INVALID";
    case CKR_DATA_LEN_RANGE:
        return "CKR_DATA_LEN_RANGE";
    case CKR_DEVICE_ERROR:
        return "CKR_DEVICE_ERROR";
    case CKR_DEVICE_MEMORY:
        return "CKR_DEVICE_MEMORY";
    case CKR_DEVICE_REMOVED:
        return "CKR_DEVICE_REMOVED";
    case CKR_ENCRYPTED_DATA_INVALID:
        return "CKR_ENCRYPTED_DATA_INVALID";
    case CKR_ENCRYPTED_DATA_LEN_RANGE:
        return "CKR_ENCRYPTED_DATA_LEN_RANGE";
    case CKR_FUNCTION_CANCELED:
        return "CKR_FUNCTION_CANCELED";
    case CKR_FUNCTION_NOT_PARALLEL:
        return "CKR_FUNCTION_NOT_PARALLEL";
    case CKR_FUNCTION_NOT_SUPPORTED:
        return "CKR_FUNCTION_NOT_SUPPORTED";
    case CKR_KEY_HANDLE_INVALID:
        return "CKR_KEY_HANDLE_INVALID";
    case CKR_KEY_SIZE_RANGE:
        return "CKR_KEY_SIZE_RANGE";
    case CKR_KEY_TYPE_INCONSISTENT:
        return "CKR_KEY_TYPE_INCONSISTENT";
    case CKR_KEY_NOT_NEEDED:
        return "CKR_KEY_NOT_NEEDED";
    case CKR_KEY_CHANGED:
        return "CKR_KEY_CHANGED";
    case CKR_KEY_NEEDED:
        return "CKR_KEY_NEEDED";
    case CKR_KEY_INDIGESTIBLE:
        return "CKR_KEY_INDIGESTIBLE";
    case CKR_KEY_FUNCTION_NOT_PERMITTED:
        return "CKR_KEY_FUNCTION_NOT_PERMITTED";
    case CKR_KEY_NOT_WRAPPABLE:
        return "CKR_KEY_NOT_WRAPPABLE";
    case CKR_KEY_UNEXTRACTABLE:
        return "CKR_KEY_UNEXTRACTABLE";
    case CKR_MECHANISM_INVALID:
        return "CKR_MECHANISM_INVALID";
    case CKR_MECHANISM_PARAM_INVALID:
        return "CKR_MECHANISM_PARAM_INVALID";
    case CKR_OBJECT_HANDLE_INVALID:
        return "CKR_OBJECT_HANDLE_INVALID";
    case CKR_OPERATION_ACTIVE:
        return "CKR_OPERATION_ACTIVE";
    case CKR_OPERATION_NOT_INITIALIZED:
        return "CKR_OPERATION_NOT_INITIALIZED";
    case CKR_PIN_INCORRECT:
        return "CKR_PIN_INCORRECT";
    case CKR_PIN_INVALID:
        return "CKR_PIN_INVALID";
    case CKR_PIN_LEN_RANGE:
        return "CKR_PIN_LEN_RANGE";
    case CKR_PIN_EXPIRED:
        return "CKR_PIN_EXPIRED";
    case CKR_PIN_LOCKED:
        return "CKR_PIN_LOCKED";
    case CKR_SESSION_CLOSED:
        return "CKR_SESSION_CLOSED";
    case CKR_SESSION_COUNT:
        return "CKR_SESSION_COUNT";
    case CKR_SESSION_HANDLE_INVALID:
        return "CKR_SESSION_HANDLE_INVALID";
    case CKR_SESSION_PARALLEL_NOT_SUPPORTED:
        return "CKR_SESSION_PARALLEL_NOT_SUPPORTED";
    case CKR_SESSION_READ_ONLY:
        return "CKR_SESSION_READ_ONLY";
    case CKR_SESSION_EXISTS:
        return "CKR_SESSION_EXISTS";
    case CKR_SESSION_READ_ONLY_EXISTS:
        return "CKR_SESSION_READ_ONLY_EXISTS";
    case CKR_SESSION_READ_WRITE_SO_EXISTS:
        return "CKR_SESSION_READ_WRITE_SO_EXISTS";
    case CKR_SIGNATURE_INVALID:
        return "CKR_SIGNATURE_INVALID";
    case CKR_SIGNATURE_LEN_RANGE:
        return "CKR_SIGNATURE_LEN_RANGE";
    case CKR_TEMPLATE_INCOMPLETE:
        return "CKR_TEMPLATE_INCOMPLETE";
    case CKR_TEMPLATE_INCONSISTENT:
        return "CKR_TEMPLATE_INCONSISTENT";
    case CKR_TOKEN_NOT_PRESENT:
        return "CKR_TOKEN_NOT_PRESENT";
    case CKR_TOKEN_NOT_RECOGNIZED:
        return "CKR_TOKEN_NOT_RECOGNIZED";
    case CKR_TOKEN_WRITE_PROTECTED:
        return "CKR_TOKEN_WRITE_PROTECTED";
    case CKR_UNWRAPPING_KEY_HANDLE_INVALID:
        return "CKR_UNWRAPPING_KEY_HANDLE_INVALID";
    case CKR_UNWRAPPING_KEY_SIZE_RANGE:
        return "CKR_UNWRAPPING_KEY_SIZE_RANGE";
    case CKR_UNWRAPPING_KEY_TYPE_INCONSISTENT:
        return "CKR_UNWRAPPING_KEY_TYPE_INCONSISTENT";
    case CKR_USER_ALREADY_LOGGED_IN:
        return "CKR_USER_ALREADY_LOGGED_IN";
    case CKR_USER_NOT_LOGGED_IN:
        return "CKR_USER_NOT_LOGGED_IN";
    case CKR_USER_PIN_NOT_INITIALIZED:
        return "CKR_USER_PIN_NOT_INITIALIZED";
    case CKR_USER_TYPE_INVALID:
        return "CKR_USER_TYPE_INVALID";
    case CKR_USER_ANOTHER_ALREADY_LOGGED_IN:
        return "CKR_USER_ANOTHER_ALREADY_LOGGED_IN";
    case CKR_USER_TOO_MANY_TYPES:
        return "CKR_USER_TOO_MANY_TYPES";
    case CKR_WRAPPED_KEY_INVALID:
        return "CKR_WRAPPED_KEY_INVALID";
    case CKR_WRAPPED_KEY_LEN_RANGE:
        return "CKR_WRAPPED_KEY_LEN_RANGE";
    case CKR_WRAPPING_KEY_HANDLE_INVALID:
        return "CKR_WRAPPING_KEY_HANDLE_INVALID";
    case CKR_WRAPPING_KEY_SIZE_RANGE:
        return "CKR_WRAPPING_KEY_SIZE_RANGE";
    case CKR_WRAPPING_KEY_TYPE_INCONSISTENT:
        return "CKR_WRAPPING_KEY_TYPE_INCONSISTENT";
    case CKR_RANDOM_SEED_NOT_SUPPORTED:
        return "CKR_RANDOM_SEED_NOT_SUPPORTED";
    case CKR_RANDOM_NO_RNG:
        return "CKR_RANDOM_NO_RNG";
    case CKR_DOMAIN_PARAMS_INVALID:
        return "CKR_DOMAIN_PARAMS_INVALID";
    case CKR_CURVE_NOT_SUPPORTED:
        return "CKR_CURVE_NOT_SUPPORTED";
    case CKR_BUFFER_TOO_SMALL:
        return "CKR_BUFFER_TOO_SMALL";
    case CKR_SAVED_STATE_INVALID:
        return "CKR_SAVED_STATE_INVALID";
    case CKR_INFORMATION_SENSITIVE:
        return "CKR_INFORMATION_SENSITIVE";
    case CKR_STATE_UNSAVEABLE:
        return "CKR_STATE_UNSAVEABLE";
    case CKR_CRYPTOKI_NOT_INITIALIZED:
        return "CKR_CRYPTOKI_NOT_INITIALIZED";
    case CKR_CRYPTOKI_ALREADY_INITIALIZED:
        return "CKR_CRYPTOKI_ALREADY_INITIALIZED";
    case CKR_MUTEX_BAD:
        return "CKR_MUTEX_BAD";
    case CKR_MUTEX_NOT_LOCKED:
        return "CKR_MUTEX_NOT_LOCKED";
    case CKR_NEW_PIN_MODE:
        return "CKR_NEW_PIN_MODE";
    case CKR_NEXT_OTP:
        return "CKR_NEXT_OTP";
    case CKR_EXCEEDED_MAX_ITERATIONS:
        return "CKR_EXCEEDED_MAX_ITERATIONS";
    case CKR_FIPS_SELF_TEST_FAILED:
        return "CKR_FIPS_SELF_TEST_FAILED";
    case CKR_LIBRARY_LOAD_FAILED:
        return "CKR_LIBRARY_LOAD_FAILED";
    case CKR_PIN_TOO_WEAK:
        return "CKR_PIN_TOO_WEAK";
    case CKR_PUBLIC_KEY_INVALID:
        return "CKR_PUBLIC_KEY_INVALID";
    case CKR_FUNCTION_REJECTED:
        return "CKR_FUNCTION_REJECTED";
    default:
        if (rvReturn & CKR_VENDOR_DEFINED)
        {
            return "Vendor defined";
        }
        else
        {
            return "Unknown ID";
        }
    }
}

/**
 * Initializes the PKCS#11 library.
 *
 * This function loads and initializes the PKCS#11 library located at the specified path. It should be
 * called before performing any cryptographic operations using PKCS#11. This is typically the first
 * function invoked in the application's PKCS#11 workflow.
 *
 * @param pchLibraryPath Pointer to a constant character string representing the file system path to the
 *                       PKCS#11 library. This library contains the implementation of the PKCS#11 interface.
 *
 * @return CK_RV Return value as defined by the PKCS#11 standard. Common return values include CKR_OK
 *               if the initialization is successful, CKR_ARGUMENTS_BAD if the input path is invalid, and
 *               other PKCS#11-specific error codes for various failure scenarios.
 *
 * The function verifies the validity of the provided path and attempts to load and initialize the library.
 * In case of failure, it returns an appropriate error code based on the nature of the failure.
 */
CK_RV PKCS11_Initialize(const char* pchLibraryPath)
{
    CK_RV rvReturn = CKR_GENERAL_ERROR;
    CK_C_INITIALIZE_ARGS sArgs;

    	rvReturn = C_GetFunctionList(&g_pCKFunctionList);
        if (rvReturn == CKR_OK)
        {
            (void)ZEROMEMORY(&sArgs, sizeof(sArgs));
            sArgs.flags = CKF_OS_LOCKING_OK;

            if (g_pCKFunctionList != NULL && g_pCKFunctionList->C_Initialize != NULL)
            {
                rvReturn = g_pCKFunctionList->C_Initialize(&sArgs);
                if (rvReturn == CKR_OK)
                {
                    DBGPRINT_INFO("Initialized");
                }
                else
                {
                    DBGPRINTF_ERROR("C_Initialize() failed 0x%lx:(%s)", rvReturn,
                                    CKR_TO_STR(rvReturn));
                }
            }
            else
            {
                rvReturn = CKR_FUNCTION_FAILED;
            }
        }

    return rvReturn;
}

/**
 * Finalizes operations related to PKCS#11 functions.
 *
 * This function is responsible for cleaning up and releasing resources associated with
 * PKCS#11 operations. It should be called when an application is done using the PKCS#11
 * module, usually at application shutdown or after all cryptographic operations are complete.
 *
 * The function does not take any parameters and does not return a value. It performs all
 * necessary internal cleanup tasks for the PKCS#11 functions that have been used.
 *
 * Note: It is important to call this function to prevent resource leaks and ensure that
 * all PKCS#11-related resources are properly released before the application terminates.
 */
void PKCS11Functions_FinalizeOperations(void)
{
    if (g_pCKFunctionList != NULL)
    {
        if (g_pCKFunctionList->C_Finalize != NULL)
        {
            CK_RV rvReturn = g_pCKFunctionList->C_Finalize(NULL);

            if (rvReturn != CKR_OK)
            {
                DBGPRINTF_WARN("C_Finalize() failed 0x%lx:(%s)", rvReturn,
                               CKR_TO_STR(rvReturn));
            }
        }
    }
}

/**
 * Retrieves a slot identifier from the PKCS#11 module.
 *
 * @param pSlotID Pointer to CK_SLOT_ID to store the identifier of the retrieved slot.
 *                This identifier is used for further PKCS#11 operations related to the slot.
 *
 * @return CK_RV Return value indicating the operation result. Common values are CKR_OK for success,
 *               CKR_ARGUMENTS_BAD for invalid input, and other PKCS#11-specific error codes.
 *
 * The function checks if pSlotID is NULL and returns CKR_ARGUMENTS_BAD in that case. On success,
 * it stores the slot identifier in pSlotID and returns CKR_OK. Failure cases, like unavailable slots,
 * result in appropriate PKCS#11 error codes.
 */
CK_RV PKCS11_GetSlot(CK_SLOT_ID* pSlotID)
{
    CK_RV rvReturn = CKR_GENERAL_ERROR;

    if (pSlotID != NULL)
    {
        if (g_pCKFunctionList != NULL && g_pCKFunctionList->C_GetSlotList != NULL)
        {
            CK_ULONG ulSlotCount = 1U;
            rvReturn = g_pCKFunctionList->C_GetSlotList(CK_TRUE, pSlotID, &ulSlotCount);

            if (rvReturn == CKR_OK)
            {
                if (ulSlotCount == 0U)
                {
                    rvReturn = CKR_SLOT_ID_INVALID;
                }
            }
            else
            {
                DBGPRINTF_WARN("C_GetSlotList() failed 0x%lx:(%s)", rvReturn,
                               CKR_TO_STR(rvReturn));
            }
        }
        else
        {
            rvReturn = CKR_FUNCTION_FAILED;
        }
    }
    else
    {
        rvReturn = CKR_ARGUMENTS_BAD;
    }

    return rvReturn;
}

/**
 * Retrieves the supported mechanisms for a given slot and stores them in a global array.
 *
 * @param ulIndex The index of the slot to query.
 * @param slotId The slot identifier.
 *
 * @return CKR_OK on success, or an error code on failure.
 */
CK_RV PKCS11_GetMechanisms(unsigned long ulIndex, CK_SLOT_ID slotId)
{
    CK_RV rvReturn = CKR_FUNCTION_FAILED;
    CK_ULONG ulMechCount = 0;

    if ((g_pCKFunctionList == NULL) || (g_pCKFunctionList->C_GetMechanismList == NULL))
    {
    }
    else if (ulIndex >= PKCS11_MAX_SESSIONS)
    {
        rvReturn = CKR_ARGUMENTS_BAD;
    }
    else
    {
        if (g_ulMechCount[ulIndex] == 0U)
        {
            rvReturn = g_pCKFunctionList->C_GetMechanismList(slotId, NULL, &ulMechCount);
            if ((rvReturn == CKR_OK) && (ulMechCount > 0U))
            {
                if (ulMechCount > PKCS11_MAX_MECHANISMS)
                {
                    ulMechCount = PKCS11_MAX_MECHANISMS;
                }

                rvReturn = g_pCKFunctionList->C_GetMechanismList(slotId, (CK_MECHANISM_TYPE_PTR)g_aMechanisms[ulIndex], &ulMechCount);
                if (rvReturn == CKR_OK)
                {
                    g_ulMechCount[ulIndex] = ulMechCount;
                }
                else
                {
                    DBGPRINTF_ERROR("C_GetMechanismList() failed 0x%lx:(%s)", rvReturn,
                                    CKR_TO_STR(rvReturn));
                }
            }
        }
        else
        {
            rvReturn = CKR_OK;
        }
    }

    return rvReturn;
}

/**
 * Checks if a given mechanism is supported by a specific slot.
 *
 * @param ulIndex The index of the slot.
 * @param ckMech The mechanism type to check.
 *
 * @return CK_TRUE (1) if supported, CK_FALSE (0) if not supported.
 */
CK_BBOOL PKCS11_IsMechanismSupported(unsigned long ulIndex, CK_MECHANISM_TYPE ckMech)
{
    CK_BBOOL bSupported = CK_FALSE;
    CK_ULONG i = 0;

    if (ulIndex < PKCS11_MAX_SESSIONS)
    {
        for (i = 0U; i < g_ulMechCount[ulIndex]; i++)
        {
            if ((CK_MECHANISM_TYPE)g_aMechanisms[ulIndex][i] == ckMech)
            {
                bSupported = CK_TRUE;
            }
        }
    }

    return bSupported;
}

/**
 * Opens a session with the PKCS#11 module.
 *
 * This function is responsible for establishing a session between the
 * application and the PKCS#11 module, allowing the application to perform
 * cryptographic operations. Opening a session is a prerequisite for most
 * PKCS#11 operations as it sets up the necessary context and resources. The
 * function allocates and initializes a session handle, which is used in
 * subsequent PKCS#11 function calls to identify the session.
 *
 * @param slotId Slot ID from PKCS#11 token.
 * @param phSession A pointer to a CK_SESSION_HANDLE where the function
 * will store the handle of the newly opened session.
 *
 * @return CK_RV A return value of type CK_RV as defined by the PKCS#11
 * standard. It indicates the success or failure of the operation. Common return
 * values include CKR_OK for success, CKR_ARGUMENTS_BAD if the input argument is
 * invalid, and other PKCS#11-specific error codes as applicable.
 *
 * The function checks if the input argument (phSession) is a valid pointer
 * and returns CKR_ARGUMENTS_BAD if it is NULL. On successfully opening a
 * session, it stores the session handle in the location pointed to by
 * phSession and returns CKR_OK. If the session cannot be opened due to
 * limitations like maximum session count, it returns an appropriate error code
 * defined by the PKCS#11 standard.
 *
 * It's important for the caller to close the session using PKCS11_CloseSession
 * once the session is no longer needed, to free up resources and maintain
 * security.
 */
CK_RV PKCS11_OpenSession(CK_SLOT_ID slotId, CK_SESSION_HANDLE_PTR phSession)
{
    CK_RV rvReturn = CKR_FUNCTION_FAILED;
    CK_SESSION_HANDLE hSession = CK_INVALID_HANDLE;

    if ((g_pchUserPin != NULL) && (phSession != NULL))
    {
        if (g_pCKFunctionList != NULL &&
            g_pCKFunctionList->C_OpenSession != NULL &&
            g_pCKFunctionList->C_Login != NULL)
        {
            rvReturn = g_pCKFunctionList->C_OpenSession(
                slotId, CKF_SERIAL_SESSION | CKF_RW_SESSION, NULL, NULL,
                &hSession);

            if (rvReturn == CKR_OK)
            {
                CK_ULONG ulPinLength = (CK_ULONG)strlen((const char*)g_pchUserPin);
                rvReturn = g_pCKFunctionList->C_Login(hSession, CKU_USER,
                                                      (CK_CHAR_PTR)g_pchUserPin,
                                                      ulPinLength);
                if ((rvReturn == CKR_OK) || (rvReturn == CKR_USER_ALREADY_LOGGED_IN))
                {
                    *phSession = hSession;
                    rvReturn = CKR_OK;
                }
                else
                {
                    DBGPRINTF_ERROR("C_Login() failed 0x%lx:(%s)", rvReturn,
                                    CKR_TO_STR(rvReturn));
                }
            }
            else
            {
                DBGPRINTF_ERROR("C_OpenSession() failed 0x%lx:(%s)", rvReturn,
                                CKR_TO_STR(rvReturn));
            }
        }
        else
        {
            rvReturn = CKR_FUNCTION_FAILED;
        }
    }
    else
    {
        rvReturn = CKR_ARGUMENTS_BAD;
    }

    return rvReturn;
}

/**
 * Closes a session with the PKCS#11 module.
 *
 * This function is used to terminate an open session between the application
 * and the PKCS#11 module. Closing a session is essential for maintaining the
 * security integrity of the cryptographic operations and for ensuring that all
 * resources associated with the session are properly released. This function
 * should be called after all necessary cryptographic operations within a
 * session are completed.
 *
 * @param sessionHandle A CK_SESSION_HANDLE that identifies the session to be
 * closed. This handle is typically obtained from a previous call to a function
 * that opens a session (e.g., PKCS11_OpenSession).
 *
 * The function does not return a value. In case of an error during session
 * closure, it is logged.
 *
 * It is important for applications to call this function for every opened
 * session to prevent resource leaks and potential security vulnerabilities.
 */
void PKCS11_CloseSession(CK_SESSION_HANDLE hSession)
{
    if (hSession != CK_INVALID_HANDLE)
    {
        CK_RV rvReturn = CKR_FUNCTION_FAILED;

        if (g_pCKFunctionList != NULL_PTR)
        {
            if (g_pCKFunctionList->C_Logout != NULL_PTR)
            {
                rvReturn = g_pCKFunctionList->C_Logout(hSession);
                if (rvReturn != CKR_OK)
                {
                    DBGPRINTF_WARN("C_Logout() failed 0x%lx:(%s)", rvReturn,
                                   CKR_TO_STR(rvReturn));
                }
            }

            if (g_pCKFunctionList->C_CloseSession != NULL_PTR)
            {
                rvReturn = g_pCKFunctionList->C_CloseSession(hSession);
                if (rvReturn != CKR_OK)
                {
                    DBGPRINTF_WARN("C_CloseSession() failed 0x%lx:(%s)", rvReturn,
                                   CKR_TO_STR(rvReturn));
                }
            }
        }
    }
}

/**
 * This function attempts to open a new session with the PKCS#11 token. It ensures
 * that a session is available for use in cryptographic operations. If the token
 * supports only a single session or if session management is implemented to restrict
 * it to a single session, this function will manage the access to ensure that the
 * session is used serially. The function handles the initialization and opening of
 * a session if one is not already active. If a session is active, it may return the
 * existing session based on the implemented session management policy.
 *
 * @param ulIndex The index for which to open or get an existing session.
 *
 * @return CK_SESSION_HANDLE If successful, the function returns a valid session handle.
 */
CK_SESSION_HANDLE PKCS11_GetSession(unsigned long ulIndex)
{
    CK_SESSION_HANDLE hSession = CK_INVALID_HANDLE;

    if (ulIndex < PKCS11_MAX_SESSIONS)
    {
        if (g_ahSession[ulIndex] == CK_INVALID_HANDLE)
        {
            CK_SLOT_ID aSlotIds[PKCS11_MAX_SESSIONS] = {0};
            CK_ULONG ulSlotCount = ARRAY_SIZE_stx(aSlotIds);
            CK_RV rvReturn = CKR_FUNCTION_FAILED;

            rvReturn = PKCS11_ListSlots(true, &aSlotIds[ulIndex], &ulSlotCount);
            if (rvReturn == CKR_OK)
            {
                CK_SLOT_ID slotId = aSlotIds[ulIndex];

                rvReturn = PKCS11_OpenSession(slotId, &g_ahSession[ulIndex]);
            }

            if (rvReturn == CKR_OK)
            {
                hSession = g_ahSession[ulIndex];
            }
        }
        else
        {
            hSession = g_ahSession[ulIndex];
        }
    }

    return hSession;
}

/**
 * Closes all currently open PKCS11 sessions.
 *
 * This function closes all PKCS11 sessions that have been opened during the
 * runtime of the application, ensuring the proper release of resources and
 * termination of cryptographic operations associated with those sessions.
 * The closure of these sessions is vital for maintaining the security and
 * integrity of the cryptographic module, as well as for freeing up system
 * resources that are no longer needed. This operation is essential for
 * applications that require a clean shutdown process or need to respond to
 * specific security events by closing all active sessions to prevent
 * unauthorized access or resource leakage.
 */
void PKCS11_CloseSessions()
{
    for (uint32_t uI = 0U; uI < ARRAY_SIZE_stx(g_ahSession); uI++)
        (void)PKCS11_CloseSession(g_ahSession[uI]);
}

/**
 * This function is responsible for releasing a session previously acquired
 * through PKCS11_GetSession. It ensures that the session is properly closed
 * or marked as available for reuse, according to the session management policy
 * implemented. If the session management supports caching or pooling of sessions,
 * this function may not immediately close the session but instead update its
 * state to reflect that it is no longer in use by the current operation.
 *
 * @param hSession The session handle to be released.
 */
void PKCS11_ReleaseSession(CK_SESSION_HANDLE hSession)
{
    /* Nothing to do */
    UNUSED_VAR (hSession);
}

/**
 * Lists the slots in the PKCS#11 module, with an option to only list slots where a token is present.
 *
 * This function provides the capability to enumerate all the slots or only those slots that
 * currently have a token present in the PKCS#11 module. It is typically used in scenarios
 * where an application needs to identify available cryptographic resources, such as hardware
 * security modules or smart cards, managed by the PKCS#11 library.
 *
 * @param bTokenPresent A boolean value that specifies the type of slots to list:
 *                      - If true, the function will only list the slots where a token is currently present.
 *                      - If false, it will list all the slots, regardless of whether a token is present.
 *
 * @param pSlots An array of CK_SLOT_ID where the function will store the IDs of the listed slots.
 *               The caller must ensure that this array is large enough to hold the results, based
 *               on the value pointed to by pulSlotCount.
 *
 * @param pulSlotCount A pointer to a CK_ULONG variable:
 *                     - On input, it should point to a variable containing the size of the pSlots array.
 *                     - On output, it will be updated with the actual number of slots found.
 *
 * @return CK_RV A return value of type CK_RV as defined by the PKCS#11 standard. It indicates
 *               the success or failure of the operation. Common return values include CKR_OK
 *               for success, CKR_ARGUMENTS_BAD if the input arguments are invalid, CKR_BUFFER_TOO_SMALL
 *               if the provided array is not large enough to hold all the slot IDs, and other
 *               PKCS#11-specific error codes as applicable.
 *
 * The function first validates the input arguments. It returns CKR_ARGUMENTS_BAD if either
 * pulSlotCount is NULL, or if pSlots is NULL while *pulSlotCount is non-zero (indicating an
 * expected output). If the array is too small to hold all the slot IDs, it returns CKR_BUFFER_TOO_SMALL.
 * On successfully listing the slots, it stores the slot IDs in the pSlots array (if provided) and
 * updates *pulSlotCount with the actual number of slots found, then returns CKR_OK.
 */
CK_RV PKCS11_ListSlots(bool bTokenPresent, CK_SLOT_ID_PTR pSlots,
                       CK_ULONG_PTR pulSlotCount)
{
    CK_RV rvReturn = CKR_GENERAL_ERROR;

    if ((pSlots != NULL) && (pulSlotCount != NULL))
    {
        CK_BBOOL bTokenPresentFlag = bTokenPresent ? CK_TRUE : CK_FALSE;

        rvReturn =
            g_pCKFunctionList->C_GetSlotList(bTokenPresentFlag, NULL, pulSlotCount);
        if (rvReturn == CKR_OK)
        {
            rvReturn = g_pCKFunctionList->C_GetSlotList(bTokenPresentFlag, pSlots,
                                                        pulSlotCount);
        }
        else
        {
            DBGPRINTF_ERROR("C_GetSlotList() failed 0x%lx:(%s)", rvReturn,
                            CKR_TO_STR(rvReturn));
        }
    }
    else
    {
        rvReturn = CKR_ARGUMENTS_BAD;
    }

    return rvReturn;
}

/**
 * Searches for a slot with a token matching the specified token name.
 *
 * This function iterates through the slots available in the PKCS#11 module to find a token
 * that matches the given token name. It is typically used in scenarios where the application
 * needs to interact with a specific cryptographic token and thus needs to locate the slot
 * in which this token is present.
 *
 * @param pchTokenName Pointer to a constant character string representing the name of the
 *                     token to search for. This name is used to match against the token
 *                     names present in the available slots.
 *
 * @param pSlotId Pointer to a CK_SLOT_ID where the function will store the ID of the found
 *                slot. CK_SLOT_ID is typically a numeric or handle-like identifier used to
 *                reference a specific slot in PKCS#11 operations.
 *
 * @return CK_RV A return value of type CK_RV as defined by the PKCS#11 standard. It indicates
 *               the success or failure of the operation. Common return values include CKR_OK
 *               for success, CKR_ARGUMENTS_BAD if either of the input arguments is invalid,
 *               CKR_TOKEN_NOT_FOUND if no matching token is found, and other PKCS#11-specific
 *               error codes as applicable.
 *
 * The function checks if the input arguments are valid and returns CKR_ARGUMENTS_BAD if either
 * is NULL. On successful finding of the slot, it stores the slot ID in the location pointed to
 * by pSlotId and returns CKR_OK. If no matching token is found, it returns CKR_TOKEN_NOT_FOUND.
 * The function may also return other PKCS#11 error codes based on the result of internal PKCS#11
 * calls made to enumerate slots and tokens.
 */
CK_RV PKCS11_FindSlotByTokenName(const char* pchTokenName,
                                 CK_SLOT_ID* pSlotId)
{
    CK_RV rvReturn = CKR_GENERAL_ERROR;

    if ((pchTokenName != NULL) && (pSlotId != NULL))
    {
        CK_SLOT_ID aSlotIds[10];
        CK_ULONG ulSlotCount = 10;

        rvReturn = PKCS11_ListSlots(true, aSlotIds, &ulSlotCount);

        if (rvReturn == CKR_OK)
        {
            CK_ULONG ulI;
            for (ulI = 0U; ulI < ulSlotCount; ++ulI)
            {
                CK_TOKEN_INFO sTokenInfo;
                rvReturn = g_pCKFunctionList->C_GetTokenInfo(aSlotIds[ulI], &sTokenInfo);

                if ((rvReturn == CKR_OK) &&
                    (strncmp((const char*)sTokenInfo.label, pchTokenName,
                             strlen(pchTokenName)) == 0))
                {
                    *pSlotId = aSlotIds[ulI];
                    rvReturn = CKR_OK;
                    break;
                }
                else
                {
                    DBGPRINTF_WARN("C_GetTokenInfo() failed 0x%lx:(%s)", rvReturn,
                                   CKR_TO_STR(rvReturn));
                }
            }

            if (ulI == ulSlotCount)
            {
                rvReturn = CKR_SLOT_ID_INVALID;
            }
        }
    }
    else
    {
        rvReturn = CKR_ARGUMENTS_BAD;
    }

    return rvReturn;
}

/**
 * Configures the PKCS#11 library settings.
 *
 * This function initializes the global configuration variables for the PKCS#11 library
 * with the provided values. It is assumed that this function is called before
 * loading the PKCS#11 library and performing any cryptographic operations.
 *
 * @param pchUserPin Pointer to a constant character string representing the user's PIN.
 *                   This PIN may be required to access certain functionalities provided
 *                   by the PKCS#11 library, typically for user authentication purposes.
 *
 * @param pchTokenName Pointer to a constant character string representing the name of the
 *                     token. This is typically used to identify and access the cryptographic
 *                     token or hardware module.
 *
 * Assertions are used to validate that none of the input pointers are NULL, ensuring that
 * valid data is provided to the function.
 */
void PKCS11_ConfigInit(const char* pchUserPin, const char* pchTokenName)
{
    ASSERT_STX(pchUserPin != NULL);
    ASSERT_STX(pchTokenName != NULL);

    g_pchUserPin = pchUserPin;
    g_pchTokenName = pchTokenName;
}

/**
 * Initializes a message-digesting operation in a given session.
 *
 * @param hSession The session handle, identifying the ongoing session.
 * @param ckMechanismType Type of hashing mechanism (e.g., CKM_SHA256).
 * @return CK_RV indicating the success or failure of the operation.
 */
CK_RV PKCS11_DigestInit(CK_SESSION_HANDLE hSession, CK_MECHANISM_TYPE ckMechanismType)
{
    CK_RV rvReturn = CKR_FUNCTION_FAILED;
    CK_MECHANISM ckMechanism = {ckMechanismType, NULL, 0};

    rvReturn = g_pCKFunctionList->C_DigestInit(hSession, &ckMechanism);
    if (rvReturn != CKR_OK)
    {
        DBGPRINTF_WARN("C_DigestInit() returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
    }

    return rvReturn;
}

/**
 * Continues a multi-part message-digesting operation, processing another data part.
 *
 * @param hSession The session handle, identifying the ongoing session.
 * @param pchPart A pointer to the data part to be digested.
 * @param ulPartLen Length of the data part in bytes.
 * @return CK_RV indicating the result of the operation.
 */
CK_RV PKCS11_DigestUpdate(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pchPart, CK_ULONG ulPartLen)
{
    CK_RV rvReturn = CKR_FUNCTION_FAILED;

    if (pchPart != NULL)
    {
        rvReturn = g_pCKFunctionList->C_DigestUpdate(hSession, pchPart, ulPartLen);
        if (rvReturn != CKR_OK)
        {
            DBGPRINTF_WARN("C_DigestUpdate() returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
        }
    }
    else
    {
        rvReturn = CKR_ARGUMENTS_BAD;
    }

    return rvReturn;
}

/**
 * Finishes a multi-part message-digesting operation, returning the message digest.
 *
 * @param hSession The session handle, identifying the ongoing session.
 * @param pchDigest Pointer to the location where the digest will be stored.
 * @param pulDigestLen Pointer to a CK_ULONG variable. On input, it represents the buffer size for pchDigest; on output, the actual digest length.
 * @return CK_RV indicating the result of the operation.
 */
CK_RV PKCS11_DigestFinal(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pchDigest, CK_ULONG_PTR pulDigestLen)
{
    CK_RV rvReturn = CKR_FUNCTION_FAILED;

    if ((pchDigest != NULL) && (pulDigestLen != NULL))
    {
        CK_ULONG ulDigestLen = *pulDigestLen;
        rvReturn = g_pCKFunctionList->C_DigestFinal(hSession, pchDigest, &ulDigestLen);
        if (rvReturn != CKR_OK)
        {
            DBGPRINTF_WARN("C_DigestFinal() returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
        }
    }
    else
    {
        rvReturn = CKR_ARGUMENTS_BAD;
    }

    return rvReturn;
}

/**
 * Perform a one-shot message-digesting operation, returning the message digest.
 *
 * @param hSession The session handle, identifying the ongoing session.
 * @param pchData A pointer to the data to be digested.
 * @param ulDataLen Length of the data in bytes.
 * @param pchDigest Pointer to the location where the digest will be stored.
 * @param pulDigestLen Pointer to a CK_ULONG variable. On input, it represents the buffer size for pchDigest; on output, the actual digest length.
 * @return CK_RV indicating the result of the operation.
 */
CK_RV PKCS11_Digest(CK_SESSION_HANDLE hSession,
                    CK_BYTE_PTR pchData, CK_ULONG ulDataLen,
                    CK_BYTE_PTR pchDigest, CK_ULONG_PTR pulDigestLen)
{
    CK_RV rvReturn = CKR_FUNCTION_FAILED;

    if ((pchData != NULL) && (pchDigest != NULL) && (pulDigestLen != NULL))
    {
        CK_ULONG ulDigestLen = *pulDigestLen;
        rvReturn = g_pCKFunctionList->C_Digest(hSession, pchData, ulDataLen, pchDigest, &ulDigestLen);
        if (rvReturn != CKR_OK)
        {
            DBGPRINTF_WARN("C_Digest() returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
        }
    }
    else
    {
        rvReturn = CKR_ARGUMENTS_BAD;
    }

    return rvReturn;
}

/**
 * Imports a secret key as volatile into a token and sets a handle.
 *
 * @param hSession The session handle with the token.
 * @param aucKey The secret key data.
 * @param uKeySize The secret key data length.
 * @param phKey A pointer to where the secret key handle will be stored.
 *
 * @return CK_RV The return value of the PKCS#11 operation. CKR_OK on success.
 */
CK_RV PKCS11_HMAC_ImportKey(CK_SESSION_HANDLE hSession,
                            const uint8_t aucKey[], uint32_t uKeySize,
                            CK_OBJECT_HANDLE_PTR phKey)
{
    CK_RV rvReturn = CKR_FUNCTION_FAILED;
    CK_BBOOL bFalse = CK_FALSE;
    CK_BBOOL bTrue = CK_TRUE;
    CK_OBJECT_CLASS ckClass = CKO_SECRET_KEY;
    CK_KEY_TYPE ckKeyType = CKK_GENERIC_SECRET;

    CK_ATTRIBUTE ckTemplate[] = {
        {CKA_CLASS, &ckClass, sizeof(ckClass)},
        {CKA_KEY_TYPE, &ckKeyType, sizeof(ckKeyType)},
        {CKA_VALUE, (CK_VOID_PTR)aucKey, uKeySize},
        {CKA_SIGN, &bTrue, sizeof(bTrue)},
        {CKA_VERIFY, &bTrue, sizeof(bTrue)},
        {CKA_DERIVE, &bTrue, sizeof(bTrue)},
        {CKA_TOKEN, &bFalse, sizeof(bFalse)},
        {CKA_EXTRACTABLE, &bFalse, sizeof(bFalse)},
    };

    if ((phKey == NULL) || (aucKey == NULL))
    {
        rvReturn = CKR_ARGUMENTS_BAD;
    }
    else
    {
        if (strncmp((const char*)aucKey, PKCS11_OBJECT_PREFIX, PKCS11_OBJECT_PREFIX_LEN) == 0)
        {
            rvReturn = PKCS11_Object_Find(hSession, CKO_SECRET_KEY, (const char*)aucKey, uKeySize, phKey);
        }
        else
        {
            CK_OBJECT_HANDLE hKey = CK_INVALID_HANDLE;
            rvReturn = g_pCKFunctionList->C_CreateObject(hSession, ckTemplate,
                                                         sizeof(ckTemplate) / sizeof(CK_ATTRIBUTE), &hKey);

            if (rvReturn == CKR_OK)
            {
                *phKey = hKey;
            }
            else
            {
                DBGPRINTF_WARN("C_CreateObject(HMAC) returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
            }
        }
    }

    return rvReturn;
}

/**
 * Computes an HMAC over given data using a specified key.
 *
 * @param hSession The session handle for PKCS#11 operations.
 * @param hKey The object handle of the key to be used for HMAC computation.
 * @param pData Pointer to the data over which the HMAC is to be computed.
 * @param ulDataLen Length of the data in bytes.
 * @param pSignature Pointer to the buffer where the computed HMAC will be stored.
 * @param pulSignatureSize Pointer to the length of the output buffer. On return, it holds
 *                     the actual length of the computed HMAC.
 *
 * @return CK_RV Returns CKR_OK on success, or an error code on failure.
 */
CK_RV PKCS11_HMAC_Compute(CK_SESSION_HANDLE hSession,
                          CK_MECHANISM_TYPE ckMechanismType, CK_OBJECT_HANDLE hKey,
                          CK_BYTE_PTR pData, CK_ULONG ulDataLen,
                          CK_BYTE_PTR pSignature, CK_ULONG_PTR pulSignatureSize)
{
    CK_RV rvReturn = CKR_FUNCTION_FAILED;
    CK_BYTE acDigest[PKCS11_MAX_DIGEST_SIZE];
    CK_ULONG ulDigestSize = sizeof(acDigest);
    CK_MECHANISM ckMechanism = {ckMechanismType, NULL_PTR, 0U};

    if ((pData == NULL_PTR) || (pSignature == NULL_PTR) || (pulSignatureSize == NULL_PTR) || (ulDataLen == 0U))
    {
        rvReturn = CKR_ARGUMENTS_BAD;
    }
    else
    {
        rvReturn = g_pCKFunctionList->C_SignInit(hSession, &ckMechanism, hKey);
        if (rvReturn == CKR_OK)
        {
            rvReturn = g_pCKFunctionList->C_Sign(hSession, pData, ulDataLen, acDigest, &ulDigestSize);
            if (rvReturn == CKR_OK)
            {
                if (*pulSignatureSize < ulDigestSize)
                {
                    rvReturn = CKR_BUFFER_TOO_SMALL;
                    DBGPRINTF_WARN("C_Sign() returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
                }
                else
                {
                    *pulSignatureSize = ulDigestSize;
                    COPYMEMORY(pSignature, acDigest, ulDigestSize);
                }
            }
            else
            {
                DBGPRINTF_WARN("C_Sign() returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
            }
        }
        else
        {
            DBGPRINTF_WARN("C_SignInit(HMAC) returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
        }
    }

    return rvReturn;
}

/**
 * Initializes a HMAC operation.
 *
 * @param hSession Pointer to the session handle.
 * @param ckMechanismType Type of the hash mechanism.
 * @param hKey The object handle of the key to be used for HMAC computation.
 * @return CK_RV Result code of the operation.
 */
CK_RV PKCS11_HMAC_Init(CK_SESSION_HANDLE hSession,
                       CK_MECHANISM_TYPE ckMechanismType,
                       CK_OBJECT_HANDLE hKey)
{
    CK_RV rvReturn = CKR_FUNCTION_FAILED;
    CK_MECHANISM ckMechanism = {ckMechanismType, NULL_PTR, 0U};

    rvReturn = g_pCKFunctionList->C_SignInit(hSession, &ckMechanism, hKey);
    if (rvReturn != CKR_OK)
    {
        DBGPRINTF_WARN("C_SignInit(HMAC INIT) returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
    }

    return rvReturn;
}

/**
 * Updates a HMAC operation with additional data.
 *
 * @param hSession Session handle.
 * @param pPart Pointer to the data to be added.
 * @param ulPartLen Length of the data.
 * @return CK_RV Result code of the operation.
 */
CK_RV PKCS11_HMAC_Update(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pPart,
                         CK_ULONG ulPartLen)
{
    CK_RV rvReturn = CKR_GENERAL_ERROR;

    if ((hSession == CK_INVALID_HANDLE) ||
        ((pPart == NULL_PTR) && (ulPartLen != 0U)))
    {
        rvReturn = CKR_ARGUMENTS_BAD;
    }
    else
    {
        rvReturn = g_pCKFunctionList->C_SignUpdate(hSession, pPart, ulPartLen);
        if (rvReturn != CKR_OK)
        {
            DBGPRINTF_WARN("C_SignUpdate() returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
        }
    }

    return rvReturn;
}

/**
 * Finalizes a HMAC operation, producing the HMAC value.
 *
 * @param hSession Session handle.
 * @param pSignature Pointer to where the HMAC value will be stored.
 * @param pulSignatureSize Pointer to the length of the HMAC value.
 * @return CK_RV Result code of the operation.
 */
CK_RV PKCS11_HMAC_Final(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pSignature,
                        CK_ULONG_PTR pulSignatureSize)
{
    CK_RV rvReturn = CKR_GENERAL_ERROR;

    CK_BYTE acDigest[PKCS11_MAX_DIGEST_SIZE];
    CK_ULONG ulDigestSize = sizeof(acDigest);

    if ((pSignature == NULL_PTR) || (pulSignatureSize == NULL_PTR))
    {
        rvReturn = CKR_ARGUMENTS_BAD;
    }
    else
    {
        rvReturn = g_pCKFunctionList->C_SignFinal(hSession, acDigest, &ulDigestSize);
        if (rvReturn == CKR_OK)
        {
            if (*pulSignatureSize < ulDigestSize)
            {
                rvReturn = CKR_BUFFER_TOO_SMALL;
            }
            else
            {
                *pulSignatureSize = ulDigestSize;
                COPYMEMORY(pSignature, acDigest, ulDigestSize);
            }
        }
        else
        {
            DBGPRINTF_WARN("C_SignFinal() returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
        }
    }

    return rvReturn;
}

/**
 * Seeds the random number generator of a PKCS#11 cryptographic token.
 *
 * This function seeds the cryptographic token's random number generator with
 * the provided seed data.
 *
 * @param hSession The session handle with the token.
 * @param aucSeed A pointer to the seed data.
 * @param ulSize  The size of the seed data in bytes.
 * @return        CKR_OK if the operation is successful; CKR_ARGUMENTS_BAD if the input is invalid;
 *                Other CK_RV values as defined by PKCS#11 if an error occurs.
 */
CK_RV PKCS11_RNG_Seed(CK_SESSION_HANDLE hSession, const uint8_t aucSeed[], uint32_t ulSize)
{
    CK_RV rvReturn = CKR_FUNCTION_FAILED;

    if ((aucSeed != NULL_PTR) && ulSize > 0)
    {
        rvReturn = g_pCKFunctionList->C_SeedRandom(hSession, (CK_BYTE_PTR)aucSeed, ulSize);
        if (rvReturn != CKR_OK)
        {
            DBGPRINTF_WARN("C_SeedRandom() returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
        }
    }
    else
    {
        rvReturn = CKR_ARGUMENTS_BAD;
    }

    return rvReturn;
}

/**
 * Retrieves entropy from the random number generator of a PKCS#11 cryptographic token.
 *
 * This function opens a session with the cryptographic token, generates random data (entropy)
 * of the specified minimum size, and then closes the session. The actual size of the generated
 * entropy is returned through the pucSize pointer.
 *
 * @param hSession The session handle with the token.
 * @param ucMinData  The minimum amount of data (in bytes) to generate.
 * @param aucData    A pointer to the buffer where the entropy will be stored.
 * @param pucSize       A pointer to a variable where the actual size of generated entropy will be stored.
 * @return              CKR_OK if the operation is successful; CKR_ARGUMENTS_BAD if the input is invalid;
 *                      Other CK_RV values as defined by PKCS#11 if an error occurs.
 */
CK_RV PKCS11_RNG_Read(CK_SESSION_HANDLE hSession, uint8_t ucMinData, uint8_t aucData[], uint8_t* pucSize)
{
    CK_RV rvReturn = CKR_FUNCTION_FAILED;

    if ((aucData != NULL_PTR) && (pucSize != NULL_PTR) && (ucMinData > 0))
    {
        rvReturn = g_pCKFunctionList->C_GenerateRandom(hSession, (CK_BYTE_PTR)aucData, ucMinData);
        if (rvReturn == CKR_OK)
        {
            *pucSize = ucMinData;
        }
        else
        {
            DBGPRINTF_WARN("C_GenerateRandom() returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
            *pucSize = 0;
        }
    }
    else
    {
        rvReturn = CKR_ARGUMENTS_BAD;
        if (pucSize != NULL_PTR)
        {
            *pucSize = 0;
        }
    }

    return rvReturn;
}

/**
 * Perform PBKDF1 using PKCS#11.
 *
 * This function implements the Password-Based Key Derivation Function 1 (PBKDF1) as per PKCS#5
 * specification, using a specified hash mechanism. It derives a key from a password and a salt.
 *
 * @param hSession The session handle with the token.
 * @param eHashMechanism Specifies the hash algorithm (e.g., CKM_SHA256).
 * @param pPassword Pointer to the password from which the key is derived.
 * @param ulPasswordLen Length of the password.
 * @param pSalt Pointer to the salt.
 * @param ulSaltLen Length of the salt.
 * @param ulIterations Number of iterations for the hash function.
 * @param pDerivedKey Buffer where the derived key will be stored.
 * @param pulDerivedKeyLen Pointer to the length of pDerivedKey.
 * @return CKR_OK on success, error code on failure.
 */
CK_RV PKCS11_PBKDF1(CK_SESSION_HANDLE hSession, CK_MECHANISM_TYPE eHashMechanism,
                    CK_BYTE_PTR pPassword, CK_ULONG ulPasswordLen,
                    CK_BYTE_PTR pSalt, CK_ULONG ulSaltLen, CK_ULONG ulIterations,
                    CK_BYTE_PTR pDerivedKey, CK_ULONG_PTR pulDerivedKeyLen)
{
    CK_RV rvReturn = CKR_OK;
    CK_MECHANISM ckMechanism = {eHashMechanism, NULL_PTR, 0};
    CK_BYTE aHashInput[PKCS11_MAX_DIGEST_SIZE + ulSaltLen];
    CK_BYTE aHashOutput[PKCS11_MAX_DIGEST_SIZE];
    CK_ULONG ulHashOutputLen = PKCS11_MAX_DIGEST_SIZE;
    CK_BYTE_PTR pCurrentInput = aHashInput;
    CK_ULONG i;

    COPYMEMORY(pCurrentInput, pPassword, ulPasswordLen);
    COPYMEMORY(pCurrentInput + ulPasswordLen, pSalt, ulSaltLen);

    for (i = 0U; (i < ulIterations) && (rvReturn == CKR_OK); ++i)
    {
        rvReturn = g_pCKFunctionList->C_DigestInit(hSession, &ckMechanism);
        if (rvReturn == CKR_OK)
        {
            rvReturn = g_pCKFunctionList->C_Digest(hSession, pCurrentInput, ulPasswordLen + ulSaltLen, aHashOutput, &ulHashOutputLen);
            pCurrentInput = aHashOutput;
            if (rvReturn == CKR_OK)
            {
                DBGPRINTF_WARN("C_Digest() returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
            }
        }
        else
        {
            DBGPRINTF_WARN("C_DigestInit() returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
        }
    }

    if (rvReturn == CKR_OK)
    {
        if (*pulDerivedKeyLen < ulHashOutputLen)
        {
            COPYMEMORY(pDerivedKey, aHashOutput, *pulDerivedKeyLen);
        }
        else
        {
            COPYMEMORY(pDerivedKey, aHashOutput, ulHashOutputLen);
            ZEROMEMORY(pDerivedKey + ulHashOutputLen, *pulDerivedKeyLen - ulHashOutputLen);
        }
    }

    return rvReturn;
}

/**
 * Performs a single-step Key Derivation Function (KDF) using PKCS#11.
 *
 * This function derives key material from a shared secret, other contextual information, and a specified hash algorithm.
 * It uses a counter-based method to generate key material of the desired length.
 *
 * @param hSession The session handle with the token.
 * @param hashAlgo - Mechanism type for the hash algorithm to be used.
 * @param pSharedSecret - Pointer to the shared secret byte array.
 * @param ulSharedSecretLen - Length of the shared secret byte array.
 * @param ulKeyDataLen - Desired length of the derived key material in bits.
 * @param pOtherInfo - Pointer to the other information byte array (context-specific data).
 * @param ulOtherInfoLen - Length of the other information byte array.
 * @param pDerivedKey - Pointer to the output buffer where the derived key will be stored.
 * @param ulDerivedKeyLen - Length of the buffer where the derived key will be stored.
 *
 * @return CKR_OK on success, error code on failure.
 */
CK_RV PKCS11_KDF_SingleStep(CK_SESSION_HANDLE hSession, CK_MECHANISM_TYPE hashAlgo,
                            CK_BYTE_PTR pSharedSecret, CK_ULONG ulSharedSecretLen,
                            CK_ULONG ulKeyDataLen, CK_BYTE_PTR pOtherInfo, CK_ULONG ulOtherInfoLen,
                            CK_BYTE_PTR pDerivedKey, CK_ULONG ulDerivedKeyLen)
{
    CK_MECHANISM digestMechanism = {hashAlgo, NULL, 0};
    CK_BYTE hashBuffer[PKCS11_MAX_DIGEST_SIZE];
    CK_RV rvReturn = CKR_OK;
    CK_ULONG reps, counter;
    CK_ULONG i, copyLen;
    CK_ULONG hashOutputLen = 0;

    switch (hashAlgo)
    {
    case CKM_SHA_1:
        hashOutputLen = 20;
        break;
    case CKM_SHA224:
        hashOutputLen = 28;
        break;
    case CKM_SHA256:
        hashOutputLen = 32;
        break;
    case CKM_SHA384:
        hashOutputLen = 48;
        break;
    case CKM_SHA512:
        hashOutputLen = 64;
        break;
    default:
        rvReturn = CKR_MECHANISM_INVALID;
    }

    if (pSharedSecret == NULL || pOtherInfo == NULL || pDerivedKey == NULL || hashOutputLen == 0)
    {
        rvReturn = CKR_ARGUMENTS_BAD;
    }
    else if (ulKeyDataLen / 8 > ulDerivedKeyLen)
    {
        rvReturn = CKR_BUFFER_TOO_SMALL;
    }
    else
    {
        reps = (ulKeyDataLen + (hashOutputLen * 8) - 1) / (hashOutputLen * 8);
        counter = 1;

        for (i = 0; (i < reps) && (rvReturn == CKR_OK); ++i)
        {
            CK_BYTE inputData[4 + ulSharedSecretLen + ulOtherInfoLen];

            inputData[0] = (CK_BYTE)((counter >> 24) & 0xFF);
            inputData[1] = (CK_BYTE)((counter >> 16) & 0xFF);
            inputData[2] = (CK_BYTE)((counter >> 8) & 0xFF);
            inputData[3] = (CK_BYTE)(counter & 0xFF);
            COPYMEMORY(inputData + 4, pSharedSecret, ulSharedSecretLen);
            COPYMEMORY(inputData + 4 + ulSharedSecretLen, pOtherInfo, ulOtherInfoLen);

            rvReturn = g_pCKFunctionList->C_DigestInit(hSession, &digestMechanism);
            if (rvReturn == CKR_OK)
                rvReturn = g_pCKFunctionList->C_DigestUpdate(hSession, inputData, sizeof(inputData));
            if (rvReturn == CKR_OK)
                rvReturn = g_pCKFunctionList->C_DigestFinal(hSession, hashBuffer, &hashOutputLen);

            if (rvReturn == CKR_OK)
            {
                copyLen = (i == reps - 1 && (ulKeyDataLen / 8) % hashOutputLen != 0) ? (ulKeyDataLen / 8) % hashOutputLen : hashOutputLen;
                if ((i * hashOutputLen + copyLen) > ulDerivedKeyLen)
                {
                    rvReturn = CKR_BUFFER_TOO_SMALL;
                    break;
                }
                COPYMEMORY(pDerivedKey + (i * hashOutputLen), hashBuffer, copyLen);
            }

            counter++;
        }
    }

    return rvReturn;
}

/**
 * HKDF Extract phase.
 *
 * @param hSession The session handle for the PKCS#11 session.
 * @param ckMechanismType The type of HMAC mechanism to use (e.g., CKM_SHA256_HMAC).
 * @param hIKM The handle of the key material (IKM) to be used for the HMAC operation.
 * @param pSalt Pointer to the salt value; may be NULL if no salt is used.
 * @param ulSaltLen Length of the salt value in bytes; may be 0 if no salt is used.
 * @param aucLabel Label of the resulting pseudorandom key (PRK).
 * @param phPRK Pointer to handle of the resulting pseudorandom key (PRK).
 *
 * @return CKR_OK on success, error code on failure.
 */
CK_RV PKCS11_HKDF_Extract(CK_SESSION_HANDLE hSession,
                          CK_MECHANISM_TYPE ckMechanismType,
                          CK_OBJECT_HANDLE hIKM,
                          CK_BYTE_PTR pSalt,
                          CK_ULONG ulSaltLen,
                          CK_BYTE_PTR aucLabel,
                          CK_OBJECT_HANDLE_PTR phPRK)
{
    CK_RV rvReturn = CKR_FUNCTION_FAILED;
    CK_MECHANISM ckMechanism = {CKM_HKDF_DERIVE, NULL, 0};

    CK_HKDF_PARAMS hkdfParams;
    CK_OBJECT_CLASS ckKeyClass = CKO_SECRET_KEY;
    CK_KEY_TYPE ckKeyType = CKK_GENERIC_SECRET;
    CK_BBOOL bFalse = CK_FALSE;
    CK_BBOOL bTrue = CK_TRUE;
    CK_ATTRIBUTE ckDerivedKeyTemplate[] = {
        {CKA_CLASS, &ckKeyClass, sizeof(ckKeyClass)},
        {CKA_KEY_TYPE, &ckKeyType, sizeof(ckKeyType)},
        {CKA_TOKEN, &bFalse, sizeof(bFalse)},
        {CKA_LABEL, aucLabel, (CK_ULONG)strlen((const char*)aucLabel)},
        {CKA_SIGN, &bTrue, sizeof(bTrue)},
        {CKA_VERIFY, &bTrue, sizeof(bTrue)},
        {CKA_DERIVE, &bTrue, sizeof(bTrue)},
    };

    hkdfParams.prfHashMechanism = ckMechanismType;
    hkdfParams.bExtract = CK_TRUE;
    hkdfParams.bExpand = CK_FALSE;
    hkdfParams.ulSaltType = CKF_HKDF_SALT_DATA;
    hkdfParams.pSalt = pSalt;
    hkdfParams.ulSaltLen = ulSaltLen;
    hkdfParams.pInfo = NULL;
    hkdfParams.ulInfoLen = 0;

    ckMechanism.pParameter = &hkdfParams;
    ckMechanism.ulParameterLen = sizeof(hkdfParams);

    rvReturn = g_pCKFunctionList->C_DeriveKey(hSession, &ckMechanism, hIKM,
                                              ckDerivedKeyTemplate, sizeof(ckDerivedKeyTemplate) / sizeof(CK_ATTRIBUTE),
                                              phPRK);
    if (rvReturn != CKR_OK)
    {
        DBGPRINTF_WARN("C_DeriveKey() returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
    }

    return rvReturn;
}

/**
 * HKDF Expand phase.
 *
 * @param hSession The session handle for the PKCS#11 session.
 * @param ckMechanismType Specifies the HMAC algorithm.
 * @param hPRK The object handle for the PRK within the token. If the PRK is provided as a byte array, this should be 0.
 * @param pInfo Pointer to the optional context and application specific information.
 * @param ulInfoLen Length of the info.
 * @param aucLabel Label of the keying material.
 * @param phOutputKey Pointer to handle of the output keying material.
 *
 * @return CKR_OK on success, error code on failure.
 */
CK_RV PKCS11_HKDF_Expand(CK_SESSION_HANDLE hSession,
                         CK_MECHANISM_TYPE ckMechanismType,
                         CK_OBJECT_HANDLE hPRK,
                         CK_BYTE_PTR pInfo,
                         CK_ULONG ulInfoLen,
                         CK_BYTE_PTR aucLabel,
                         CK_OBJECT_HANDLE_PTR phOutKey)
{
    CK_RV rvReturn = CKR_FUNCTION_FAILED;
    CK_MECHANISM ckMechanism = {CKM_HKDF_DERIVE, NULL, 0};

    CK_HKDF_PARAMS hkdfParams;
    CK_OBJECT_CLASS ckKeyClass = CKO_SECRET_KEY;
    CK_KEY_TYPE ckKeyType = CKK_GENERIC_SECRET;
    CK_BBOOL bFalse = CK_FALSE;
    CK_ATTRIBUTE ckDerivedKeyTemplate[] = {
        {CKA_CLASS, &ckKeyClass, sizeof(ckKeyClass)},
        {CKA_KEY_TYPE, &ckKeyType, sizeof(ckKeyType)},
        {CKA_TOKEN, &bFalse, sizeof(bFalse)},
        {CKA_LABEL, aucLabel, (CK_ULONG)strlen((const char*)aucLabel)},
    };

    hkdfParams.prfHashMechanism = ckMechanismType;
    hkdfParams.bExtract = CK_TRUE;
    hkdfParams.bExpand = CK_TRUE;
    hkdfParams.ulSaltType = CKF_HKDF_SALT_NULL;
    hkdfParams.pSalt = NULL;
    hkdfParams.ulSaltLen = 0;
    hkdfParams.pInfo = pInfo;
    hkdfParams.ulInfoLen = ulInfoLen;

    ckMechanism.pParameter = &hkdfParams;
    ckMechanism.ulParameterLen = sizeof(hkdfParams);

    rvReturn = g_pCKFunctionList->C_DeriveKey(hSession, &ckMechanism, hPRK,
                                              ckDerivedKeyTemplate, sizeof(ckDerivedKeyTemplate) / sizeof(CK_ATTRIBUTE),
                                              phOutKey);

    if (rvReturn != CKR_OK)
    {
        DBGPRINTF_WARN("C_DeriveKey() returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
    }
    return rvReturn;
}

/**
 * HKDF Extract & Expand.
 *
 * @param hSession The session handle for the PKCS#11 session.
 * @param ckMechanismType Specifies the HMAC algorithm.
 * @param hBaseKey The object handle for the base key for HMAC function.
 * @param pSalt Pointer to the salt value; may be NULL if no salt is used.
 * @param ulSaltLen Length of the salt value in bytes; may be 0 if no salt is used.
 * @param pInfo Pointer to the optional context and application specific information.
 * @param ulInfoLen Length of the info.
 * @param aucLabel Label of the output keying material.
 * @param phOutKey Pointer to handle of the output keying material.
 *
 * @return CKR_OK on success, error code on failure.
 */
CK_RV PKCS11_HKDF_Derive(CK_SESSION_HANDLE hSession,
                         CK_MECHANISM_TYPE ckMechanismType,
                         CK_OBJECT_HANDLE hBaseKey,
                         CK_BYTE_PTR pSalt,
                         CK_ULONG ulSaltLen,
                         CK_BYTE_PTR pInfo,
                         CK_ULONG ulInfoLen,
                         CK_BYTE_PTR aucLabel,
                         CK_OBJECT_HANDLE_PTR phOutKey)
{
    CK_RV rvReturn = CKR_FUNCTION_FAILED;
    CK_MECHANISM ckMechanism = {CKM_HKDF_DERIVE, NULL, 0};

    CK_HKDF_PARAMS hkdfParams;
    CK_OBJECT_CLASS ckKeyClass = CKO_SECRET_KEY;
    CK_KEY_TYPE ckKeyType = CKK_GENERIC_SECRET;
    CK_BBOOL bTrue = CK_TRUE;
    CK_BBOOL bFalse = CK_FALSE;
    CK_ATTRIBUTE ckDerivedKeyTemplate[] = {
        {CKA_CLASS, &ckKeyClass, sizeof(ckKeyClass)},
        {CKA_KEY_TYPE, &ckKeyType, sizeof(ckKeyType)},
        {CKA_TOKEN, &bFalse, sizeof(bFalse)},
        {CKA_LABEL, aucLabel, (CK_ULONG)strlen((const char*)aucLabel)},
        {CKA_SIGN, &bTrue, sizeof(bTrue)},
        {CKA_VERIFY, &bTrue, sizeof(bTrue)},
        {CKA_DERIVE, &bTrue, sizeof(bTrue)},
    };

    hkdfParams.prfHashMechanism = ckMechanismType;
    hkdfParams.bExtract = CK_TRUE;
    hkdfParams.bExpand = CK_TRUE;
    hkdfParams.ulSaltType = CKF_HKDF_SALT_DATA;
    hkdfParams.pSalt = pSalt;
    hkdfParams.ulSaltLen = ulSaltLen;
    hkdfParams.pInfo = pInfo;
    hkdfParams.ulInfoLen = ulInfoLen;

    ckMechanism.pParameter = &hkdfParams;
    ckMechanism.ulParameterLen = sizeof(hkdfParams);

    rvReturn = g_pCKFunctionList->C_DeriveKey(hSession, &ckMechanism, hBaseKey,
                                              ckDerivedKeyTemplate, sizeof(ckDerivedKeyTemplate) / sizeof(CK_ATTRIBUTE),
                                              phOutKey);
    if (rvReturn != CKR_OK)
    {
        DBGPRINTF_WARN("C_DeriveKey() returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
    }
    return rvReturn;
}

/**
 * TODO: This should be removed once the stack implements proper way to handle key in a token.
 * Function to convert raw AES key data to a CK_OBJECT_HANDLE
 */
CK_RV PKCS11_AES_ImportKey(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pchKey, CK_ULONG ulKeyLength, CK_OBJECT_HANDLE_PTR phKey)
{
    CK_RV rvReturn = CKR_FUNCTION_FAILED;
    CK_OBJECT_CLASS ckKeyClass = CKO_SECRET_KEY;
    CK_KEY_TYPE ckKeyType = CKK_AES;
    CK_BBOOL bTrue = CK_TRUE;
    CK_BBOOL bFalse = CK_FALSE;

    if ((hSession == CK_INVALID_HANDLE) || (pchKey == NULL) || (phKey == NULL))
    {
        rvReturn = CKR_ARGUMENTS_BAD;
    }
    else
    {
        CK_ATTRIBUTE keyTemplate[] = {
            {CKA_CLASS, &ckKeyClass, sizeof(ckKeyClass)},
            {CKA_KEY_TYPE, &ckKeyType, sizeof(ckKeyType)},
            {CKA_VALUE, (CK_VOID_PTR)pchKey, ulKeyLength},
            {CKA_ENCRYPT, &bTrue, sizeof(bTrue)},
            {CKA_DECRYPT, &bTrue, sizeof(bTrue)},
            {CKA_TOKEN, &bFalse, sizeof(bFalse)},
            {CKA_PRIVATE, &bTrue, sizeof(bTrue)}};

        rvReturn = g_pCKFunctionList->C_CreateObject(hSession, keyTemplate, sizeof(keyTemplate) / sizeof(CK_ATTRIBUTE), phKey);
        if (rvReturn != CKR_OK)
        {
            DBGPRINTF_WARN("C_CreateObject() returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
        }
    }

    return rvReturn;
}

/**
 * @brief Initializes the AES CBC encryption operation with the specified key and IV.
 *
 * @param hSession Handle to the current session.
 * @param hKey Handle to the key to be used for encryption.
 * @param pIV Pointer to the Initialization Vector. This parameter is ignored if 'generateIV' is TRUE.
 * @param ulIVLen Length of the IV in bytes.
 * @param generateIV Flag indicating whether to generate a new IV (CK_TRUE) or use the provided IV (CK_FALSE).
 * @return CK_RV Result code indicating the result of the operation.
 */
CK_RV PKCS11_AES_CBC_EncryptInit(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE hKey, CK_BYTE_PTR pIV, CK_ULONG ulIVLen, CK_BBOOL generateIV)
{
    CK_RV rvReturn = CKR_FUNCTION_FAILED;

    if ((pIV == NULL) && (generateIV == CK_FALSE))
    {
        rvReturn = CKR_ARGUMENTS_BAD;
    }
    else
    {
        uint8_t ucGeneratedIVLen = (uint8_t)ulIVLen;
        if (generateIV == CK_TRUE)
        {
            rvReturn = PKCS11_RNG_Read(hSession, ulIVLen, pIV, &ucGeneratedIVLen);
        }
        else
        {
            rvReturn = CKR_OK;
        }

        if (rvReturn == CKR_OK)
        {
            CK_MECHANISM ckMechanism;
            ckMechanism.mechanism = CKM_AES_CBC;
            ckMechanism.pParameter = (CK_VOID_PTR)pIV;
            ckMechanism.ulParameterLen = ulIVLen;

            rvReturn = g_pCKFunctionList->C_EncryptInit(hSession, &ckMechanism, hKey);
            if (rvReturn != CKR_OK)
            {
                DBGPRINTF_WARN("C_EncryptInit() returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
            }
        }
    }

    return rvReturn;
}

/**
 * Encrypts data using AES CBC mode.
 *
 * @param hSession Handle to the current session.
 * @param pData Pointer to the data to be encrypted.
 * @param ulDataLen Length of the data in bytes.
 * @param pEncryptedData Pointer to the buffer where the encrypted data will be stored.
 * @param pulEncryptedDataLen Pointer to the length of the encrypted data buffer. Updated to actual size of encrypted data on return.
 * @return CK_RV Result code indicating the result of the operation.
 */
CK_RV PKCS11_AES_CBC_Encrypt(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pData, CK_ULONG ulDataLen, CK_BYTE_PTR pEncryptedData, CK_ULONG_PTR pulEncryptedDataLen)
{
    CK_RV rvReturn = CKR_FUNCTION_FAILED;

    if ((pData == NULL) || (pEncryptedData == NULL) || (pulEncryptedDataLen == NULL))
    {
        rvReturn = CKR_ARGUMENTS_BAD;
    }
    else
    {
        rvReturn = g_pCKFunctionList->C_Encrypt(hSession, pData, ulDataLen, pEncryptedData, pulEncryptedDataLen);
        if (rvReturn != CKR_OK)
        {
            DBGPRINTF_WARN("C_Encrypt() returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
        }
    }

    return rvReturn;
}

/**
 * Initializes the AES CBC decryption operation with the specified key and IV.
 *
 * @param hSession Handle to the current session.
 * @param hKey Handle to the key to be used for decryption.
 * @param pIV Pointer to the Initialization Vector.
 * @param ulIVLen Length of the IV in bytes.
 * @return CK_RV Result code indicating the result of the operation.
 */
CK_RV PKCS11_AES_CBC_DecryptInit(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE hKey, CK_BYTE_PTR pIV, CK_ULONG ulIVLen)
{
    CK_RV rvReturn = CKR_FUNCTION_FAILED;

    if (pIV == NULL)
    {
        rvReturn = CKR_ARGUMENTS_BAD;
    }
    else
    {
        CK_MECHANISM ckMechanism;
        ckMechanism.mechanism = CKM_AES_CBC;
        ckMechanism.pParameter = (CK_VOID_PTR)pIV;
        ckMechanism.ulParameterLen = ulIVLen;

        rvReturn = g_pCKFunctionList->C_DecryptInit(hSession, &ckMechanism, hKey);
        if (rvReturn != CKR_OK)
        {
            DBGPRINTF_WARN("C_DecryptInit() returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
        }
    }

    return rvReturn;
}

/**
 * Decrypts data using AES CBC mode.
 *
 * @param hSession Handle to the current session.
 * @param pEncryptedData Pointer to the encrypted data.
 * @param ulEncryptedDataLen Length of the encrypted data in bytes.
 * @param pDecryptedData Pointer to the buffer where the decrypted data will be stored.
 * @param pulDecryptedDataLen Pointer to the length of the decrypted data buffer. Updated to actual size of decrypted data on return.
 * @return CK_RV Result code indicating the result of the operation.
 */
CK_RV PKCS11_AES_CBC_Decrypt(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pEncryptedData, CK_ULONG ulEncryptedDataLen, CK_BYTE_PTR pDecryptedData, CK_ULONG_PTR pulDecryptedDataLen)
{
    CK_RV rvReturn = CKR_FUNCTION_FAILED;

    if ((pEncryptedData == NULL) || (pDecryptedData == NULL) || (pulDecryptedDataLen == NULL))
    {
        rvReturn = CKR_ARGUMENTS_BAD;
    }
    else
    {
        rvReturn = g_pCKFunctionList->C_Decrypt(hSession, pEncryptedData, ulEncryptedDataLen, pDecryptedData, pulDecryptedDataLen);
        if (rvReturn != CKR_OK)
        {
            DBGPRINTF_WARN("C_Decrypt() returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
        }
    }
    return rvReturn;
}

/**
 * Encrypts a single block of data using AES.
 *
 * This function encrypts exactly one block of data using the AES algorithm with the specified key.
 * The caller must ensure that the data block and key are appropriately sized and allocated.
 * The encryption uses CBC_PAD mode for simplicity; hence, an IV is required but not provided here for security reasons.
 *
 * @param hSession The session handle with the cryptographic token.
 * @param hKey The encryption key handle.
 * @param pData Pointer to the data block to be encrypted. Must not be NULL.
 * @param dataLen Length of the data to be encrypted.
 * @param pEncryptedData Pointer to the buffer where the encrypted data will be stored.
 * @param pEncryptedDataLen Pointer to the size of the encrypted data buffer. On return, it will contain the actual size of the encrypted data.
 * @return CK_RV Returns CKR_OK on success, error code on failure.
 */
CK_RV PKCS11_AES_EncryptBlock(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE hKey,
                              const CK_BYTE_PTR pData, CK_ULONG dataLen,
                              CK_BYTE_PTR pEncryptedData, CK_ULONG_PTR pEncryptedDataLen)
{
    CK_RV rvReturn = CKR_GENERAL_ERROR;
    CK_MECHANISM mechanism = {CKM_AES_CBC_PAD, NULL_PTR, 0U};

    if ((pData != NULL_PTR) && (pEncryptedData != NULL_PTR) && (pEncryptedDataLen != NULL_PTR))
    {
        rvReturn = g_pCKFunctionList->C_EncryptInit(hSession, &mechanism, hKey);
        if (rvReturn == CKR_OK)
        {
            rvReturn = g_pCKFunctionList->C_Encrypt(hSession, pData, dataLen, pEncryptedData, pEncryptedDataLen);
            if (rvReturn != CKR_OK)
            {
                DBGPRINTF_WARN("C_Encrypt() returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
            }
        }
        else
        {
            DBGPRINTF_WARN("C_EncryptInit() returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
        }
    }

    return rvReturn;
}

/**
 * Decrypts a single block of data using AES.
 *
 * This function decrypts exactly one block of encrypted data using the AES algorithm with the specified key.
 * The caller must ensure that the encrypted data block and key are appropriately sized and allocated.
 * The decryption uses CBC_PAD mode for simplicity; hence, an IV is required but not provided here for security reasons.
 *
 * @param hSession The session handle with the cryptographic token.
 * @param hKey The decryption key handle.
 * @param pEncryptedData Pointer to the encrypted data block to be decrypted. Must not be NULL.
 * @param encryptedDataLen Length of the encrypted data.
 * @param pData Pointer to the buffer where the decrypted data will be stored.
 * @param pDataLen Pointer to the size of the decrypted data buffer. On return, it will contain the actual size of the decrypted data.
 * @return CK_RV Returns CKR_OK on success, error code on failure.
 */
CK_RV PKCS11_AES_DecryptBlock(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE hKey,
                              const CK_BYTE_PTR pEncryptedData, CK_ULONG encryptedDataLen,
                              CK_BYTE_PTR pData, CK_ULONG_PTR pDataLen)
{
    CK_RV rvReturn = CKR_GENERAL_ERROR;
    CK_MECHANISM mechanism = {CKM_AES_CBC_PAD, NULL_PTR, 0U};

    if ((pEncryptedData != NULL_PTR) && (pData != NULL_PTR) && (pDataLen != NULL_PTR))
    {
        rvReturn = g_pCKFunctionList->C_DecryptInit(hSession, &mechanism, hKey);
        if (rvReturn == CKR_OK)
        {
            rvReturn = g_pCKFunctionList->C_Decrypt(hSession, pEncryptedData, encryptedDataLen, pData, pDataLen);
            if (rvReturn != CKR_OK)
            {
                DBGPRINTF_WARN("C_Decrypt() returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
            }
        }
        else
        {
            DBGPRINTF_WARN("C_DecryptInit() returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
        }
    }

    return rvReturn;
}

typedef struct
{
    const char* pchName;
    const char* pchOid;
    const uint8_t aucOid[PKCS11_MAX_OID_LENGTH];
    uint32_t uOidLength;
    uint32_t uSizeBits;
    CK_KEY_TYPE ckMechanismType;
} EC_Curve_Info_Type;

static EC_Curve_Info_Type g_EC_CruveInfos[] = {
    {"secp192r1", "1.2.840.10045.3.1.1", {0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x03, 0x01, 0x01}, 10, 192, 0},
    {"prime192v1", "1.2.840.10045.3.1.1", {0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x03, 0x01, 0x01}, 10, 192, 0},
    {"prime192v2", "1.2.840.10045.3.1.2", {0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x03, 0x01, 0x02}, 10, 192, 0},
    {"prime192v3", "1.2.840.10045.3.1.3", {0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x03, 0x01, 0x03}, 10, 192, 0},
    {"nistp192", "1.2.840.10045.3.1.1", {0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x03, 0x01, 0x01}, 10, 192, 0},
    {"ansiX9p192r1", "1.2.840.10045.3.1.1", {0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x03, 0x01, 0x01}, 10, 192, 0},
    {"secp224r1", "1.3.132.0.33", {0x06, 0x05, 0x2B, 0x81, 0x04, 0x00, 0x21}, 7, 224, 0},
    {"nistp224", "1.3.132.0.33", {0x06, 0x05, 0x2B, 0x81, 0x04, 0x00, 0x21}, 7, 224, 0},
    {"prime256v1", "1.2.840.10045.3.1.7", {0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x03, 0x01, 0x07}, 10, 256, 0},
    {"secp256r1", "1.2.840.10045.3.1.7", {0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x03, 0x01, 0x07}, 10, 256, 0},
    {"ansiX9p256r1", "1.2.840.10045.3.1.7", {0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x03, 0x01, 0x07}, 10, 256, 0},
    {"frp256v1", "1.2.250.1.223.101.256.1", {0x06, 0x0a, 0x2a, 0x81, 0x7a, 0x01, 0x81, 0x5f, 0x65, 0x82, 0x00, 0x01}, 12, 256, 0},
    {"secp384r1", "1.3.132.0.34", {0x06, 0x05, 0x2B, 0x81, 0x04, 0x00, 0x22}, 7, 384, 0},
    {"prime384v1", "1.3.132.0.34", {0x06, 0x05, 0x2B, 0x81, 0x04, 0x00, 0x22}, 7, 384, 0},
    {"ansiX9p384r1", "1.3.132.0.34", {0x06, 0x05, 0x2B, 0x81, 0x04, 0x00, 0x22}, 7, 384, 0},
    {"prime521v1", "1.3.132.0.35", {0x06, 0x05, 0x2B, 0x81, 0x04, 0x00, 0x23}, 7, 521, 0},
    {"secp521r1", "1.3.132.0.35", {0x06, 0x05, 0x2B, 0x81, 0x04, 0x00, 0x23}, 7, 521, 0},
    {"nistp521", "1.3.132.0.35", {0x06, 0x05, 0x2B, 0x81, 0x04, 0x00, 0x23}, 7, 521, 0},
    {"brainpoolP192r1", "1.3.36.3.3.2.8.1.1.3", {0x06, 0x09, 0x2B, 0x24, 0x03, 0x03, 0x02, 0x08, 0x01, 0x01, 0x03}, 11, 192, 0},
    {"brainpoolP224r1", "1.3.36.3.3.2.8.1.1.5", {0x06, 0x09, 0x2B, 0x24, 0x03, 0x03, 0x02, 0x08, 0x01, 0x01, 0x05}, 11, 224, 0},
    {"brainpoolP256r1", "1.3.36.3.3.2.8.1.1.7", {0x06, 0x09, 0x2B, 0x24, 0x03, 0x03, 0x02, 0x08, 0x01, 0x01, 0x07}, 11, 256, 0},
    {"brainpoolP320r1", "1.3.36.3.3.2.8.1.1.9", {0x06, 0x09, 0x2B, 0x24, 0x03, 0x03, 0x02, 0x08, 0x01, 0x01, 0x09}, 11, 320, 0},
    {"brainpoolP384r1", "1.3.36.3.3.2.8.1.1.11", {0x06, 0x09, 0x2B, 0x24, 0x03, 0x03, 0x02, 0x08, 0x01, 0x01, 0x0B}, 11, 384, 0},
    {"brainpoolP512r1", "1.3.36.3.3.2.8.1.1.13", {0x06, 0x09, 0x2B, 0x24, 0x03, 0x03, 0x02, 0x08, 0x01, 0x01, 0x0D}, 11, 512, 0},
    {"secp192k1", "1.3.132.0.31", {0x06, 0x05, 0x2B, 0x81, 0x04, 0x00, 0x1F}, 7, 192, 0},
    {"secp256k1", "1.3.132.0.10", {0x06, 0x05, 0x2B, 0x81, 0x04, 0x00, 0x0A}, 7, 256, 0},
    {"secp521k1", "1.3.132.0.35", {0x06, 0x05, 0x2B, 0x81, 0x04, 0x00, 0x23}, 7, 521, 0},
    {"edwards25519", "1.3.6.1.4.1159.15.1", {0x13, 0x0c, 0x65, 0x64, 0x77, 0x61, 0x72, 0x64, 0x73, 0x32, 0x35, 0x35, 0x31, 0x39}, 14, 255, CKM_EC_EDWARDS_KEY_PAIR_GEN},
    {"curve25519", "1.3.6.1.4.3029.1.5.1", {0x13, 0x0b, 0x63, 0x75, 0x72, 0x76, 0x65, 0x32, 0x35, 0x35, 0x31, 0x39}, 12, 255, CKM_EC_MONTGOMERY_KEY_PAIR_GEN},
    {NULL, NULL, {0}, 0, 0, 0}};

/**
 * Retrieves the Object Identifier (OID) of a given curve name.
 *
 * @param pchCurveName The name of the curve to search for.
 * @param pulOidLen Pointer to a variable where the length of the OID will be stored.
 * @return A pointer to the OID byte array if found; otherwise, NULL.
 */
static const uint8_t* EC_CRUVE_NAME_2_OID(const char* pchCurveName, uint32_t* pulOidLen)
{
    const uint8_t* pRetVal = NULL;
    uint32_t len = 0;

    if (pchCurveName != NULL && pulOidLen != NULL)
    {
        for (uint32_t i = 0; g_EC_CruveInfos[i].pchName != NULL; ++i)
        {
            if (strcmp(g_EC_CruveInfos[i].pchName, pchCurveName) == 0)
            {
                len = g_EC_CruveInfos[i].uOidLength;
                pRetVal = g_EC_CruveInfos[i].aucOid;
                break;
            }
        }
    }

    if (pulOidLen != NULL)
    {
        *pulOidLen = len;
    }

    return pRetVal;
}

/**
 * Get EC curve information by OID.
 *
 * @param pchOid Byte array of the OID to match.
 * @param ulOidLength Length of the OID array.
 * @param[out] ppchCurveName Pointer to a variable where the curve name will be stored if found.
 * @param[out] ulCurveSizeBytes Pointer to a variable where the curve size in bytes will be stored if found.
 */
void PKCS11_EC_GetCurveInfoByOid(const uint8_t* pchOid, uint32_t ulOidLength, const char** ppchCurveName, uint32_t* ulCurveSizeBytes)
{
    bool bFound = false;

    if ((pchOid != NULL) && (ppchCurveName != NULL) && (ulCurveSizeBytes != NULL))
    {
        for (uint32_t i = 0; (g_EC_CruveInfos[i].pchName != NULL) && (bFound == false); ++i)
        {
            if ((g_EC_CruveInfos[i].uOidLength == ulOidLength) &&
                (memcmp(g_EC_CruveInfos[i].aucOid, pchOid, ulOidLength) == 0))
            {
                *ppchCurveName = g_EC_CruveInfos[i].pchName;
                *ulCurveSizeBytes = (g_EC_CruveInfos[i].uSizeBits + 7U) / 8U;
                bFound = true;
            }
        }

        if (bFound == false)
        {
            *ppchCurveName = NULL;
            *ulCurveSizeBytes = 0U;
        }
    }
}

/**
 * Generates an elliptic curve (EC) key pair.
 *
 * @param hSession Handle to the session.
 * @param pchCurveName Pointer to a string specifying the name of the elliptic curve to use for key generation.
 * @param aucLabel Label of the key pair.
 * @param phPublicKey Pointer to a CK_OBJECT_HANDLE where the handle to the generated public key will be stored.
 * @param phPrivateKey Pointer to a CK_OBJECT_HANDLE where the handle to the generated private key will be stored.
 *
 * @return CK_RV Returns CKR_OK on success, error code otherwise.
 */
CK_RV PKCS11_EC_GenKeyPair(CK_SESSION_HANDLE hSession, const char* pchCurveName, CK_BYTE_PTR aucLabel, CK_OBJECT_HANDLE_PTR phPublicKey, CK_OBJECT_HANDLE_PTR phPrivateKey)
{
    CK_RV rvReturn = CKR_OK;
    CK_MECHANISM mechanism = {CKM_EC_KEY_PAIR_GEN, NULL_PTR, 0};
    CK_KEY_TYPE ckKeyType = CKK_EC;
    CK_BBOOL bFalse = CK_FALSE;
    CK_BBOOL bTrue = CK_TRUE;
    uint32_t ulOidLen = 0;
    CK_OBJECT_CLASS ckClassPrivate = CKO_PRIVATE_KEY;
    CK_OBJECT_CLASS ckClassPublic = CKO_PUBLIC_KEY;

    if (pchCurveName == NULL || phPublicKey == NULL || phPrivateKey == NULL)
    {
        rvReturn = CKR_ARGUMENTS_BAD;
    }
    else
    {
        const uint8_t* pchOid = EC_CRUVE_NAME_2_OID(pchCurveName, &ulOidLen);
        if (pchOid != NULL)
        {
            CK_ATTRIBUTE ckPublicTemplate[] = {
                {CKA_EC_PARAMS, NULL, 0},
                {CKA_CLASS, &ckClassPublic, sizeof(ckClassPublic)},
                {CKA_KEY_TYPE, &ckKeyType, sizeof(ckKeyType)},
                {CKA_VERIFY, &bTrue, sizeof(bTrue)},
                {CKA_TOKEN, &bFalse, sizeof(bTrue)},
                {CKA_PRIVATE, &bFalse, sizeof(bFalse)},
                {CKA_LABEL, aucLabel, (CK_ULONG)strlen((const char*)aucLabel)},
            };

            CK_ATTRIBUTE ckPrivateTemplate[] = {
                {CKA_CLASS, &ckClassPrivate, sizeof(ckClassPrivate)},
                {CKA_KEY_TYPE, &ckKeyType, sizeof(ckKeyType)},
                {CKA_SIGN, &bTrue, sizeof(bTrue)},
                {CKA_DERIVE, &bTrue, sizeof(bTrue)},
                {CKA_PRIVATE, &bTrue, sizeof(bTrue)},
                {CKA_SENSITIVE, &bTrue, sizeof(bTrue)},
                {CKA_TOKEN, &bFalse, sizeof(bTrue)},
                {CKA_EXTRACTABLE, &bFalse, sizeof(bFalse)},
                {CKA_LABEL, aucLabel, (CK_ULONG)strlen((const char*)aucLabel)},
            };

            ckPublicTemplate[0].pValue = (CK_BYTE_PTR)pchOid;
            ckPublicTemplate[0].ulValueLen = ulOidLen;

            rvReturn = g_pCKFunctionList->C_GenerateKeyPair(hSession, &mechanism,
                                                            ckPublicTemplate, sizeof(ckPublicTemplate) / sizeof(CK_ATTRIBUTE),
                                                            ckPrivateTemplate, sizeof(ckPrivateTemplate) / sizeof(CK_ATTRIBUTE),
                                                            phPublicKey, phPrivateKey);
            if (rvReturn == CKR_OK)
            {
                if ((*phPublicKey == CK_INVALID_HANDLE) ||
                    (*phPrivateKey == CK_INVALID_HANDLE))
                {
                    rvReturn = CKR_HOST_MEMORY;
                    DBGPRINTF_WARN("C_GenerateKeyPair() returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
                }
            }
            else
            {
                DBGPRINTF_WARN("C_GenerateKeyPair() returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
            }
        }

        else
        {
            rvReturn = CKR_ARGUMENTS_BAD;
        }
    }
    return rvReturn;
}

/**
 * Retrieves the value and value length of an EC public key.
 *
 * @param hSession The session handle.
 * @param hPublicKey The handle of the EC public key.
 * @param pchValue A pointer to a buffer where the value will be stored.
 * @param pulValueLen A pointer to a size_t variable where the value length will be stored.
 * @return CK_RV The result of the operation; CKR_OK if successful.
 */
CK_RV PKCS11_EC_GetPublicKey(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE hPublicKey, uint8_t* pchValue, uint32_t* pulValueLen)
{
    CK_RV rvReturn;
    CK_ATTRIBUTE ckTemplate[] = {
        {CKA_EC_POINT, NULL_PTR, 0}};

    if (pulValueLen == NULL_PTR || pulValueLen == NULL_PTR)
    {
        rvReturn = CKR_ARGUMENTS_BAD;
    }
    else
    {

        rvReturn = g_pCKFunctionList->C_GetAttributeValue(hSession, hPublicKey, ckTemplate, 1);
        if (rvReturn != CKR_OK)
        {
            DBGPRINTF_WARN("C_GetAttributeValue() returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
        }
        else
        {

            *pulValueLen = ckTemplate[0].ulValueLen;
            ckTemplate[0].pValue = pchValue;

            rvReturn = g_pCKFunctionList->C_GetAttributeValue(hSession, hPublicKey, ckTemplate, 1);
            if (rvReturn != CKR_OK)
            {
                DBGPRINTF_WARN("C_GetAttributeValue() returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
            }
        }
    }

    return CKR_OK;
}

/**
 * Imports an EC public key as volatile into a token and sets a handle.
 *
 * @param hSession The session handle with the token.
 * @param pchCurveName The EC curve name.
 * @param pchPublicKey The public key data.
 * @param ulPublicKeySize The public key data length.
 * @param phPublicKey A pointer to where the public key handle will be stored.
 * @return CK_RV The return value of the PKCS#11 operation. CKR_OK on success.
 */
CK_RV PKCS11_EC_ImportPublicKey(CK_SESSION_HANDLE hSession, const char* pchCurveName, const CK_BYTE_PTR pchPublicKey, CK_ULONG ulPublicKeySize, CK_OBJECT_HANDLE_PTR phPublicKey)
{
    CK_RV rvReturn = CKR_FUNCTION_FAILED;
    CK_OBJECT_CLASS ckKeyClass = CKO_PUBLIC_KEY;
    CK_KEY_TYPE ckKeyType = CKK_EC;
    CK_BBOOL bTrue = CK_TRUE;
    CK_BBOOL bFalse = CK_FALSE;
    uint32_t ulOidLen = 0;

    if ((pchCurveName == NULL) || (pchPublicKey == NULL) || (phPublicKey == NULL))
    {
        rvReturn = CKR_ARGUMENTS_BAD;
    }
    else
    {
        CK_ATTRIBUTE ckTemplate[] = {
            {CKA_EC_PARAMS, NULL, 0},
            {CKA_CLASS, &ckKeyClass, sizeof(ckKeyClass)},
            {CKA_KEY_TYPE, &ckKeyType, sizeof(ckKeyType)},
            {CKA_TOKEN, &bFalse, sizeof(bFalse)},
            {CKA_ENCRYPT, &bTrue, sizeof(bTrue)},
            {CKA_VERIFY, &bTrue, sizeof(bTrue)},
            {CKA_EC_POINT, pchPublicKey, ulPublicKeySize}};
        const unsigned char* pchOid = EC_CRUVE_NAME_2_OID(pchCurveName, &ulOidLen);
        if (pchOid != NULL)
        {
            ckTemplate[0].pValue = (CK_BYTE_PTR)pchOid;
            ckTemplate[0].ulValueLen = ulOidLen;
            rvReturn = g_pCKFunctionList->C_CreateObject(hSession, ckTemplate, sizeof(ckTemplate) / sizeof(CK_ATTRIBUTE), phPublicKey);
            if (rvReturn != CKR_OK)
            {
                DBGPRINTF_WARN("C_CreateObject() returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
            }
        }
        else
        {
            rvReturn = CKR_ARGUMENTS_BAD;
        }
    }

    return rvReturn;
}

/**
 * Imports an EC private key as volatile into a token and sets a handle.
 *
 * @param hSession The session handle with the token.
 * @param pchCurveName The name of the EC curve.
 * @param pchPrivateKey The private key data.
 * @param ulPrivateKeySize The private key data length.
 * @param phPrivateKey A pointer to where the private key handle will be stored.
 * @return CK_RV The return value of the PKCS#11 operation. CKR_OK on success.
 */
CK_RV PKCS11_EC_ImportPrivateKey(CK_SESSION_HANDLE hSession, const char* pchCurveName, CK_BYTE_PTR pchPrivateKey, CK_ULONG ulPrivateKeySize, CK_OBJECT_HANDLE* phPrivateKey)
{
    CK_RV rvReturn = CKR_FUNCTION_FAILED;
    CK_OBJECT_CLASS ckKeyClass = CKO_PRIVATE_KEY;
    CK_KEY_TYPE ckKeyType = CKK_EC;
    CK_BBOOL bTrue = CK_TRUE;
    CK_BBOOL bFalse = CK_FALSE;
    uint32_t ulOidLen = 0;

    if ((pchPrivateKey == NULL) || (phPrivateKey == NULL))
    {
        rvReturn = CKR_ARGUMENTS_BAD;
    }
    else
    {
        if (strncmp((const char*)pchPrivateKey, PKCS11_OBJECT_PREFIX, PKCS11_OBJECT_PREFIX_LEN) == 0)
        {
            /* Find an object with given label and return the hanlde */
            rvReturn = PKCS11_Object_Find(hSession, CKO_PRIVATE_KEY, (const char*)pchPrivateKey, ulPrivateKeySize, phPrivateKey);
        }
        else
        {
            /* Create a transient object with given values */
            CK_ATTRIBUTE ckTemplate[] = {
                {CKA_EC_PARAMS, NULL, 0},
                {CKA_CLASS, &ckKeyClass, sizeof(ckKeyClass)},
                {CKA_KEY_TYPE, &ckKeyType, sizeof(ckKeyType)},
                {CKA_TOKEN, &bFalse, sizeof(bFalse)},
                {CKA_PRIVATE, &bTrue, sizeof(bTrue)},
                {CKA_SIGN, &bTrue, sizeof(bTrue)},
                {CKA_DECRYPT, &bTrue, sizeof(bTrue)},
                {CKA_DERIVE, &bTrue, sizeof(bTrue)},
                {CKA_VALUE, pchPrivateKey, ulPrivateKeySize}};
            const unsigned char* pchOid = EC_CRUVE_NAME_2_OID(pchCurveName, &ulOidLen);
            if (pchOid != NULL)
            {
                ckTemplate[0].pValue = (CK_BYTE_PTR)pchOid;
                ckTemplate[0].ulValueLen = ulOidLen;
                rvReturn = g_pCKFunctionList->C_CreateObject(hSession, ckTemplate, sizeof(ckTemplate) / sizeof(CK_ATTRIBUTE), phPrivateKey);
                if (rvReturn != CKR_OK)
                {
                    DBGPRINTF_WARN("C_CreateObject() returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
                }
            }
            else
            {
                rvReturn = CKR_ARGUMENTS_BAD;
            }
        }
    }

    return rvReturn;
}

/**
 * Retrieves the curve OID of an EC private key.
 *
 * @param hSession The session handle to the PKCS#11 token.
 * @param hPrivateKey The object handle for the EC private key.
 * @param pchOid A caller-allocated buffer where the curve OID will be stored.
 * @param pulOidLength Pointer to the length of the provided pchOid in bytes.
 *
 * @return CK_RV Returns CKR_OK on success, error code otherwise.
 */
CK_RV PKCS11_EC_GetCurveOID(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE hPrivateKey,
                            uint8_t* pchOid, uint32_t* pulOidLength)
{
    CK_ATTRIBUTE ckTemplate = {CKA_EC_PARAMS, NULL_PTR, 0};
    CK_RV rvReturn = CKR_FUNCTION_FAILED;
    uint8_t aucTempOid[PKCS11_MAX_OID_LENGTH] = {0};

    if ((pchOid == NULL) || (pulOidLength == NULL) || (*pulOidLength < PKCS11_MAX_OID_LENGTH))
    {
        rvReturn = CKR_ARGUMENTS_BAD;
    }
    else
    {
        ckTemplate.pValue = aucTempOid;
        ckTemplate.ulValueLen = *pulOidLength;
        rvReturn = g_pCKFunctionList->C_GetAttributeValue(hSession, hPrivateKey, &ckTemplate, 1U);
        if (rvReturn == CKR_OK)
        {
            uint32_t i;
            *pulOidLength = ckTemplate.ulValueLen;
            for (i = 0U; (i < ckTemplate.ulValueLen) && (i < *pulOidLength); ++i)
            {
                pchOid[i] = (uint8_t)aucTempOid[i];
            }
        }
        else
        {
            DBGPRINTF_WARN("C_GetAttributeValue() returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
        }
    }

    return rvReturn;
}

/**
 * Derives a shared secret key using Elliptic Curve Diffie-Hellman (ECDH) key agreement protocol.
 *
 * This function performs a key derivation operation using the ECDH protocol, based on the private key
 * and the public data provided.
 *
 * @param hSession The session handle.
 * @param hPrivKey The handle of the private key to be used for derivation.
 * @param pPublicData Pointer to the other party's public key data.
 * @param ulPublicDataLen Length of the other party's public key data in bytes.
 * @param ulSecretLen The length of the secret to be derived.
 * @param phSecret Pointer to the handle of the derived secret key object.
 *
 * @return CK_RV A return value of type CK_RV that indicates the result of the operation.
 *
 * @note Ensure that `pPublicData` points to valid memory of size `ulPublicDataLen` and that `phSecret`
 * is a valid pointer to receive the handle of the derived key. The caller is responsible for the
 * management of the derived key object's lifecycle, including its deallocation.
 */
CK_RV PKCS11_ECDH_DeriveKey(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE hPrivateKey,
                            CK_BYTE_PTR pPublicData, CK_ULONG ulPublicDataLen,
                            CK_ULONG ulSecretLen, CK_BYTE_PTR aucSecretLabel, CK_OBJECT_HANDLE* phSharedSecret)
{
    CK_RV rvReturn = CKR_GENERAL_ERROR;

    if (pPublicData != NULL && ulPublicDataLen > 0)
    {
        CK_ECDH1_DERIVE_PARAMS ckEcdhParams;
        CK_MECHANISM ckMechanism;
        CK_OBJECT_CLASS ckeyClass = CKO_SECRET_KEY;
        CK_KEY_TYPE cKeyType = CKK_GENERIC_SECRET;
        CK_BBOOL bTrue = CK_TRUE;
        CK_BBOOL bFalse = CK_FALSE;

        CK_ATTRIBUTE ckDeriveTemplate[] = {
            {CKA_CLASS, &ckeyClass, sizeof(ckeyClass)},
            {CKA_KEY_TYPE, &cKeyType, sizeof(cKeyType)},
            {CKA_SENSITIVE, &bFalse, sizeof(bFalse)},
            {CKA_VALUE_LEN, &ulSecretLen, sizeof(CK_ULONG)},
            {CKA_EXTRACTABLE, &bTrue, sizeof(bTrue)},
            {CKA_TOKEN, &bFalse, sizeof(bFalse)},
            {CKA_SENSITIVE, &bFalse, sizeof(bFalse)},
            {CKA_LABEL, aucSecretLabel, (CK_ULONG)strlen((const char*)aucSecretLabel)},
            {CKA_SIGN, &bTrue, sizeof(bTrue)},
            {CKA_VERIFY, &bTrue, sizeof(bTrue)},
            {CKA_DERIVE, &bTrue, sizeof(bTrue)},
        };

        ckMechanism.mechanism = CKM_ECDH1_DERIVE;
        ckMechanism.pParameter = &ckEcdhParams;
        ckMechanism.ulParameterLen = sizeof(ckEcdhParams);

        memset(&ckEcdhParams, 0, sizeof(ckEcdhParams));
        ckEcdhParams.kdf = CKD_NULL;
        ckEcdhParams.pPublicData = pPublicData;
        ckEcdhParams.ulPublicDataLen = ulPublicDataLen;

        rvReturn = g_pCKFunctionList->C_DeriveKey(hSession, &ckMechanism, hPrivateKey, ckDeriveTemplate,
                                                  sizeof(ckDeriveTemplate) / sizeof(CK_ATTRIBUTE), phSharedSecret);
        if (rvReturn != CKR_OK)
        {
            DBGPRINTF_WARN("C_DeriveKey() returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
        }
    }
    else
    {
        rvReturn = CKR_ARGUMENTS_BAD;
    }
    return rvReturn;
}

/**
 * Retrieves the value and value length of a PKCS#11 object.
 *
 * @param hSession The session handle.
 * @param hObject The handle of the object to query.
 * @param pchValue A pointer to a buffer where the value will be stored.
 * @param pulValueLen A pointer to a size_t variable where the value length will be stored.
 * @return CK_RV The result of the operation; CKR_OK if successful.
 */
CK_RV PKCS11_Object_GetValue(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE hObject, uint8_t* pchValue, uint32_t* pulValueLen)
{
    CK_RV rvReturn;
    CK_ATTRIBUTE ckTemplate[] = {
        {CKA_VALUE, NULL_PTR, 0}};

    if (pulValueLen == NULL_PTR || pulValueLen == NULL_PTR)
    {
        rvReturn = CKR_ARGUMENTS_BAD;
    }
    else
    {

        rvReturn = g_pCKFunctionList->C_GetAttributeValue(hSession, hObject, ckTemplate, 1);
        if (rvReturn != CKR_OK)
        {
            DBGPRINTF_WARN("C_GetAttributeValue() returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
        }
        else
        {

            *pulValueLen = ckTemplate[0].ulValueLen;

            ckTemplate[0].pValue = pchValue;

            rvReturn = g_pCKFunctionList->C_GetAttributeValue(hSession, hObject, ckTemplate, 1);
            if (rvReturn != CKR_OK)
            {
                DBGPRINTF_WARN("C_GetAttributeValue() returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
            }
        }
    }

    return CKR_OK;
}

/**
 * Generates a Diffie-Hellman key pair.
 *
 * This function generates a DH key pair using the provided prime (P) and generator (G) values.
 *
 * @param hSession Session handle.
 * @param phPrivKey Pointer to where the handle of the generated private key will be stored.
 * @param phPubKey Pointer to where the handle of the generated public key will be stored.
 * @param pchPrime Pointer to the prime value (P).
 * @param ulPrimeLen Length of the prime value.
 * @param pchBase Pointer to the generator value (G).
 * @param ulBaseLen Length of the generator value.
 * @return CK_RV Returns CKR_OK on success, error code otherwise.
 */
CK_RV PKCS11_DH_GenKeyPair(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE* phPrivKey, CK_OBJECT_HANDLE* phPubKey,
                           CK_BYTE* pchPrime, CK_ULONG ulPrimeLen, CK_BYTE* pchBase, CK_ULONG ulBaseLen)
{
    CK_RV rvReturn;
    if (phPrivKey == NULL || phPubKey == NULL || pchPrime == NULL || pchBase == NULL)
    {
        rvReturn = CKR_ARGUMENTS_BAD;
    }
    else
    {

        CK_MECHANISM ckMechanism = {CKM_DH_PKCS_KEY_PAIR_GEN, NULL_PTR, 0};
        CK_BBOOL bTrue = CK_TRUE;

        CK_ATTRIBUTE ckPubKeyTemplate[] = {
            {CKA_PRIME, pchPrime, ulPrimeLen},
            {CKA_BASE, pchBase, ulBaseLen}};

        CK_ATTRIBUTE ckPrivKeyTemplate[] = {
            {CKA_TOKEN, &bTrue, sizeof(bTrue)},
            {CKA_PRIVATE, &bTrue, sizeof(bTrue)},
            {CKA_SENSITIVE, &bTrue, sizeof(bTrue)},
            {CKA_EXTRACTABLE, &bTrue, sizeof(bTrue)}};

        rvReturn = g_pCKFunctionList->C_GenerateKeyPair(hSession, &ckMechanism,
                                                        ckPubKeyTemplate, sizeof(ckPubKeyTemplate) / sizeof(CK_ATTRIBUTE),
                                                        ckPrivKeyTemplate, sizeof(ckPrivKeyTemplate) / sizeof(CK_ATTRIBUTE),
                                                        phPubKey, phPrivKey);
        if (rvReturn != CKR_OK)
        {
            DBGPRINTF_WARN("C_GenerateKeyPair() returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
        }
    }
    return rvReturn;
}

/**
 * @brief Derives a shared key using Diffie-Hellman key agreement mechanism.
 *
 * Given a base key and the public key of another party, derives a shared secret key.
 *
 * @param hSession Session handle.
 * @param hPrivKey Handle to the base key for derivation.
 * @param pPublicData Pointer to the public key of the other party.
 * @param ulPublicDataLen Length of public key of the other party.
 * @param ulSecretLen Length of requested key.
 * @param[out] phSecret Pointer to where the handle of the derived key will be stored.
 * @return CK_RV Returns CKR_OK on success, or an error code on failure.
 */
CK_RV PKCS11_DH_DeriveKey(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE hPrivKey, CK_BYTE_PTR pPublicData, CK_ULONG ulPublicDataLen, CK_ULONG ulSecretLen, CK_OBJECT_HANDLE* phSecret)
{
    CK_RV rvReturn = CKR_GENERAL_ERROR;

    if (pPublicData != NULL && ulPublicDataLen > 0)
    {
        CK_ECDH1_DERIVE_PARAMS ckDHParams;
        CK_MECHANISM ckMechanism = {CKM_DH_PKCS_DERIVE, NULL_PTR, 0};
        ;
        CK_OBJECT_CLASS ckeyClass = CKO_SECRET_KEY;
        CK_KEY_TYPE cKeyType = CKK_GENERIC_SECRET;
        CK_BBOOL bTrue = CK_TRUE;
        CK_BBOOL bFalse = CK_FALSE;

        CK_ATTRIBUTE ckDeriveTemplate[] = {
            {CKA_CLASS, &ckeyClass, sizeof(ckeyClass)},
            {CKA_KEY_TYPE, &cKeyType, sizeof(cKeyType)},
            {CKA_SENSITIVE, &bFalse, sizeof(bFalse)},
            {CKA_VALUE_LEN, &ulSecretLen, sizeof(CK_ULONG)},
            {CKA_EXTRACTABLE, &bTrue, sizeof(bTrue)},
            {CKA_TOKEN, &bFalse, sizeof(bFalse)},
            {CKA_SENSITIVE, &bFalse, sizeof(bFalse)},
            {CKA_SIGN, &bTrue, sizeof(bTrue)},
            {CKA_VERIFY, &bTrue, sizeof(bTrue)},
            {CKA_DERIVE, &bTrue, sizeof(bTrue)},
        };

        ckMechanism.pParameter = &ckDHParams;
        ckMechanism.ulParameterLen = sizeof(ckDHParams);

        memset(&ckDHParams, 0, sizeof(ckDHParams));
        ckDHParams.kdf = CKD_NULL;
        ckDHParams.pPublicData = pPublicData;
        ckDHParams.ulPublicDataLen = ulPublicDataLen;

        rvReturn = g_pCKFunctionList->C_DeriveKey(hSession, &ckMechanism, hPrivKey, ckDeriveTemplate,
                                                  sizeof(ckDeriveTemplate) / sizeof(CK_ATTRIBUTE), phSecret);
        if (rvReturn != CKR_OK)
        {
            DBGPRINTF_WARN("C_DeriveKey() returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
        }
    }
    else
    {
        rvReturn = CKR_ARGUMENTS_BAD;
    }
    return rvReturn;
}

/**
 * Imports an DH private key as volatile into a token and sets a handle.
 *
 * @param hSession The session handle with the token.
 * @param pchPrime The prime (P) data.
 * @param ulPrimeLen The prime data length.
 * @param pchBase The base (G) data.
 * @param ulBaseLen The base data length.
 * @param pchPrivKey The private key (Y) data.
 * @param ulPrivKeyLen The private key data length.
 * @param hPrivKey A pointer to where the private key handle will be stored.
 * @return CK_RV The return value of the PKCS#11 operation. CKR_OK on success.
 */
CK_RV PKCS11_DH_ImportKey(CK_SESSION_HANDLE hSession,
                          CK_BYTE_PTR pchPrime, CK_ULONG ulPrimeLen,
                          CK_BYTE_PTR pchBase, CK_ULONG ulBaseLen,
                          CK_BYTE_PTR pchPrivKey, CK_ULONG ulPrivKeyLen,
                          CK_OBJECT_HANDLE* hPrivKey)
{
    CK_RV rvReturn = CKR_GENERAL_ERROR;
    CK_OBJECT_CLASS ckKeyClass = CKO_PRIVATE_KEY;
    CK_KEY_TYPE ckKeyType = CKK_DH;
    CK_BBOOL bTrue = CK_TRUE;
    CK_BBOOL bFalse = CK_FALSE;

    if ((pchPrime == NULL) || (pchBase == NULL) || (pchPrivKey == NULL) || (hPrivKey == NULL))
    {
        rvReturn = CKR_ARGUMENTS_BAD;
    }
    else
    {
        /* Template for the DH private key */
        CK_ATTRIBUTE ckPrivKeyTemplate[] = {
            {CKA_CLASS, &ckKeyClass, sizeof(ckKeyClass)},
            {CKA_KEY_TYPE, &ckKeyType, sizeof(ckKeyType)},
            {CKA_TOKEN, &bFalse, sizeof(bFalse)},
            {CKA_PRIVATE, &bTrue, sizeof(bTrue)},
            {CKA_DERIVE, &bTrue, sizeof(bTrue)},
            {CKA_PRIME, pchPrime, ulPrimeLen},
            {CKA_BASE, pchBase, ulBaseLen},
            {CKA_VALUE, pchPrivKey, ulPrivKeyLen}};

        rvReturn = g_pCKFunctionList->C_CreateObject(hSession, ckPrivKeyTemplate, sizeof(ckPrivKeyTemplate) / sizeof(CK_ATTRIBUTE), hPrivKey);
        if (rvReturn != CKR_OK)
        {
            DBGPRINTF_WARN("C_CreateObject() returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
        }
    }

    return rvReturn;
}

/**
 * Find an object by its label and class.
 *
 * This function searches for an object within a session that matches a given label.
 *
 * @param hSession The session's handle.
 * @param ckClass The object's class.
 * @param aucKeyLabel A pointer to the array containing the label of the object to find.
 * @param ulSizeKeyLabelSize The size of the label pointed to by aucKeyLabel, in bytes.
 * @param phKey An output parameter. On successful return, *phKey contains the handle of the first object that matches the label. If no object matches the label, *phKey is set to an invalid object handle.
 *
 * @return CKR_OK if the function succeeded, or an error code if the operation failed.
 */
CK_RV PKCS11_Object_Find(CK_SESSION_HANDLE hSession, CK_OBJECT_CLASS ckClass,
                         char const* aucKeyLabel, CK_ULONG ulSizeKeyLabelSize,
                         CK_OBJECT_HANDLE_PTR phKey)
{
    CK_RV rvReturn = CKR_FUNCTION_FAILED;
    CK_ULONG ulObjectCount;

    UNUSED_VAR(ulSizeKeyLabelSize);

    if (aucKeyLabel == NULL || phKey == NULL)
    {
        rvReturn = CKR_ARGUMENTS_BAD;
    }
    else
    {
        CK_ATTRIBUTE ckTemplate[] = {
            {CKA_CLASS, &ckClass, sizeof(CK_OBJECT_CLASS)},
            {CKA_LABEL, (CK_VOID_PTR)aucKeyLabel, strlen((const char*)aucKeyLabel)},
        };

        rvReturn = g_pCKFunctionList->C_FindObjectsInit(hSession, ckTemplate, sizeof(ckTemplate) / sizeof(CK_ATTRIBUTE));
        if (rvReturn == CKR_OK)
        {
            rvReturn = g_pCKFunctionList->C_FindObjects(hSession, phKey, 1, &ulObjectCount);
            if (rvReturn == CKR_OK)
            {
                rvReturn = g_pCKFunctionList->C_FindObjectsFinal(hSession);
                if (rvReturn == CKR_OK)
                {
                    if (ulObjectCount == 0)
                    {
                        *phKey = CK_INVALID_HANDLE;
                    }
                }
                else
                {
                    DBGPRINTF_WARN("C_FindObjectsFinal() returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
                }
            }
            else
            {
                *phKey = CK_INVALID_HANDLE;
                DBGPRINTF_WARN("C_FindObjects() returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
            }
        }
        else
        {
            DBGPRINTF_WARN("C_FindObjectsInit() returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
        }
    }
    return rvReturn;
}

/**
 * Generate a label for a PKCS#11 object.
 *
 * The object label consists of the prefix "sss:" followed by an hexadecimal
 * representation of a static local address that increments on each function
 * invocation.
 *
 * @param acOutputLabel A pointer to a buffer where the generated label will be stored.
 * @param ulOutputLabelMaxSize The size of the output label buffer in bytes.
 *
 * @return CK_RV Return value (CKR_OK on success, error code otherwise).
 */
CK_RV PKCS11_Object_GenerateLabel(char acOutputLabel[], uint32_t ulOutputLabelMaxSize)
{
    CK_RV rvReturn = CKR_FUNCTION_FAILED;

    /*
     * On SE05x, avoid global ranges:
     * [00000000 - 00000000],
     * [7FFF0200 - 7FFF0202],
     * [7FFF0204 - 7FFF020B],
     * [F0000000 - FFFFFFFF]
     */
    static uint32_t uObjectAddress = 0x00001001;

    if (acOutputLabel == NULL)
    {
        rvReturn = CKR_ARGUMENTS_BAD;
    }
    else
    {
        snprintf(acOutputLabel, ulOutputLabelMaxSize, "%s%08X", PKCS11_OBJECT_PREFIX, uObjectAddress);
        uObjectAddress++; /* Increment the address for the next call */
        rvReturn = CKR_OK;
    }

    return rvReturn;
}

/**
 * Set the label of a PKCS#11 object with a prefix followed by a random hex string.
 *
 * This function generates a random hex string, prepends it with the "sss:" prefix,
 * and sets this string as the label of the given PKCS#11 object. The caller
 * must provide a sufficiently large buffer for the output label.
 *
 * @param hSession The session handle to access the PKCS#11 token.
 * @param hObject Pointer to object handle of the PKCS#11 object to be labeled.
 * @param aucLabel A pointer to a buffer where the label to use is stored.
 * @return CK_RV Return value (CKR_OK on success, error code otherwise).
 */
CK_RV PKCS11_Object_SetLabel(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE_PTR hObject, char* aucLabel)
{
    CK_RV rvReturn = CKR_FUNCTION_FAILED;

    if (hObject == NULL || aucLabel == NULL)
    {
        rvReturn = CKR_ARGUMENTS_BAD;
    }
    else
    {
        CK_ATTRIBUTE ckAttribute = {CKA_LABEL, aucLabel, strlen((const char*)aucLabel)};

        rvReturn = g_pCKFunctionList->C_SetAttributeValue(hSession, *hObject, &ckAttribute, 1);
        if (rvReturn != CKR_OK)
        {
            DBGPRINTF_WARN("C_SetAttributeValue() returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
        }
    }
    return rvReturn;
}

/**
 * Deletes a PKCS#11 object given its handle.
 *
 * @param hSession An active PKCS#11 session handle.
 * @param hObject The handle of the object to be deleted.
 * @return CK_RV Return value (CKR_OK on success, error code otherwise).
 */
CK_RV PKCS11_Object_Destroy(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE hObject)
{
    CK_RV rvReturn = CKR_OK;

    if ((hSession == CK_INVALID_HANDLE) || (hObject == CK_INVALID_HANDLE))
    {
        rvReturn = CKR_ARGUMENTS_BAD;
    }
    else
    {
        rvReturn = g_pCKFunctionList->C_DestroyObject(hSession, hObject);
        if (rvReturn != CKR_OK)
        {
            DBGPRINTF_WARN("C_DestroyObject() returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
        }
    }

    return rvReturn;
}

/**
 * Generates a digital signature of specified data using a specified signing key and algorithm.
 *
 * @param hSession The session's handle.
 * @param hPrivateKey The handle of the private key that will be used to sign the data.
 * @param ckMechanismType The cryptographic mechanism to be used for signing (e.g CKM_ECDSA).
 * @param pchData Pointer to the data that will be signed.
 * @param ulDataLen Length in bytes of the data to be signed.
 * @param pchSignature Pointer to the buffer where the generated signature will be stored.
 * @param pulSignatureLen Pointer to the location where the length of the signature is stored.
 *
 * @return CKR_OK on success, error code otherwise.
 */
CK_RV PKCS11_Signature_Generate(CK_SESSION_HANDLE hSession,
                                CK_OBJECT_HANDLE hPrivateKey, CK_MECHANISM_TYPE ckMechanismType,
                                CK_BYTE_PTR pchData, CK_ULONG ulDataLen,
                                CK_BYTE_PTR pchSignature, CK_ULONG_PTR pulSignatureLen)
{
    CK_RV rvReturn = CKR_FUNCTION_FAILED;
    CK_MECHANISM ckMechanism;

    ckMechanism.mechanism = ckMechanismType;
    ckMechanism.ulParameterLen = 0;
    ckMechanism.pParameter = NULL;

    rvReturn = g_pCKFunctionList->C_SignInit(hSession, &ckMechanism, hPrivateKey);
    if (CKR_OK == rvReturn)
    {
        rvReturn = g_pCKFunctionList->C_Sign(hSession, pchData, ulDataLen, pchSignature, pulSignatureLen);
        if (rvReturn != CKR_OK)
        {
            DBGPRINTF_WARN("C_Sign() returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
        }
    }
    else
    {
        DBGPRINTF_WARN("C_SignInit(Sig) returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
    }

    return rvReturn;
}

/**
 * Verifies a digital signature against given data using a specified public key and algorithm..
 *
 * @param hSession The session's handle.
 * @param hPublicKey The handle of the public key that will be used to verify the signature.
 * @param ckMechanismType The cryptographic mechanism to be used for verification (e.g CKM_ECDSA).
 * @param pchData Pointer to the original data that is being verified against the signature.
 * @param ulDataLen Length in bytes of the data whose signature is to be verified.
 * @param pchSignature Pointer to the signature that will be verified against the data.
 * @param ulSignatureLen Length in bytes of the signature.
 *
 * @return CKR_OK on success, error code otherwise.
 */
CK_RV PKCS11_Signature_Verify(CK_SESSION_HANDLE hSession,
                              CK_OBJECT_HANDLE hPublicKey, CK_MECHANISM_TYPE ckMechanismType,
                              CK_BYTE_PTR pchData, CK_ULONG ulDataLen,
                              CK_BYTE_PTR pchSignature, CK_ULONG ulSignatureLen)
{
    CK_RV rvReturn = CKR_FUNCTION_FAILED;
    CK_MECHANISM ckMechanism;

    ckMechanism.mechanism = ckMechanismType;
    ckMechanism.ulParameterLen = 0;
    ckMechanism.pParameter = NULL;

    rvReturn = g_pCKFunctionList->C_VerifyInit(hSession, &ckMechanism, hPublicKey);
    if (CKR_OK == rvReturn)
    {
        rvReturn = g_pCKFunctionList->C_Verify(hSession, pchData, ulDataLen, pchSignature, ulSignatureLen);
        if (rvReturn != CKR_OK)
        {
            DBGPRINTF_WARN("C_Verify() returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
        }
    }
    else
    {
        DBGPRINTF_WARN("C_VerifyInit() returns 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
    }

    return rvReturn;
}

/**
 * Initializes the PKCS#11 module.
 *
 * This function initializes the PKCS#11 module.
 * It is assumed that this function is called before loading the PKCS#11
 * library and performing any cryptographic operations.
 */
bool nxpPKCS11_Init()
{
    bool bSuccess = false;

    DBGPRINT_NOTICE("nxpPKCS11_Init()");

    g_pchTokenName = "EasyEVSE";
    g_pchUserPin = "0000";

    CK_RV rvReturn = PKCS11_Initialize(NULL);
    if (CKR_OK == rvReturn)
    {
    	DBGPRINTF_INFO("nxpPKCS11_Init() successful 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
    	bSuccess = true;
    }
    else
    {
    	DBGPRINTF_CRIT("nxpPKCS11_Init() failed 0x%lx:(%s)", rvReturn, CKR_TO_STR(rvReturn));
    }

    return bSuccess;
}
#endif /* #if (PKCS11_SUPPORTED == 1) */
#endif /* ENABLE_ISO15118 */
