/*
* Copyright 2018-2019, 2024-2025 NXP
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "board.h"
#if defined(SDK_I2C_BASED_COMPONENT_USED) && SDK_I2C_BASED_COMPONENT_USED
#include "fsl_lpi2c.h"
#endif /* SDK_I2C_BASED_COMPONENT_USED */
#include "fsl_iomuxc.h"

#include "EVSE_Utils.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
extern uint32_t __Vectors[];
extern uint32_t Image$$ARM_LIB_STACK$$ZI$$Limit;
#define __VECTOR_TABLE __Vectors
#define __StackTop     Image$$ARM_LIB_STACK$$ZI$$Limit
#elif defined(__MCUXPRESSO)
extern uint32_t __Vectors[];
extern void _vStackTop(void);
#define __VECTOR_TABLE __Vectors
#define __StackTop     _vStackTop
#elif defined(__ICCARM__)
extern uint32_t __vector_table[];
extern uint32_t CSTACK$$Limit;
#define __VECTOR_TABLE __vector_table
#define __StackTop     CSTACK$$Limit
#elif defined(__GNUC__)
extern uint32_t __StackTop;
extern uint32_t __Vectors[];
#define __VECTOR_TABLE __Vectors
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/

__attribute__((section(".vectorTableRam"), aligned(0x400))) uint32_t g_vectorTable[256] = {0};

/*******************************************************************************
 * Code
 ******************************************************************************/

/* Get debug console frequency. */
uint32_t BOARD_DebugConsoleSrcFreq(void)
{
    uint32_t freq;

    /* To make it simple, we assume default PLL and divider settings, and the only variable
    from application is use PLL3 source or OSC source */
    if (CLOCK_GetMux(kCLOCK_UartMux) == 0) /* PLL3 div6 80M */
    {
        freq = (CLOCK_GetPllFreq(kCLOCK_PllUsb1) / 6U) / (CLOCK_GetDiv(kCLOCK_UartDiv) + 1U);
    }
    else
    {
        freq = CLOCK_GetOscFreq() / (CLOCK_GetDiv(kCLOCK_UartDiv) + 1U);
    }

    return freq;
}

/* Initialize debug console. */
void BOARD_InitDebugConsole(void)
{
    uint32_t uartClkSrcFreq = BOARD_DebugConsoleSrcFreq();

    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, uartClkSrcFreq);
}

#if defined(SDK_I2C_BASED_COMPONENT_USED) && SDK_I2C_BASED_COMPONENT_USED

void BOARD_LPI2C_Init(LPI2C_Type *base, uint32_t clkSrc_Hz)
{
    uint32_t instance                  = LPI2C_GetInstance(base);
    lpi2c_master_config_t masterConfig = {0};
    if (EVSE_LPI2C_InitMutex(instance) == EVSE_Peripheral_AlreadyInit)
    {
        return;
    }
    EVSE_LPI2C_GetMutex(instance);
    /*
    * masterConfig.debugEnable = false;
    * masterConfig.ignoreAck = false;
    * masterConfig.pinConfig = kLPI2C_2PinOpenDrain;
    * masterConfig.baudRate_Hz = 100000U;
    * masterConfig.busIdleTimeout_ns = 0;
    * masterConfig.pinLowTimeout_ns = 0;
    * masterConfig.sdaGlitchFilterWidth_ns = 0;
    * masterConfig.sclGlitchFilterWidth_ns = 0;
    */
    LPI2C_MasterGetDefaultConfig(&masterConfig);
#if defined(SDK_I2C_FREERTOS) && SDK_I2C_FREERTOS
    LPI2C_RTOS_Init(&s_masterRTOSHandle[instance], base, &masterConfig, clkSrc_Hz);
#else
    LPI2C_MasterInit(base, &masterConfig, clkSrc_Hz);
#endif

    LPI2C_MasterCheckAndClearError(base, LPI2C_MasterGetStatusFlags(base));

    EVSE_LPI2C_PostMutex(instance);
}

