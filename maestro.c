#include <maestro.h>

void main()
{
   //CONFIGURACION GENERAL
   #USE RS232(BAUD=9600, XMIT=PIN_C6, RCV=PIN_C7, BITS=8)//DIRECTIVA USO UART
   SETUP_ADC_PORTS(ALL_ANALOG);//CONFIGURAMOS TODO EL PUERTO COMO SALIDA ANALOGA
   SETUP_ADC(ADC_CLOCK_DIV_32);//CONFIGURAMOS VELOCIDAD DE CONVERSION
   
   //JUSTIFICACION A LA DERECHA DE ADRESH Y ADRESL
   
   #ASM
      BSF    3.5
      BSF    0X1F.7
      BCF    3.5
   #ENDASM
   
   SET_ADC_CHANNEL(0);//CANAL A USAR PARA CONVERSION AN0
   
   //CONFIGURAMOS PUERTOS
   SET_TRIS_B(0B00000001);
   SET_TRIS_D(0);
   
   //VARIABLES DE USO GENERAL
   INT TEMP_H=0;//VARIABLE TEMPERATURA BYTE ALTO
   INT TEMP_L=0;//VARIABLE TEMPERATURA BYTE BAJO
   INT ESTADO=0;//VARIABLE A USAR EN SWITCH
   INT FLAG=0;//VARIABLE USAR EN BUS SERIAL
   
   //DEFINE DE PINES PIC
   #DEFINE START PIN_B0
   #DEFINE VA PIN_D0
   #DEFINE VB PIN_D1
   #DEFINE VC PIN_D2
   
  // OUTPUT_D(0);//BORRAMOS SALIDA D
   OUTPUT_D(0);
   
   while(TRUE)//BUCLE INFINITO
   {
     SWITCH(ESTADO)
      {
         CASE 0:
         {
            IF(INPUT(START)==1)//DETERMINAMOS ESTADO DEL INTERRUPTOR START
            {
               FLAG=1;//VALOR A ENVIAR EN BUS SERIAL
               PUTC(FLAG);//ENVIAMOS DATO POR BUS SERIAL
               DELAY_MS(10);//RETARDO DE 10 MS
               FLAG=0;//INICIALIZAMOS VARIABLE
               ESTADO=1;//SALTAMOS A ESTADO UNO
            }
         }
         BREAK;
         
         CASE 1:
         {
            //INICIAMOS CONVERSION ADC
            #ASM         
             BSF    0X1F.2
          ESPERAR_1:
             BTFSC 0X1F.2
             GOTO   ESPERAR_1
             MOVF   0X1E,W
             MOVWF  0X21
             BSF    03.5
             MOVF   0X1E,W
             BCF    03.5
             MOVWF  0X22
             #ENDASM
             DELAY_MS(1);//RETARO PARA LA CONVERSION
            
            OUTPUT_HIGH(VA); //ENCENDEMOS VALVULA A
            IF(TEMP_L>=0B11010000 && TEMP_H==0B00000000)//EVALUAMOS VALOR DE 1 VOLTIO EN RESGISTROS DE CONVERSION
            {
               FLAG=1;//VALOR A ENVIAR EN BUS SERIAL
               PUTC(FLAG);//ENVIAMOS DATO POR BUS SERIAL
               DELAY_MS(10);//RETARDO DE 10 MS
               FLAG=0;//INICIALIZAMOS VARIABLE
               ESTADO=2;//SALTAMOS A ESTADO DOS
            }
         }
         BREAK;
         
         CASE 2:
         {
            //INICIAMOS CONVERSION ADC
             #ASM         
             BSF    0X1F.2
          ESPERAR_2:
             BTFSC 0X1F.2
             GOTO   ESPERAR_2
             MOVF   0X1E,W
             MOVWF  0X21
             BSF    03.5
             MOVF   0X1E,W
             BCF    03.5
             MOVWF  0X22
             #ENDASM
             DELAY_MS(1);//RETARO PARA LA CONVERSION
         
            OUTPUT_LOW(VA); //APAGAMOS VALVULA A
            OUTPUT_HIGH(VB); //ENCENDEMOS VALVULA B
            IF(TEMP_L>0B10100001 && TEMP_H==0B00000001)//EVALUAMOS VALOR DE 2 VOLTIOS EN RESGISTROS DE CONVERSION
            {
               FLAG=1;//VALOR A ENVIAR EN BUS SERIAL
               PUTC(FLAG);//ENVIAMOS DATO POR BUS SERIAL
               DELAY_MS(10);//RETARDO DE 10 MS
               FLAG=0;//INICIALIZAMOS VARIABLE
               ESTADO=3;//SALTAMOS A ESTADO 3
            }
         }
         BREAK;
         
         CASE 3:
         {
            OUTPUT_LOW(VB);//APAGAMOS VALVULA B
            OUTPUT_HIGH(VC);//ENCENDEMOS VALVULA C
            FLAG=(GETC());//LEEMOS VARIBLE POR BUS SERIAL
            IF(FLAG==1)//DETERMINAMOS ESTADO DE ESTA VARIBLE
              {
                ESTADO=4;//SALTAMOS A ESTADO 4
                FLAG=0;//INICIALIZAMOS VARIBLE
              }
            }  
          BREAK;
         
         CASE 4:
         {
            OUTPUT_LOW(VC);//APAGAMOS VALVULA C
            FLAG=(GETC());//LEEMOS VARIBLE EN BUS SERIAL
             IF(FLAG==1)//DETERMINAMOS ESTADO DE ESTA VARIBLE
              {
                ESTADO=0;//SALTAMOS A ESTADO 0
                FLAG=0;//INICIALIZAMOS VARIBLE
              }
            
            }  
        BREAK;
        
   }
      }
   }


