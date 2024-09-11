/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>

// Constants
#define STEFAN_BOLTZMANN 5.670374419E-8f
#define KELVIN	273.15f

// Electrical efficiency
#define EFFICIENCY	.97f

// Convection
#define HEAT_TRANSFER_COEFFICIENT	40.f
#define SURFACE_AREA	100.f
#define AMBIENT_TEMPERATURE	294.f

// Radiation
#define EMISSIVITY	0.95f

#define HEAT_CAPACITY	3E5f	// J K-1

float evse_temperature_C;

static float T = AMBIENT_TEMPERATURE;

float power_convection() {
	return HEAT_TRANSFER_COEFFICIENT * SURFACE_AREA * (T - AMBIENT_TEMPERATURE);
}

float power_radiation() {
	return STEFAN_BOLTZMANN * EMISSIVITY * SURFACE_AREA
			* ((T * T) * (T * T) - (AMBIENT_TEMPERATURE * AMBIENT_TEMPERATURE) * (AMBIENT_TEMPERATURE * AMBIENT_TEMPERATURE));
}

float power_electricity(float I, float V) {
	return I * V * EFFICIENCY;
}

// dt - Time step in seconds
void calc_temperature(float dt) {
	/* TODO */
//	T += (power_electricity(message_irms, message_vrms) - power_convection() - power_radiation()) / HEAT_CAPACITY * dt;
//	evse_temperature_C = T - KELVIN;
}
