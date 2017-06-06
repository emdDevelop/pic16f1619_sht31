
void i2c_setup(){
    SSP1ADD = 19;
    SSP1CON1bits.SSPM=0b1000;   //I2C Master mode, clock = FOSC / (4 * (SSP1ADD+1))    100KHz
    SSP1CON1bits.SSPEN=1; //Enables the serial port and configures the SDA and SCL pins as the source of the serial port pins  
    SSP1STATbits.SMP=1; //Slew rate control disabled for Standard Speed mode (100 kHz and 1 MHz)
    SSP1CON3=0x01; //Minimum of 100 ns hold time on SDA after the falling edge of SCL
    SSPCON2 = 0x00;
    SSP1STAT=0x00;
}

void send_Ack(){
    SSP1CON2bits.ACKDT=0; //prepare to send ACK
    SSP1CON2bits.ACKEN=1; //Initiate ACK
    while(SSP1CON2bits.ACKEN); //Wait for ACK to complete 
    return ;
}

void send_Nack(){
    SSP1CON2bits.ACKDT=1; //prepare to send NACK
    SSP1CON2bits.ACKEN=1; //Initiate ACK
    while(SSP1CON2bits.ACKEN); //Wait for ACK to complete 
    return;
}

void i2c_Stop(){
     SSP1CON2bits.PEN=1; //Initiate stop condition
     while(PEN); //Wait to complete
     return;
}

void heater_enable(char slaveAddr,char commandMsb,char commandLsb){
     SSP1CON2bits.SEN = 1; //Initiate Start condition on SDA and SCL pins. Automatically cleared by hardware.
    while(SSP1CON2bits.SEN);  //Wait until start condition complete                                                                                             
    PIR1bits.SSP1IF=0;  //clear SSP1 interrupt flag

    SSP1BUF = slaveAddr<<1; //Send the slave address + Write bit  when write to I2C bit=0;                                                                                                                                           
    while(!PIR1bits.SSP1IF);
    PIR1bits.SSP1IF=0; //Wait for Acknowledge.ssp1if is set every 9th clock cycle
    if(SSP1CON2bits.ACKSTAT){
        SSP1CON2bits.PEN=1; //Initiate stop condition
        while(PEN);
        return;
    }
        
    SSP1BUF = commandMsb; //Send commandLsb                                                                                                                                          
    while(!PIR1bits.SSP1IF);
    PIR1bits.SSP1IF=0; //Wait for Acknowledge.ssp1if is set every 9th clock cycle
    if(SSP1CON2bits.ACKSTAT){
        SSP1CON2bits.PEN=1; //Initiate stop condition
        while(PEN);
        return;
    }                        
    
     SSP1BUF = commandLsb; //Send commandLsb                                                                                                                                          
    while(!PIR1bits.SSP1IF);
    PIR1bits.SSP1IF=0; //Wait for Acknowledge.ssp1if is set every 9th clock cycle
    if(SSP1CON2bits.ACKSTAT){
        SSP1CON2bits.PEN=1; //Initiate stop condition
        while(PEN); //Wait to complete
        return;
    }  
    i2c_Stop();
}

