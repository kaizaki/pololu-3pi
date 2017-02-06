#include<pololu/3pi.h>
#include <avr/pgmspace.h>
unsigned int sensors[5]; 
unsigned int last_proportional=0;
long integral=0;
unsigned char speed;

const char levels[] PROGMEM = 
{
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b01010,
	0b00000,
	0b10001,
	0b01010,
	0b00100,
	0b00000,
	0b00000,
	0b00000,
	0b00000
	
};
///-----------------------------------------------------------------------------------------------
///-----------------------------------------------------------------------------------------------

void load_custom_characters()
{
	lcd_load_custom_character(levels+0,0); 
	lcd_load_custom_character(levels+1,1); 
	lcd_load_custom_character(levels+2,2); 
	lcd_load_custom_character(levels+3,3);
	lcd_load_custom_character(levels+4,4);
	lcd_load_custom_character(levels+5,5);
	lcd_load_custom_character(levels+6,6);
	clear(); 
}
///-----------------------------------------------------------------------------------------------
///-----------------------------------------------------------------------------------------------

void display_readings(const unsigned int *calibrated_values)
{
	unsigned char i;

	for(i=0;i<5;i++) {
		
		const char display_characters[10] = {' ',0,0,1,2,3,4,5,6,255};
		
		char c = display_characters[calibrated_values[i]/101];

		print_character(c);
	}
}
///-----------------------------------------------------------------------------------------------
///-----------------------------------------------------------------------------------------------

void initialize()
{
	unsigned int counter; 
	unsigned int sensors[5];

	
	pololu_3pi_init(2000);
	load_custom_characters();
	while(!button_is_pressed(BUTTON_A))
	{
		int bat = read_battery_millivolts();

		clear();
		print_long(bat);
		print("mV");
		lcd_goto_xy(0,1);
		print("Tekan A");

		delay_ms(100);
	}
	for(counter=0;counter<80;counter++)
	{
		if(counter < 20 || counter >= 60)
			set_motors(40,-40);
		else
			set_motors(-40,40);
	
		calibrate_line_sensors(IR_EMITTERS_ON);

		delay_ms(20);
	}
	set_motors(0,0);


	while(!button_is_pressed(BUTTON_A))
	{

		unsigned int position = read_line(sensors,IR_EMITTERS_ON);
		clear();
		print_long(position);
		lcd_goto_xy(0,1);
		display_readings(sensors);
		delay_ms(100);
	}
	wait_for_button_release(BUTTON_A);

	clear();

	print("LET'S GO");


	play("L8bfareg4");
	while(is_playing());
}
///-------------------------------------------- KALIBRASI -----------------------------------------
///-----------------------------------------------------------------------------------------------

volatile unsigned char cepat_h;
void pid_h()
{
        unsigned int position = read_line(sensors,IR_EMITTERS_ON);
        int proportional = ((int)position) - 2000;

		int derivative = proportional - last_proportional;
		integral += proportional;

		last_proportional = proportional;

		int power_difference = proportional/4 + integral/10000 + derivative*3/2;

		const int speed =  cepat_h;
		if(power_difference > speed)
			power_difference = speed;
		if(power_difference < -speed)
			power_difference = -speed;

		if(power_difference < 0)
			set_motors(speed+power_difference, speed);
		else
			set_motors(speed, speed-power_difference);
}
///============================================= PID HITAM =============================================

volatile unsigned char cepat_p;
void pid_p()
{
        unsigned int position = read_line_white(sensors,IR_EMITTERS_ON);
        int proportional = ((int)position) - 2000;

		int derivative = proportional - last_proportional;
		integral += proportional;

		last_proportional = proportional;

		int power_difference = proportional/4 + integral/10000 + derivative*3/2;

		const int speed =  cepat_p;
		if(power_difference > speed)
			power_difference = speed;
		if(power_difference < -speed)
			power_difference = -speed;

		if(power_difference < 0)
			set_motors(speed+power_difference, speed);
		else
			set_motors(speed, speed-power_difference);
}
///============================================= PID PUTIH =============================================

void kkn()
{	 
     unsigned int sensors[5]; 
	 while (1)
	 {
	 read_line(sensors,IR_EMITTERS_ON);	 
	 pid_h();
	 	if (sensors[2]>500 && sensors[3]>500 && sensors[4]>500 )
		{ 
		play("L16ga");
		set_motors(70,70);
        delay_ms(50);
        set_motors(0,0);
		break;
		}
     }
}
///============================================== KETEMU GARIS KANAN ================================================

