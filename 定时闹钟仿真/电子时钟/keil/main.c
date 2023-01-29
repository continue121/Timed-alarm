#include <REGX51.H>
#include<stdlib.h> /*?????*/
#include<stdio.h>	/*goto??????*/
#define uint unsigned int
#define uchar unsigned char
uint num,A_num,x=0;
uint h,m,s;
uint h1=0,m1=0,s1=0,sign=1; 
uchar code Zifu[]="0123456789";
sbit beep = P2^3;/*???*/
sbit LCD_EN = P3^4;/*LCD?E??*/
sbit LCD_RS = P3^5;/*LCD?RS??*/
sbit key_A = P3^6;
sbit key_B = P3^7;
/*??????
*/
void delay(uint i)
{
	uchar j;
  while(i--)
	for(j=0;j<125;j++);
}
/*?????????????RW=0,???RW??*/
/*????
*/
void write_command(uchar command)
	{
	LCD_RS = 0;
	LCD_EN = 0;/*???RS?EN???0????????*/
	P0 = command;/*???command??P0?*/
		LCD_EN = 1;	/*???????,E?????,????,??????E=0*/ 		
	delay(2);/*??,??????*/
	LCD_EN = 0; /*E?????????,LCD??????*/
		/*???*????*/
}
/*???
*/
void write_data(uchar shuju)
	{
	LCD_RS = 1;/*???RS=1?RW=0???????*/
	LCD_EN = 0;/*??????,E?????,????E=0*/
	P0 = shuju;/*????P0???,???LCD*/
	LCD_EN = 1; /*E?????*/
	delay(2);/*???????*/
	LCD_EN = 0; /*E?????????,???????*/
} 
void lcd_post(int X,int Y)
{		
	write_command(0x80+X*(0x40)+Y);
}
 /*??????????,??????:???“??????”;???“alarm clock”
	?????
	???11.0592M,???????12/11.0592*250,?????3686,???1?;?????12M,??????4000
*/
void init() 
	{
	h=m=s=0;
	num=A_num=0;
	LCD_EN=0;
	write_command(0x38); /*??????(16*2??,5*7??,)*/
		write_command(0x0c); /*????0x0c:?????,???,???*/	
		write_command(0x06); /*????0x06:????*/	
		write_command(0x01); /*????0x01:??*/			
	TMOD = 0x02;/*???T0????2,????256??,??????*/
	TH0 = 6;
	TL0 = 6;
	EA = 1;
	ET0 = 1;
	TR0 = 1;
	/*???*/
		/*?*/
	lcd_post(0,0); write_data(Zifu[h/10]);
	lcd_post(0,1); write_data(Zifu[h%10]);
	lcd_post(0,2); write_data(':');
		/*?*/
	lcd_post(0,3); write_data(Zifu[m/10]);
	lcd_post(0,4); write_data(Zifu[m%10]);
	lcd_post(0,5); write_data(':');
	/*?*/
	lcd_post(0,6); write_data(Zifu[s/10]);
	lcd_post(0,7); write_data(Zifu[s%10]);
	/*???*/
	lcd_post(1,5); write_data('a');
	lcd_post(1,6); write_data('l');
	lcd_post(1,7); write_data('a');
	lcd_post(1,8); write_data('r');
	lcd_post(1,9); write_data('m');
	lcd_post(1,10); write_data(' ');
	lcd_post(1,11); write_data('c'); 
	lcd_post(1,12); write_data('l');
	lcd_post(1,13); write_data('o');
	lcd_post(1,14); write_data('c');
	lcd_post(1,15); write_data('k');
}
/*???
*/
void ling()
{
	unsigned char a;

	for(a=0;a<60;a++)	/*???????(?????60?)*/
	{		
	if((P1&0x80)==0)/*????????,????*/
	 break;
	/*“ling ling”??,??????
	*/
	lcd_post(1,5); write_data('l');
	lcd_post(1,6); write_data('i');
	lcd_post(1,7); write_data('n');
	lcd_post(1,8); write_data('g');
	lcd_post(1,9); write_data('!');
	lcd_post(1,10); write_data(' ');
	lcd_post(1,11); write_data('l');
	lcd_post(1,12); write_data('i');
	lcd_post(1,13); write_data('n');
	lcd_post(1,14); write_data('g');
	lcd_post(1,15); write_data('!');
	beep=0;
	delay(500);
	lcd_post(1,5); write_data(' ');
	lcd_post(1,6); write_data(' ');
	lcd_post(1,7); write_data(' ');
	lcd_post(1,8); write_data(' ');
	lcd_post(1,9); write_data(' ');
	lcd_post(1,10); write_data(' ');
	lcd_post(1,11); write_data(' ');
	lcd_post(1,12); write_data(' ');
	lcd_post(1,13); write_data(' ');
	lcd_post(1,14); write_data(' ');
	lcd_post(1,15); write_data(' ');
	beep=1;	
	delay(500);
	}	
	/*????,???????“alarm clock”
	*/
	lcd_post(1,5); write_data('a');
	lcd_post(1,6); write_data('l');
	lcd_post(1,7); write_data('a');
	lcd_post(1,8); write_data('r');
	lcd_post(1,9); write_data('m');
	lcd_post(1,10); write_data(' ');
	lcd_post(1,11); write_data('c');
	lcd_post(1,12); write_data('l');
	lcd_post(1,13); write_data('o');
	lcd_post(1,14); write_data('c');
	lcd_post(1,15); write_data('k');
}
 /*???????????
*/
void keyscan()    
	{
	if(key_A==0)
		{
		delay(3);
		if(key_A==0)
			{
			A_num++;	
			switch(A_num)
				{
				  case 1:							  
					TR0=0;						  
					lcd_post(0,1);
					write_command(0x0f);/*????*/
					break;
				case 2:lcd_post(0,4);break;		 
				case 3:lcd_post(0,7);break;		 				
				case 4:
					A_num=0;
					write_command(0x0c);/*?????,?????*/
					TR0=1;
				  sign = 0;
					break;
			}		
		}
		while(!key_A);
	}
	if(A_num!=0)
		{
		if(key_B==0)
			{
			delay(3);
			if(key_B==0)
				{					
				switch(A_num)
					{
				case 1:
					h1=(++h1)%24;
					lcd_post(0,0); write_data(Zifu[h1/10]);
					lcd_post(0,1); write_data(Zifu[h1%10]);
					break;		  
				case 2:
					m1=(++m1)%60;
					lcd_post(0,3); write_data(Zifu[m1/10]);
					lcd_post(0,4); write_data(Zifu[m1%10]);
					break;		
				case 3:
					s1=(++s1)%60;
					lcd_post(0,6); write_data(Zifu[s1/10]);
					lcd_post(0,7); write_data(Zifu[s1%10]);
					break;		  
				case 4:sign = 0;break;
				}write_command(0x10);		
			}
			while(!key_B);
		}
	}	
} 
/*???
*/
void main()
{ 	
  int clk1 =0;
	int clk2 =0;
  there:
	beep=0;
	init();	
	 h1=m1=s1=0;
	while(sign)
	{
		keyscan();
	}
	clk1 = 0;
  clk2 = h1*3600+m1*60+s1;
	init();
	while(1)
		{		  
		if(num==3686)/*1?*/
			{
			num=0;
			  s++;
   			clk1++;
				if(clk1 == clk2) 
				{
				  ling();
					sign = 1;
					goto there;	/*if?????,????,??there????*/				
				}
			if(s==60)
				{
				s=0;
				m++;				
				if(m==60)
					{
					m=0;
					h++;
					if(h==24)h=0;
					lcd_post(0,0); write_data(Zifu[h/10]);
					lcd_post(0,1); write_data(Zifu[h%10]);
				}
				  lcd_post(0,3); write_data(Zifu[m/10]);
			   	lcd_post(0,4); write_data(Zifu[m%10]);
		  	}
		    	lcd_post(0,6); write_data(Zifu[s/10]);
			    lcd_post(0,7); write_data(Zifu[s%10]);
	    	}
		
	      }
}
/*????
*/
void T0_time() interrupt 1
{
	num++;}
