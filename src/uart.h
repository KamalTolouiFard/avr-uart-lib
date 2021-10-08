//
//  uart.h
//  
//  UART library for AVR microcontroller
//  
//  Author          Kamal toloui fard
//  Date            8-oct-2021
//  Version         1.0
//
//  Copyright (C) 2021-2022   Kamal toloui fard

//  This code is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This code is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef UART_H
#define UART_H  0

#include <stddef.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>

#define UART_BAUD_RATE      9600    //UART baud rate in bps
#define UART_DATA_BITS      7       //data bits(5-8)
#define UART_PARITY         0       //parity bit(0-disabled 1-reserved 2-even 3-odd)
#define UART_STOP_BITS      2       //stop bits(1,2)

//UART baud rate register(s) value
//do not modify this macro
#define UART_UBRR   F_CPU/16UL/UART_BAUD_RATE - 1

void uart_init(uint16_t ubrr);
void uart_send(const void*src, uint8_t size);
void uart_recv(void*dest, uint8_t size);

#define uart_open()     uart_init(UART_UBRR)

#endif