status_t BOARD_LPI2C_Send(LPI2C_Type *base,
                        uint8_t deviceAddress,
                        uint32_t subAddress,
                        uint8_t subAddressSize,
                        uint8_t *txBuff,
                        uint8_t txBuffSize)
{
    status_t status;
    uint32_t instance = LPI2C_GetInstance(base);
    lpi2c_master_transfer_t xfer;

    EVSE_LPI2C_GetMutex(instance);
    xfer.flags          = kLPI2C_TransferDefaultFlag;
    xfer.slaveAddress   = deviceAddress;
    xfer.direction      = kLPI2C_Write;
    xfer.subaddress     = subAddress;
    xfer.subaddressSize = subAddressSize;
    xfer.data           = txBuff;
    xfer.dataSize       = txBuffSize;

#if defined(SDK_I2C_FREERTOS) && SDK_I2C_FREERTOS
    status = LPI2C_RTOS_Transfer(&s_masterRTOSHandle[instance], &xfer);
#else
    status = LPI2C_MasterTransferBlocking(base, &xfer);
#endif

    EVSE_LPI2C_PostMutex(instance);
    return status;
}

status_t BOARD_LPI2C_Receive(LPI2C_Type *base,
                            uint8_t deviceAddress,
                            uint32_t subAddress,
                            uint8_t subAddressSize,
                            uint8_t *rxBuff,
                            uint8_t rxBuffSize)
{
    status_t status;
    uint32_t instance = LPI2C_GetInstance(base);
    lpi2c_master_transfer_t xfer;

    EVSE_LPI2C_GetMutex(instance);
    xfer.flags          = kLPI2C_TransferDefaultFlag;
    xfer.slaveAddress   = deviceAddress;
    xfer.direction      = kLPI2C_Read;
    xfer.subaddress     = subAddress;
    xfer.subaddressSize = subAddressSize;
    xfer.data           = rxBuff;
    xfer.dataSize       = rxBuffSize;

#if defined(SDK_I2C_FREERTOS) && SDK_I2C_FREERTOS
    status = LPI2C_RTOS_Transfer(&s_masterRTOSHandle[instance], &xfer);
#else
    status = LPI2C_MasterTransferBlocking(base, &xfer);
#endif

    EVSE_LPI2C_PostMutex(instance);
    return status;
}

status_t BOARD_LPI2C_SendSCCB(LPI2C_Type *base,
                            uint8_t deviceAddress,
                            uint32_t subAddress,
                            uint8_t subAddressSize,
                            uint8_t *txBuff,
                            uint8_t txBuffSize)
{
    lpi2c_master_transfer_t xfer;

    xfer.flags          = kLPI2C_TransferDefaultFlag;
    xfer.slaveAddress   = deviceAddress;
    xfer.direction      = kLPI2C_Write;
    xfer.subaddress     = subAddress;
    xfer.subaddressSize = subAddressSize;
    xfer.data           = txBuff;
    xfer.dataSize       = txBuffSize;

    return LPI2C_MasterTransferBlocking(base, &xfer);
}

status_t BOARD_LPI2C_ReceiveSCCB(LPI2C_Type *base,
                                uint8_t deviceAddress,
                                uint32_t subAddress,
                                uint8_t subAddressSize,
                                uint8_t *rxBuff,
                                uint8_t rxBuffSize)
{
    status_t status;
    lpi2c_master_transfer_t xfer;

    xfer.flags          = kLPI2C_TransferDefaultFlag;
    xfer.slaveAddress   = deviceAddress;
    xfer.direction      = kLPI2C_Write;
    xfer.subaddress     = subAddress;
    xfer.subaddressSize = subAddressSize;
    xfer.data           = NULL;
    xfer.dataSize       = 0;

    status = LPI2C_MasterTransferBlocking(base, &xfer);

    if (kStatus_Success == status)
    {
        xfer.subaddressSize = 0;
        xfer.direction      = kLPI2C_Read;
        xfer.data           = rxBuff;
        xfer.dataSize       = rxBuffSize;

        status = LPI2C_MasterTransferBlocking(base, &xfer);
    }

    return status;
}

