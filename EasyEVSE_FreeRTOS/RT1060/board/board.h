/*
 * Copyright 2018-2023, 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _BOARD_H_
#define _BOARD_H_

#include "clock_config.h"
#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_clock.h"
#include "fsl_lpspi.h"
#include "fsl_flexspi.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief The board name */
#define BOARD_NAME "MIMXRT1060-EVK"

/*! @brief The board revision version */
#ifndef MIMXRT1060_REVA
#define MIMXRT1060_REVA 0
#endif
#ifndef MIMXRT1060_REVB
#define MIMXRT1060_REVB 1
#endif
#ifndef MIMXRT1060_REVC
#define MIMXRT1060_REVC 0
#endif

#define BOARD_FLEXSPI       FLEXSPI
#define BOARD_FLEXSPI_BASE  FlexSPI_AMBA_BASE


/* The UART to use for debug messages. */
#define BOARD_DEBUG_UART_CLK_FREQ BOARD_DebugConsoleSrcFreq()
#define BOARD_DEBUG_UART_TYPE     kSerialPort_Uart
#ifndef BOARD_DEBUG_UART_BASEADDR
#define BOARD_DEBUG_UART_BASEADDR (uint32_t) LPUART1
#endif
#ifndef BOARD_DEBUG_UART_INSTANCE
#define BOARD_DEBUG_UART_INSTANCE 1U
#endif
#ifndef BOARD_UART_IRQ
#define BOARD_UART_IRQ LPUART1_IRQn
#endif
#ifndef BOARD_UART_IRQ_HANDLER
#define BOARD_UART_IRQ_HANDLER LPUART1_IRQHandler
#endif
#ifndef BOARD_DEBUG_UART_BAUDRATE
#define BOARD_DEBUG_UART_BAUDRATE (230400U)
#endif

/*! @brief The USER_LED used for board */
#define LOGIC_LED_ON  (0U)
#define LOGIC_LED_OFF (1U)
#ifndef BOARD_USER_LED_GPIO
#define BOARD_USER_LED_GPIO GPIO1
#endif
#ifndef BOARD_USER_LED_GPIO_PIN
#define BOARD_USER_LED_GPIO_PIN (9U)
#endif

#define USER_LED_INIT(output)                                            \
    GPIO_PinWrite(BOARD_USER_LED_GPIO, BOARD_USER_LED_GPIO_PIN, output); \
    BOARD_USER_LED_GPIO->GDIR |= (1U << BOARD_USER_LED_GPIO_PIN) /*!< Enable target USER_LED */
#define USER_LED_ON() \
    GPIO_PortClear(BOARD_USER_LED_GPIO, 1U << BOARD_USER_LED_GPIO_PIN)                  /*!< Turn off target USER_LED */
#define USER_LED_OFF() GPIO_PortSet(BOARD_USER_LED_GPIO, 1U << BOARD_USER_LED_GPIO_PIN) /*!<Turn on target USER_LED*/
#define USER_LED_TOGGLE()                                       \
    GPIO_PinWrite(BOARD_USER_LED_GPIO, BOARD_USER_LED_GPIO_PIN, \
                  0x1 ^ GPIO_PinRead(BOARD_USER_LED_GPIO, BOARD_USER_LED_GPIO_PIN)) /*!< Toggle target USER_LED */

/*! @brief Define the port interrupt number for the board switches */
#ifndef BOARD_USER_BUTTON_GPIO_PORT
#define BOARD_USER_BUTTON_GPIO_PORT (5U)
#endif
#ifndef BOARD_USER_BUTTON_GPIO
#define BOARD_USER_BUTTON_GPIO GPIO_BASE_PTRS[BOARD_USER_BUTTON_GPIO_PORT]
#endif
#ifndef BOARD_USER_BUTTON_GPIO_PIN
#define BOARD_USER_BUTTON_GPIO_PIN (0U)
#endif
#define BOARD_USER_BUTTON_IRQ         GPIO5_Combined_0_15_IRQn
#define BOARD_USER_BUTTON_IRQ_HANDLER GPIO5_Combined_0_15_IRQHandler
#define BOARD_USER_BUTTON_NAME        "SW8"

