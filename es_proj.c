//Embedded Systems Term Project
//Building a digital clock using RTC of LPC2148
//Lavanya Sirisha, Trisha Chander, Vinamratha Pattar

#include<lpc21xx.h> 				

#define rs (1<<24) 				// register select pin
#define rw (1<<25) 				// read write pin
#define en (1<<26) 				// enable pin

void delay(int j ) 				// Time delay function in milli seconds
{
int i;
for(;j;j--)
for(i=6000;i;i--);
}

void data_lcd(char ch) 				// Function to send data to LCD
{
int i =0;
i = ch;
i = i<<16; 
IOPIN1 &=(0XFF00FFFF);
IOPIN1 |= i;

IOSET1 = rs;
IOCLR1 = rw;
IOSET1 = en;
delay(2);
IOCLR1 = en;
}
void cmd_lcd(char ch) 				
{
int i =0;
i = ch;
i = i<<16; 
IOPIN1 &=(0XFF00FFFF);

IOPIN1 |= i;
IOCLR1 = rs;
IOCLR1 = rw;
IOSET1 = en;
delay(2);
IOCLR1 = en;
}
void init_lcd() 				// function to initiliaze LCD
{
cmd_lcd(0x38); 					//set up the 16*2 LCD screen (5x7 matrix)
cmd_lcd(0x01); 					//Clear display screen
cmd_lcd(0x06); 					//Increment cursor (shift cursor to right
cmd_lcd(0x0c); 					//Display ON, cursor OFF
cmd_lcd(0x80); 					//Force cursor to beginning of second row of matrix
}

void str_lcd(char *str) 			 			//function to display full string
{
while(*str)
data_lcd(*str++);
}

void time(void) 				// function to perfom the clock operation
{

cmd_lcd(0x80);
str_lcd("HH:MM:SS");

cmd_lcd(0xc0);
data_lcd(48+(HOUR/10));  //ascii conversion of integer data and passing to data_lcd 
data_lcd(48+(HOUR%10));
data_lcd(':');
data_lcd(48+(MIN/10));
data_lcd(48+(MIN%10));
data_lcd(':');
data_lcd(48+(SEC/10));
data_lcd(48+(SEC%10));
}
void SetTime(void) 				// function to initialize RTC
{
CCR = 0x02;

HOUR = 10;
MIN = 50;
SEC = 0;

CCR = 0x11;
}

int main(void)
{
SetTime();
PINSEL2 = 0X00000000; 				// select PORT1 as GPIO mode
IODIR1 = 0XFFFFFFFF; 				// make PORT1 pin as Output mode 
init_lcd();
while (1) 					// Repeat(loop) forever
{
time();
}
}