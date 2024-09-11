/*
 * Copyright 2024 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply
 * with and are bound by, such license terms. If you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#include <stdio.h>
#include "IEC61851.h"
#include "hal_uart_bridge.h"
#include "FreeRTOSConfig.h"
#include "EVSE_Utils.h"
#include "EVSE_ChargingProtocol.h"

static char *j1772_state_prompt[J1772_LAST_STATE] =
{
        [STATE_A]  = "Standby",
        [STATE_A2] = "Standby,PWM ON",
        [STATE_B]  = "Vehicle detected",
        [STATE_B2] = "Vehicle detected",
        [STATE_C]  = "Ready (charging)",
        [STATE_C2] = "Ready (No charging)",
        [STATE_D]  = "Ventilation",
        [STATE_D2] = "Ventilation(No charging)",
        [STATE_E]  = "No power",
        [STATE_F]  = "Error",
};

j1772_status_t EVSE_J1772_GetAmpsFromDutyCycle(uint8_t dutyCycle, uint8_t *amps)
{
    j1772_status_t status = J1772_Succes;

    if (amps == NULL)
    {
        return J1772_Error;
    }

    if (dutyCycle < 3)
    {
        // Charging not allowed
        *amps = 0;
    }
    else if ((dutyCycle >= 3) && (dutyCycle <= 7))
    {
        // Digital Comms
        status = J1772_CP_Req_Digital_mode;
    }
    else if ((dutyCycle > 7) && (dutyCycle < 8))
    {
        *amps = 0;
    }
    else if ((dutyCycle >= 8) && (dutyCycle < 10))
    {
        *amps = 6;
    }
    else if (dutyCycle < 85)
    {
        *amps = (uint8_t)dutyCycle * 0.6;
    }
    else if (dutyCycle < 97)
    {
        *amps = (uint8_t)(dutyCycle - 64) * 2.5;
    }
    else if (dutyCycle == 100)
    {
        *amps = 0;
    }
    else
    {
        status = J1772_Error;
    }
    return status;
}

j1772_status_t EVSE_J1772_GetCPValue(uint32_t *cp_value)
{
    if (cp_value != NULL)
    {
        *cp_value = 0;
        SIGBRD_GetADCVal(cp_value, 1);
        *cp_value = *cp_value * 16;
    }

    return J1772_Succes;
}

j1772_status_t EVSE_J1772_DisablePower()
{
    SIGBRD_OpenRelay();
    return J1772_Succes;
}

j1772_status_t EVSE_J1772_EnablePower()
{
    SIGBRD_CloseRelay();
    return J1772_Succes;
}

j1772_status_t EVSE_J1772_SetCPFromPWM(uint8_t pwm)
{
    SIGBRD_SetPWMDutyInPercent(pwm);
    return J1772_Succes;
}

j1772_status_t EVSE_J1772_SetCPResistor (j1772_state_t eEvState)
{
    j1772_status_t eRet = J1772_Succes;

    switch (eEvState)
    {
    case STATE_B:
    case STATE_B2:
        SIGBRD_SetCPResistorValue(kCPEVResistor_270R, kCPStateOff);
        SIGBRD_SetCPResistorValue(kCPEVResistor_1K3, kCPStateOff);
        break;
    case STATE_C:
    case STATE_C2:
        SIGBRD_SetCPResistorValue(kCPEVResistor_270R, kCPStateOff);
        SIGBRD_SetCPResistorValue(kCPEVResistor_1K3, kCPStateOn);
        break;
    case STATE_D:
    case STATE_D2:
        SIGBRD_SetCPResistorValue(kCPEVResistor_270R, kCPStateOn);
        SIGBRD_SetCPResistorValue(kCPEVResistor_1K3, kCPStateOff);
        break;
    default:
        eRet = J1772_Error;
        break;
    }

    return eRet;
}

j1772_status_t EVSE_J1772_GetDutyCycleFromAmps(uint8_t amps, uint8_t *dutyCycle)
{
    j1772_status_t status = J1772_Succes;

    if (dutyCycle == NULL)
    {
        return J1772_Error;
    }

    if (amps < 6)
    {
        status = J1772_Error;
    }
    else if ((amps >= 6) && (amps < 51))
    {
        *dutyCycle = (uint8_t)amps / 0.6;
    }
    else if ((amps >= 51) && (amps <= 81))
    {
        *dutyCycle = (uint8_t)((amps / 2.5) + 64);
    }
    else
    {
        *dutyCycle = 0;
        status     = J1772_Error;
    }

    return status;
}

j1772_status_t EVSE_J1772_SetCPFromState(j1772_state_t j1772_state, uint16_t amps)
{
    j1772_status_t status = J1772_Succes;
    uint8_t pwm           = 0;

    if (j1772_state == STATE_A)
    {
        pwm = 100;
    }
    else if ((j1772_state == STATE_B) || (j1772_state == STATE_C) || (j1772_state == STATE_D))
    {
        pwm = 100;
    }
    else if ((j1772_state == STATE_A2) || (j1772_state == STATE_B2) || (j1772_state == STATE_C2) || (j1772_state == STATE_D2))
    {
        status = EVSE_J1772_GetDutyCycleFromAmps(amps, &pwm);
    }
    else if (j1772_state == STATE_E)
    {
        pwm = 0;
    }

    if (status == J1772_Succes)
    {
        status = EVSE_J1772_SetCPFromPWM(pwm);
    }

    return status;
}

j1772_state_t EVSE_J1772_GetStateFromCPValue(uint32_t cp_value)
{
    j1772_state_t state = J1772_LAST_STATE;

    if ((cp_value > STATEA_MIN_LEVEL) && (cp_value < STATEA_MAX_LEVEL))
    {
        state = STATE_A;
    }
    else if ((cp_value > STATEB_MIN_LEVEL) && (cp_value < STATEB_MAX_LEVEL))
    {
        state = STATE_B;
    }
    else if ((cp_value > STATEC_MIN_LEVEL) && (cp_value < STATEC_MAX_LEVEL))
    {
        state = STATE_C;
    }
    else if ((cp_value > STATED_MIN_LEVEL) && (cp_value < STATED_MAX_LEVEL))
    {
        state = STATE_D;
    }
    else if ((cp_value > STATEE_MIN_LEVEL) && (cp_value < STATEE_MAX_LEVEL))
    {
        state = STATE_E;
    }
    else
    {
        state = STATE_F;
    }

    return state;
}

j1772_state_t EVSE_J1772_GetState()
{
    uint32_t cp_value;

    EVSE_J1772_GetCPValue(&cp_value);

    return EVSE_J1772_GetStateFromCPValue(0xFFFF & cp_value);
}

const char *EVSE_J1772_GetStateString(j1772_state_t j1772_state)
{
    if (j1772_state < J1772_LAST_STATE)
    {
        return j1772_state_prompt[j1772_state];
    }
    else
    {
        return NULL;
    }
}

#if ENABLE_J1772

typedef struct _j1772_simulation_evse
{
    double fMeasuredVoltage;
    double fMeasuredCurrent;
    double fMeasuredPower;
    double fMeterReading;
    uint32_t uElapseTime;
    uint32_t uCarDetectTime;
}j1772_simulation_evse_t;

typedef struct _j1772_simulation_ev
{
    double fBatteryCapacity;
    double currentCapacity;
    double desiredCapacity;
    uint8_t startingBatteryLevel;
    uint8_t desiredBatteryLevel;
    uint8_t bBatteryFull;
}j1772_simulation_ev_t;

typedef struct _j1772_simulation
{
    uint32_t max_current;
    uint32_t max_voltage;
    uint32_t ulLastCalcTime;
    union {
        j1772_simulation_evse_t evse_data;
        j1772_simulation_ev_t ev_data;
    };
    j1772_state_t current_state;
    uint16_t current_pwm;
    bool isCharging;
    bool ready_to_charge;

}j1772_simulation_t;


j1772_simulation_t j1772_simulation;

static double prvCalculateEnergy (double current, double voltage, double time_interval)
{
    double fEnergy = 0.0f;

    fEnergy = current * voltage;
    fEnergy = fEnergy * time_interval / 1000.0f / 3600.0f;
    return fEnergy;
}

char EVSE_J1772_GetCpStateString()
{
    char cpStateChar = 'A';

    switch(j1772_simulation.current_state)
    {
    case STATE_A:
    case STATE_A2:
        cpStateChar = 'A';
        break;
    case STATE_B:
    case STATE_B2:
        cpStateChar = 'B';
        break;
    case STATE_C:
    case STATE_C2:
        cpStateChar = 'C';
        break;
    case STATE_D:
    case STATE_D2:
        cpStateChar = 'D';
        break;
    case STATE_E:
        cpStateChar = 'E';
        break;
    case STATE_F:
        cpStateChar = 'F';
        break;
    }

    return cpStateChar;
}
void EVSE_J1772_isCharging(bool *bCharging)
{
    if (bCharging != NULL)
    {
        *bCharging = j1772_simulation.isCharging;
    }
}

void  EVSE_J1772_StopCharging(bool bCharging)
{
   j1772_simulation.ready_to_charge = !bCharging;
}

void  EVSE_J1772_GetVehicleData(vehicle_data_t *vehicle_data)
{
    if (vehicle_data == NULL)
    {
        return;
    }

    vehicle_data->chargeCurrent        = (float)j1772_simulation.evse_data.fMeasuredCurrent;
    vehicle_data->deliveredEnergy      = (uint32_t)j1772_simulation.evse_data.fMeasuredPower;

    vehicle_data->elapsedTime = j1772_simulation.evse_data.uElapseTime / 1000;
}

void EVSE_J1772_Init()
{
    j1772_simulation.max_voltage = 230;

    if (j1772_simulation.max_current == 0)
    {
        j1772_simulation.max_current = 16;
    }

    j1772_simulation.ready_to_charge = true;
    EVSE_ChargingProtocol_SetTaskDelay(CHARGINGPROTOCOL_MEDIUMTICK_DELAY);
    j1772_simulation.current_state = STATE_A;
#if EASYEVSE_EV
    j1772_simulation.ev_data.currentCapacity  = BATTERY_CAPACITY * BATTERY_STARTING_LEVEL /100;
    j1772_simulation.ev_data.desiredCapacity  = BATTERY_CAPACITY * BATTERY_DESIRED_LEVEL / 100;
    j1772_simulation.ev_data.fBatteryCapacity = BATTERY_CAPACITY;
    j1772_simulation.ev_data.startingBatteryLevel = BATTERY_STARTING_LEVEL;

    j1772_simulation.ev_data.desiredBatteryLevel = BATTERY_DESIRED_LEVEL;
    j1772_simulation.ev_data.bBatteryFull = false;
#else
    j1772_simulation.current_pwm = PWM100_MAX_VALUE;
    SIGBRD_SetPWMDutyInPerMilli(j1772_simulation.current_pwm);
#endif /* EASYEVSE_EV */
}

