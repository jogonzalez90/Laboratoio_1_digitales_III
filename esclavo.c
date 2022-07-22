#include <esclavo.h>

void main()
{
   //CONFIGURACION GENERAL
   #USE RS232(BAUD=9600, XMIT=PIN_C6, RCV=PIN_C7, BITS=8)//DIRECTIVA UART
   SETUP_COUNTERS (RTCC_INTERNAL, RTCC_DIV_256) ;//CONFIGURACION DEL TIMER
   
   //CONFIGURAMOS PUERTOS
   SET_TRIS_B(0B00001100);
   SET_TRIS_D(0);
   
   //VARIABLES DE USO GENRAL
   INT ESTADO=0;//VARIABLE A USAR EN SWITCH
   INT FLAG=0;//VARIBLE A USAR EN BUS SERIAL
   INT MEM=0;//MEMORIA A USAR EN CONTEO DE BRIQUETAS
   INT REG1=0;//REGISTRO PARA RETARDO DE 5 SEGUNDOS
   
   //DEFINE DE PINES PIC
   #DEFINE DETECD PIN_B2
   #DEFINE PO PIN_B3
   #DEFINE MT PIN_D0
   #DEFINE MEZPIV PIN_D5
   
   OUTPUT_D(0);
   
   while(TRUE)//BUCLE INFINITO
   {
      SWITCH(ESTADO)
      {
         CASE 0:
         {
             OUTPUT_LOW(MEZPIV);//APAGAMOS MEZCALDOR
             FLAG=(GETC());//LEEMOS VARIBLE EN BUS SERIAL
             IF(FLAG==1)//DETERMINAMOS ESTADO DE ESTA VARIBLE
              {
                ESTADO=1;//SALTAMOS A ESTADO 1
                FLAG=0;//INICIALIZAMOS VARIBLE
              }
         }
         BREAK;
         
         CASE 1:
         {
            FLAG=(GETC());//LEEMOS VARIBLE EN BUS SERIAL
            IF(FLAG==1)//DETERMINAMOS ESTADO DE ESTA VARIBLE
            {
                ESTADO=2;//SALTAMOS A ESTADO 2
                FLAG=0;//INICIALIZAMOS VARIBLE
            }
         }
         BREAK;
         
         CASE 2:
         {
            FLAG=(GETC());//LEEMOS VARIBLE EN BUS SERIAL
            IF(FLAG==1)//DETERMINAMOS ESTADO DE ESTA VARIBLE
            {
                ESTADO=3;//SALTAMOS A ESTADO 3
                FLAG=0;//INICIALIZAMOS VARIBLE
            }
         }
         BREAK;
         
         CASE 3:
         {
            OUTPUT_HIGH(MT);//ENCENDEMOS MOTOR BANDA
            IF(INPUT(DETECD)==1)//DETECTOR PASO DE BRIDAS
            {
               MEM++;//VARIBLE CONTEO NUMERO DE BRIDAS
               IF(MEM==2)//DETERMINAMOS EL PASO DE DOS BRIDAS
                {
                  FLAG=1;//CARGAMOS A UNO VARIBALE BUS SERIAL
                  PUTC(FLAG);//ENVIAMOS VARIABLE POR BUS SERIAL
                  DELAY_MS(10);//RETARDO DE 10 MS
                  FLAG=0;//BORRAMOS VALOR VARIBLE
                  MEM=0;//BORRAMOS ESTADO DE MEMORIA BRIDAS
                  ESTADO=4;//SALTAMOS A ESTAO 4
                }
                ELSE
                {
                    DELAY_MS(1000);//RETARDO DE UN SEGUNDO PARA SENSOR BRIDAS
                }
            }
         }  
        BREAK;
         
         CASE 4:
         {
            OUTPUT_LOW(MT);//APAGAMOS MOTOR DE BANDA
            OUTPUT_HIGH(MEZPIV);//ENCENDEMOS MEZCLADOR
           
            reg1=76;//INICIAMOS RETARDO DE 5 SEGUNDOS
             SIGUE1:
               SET_RTCC(0);
               SIGUE_2: 
               IF (get_timer0()==255) // CONFIRMA QUE EL TIMER 0 ESTA LLENO
               {
               reg1--;//DECREMENTO VARIABLE EN UNA UNIDAD
               }
               ELSE
               {GOTO SIGUE_2;}
               if(reg1!=0) GOTO SIGUE1;
               OUTPUT_HIGH(PIN_D1);
               
               TEMP://CICLO PARA DETECTAR EL ACCIONAMIENTO DEL FINAL DE CARRERA PO
               IF(INPUT(PO)==1)//DETERMINAMOS ESTADO DEL FINAL DE CARRERA PO
               {
                  FLAG=1;//CARGAMOS A UNO VARIBALE BUS SERIAL
                  PUTC(FLAG);//ENVIAMOS VARIABLE POR BUS SERIAL
                  DELAY_MS(10);//RETARDO DE 10 MS
                  FLAG=0;//BORRAMOS VALOR VARIBLE
                  ESTADO=0;//RETORNAMOS A POSICION INICIAL O ESTADO CERO
               }
               ELSE
               {
                  GOTO   TEMP;//CICLO GOTO A LA ESPERA DEL ACCIONAMIENTO DE PO
               }
            }  
        BREAK;
        
  }
      }
   }


