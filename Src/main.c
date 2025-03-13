
#include <stdint.h>
#include "stm32xxx.h"
int main(void)
{
	RCC->AHB1ENR |= (1<<0);//A
	RCC->AHB1ENR |= (1<<1);//B

	GPIOA->MODER &=~ (3 << (0 * 2));//FOR BTN 1
	GPIOA->PUPDR |= (2 << (0 * 2));//PULL UP

	GPIOA->MODER &=~ (3 << (1 * 2));//FOR BTN 2
	GPIOA->PUPDR |= (2 << (1 * 2));//PULL UP

	GPIOA->MODER &=~ (3 << (2 * 2));//FOR BTN 3
	GPIOA->PUPDR |= (2 << (2 * 2));//PULL UP

	GPIOA->MODER &=~ (3 << (3 * 2));//FOR BTN 4
	GPIOA->PUPDR |= (2 << (3 * 2));//PULL UP

	GPIOA->MODER &=~ (3 << (4 * 2));//FOR BTN 5 .. RESET
	GPIOA->PUPDR |= (1 << (4 * 2));//PULL UP

	//////////////////////////
	//       OUTPUT's       //

	GPIOB->MODER &=~ (3 << (0 * 2));
	GPIOB->MODER |= (1 << (0 * 2));

	GPIOB->MODER &=~ (3 << (1 * 2));
	GPIOB->MODER |= (1 << (1 * 2));

	GPIOB->MODER &=~ (3 << (2 * 2));
	GPIOB->MODER |= (1 << (2 * 2));

	const int CorrectSeq[4] = {1 , 3 , 2 , 4 };
	int EnterSeq[4] = {0};
	int indexx = 0;

	while(1)
	{
			if(((GPIOA->IDR >> 0) &1))
			{
				EnterSeq[indexx++] = 1;
				while(!((GPIOA->IDR >> 0) &1));
			}
			else if(!((GPIOA->IDR >> 1) &1))
			{
				EnterSeq[indexx++] = 2;
				while(!((GPIOA->IDR >> 1) &1));
			}
			else if(!((GPIOA->IDR >> 2) &1))
			{
				EnterSeq[indexx++] = 3;
				while(!((GPIOA->IDR >> 2) &1));
			}
			else if(!((GPIOA->IDR >> 3) &1))
			{
				EnterSeq[indexx++] = 4;
				while(!((GPIOA->IDR >> 3) &1));
			}

			if(indexx == 4)
			{
				int correct = 1;
				for(volatile int i = 0 ; i < 4 ; i++)
				{
					if(EnterSeq[i] != CorrectSeq[i])
					{
						correct = 0;
						break;
					}
				}
				if(correct)
				{
					GPIOB->ODR |= (1 << 0 );//green on
				}
				else
				{
					GPIOB->ODR |= (1 << 1);//red on
					GPIOB->ODR |= (1 << 2);//buzzer on
				}
				indexx=0;

			}
		if(!(GPIOA->IDR |= (1 << (4))))
		{
			indexx=0;
			GPIOB->ODR &=~ (1 << 0 );
			GPIOB->ODR &=~ (1 << 1);
			GPIOB->ODR &=~ (1 << 2 );
		}
	}
}