void BOARD_Accel_I2C_Init(void)
{
    BOARD_LPI2C_Init(BOARD_ACCEL_I2C_BASEADDR, BOARD_ACCEL_I2C_CLOCK_FREQ);
}

status_t BOARD_Accel_I2C_Send(uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, uint32_t txBuff)
{
    uint8_t data = (uint8_t)txBuff;

    return BOARD_LPI2C_Send(BOARD_ACCEL_I2C_BASEADDR, deviceAddress, subAddress, subaddressSize, &data, 1);
}

status_t BOARD_Accel_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    return BOARD_LPI2C_Receive(BOARD_ACCEL_I2C_BASEADDR, deviceAddress, subAddress, subaddressSize, rxBuff, rxBuffSize);
}

void BOARD_Codec_I2C_Init(void)
{
    BOARD_LPI2C_Init(BOARD_CODEC_I2C_BASEADDR, BOARD_CODEC_I2C_CLOCK_FREQ);
}

status_t BOARD_Codec_I2C_Send(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize)
{
    return BOARD_LPI2C_Send(BOARD_CODEC_I2C_BASEADDR, deviceAddress, subAddress, subAddressSize, (uint8_t *)txBuff,
                            txBuffSize);
}

status_t BOARD_Codec_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    return BOARD_LPI2C_Receive(BOARD_CODEC_I2C_BASEADDR, deviceAddress, subAddress, subAddressSize, rxBuff, rxBuffSize);
}

void BOARD_Camera_I2C_Init(void)
{
    CLOCK_SetMux(kCLOCK_Lpi2cMux, BOARD_CAMERA_I2C_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Lpi2cDiv, BOARD_CAMERA_I2C_CLOCK_SOURCE_DIVIDER);
    BOARD_LPI2C_Init(BOARD_CAMERA_I2C_BASEADDR, BOARD_CAMERA_I2C_CLOCK_FREQ);
}

status_t BOARD_Camera_I2C_Send(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize)
{
    return BOARD_LPI2C_Send(BOARD_CAMERA_I2C_BASEADDR, deviceAddress, subAddress, subAddressSize, (uint8_t *)txBuff,
                            txBuffSize);
}

status_t BOARD_Camera_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    return BOARD_LPI2C_Receive(BOARD_CAMERA_I2C_BASEADDR, deviceAddress, subAddress, subAddressSize, rxBuff,
                            rxBuffSize);
}

status_t BOARD_Camera_I2C_SendSCCB(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize)
{
    return BOARD_LPI2C_SendSCCB(BOARD_CAMERA_I2C_BASEADDR, deviceAddress, subAddress, subAddressSize, (uint8_t *)txBuff,
                                txBuffSize);
}

status_t BOARD_Camera_I2C_ReceiveSCCB(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    return BOARD_LPI2C_ReceiveSCCB(BOARD_CAMERA_I2C_BASEADDR, deviceAddress, subAddress, subAddressSize, rxBuff,
                                rxBuffSize);
}

status_t BOARD_Touch_I2C_Send(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize)
{
    return BOARD_LPI2C_Send(BOARD_TOUCH_I2C_BASEADDR, deviceAddress, subAddress, subAddressSize, (uint8_t *)txBuff,
                            txBuffSize);
}

status_t BOARD_Touch_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    return BOARD_LPI2C_Receive(BOARD_TOUCH_I2C_BASEADDR, deviceAddress, subAddress, subAddressSize, rxBuff, rxBuffSize);
}

#endif /* SDK_I2C_BASED_COMPONENT_USED */

