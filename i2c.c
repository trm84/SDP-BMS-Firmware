/*
 * File:   i2c.c
 * Author: trm84
 *
 * Created on February 14, 2019, 12:44 PM
 */

#include "i2c.h"

//Display Ox3C;

void i2cTest(){

}
void i2cWrite(char data){
    SSPBUF = data; //Write to buffer
    __delay_us(100); //Delay to ensure buffer collision doesn't occur
}
void i2cStart(){
    SSPCON2bits.SEN = 1; //Initiates Start condition
    __delay_us(100);
}
void i2cStop(){
    SSPCON2bits.PEN = 1; //Initiates Stop Condition
    __delay_us(100);
}
char i2cHasStarted(){
    return SSPSTATbits.S; //1 if started, 0 if not
}
char i2cHasStoped(){
    return SSPSTATbits.P; //1 is stopped, 0 if it hasn't
}

void i2cSwitch(){
    APFCON1bits.SDISEL = 1; //SDA on RB6
    APFCON1bits.SCKSEL = 1; //SCL on RB7
    SSPCON1 = 0x28; //Sets up i2c as master and enables the serial port (SCL & SDA)
}

void i2cSetup(){ 
    APFCON1bits.SDISEL = 1; //SDA on RB6
    APFCON1bits.SCKSEL = 1; //SCL on RB7
    PIE1bits.SSP1IE = 0; //Enables MSSP interrupt
    PIE2bits.BCL1IE = 0; //Disables MSSP Bus collisions interrupt
    PIR1bits.SSP1IF = 0; //MSSP Interrupt Flag
    PIR2bits.BCL1IF = 0; //Bus collision interrupt flag
    SSPADD = 0x4F; //Baud Rate generator set to 100KHz when Fosc = 32MHz
    SSPCON1 = 0x28; //Sets up i2c as master and enables the serial port (SCL & SDA)
    SSPSTAT = 0xC0; //Slew rate, input logic thresholds, & read only status bits
}