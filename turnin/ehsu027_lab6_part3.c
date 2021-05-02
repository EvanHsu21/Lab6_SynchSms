/*	Author: Evan Hsu
 *  Partner(s) Name: 
 *	Lab Section: 22
 *	Assignment: Lab 6  Exercise 3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/interrupt.h>
#include "../header/timer.h"

void oneSec(unsigned char currButton) {
	for (int i = 0; i < 10; ++i) {
		while (!TimerFlag);
               	TimerFlag = 0;
		if ((PINA & 0x03) != currButton) {
			return;
		}
	}
	if ((currButton == 0x02) && (PORTB < 9)) {
			PORTB++;
	}
	else if ((currButton == 0x01) && (PORTB > 0)) {
			PORTB--;
	}
}

enum SM_STATES { SM_SMStart, SM_Choose, SM_Wait, SM_Both, SM_Wait2} SM_STATE;
        void Tick_Toggle() {
                switch(SM_STATE) {
                        case SM_SMStart:
                                SM_STATE = SM_Choose;		
                        break;
                        case SM_Choose:
				if ((PINA & 0x03) == 0x00) {
                                        SM_STATE = SM_Both;
					PORTB = 0;
					while (!TimerFlag);
					TimerFlag = 0;
                                }
                                else if ((PINA & 0x03) == 0x01) {
                                        SM_STATE = SM_Wait2;
					if (PORTB != 0) {
						PORTB--;
						while (!TimerFlag);
						TimerFlag = 0;
					}
                                }
                               	else if ((PINA & 0x03) == 0x02) {
                                        SM_STATE = SM_Wait;
					if (PORTB != 9) {
						PORTB++;
						while (!TimerFlag);
						TimerFlag = 0;
					}
                                }
				else {
					SM_STATE = SM_Choose;
				}
                        break;
			case SM_Wait:
				if ((PINA & 0x03) == 0x00) {
					SM_STATE = SM_Both;
					PORTB = 0;
					while (!TimerFlag);
					TimerFlag = 0;
				}
				else if ((PINA & 0x03) == 0x01) {
					SM_STATE = SM_Wait2;
					if (PORTB != 0) {
						PORTB--;
						while (!TimerFlag);
						TimerFlag = 0;
					}
				}
				else if ((PINA & 0x03) == 0x03) {
					SM_STATE = SM_Choose;
					while (!TimerFlag);
					TimerFlag = 0;
				}
				else if ((PINA & 0x03) == 0x02) {
					oneSec(0x02);
				}
			break;
			case SM_Both:
				SM_STATE = SM_Wait;
			break;
			case SM_Wait2:
				if ((PINA & 0x03) == 0x00) {
					SM_STATE = SM_Both;
					PORTB = 0;
					while(!TimerFlag);
					TimerFlag = 0;
				}
				else if ((PINA & 0x03) == 0x02) {
					SM_STATE = SM_Wait;
					if (PORTB != 9) {
						PORTC++;
						while (!TimerFlag);
						TimerFlag = 0;
					}
				}
				else if ((PINA & 0x03) == 0x03) {
					SM_STATE = SM_Choose;
					while (!TimerFlag);
					TimerFlag = 0;
				}
				else if ((PINA & 0x03) == 0x01) {
					oneSec(0x01);
				}	
			break;
								
                        default:
                                SM_STATE = SM_SMStart;
                        break;
}
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */
	TimerSet(100);
	TimerOn();
	SM_STATE = SM_SMStart;
    while (1) {
	Tick_Toggle();
    }
    return 1;
}
