/*	Author: Evan Hsu
 *  Partner(s) Name: 
 *	Lab Section: 22
 *	Assignment: Lab 6  Exercise 1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/interrupt.h>
#include "../header/timer.h"

unsigned char i = 0;
unsigned char pattern[4] = { 0x01, 0x02, 0x04, 0x01 };


enum SM_STATES { SM_Start, SM_Wait, SM_Light } SM_State;
void Tick_Sec() {
	switch(SM_State) {
		case SM_Start:
			SM_State = SM_Light;
		break;

		case SM_Wait:
			while (!TimerFlag);
			TimerFlag = 0;
			i++;
			SM_State = SM_Light;
		break;

		case SM_Light:
			SM_State = SM_Wait;
		break;
	}

	switch(SM_State) {
		case SM_Start:
		break;

		case SM_Light:
			PORTB = pattern[i];
			if (i == 3) {
				i = 0;
			}
		break;

		case SM_Wait:
			PORTB = PORTB;
		break;

		default:
		break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */
	TimerSet(1000);
	TimerOn();
	SM_State = SM_Start;
    while (1) {
	Tick_Sec();
    }
    return 1;
}
