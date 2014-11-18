#include <18F4550.h>
#device ADC = 8
#fuses HS, NOWDT, NOPROTECT, NOLVP, BROWNOUT, MCLR, CPUDIV4
#use delay (crystal=20000000, clock=5000000)
#use rs232 (baud=9600, parity=N, xmit=PIN_C6, rcv=PIN_C7, bits=8)

/*Pinos sensores
pin_a0 (Sensor 0)
pin_a1 (Sensor 1)
pin_a2 (Sensor 2)
pin_a3 (Sensor 3)
*/
//Pinos sensores da Garra
#define Aberta pin_a4
#define Fechada pin_a5
#define LedGarra pin_d0
//Pinos enable para Pontes-H
#define Enable_0 pin_d2
#define Enable_1 pin_d3
#define Enable_2 pin_c4
#define Enable_3 pin_c5
//Leds de Informa��o
#define Led_B pin_c0               // Azul
#define Led_G pin_c1               // Verde
#define Led_R pin_c2               // Vermelho
/*Pinos de acionamento de motores
das juntas*/
#define Junta0E pin_b7
#define Junta0D pin_b6
#define Junta1E pin_b5
#define Junta1D pin_b4
#define Junta2E pin_b3
#define Junta2D pin_b2
#define Junta3E pin_b1
#define Junta3D pin_b0
/*Pinos de acionamento de motores
da garra*/
#define GarraA pin_d7
#define GarraF pin_d6

/*Vari�vel de faixa de toler�ncia
para posicionamento das juntas 
(por unidade de Bits do ADC)*/
unsigned int8 Range=4;
//Vari�veis auxiliares para sinaliza��o de parada 
int1 a=0; int1 b=0; int1 c=0; int1 d=0; int1 e=0;
//Vari�vel auxiliar para comunica��o de parada, inicia em 1 para n�o enviar o sinal na primeira itera��o
int1 z=1;
/*Vari�veis para armazenamento dos valores 
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
/* Vari�veis auxiliares para 
verifica��o de comandos em uma 
nova entrada de dados desejados*/
unsigned int8 Aux_0;
unsigned int8 Aux_1;
unsigned int8 Aux_2;
unsigned int8 Aux_3;

