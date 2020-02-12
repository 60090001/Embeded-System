#include "mbed.h"



#define PressButton 1
#define UnpressButton 0



DigitalOut led1(LED1);

DigitalOut led2(LED2);

DigitalOut led3(LED3);



DigitalIn  User_Button(PA_0,PullNone);



int main()

{

    while (true)

    {

        if(User_Button.read() == PressButton ) // pressed button

        {
            thread_sleep_for(3000);
            if(User_Button.read() == PressButton)
            {
                led3.write(1);
                
            } 
            else
            {
                led3.write(0);
                
            }  

        }
        


        

    }

}


