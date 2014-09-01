/**********************************************\
 *       Kradac Robotics & Electronics        *
 *                                            *
 *  Proyecto:     Reloj Rally                 *
 *  Programador:  Ernesto P                   *
 *  version:      0.0.1                       *
 *  Fecha:        31/08/2014                  *
 *                                            *
 **********************************************
 *
 *                                        
 *
 *                                 
 *
 *                                    
 *                                                
 *                                                   
 *
 */


#include <18f452.h>
#include "ds1307.c"

#fuses HS, NOWDT,NOLVP, NOPUT, NOPROTECT
#use delay(clock=20M)

#use RS232(BAUD=9600, UART1, PARITY= N, STREAM = COM_int )

//#use RS232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8,STREAM=COM_INT)//Puerto soft skypatrol


#define SZ_BUF_SER_0  250       // TamañoBuffer para el Skypatrol
#define SZ_BUF_SER_1  250      //  Buffer para comunicacion con Arduino

// VARIABLES GLOBALES

   // Inicializacion de los buffer para comunicacion serial
    //
      // Buffer Serial 0 - Hardware
      char bufferSerial_0[ SZ_BUF_SER_0 ];
      
      #if SZ_BUF_SER_0 > 255
         unsigned long i_ser_0 = 0;
      #else
         unsigned int i_ser_0;

      unsigned int serial_0_overflow = 0;


      // Buffer Serial 1 - Software
      char bufferSerial_1[ SZ_BUF_SER_1 ];
      
      #if SZ_BUF_SER_1 > 255
         unsigned long i_ser_1 = 0;
      #else
         unsigned int i_ser_1;

      unsigned int serial_1_overflow = 0;
   
   
   int i_timer;
   int yr;
   int month;
   int day;
   int hrs;
   int min;
   int sec;
   int dow;
   
   long valor, valor1;


/**
 * @brief Manejo de Interrupcion serial
 * @details Coloca los caracteres que llegan al puerto serial
 * en un arreglo de caracteres, si el mensaje enviado supera
 * el tamano del buffer se pone en alto la bandera: 
 * serial_0_overflow = 1;
 */
#INT_RDA 
   void rda_isr()
   {
      if ( kbhit( COM_INT ) )
      {
         bufferSerial_0[ i_ser_0++ ] = fgetc( COM_INT );

         //Deja siempre un Byte de terminacion del buffer 0x00
         if( ++i_ser_0 == SZ_BUF_SER_0 ) 
         {
            i_ser_0 = 0;
            serial_0_overflow = 1;  // Se desbordo el Serial_0  COM_INT
         }
      }
   }

void main(void)
{
   
   enable_interrupts( GLOBAL );
   enable_interrupts( INT_RDA );
   enable_interrupts( INT_TIMER0 );
   // ENABLE WDT
   ds1307_init();

   while(1)
   {
        //ds1307_get_date (day, month, yr, dow);  // se obtiene la fecha
      ds1307_get_time (hrs, min, sec);
      
      output_high(PIN_C1);
      delay_ms(500);
      output_low(PIN_C1);
      
      fprintf(COM_INT, "%d:%d:%d\n",hrs,min,sec );
      
   }
}

