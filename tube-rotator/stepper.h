/*
 *  spreadspace avr utils
 *
 *
 *  Copyright (C) 2014 Christian Pointner <equinox@spreadspace.org>
 *
 *  This file is part of spreadspace avr utils.
 *
 *  spreadspace avr utils is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *
 *  spreadspace avr utils is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with spreadspace avr utils. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef R3TUBE_stepper_h_INCLUDED
#define R3TUBE_stepper_h_INCLUDED

#define STEPPER_SPEED_MIN 1000
#define STEPPER_SPEED_MAX 150

#define STEPPER_SPEED_20RPM 936
#define STEPPER_SPEED_30RPM 624
#define STEPPER_SPEED_40RPM 468
#define STEPPER_SPEED_50RPM 374
#define STEPPER_SPEED_60RPM 312
#define STEPPER_SPEED_70RPM 267
#define STEPPER_SPEED_80RPM 233
#define STEPPER_SPEED_90RPM 207
#define STEPPER_SPEED_100RPM 187

#define STEPPER_SPEED_DEFAULT STEPPER_SPEED_60RPM

void stepper_init(void);
void stepper_start(void);
void stepper_stop(void);

void stepper_set_speed(uint16_t new_speed);
void stepper_inc_speed(void);
void stepper_dec_speed(void);
uint16_t stepper_get_speed(void);

#endif
