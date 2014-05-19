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
    case '+': stepper_inc_speed(); break;
    case '-': stepper_dec_speed(); break;
    case '1': stepper_set_speed(STEPPER_SPEED_60RPM); break;
    case '2': stepper_set_speed(STEPPER_SPEED_120RPM); break;
    case '3': stepper_set_speed(STEPPER_SPEED_180RPM); break;
    default: printf("Error(cmd): unknown command %02X '%c'\r\n", cmd, cmd); return;
  }
  printf("current speed: %d \r", stepper_get_speed());
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