/* MPU configuration. */
void BOARD_ConfigMPU(void)
{
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
    extern uint32_t Image$$RW_m_ncache$$Base[];
    /* RW_m_ncache_unused is a auxiliary region which is used to get the whole size of noncache section */
    extern uint32_t Image$$RW_m_ncache_unused$$Base[];
    extern uint32_t Image$$RW_m_ncache_unused$$ZI$$Limit[];
    uint32_t nonCacheStart = (uint32_t)Image$$RW_m_ncache$$Base;
    uint32_t size          = ((uint32_t)Image$$RW_m_ncache_unused$$Base == nonCacheStart) ?
                        0 :
                        ((uint32_t)Image$$RW_m_ncache_unused$$ZI$$Limit - nonCacheStart);
#elif defined(__MCUXPRESSO)
    extern uint32_t __base_NCACHE_REGION;
    extern uint32_t __top_NCACHE_REGION;
    uint32_t nonCacheStart = (uint32_t)(&__base_NCACHE_REGION);
    uint32_t size          = (uint32_t)(&__top_NCACHE_REGION) - nonCacheStart;
#elif defined(__ICCARM__) || defined(__GNUC__)
    extern uint32_t __NCACHE_REGION_START[];
    extern uint32_t __NCACHE_REGION_SIZE[];
    uint32_t nonCacheStart = (uint32_t)__NCACHE_REGION_START;
    uint32_t size          = (uint32_t)__NCACHE_REGION_SIZE;
#endif
    volatile uint32_t i = 0;

    /* Disable I cache and D cache */
    if (SCB_CCR_IC_Msk == (SCB_CCR_IC_Msk & SCB->CCR))
    {
        SCB_DisableICache();
    }
    if (SCB_CCR_DC_Msk == (SCB_CCR_DC_Msk & SCB->CCR))
    {
        SCB_DisableDCache();
    }

    /* Disable MPU */
    ARM_MPU_Disable();

    /* MPU configure:
    * Use ARM_MPU_RASR(DisableExec, AccessPermission, TypeExtField, IsShareable, IsCacheable, IsBufferable,
    * SubRegionDisable, Size)
    * API in mpu_armv7.h.
    * param DisableExec       Instruction access (XN) disable bit,0=instruction fetches enabled, 1=instruction fetches
    * disabled.
    * param AccessPermission  Data access permissions, allows you to configure read/write access for User and
    * Privileged mode.
    *      Use MACROS defined in mpu_armv7.h:
    * ARM_MPU_AP_NONE/ARM_MPU_AP_PRIV/ARM_MPU_AP_URO/ARM_MPU_AP_FULL/ARM_MPU_AP_PRO/ARM_MPU_AP_RO
    * Combine TypeExtField/IsShareable/IsCacheable/IsBufferable to configure MPU memory access attributes.
    *  TypeExtField  IsShareable  IsCacheable  IsBufferable   Memory Attribute    Shareability        Cache
    *     0             x           0           0             Strongly Ordered    shareable
    *     0             x           0           1              Device             shareable
    *     0             0           1           0              Normal             not shareable   Outer and inner write
    * through no write allocate
    *     0             0           1           1              Normal             not shareable   Outer and inner write
    * back no write allocate
    *     0             1           1           0              Normal             shareable       Outer and inner write
    * through no write allocate
    *     0             1           1           1              Normal             shareable       Outer and inner write
    * back no write allocate
    *     1             0           0           0              Normal             not shareable   outer and inner
    * noncache
    *     1             1           0           0              Normal             shareable       outer and inner
    * noncache
    *     1             0           1           1              Normal             not shareable   outer and inner write
    * back write/read acllocate
    *     1             1           1           1              Normal             shareable       outer and inner write
    * back write/read acllocate
    *     2             x           0           0              Device              not shareable
    *  Above are normal use settings, if your want to see more details or want to config different inner/outter cache
    * policy.
    *  please refer to Table 4-55 /4-56 in arm cortex-M7 generic user guide <dui0646b_cortex_m7_dgug.pdf>
    * param SubRegionDisable  Sub-region disable field. 0=sub-region is enabled, 1=sub-region is disabled.
    * param Size              Region size of the region to be configured. use ARM_MPU_REGION_SIZE_xxx MACRO in
    * mpu_armv7.h.
    */

    /*
    * Add default region to deny access to whole address space to workaround speculative prefetch.
    * Refer to Arm errata 1013783-B for more details.
    *
    */
    /* Region 0 setting: Instruction access disabled, No data access permission. */
    MPU->RBAR = ARM_MPU_RBAR(0, 0x00000000U);
    MPU->RASR = ARM_MPU_RASR(1, ARM_MPU_AP_NONE, 0, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_4GB);

    /* Region 1 setting: Memory with Device type, not shareable, non-cacheable. */
    MPU->RBAR = ARM_MPU_RBAR(1, 0x80000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_512MB);

    /* Region 2 setting: Memory with Device type, not shareable,  non-cacheable. */
    MPU->RBAR = ARM_MPU_RBAR(2, 0x60000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_512MB);

#if defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1)
    /* Region 3 setting: Memory with Normal type, not shareable, outer/inner write back. */
    MPU->RBAR = ARM_MPU_RBAR(3, 0x70000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_RO, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_8MB);
#endif

    /* Region 4 setting: Memory with Device type, not shareable, non-cacheable. */
    MPU->RBAR = ARM_MPU_RBAR(4, 0x00000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_1GB);

    /* Region 5 setting: Memory with Normal type, not shareable, outer/inner write back */
    MPU->RBAR = ARM_MPU_RBAR(5, 0x00000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_128KB);

    /* Region 6 setting: Memory with Normal type, not shareable, outer/inner write back */
    MPU->RBAR = ARM_MPU_RBAR(6, 0x20000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_128KB);

    /* Region 7 setting: Memory with Normal type, not shareable, outer/inner write back */
    MPU->RBAR = ARM_MPU_RBAR(7, 0x20200000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_512KB);

    /* Region 8 setting: Memory with Normal type, not shareable, outer/inner write back */
    MPU->RBAR = ARM_MPU_RBAR(8, 0x20280000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_256KB);

    /* Region 9 setting: Memory with Normal type, not shareable, outer/inner write back */
    MPU->RBAR = ARM_MPU_RBAR(9, 0x80000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 0, 0, ARM_MPU_REGION_SIZE_32MB);

    while ((size >> i) > 0x1U)
    {
        i++;
    }

    if (i != 0)
    {
        /* The MPU region size should be 2^N, 5<=N<=32, region base should be multiples of size. */
        assert(!(nonCacheStart % size));
        assert(size == (uint32_t)(1 << i));
        assert(i >= 5);

        /* Region 10 setting: Memory with Normal type, not shareable, non-cacheable */
        MPU->RBAR = ARM_MPU_RBAR(10, nonCacheStart);
        MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 1, 0, 0, 0, 0, i - 1);
    }

    /* Region 10 setting: Memory with Device type, not shareable, non-cacheable */
    MPU->RBAR = ARM_MPU_RBAR(11, 0x40000000);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_4MB);

    /* Region 12 setting: Memory with Device type, not shareable, non-cacheable */
    MPU->RBAR = ARM_MPU_RBAR(12, 0x42000000);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_1MB);

    /* Enable MPU */
    ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk | MPU_CTRL_HFNMIENA_Msk);

    /* Enable I cache and D cache */
    SCB_EnableDCache();
    SCB_EnableICache();
}

