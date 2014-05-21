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

#include <avr/sfr_defs.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <math.h>

#include "stepper.h"

uint8_t step_table [] =
{
  2,  // 0010
  6,  // 0110
  4,  // 0100
  5,  // 0101
  1,  // 0001
  9,  // 1001
  8,  // 1000
  10, // 1010
};

#define STEPPER_PORT PORTF
#define STEPPER_DDR DDRF
#define STEPPER_FIRST_BIT 4
#define STEPPER_ENABLE_A_BIT 0
#define STEPPER_ENABLE_B_BIT 1
#define LENGTH_STEP_TABLE (sizeof(step_table)/sizeof(uint8_t))
#define STEPPER_OUTPUT_BITMASK (~(0xF << STEPPER_FIRST_BIT ))

volatile uint16_t target_speed;
uint16_t current_speed;
stepper_state_t current_state;

void stepper_init(void)
{
  target_speed = STEPPER_SPEED_DEFAULT;
  STEPPER_PORT &= ~(0xF << STEPPER_FIRST_BIT | 1<<STEPPER_ENABLE_A_BIT | 1<<STEPPER_ENABLE_B_BIT);
  STEPPER_DDR |= (0xF << STEPPER_FIRST_BIT) | (1<<STEPPER_ENABLE_A_BIT) | (1<<STEPPER_ENABLE_B_BIT);
  stepper_stop();
}

void stepper_start(void)
{
  if(current_state == stepper_running) return;

  current_speed = STEPPER_SPEED_MIN;
  STEPPER_PORT |= (1<<STEPPER_ENABLE_A_BIT) | (1<<STEPPER_ENABLE_B_BIT);
  TCNT1 = 0;
  OCR1A = current_speed;
  TCCR1A = 0;                              // prescaler 1:64, WGM = 4 (CTC)
  TCCR1B = 1<<WGM12 | 1<<CS11 | 1<<CS10;   //
  TIMSK1 = 1<<OCIE1A;
  current_state = stepper_running;
}

void stepper_stop(void)
{
  STEPPER_PORT &= ~(0xF << STEPPER_FIRST_BIT | 1<<STEPPER_ENABLE_A_BIT | 1<<STEPPER_ENABLE_B_BIT);
  TCCR1B = 0; // no clock source
  TIMSK1 = 0; // disable timer interrupt
  current_state = stepper_stopped;
}

static inline void stepper_handle(void)
{
  static uint8_t step_idx = 0;

  uint8_t stepper_output = step_table[step_idx];
  stepper_output <<= STEPPER_FIRST_BIT;
  STEPPER_PORT = (STEPPER_PORT & STEPPER_OUTPUT_BITMASK ) | stepper_output;
  step_idx++;
  step_idx %= LENGTH_STEP_TABLE;

  if(step_idx != 0 && step_idx % 8) {
    if(current_speed > target_speed)
      OCR1A = --current_speed;
    else if(current_speed < target_speed)
      OCR1A = ++current_speed;
  }
}

ISR(TIMER1_COMPA_vect)
{
  stepper_handle();
}

stepper_state_t stepper_get_state(void)
{
  return current_state;
}

const char* stepper_state_to_string(stepper_state_t state)
{
  switch(state) {
    case stepper_running: return "running";
    case stepper_stopped: return "stopped";
  }
  return "?";
}

void stepper_set_speed(uint16_t new_speed)
{
  if(new_speed <= STEPPER_SPEED_MIN && new_speed >= STEPPER_SPEED_MAX) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
      target_speed = new_speed;
    }
  }
}

void stepper_inc_speed(void)
{
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    target_speed = (target_speed <= STEPPER_SPEED_MAX) ? STEPPER_SPEED_MAX : target_speed - 1;
  }
}

void stepper_dec_speed(void)
{
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    target_speed = (target_speed >= STEPPER_SPEED_MIN) ? STEPPER_SPEED_MIN : target_speed + 1;
  }
}

uint16_t stepper_get_speed(void)
{
  return target_speed;
}

void stepper_set_speed_rpm(uint8_t new_rpm)
{
  stepper_set_speed(( (uint16_t)lround( (double)(60.0 * F_CPU) / (double)(64.0 * 800.0 * (double)new_rpm) ) ) - 1);
}

uint8_t stepper_get_speed_rpm(void)
{
  return (uint8_t)lround( ( (double)F_CPU / (double)(64.0 * 800.0 * (double)(target_speed + 1) ) ) * 60.0 );
}

void stepper_inc_speed_rpm(void)
{
  uint8_t rpm = stepper_get_speed_rpm();
  stepper_set_speed_rpm((rpm >= 255) ? 255 : rpm+1);
}

void stepper_dec_speed_rpm(void)
{
  uint8_t rpm = stepper_get_speed_rpm();
  stepper_set_speed_rpm((rpm <= 1) ? 1 : rpm-1);
}
