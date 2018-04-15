


#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include<lcd.h>
int sl=150,sr=134;
int arr[30],st=1,br=0,b=0,i=0;
int a=0,f1=1,f2=1,f3=1,f4=1;
int k=5;
unsigned char data; //to store received data from UDR1
//Function to configure INT4 (PORTE 4) pin as input for the left position encoder
void left_encoder_pin_config (void)
{
 DDRE  = DDRE & 0xEF;  //Set the direction of the PORTE 4 pin as input
 PORTE = PORTE | 0x10; //Enable internal pull-up for PORTE 4 pin
}

//Function to configure INT5 (PORTE 5) pin as input for the right position encoder
void right_encoder_pin_config (void)
{
 DDRE  = DDRE & 0xDF;  //Set the direction of the PORTE 4 pin as input
 PORTE = PORTE | 0x20; //Enable internal pull-up for PORTE 4 pin
}

void lcd_port_config (void)
{
 DDRC = DDRC | 0xF7; //all the LCD pin's direction set as output
 PORTC = PORTC & 0x80; // all the LCD pins are set to logic 0 except PORTC 7
}

//ADC pin configuration
void adc_pin_config (void)
{
 DDRF = 0x00; 
 PORTF = 0x00;
 DDRK = 0x00;
 PORTK = 0x00;
}
void buzzer_pin_config (void)
{
 DDRC = DDRC | 0x08;		//Setting PORTC 3 as outpt
 PORTC = PORTC & 0xF7;		//Setting PORTC 3 logic low to turnoff buzzer
}
void servo1_pin_config (void)
{
 DDRB  = DDRB | 0x20;  //making PORTB 5 pin output
 PORTB = PORTB | 0x20; //setting PORTB 5 pin to logic 1
}
void motion_pin_config (void)
{
 DDRA = DDRA | 0x0F;
 PORTA = PORTA & 0xF0;
 DDRL = DDRL | 0x18;   //Setting PL3 and PL4 pins as output for PWM generation
 PORTL = PORTL | 0x18; //PL3 and PL4 pins are for velocity control using PWM.
}
void left_position_encoder_interrupt_init (void) //Interrupt 4 enable
{
 cli(); //Clears the global interrupt
 EICRB = EICRB | 0x02; // INT4 is set to trigger with falling edge
 EIMSK = EIMSK | 0x10; // Enable Interrupt INT4 for left position encoder
 sei();   // Enables the global interrupt 
}

void right_position_encoder_interrupt_init (void) //Interrupt 5 enable
{
 cli(); //Clears the global interrupt
 EICRB = EICRB | 0x08; // INT5 is set to trigger with falling edge
 EIMSK = EIMSK | 0x20; // Enable Interrupt INT5 for right position encoder
 sei();   // Enables the global interrupt 
}

//ISR for right position encoder
ISR(INT5_vect)  
{
 ShaftCountRight++;  //increment right shaft position count
}


//ISR for left position encoder
ISR(INT4_vect)
{
 ShaftCountLeft++;  //increment left shaft position count
}void forward (void) //both wheels forward
{
  motion_set(0x06);
}

void back (void) //both wheels backward
{
  motion_set(0x09);
}

void left (void) //Left wheel backward, Right wheel forward
{
  motion_set(0x05);
}

void right (void) //Left wheel forward, Right wheel backward
{
  motion_set(0x0A);
}

void soft_left (void) //Left wheel stationary, Right wheel forward
{
 motion_set(0x04);
}

void soft_right (void) //Left wheel forward, Right wheel is stationary
{
 motion_set(0x02);
}

void soft_left_2 (void) //Left wheel backward, right wheel stationary
{
 motion_set(0x01);
}

void soft_right_2 (void) //Left wheel stationary, Right wheel backward
{
 motion_set(0x08);
}

void stop (void)
{
  motion_set(0x00);
}


//Function used for turning robot by specified degrees
void angle_rotate(unsigned int Degrees)
{
 float ReqdShaftCount = 0;
 unsigned long int ReqdShaftCountInt = 0;

 ReqdShaftCount = (float) Degrees/ 4.090; // division by resolution to get shaft count
 ReqdShaftCountInt = (unsigned int) ReqdShaftCount;
 ShaftCountRight = 0; 
 ShaftCountLeft = 0; 

 while (1)
 {
  if((ShaftCountRight >= ReqdShaftCountInt) | (ShaftCountLeft >= ReqdShaftCountInt))
  break;
 }
 stop(); //Stop robot
}

//Function used for moving robot forward by specified distance

