/*
 * File:   spi.c
 * Author: trm84
 *
 * Created on February 20, 2019, 12:28 PM
 */


#include <xc.h>
#include "spi.h"

void spiSetup(){
            TRISCbits.TRISC5 = 0; //Data Out Line
        TRISCbits.TRISC4 = 1; //Data In Line
        TRISCbits.TRISC3 = 0; //Driving Clock Line
        TRISDbits.TRISD0 = 0; //CS Pin -- Active when pulled to ground
        LATDbits.LATD0 = 1; 

        APFCON1bits.SDOSEL = 0; //SDO on pin 24
        APFCON1bits.SCKSEL = 0; //SCK on pin 18
        APFCON1bits.SDISEL = 0; //SDI on pin 23

        SSP1CON3bits.BOEN = 0; //Override Buffer if necessary 

        //SSPCON3 only for slave mode

        SSP1STATbits.SMP = 1; //Data sampled at middle of data output time
        SSP1STATbits.CKE = 1; //

        SSP1CON1 = 0b00010010; //Serial Port Pins Config, Idle CLK HIGH, clk = FSC/64

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