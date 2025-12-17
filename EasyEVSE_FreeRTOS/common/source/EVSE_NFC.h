/*
 * Copyright 2023-2025 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply
 * with and are bound by, such license terms. If you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#ifndef EVSE_NFC_H
#define EVSE_NFC_H

#define NFC_CALLBACKS_NUMBER 5

/* CARD_MAX_SIZE_UID should be MAX(PHAC_DISCLOOP_I3P3B_PUPI_LENGTH, PHAC_DISCLOOP_I3P3A_MAX_UID_LENGTH,
 * PHAC_DISCLOOP_FELICA_IDM_LENGTH, PHAC_V_DISCLOOP_MAX_UID_LENGTH) * ASCII size + NULL terminator */
#define CARD_MAX_SIZE_UID (10 * 2 + 1)

#define CARD_NOT_PRESENT "N/A"

// CARD security
#define CARD_NOT_SECURED "Card not secured"
#define CARD_ACCEPTED    "Card Accepted"

// CARD TYPE
#define CARD_MIFARE_DUOX        "MIFARE DUOX"
#define CARD_DESFIRE_LIGTH       "MIFARE DESFire Light"
#define CARD_DESFIRE_EV3         "MIFARE DESFire EV3"
#define CARD_DESFIRE_EV1        "MIFARE DESFire EV1"
#define CARD_MIFARE_ULTRALIGHT  "MIFARE Ultralight"
#define CARD_GENERIC            "Generic card"
#define CARD_COPY               "Card is a copy"

// Card rejection messages
#define EMPTY_CARD  "Card rejected - Empty card"
#define READ_FAIL   "Card rejected - Reading data failed"
#define SIGN_FAIL   "Card rejected - Challenge sign failed"
#define CHALLENGE_VERIFY_FAIL "Card rejected - Challenge verify failed"
#define PARSE_FAIL  "Card rejected - Certificate parse failed"
#define CERT_VERIFY_FAIL "Card rejected - Certificate verify failed"

#define LPCD_CONFIGURE_INTERVAL 200

/*! @brief NFC retrieved data structure. */
typedef struct _nfc_data_info
{
    char CardUID[CARD_MAX_SIZE_UID];
    char CardTek[2];  /* A, B, F, V*/
    char CardType[4]; /* 1 , 2, 3, P2P */
    uint8_t SizeUID;
    char *ExactCardType;
    char *isSecured;
} nfc_data_info_t;

typedef struct _nfc_whitelist_info
{
    char UID[CARD_MAX_SIZE_UID];
    char *ExactType;
} nfc_whitelist_info_t;

typedef enum
{
    /** Operation was successful */
    kNFCStatus_Success = 0x0,
    /** Operation failed */
    kNFCStatus_Fail = 0x1,
} nfc_status_t;

typedef void (*CardDetect)(uint8_t *CardUID, uint8_t SizeUID);

/**
 * @brief Register a callback that is going to be called when a card is detect
 *
 * @param cb Callback to the function
 */
void EVSE_NFC_RegisterCallbackFunction(CardDetect cb);

/**
 * @brief Set the vehicleId from an external source
 *
 * @param cardUID vehicleId
 * @param size vehicleId size
 */

void EVSE_NFC_SetVehicleID(uint8_t *cardUID, uint8_t size);

/**
 * @brief Clear the vehicle ID
 *
 */
void EVSE_NFC_ClearVehicleID(void);
/*
 * NFC task entry point.
 * Handles interaction to an NFC capable device

 */
void EVSE_NFC_Init(void);

/*
 * @brief Retrieves Vehicle ID from NFC data.
 */
const char *EVSE_NFC_Get_VehicleID();

/*
 * @brief Retrieves card type from NFC data.
 */
const char *EVSE_NFC_Get_CardType();

/*
 * @brief Checks if the card is secure
 */
const char *EVSE_NFC_Get_CardSecurityStatus();

/*
 * @brief Returns if NFC read should be activated
 */
bool EVSE_NFC_getNFCActivationStatus();

/*
 * @brief Activated NFC read
 */
void EVSE_NFC_setNFCActivationStatus(bool activateNFC);

bool EVSE_NFC_isWhitelisted(bool *auth_result);

#endif /* EVSE_NFC_H */