void kkn2()
{
	unsigned int sensors[5];
	while (1)
	{
		read_line(sensors,IR_EMITTERS_ON);
		pid_h();
		if (sensors[2]>500 && sensors[4]>500 )
		{
			play("L16ga");
			set_motors(70,70);
			delay_ms(130);
			set_motors(0,0);
			break;
		}
	}
}
///============================================= KETEMU GARIS KANAN MIRING  ================================================

void kknp()
{	 
     unsigned int sensors[5]; 
	 while (1)
	 {
	 read_line_white (sensors,IR_EMITTERS_ON);	 
	 pid_p();
	 	if (sensors[2]<100 && sensors[3]<100 && sensors[4]<100 )
		{ 
		play("L16ga");
		set_motors(70,70);
        delay_ms(50);
        set_motors(0,0);
		break;
		}
     }
}
///=========================================== KETEMU GARIS KANAN UNTUK PUTIH ================================================

void kknp2()
{
	unsigned int sensors[5];
	while (1)
	{
		read_line_white (sensors,IR_EMITTERS_ON);
		pid_p();
		if (sensors[2]<100 && sensors[4]<100 )
		{
			play("L16ga");
			set_motors(70,70);
			delay_ms(130);
			set_motors(0,0);
			break;
		}
	}
}
///============================================= KETEMU GARIS KANAN MIRING UNTUK PUTIH ================================================

void kki()
{	 
     unsigned int sensors[5]; 
	 while (1)
	 {
	 read_line(sensors,IR_EMITTERS_ON);	 
	 pid_h();
	 	if (sensors[2]>500 && sensors[0]>500 && sensors[1]>500  )
		{ 
		 play("L16ag");
         set_motors(70,70);
         delay_ms(50);
         set_motors(0,0);
	     break;
		}
     }
}
///============================================= KETEMU GARIS KIRI =================================================

void kki2()
{
	unsigned int sensors[5];
	while (1)
	{
		read_line(sensors,IR_EMITTERS_ON);
		pid_h();
		if ( sensors[2]>500 && sensors[0]>500 )
		{
			play("L16ag");
			set_motors(70,70);
			delay_ms(145);
			set_motors(0,0);
			break;
		}
	}
}
///============================================= KETEMU GARIS KIRI MIRING  =================================================

void kkip()
{	 
     unsigned int sensors[5]; 
	 while (1)
	 {
	 read_line_white(sensors,IR_EMITTERS_ON);	 
	 pid_p();
	 	if (sensors[2]<100 && sensors[0]<100 && sensors[1]<100  )
		{ 
		 play("L16ag");
         set_motors(70,70);
         delay_ms(50);
         set_motors(0,0);
	     break;
		}
     }
}
///======================================== KETEMU GARIS KIRI UNTUK PUTIH =================================================

void kkip2()
{
	unsigned int sensors[5];
	while (1)
	{
		read_line_white(sensors,IR_EMITTERS_ON);
		pid_p();
		if ( sensors[2]<100 && sensors[0]<100 )
		{
			play("L16ag");
			set_motors(70,70);
			delay_ms(145);
			set_motors(0,0);
			break;
		}
	}
}
///========================================= KETEMU GARIS KIRI MIRING UNTUK PUTIH =================================================

void ks()
{	 
     unsigned int sensors[5]; 
	 while (1)
	 {
	 read_line(sensors,IR_EMITTERS_ON);	 
	 pid_h();
	 if (sensors[2]>500 && sensors[0]>500 && sensors[1]>500 && sensors[3]>500 && sensors[4]>500 )
		{ 
		play("L16ag");
        set_motors(70,70);
        delay_ms(50);
        set_motors(0,0);
		break;
		}
     }
}
///============================================== KETEMU SEMUA GARIS ================================================ 

void ks2()
{
	unsigned int sensors[5];
	while (1)
	{
		read_line(sensors,IR_EMITTERS_ON);
		pid_h();
		if (sensors[2]>500 && sensors[0]>400 && sensors[4]>400 )
		{
			play("L16ag");
			set_motors(70,70);
			delay_ms(300);
			set_motors(0,0);
			break;
		}
	}
}
///============================================= KETEMU SEMUA GARIS MIRING  ==================================================