void get_status(char slaveAddr,char commandMsb,char commandLsb){
    unsigned char statusMsb;
    unsigned char statusLsb;
    unsigned char statusCrc;
    unsigned short status;
    char statusGet[5];
    
    SSP1CON2bits.SEN = 1; //Initiate Start condition on SDA and SCL pins. Automatically cleared by hardware.
    while(SSP1CON2bits.SEN);  //Wait until start condition complete                                                                                             
    PIR1bits.SSP1IF=0;  //clear SSP1 interrupt flag

    SSP1BUF = slaveAddr<<1; //Send the slave address + Write bit  when write to I2C bit=0;                                                                                                                                           
    while(!PIR1bits.SSP1IF);
    PIR1bits.SSP1IF=0; //Wait for Acknowledge.ssp1if is set every 9th clock cycle
    if(SSP1CON2bits.ACKSTAT){
        SSP1CON2bits.PEN=1; //Initiate stop condition
        while(PEN);
        return;
    }
        
    SSP1BUF = commandMsb; //Send commandLsb                                                                                                                                          
    while(!PIR1bits.SSP1IF);
    PIR1bits.SSP1IF=0; //Wait for Acknowledge.ssp1if is set every 9th clock cycle
    if(SSP1CON2bits.ACKSTAT){
        SSP1CON2bits.PEN=1; //Initiate stop condition
        while(PEN);
        return;
    }                        
    
     SSP1BUF = commandLsb; //Send commandLsb                                                                                                                                          
    while(!PIR1bits.SSP1IF);
    PIR1bits.SSP1IF=0; //Wait for Acknowledge.ssp1if is set every 9th clock cycle
    if(SSP1CON2bits.ACKSTAT){
        SSP1CON2bits.PEN=1; //Initiate stop condition
        while(PEN); //Wait to complete
        return;
    }  
    
    i2c_Stop();
    
    //time to read from slave.....................................................................
    SSP1CON2bits.SEN = 1; //Initiate Start condition on SDA and SCL pins. Automatically cleared by hardware.
    while(SSP1CON2bits.SEN);  //Wait until start condition complete                                                                                             
    PIR1bits.SSP1IF=0;  //clear SSP1 interrupt flag
    
    SSP1BUF = (slaveAddr<<1) | 1; //Send the slave address + read bit  when read from I2C bit=1;                                                                                                                                           
    while(!PIR1bits.SSP1IF);
    PIR1bits.SSP1IF=0; //Wait for Acknowledge.ssp1if is set every 9th clock cycle
    if(SSP1CON2bits.ACKSTAT){
        SSP1CON2bits.PEN=1; //Initiate stop condition
        while(PEN);
        return;
    }
    
    SSP1CON2bits.RCEN=1; //Enables Receive mode for I2C
    while(!SSP1STATbits.BF);  //Wait for Receive complete, SSP1BUF is full
    statusMsb = SSP1BUF;
    send_Ack();
     
    SSP1CON2bits.RCEN=1; //Enables Receive mode for I2C
    while(!SSP1STATbits.BF);  //Wait for Receive complete, SSP1BUF is full
    statusLsb = SSP1BUF;
    send_Ack();
     
    SSP1CON2bits.RCEN=1; //Enables Receive mode for I2C
     while(!SSP1STATbits.BF);  //Wait for Receive complete, SSP1BUF is full
    statusCrc = SSP1BUF;
    send_Nack();
     
    i2c_Stop();
}

