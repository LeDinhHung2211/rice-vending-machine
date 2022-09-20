#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <string.h>
#include "LCD_I2C.h"
int LCDAddr = 0x27;
extern int fd;

void main(){
    fd = wiringPiI2CSetup(LCDAddr);
    lcd_init();
	int data =10;
	char text[10];
	sprintf(text, "%d", data);
    
    //write(1, 1, "From SunFounder");
    while(1)
	{
		write(0, 0, text);
		delay(200);
		clearAt(0,1,0);
		delay(200);
	}
	//delay(2000);
    //clear();
}