void linear_distance_mm(unsigned int DistanceInMM)
{
 float ReqdShaftCount = 0;
 unsigned long int ReqdShaftCountInt = 0;

 ReqdShaftCount = DistanceInMM / 5.338; // division by resolution to get shaft count
 ReqdShaftCountInt = (unsigned long int) ReqdShaftCount;
  
 ShaftCountRight = 0;
 while(1)
 {
  if(ShaftCountRight > ReqdShaftCountInt)
  {
  	break;
  }
 } 
 stop(); //Stop robot
}

void forward_mm(unsigned int DistanceInMM)
{
 forward();
 linear_distance_mm(DistanceInMM);
}

void back_mm(unsigned int DistanceInMM)
{
 back();
 linear_distance_mm(DistanceInMM);
}

void left_degrees(unsigned int Degrees) 
{
// 88 pulses for 360 degrees rotation 4.090 degrees per count
 left(); //Turn left
 angle_rotate(Degrees);
}



void right_degrees(unsigned int Degrees)
{
// 88 pulses for 360 degrees rotation 4.090 degrees per count
 right(); //Turn right
 angle_rotate(Degrees);
}
//Function to initialize ports
void port_init()
{
	left_encoder_pin_config(); //left encoder pin config
	right_encoder_pin_config(); //right encoder pin config	
	lcd_port_config();
	adc_pin_config();
	motion_pin_config();
	buzzer_pin_config();
	servo1_pin_config();
	
}
void timer1_init(void)
{
 TCCR1B = 0x00; //stop
 TCNT1H = 0xFC; //Counter high value to which OCR1xH value is to be compared with
 TCNT1L = 0x01;	//Counter low value to which OCR1xH value is to be compared with
 OCR1AH = 0x03;	//Output compare Register high value for servo 1
 OCR1AL = 0xFF;	//Output Compare Register low Value For servo 1
 OCR1BH = 0x03;	//Output compare Register high value for servo 2
 OCR1BL = 0xFF;	//Output Compare Register low Value For servo 2
 OCR1CH = 0x03;	//Output compare Register high value for servo 3
 OCR1CL = 0xFF;	//Output Compare Register low Value For servo 3
 ICR1H  = 0x03;	
 ICR1L  = 0xFF;
 TCCR1A = 0xAB; /*{COM1A1=1, COM1A0=0; COM1B1=1, COM1B0=0; COM1C1=1 COM1C0=0}
 					For Overriding normal port functionality to OCRnA outputs.
				  {WGM11=1, WGM10=1} Along With WGM12 in TCCR1B for Selecting FAST PWM Mode*/
 TCCR1C = 0x00;
 TCCR1B = 0x0C; //WGM12=1; CS12=1, CS11=0, CS10=0 (Prescaler=256)
}
void servo_1(unsigned char degrees)  
{
 float PositionPanServo = 0;
  PositionPanServo = ((float)degrees / 1.86) + 35.0;
 OCR1AH = 0x00;
 OCR1AL = (unsigned char) PositionPanServo;
}
void buzzer_on (void)
{
 unsigned char port_restore = 0;
 port_restore = PINC;
 port_restore = port_restore | 0x08;
 PORTC = port_restore;
}

void buzzer_off (void)
{
 unsigned char port_restore = 0;
 port_restore = PINC;
 port_restore = port_restore & 0xF7;
 PORTC = port_restore;
}

//Function To Initialize UART0
// desired baud rate:9600
// actual baud rate:9600 (error 0.0%)
// char size: 8 bit
// parity: Disabled
void uart0_init(void)
{
 UCSR0B = 0x00; //disable while setting baud rate
 UCSR0A = 0x00;
 UCSR0C = 0x06;
 UBRR0L = 0x5F; //set baud rate lo
 UBRR0H = 0x00; //set baud rate hi
 UCSR0B = 0x98;
}
void timer5_init()
{
	TCCR5B = 0x00;	//Stop
	TCNT5H = 0xFF;	//Counter higher 8-bit value to which OCR5xH value is compared with
	TCNT5L = 0x01;	//Counter lower 8-bit value to which OCR5xH value is compared with
	OCR5AH = 0x00;	//Output compare register high value for Left Motor
	OCR5AL = 0xFF;	//Output compare register low value for Left Motor
	OCR5BH = 0x00;	//Output compare register high value for Right Motor
	OCR5BL = 0xFF;	//Output compare register low value for Right Motor
	OCR5CH = 0x00;	//Output compare register high value for Motor C1
	OCR5CL = 0xFF;	//Output compare register low value for Motor C1
	TCCR5A = 0xA9;	/*{COM5A1=1, COM5A0=0; COM5B1=1, COM5B0=0; COM5C1=1 COM5C0=0}
 					  For Overriding normal port functionality to OCRnA outputs.
				  	  {WGM51=0, WGM50=1} Along With WGM52 in TCCR5B for Selecting FAST PWM 8-bit Mode*/
	
	TCCR5B = 0x0B;	//WGM12=1; CS12=0, CS11=1, CS10=1 (Prescaler=64)
}

