#include <msp430f5529.h>  
#include "i2c.h"

void IIC_Init()
{
  P3SEL |= BIT0 + BIT1; //P3.0 P3.1 ¸´ÓÃÄ£Ê½
  UCB0CTL1 |= UCSWRST;
  UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;
  UCB0CTL1 |= UCSSEL_2; 
  UCB0BR0 = 10;
  UCB0BR1 = 0;
  UCB0CTL0 &= ~UCSLA10;
  UCB0I2CSA = 0x68;
  UCB0CTL1 &= ~UCSWRST;
}