void sht32_read_measurement(char slaveAddr,char commandMsb,char commandLsb){
    
unsigned char temperatureMsb;
unsigned char temperatureLsb;
unsigned char temperatureCrc;
unsigned short temperature16Bit;
float temperature;
unsigned char humidityMsb;
unsigned char humidityLsb;
unsigned char humidityCrc;
unsigned short humidity16Bit;
float humidity;

unsigned char temperatureC[]="00";
unsigned char humidityN[]="00";
unsigned char result[]="00 Degrees Celsius, 00% humidity";
    
    SSP1CON2bits.SEN = 1; //Initiate Start condition on SDA and SCL pins. Automatically cleared by hardware.
    while(SSP1CON2bits.SEN);  //Wait until start condition complete                                                                                             
    PIR1bits.SSP1IF=0;  //clear SSP1 interrupt flag

    SSP1BUF = slaveAddr<<1; //Send the slave address + Write bit  when write to I2C bit=0;                                                                                                                                           
    while(!PIR1bits.SSP1IF);
    PIR1bits.SSP1IF=0; //Wait for Acknowledge.ssp1if is set every 9th clock cycle
    if(SSP1CON2bits.ACKSTAT){
        SSP1CON2bits.PEN=1; //Initiate stop condition
        while(PEN);
        return;
    }
        
    SSP1BUF = commandMsb; //Send commandLsb                                                                                                                                          
    while(!PIR1bits.SSP1IF);
    PIR1bits.SSP1IF=0; //Wait for Acknowledge.ssp1if is set every 9th clock cycle
    if(SSP1CON2bits.ACKSTAT){
        SSP1CON2bits.PEN=1; //Initiate stop condition
        while(PEN);
        return;
    }                        
    
     SSP1BUF = commandLsb; //Send commandLsb                                                                                                                                          
    while(!PIR1bits.SSP1IF);
    PIR1bits.SSP1IF=0; //Wait for Acknowledge.ssp1if is set every 9th clock cycle
    if(SSP1CON2bits.ACKSTAT){
        SSP1CON2bits.PEN=1; //Initiate stop condition
        while(PEN); //Wait to complete
        return;
    }  
    
    i2c_Stop();
    
    //time to read from slave..................................................................
    SSP1CON2bits.SEN = 1; //Initiate Start condition on SDA and SCL pins. Automatically cleared by hardware.
    while(SSP1CON2bits.SEN);  //Wait until start condition complete                                                                                             
    PIR1bits.SSP1IF=0;  //clear SSP1 interrupt flag
    
    SSP1BUF = (slaveAddr<<1) | 1; //Send the slave address + read bit  when read from I2C bit=1;                                                                                                                                           
    while(!PIR1bits.SSP1IF);
    PIR1bits.SSP1IF=0; //Wait for Acknowledge.ssp1if is set every 9th clock cycle
    if(SSP1CON2bits.ACKSTAT){
        SSP1CON2bits.PEN=1; //Initiate stop condition
        while(PEN);
    }
    
    SSP1CON2bits.RCEN=1; //Enables Receive mode for I2C
    while(!SSP1STATbits.BF);  //Wait for Receive complete, SSP1BUF is full
    temperatureMsb = SSP1BUF;
    send_Ack();
     
    SSP1CON2bits.RCEN=1; //Enables Receive mode for I2C
    while(!SSP1STATbits.BF);  //Wait for Receive complete, SSP1BUF is full
    temperatureLsb = SSP1BUF;
    send_Ack();
     
     SSP1CON2bits.RCEN=1; //Enables Receive mode for I2C
    while(!SSP1STATbits.BF);  //Wait for Receive complete, SSP1BUF is full
    temperatureCrc = SSP1BUF;
    send_Ack();
     
    SSP1CON2bits.RCEN=1; //Enables Receive mode for I2C
    while(!SSP1STATbits.BF);  //Wait for Receive complete, SSP1BUF is full
    humidityMsb = SSP1BUF;
    send_Ack();
     
    SSP1CON2bits.RCEN=1; //Enables Receive mode for I2C
    while(!SSP1STATbits.BF);  //Wait for Receive complete, SSP1BUF is full
    humidityLsb = SSP1BUF;
    send_Ack();
    
    SSP1CON2bits.RCEN=1; //Enables Receive mode for I2C
    while(!SSP1STATbits.BF);  //Wait for Receive complete, SSP1BUF is full
    humidityCrc = SSP1BUF;
    send_Nack();
 
    i2c_Stop();
    
   //Show results on LCD..............................................................
    temperature16Bit=temperatureMsb<<8 | temperatureLsb;
    temperature=-45+ 175*(temperature16Bit / 65534.0) ;
 
    humidity16Bit=humidityMsb<<8 | humidityLsb;
    humidity=100 * (humidity16Bit / 65535.0);
   
    //sprintf(temperatureC,"%.0f",temperature);
    //sprintf(humidityN,"%.0f",humidity);
    
    temperatureC[0]=(unsigned short)temperature/10 + 48;
    temperatureC[1]=(unsigned short)temperature%10 + 48;
    
    humidityN[0]=(unsigned short)humidity/10 +48;
    humidityN[1]=(unsigned short)humidity%10 +48;
    
    result[0]=temperatureC[0];
    result[1]=temperatureC[1];
    result[20]=humidityN[0];
    result[21]=humidityN[1];
 
    //Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("Celsius:");
    Lcd_Set_Cursor(1,11);
    Lcd_Write_String(temperatureC);
    Lcd_Set_Cursor(1,13);
    Lcd_Write_Char(0b10110010);
    Lcd_Set_Cursor(2,1);
    Lcd_Write_String("Humidity:");
    Lcd_Set_Cursor(2,11);
    Lcd_Write_String(humidityN);
    Lcd_Set_Cursor(2,13);
    Lcd_Write_String("%");
    
    wifi_message(result);
}                                                                       