void adc_init()
{
	ADCSRA = 0x00;
	ADCSRB = 0x00;		//MUX5 = 0
	ADMUX = 0x20;		//Vref=5V external --- ADLAR=1 --- MUX4:0 = 0000
	ACSR = 0x80;
	ADCSRA = 0x86;		//ADEN=1 --- ADIE=1 --- ADPS2:0 = 1 1 0
}

//Function For ADC Conversion
unsigned char ADC_Conversion(unsigned char Ch) 
{
	unsigned char a;
	if(Ch>7)
	{
		ADCSRB = 0x08;
	}
	Ch = Ch & 0x07;  			
	ADMUX= 0x20| Ch;	   		
	ADCSRA = ADCSRA | 0x40;		//Set start conversion bit
	while((ADCSRA&0x10)==0);	//Wait for conversion to complete
	a=ADCH;
	ADCSRA = ADCSRA|0x10; //clear ADIF (ADC Interrupt Flag) by writing 1 to it
	ADCSRB = 0x00;
	return a;
}

//Function To Print Sesor Values At Desired Row And Coloumn Location on LCD
void print_sensor(char row, char coloumn,unsigned char channel)
{
	
	ADC_Value = ADC_Conversion(channel);
	lcd_print(row, coloumn, ADC_Value, 3);
}

//Function for velocity control
void velocity (unsigned char left_motor, unsigned char right_motor)
{
	OCR5AL = (unsigned char)left_motor;
	OCR5BL = (unsigned char)right_motor;
}

//Function used for setting motor's direction
void motion_set (unsigned char Direction)
{
 unsigned char PortARestore = 0;

 Direction &= 0x0F; 		// removing upper nibbel for the protection
 PortARestore = PORTA; 		// reading the PORTA original status
 PortARestore &= 0xF0; 		// making lower direction nibbel to 0
 PortARestore |= Direction; // adding lower nibbel for forward command and restoring the PORTA status
 PORTA = PortARestore; 		// executing the command
}

