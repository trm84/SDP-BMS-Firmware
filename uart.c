/*
 * File:   uart.c
 * Author: trm84
 *
 * Created on February 6, 2019, 9:05 AM
 */


#include "uart.h"

void writeValuesToUart(float voltageArr[], int voltageArrLength, float totalVoltage, int temperatureArr[], int temperatureArrLength, int temperatureHigh, float current, float soc, int uartLines){
    clearScreen(uartLines);
    writeVoltages(voltageArr, voltageArrLength, totalVoltage);
    writeTemps(temperatureArr, temperatureHigh , temperatureArrLength);
    writeCurrent(current);
    writeSOC(soc);
}

void writeSOC(float soc){
    sprintf(&str[0], "SOC = %0.4f percent \n\r", (soc*100));
    uartEnable();
    while(PIE1bits.TXIE);
}

void writeVoltages(float volts[], int length, float totalVoltage){
    int maxCell = 0;
    int minCell = 0;
    for(int k = 0; k < length; k++){
        if(volts[k]<volts[minCell]){
            minCell = k;
        }else if(volts[k] > volts[maxCell]){
            maxCell = k;
        }
        
        sprintf(&str[0], "V%i = %0.4fV\n\r", k+1, volts[k]);
        uartEnable();
        while(PIE1bits.TXIE);
    }
    //writes the pack voltage to the uart buffer
    sprintf(&str[0], "Pack Voltage: %0.4fV\n\r", totalVoltage); 
    uartEnable();
    while(PIE1bits.TXIE);
    
    sprintf(&str[0], "Max Difference = V%i & V%i @ %0.4fV\n\r", minCell+1, maxCell+1, (volts[maxCell] - volts[minCell]));     
    uartEnable();
    while(PIE1bits.TXIE);
    
}

void writeCurrent(float current){
    sprintf(&str[0], "current = %0.4fA\n\r", current);
    uartEnable();
    while(PIE1bits.TXIE);
}
void writeTemps(int temps[], int highestTemp, int numTemps){
    for(int k = 0; k<numTemps; k++){ //loops through temperature array and writes each temp to uart buffer
        sprintf(&str[n], "Temp%i = %iC\n\r", k+1, temps[k]); // 
        uartEnable();
        while(PIE1bits.TXIE);
   }

    //writes the highest temperature to the uart buffer
    sprintf(&str[0], "Highest Temp: %iC\n\r", highestTemp); 
    uartEnable();
    while(PIE1bits.TXIE);
}

void clearScreen(int numLines){
    int j = 0;
    
    for(j = 0; j < numLines-1; j++){
       sprintf(&str[0], "\33[2K \033[A");
        uartEnable();
        while(PIE1bits.TXIE);
    }
    //Moves to beginning of the line
    sprintf(&str[0], "\33[2K \033[A \r");
    uartEnable();
    while(PIE1bits.TXIE);
}

void uartEnable(){
    PIE1bits.TXIE = 1;
}

void uartDisable(){
    PIE1bits.TXIE = 0;   
}

void uartSetup(){
    TXSTAbits.TX9 = 0; //8 bit transmission
    TXSTAbits.TXEN = 1; //TX Enable
    TXSTAbits.SYNC = 0; //Asynch
    TXSTAbits.BRGH = 0; //Low Baud
    RCSTAbits.RX9 = 0; //8 BIT DATA TRANSFER
    RCSTAbits.SPEN = 1; //SERIAL PORT EN
    SPBRGH = 0; //VAUD RATE
    SPBRGL = 2; //BAUD RATE: 207 = 2400, 51 = 9600, 47 = 10417, 25 = 19200, 3 = 125000
    PIR1bits.TXIF = 0; //Interrupt Flag
}