void ksp()
{	 
     unsigned int sensors[5]; 
	 while (1)
	 {
	 read_line_white(sensors,IR_EMITTERS_ON);	 
	 pid_p();
	 if (sensors[0]<100 && sensors[1]<100 && sensors[2]<100 && sensors[3]<100 && sensors[4]<100 )
		{ 
		play("L16ag");
        set_motors(70,70);
        delay_ms(50);
        set_motors(0,0);
		break;
		}
     }
}
///========================================== KETEMU SEMUA GARIS UNTUK PUTIH ================================================ 

void ksp2()
{
	unsigned int sensors[5];
	while (1)
	{
		read_line(sensors,IR_EMITTERS_ON);
		pid_p();
		if (sensors[2]<100 && sensors[0]<100 && sensors[4]<100 )
		{
			play("L16ag");
			set_motors(70,70);
			delay_ms(300);
			set_motors(0,0);
			break;
		}
	}
}
///============================================= KETEMU SEMUA GARIS UNTUK PUTIH MIRING  ==================================================

void kp()
{ 
  while(1)
	{
	read_line(sensors,IR_EMITTERS_ON);
	pid_h();
	 if (sensors[0]<100 && sensors[1]<100  && sensors[2]<100  && sensors[3]<100  && sensors[4]<100 )
	 {
	 play("L16ga");
	 set_motors(70,70);
     delay_ms(50);
     set_motors(0,0);
	 break;
	 }
    }
}
///========================================= KETEMU GARIS PUTIH =====================================================

void kh()
{ 
  while(1)
	{
	read_line(sensors,IR_EMITTERS_ON);
	pid_p();
	 if (sensors[0]>500 && sensors[1]>500  && sensors[2]>500  && sensors[3]>500  && sensors[4]>500 )
	 {
	 play("L16ga");
	 set_motors(70,70);
     delay_ms(50);
     set_motors(0,0);
	 break;
	 }
    }
}
///========================================= KETEMU GARIS HITAM =====================================================

void ky()
{	 
   unsigned int sensors[5]; 
   while (1)
	 {
	 read_line(sensors,IR_EMITTERS_ON);	 
	 pid_h();
	 	if ( sensors[0]>400 &&  sensors[4]>400 )
		{ 
	 play("L16ag");
     set_motors(70,70);
     delay_ms(50);
     set_motors(0,0);
     break;
		}
     }
}
///========================================= KETEMU PERSIMPANGAN (Y) =====================================================

void kyp()
{	 
   unsigned int sensors[5]; 
   while (1)
	 {
	 read_line(sensors,IR_EMITTERS_ON);	 
	 pid_p();
	 	if ( sensors[0]<100 && sensors[4]<100 )
		{ 
	 play("L16ag");
     set_motors(70,70);
     delay_ms(50);
     set_motors(0,0);
     break;
		}
     }
}
///=================================== KETEMU PERSIMPANGAN (Y) UNTUK PUTIH =====================================================

void putus_putus()
{
	unsigned int sensors[5];
	while(1)
	{
		read_line(sensors,IR_EMITTERS_ON);
		if(sensors[2]<100)
		{
			set_motors(70,70);
			while(1)
			{
				read_line_white(sensors,IR_EMITTERS_ON);
				if(sensors[3]>400 || sensors[2]>400 || sensors[1]>400)
				{
					set_motors(0,0);
					delay_ms(50);
					break;
				}
			}
		}
		break;
	}
}
///============================================= MAJU PUTUS PUTUS  ================================================

void putus_putus_p()
{
	unsigned int sensors[5];
	while(1)
	{
		read_line_white(sensors,IR_EMITTERS_ON);
		if(sensors[2]>500)
		{
			set_motors(70,70);
			while(1)
			{
				read_line(sensors,IR_EMITTERS_ON);
				if(sensors[3]<100 || sensors[2]<100 || sensors[1]<100)
				{
					set_motors(0,0);
					delay_ms(50);
					break;
				}
			}
		}
		break;
	}
}
///==================================== MAJU PUTUS PUTUS BACKGROUND HITAM ================================================

void u()
{ 
  set_motors(100,-100);
  delay_ms(300);
  set_motors(0,0);
}
///========================================= BERPUTAR (U) =====================================================