void read_sensors(void)
{
		Left_white_line = ADC_Conversion(3)-8;	//Getting data of Left WL Sensor
		Center_white_line = ADC_Conversion(2)-7;	//Getting data of Center WL Sensor
		Right_white_line = ADC_Conversion(1)-8;	//Getting data of Right WL Sensor
		Left_white_line =Left_white_line>2?1:0;
		Center_white_line =Center_white_line>2?1:0;
		Right_white_line =Right_white_line>2?1:0;
}
void linef(void)
{
	while(1)
	{
		
	if(st==1 && br==1)///////directionn array for line following
	{
		st=0;
		if(b==1)
		arr={2,3,7,1,2,3,1,2};
		else if(b==2)
		arr={3,7,1,3,1,2};
		else if(b==3)
		arr={1,7,3,3,1,2};
		else if(b==4)
		arr={2,1,7,3,2,3,1,2};
	}
	if(st==1 && br==2)
	{
		st=0;
		if(b==1)
		arr={2,3,8,1,2,1,3,2};
		else if(b==2)
		arr={3,7,1,1,3,2};
		else if(b==3)
		arr={1,7,3,1,3,2};
		else if(b==4)
		arr={2,1,8,3,2,1,3,2};
	}
	if(st==0 && br==1)
	{
		if(b==1)
		arr={2,3,1,2,3,8,1,2,3,1,2};
		else if(b==2)
		arr={2,3,1,3,7,1,3,1,2};
		else if(b==3)
		arr={2,3,1,1,7,3,3,1,2};
		else if(b==4)
		arr={2,3,1,2,1,8,3,2,3,1,2};
	}
	if(st==0 && br==2)
	{
		if(b==1)
		arr={2,1,3,2,3,8,1,2,1,3,2};
		else if(b==2)
		arr={2,1,3,3,7,1,1,3,2};
		else if(b==3)
		arr={2,1,3,1,7,3,1,3,2};
		else if(b==4)
		arr={2,1,3,2,1,8,3,2,1,3,2};
	}
	if(st==2 && br==2)
	{
		arr={3,5};
	}
	if(st==2 && br==1)
	{
		arr={1,5};
	}
	read_sensors();
	if(Center_white_line==1)
		{
			flag=1;
			forward();
			velocity(sl,sr);
		}

		else if((Left_white_line==1) )
		{
			flag=1;
			forward();
			velocity(sl,sr);
		}

		else if((Right_white_line==1) )
		{
			flag=1;
			forward();
			velocity(sl,sr);
		}

		else if(Center_white_line==0 && Left_white_line==0 && Right_white_line==0)
		{
			forward();
			velocity(50,34);
		}
			else if(Center_white_line==1 && (Left_white_line==1 || Right_white_line==1)
		{
			if (arr[i]==1)
			{
				//left
				forward_mm(55);
				left_degrees(96);
				i++;
				
			}
			else if (arr[i]==3)
			{
				//right
				forward_mm(55);
				right_degrees(96);
				i++;
				
			}
			else if (arr[i]==2)
			{
				//forward
				forward_mm(55);
				
				i++;
				
			}
			else if (arr[i]==7)
			{
				//forward
				forward_mm(64);
				for(int i=180;i>=50;i--)
			{
				servo_1(i);
				_delay_ms(2);
			}
			right_degrees(191);
				i++;
				
			}
			else if (arr[i]==8)
			{
				//forward
				forward_mm(64);
				for(int i=180;i>=50;i--)
			{
				servo_1(i);///////servo driving
				_delay_ms(2);
			}
			left_degrees(191);
				i++;
				
			}
			else if (arr[i]==5)
			{
				//forward
				forward_mm(35);
				stop();
			buzzer_on();
			delay_ms(5000);
			buzzer_off();
			while(1);
			}
			else 
			{
			stop();
			break;
			}
		}
		}

}

SIGNAL(SIG_USART0_RECV) 		// ISR for receive complete interrupt
{
	data = UDR0; 				//making copy of data from UDR0 in 'data' variable 

//	UDR0 = data; 				//echo data back to PC
 	
	if(data=='#')                // if # appears it means it recives the boulder numbers the bridge to be traveled
								//  and boulders to be picked up
	{
		for(int r=0;r<9;r++)
		{
			data = UDR0; 
			dd[r]= data;			
		}
		for(int r=0;r<4;r++)		///extracting boulder numbers
		{
			b0[r]=dd[r];			
		}
		br=dd[4];
		
		for(int r=0;r<4;r++)
		{
			if(dd[k]!='@')
			{
			bd[r]=dd[k];			// boulders  numbers to be picked up
			k++;
			}
		}
		if(data == 0x38) //ASCII value of 8
		{
			velocity(sl,sr);
			forward();  //forward
		}

		if(data == 0x32) //ASCII value of 2
		{
			velocity(sl,sr);
			back(); //back
		}

		if(data == 0x34) //ASCII value of 4
		{
			velocity(sl,sr);
			left();  //left
			_delay_ms(54);
		}

		if(data == 0x36) //ASCII value of 6
		{
				velocity(sl,sr);
				right(); //right
				_delay_ms(54);
		}

		if(data == 0x35) //ASCII value of 5
		{
			velocity(sl,sr);
			stop(); //stop
		}

		if(data == 'y') 
		{
		//printing lcd 
			lcd_cursor (1,1);
			lcd_string("bz1= ")
			lcd_print (1,6,bo[0],1);
			lcd_cursor (1,8);
			lcd_string("bz2= ")
			lcd_print (1,13,bo[1],1);
			lcd_cursor (2,1);
			lcd_string("bz3= ")
			lcd_print (2,6,bo[2],1);
			lcd_cursor (2,8);
			lcd_string("bz1= ")
			lcd_print (2,13,bo[3],1);


		}

		if(data == 'r') 
		{
			for(int i=50;i<=180;i++)
			{
				servo_1(i);
				_delay_ms(5);
			}
		}
		if(data == 'l') 
		{
		/// detection of boulder to be picked
			if(bd[a]=bo[0] && f1==1)
			{
				b=1;
				f1=0;
			}
			if(bd[a]=bo[1] && f2==1)
			{
				b=2;
				f2=0;
			}
			if(bd[a]=bo[2] && f3==1)
			{
				b=3;
				f3=0;
			}
			if(bd[a]=bo[3] && f4==1)
			{
				b=3;
				f4=0;
			}
			a++;
			linef();
			if(a==(k-5)) //detection of base station
			st=2;
			
		}

}


//Function To Initialize all The Devices
void init_devices()
{
 cli(); //Clears the global interrupts
 port_init();  //Initializes all the ports
 timer1_init();
 left_position_encoder_interrupt_init();
 right_position_encoder_interrupt_init();
 adc_init();
 timer5_init()
 uart0_init(); //Initailize UART1 for serial communiaction

 sei();   //Enables the global interrupts
}

//Main Function
int main(void)
{
	init_devices();
	while(1);
}

