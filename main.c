/*
 *  File:   main.c
 *  Author: Tyler Matthews
 */

//Includes
    #include <xc.h>
    #include "timer.h"
    #include "ltc6804.h"
    #include "adc.h"
    #include "uart.h"
    #include "i2c.h"
    #include "spi.h"
    #include "SSD1306.h"
    #include "config.h"

//Defines
    #define FOSC 32000000
    #define FCY 32000000/2
    #define numTemps 5
    #define numCurrent 20
    #define numVoltages 12
    #define maxVoltage 50.4 //Battery Pack Voltages at 100% charge
    #define DISCHARGE_EN LATDbits.LATD5 //Discharge Enable Pin
    #define CHARGE_EN  LATDbits.LATD4 //Charge Enable Pin
    #define CHARGE_SWITCH PORTAbits.RA0
    #define uartLines 20

//Prototypes
    void setup();
    int startUp();

//Global Variables
    int j = 0, k = 0 , z = 0; //incrementing variables
    int uartBool = 0; //Timer enabled bool to write to UART for testing
    
    float currentBuff[numCurrent]; //buffer to store current values
    int currentIndex = 0; //index for current buffer
    float current = 0; //Current 
    int currentBool = 0; //Measuring Current bool
    
    float voltages[numVoltages]; //Voltages
    float totalVoltage; //Total Voltage
    
    int highestTemp; //Highest Temperature
    int temps[numTemps]; //Temperatures
    float temp; //
    
    int SOC = 0; //SOC Percentage out of 100
    
    
//Main
void main(void){

    setup();
    DISCHARGE_EN = 0; //Defaults to charge and discharge circuits being off
    CHARGE_EN = 0; 
    NOP();
    /* 
    if(CHARGE_SWITCH == 1 ){
        CHARGE_EN = startUp();   //If startup check is okay, enable charging
    }else{
        DISCHARGE_EN = startUp();               //If startup check is okay, enable discharging
    }
    */
   /*8 int address;
    __delay_ms(1000);
    
    address= 0x27;
    address = 0x27 <<1;
    i2cStart();
    i2cWrite(address); //address
    i2cWrite(0x00);
    i2cWrite(0x08); //turn on
    i2cStop();
    */
    
    while(1){
        
        LATAbits.LATA5 ^= 1;
        __delay_ms(1000);
     
        measureVoltages(voltages, numVoltages); // Voltages 
        totalVoltage = sumVoltages(voltages, numVoltages); 

        highestTemp = getTemps(temps, numTemps); // Temperatures
        
        if(currentBool == 1){ //Add current to buffer
            currentBuff[currentIndex] = getCurrent();
            currentIndex ++;
            
            if(currentIndex >= numCurrent){ //Average buffer to get finalized current value
                current = avgBuff(currentBuff, currentIndex);
                currentIndex = 0;
            }
            currentBool = 0;
        }
        if(uartBool == 1){ //UART
            writeValuesToUart(voltages, numVoltages, totalVoltage, temps, numTemps, highestTemp, current, uartLines);
            uartBool = 0;
        }
     
    }
}
 
//Calculates initial SOC on startup and does the following startup checks
//open circuit for voltage sense, short circuit for current sense
// and open circuit temperature sense)
int startUp(){
    /*highestTemp = getTemps(temps, numTemps);
    for(int i = 0; i < numTemps; i++){
        if(temps[i] < 5 || temps[i] > 50){
            //Two Possibilities:
            //Open circuit temp sensor -- do not allow battery to charge or discharge
            //Batteries are too hot -- possible short circuit
            return 0;
        }
    }*/
    
    current = getCurrent();
    if(current < -2 || current > 2){
       //Current Sensor Issue
        return 0;
    }
    
    measureVoltages(voltages, numVoltages);
    for(int i = 0; i < numVoltages; i++){
        if(voltages[i] > 4.5 || voltages[i] < 3.2){
            //Batteries are over or under charged
            return 0;
        }
    }
    totalVoltage = sumVoltages(voltages, numVoltages); 
    SOC = (int)(100.0*(maxVoltage / totalVoltage)); //Calculate Initial SOC
    return 1;
}

void __interrupt ISR(void){
    //TIMER0 
    if(INTCONbits.TMR0IF == 1 && INTCONbits.TMR0IE == 1){
        k ++;
        if(k >= 3){ //0.1mS
            currentBool = 1;
            k=0;
            j++;
        }
        
        if(j>=10000){//1s
            uartBool = 1;
            j = 0;
        }
        INTCONbits.TMR0IF = 0; //Interrupt Disable
    }
    //Timer2
    if(PIE1bits.TMR2IE == 1 && PIR1bits.TMR2IF == 1){
        currentBool = 1; //Set bool so that current can be measured
        PIR1bits.TMR2IF = 0; //Interrupt Disable
    }
    //UART
    if(PIR1bits.TXIF == 1 && PIE1bits.TXIE == 1){
        if(str[z] != '\0'){
            TXREG = str[z];
            z++;
        }else{
            z = 0;
            uartDisable(); //UART TX INTERRUPT EN
        }
    }
    //SPI
    if(PIR1bits.SSP1IF == 1 && PIE1bits.SSP1IE == 1){
        PIR1bits.SSP1IF = 0;
    }
     
}
  
void setup(void){
    //Misc
        TRISDbits.TRISD4 = 0; //Enable Charge Pin
        TRISDbits.TRISD5 = 0; //Enable Discharge Pin
        TRISAbits.TRISA0 = 1; //Charge Enable Switch -- 5V = Charge, 0V = Discharge
        
    //General Setup
        INTCONbits.GIE = 1; //GLOBAL INTERRUPT EN
        INTCONbits.PEIE = 1; //PERIPERIAL INTERRUPT EN
      
    //Timer
        timerSetup();
   
    //LED
        TRISAbits.TRISA5 = 0; //Test LED
        LATAbits.LATA5 = 1; //Turn on LED
        
    //GPIO
        TRISAbits.TRISA4 = 0;
     
    //ADCs
        adcSetup();
    
    //UART
        uartSetup();
        
    //I2C
       i2cSetup();
   
    //SPI: PIC is Master -> LTC6804-2 is Slave
    //   spiSetup();
        
    //LTC
    //   LTC6804_initialize();
        
}