/*! @brief The board flash size */
#define BOARD_FLASH_SIZE (0x800000U)

/* USB PHY condfiguration */
#define BOARD_USB_PHY_D_CAL     (0x0CU)
#define BOARD_USB_PHY_TXCAL45DP (0x06U)
#define BOARD_USB_PHY_TXCAL45DM (0x06U)

#define BOARD_ARDUINO_INT_IRQ   (GPIO1_INT3_IRQn)
#define BOARD_ARDUINO_I2C_IRQ   (LPI2C1_IRQn)
#define BOARD_ARDUINO_I2C_INDEX (1)

/* @Brief Board accelerator sensor configuration */
#define BOARD_ACCEL_I2C_BASEADDR LPI2C1

/* Select USB1 PLL (480 MHz) as LPI2C's clock source */
#define BOARD_ACCEL_I2C_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for LPI2C clock source */
#define BOARD_ACCEL_I2C_CLOCK_SOURCE_DIVIDER (5U)
#define BOARD_ACCEL_I2C_CLOCK_FREQ           (CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8 / (BOARD_ACCEL_I2C_CLOCK_SOURCE_DIVIDER + 1U))

#define BOARD_CODEC_I2C_BASEADDR             LPI2C1
#define BOARD_CODEC_I2C_INSTANCE             1U
#define BOARD_CODEC_I2C_CLOCK_SOURCE_SELECT  (0U)
#define BOARD_CODEC_I2C_CLOCK_SOURCE_DIVIDER (5U)
#define BOARD_CODEC_I2C_CLOCK_FREQ           (10000000U)

/* @Brief Board CAMERA configuration */
#define BOARD_CAMERA_I2C_BASEADDR             LPI2C1
#define BOARD_CAMERA_I2C_CLOCK_SOURCE_DIVIDER (5U)
#define BOARD_CAMERA_I2C_CLOCK_SOURCE_SELECT  (0U) /* Select USB1 PLL (480 MHz) as LPI2C's clock source */
#define BOARD_CAMERA_I2C_CLOCK_FREQ \
    (CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8 / (BOARD_CAMERA_I2C_CLOCK_SOURCE_DIVIDER + 1U))

#define BOARD_CAMERA_I2C_SCL_GPIO GPIO1
#define BOARD_CAMERA_I2C_SCL_PIN  16
#define BOARD_CAMERA_I2C_SDA_GPIO GPIO1
#define BOARD_CAMERA_I2C_SDA_PIN  17
#define BOARD_CAMERA_PWDN_GPIO    GPIO1
#define BOARD_CAMERA_PWDN_PIN     18

/* @Brief Board touch panel configuration */
#define BOARD_TOUCH_I2C_BASEADDR LPI2C1
#define BOARD_TOUCH_RST_GPIO     GPIO1
#define BOARD_TOUCH_RST_PIN      2
#define BOARD_TOUCH_INT_GPIO     GPIO1
#define BOARD_TOUCH_INT_PIN      11

/* @Brief Board Bluetooth HCI UART configuration */
#define BOARD_BT_UART_BASEADDR    LPUART3
#define BOARD_BT_UART_INSTANCE    3
#define BOARD_BT_UART_BAUDRATE    3000000
#define BOARD_BT_UART_CLK_FREQ    BOARD_DebugConsoleSrcFreq()
#define BOARD_BT_UART_IRQ         LPUART3_IRQn
#define BOARD_BT_UART_IRQ_HANDLER LPUART3_IRQHandler

/*! @brief board has sdcard */
#define BOARD_HAS_SDCARD (1U)

/* In order for the NFC to be used over the ARDUINO Header
 * the SPI lines need to be enabled by soldering 0 Ohm resistors over SPI lines.
 * RT1060 EVK-A R281, R279, R278, R280
 * RT1060 EVK-B
 */