/* return the ENET MDIO interface clock frequency */
uint32_t BOARD_GetMDIOClock(void)
{
    return CLOCK_GetFreq(kCLOCK_IpgClk);
}

void BOARD_RelocateVectorTableToRam(void)
{
    uint32_t n;
    uint32_t irqMaskValue;

    irqMaskValue = DisableGlobalIRQ();

    SCB_DisableDCache();
    SCB_DisableICache();

    /* Copy the vector table from ROM to RAM */
    for (n = 0; n < ((uint32_t)0x400) / sizeof(uint32_t); n++)
    {
        g_vectorTable[n] = __VECTOR_TABLE[n];
    }

    /* Set application defined stack pointer */
    volatile unsigned int vStackTop = (unsigned int)&__StackTop;
    g_vectorTable[0]                = vStackTop;

    /* Point the VTOR to the position of vector table */
    SCB->VTOR = (uint32_t)g_vectorTable;
    __DSB();

    SCB_EnableICache();
    SCB_EnableDCache();

    EnableGlobalIRQ(irqMaskValue);
}

void BOARD_LPSPI_Init(LPSPI_Type *base, const lpspi_master_config_t *config)
{
    uint8_t instance = EVSE_LPSPI_GetInstance(base);

    assert(config != NULL);
    assert(instance != 0);

    evse_peripheral_status_t status = EVSE_LPSPI_InitMutex(instance);

    if (status != EVSE_Peripheral_Success)
    {
        /* Already init */
        return;
    }

    /*Set clock source for LPSPI*/
    CLOCK_SetMux(kCLOCK_LpspiMux, LPSPI_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_LpspiDiv, LPSPI_CLOCK_SOURCE_DIVIDER);

    LPSPI_MasterInit(base, config, LPSPI_CLOCK_FREQ);
}

