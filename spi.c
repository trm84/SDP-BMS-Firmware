/*
 * File:   spi.c
 * Author: trm84
 *
 * Created on February 20, 2019, 12:28 PM
 */


#include <xc.h>
#include "spi.h"
                                                                                   
void spiSetup(){
        
       
        LATDbits.LATD3 = 1; //Active low, set high on startup

        APFCON1bits.SDOSEL = 0; //SDO on RC5
        APFCON1bits.SCKSEL = 0; //SCK on RC3
        APFCON1bits.SDISEL = 0; //SDI on RC4

        SSP1CON3bits.BOEN = 0; //Override Buffer if necessary 

        //SSPCON3 only for slave mode

        SSP1STATbits.SMP = 1; //Data sampled at rising edge of clk
        SSP1STATbits.CKE = 1; //clk idle high

        SSP1CON1 = 0x12; //Serial Port Pins Config, Idle CLK HIGH, clk = FSC/64

        PIE1bits.SSP1IE = 1; //Enable interrupt
        PIR1bits.SSP1IF = 0; //Clear interrupt flag

        SSP1CON1bits.SSPEN = 1; //Enable SPI
}

void spi_write(char data){
    SSP1BUF = data;
    while(SSP1STATbits.BF == 0);
    char readData =  SSP1BUF;
}

char spi_read(char data){
    SSP1BUF = data;
    while(SSP1STATbits.BF == 0);
    char readData =  SSP1BUF;
    return readData;
}
void spiSwitch(){
        LATDbits.LATD3 = 1; //Active low, set high on startup
        APFCON1bits.SDOSEL = 0; //SDO on RC5
        APFCON1bits.SCKSEL = 0; //SCK on RC3
        APFCON1bits.SDISEL = 0; //SDI on RC4
        
        SSP1CON1 = 0x12; //Serial Port Pins Config, Idle CLK HIGH, clk = FSC/64
        SSP1CON1bits.SSPEN = 1; //Enable SPI
}