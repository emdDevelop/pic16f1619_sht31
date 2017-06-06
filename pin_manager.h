    
void init_pin_manager(){
    
    ANSELAbits.ANSA2 = 0; //RA2 pin configured as digital
    ANSELCbits.ANSC3 = 0; //RC3 pin configured as digital
    ANSELCbits.ANSC7 = 0; //RC7 pin configured as digital
    ANSELCbits.ANSC6 = 0; //RC6 pin configured as digital
    ADCON0bits.ADON = 0;  //disable ADC
    CM1CON0bits.C1ON = 0; // Disable comparators
    
    //LCD GPIO configuration
    TRISAbits.TRISA2 =0; //configure as output
    TRISCbits.TRISC3 =0; //configure as output
    TRISCbits.TRISC4 =0; //configure as output
    TRISCbits.TRISC5 =0; //configure as output
    TRISCbits.TRISC6 =0; //configure as output
    TRISCbits.TRISC7 =0; //configure as output
    
    //I2C GPIO configuration
    ODCONB = 0x00; //Port pin operates as standard push-pull drive (source and sink current)
    WPUB = 0xF0; //Pull-up enabled
    OPTION_REGbits.nWPUEN = 0;//Weak pull-ups are enabled by individual WPUx latch values
    ANSELBbits.ANSB4 = 0; //RB4 pin configured as digital
    ANSELBbits.ANSB6 = 0; //RB6 pin configured as digital
    TRISBbits.TRISB4=1; //configure as input  SDA
    TRISBbits.TRISB6=1; //configure as input  SCL
    
    //  configure peripheral pin select 
    /*The PPS includes a mode in which all input and output
    selections can be locked to prevent inadvertent
    changes. PPS selections are locked by setting the
    PPSLOCKED bit of the PPSLOCK register. Setting and
    clearing this bit requires a special sequence as an extra
    precaution against inadvertent changes.*/
    INTCONbits.GIE=0; //Disable general interrupts
    PPSLOCK = 0x55;//START SEQUENCE
    PPSLOCK = 0xAA;//Part of the sequence
    PPSLOCKbits.PPSLOCKED = 0x00; // unlock PPS  Part of the sequence

    SSPCLKPPS = 0x0E;   //RB6->MSSP:SCL;
    SSPDATPPS = 0x0C;   //RB4->MSSP:SDA;
    RXPPS  = 0x0D;         //RB5->EUSART RX
    RB6PPS = 0x10;      //RB6->MSSP:SCL;
    RB4PPS = 0x11;      //RB4->MSSP:SDA;
    RB7PPS = 0x12;        //RB7->TX EUSART      

    PPSLOCK = 0x55;//Part of the sequence
    PPSLOCK = 0xAA;//Part of the sequence
    PPSLOCKbits.PPSLOCKED = 0x01; // lock PPS - Part of the sequence
    
    INTCONbits.PEIE=1;// enable periphereal interrupts
    INTCONbits.GIE=1; //enable general interrupts
    
   //End of settings ............................................................

    return ;
}