/******************************************************************
 * LPSPI config
 ******************************************************************/
#define LPSPI_DATA_RATE 1200000U

/* Select USB1 PLL PFD0 (720 MHz) as lpspi clock source */
#define LPSPI_CLOCK_SOURCE_SELECT (1U)
/* Clock divider for master lpspi clock source */
#define LPSPI_CLOCK_SOURCE_DIVIDER (7U)

#define LPSPI_CLOCK_FREQ (CLOCK_GetFreq(kCLOCK_Usb1PllPfd0Clk) / (LPSPI_CLOCK_SOURCE_DIVIDER + 1U))

#define LPSPI_MASTER_CLOCK_FREQ LPSPI_CLOCK_FREQ

#define BOARD_NFC_RESET ((GPIO_PORT_A << 8) | 24) /**< RC663 nPDOWN,  GPIO1 b24, J35 P13   */
#define BOARD_NFC_IF0   ((GPIO_PORT_A << 8) | 20) /**< IFSEL0, GPIO1 P20, not used on CLRC663ARD  */
#define BOARD_NFC_IF1   ((GPIO_PORT_A << 8) | 21) /**< IFSEL1, GPIO1 P21, not used on CLRC663ARD  */


/* Need to check if the Lumissil chip can go higher with the rate */
#define BOARD_NFC_IMX_SPI_DATA_RATE    LPSPI_DATA_RATE
#define BOARD_NFC_PIN_IRQ_TRIGGER_TYPE PH_DRIVER_INTERRUPT_FALLINGEDGE
#define BOARD_NFC_IMX_SPI_CLK_SRC      LPSPI_MASTER_CLOCK_FREQ

#if (BOARD_NFC_ARDUINO_HEADER == 1)
#define BOARD_NFC_IRQ   ((GPIO_PORT_A << 8) | 03) /**< RC663 IRQ,     GPIO1 b03, J24 P1 */
#define BOARD_NFC_IRQn                 GPIO1_Combined_0_15_IRQn
#define BOARD_NFC_INT_PRIORITY         3
#define BOARD_NFC_IRQHandler           GPIO1_Combined_0_15_IRQHandler

#define BOARD_NFC_SPI_BASE          LPSPI1_BASE
#define BOARD_NFC_SPI               LPSPI1
#define BOARD_NFC_IMX_SPI_IRQ       LPSPI1_IRQn
#define BOARD_NFC_PIN_SSEL          ((GPIO_PORT_C << 8) | 13) /* Chip select pin 13 gpio3. This is the same as SPI1 PCS */
#define BOARD_NFC_PCS               kLPSPI_Pcs0
#else
#define BOARD_NFC_IRQ                   ((GPIO_PORT_A << 8) | 25) /**< RC663 IRQ,     GPIO1 b25, J35 P11 */
#define BOARD_NFC_IRQn                  GPIO1_Combined_16_31_IRQn
#define BOARD_NFC_INT_PRIORITY          3
#define BOARD_NFC_IRQHandler            GPIO1_Combined_16_31_IRQHandler

#define BOARD_NFC_SPI_BASE            LPSPI3_BASE
#define BOARD_NFC_SPI               LPSPI3
#define BOARD_NFC_IMX_SPI_IRQ       LPSPI3_IRQn
#define BOARD_NFC_PIN_SSEL          ((GPIO_PORT_A << 8) | 27) /* Chip select pin 27 gpio1 GPIO_AD_B1_11*/
#define BOARD_NFC_PCS                kLPSPI_Pcs1
#endif /* BOARD_NFC_ARDUINO_HEADER */

#define BOARD_CP_TOGGLE_PORT    GPIO1
#define BOARD_CP_TOGGLE_PIN     27U

/* SIGBOARD */

#define SIGBOARD_SPI_RX_IRQ_PORT     GPIO1
#define SIGBOARD_SPI_RX_IRQ_PIN      20U /* SPI RX IRQ pin 20 gpio 1 GPIO_AD_B1_04 */