void kn()
{
unsigned int sensors[5]; 
while(1)
 	{ 
	read_line(sensors,IR_EMITTERS_ON);
	set_motors(70,-70);
	if(sensors[2]<100)
	{
	 while(1)
       {
	     read_line(sensors,IR_EMITTERS_ON);
 	     right_led(1);
	     set_motors(70,-70);
	     if(sensors[2]>500)
	     {
		 right_led(0);
	     break;
	     }
		 }
		 break;
	   }
      
	} 
}
///============================================ BELOK KANAN ==================================================

void knp()
{
unsigned int sensors[5]; 
while(1)
 	{ 
	read_line_white(sensors,IR_EMITTERS_ON);
	set_motors(70,-70);
	if(sensors[2]>500)
	{
	 while(1)
       {
	     read_line_white(sensors,IR_EMITTERS_ON);
 	     right_led(1);
	     set_motors(70,-70);
	     if(sensors[2]<100)
	     {
		 right_led(0);
	     break;
	     }
		 }
		 break;
	   }
      
	} 
}
///======================================= BELOK KANAN UNTUK PUTIH ==================================================

void ki()
{
unsigned int sensors[5]; 
while(1)
 	{ 
	read_line(sensors,IR_EMITTERS_ON);
	set_motors(-70,70);
	if(sensors[2]<100)
	   {
	     while(1)
         {
	      read_line(sensors,IR_EMITTERS_ON);
 	      left_led(1);
	      set_motors(-70,70);
	      if(sensors[2]>500)
	       {
		   left_led(0);
	       break;
	       }
		 }
		 break;
	   }
      
	} 
}
///=============================================== BELOK KIRI ===============================================

void kip()
{
unsigned int sensors[5]; 
while(1)
 	{ 
	read_line_white(sensors,IR_EMITTERS_ON);
	set_motors(-70,70);
	if(sensors[2]>500)
	   {
	     while(1)
         {
	      read_line_white(sensors,IR_EMITTERS_ON);
 	      left_led(1);
	      set_motors(-70,70);
	      if(sensors[2]<100)
	       {
		   left_led(0);
	       break;
	       }
		 }
		 break;
	   }
      
	} 
}
///=============================================== BELOK KIRI UNTUK PUTIH ===============================================

void maju()
{ 
  set_motors(70,70);
  delay_ms(50);
  set_motors(0,0);
}
///=============================================== MAJU ===============================================

void stop()
{
set_motors(0,0);
}
///============================================== STOP ==============================================

void end()
{
set_motors(0,0);
while(!button_is_pressed(BUTTON_A));
}
///============================================= STOP LOOP ================================================

void kn45()
{ 
  set_motors(50,-50);
  delay_ms(170);
  set_motors(0,0);
}
//=================== kanan 45 =====================================================================

void kn90()
{ 
  set_motors(50,-50);
  delay_ms(350);
  set_motors(0,0);
}
//=================== kanan 90 ====================================================================

void kn135()
{ 
  set_motors(50,-50);
  delay_ms(470);
  set_motors(0,0);
}
///=================== kanan 135 ====================================================================

void ki45()
{ 
  set_motors(-50,50);
  delay_ms(250);
  set_motors(0,0);
}
///=================== KIRI 45 =====================================================================

void ki90()
{ 
  set_motors(-50,50);
  delay_ms(300);
  set_motors(0,0);
}
///=================== KIRI 90 ====================================================================

void ki135()
{ 
  set_motors(-50,50);
  delay_ms(400);
  set_motors(0,0);
}
///=================== KIRI 135 ====================================================================

void kn30()
{ 
  set_motors(50,-50);
  delay_ms(90);
  set_motors(0,0);
}
//=================== kanan 90 ====================================================================

void ki30()
{ 
  set_motors(-50,50);
  delay_ms(100);
  set_motors(0,0);
}
//=================== kiri 90 ====================================================================

/*---------------------------------------------------------------------------------------------------------------------------
                                          PROGRAM  MULAI   
                                  JANGAN MERUBAH DATA PROGRAM 
----------------------------------------------------------------------------------------------------------------------------*/

int main()
{
cepat_h = 150 ; // Kecepatan motor pada lintasan hitam
cepat_p = 100 ; // Kecepatan motor pada lintasan putih

initialize();
while(1)
  {
  
  





  }
}
/*----------------------------------------------------------------------------------------------------------------------------
                                         SELESAI
-----------------------------------------------------------------------------------------------------------------------------*/

