#include <avr\io.h>
#include <util/delay.h>

int main(void) {

	int isOpen = 0;

	OCR1A=600;
	
	DDRB=0xFF;
	DDRD = 0xFF;
	
	//Button inputs
	DDRC &= ~(1 << PC5 );
	
	PORTD |= (1<<PD3);

	ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));    //Prescaler at 128 so we have an 125Khz clock source
	ADMUX |= (1<<REFS0);
	ADMUX &= ~(1<<REFS1);                //Avcc(+5v) as voltage reference
	ADCSRA |= (1<<ADFR);                
	ADCSRA |= (1<<ADEN);                //Power up the ADC
	ADCSRA |= (1<<ADSC);                //Start converting

	//TOP=ICR1;
	//ICR1=20000 defines 50Hz PWM
	
	ICR1=10000;
	TCCR1A|=(0<<COM1A0)|(1<<COM1A1)|(0<<COM1B0)|(0<<COM1B1)|(0<<FOC1A)|(0<<FOC1B)|(1<<WGM11)|(0<<WGM10);
	TCCR1B|=(0<<ICNC1)|(0<<ICES1)|(1<<WGM13)|(1<<WGM12)|(0<<CS12)|(1<<CS11)|(0<<CS10);
	
	while(1) {
			
		if (ADCW > 520)	{
			OCR1A=600;
			_delay_ms(750);
			isOpen = 0;
		}
		
		else if (((PINC & (1<<PC5)) == 0) && (ADCW < 520))	{
		
			PORTD &= (0<<PD3);
			
			if (isOpen == 0)	{			
				OCR1A=1600;
				_delay_ms(750);
				isOpen = 1;
			}
			
			else if (isOpen == 1)	{
				OCR1A=600;
				_delay_ms(750);
				isOpen = 0;
			}
				
			PORTD |= (1<<PD3);
		}
	}
}