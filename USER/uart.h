#ifndef _UART_H_
#define _UART_H_

#include "stm32f10x.h"                  // Device header
#include "stdio.h"

void uart_speed(int baud);
void uart_send_char(char c);
void uart_send_string(const char *string);
void uart_delay_long(int t);
#endif