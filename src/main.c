
#define F_CPU   16000000UL

#include "uart.h"

#define uart_recv_byte(d)   uart_recv((void*)&d, 1)
#define uart_send_byte(d)   uart_send((const void*)&d, 1)

int main(void)
{
    char msg[] = "Hello World\n\r\0";
    uint8_t buf[3] = {0x5B, 0x00, 0x5D};
    
    //9.6kbps
    //7-data 2-stop no parity
    uart_open();
    uart_send((const void*)msg, 14);
    
    while(1)
    {
        uart_recv_byte(buf[1]);
        uart_send((const void*)buf, 3);
    }
}