void EVSE_J1772_SetMaxCurrent(uint32_t max_current)
{
    if (j1772_simulation.max_current != max_current)
    {
        j1772_simulation.max_current = max_current;
    }
}

#if EASYEVSE_EV

static void EVSE_J1772_SetState(j1772_state_t new_state)
{
    if (new_state == j1772_simulation.current_state)
    {
        return;
    }

    switch (new_state)
    {
    case STATE_A:
    case STATE_A2:
    {
        j1772_simulation.isCharging = false;
        EVSE_J1772_SetCPResistor(STATE_B);
    }
    break;
    case STATE_B:
    case STATE_B2:
    {
        j1772_simulation.isCharging = false;
        EVSE_J1772_SetCPResistor(STATE_B);
    }
    break;
    case STATE_C:
    case STATE_D:
    {
        j1772_simulation.isCharging = false;
        EVSE_J1772_SetCPResistor(STATE_C);
    }
    break;
    case STATE_C2:
    case STATE_D2:
    {
        j1772_simulation.isCharging = true;
        EVSE_J1772_SetCPResistor(STATE_C);
        j1772_simulation.ulLastCalcTime = 0;
    }
    break;
    break;
    }

    j1772_simulation.current_state = new_state;
}

static void EVSE_J1772_Loop_EV()
{

    uint8_t amps = 0;
    SIGBRD_GetPWMDutyInPerMilli(&j1772_simulation.current_pwm);
    EVSE_J1772_GetAmpsFromDutyCycle(j1772_simulation.current_pwm / 10, &amps);
    amps = MIN (amps, j1772_simulation.max_current);

    switch (j1772_simulation.current_state)
    {
    case STATE_A:
    {
        /* Can't detect real STATE_A. We will check 0 PWM for STATE A */
        if (j1772_simulation.current_pwm != 0)
        {
            configPRINTF(("AC Basic Charging state transition from A1 to B1"));
            EVSE_J1772_SetState(STATE_B);
        }
    }
    break;
    case STATE_A2:
    {
        EVSE_J1772_SetState(STATE_A);
    }
    break;
    case STATE_B:
    {
        if (j1772_simulation.current_pwm == 0)
        {
            configPRINTF(("AC Basic Charging state transition from B1 to A1"));
            EVSE_J1772_SetState(STATE_A);
        }
        else if ((j1772_simulation.current_pwm != 0) && (amps != 0))
        {
            configPRINTF(("AC Basic Charging state transition from B1 to B2"));
            EVSE_J1772_SetState(STATE_B2);
        }
    }
    break;
    case STATE_B2:
    {
        if ((j1772_simulation.current_pwm != 0) && (amps != 0) && (j1772_simulation.ready_to_charge == true) && (j1772_simulation.ev_data.bBatteryFull == false))
        {
            configPRINTF(("AC Basic Charging state transition from B2 to C2/D2"));
            EVSE_J1772_SetState(STATE_C2);
        }
        else if (j1772_simulation.current_pwm == PWM100_MAX_VALUE)
        {
            configPRINTF(("AC Basic Charging state transition from B2 to B1"));
            EVSE_J1772_SetState(STATE_B);
        }
        else if ((j1772_simulation.ready_to_charge == false) || (j1772_simulation.ev_data.bBatteryFull == true))
        {
            /* Do nothing */
        }
    }
    break;
    case STATE_C:
    case STATE_D:
    {
        if ((j1772_simulation.ev_data.bBatteryFull == true) || (j1772_simulation.ready_to_charge == false))
        {
            configPRINTF(("AC Basic Charging state transition from C1/D1 to B1"));
            EVSE_J1772_SetState(STATE_B);
        }
        else if ((j1772_simulation.current_pwm != 0) && (amps != 0))
        {
            configPRINTF(("AC Basic Charging state transition from C1/D1 to C2/D2"));
            EVSE_J1772_SetState(j1772_simulation.current_state + 1);
        }
        else if (j1772_simulation.current_pwm == 0)
        {
            configPRINTF(("AC Basic Charging state transition from C1/D1 to A1"));
            EVSE_J1772_SetState(STATE_A);
        }
    }
    break;
    case STATE_C2:
    case STATE_D2:
    {
        if ((j1772_simulation.ev_data.bBatteryFull == true) || (j1772_simulation.ready_to_charge == false))
        {
            configPRINTF(("AC Basic Charging state transition from C2/D2 to B2"));
            EVSE_J1772_SetState(STATE_B2);
        }
        else if (j1772_simulation.ulLastCalcTime == 0)
        {
            j1772_simulation.ulLastCalcTime = EVSE_GetMsSinceBoot();
        }
        else if (j1772_simulation.current_pwm == PWM100_MAX_VALUE)
        {
            configPRINTF(("AC Basic Charging state transition from C2/D2 to C1/D1"));
            EVSE_J1772_SetState(j1772_simulation.current_state - 1);
        }
        else if (j1772_simulation.current_pwm == 0)
        {
            configPRINTF(("AC Basic Charging state transition from C2/D2 to A2"));
            EVSE_J1772_SetState(STATE_A2);
        }
        else if (amps != 0)
        {
            uint32_t time_lapse = EVSE_GetMsSinceBoot() - j1772_simulation.ulLastCalcTime;

            if (time_lapse >= BC_MEASURE_INTERVAL_MS)
            {
                j1772_simulation.ulLastCalcTime = EVSE_GetMsSinceBoot();
                j1772_simulation.ev_data.currentCapacity += prvCalculateEnergy(amps, j1772_simulation.max_voltage, time_lapse);
                if(j1772_simulation.ev_data.currentCapacity >= j1772_simulation.ev_data.desiredCapacity)
                {
                    j1772_simulation.ev_data.bBatteryFull = true;
                    configPRINTF(("AC Basic Charging state transition from C2/D2 to B2"));
                    EVSE_J1772_SetState(STATE_B2);
                }
                configPRINTF(("Current Battery Level: %.1f%", j1772_simulation.ev_data.currentCapacity / (j1772_simulation.ev_data.fBatteryCapacity / 100)));
            }
        }
    }
    break;

    case STATE_E:
    case STATE_F:
    {
    }
    break;
    }

}
#else
static void EVSE_J1772_SetState(j1772_state_t new_state)
{
    if (new_state == j1772_simulation.current_state)
    {
        return;
    }

    switch (new_state)
    {
    case STATE_A:
    case STATE_A2:
    {
        configPRINTF(("Car Disconnected"));
        j1772_simulation.evse_data.fMeasuredVoltage = 0;
        j1772_simulation.evse_data.fMeasuredCurrent = 0;
        j1772_simulation.evse_data.uCarDetectTime = 0;
        j1772_simulation.evse_data.fMeterReading = 0;
        j1772_simulation.evse_data.uElapseTime = 0;
        j1772_simulation.ulLastCalcTime = 0;

        if (j1772_simulation.isCharging)
        {
            EVSE_J1772_DisablePower();
        }
        j1772_simulation.isCharging = false;
        j1772_simulation.ready_to_charge = true;
    }
    break;
    case STATE_B:
    case STATE_B2:
    {
        j1772_simulation.isCharging = false;
        if ((j1772_simulation.current_state == STATE_A2) || (j1772_simulation.current_state == STATE_A))
        {
            j1772_simulation.evse_data.uCarDetectTime = EVSE_GetMsSinceBoot();
        }
        else if((j1772_simulation.current_state >= STATE_C) && (j1772_simulation.current_state <= STATE_D2))
        {
            EVSE_J1772_DisablePower();
        }
    }
    break;
    case STATE_C:
    case STATE_D:
    {
        j1772_simulation.isCharging = false;
    }
    break;
    case STATE_C2:
    case STATE_D2:
        EVSE_J1772_EnablePower();
        j1772_simulation.ulLastCalcTime = 0;
        j1772_simulation.isCharging = true;
        break;
    break;
    }

    j1772_simulation.current_state = new_state;
}

