#include <control_temp.h>

void main()
{
   //CONFIGURACION GENERAL
   SETUP_ADC_PORTS(ALL_ANALOG);//CONFIGURAMOS TODO EL PUERTO COMO SALIDA ANALOGA
   SETUP_ADC(ADC_CLOCK_DIV_32);//CONFIGURAMOS VELOCIDAD DE CONVERSION
   
   #ASM
      BSF    3.5
      BSF    0X1F.7
      BCF    3.5
   #ENDASM
   
   SET_ADC_CHANNEL(0);//CANAL A USAR PARA CONVERSION
   
   //CONFIGURAMOS PUERTOS
   SET_TRIS_B(0B00000001);
   SET_TRIS_C(0);
   SET_TRIS_D(0);
   
   //INICIALIZAMO SALIDAS A 0
   OUTPUT_LOW(PIN_B1);
   OUTPUT_LOW(PIN_B2);
   OUTPUT_LOW(PIN_B3);
   OUTPUT_LOW(PIN_B4);
   OUTPUT_LOW(PIN_B5);
   OUTPUT_LOW(PIN_B6);
   OUTPUT_LOW(PIN_B7);
   OUTPUT_LOW(PIN_C0);
   OUTPUT_LOW(PIN_C1);
   OUTPUT_LOW(PIN_C2);
   OUTPUT_LOW(PIN_C3);
   OUTPUT_LOW(PIN_C4);
   OUTPUT_LOW(PIN_C5);
   OUTPUT_LOW(PIN_C6);
   OUTPUT_LOW(PIN_C7);
   OUTPUT_LOW(PIN_D0);
   OUTPUT_LOW(PIN_D1);
   OUTPUT_LOW(PIN_D2);
   OUTPUT_LOW(PIN_D3);
   OUTPUT_LOW(PIN_D4);
   OUTPUT_LOW(PIN_D5);
   OUTPUT_LOW(PIN_D6);
   OUTPUT_LOW(PIN_D7);
   
   
   INT CONVERSIONH=0;//VARIABLE PARA TOMAR CONVERSION
   INT CONVERSIONL=0;//VARIABLE PARA TOMAR CONVERSION
   INT ESTADO=0;

   while(TRUE)
   {
      
      SWITCH (ESTADO)
      {
         CASE 0:
         {
            IF(INPUT(PIN_B0)==1)
            {
             //  CONVERSIONL=READ_ADC();//ENCENDEMOS MODULOS PARA INICIAR CONVERSION Y GUARDAMOS EN LA VARIABLE
               #ASM
               
              // BCF    0X3.5
               BSF    0X1F.2
            ESPERAR:
               BTFSC 0X1F.2
               GOTO   ESPERAR
               //BCF    0X3.5
               MOVF   0X1E,W
               MOVWF  0X21
               BSF    03.5
               MOVF   0X1E,W
               BCF    03.5
               MOVWF  0X22
               
               #ENDASM
               
               
               DELAY_MS(1);//RETARO PARA LA CONVERSION
               OUTPUT_C(CONVERSIONH);//SACAMOS POR EL PUERTO C LA CONVERSION ALTA
               
               #ASM
                  MOVLW  0X0
                  MOVWF  0X20
               #ENDASM
               
               OUTPUT_D(CONVERSIONL);//SACAMOS POR EL PUERTO D LA CONVERSION BAJA
               ESTADO=1;
            }
            ELSE
            {
               OUTPUT_LOW(PIN_B1);
               OUTPUT_LOW(PIN_B2);
            }
         }
            BREAK;
            
            CASE 1:
            {
               IF(CONVERSIONL>0B10000000)
               {
                  OUTPUT_LOW(PIN_B1);
                  OUTPUT_HIGH(PIN_B2);
               }
               ELSE
               {
                  IF(CONVERSIONL>0B00010000)
                  {
                     OUTPUT_HIGH(PIN_B1);
                     OUTPUT_LOW(PIN_B2);
                    
                  }
                   ELSE
                   {
                      IF(CONVERSIONL<0B00010000)
                        {
                           OUTPUT_LOW(PIN_B1);
                           OUTPUT_LOW(PIN_B2);
                        }
                   }
               }
               
               ESTADO=0;
            }
            BREAK;
         }
   }
   }


