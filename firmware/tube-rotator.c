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

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/power.h>

#include <stdio.h>
#include <string.h>

#include "util.h"
#include "led.h"
#include "anyio.h"

#include "stepper.h"

void handle_cmd(uint8_t cmd)
{
  switch(cmd) {
    case '!': reset2bootloader(); break;
    case 'r': led_on(); stepper_start(); break;
    case 's': stepper_stop(); led_off(); break;
    case '*': stepper_inc_speed(); break;
    case '+': stepper_inc_speed_rpm(); break;
    case '-': stepper_dec_speed_rpm(); break;
    case '_': stepper_dec_speed(); break;
    case '3': stepper_set_speed_rpm(30); break;
    case '4': stepper_set_speed_rpm(40); break;
    case '5': stepper_set_speed_rpm(50); break;
    case '6': stepper_set_speed_rpm(60); break;
    case '7': stepper_set_speed_rpm(70); break;
    case '8': stepper_set_speed_rpm(80); break;
    case '9': stepper_set_speed_rpm(90); break;
    case '0': stepper_set_speed_rpm(100); break;
    case '1': stepper_set_speed_rpm(110); break;
    case '2': stepper_set_speed_rpm(120); break;
  }
  printf("state: %s, %3d rpm\r", stepper_state_to_string(stepper_get_state()), stepper_get_speed_rpm());
}

int main(void)
{
  MCUSR &= ~(1 << WDRF);
  wdt_disable();

  cpu_init();
  jtag_disable();
  led_init();
  anyio_init(115200, 0);

  stepper_init();
  sei();

  for(;;) {
    anyio_task();

    int16_t bytes_received = anyio_bytes_received();
    if(bytes_received > 0)
      handle_cmd(fgetc(stdin));
  }
}
