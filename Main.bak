#include <18F4550.h>
#device ADC = 8
#fuses HSPLL, NOWDT, NOPROTECT, NOLVP, NODEBUG, USBDIV, PLL5, CPUDIV1, VREGEN, NOMCLR
#use delay (clock=20000000)
#include "usb_cdc.h"
#include "pic18_usb.h"

//Leds de Informação
#define LedPos pin_b4               // Vermelho
#define LedTrab pin_b3              // Verde
#define LedUsb pin_b2               // Branco
/*Pinos de acionamento de motores
das juntas*/
#define Junta0D pin_d0
#define Junta0E pin_d1
#define Junta1D pin_d2
#define Junta1E pin_d3
#define Junta2D pin_d4
#define Junta2E pin_d5
#define Junta3D pin_d6
#define Junta3E pin_d7
/*Pinos de acionamento de motores
da garra*/
#define GarraA pin_a4
#define GarraF pin_a5
//Pinos sensores da Garra
#define Aberta pin_e0
#define Fechada pin_e1
#define LedGarra pin_b5

void main ()
{
   /*Setando portas de leitura 
   analógica*/
   SETUP_ADC_PORTS(AN0_TO_AN3);
   SETUP_ADC(ADC_CLOCK_INTERNAL);
   delay_us(20);
   //Inicia hardware USB
   usb_cdc_init();
   usb_init();
   //Setando pinos como entradas digitais
   output_float(Aberta);
   output_float(Fechada);
   /*Ativa pino de interrupção 
   externa (A implementar)*/
   ENABLE_INTERRUPTS(GLOBAL);
   /*Variável de faixa de tolerância
   para posicionamento das juntas 
   (por unidade de Bits do ADC)*/
   unsigned int8 Range=4;
   //Variáveis auxiliares para sinalização de parada 
   int1 a=0;
   int1 b=0;
   int1 c=0;
   int1 d=0;
   int1 e=0;
   //Variável auxiliar para comunicação de parada
   // inicia em 1 para não enviar o sinal na primeira iteração
   int1 z=1;
   /*Variáveis para armazenamento dos valores 
   dos sensores das juntas*/
   unsigned int8 Junta0_Atual;
   unsigned int8 Junta0_Desejado;
   unsigned int8 Junta1_Atual;
   unsigned int8 Junta1_Desejado;
   unsigned int8 Junta2_Atual;
   unsigned int8 Junta2_Desejado;
   unsigned int8 Junta3_Atual;
   unsigned int8 Junta3_Desejado;
   int1 Garra_desejado=0;              // 1=Fechada, 0=Aberta
   /* Variáveis auxiliares para 
   verificação de comandos em uma 
   nova entrada de dados desejados via USB*/
   unsigned int8 Aux_0;
   unsigned int8 Aux_1;
   unsigned int8 Aux_2;
   unsigned int8 Aux_3;
   //Setando Porto de motores como desligado
   SET_TRIS_D( 0xFF );
   output_d(0x00);
   //Setando Motores da Garra como desligados
   output_low(GarraF);
   output_low(GarraA);
   //Setando Leds como desligados
   output_low(LedUsb);
   output_low(LedPos);
   output_high(LedTrab);
   output_low(LedGarra);
   /*Atribuindo os valores desejados para as juntas 
   como os valores obtidos pela leitura do ADC.
   Isto é feito para que, durante a primeira iteração,
   os valores desejados para as juntas sejam válidos (e não um lixo).
   Uma possibilidade seria definir esses valores iniciais como parâmetros,
   assim toda vez que o sistema fosse iniciado, como primeira tarefa, 
   o braço iria para uma posição inicial determinada por esses parâmetros*/
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
      ////Acionamento do Motor 0
      // Leitura dos sensores da junta 0
      set_adc_channel(0);
      delay_us(20);
      Junta0_Atual = read_adc();
      delay_us(20);
      
      if (Junta0_Desejado < Junta0_Atual-Range || Junta0_Desejado > Junta0_Atual+Range)  //Fora do range
      {
         if(Junta0_Desejado < Junta0_Atual-Range)  //Depois do valor desejado
         { 
            output_high(Junta0E); 
            output_low(Junta0D);
         }
         else  //Antes do Valor desejado 
         { 
            output_high(Junta0D); 
            output_low(Junta0E);
         }
      }
      else  //Dentro do range
      {
         output_low(Junta0D); 
         output_low(Junta0E); 
         a=1;  //Contador de parada
      }
      
      ////Acionamento do Motor 1
      // Leitura dos sensores da junta 1
      set_adc_channel(1);
      delay_us(20);
      Junta1_Atual = read_adc();
      delay_us(20);
      
      if (Junta1_Desejado < Junta1_Atual-Range || Junta1_Desejado > Junta1_Atual+Range)   //Fora do range
      {
         if(Junta1_Desejado < Junta1_Atual-Range)  //Depois do valor desejado
         { 
            output_high(Junta1E); 
            output_low(Junta1D);
         }
         else  //Antes do valor desejado
         { 
            output_high(Junta1D); 
            output_low(Junta1E);
         }
      }
      else  //Dentro do range 
      { 
         output_low(Junta1D); 
         output_low(Junta1E);
         b=1;  //Contador de parada 
      }
      
      ////Acionamento do Motor 2
      // Leitura dos sensores da junta 2
      set_adc_channel(2);
      delay_us(20);
      Junta2_Atual = read_adc();
      delay_us(20);
      
      if (Junta2_Desejado < Junta2_Atual-Range || Junta2_Desejado > Junta2_Atual+Range)  //Fora do range
      {
         if(Junta2_Desejado < Junta2_Atual-Range)  //Depois do valor desejado 
         { 
            output_high(Junta2E); 
            output_low(Junta2D);
         }
         else  //Antes do valor desejado 
         { 
            output_high(Junta2D); 
            output_low(Junta2E);
         }
      }
      else  //Dentro do range 
      {  
         output_low(Junta2D); 
         output_low(Junta2E); 
         c=1; //Contador de parada
      }
      
      ////Acionamento do Motor 3
      // Leitura dos sensores da junta 3
      set_adc_channel(3);
      delay_us(20);
      Junta3_Atual = read_adc();
      delay_us(20);
      
      if (Junta3_Desejado < Junta3_Atual-Range || Junta3_Desejado > Junta3_Atual+Range)  //Fora do range
      {
         if(Junta3_Desejado < Junta3_Atual-Range)  //Depois do valor desejado 
         { 
            output_high(Junta3E);
            output_low(Junta3D);
         }
         else  //Antes do valor desejado 
         { 
            output_high(Junta3D); 
            output_low(Junta3E);
         }
      }
      else  //Dentro do range 
      { 
         output_low(Junta3D); 
         output_low(Junta3E); 
         d=1;  //Contador de parada 
      }
      
      ////Acionamento da Garra
      if (Garra_desejado==1 && input(Fechada)!=0)   //Quer fechar mas ainda não fechou
      {
         output_high (GarraF);
         output_low (GarraA);
      }
      else if (Garra_desejado==0 && input(Aberta)!=0)   //Quer abrir mas ainda não abriu
      {
         output_high (GarraA);
         output_low (GarraF);
      }
      else  //Não quer fechar nem abrir
      {
         output_low (GarraF);
         output_low (GarraA);
         e=1;  //Contador de parada
      }
      
      //Comunicação e sinalização de parada
      if (a==1 && b==1 && c==1 && d==1 && e==1)  //Todos os sensores já chegaram 
      {
         output_high(LedPos);     //Aciona led de parada
         if(z==0)                // O 'z' somente será zero quando um novo dado chegar no buffer 
         {                      //e este for um dado de posicionamento
            usb_task();
            delay_us(20);
            usb_cdc_putc(0);
            z=1;
         }
      }
      
      /* Nova entrada de dados desejados
      via comunicação USB*/
      usb_task();  //Confirma estado do Hardware USB
      delay_us(20);
      
      // Comunicação USB (Entrada de novos dados)
      if (usb_cdc_kbhit())  //Testa dados no buffer de leitura 
      {
         output_high(LedUsb);       //Aciona led comunicação
         output_low(LedPos);        //Desliga led de parada
         
         a=0; b=0; c=0; d=0; e=0;   //zera todos contadores de parada
         //Armazenando dados lidos do buffer em variáveis auxiliares
         Aux_0 = usb_cdc_getc();
         Aux_1 = usb_cdc_getc();
         Aux_2 = usb_cdc_getc();
         Aux_3 = usb_cdc_getc();
         //Testando se os novos dados colhidos são comandos espefícos
         if((char)Aux_0 == 's' && (char)Aux_1 == 't' && (char)Aux_2 == 'o' && (char)Aux_3 == 'p')        //Comando para parar acionamento
         { 
            output_b(0x00);
            Junta0_Desejado = Junta0_Atual;
            Junta1_Desejado = Junta1_Atual;
            Junta2_Desejado = Junta2_Atual;
            Junta3_Desejado = Junta3_Atual;
            z=0; //Zera contador (novo acionamento)
         }
         //Comando para enviar dados dos sensores
         else if((char)Aux_0 == 'r' && (char)Aux_1 == 'e' && (char)Aux_2 == 'a' && (char)Aux_3 == 'd')   
         { 
            usb_cdc_putc(Junta0_Atual);
            usb_cdc_putc(Junta1_Atual);
            usb_cdc_putc(Junta2_Atual);
            usb_cdc_putc(Junta3_Atual);
            z=1; //z deve ser 1 pois após o comando "read" dados não devem ser retornados para o matlab
         }
         //Comando para fechar garra
         else if((char)Aux_0 == 'g' && (char)Aux_1 == 'r' && (char)Aux_2 == 'a' && (char)Aux_3 == 'b')   
         { 
            Garra_desejado=1;
            z=0; //Zera contador (novo acionamento)
         }
         //Comando para abrir garra
         else if((char)Aux_0 == 'd' && (char)Aux_1 == 'r' && (char)Aux_2 == 'o' && (char)Aux_3 == 'p')  
         {
            Garra_desejado=0;
            z=0; //Zera contador (novo acionamento)
         }
         //Comando para ligar/desligar lanterna led
         else if((char)Aux_0 == 'l' && (char)Aux_1 == 'u' && (char)Aux_2 == 'z' && (char)Aux_3 == '!')  
         {
            output_toggle(LedGarra);
            z=1; //z deve ser 1 pois após o comando "read" dados não devem ser retornados para o matlab
         }
         //Comando para posição de repouso
         else if((char)Aux_0 == 's' && (char)Aux_1 == 't' && (char)Aux_2 == 'n' && (char)Aux_3 == 'd')   
         {
            Junta0_Desejado = 127;
            Junta1_Desejado = 127;
            Junta2_Desejado = 110;
            Junta3_Desejado = 127;
            z=0; //Zera contador (novo acionamento)
         }
         /*Se não são comandos, serão dados de novas posições desejadas:
         Limita extremo inferior
         Limita extremo superior
         Atribui nova posição desejada */
         else
         {
            z=0; //Zera contador (novo acionamento)
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
         output_low(LedUsb); //Desliga led de comunicação
      }
      usb_task();  //Confirma estado do Hardware USB
      delay_us(20);
   }
}
