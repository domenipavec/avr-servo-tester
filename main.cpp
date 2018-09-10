/* File: main.cpp
 * Contains base main function and usually all the other stuff that avr does...
 */
/* Copyright (c) 2012-2013 Domen Ipavec (domen.ipavec@z-v.si)
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <util/delay.h>

#include <avr/io.h>
#include <avr/interrupt.h>
//#include <avr/pgmspace.h>
//#include <avr/eeprom.h>

#include <stdint.h>

#include "bitop.h"

static volatile uint8_t v = 0;

static const uint8_t min = 50;
static const uint8_t max = 240;

ISR(ADC_vect) {
	v = ADCH;
	if (v < min) {
		v = min;
	}
	if (v > max) {
		v = max;
	}
}

int main() {
	// init
	SETBIT(DDRB, PB1);

	// positive for trimmer
	SETBIT(DDRA, PA0);
	SETBIT(PORTA, PA0);

	// adc1
	ADMUXA = 1;
	ADCSRA = BIT(ADEN) | BIT(ADSC) | BIT(ADATE) | BIT(ADIE) | BIT(ADPS2) | BIT(ADPS1) | BIT(ADPS0);
	ADCSRB = BIT(ADLAR);

	// enable interrupts
	sei();

	for (;;) {
		for (uint16_t i = 0; i < 2000; i++) {
			if (i < v) {
				SETBIT(PORTB, PB1);
			} else {
				CLEARBIT(PORTB, PB1);
			}
			_delay_us(8);
		}
	}
}
