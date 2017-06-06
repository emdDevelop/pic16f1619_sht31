void eusart_init(void){
    
  //Configure I/O pins
ANSELBbits.ANSB5 = 0; //RB5 pin configured as digital
TRISBbits.TRISB7=1;   //port B7 for eusart i/o TX input
TRISBbits.TRISB5=1;   //port B5 for eusart i/o RX input automatically configured   
    
 TX1STAbits.TXEN=1;  //Transmit Enable bit 1= Transmit enabled  
 TX1STAbits.BRGH=1;  //High Baud Rate Select bit 1= High speed  
 BAUD1CONbits.BRG16=0;  //16-bit Baud Rate Generator bit 0= 8-bit baud rate generator is used
 TX1STAbits.TX9=0;
 TX1STAbits.SYNC=0;  //USART Mode Select bit 0 = Asynchronous mode 
 RC1STAbits.CREN=1;  //Continuous Receive Enable bit 1= Enables receiver
 
 SPBRG=51;  //Writing SPBRG Register for 9600 baud rate for FOSC=8MHz
 
PIE1bits.RCIE=1; //Enable usart receive interrupt
PIE1bits.TXIE=0; //disable uart transmitt
RC1STAbits.SPEN=1;  //Serial Port Enable bit 1= Serial port enabled (configures RX/DT and TX/CK pins as serial port pins)
}

void eusartWriteChar(const char c){
      
    while(!TX1STAbits.TRMT)//wait until transmit shift register is empty
         ;
    TXREG=c; //write character to TXREG and start transmission
}

void eusartWriteText(const char *text){
    
    while(*text){
     eusartWriteChar(*text);
     text++;
    }
}


void eusart_check_tr(void){
       
}

char eusart_read_char(void){
    while(!RCIF)
        ;
     return RCREG; 
}

void eusart_read_text(void){
    
       
}

void eusart_check_rx(void){
    
       
}
  