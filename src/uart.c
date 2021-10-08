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

#include "uart.h"

#if defined(__AVR_ATmega328__) || defined(__AVR_ATmega328p__)
//  in case of a ATmega328(Arduino-uno) or similar micro controllers we need to
//  redefine some macros to appropriate values.
#define UCSRA   UCSR0A
#define UCSRB   UCSR0B
#define UCSRC   UCSR0C
#define UBRRH   UBRR0H
#define UBRRL   UBRR0L
#define UCSZ0   UCSZ00

#define RXEN    RXEN0
#define RXC     RXC0
#define TXEN    TXEN0
#define UDR     UDR0
#define UDRE    UDRE0

#define UPM0    UPM00
#define PE      UPE0
#define USBS    USBS0
#define FE      FE0
#endif

#define CONSTRAINT(AMP,LOW,HIGH)    ((AMP)<(LOW)?(LOW):((AMP)>(HIGH)?(HIGH):(AMP)))
#define UART_DATA_WIDTH             ((CONSTRAINT(UART_DATA_BITS,5,8)-5)<<UCSZ0)
#define UART_PARITY_BIT             ( CONSTRAINT(UART_PARITY,0,3)<<UPM0)
#define UART_STOP_WIDTH             ((CONSTRAINT(UART_STOP_BITS,1,2)-1)<<USBS )

void uart_init(uint16_t ubrr)
{
    if( bit_is_set(UCSRB,RXEN)||bit_is_set(UCSRB,TXEN))
        return;
    
    UBRRH = (uint8_t)(ubrr>>8);
    UBRRL = (uint8_t)(ubrr);
    UCSRA = 0x00;
    #if defined(__AVR_ATmega328__) || defined(__AVR_ATmega328p__)
        UCSRC = UART_PARITY_BIT | UART_STOP_WIDTH | UART_DATA_WIDTH;
        #else
        //  in case of a ATmega8/16/32 microcontroller we need to set URSEL bit of UCSRC register too.
            UCSRC = (1<<URSEL ) | UART_PARITY_BIT | UART_STOP_WIDTH | UART_DATA_WIDTH;
            #endif
    UCSRB = (1<<RXEN) | (1<<TXEN);
}

static uint8_t uart_rtx(uint8_t*ptr)
{
    uint8_t flag = RXC;
    if( ptr!=NULL) flag = UDRE;
    
    loop_until_bit_is_set(UCSRA, flag);
    if( ptr!=NULL)
    {
        UDR = *ptr;
        return 0;
    }
    
    return UDR;
}

void uart_send(const void*src, uint8_t size)
{
    uint8_t i;
    uint8_t*p = (uint8_t*) src;
    
    if( bit_is_clear(UCSRB,TXEN))
        return;
    
    //sending...
    for(i=0; i<size; i++)
        uart_rtx(p++);
}

void uart_recv(void*dest, uint8_t size)
{
    uint8_t i;
    uint8_t*p = (uint8_t*)dest;
    
    if( bit_is_clear(UCSRB,RXEN))
        return;
    
    //receiving...
    for(i=0; i<size; i++)
    {
       *p = uart_rtx(NULL);
        p++;
    }
}