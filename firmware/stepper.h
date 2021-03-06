/*
 *  tube-rotator
 *
 *
 *  Copyright (C) 2014 Christian Pointner <equinox@spreadspace.org>
 *
 *  This file is part of tube-rotator.
 *
 *  tube-rotator is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *
 *  tube-rotator is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with tube-rotator. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef R3TUBE_stepper_h_INCLUDED
#define R3TUBE_stepper_h_INCLUDED

#define STEPPER_SPEED_MIN 150
#define STEPPER_SPEED_MAX 700
#define STEPPER_SPEED_CONVERT_VALUE (uint32_t)(105000)

#define STEPPER_SPEED_30RPM 168
#define STEPPER_SPEED_40RPM 224
#define STEPPER_SPEED_50RPM 281
#define STEPPER_SPEED_60RPM 336
#define STEPPER_SPEED_70RPM 393
#define STEPPER_SPEED_80RPM 451
#define STEPPER_SPEED_90RPM 507
#define STEPPER_SPEED_100RPM 561

#define STEPPER_SPEED_DEFAULT STEPPER_SPEED_60RPM

typedef enum { stepper_stopped = 0, stepper_running = 1 } stepper_state_t;

void stepper_init(void);
void stepper_start(void);
void stepper_stop(void);
stepper_state_t stepper_get_state(void);
const char* stepper_state_to_string(stepper_state_t state);

void stepper_set_speed(uint16_t new_speed);
void stepper_inc_speed(void);
void stepper_dec_speed(void);
uint16_t stepper_get_speed(void);

void stepper_set_speed_rpm(uint8_t new_rpm);
uint8_t stepper_get_speed_rpm(void);
void stepper_inc_speed_rpm(void);
void stepper_dec_speed_rpm(void);

#endif