#define SIGBOARD_HGPG_RESET_PORT     GPIO1
#define SIGBOARD_HGPG_RESET_PIN      21U /* HGPG reset pin 21 gpio 1 GPIO_AD_B1_05 */

#define SIGBOARD_HGPG_GP_IRQ_PORT    GPIO1
#define SIGBOARD_HGPG_GP_IRQ_PIN     26U /* HGPG GP IRQ pin 26 gpio 1 GPIO_AD_B1_10 */

#define SIGBOARD_SPI_DATA_RATE      LPSPI_DATA_RATE
#define SIGBOARD_SPI_CLK_SRC        LPSPI_MASTER_CLOCK_FREQ
#define SIGBOARD_SPI_LSB_FIRST      (1U)

#if defined(BOARD_SIGBOARD_ARDUINO_HEADER) && (BOARD_SIGBOARD_ARDUINO_HEADER == 1)
#define SIGBOARD_SPI_BASE                LPSPI1_BASE
#define SIGBOARD_SPI                     LPSPI1
#define SIGBOARD_SPI_IMX_SPI_IRQ         LPSPI1_IRQn
#define SIGBOARD_SPI_CS_PORT             GPIO3
#define SIGBOARD_SPI_CS_PIN              13U /* Chip select pin 13 gpio 3 GPIO_SD_B0_01 same as LPSPI1_PCS0*/
#elif defined(BOARD_SIGBOARD_ARDUINO_HEADER) && (BOARD_SIGBOARD_ARDUINO_HEADER == 0)
#define SIGBOARD_SPI_BASE                LPSPI3_BASE
#define SIGBOARD_SPI                     LPSPI3
#define SIGBOARD_SPI_IMX_SPI_IRQ         LPSPI3_IRQn
#define SIGBOARD_SPI_CS_PORT             GPIO1
#define SIGBOARD_SPI_CS_PIN              28U /* Chip select pin 28 gpio 1 GPIO_AD_B1_12 same as LPSPI3_PCS0*/
#else
#endif

#define SIGBRD_LPUART_Base       LPUART3_BASE
#define SIGBRD_LPUART            LPUART3
#define SIGBRD_LPUART_CLK_FREQ   BOARD_DebugConsoleSrcFreq()
#define SIGBRD_LPUART_IRQn       LPUART3_IRQn
#define SIGBRD_LPUART_IRQHandler LPUART3_IRQHandler

#define METER_LPUART_Base		LPUART3_BASE
#define METER_LPUART            LPUART3
#define METER_LPUART_CLK_FREQ   BOARD_DebugConsoleSrcFreq()
#define METER_LPUART_IRQn       LPUART3_IRQn
#define METER_LPUART_IRQHandler LPUART3_IRQHandler

#if (METER_LPUART_Base == SIGBRD_LPUART_Base)
#define METER_SIGBOARD_LPUART_SHARED 1
#else
#define METER_SIGBOARD_LPUART_SHARED 0
#endif /* (METER_LPUART_Base == SIGBRD_LPUART_Base) */

/* FILESYSTEM */

#ifndef FS_BASE_ADDRESS
#define FS_BASE_ADDRESS 0x700000
#endif

#ifndef FS_SIZE
#define FS_SIZE 0x100000
#endif

/* Ethernet configuration. */

/*! @brief The ENET PHY address. */
#define BOARD_ENET0_PHY_ADDRESS (0x02U) /* Phy address of enet port 0. */

#define BOARD_ENET_INT_GPIO     (GPIO1)
#define BOARD_ENET_INIT_PIN     (10U)
#define BOARD_ENET_RESET_GPIO   (GPIO1)
#define BOARD_ENET_RESET_PIN    (9U)

#define EVSE_ETH_ENET         ENET
#define EVSE_ETH_PHY_ADDRESS  BOARD_ENET0_PHY_ADDRESS
#define EVSE_ETH_PHY_OPS      &phyksz8081_ops
#define EVSE_ETH_PHY_RESOURCE &s_phy_resource
#define EVSE_ETH_CLOCK_FREQ   CLOCK_GetFreq(kCLOCK_IpgClk)

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * API
 ******************************************************************************/
