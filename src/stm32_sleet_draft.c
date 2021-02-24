
// C draft of Sleet intro for STM32 (for asm release see stm32_sleet: https://github.com/petersobolev/stm32_sleet)
//
// take notice that there is least possible chip init code, so I don't recommend it for using as template for any of your projects.

#include "stm32l100xc.h"


uint32_t r_seed = 0;

uint32_t random(void) {
  r_seed ^= (r_seed << 4 ); 
  r_seed ^= (r_seed >> 1); 
  return r_seed;
}

int main(void)
{
   
   //PA5 push-pull
   GPIOA->MODER |= GPIO_MODER_MODER5_1;
   GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR5;
   GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR5;

   //enable DAC clock
   RCC->APB1ENR |= RCC_APB1ENR_DACEN;

   //enable DAC1 (PA4 pin)
   DAC->CR |= DAC_CR_EN1;  

   //enable DAC2 (PA5 pin)
   DAC->CR |= DAC_CR_EN2;  

   //load 12bit value, right align
   //DAC->DHR12R2 = 1000;

// 12bit (0-4096) = 0-3v.

// for 8 bit channel1, at once:  DAC_DHR8RD [bits 7:0] , for 8 bit channel 2: DAC_DHR8RD [bits 15:8]
// for 8 bit channel1, separately:  DAC_DHR8R1 , for 8 bit channel 2: DAC_DHR8R2
// for 12 bit channel1, separately:  DAC->DHR12R1 , for 12 bit channel 2: DAC->DHR12R2
// for 12 bit channel1, at once:  DAC_DHR12RD [bits ] , for 12 bit channel 2: DAC_DHR12RD [bits ]

register uint32_t c  = 0;
register uint32_t d = 0;

while(1) {
   for (uint32_t i = 0; i < 4095; i++) {
      
      DAC->DHR12R1 = random() % c + c*(i/2)+d/6;   // DAC channel X
      DAC->DHR12R2 = random() % c - i;    // DAC channel Y

      c++;
      c %= 3; 

      d++;
      d %= 15000; 
  
   }
   r_seed++;
}


}