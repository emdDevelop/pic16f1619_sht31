#include "configuration.h"
#include <stdio.h>
#include <string.h>

#define _XTAL_FREQ 8000000 //8Mhz

#define RS LATCbits.LATC3
#define E LATAbits.LATA2
#define D4 LATCbits.LATC4
#define D5 LATCbits.LATC5
#define D6 LATCbits.LATC6
#define D7 LATCbits.LATC7


unsigned char state;
unsigned char flag;

unsigned char response_rcvd;
unsigned char responseID, response = 0;

char received_data[16], ip_address[16],len[4];
unsigned short i, tmp, DataReady;
unsigned char id='1';

const short OK=1;
const short ERROR = 2;
const short NO_CHANGE = 3;
const short FAIL = 4;
const short READY = 5;
const short WIFI_GOT_IP=6;
const short CONNECT=7;
const short SEND_OK=8;
const short SEND_SYMBOL=9;

char Get_Response();
void wifi_message_http(const char* text);
void wifi_message(const char* text);
void config(const unsigned char* mode,short int type);
void interrupt ISR(void);

#include "pin_manager.h"
#include "lcd.h"
#include "comm.h"
#include "esp8266.h"
#include "i2c.h"


void main(void)
{ 
    init_pin_manager();
    
     Lcd_Init();
    __delay_ms(2000);
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("Welcome");
    Lcd_Set_Cursor(2,1);
    Lcd_Write_String("DIO");
         __delay_ms(1000);
   
    eusart_init();
          __delay_ms(1000);
          
    config("1",2); //SETUP ESP // 1 station mode (client) , 2 access point mode (host) , 3 both -> 1 server , 2 client
    response=0;      
    
    Lcd_Clear();      
    i2c_setup();
          __delay_ms(2000);

    //heater_enable(0x44,0x30,0x6D);
    //get_status(0x44,0xF3,0x2D);
   
    while (1)
    {
    sht32_read_measurement(0x44,0x2C,0x06); //High repeatabillity
   // sht32_read_measurement(0x44,0x2C,0x0D); //Medium repeatabillity
   // sht32_read_measurement(0x44,0x2C,0x10); //Low repeatabillity
    /*
          if(DataReady)
     {
      DataReady = 0;
      Lcd_Clear();
      Lcd_Set_Cursor(1,1);
      Lcd_Write_String(received_data);
     }   
     */
    
        __delay_ms(1000);
        
    }
}
/**
 End of File
*/