status_t BOARD_LPSPI_ReserveExchange(LPSPI_Type *base, uint8_t reserve)
{
    status_t status  = kStatus_Fail;
    uint8_t instance = EVSE_LPSPI_GetInstance(base);

    if (reserve == true)
    {
        status = EVSE_LPSPI_GetMutex(instance);
    }
    else
    {
        status = EVSE_LPSPI_PostMutex(instance);
    }

    return status;
}

status_t BOARD_LPSPI_Exchange(LPSPI_Type *base, lpspi_transfer_t *transfer, uint8_t handle_exclusivity)
{
    evse_peripheral_status_t pheripheral_status = EVSE_Peripheral_Success;
    uint8_t error                               = 0;
    uint8_t instance                            = EVSE_LPSPI_GetInstance(base);
    status_t status                             = kStatus_Fail;

    assert(transfer != NULL);
    assert(instance != 0);

    if (handle_exclusivity)
    {
        pheripheral_status = EVSE_LPSPI_GetMutex(instance);
    }

    if (pheripheral_status == EVSE_Peripheral_Success)
    {
        status = LPSPI_MasterTransferBlocking(base, transfer);

        if (handle_exclusivity)
        {
            if (EVSE_LPSPI_PostMutex(instance) != EVSE_Peripheral_Success)
            {
                status = kStatus_Fail;
            }
        }
    }

    return status;
}

status_t BOARD_EnableETH()
{
    gpio_pin_config_t gpio_config        = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};
    const clock_enet_pll_config_t config_clock = {.enableClkOutput = true, .enableClkOutput25M = false, .loopDivider = 1};

    CLOCK_InitEnetPll(&config_clock);
    IOMUXC_EnableMode(IOMUXC_GPR, kIOMUXC_GPR_ENET1TxClkOutputDir, true);

    GPIO_PinInit(BOARD_ENET_INT_GPIO, BOARD_ENET_INIT_PIN, &gpio_config);
    GPIO_PinInit(BOARD_ENET_RESET_GPIO, BOARD_ENET_RESET_PIN, &gpio_config);
    /* pull up the ENET_INT before RESET. */
    GPIO_WritePinOutput(BOARD_ENET_INT_GPIO, BOARD_ENET_INIT_PIN, 1);
    GPIO_WritePinOutput(BOARD_ENET_RESET_GPIO, BOARD_ENET_RESET_PIN, 0);
    SDK_DelayAtLeastUs(10000, CLOCK_GetFreq(kCLOCK_CpuClk));
    GPIO_WritePinOutput(BOARD_ENET_RESET_GPIO, BOARD_ENET_RESET_PIN, 1);
}
