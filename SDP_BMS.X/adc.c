/*
 * File:   adc.c
 * Author: trm84
 *
 * Created on February 5, 2019, 1:02 PM
 */
#include "adc.h"

//Returns the current

//Calculates the temperature based off an ADC value
int calculateTemp(int adcValue){
    float voltage = (((float)adcValue/4096.0)*5.0);
    float r = (voltage * 10000.0 / (5.0 - voltage));
    return (int)(((t0 * Beta)/(t0*log(r / 10000.0) + Beta))-273); //Log function is slow...wait to approximate?
}

float calculateCurrent(float adcValue){
    float curr = ((2.5 - ((((float)adcValue)/4095.0)*5.0))/0.04);///0.04;
    if(curr < 0){
        curr * = -1.0;
    }
    return curr;
}

float getCurrent(){
    float currentValue = (float)adcRead((char)CSENSE);
    currentValue = calculateCurrent(currentValue);
       
    return currentValue;
}

//Returns the highest temperature
int getTemps(int temperatures[], int numTemps){
    int highestTemp = 0;
    
    TEMPFET = 0; //Enable temperature readings
    highestTemp = calculateTemp(adcRead(tempChannels[0])); //Read first temp sensor (TEMP1))
    temperatures[0] = highestTemp; //Save value into temperature array
    for(int inc = 1; inc < numTemps; inc ++){ //Read all of the other temp sensors
        temperatures[inc] = calculateTemp(adcRead(tempChannels[inc])); //Save value into temperature array
        if(temperatures[inc] > highestTemp){ //if the measured value is the highest yet, set it (high temp = lower voltage)
            highestTemp = temperatures[inc];
        }
    }

    TEMPFET = 1; //Disable temperature readings
    return highestTemp;    //return the highest temperature
}

//reads ADC value from given channel
int adcRead(char ch){
    ADCON0bits.CHS = ch; //Select Channel
    ADCON0bits.ADON = 1;
    
    __delay_us(15); //Wait for holding cap to charge
    ADCON0bits.GO = 1; //Start Conversion
    
    while(ADCON0bits.DONE == 1);//Wait for conversion to finish
    
    int ansHigh = ADRESH; //Get high byte
    int ansLow = ADRESL; //Get low byte
    
    int total = ((ansHigh << 4) | (ansLow >> 4) & 0x0FFF); //Calculate Total ADC Value
    ADCON0bits.ADON = 0; 
    return total; 
}

//Averages the ADC samples 
float avgBuff(float buff[], int size){
    int inc = 0;
    float total = 0;
    for(inc = 0; inc < size; inc++){
        total += buff[inc];
    }
      
    return ((float) total)/((float) size);
}

void adcSetup(){
    //Setting TEMPFET pin as an output
    TRISBbits.TRISB5 = 0;
    
    //Setting GPIO pins as inputs
    TRISAbits.TRISA0 = 1; //CH0
    TRISAbits.TRISA1 = 1; //CH1
    TRISAbits.TRISA2 = 1; //CH2
    TRISAbits.TRISA3 = 1; //CH3
    
    //Setting TEMP pins as inputs
    TRISBbits.TRISB0 = 1; //CH12
    TRISBbits.TRISB1 = 1; //CH10
    TRISBbits.TRISB2 = 1; //CH8
    TRISBbits.TRISB3 = 1; //CH9
    TRISBbits.TRISB4 = 1; //CH11
    
    //Setting Current Sensor as input
    TRISDbits.TRISD1 = 1;  //CH21
    
    //Enabling Analog Inputs
    ANSELA = 0x0F; //A0, A1, A2, A3
    ANSELB = 0x1F; //B0, B1, B2, B3, B4
    ANSELD = 0x02; //D2
    
    //Configuration Registers
    ADCON0 = 0x00; //12Bit, CH = AN0, ADC conversion not in progress, ADC disabled
    ADCON1 = 0x60; //Sign-Magnitude, FOSC/16 = 32MHz/64 = 500kHz = 2uS, VREF- = Vss, VREF+ = Vdd
    ADCON2 = 0x0F; //AutoTrigger Disabled, Negative Diff Input selected by ADNREF
}