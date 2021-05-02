/*	Author: Evan Hsu
 *  Partner(s) Name: 
 *	Lab Section: 22
 *	Assignment: Lab 6  Exercise 2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/interrupt.h>
#include "../header/timer.h"

unsigned char press = 0;
unsigned char i = 0;
unsigned char pattern[5] = { 0x01, 0x02, 0x04, 0x02, 0x01 };


enum SM_STATES { SM_Start, SM_Wait, SM_Light, SM_Press, SM_WaitPress, SM_Restart } SM_State;
void Tick_Sec() {
	switch(SM_State) {
		case SM_Start:
			SM_State = SM_Light;
		break;

		case SM_Wait:
			while (!TimerFlag) {
				if ((PINA & 0x01) == 0x00) {
					press = 1;
				}
			}
			TimerFlag = 0;
			if (press == 0) {
				i++;
				SM_State = SM_Light;
			}
			else if (press == 1) {
				SM_State = SM_Press;
			}
		break;

		case SM_Press:
			if ((PINA & 0x01) == 0x01) {
				SM_State = SM_WaitPress;
			}
			else if ((PINA & 0x01) == 0x00) {
				SM_State = SM_Press;
			}
		break;

		case SM_WaitPress:
			if ((PINA & 0x01) == 0x00) {
				SM_State = SM_Restart;
			}
			else if ((PINA & 0x01) == 0x01) {
				SM_State = SM_WaitPress;
			}
		break;

		case SM_Light:
			SM_State = SM_Wait;
		break;

		case SM_Restart:
			if ((PINA & 0x01) == 0x00) {
				SM_State = SM_Restart;
			}
			else if ((PINA & 0x01) == 0x01) {
				press = 0;
				SM_State = SM_Start;
			}
		break;
		default:
			SM_State = SM_Start;
		break;
	}

	switch(SM_State) {
		case SM_Start:
			i = 0;
		break;

		case SM_Light:
			PORTB = pattern[i];
			if (i == 4) {
				i = 0;
			}
		break;

		case SM_Press:
			PORTB = pattern[i];
		break;

		case SM_WaitPress:
			PORTB = pattern[i];
		break;
			

		case SM_Wait:
			PORTB = pattern[i];
		break;

		case SM_Restart:
			PORTB = pattern[0];
		break;

		default:
		break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */
	TimerSet(300);
	TimerOn();
	SM_State = SM_Start;
    while (1) {
	Tick_Sec();
    }
    return 1;
}
