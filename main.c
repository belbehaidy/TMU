/*
 * M=main.c
 *
 *  Created on: Sep 24, 2022
 *      Author: Bassem El-Behaidy
 */
#include "stdTypes.h"
#include "errorState.h"
#include "TMU_int.h"
#include "DIO/DIO_int.h"

void t1(void*p);
void t2(void*p);
void t3(void*p);

int
main(void)
{

	u8 arr[3]={DIO_u8GROUP_D , DIO_u8PIN2};

	DIO_enuSetPinDirection(DIO_u8GROUP_C , DIO_u8PIN7, DIO_u8OUTPUT);
	DIO_enuSetPinDirection(DIO_u8GROUP_C , DIO_u8PIN2, DIO_u8OUTPUT);
	DIO_enuSetPinDirection(DIO_u8GROUP_D , DIO_u8PIN2, DIO_u8INPUT);
	DIO_enuSetPinValue(DIO_u8GROUP_D , DIO_u8PIN2, DIO_u8FLOAT);
	TMU_vidInit();

	TMU_vidCreateTask(t1 , arr+2 , 3 , 2 , 0);
	TMU_vidCreateTask(t2 , arr ,   2 , 1 , 0 );


	TMU_vidStartScheduler();
}
void t2(void*p)
{
	u8 * ptr = (u8*)p;
	u8 value;

	DIO_enuGetPinValue(ptr[0] , ptr[1] , &value);
	static u8 press = 0, unpress = 0, hold = 0;
	if ( (value==1) && press == 0 && unpress == 0 && hold == 0 )  //First press
	{
		press = 1;
		*(ptr+2) = DIO_u8HIGH;
	}
	else if ( (value==1) && press == 1 && unpress == 0 && hold == 0 ) //continued first press
	{
		hold = 1;
	}
	else if ( (value== 0) && press == 1 && unpress == 0 && hold == 1 ) // removed first press
	{
		unpress = 1;
		hold = 0;
	}
	else if ( (value==1) && press == 1 && unpress == 1 && hold == 0 ) //second press
	{
		press = 0;
		*(ptr+2) = DIO_u8LOW;
	}
	else if ( (value==1) && press == 0 && unpress == 1 && hold == 0) //continued second press
	{
		hold = 1;
	}
	else if ( (value==0) && press == 0 && unpress == 1 && hold == 1) // removed second press
	{
		hold = 0;
		press = 0;
		unpress = 0;
	}
}
void t1(void*p)
{
	u8 * ptr = (u8*)p;
	DIO_enuSetPinValue(DIO_u8GROUP_C , DIO_u8PIN2 , *ptr);
}
void t3(void*p)
{
	DIO_enuTogglePinValue(DIO_u8GROUP_D , DIO_u8PIN3);
}