static void EVSE_J1772_Loop_EVSE()
{
    uint16_t new_usPWMDutyCyclePerMil   = j1772_simulation.current_pwm;
    uint8_t new_usPwmDutyCycle          = 0;
    j1772_state_t new_state             = EVSE_J1772_GetState();
    EVSE_J1772_GetDutyCycleFromAmps((uint8_t)j1772_simulation.max_current, &new_usPwmDutyCycle);


    switch (j1772_simulation.current_state)
    {
    case STATE_A:
    {
        if (new_state == STATE_B)
        {
            EVSE_J1772_SetState(STATE_B);
            new_usPWMDutyCyclePerMil        = PWM100_MAX_VALUE;
            configPRINTF(("AC Basic Charging state transition from A1 to B1"));
        }
    }
        break;
    case STATE_A2:
    {
        if (new_state == STATE_A)
        {
            if (j1772_simulation.current_pwm < PWM100_MIN_VALUE)
            {
                EVSE_J1772_SetState(STATE_A);
                new_usPWMDutyCyclePerMil = PWM100_MAX_VALUE;
                configPRINTF(("AC Basic Charging state transition from A2 to A1"));
            }
        }
        else if (new_state == STATE_B)
        {
            EVSE_J1772_SetState(STATE_B);
            configPRINTF(("AC Basic Charging state transition from A2 to B2"));
            /* Nothing to do */
        }
    }
        break;
    case STATE_B:
    {
        if (new_state == STATE_A)
        {
            configPRINTF(("AC Basic Charging state transition from B1 to A1"));
            new_usPWMDutyCyclePerMil = PWM100_MAX_VALUE;
            EVSE_J1772_SetState(STATE_A);
        }
        else if (new_state == STATE_B)
        {
            if ((j1772_simulation.current_pwm == PWM100_MAX_VALUE) && (j1772_simulation.ready_to_charge == true))
            {
                configPRINTF(("AC Basic Charging state transition from B1 to B2"));
                new_usPWMDutyCyclePerMil = new_usPwmDutyCycle * 10;
                EVSE_J1772_SetState(STATE_B2);
            }
        }
    }
        break;
    case STATE_B2:
    {
        if (new_state == STATE_A)
        {
            configPRINTF(("AC Basic Charging state transition from B2 to A2"));
            EVSE_J1772_SetState(STATE_A2);
        }
        else if ((new_state == STATE_B) && (j1772_simulation.ready_to_charge == false))
        {
            configPRINTF(("AC Basic Charging state transition from B2 to B1"));
            new_usPWMDutyCyclePerMil = PWM100_MAX_VALUE;
            EVSE_J1772_SetState(STATE_B);
        }
        else if ((new_state == STATE_C) || (new_state == STATE_D))
        {
            configPRINTF(("AC Basic Charging state transition from B2 to C2/D2"));
            EVSE_J1772_SetState(new_state + 1);

        }
    }
    break;
    case STATE_C:
    case STATE_D:
    {
        if (new_state == STATE_B)
        {
            configPRINTF(("AC Basic Charging state transition from C1/D1 to B1"));
            new_usPWMDutyCyclePerMil = PWM100_MAX_VALUE;
            EVSE_J1772_SetState(STATE_B);
        }
        else if ((new_state == STATE_C) || (new_state == STATE_D))
        {
            if ((j1772_simulation.ready_to_charge == true) && (j1772_simulation.current_pwm == PWM100_MAX_VALUE))
            {
                configPRINTF(("AC Basic Charging state transition from C1/D1 to C2/D2"));
                new_usPWMDutyCyclePerMil = new_usPwmDutyCycle * 10;
                EVSE_J1772_SetState(new_state + 1);
            }
        }
        else if (new_state == STATE_A)
        {
            configPRINTF(("AC Basic Charging state transition from C1/D1 to A1"));
            EVSE_J1772_SetState(STATE_A);
        }
    }
        break;
    case STATE_C2:
    case STATE_D2:
    {
        if (j1772_simulation.ulLastCalcTime == 0)
        {
            j1772_simulation.ulLastCalcTime = EVSE_GetMsSinceBoot();
        }

        new_usPWMDutyCyclePerMil = new_usPwmDutyCycle * 10;
        uint32_t time_lapse = EVSE_GetMsSinceBoot() - j1772_simulation.ulLastCalcTime;

        if ((time_lapse >= BC_MEASURE_INTERVAL_MS) || (new_state == STATE_B) || (new_state == STATE_A))
        {
            j1772_simulation.evse_data.uElapseTime += time_lapse;
            j1772_simulation.ulLastCalcTime = EVSE_GetMsSinceBoot();
            SIGBRD_GetMeterVoltage(&j1772_simulation.evse_data.fMeasuredVoltage);
            SIGBRD_GetMeterCurrent(&j1772_simulation.evse_data.fMeasuredCurrent);
            j1772_simulation.evse_data.fMeasuredCurrent = MIN(j1772_simulation.evse_data.fMeasuredCurrent, j1772_simulation.max_current);
            j1772_simulation.evse_data.fMeasuredVoltage = MIN(j1772_simulation.evse_data.fMeasuredVoltage, j1772_simulation.max_voltage);
            j1772_simulation.evse_data.fMeterReading += prvCalculateEnergy (j1772_simulation.evse_data.fMeasuredCurrent, j1772_simulation.evse_data.fMeasuredVoltage, time_lapse);
        }

        if (new_state == STATE_A)
        {
            configPRINTF(("AC Basic Charging state transition from C2/D2 to A2"));
            EVSE_J1772_SetState(STATE_A2);
        }
        else if (new_state == STATE_B)
        {
            configPRINTF(("AC Basic Charging state transition from C2/D2 to B2"));
            EVSE_J1772_SetState(STATE_B2);
        }
        else if ((new_state == STATE_C) || (new_state == STATE_D))
        {
            if (j1772_simulation.ready_to_charge == false)
            {
                configPRINTF(("AC Basic Charging state transition from C2/D2 to C1/D1"));
                new_usPWMDutyCyclePerMil = PWM100_MAX_VALUE;
                EVSE_J1772_SetState(j1772_simulation.current_state - 1);

            }
        }
    }
    break;
    case STATE_E:
    case STATE_F:
    {
        if(j1772_simulation.isCharging == true)
        {
            EVSE_J1772_DisablePower();
            j1772_simulation.isCharging = false;
        }

    }
    break;
    }

    if (new_usPWMDutyCyclePerMil != j1772_simulation.current_pwm)
    {
        SIGBRD_SetPWMDutyInPerMilli(new_usPWMDutyCyclePerMil);
        j1772_simulation.current_pwm = new_usPWMDutyCyclePerMil;
    }

}

#endif /* EASYEVSE_EV */

void EVSE_J1772_Loop(bool *stopCharging)
{
#if EASYEVSE_EV
    EVSE_J1772_Loop_EV();
#else
    EVSE_J1772_Loop_EVSE();
#endif /* EASYEVSE_EV */

    if (stopCharging != NULL)
    {
        *stopCharging = false;
    }

}

#endif /* ENABLE_J1772 */
