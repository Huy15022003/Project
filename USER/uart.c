#include "uart.h"


struct __FILE {
 int dummy;
};
FILE __stdout;
int fputc(int ch, FILE *f) {
 uart_send_char(ch);
 return ch;
}

void uart_speed(int baud){
	USART_InitTypeDef USART_Structure;
	GPIO_InitTypeDef GPIO_Structure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	// config TX
	GPIO_Structure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Structure.GPIO_Pin  = GPIO_Pin_9;
	GPIO_Structure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Structure);
	// config RX
	GPIO_Structure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Structure.GPIO_Pin  = GPIO_Pin_10;
	GPIO_Structure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Structure);
	
	USART_Structure.USART_BaudRate = baud;
	USART_Structure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Structure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Structure.USART_Parity = USART_Parity_No;
	USART_Structure.USART_StopBits = USART_StopBits_1;
	USART_Structure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1,&USART_Structure);
	
	USART_Cmd(USART1,ENABLE);
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_ClearFlag(USART1,ENABLE);
	NVIC_EnableIRQ(USART1_IRQn);	
}
void uart_send_char(char c){
	USART_SendData(USART1,c);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);	
}
void uart_send_string(const char *string){
	while(*string != NULL)
	{
		uart_send_char(*string++);
	}	
}

void uart_delay_long(int t){
	for(int i = 0; i < t; i++){
		for(int j = 0; j < 0x2aff; j++){
			
		}
	}
}