uint32_t BOARD_DebugConsoleSrcFreq(void);

void BOARD_InitDebugConsole(void);

void BOARD_ConfigMPU(void);
#if defined(SDK_I2C_BASED_COMPONENT_USED) && SDK_I2C_BASED_COMPONENT_USED
void BOARD_LPI2C_Init(LPI2C_Type *base, uint32_t clkSrc_Hz);
status_t BOARD_LPI2C_Send(LPI2C_Type *base,
                          uint8_t deviceAddress,
                          uint32_t subAddress,
                          uint8_t subaddressSize,
                          uint8_t *txBuff,
                          uint8_t txBuffSize);
status_t BOARD_LPI2C_Receive(LPI2C_Type *base,
                             uint8_t deviceAddress,
                             uint32_t subAddress,
                             uint8_t subaddressSize,
                             uint8_t *rxBuff,
                             uint8_t rxBuffSize);
status_t BOARD_LPI2C_SendSCCB(LPI2C_Type *base,
                              uint8_t deviceAddress,
                              uint32_t subAddress,
                              uint8_t subaddressSize,
                              uint8_t *txBuff,
                              uint8_t txBuffSize);
status_t BOARD_LPI2C_ReceiveSCCB(LPI2C_Type *base,
                                 uint8_t deviceAddress,
                                 uint32_t subAddress,
                                 uint8_t subaddressSize,
                                 uint8_t *rxBuff,
                                 uint8_t rxBuffSize);
void BOARD_Accel_I2C_Init(void);
status_t BOARD_Accel_I2C_Send(uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, uint32_t txBuff);
status_t BOARD_Accel_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);
void BOARD_Codec_I2C_Init(void);
status_t BOARD_Codec_I2C_Send(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize);
status_t BOARD_Codec_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);
void BOARD_Camera_I2C_Init(void);
status_t BOARD_Camera_I2C_Send(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize);
status_t BOARD_Camera_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);

status_t BOARD_Camera_I2C_SendSCCB(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize);
status_t BOARD_Camera_I2C_ReceiveSCCB(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);
status_t BOARD_Touch_I2C_Send(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize);
status_t BOARD_Touch_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);
#endif /* SDK_I2C_BASED_COMPONENT_USED */

void BOARD_SD_Pin_Config(uint32_t freq);
void BOARD_RelocateVectorTableToRam(void);
/* return the ENET MDIO interface clock frequency */
uint32_t BOARD_GetMDIOClock(void);

/**
 * @brief Init the LPSPI for a Multithread system
 * 
 * @param base base address of the LPSPI.
 * @param config config which needs to be applied
 */
void BOARD_LPSPI_Init(LPSPI_Type *base, const lpspi_master_config_t *config);

/**
 * @brief Reinit LPSPI interface
 *
 */
void BOARD_LPSPI_Reinit(LPSPI_Type *base, const lpspi_master_config_t *config);

/**
 * @brief Used to send or exchange over SPI in a Multithread system
 * 
 * @param base base address of the LPSPI.
 * @param transfer exchange struct
 * @param handle_exclusivity if true, the function will handle also exclusivity access
 * @return status_t 
 */
status_t BOARD_LPSPI_Exchange(LPSPI_Type *base, lpspi_transfer_t *transfer, uint8_t handle_exclusivity);

/**
 * @brief Rezerve the right for a transaction
 * 
 * @param base base address of the LPSPI
 * @param reserve true if want to reserve, false if cancel.
 * @return status_t
 */
status_t BOARD_LPSPI_ReserveExchange(LPSPI_Type *base, uint8_t reserve);

/**
 * @brief Activate the ETH interface
 *
 */
status_t BOARD_EnableETH();

#define BOARD_SDRAM_ALIGN(var, alignbytes) \
    __attribute__((section("Boardsdram,\"aw\",%nobits @"))) var __attribute__((aligned(alignbytes)))

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _BOARD_H_ */
