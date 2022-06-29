#include <parcial_dos.h>

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
   
   SET_TRIS_B(0B00000101);
   SET_TRIS_C(0);
   
   INT TEMP_H=0;//VARIABLE TEMPERATURA BYTE ALTO
   INT TEMP_L=0;//VARIABLE TEMPERATURA BYTE BAJO
   INT ESTADO=0;//VARIABLE A USAR EN SWITCH
   INT MEM=0;
   INT REG1=0;
   
   #DEFINE START PIN_B0
   #DEFINE DETECD PIN_B2
   #DEFINE PO PIN_B3
   #DEFINE VA PIN_C0
   #DEFINE VB PIN_C1
   #DEFINE VC PIN_C2
   #DEFINE MT PIN_C3
   #DEFINE MEZPIV PIN_C4
   
   OUTPUT_C(0);
   
   while(TRUE)
   {
      SWITCH(ESTADO)
      {
         CASE 0:
         {
            OUTPUT_LOW(MEZPIV);
            OUTPUT_LOW(VC);
           /*
            #ASM         
             BSF    0X1F.2
          ESPERAR:
             BTFSC 0X1F.2
             GOTO   ESPERAR
             MOVF   0X1E,W
             MOVWF  0X21
             BSF    03.5
             MOVF   0X1E,W
             BCF    03.5
             MOVWF  0X22
             #ENDASM
             DELAY_MS(1);//RETARO PARA LA CONVERSION
             */
            IF(INPUT(START)==1)
            {
               ESTADO=1;
            }
         }
         BREAK;
         
         CASE 1:
         {
         
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
            
            OUTPUT_HIGH(VA); 
            IF(TEMP_L>0B11010000 && TEMP_H==0B00000000)
            {
               ESTADO=2;
            }
         }
         BREAK;
         
         CASE 2:
         {
         
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
         
            OUTPUT_LOW(VA); 
            OUTPUT_HIGH(VB); 
            IF(TEMP_L>0B11010000 && TEMP_H==0B00000001)
            {
               ESTADO=3;
            }
         }
         BREAK;
         
         CASE 3:
         {
            OUTPUT_LOW(VB);
            OUTPUT_HIGH(VC);
            OUTPUT_HIGH(MT);
             
            IF(INPUT(DETECD)==1)
            {
               MEM++;
               DELAY_MS(1000);
               IF(MEM==2)
                {
                  MEM=0;
                  ESTADO=4;
                }
            }  
         }
         BREAK;
         
         CASE 4:
         {
            OUTPUT_LOW(MT);
            OUTPUT_HIGH(MEZPIV);
            
            reg1=76;//RETARDO
             SIGUE1:
               SET_RTCC(0);
               SIGUE_2: 
               IF (get_timer0()==255) // CONFIRMA QUE EL TIMER 0 ESTA LLENO
               {
               reg1--;
               }
               ELSE
               {GOTO SIGUE_2;}
               if(reg1!=0) GOTO SIGUE1;
     
               TEMP:
               IF(INPUT(PO)==1)
               {
                  ESTADO=0;
               }
               GOTO   TEMP;
            }  
        BREAK;
   }
      }
   }