#int_RDA
void RDA(void)
{ 
         output_high(Led_R);            //Quando recebe dado aciona led na cor azul
         output_high(Led_G);
         output_low(Led_B);
         
          a=0; b=0; c=0; d=0; e=0;   //zera todos contadores de parada
         //Armazenando dados lidos do buffer em vari�veis auxiliares
         Aux_0 = getc();
         Aux_1 = getc();
         Aux_2 = getc();
         Aux_3 = getc();
         //Testando se os novos dados colhidos s�o comandos espef�cos
         if((char)Aux_0 == 's' && (char)Aux_1 == 't' && (char)Aux_2 == 'o' && (char)Aux_3 == 'p')        //Comando para parar acionamento
         {  
            output_low(Junta0D);output_low(Junta1D);output_low(Junta2D);output_low(Junta3D);
            output_low(Junta0E);output_low(Junta1E);output_low(Junta2E);output_low(Junta3E);
            Junta0_Desejado = Junta0_Atual;
            Junta1_Desejado = Junta1_Atual;
            Junta2_Desejado = Junta2_Atual;
            Junta3_Desejado = Junta3_Atual;
            z=0;
         }
         //Comando para enviar dados dos sensores
         else if((char)Aux_0 == 'r' && (char)Aux_1 == 'e' && (char)Aux_2 == 'a' && (char)Aux_3 == 'd')   
         { 
            putc(Junta0_Atual);
            putc(Junta1_Atual);
            putc(Junta2_Atual);
            putc(Junta3_Atual);
            z=1; //z deve ser 1 pois ap�s o comando "read" dados n�o devem ser retornados 
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
            z=1; //z deve ser 1 pois ap�s o comando "read" dados n�o devem ser retornados 
         }
         //Comando para posi��o de repouso
         else if((char)Aux_0 == 's' && (char)Aux_1 == 't' && (char)Aux_2 == 'n' && (char)Aux_3 == 'd')   
         {
            Junta0_Desejado = 127;
            Junta1_Desejado = 127;
            Junta2_Desejado = 110;
            Junta3_Desejado = 127;
            z=0; //Zera contador (novo acionamento)
         }
         /*Se n�o s�o comandos, ser�o dados de novas posi��es desejadas:
         Limita extremo inferior**
         Limita extremo superior**
         Atribui nova posi��o desejada */
         else                                                             
         {
            z=0; //Zera contador (novo acionamento)
            //if (Aux_0 < 50) Junta0_Desejado = 50;           
            //else if (Aux_0 > 190) Junta0_Desejado = 190;    
            //else 
            Junta0_Desejado = Aux_0;
            //if (Aux_1 < 25) Junta1_Desejado = 25;           
            //else if (Aux_1 > 195) Junta1_Desejado = 195;
            //else 
            Junta1_Desejado = Aux_1;
            //if (Aux_2 < 40) Junta2_Desejado = 40;
            //else if (Aux_2 > 250) Junta2_Desejado = 250;
            //else 
            Junta2_Desejado = Aux_2;
            //if (Aux_3 < 70) Junta3_Desejado = 70;
            //else if (Aux_3 > 190) Junta3_Desejado = 190;
            //else 
            Junta3_Desejado = Aux_3;
         }
}
void main ()
{  
   //Habilitando interrup��es
   enable_interrupts(INT_RDA);
   enable_interrupts(GLOBAL);
   /*Setando portas de leitura 
   anal�gica*/
   SETUP_ADC_PORTS(AN0_TO_AN3);
   SETUP_ADC(ADC_CLOCK_INTERNAL);
   delay_us(20);
   //Setando pinos como entradas digitais
   output_float(Aberta);
   output_float(Fechada);
 
   //Setando Led na cor amarela
   output_low(Led_R);
   output_low(Led_G);
   output_high(Led_B);
   //reseta enable's
   output_low(Enable_0);
   output_low(Enable_1);
   output_low(Enable_2);
   output_low(Enable_3);
   //Setando Porto de motores como desligado
   output_low(Junta0E);
   output_low(Junta0D);
   output_low(Junta1E);
   output_low(Junta1D);
   output_low(Junta2E);
   output_low(Junta2D);
   output_low(Junta3E);
   output_low(Junta3D);
   output_low(GarraF);
   output_low(GarraA);
   //Setando led da garra como desligado 
   output_low(LedGarra);
   /*Atribuindo os valores desejados para as juntas 
   como os valores obtidos pela leitura do ADC.
   Isto � feito para que, durante a primeira itera��o,
   os valores desejados para as juntas sejam v�lidos (e n�o um lixo).
   Uma possibilidade seria definir esses valores iniciais como par�metros,
   assim toda vez que o sistema fosse iniciado, como primeira tarefa, 
   o bra�o iria para uma posi��o inicial determinada por esses par�metros*/
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
   
   while(true)
   {
      ////Acionamento do Motor 0
      // Leitura dos sensores da junta 0
      set_adc_channel(0);
      delay_us(20);
      Junta0_Atual = read_adc();
      delay_us(20);
      
      if (Junta0_Desejado < Junta0_Atual-Range || Junta0_Desejado > Junta0_Atual+Range)  //Fora do range
      {
         //Seta enable (Provis�rio -> at� que sejam usados PWMs)
         output_high(Enable_0);
         
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
         //Reseta enable
         output_low(Enable_0);
         
         output_low(Junta0D); 
         output_low(Junta0E); 
         if (a==0) a=1;  //Contador de parada
      }
      
      ////Acionamento do Motor 1
      // Leitura dos sensores da junta 1
      set_adc_channel(1);
      delay_us(20);
      Junta1_Atual = read_adc();
      delay_us(20);
      
      if (Junta1_Desejado < Junta1_Atual-Range || Junta1_Desejado > Junta1_Atual+Range)   //Fora do range
      {
         //Seta enable (Provis�rio -> at� que sejam usados PWMs)
         output_high(Enable_1);
         
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
         //Reseta enable
         output_low(Enable_1);
         
         output_low(Junta1D); 
         output_low(Junta1E);
         if (b==0) b=1;  //Contador de parada 
      }
      
      ////Acionamento do Motor 2
      // Leitura dos sensores da junta 2
      set_adc_channel(2);
      delay_us(20);
      Junta2_Atual = read_adc();
      delay_us(20);
      
      if (Junta2_Desejado < Junta2_Atual-Range || Junta2_Desejado > Junta2_Atual+Range)  //Fora do range
      {
         //Seta enable (Provis�rio -> at� que sejam usados PWMs)
         output_high(Enable_2);
         
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
         //Reseta enable
         output_low(Enable_2);
         
         output_low(Junta2D); 
         output_low(Junta2E); 
         if (c==0) c=1; //Contador de parada
      }
      
      ////Acionamento do Motor 3
      // Leitura dos sensores da junta 3
      set_adc_channel(3);
      delay_us(20);
      Junta3_Atual = read_adc();
      delay_us(20);
      
      if (Junta3_Desejado < Junta3_Atual-Range || Junta3_Desejado > Junta3_Atual+Range)  //Fora do range
      {
         //Seta enable (Provis�rio -> at� que sejam usados PWMs)
         output_high(Enable_3);
         
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
         //Reseta enable
         output_low(Enable_3);
         
         output_low(Junta3D); 
         output_low(Junta3E); 
         if (d==0) d=1;  //Contador de parada 
      }
      
      ////Acionamento da Garra
      if (Garra_desejado==1 && input(Fechada)!=0)   //Quer fechar mas ainda n�o fechou
      {
         output_high (GarraF);
         output_low (GarraA);
      }
      else if (Garra_desejado==0 && input(Aberta)!=0)   //Quer abrir mas ainda n�o abriu
      {
         output_high (GarraA);
         output_low (GarraF);
      }
      else  //N�o quer fechar nem abrir
      {
         output_low (GarraF);
         output_low (GarraA);
         e=1;  //Contador de parada
      }
      
      //Comunica��o e sinaliza��o de parada
      if (a==1 && b==1 && c==1 && d==1 && e==1)  //Todos os sensores j� chegaram 
      {
         if(z==0)                // O 'z' somente ser� zero quando um novo dado chegar no buffer 
         {                      //e este for um dado de posicionamento
            printf("OK");
            z=1;
         }
      }
      
      if (z==0)
      {
         output_low(Led_R);            //Aciona led na cor vermelha
         output_high(Led_G);
         output_high(Led_B);
      }
      else
      {
         output_high(Led_R);            //Aciona led na cor verde
         output_low(Led_G);
         output_high(Led_B);
      }
   }
}
