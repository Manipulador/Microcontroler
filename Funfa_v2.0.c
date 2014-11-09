#include <18F4550.h>
#device ADC = 8
#fuses HSPLL, NOWDT, NOPROTECT, NOLVP, NODEBUG, USBDIV, PLL5, CPUDIV1, VREGEN, NOMCLR
#use delay (clock=20000000)
#include "usb_cdc.h"
#include "pic18_usb.h"

#define LedPos pin_b4    // Vermelho
#define LedTrab pin_b3   // Verde
#define LedUsb pin_b2    // Branco

#define Junta0D pin_d0
#define Junta0E pin_d1
#define Junta1D pin_d2
#define Junta1E pin_d3
#define Junta2D pin_d4
#define Junta2E pin_d5
#define Junta3D pin_d6
#define Junta3E pin_d7

#define GarraA pin_a4
#define GarraF pin_a5
#define Aberta pin_e0
#define Fechada pin_e1

#define LedGarra pin_b5

void main ()
{
   SETUP_ADC_PORTS(AN0_TO_AN3);
   SETUP_ADC(ADC_CLOCK_INTERNAL);
   delay_us(20);
   
   usb_cdc_init();
   usb_init();
   
   output_float(Aberta);
   output_float(Fechada);
   
   ENABLE_INTERRUPTS(GLOBAL);
   
   unsigned int8 Range=4;                                                        // DIMINUIR !
   
   int1 a=0;
   int1 b=0;
   int1 c=0;
   int1 d=0;
   int1 e=0;
   
   unsigned int8 Junta0_Atual;
   unsigned int8 Junta0_Desejado;
   unsigned int8 Junta1_Atual;
   unsigned int8 Junta1_Desejado;
   unsigned int8 Junta2_Atual;
   unsigned int8 Junta2_Desejado;
   unsigned int8 Junta3_Atual;
   unsigned int8 Junta3_Desejado;
   int1 Garra_desejado= 0;                                                        // 1 - fechado; 0 - aberto
   
   unsigned int8 Aux_0;
   unsigned int8 Aux_1;
   unsigned int8 Aux_2;
   unsigned int8 Aux_3;
   
   SET_TRIS_D( 0xFF );
   output_d(0x00);
   output_low(GarraF);
   output_low(GarraA);
   output_low(LedUsb);
   output_low(LedPos);
   output_high(LedTrab);
   output_low(LedGarra);
   
   set_adc_channel(0);
   delay_us(20);
   Junta0_Desejado = read_adc();
   delay_us(20);
   
   set_adc_channel(1);
   delay_us(20);
   Junta1_Desejado = read_adc();
   delay_us(20);
   
   set_adc_channel(2);
   delay_us(20);
   Junta2_Desejado = read_adc();
   delay_us(20);
   
   set_adc_channel(3);
   delay_us(20);
   Junta3_Desejado = read_adc();
   delay_us(20);
   
   while(1)
   {
      ///////// Motor 0
      
      set_adc_channel(0);
      delay_us(20);
      Junta0_Atual = read_adc();
      delay_us(20);
      
      if (Junta0_Desejado < Junta0_Atual-Range || Junta0_Desejado > Junta0_Atual+Range)
      {
         if(Junta0_Desejado < Junta0_Atual-Range) 
         { 
            output_high(Junta0E); 
            output_low(Junta0D); 
         }
         else 
         { 
            output_high(Junta0D); 
            output_low(Junta0E); 
         }
      }
      else 
      {
         output_low(Junta0D); 
         output_low(Junta0E); 
         a=1; 
      }
      
      ///////// Motor 1
      
      set_adc_channel(1);
      delay_us(20);
      Junta1_Atual = read_adc();
      delay_us(20);
      
      if (Junta1_Desejado < Junta1_Atual-Range || Junta1_Desejado > Junta1_Atual+Range)
      {
         if(Junta1_Desejado < Junta1_Atual-Range) 
         { 
            output_high(Junta1E); 
            output_low(Junta1D); 
         }
         else
         { 
            output_high(Junta1D); 
            output_low(Junta1E); 
         }
      }
      else 
      { 
         output_low(Junta1D); 
         output_low(Junta1E);
         b=1; 
      }
      
      ///////// Motor 2
      
      set_adc_channel(2);
      delay_us(20);
      Junta2_Atual = read_adc();
      delay_us(20);
      
      if (Junta2_Desejado < Junta2_Atual-Range || Junta2_Desejado > Junta2_Atual+Range)
      {
         if(Junta2_Desejado < Junta2_Atual-Range) 
         { 
            output_high(Junta2E); 
            output_low(Junta2D); 
         }
         else 
         { 
            output_high(Junta2D); 
            output_low(Junta2E); 
         }
      }
      else 
      { 
         output_low(Junta2D); 
         output_low(Junta2E); 
         c=1; 
      }
      
      ///////// Motor 3
      
      set_adc_channel(3);
      delay_us(20);
      Junta3_Atual = read_adc();
      delay_us(20);
      
      if (Junta3_Desejado < Junta3_Atual-Range || Junta3_Desejado > Junta3_Atual+Range)
      {
         if(Junta3_Desejado < Junta3_Atual-Range) 
         { 
            output_high(Junta3E);
            output_low(Junta3D); 
         }
         else 
         { 
            output_high(Junta3D); 
            output_low(Junta3E); 
         }
      }
      else 
      { 
         output_low(Junta3D); 
         output_low(Junta3E); 
         d=1; 
      }
      
      ///////// Garra
      
      if (Garra_desejado==1 && input(Fechada)!=0)
      {
         output_high (GarraF);
         output_low (GarraA);
      }
      else if (Garra_desejado==0 && input(Aberta)!=0)
      {
         output_high (GarraA);
         output_low (GarraF);
      }
      else 
      {
         output_low (GarraF);
         output_low (GarraA);
         e=1;
      }
      
      if (a==1 && b==1 && c==1 && d==1 && e==1) output_high(LedPos);
      
      
      
      
      /////////   Matlab input
      
      //Confirma estado do Hardware USB
      usb_task();
      delay_us(20);
         
      if (usb_cdc_kbhit()) 
      {
         output_high(LedUsb);
         
         output_low(LedPos);
         a=0; b=0; c=0; d=0; e=0;
   
         Aux_0 = usb_cdc_getc();
         Aux_1 = usb_cdc_getc();
         Aux_2 = usb_cdc_getc();
         Aux_3 = usb_cdc_getc();
               
         if((char)Aux_0 == 's' && (char)Aux_1 == 't' && (char)Aux_2 == 'o' && (char)Aux_3 == 'p')
         { 
            output_b(0x00);
            Junta0_Desejado = Junta0_Atual;
            Junta1_Desejado = Junta1_Atual;
            Junta2_Desejado = Junta2_Atual;
            Junta3_Desejado = Junta3_Atual;
         }
         else if((char)Aux_0 == 'r' && (char)Aux_1 == 'e' && (char)Aux_2 == 'a' && (char)Aux_3 == 'd')
         { 
            usb_cdc_putc(Junta0_Atual);
            usb_cdc_putc(Junta1_Atual);
            usb_cdc_putc(Junta2_Atual);
            usb_cdc_putc(Junta3_Atual);
         }
         else if((char)Aux_0 == 'g' && (char)Aux_1 == 'r' && (char)Aux_2 == 'a' && (char)Aux_3 == 'b')
         { 
            Garra_desejado =1;
         }
         else if((char)Aux_0 == 'd' && (char)Aux_1 == 'r' && (char)Aux_2 == 'o' && (char)Aux_3 == 'p')
         {
            Garra_desejado =0;
         }
         else if((char)Aux_0 == 'l' && (char)Aux_1 == 'u' && (char)Aux_2 == 'z' && (char)Aux_3 == '!')
         {
            output_toggle(LedGarra);
         }
         else if((char)Aux_0 == 's' && (char)Aux_1 == 't' && (char)Aux_2 == 'n' && (char)Aux_3 == 'd')
         {
            Junta0_Desejado = 127;
            Junta1_Desejado = 127;
            Junta2_Desejado = 110;
            Junta3_Desejado = 127;
         }
         else 
         {
            if (Aux_0 < 50) Junta0_Desejado = 50;
            else if (Aux_0 > 190) Junta0_Desejado = 190;
            else Junta0_Desejado = Aux_0;
            
            if (Aux_1 < 25) Junta1_Desejado = 25;
            else if (Aux_1 > 195) Junta1_Desejado = 195;
            else Junta1_Desejado = Aux_1;
            
            if (Aux_2 < 40) Junta2_Desejado = 40;
            else if (Aux_2 > 250) Junta2_Desejado = 250;
            else Junta2_Desejado = Aux_2;
            
            if (Aux_3 < 70) Junta3_Desejado = 70;
            else if (Aux_3 > 190) Junta3_Desejado = 190;
            else Junta3_Desejado = Aux_3;
         }
         
         output_low(LedUsb);
      }
         
      //Confirma estado do Hardware USB
      usb_task();
      delay_us(20);
   }
}
