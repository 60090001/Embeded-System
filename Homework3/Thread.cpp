#include "mbed.h"

#include "stm32f413h_discovery.h"

#include "stm32f413h_discovery_ts.h"

#include "stm32f413h_discovery_lcd.h"
 

DigitalOut led1(LED1);

DigitalOut led2(LED2);

DigitalOut led3(LED3);

Thread thread;
Thread thread2;
Thread thread3;

void LCD_thread()
{
    BSP_LCD_Init();



    /* Touchscreen initialization */

    if (BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize()) == TS_ERROR) {

        printf("BSP_TS_Init error\n");

    }



    /* Clear the LCD */

    BSP_LCD_Clear(LCD_COLOR_WHITE);

    while (true) 

    {   
        printf("KO CHECK NOI\n");

        BSP_LCD_Clear(LCD_COLOR_YELLOW);
        thread_sleep_for(200);
        BSP_LCD_Clear(LCD_COLOR_MAGENTA);
        thread_sleep_for(200);
        
    }
}
void led3_thread()

{
    while (true)
    {
        led3 = !led3;

        wait(1.5);
    }
}
void led2_thread() 

{

    while (true) 

    {

        led2 = !led2;

        wait(1);

    }

}

 

int main()

{

    thread.start(led2_thread);
    thread2.start(led3_thread);
    thread3.start(LCD_thread);

while (true)
{
    led1 = !led1;

    wait(0.5);
}

}