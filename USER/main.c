#include "stm32f10x.h"
#include "FreeRTOS.h"                   // ARM.FreeRTOS::RTOS:Core
#include "semphr.h"                     // ARM.FreeRTOS::RTOS:Core
#include "task.h"                       // ARM.FreeRTOS::RTOS:Core
#include "stm32f10x_adc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "adc.h"
#include "lcd_i2c.h"

xSemaphoreHandle xMutex;

float map_adc_value(float value, float from_low, float from_high, float to_low, float to_high) ;

void vTaskVoltage(void *p);
void vTaskAmperage(void *p);

int main(void) {
  SystemInit();
	SystemCoreClockUpdate();
	
	xMutex = xSemaphoreCreateMutex();
	
	lcd_init();
	adc_1_init();
	adc_2_init();
	
	xTaskCreate(vTaskVoltage, (const char*) "Task1 LCD", 128, NULL, 1, NULL);
	xTaskCreate(vTaskAmperage, (const char*) "Task2 LCD", 128, NULL, 1, NULL);
	
	vTaskStartScheduler();
	return 0;
}

void vTaskVoltage(void *p){
	char tmp[50];
	uint16_t ADC = 0;
	float volt = 0;
	while(1){
		xSemaphoreTake(xMutex, portMAX_DELAY);
		lcd_gotoxy(0, 0);
		ADC = (map(adc_1_get_value(), 0, 4095, 0, 170));
		volt = (float) ADC / 10 ;
		sprintf(tmp, "Volt: %.1f    ", volt);
		lcd_send_string(tmp);
		xSemaphoreGive( xMutex );
		vTaskDelay(400);
	}
}

void vTaskAmperage(void *p){
	char tmp[50];
	uint16_t ADC = 0;
	float Volt = 0;
	while(1){
		xSemaphoreTake(xMutex, portMAX_DELAY);    
		lcd_gotoxy(0, 1);
		ADC = adc_2_get_value();
		Volt = ADC * (3.3 / 4095.0);
		sprintf(tmp, "Ampe: %.1f    ", map_adc_value(Volt, 0, 5, -30, 30)+0.1);
		lcd_send_string(tmp);
		xSemaphoreGive( xMutex );		
		vTaskDelay(400);
	}
}

float map_adc_value(float value, float from_low, float from_high, float to_low, float to_high) {
    float from_range = from_high - from_low;
    float to_range = to_high - to_low;
    float scaled_value = (value - from_low) / from_range;
    float mapped_value = to_low + (scaled_value * to_range);

    if(mapped_value < to_low) {
        return to_low;
    } else if(mapped_value > to_high) {
        return to_high;
    } else {
        return mapped_value;
    }
}
