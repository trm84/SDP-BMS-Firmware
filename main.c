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
    #define NUM_TEMPS 5
    #define NUM_CURRENT 20
    #define NUM_VOLTAGES 12
    #define MAX_VOLTAGE 50.4 //Battery Pack Voltages at 100% charge
    #define CAPACITY 12 //Ahr
    #define DISCHARGE_EN LATDbits.LATD5 //Discharge Enable Pin
    #define CHARGE_EN  LATDbits.LATD4 //Charge Enable Pin
    #define CHARGE_SWITCH PORTAbits.RA0
    #define UART_LINES 21
    #define TEST_LED LATAbits.LATA5

//Prototypes
    void setup();
    int startUp(int highestTemp, int temps[], float voltages[], float totalVoltage, float current, int *soc);
    char running();
    
//Global Variables
    int j = 0, k = 0 , z = 0; //incrementing variables
    int uartBool = 0; //Timer  enabled bool to write to UART for testing
    int currentBool = 0; //Measuring Current bool
    
    
    
    
    
    
//Main
void main(void){    
    float voltages[NUM_VOLTAGES]; //Voltages
    float totalVoltage; //Total Voltage
    
    int currentIndex = 0; //Index for current buffer
    float currentBuff[NUM_CURRENT]; //Buffer to store current values
    float current = 0; //Current 
    
    int temps[NUM_TEMPS]; //Temperatures
    int highestTemp; //Highest Temperature

    int numFaults = 0; //Number of faults
    int soc = 0; //SOC Percentage out of 100
    
    setup();
    
    __delay_ms(1000); //start delay
    
    DISCHARGE_EN = 0; //Defaults to charge and discharge circuits being off
    CHARGE_EN = startUp(&highestTemp, temps[], voltages[], &totalVoltage, &current, &soc); 
    
    DISCHARGE_EN = 1;
    /* Design for charging circuit went belly up -- might bodge it in
    if(CHARGE_SWITCH == 1 ){
        CHARGE_EN = startUp(highestTemp);   //If startup check is okay, enable charging
    }else{
        DISCHARGE_EN = startUp(highestTemp);               //If startup check is okay, enable discharging
    }
    */

    
    while(1){
        
       // while(running());
       // while(!(running()));
        TEST_LED ^= 1;
        
        /*MEASUREMENTS*/
        //VOLTAGE
        measureVoltages(voltages, &totalVoltage, NUM_VOLTAGES); // Voltages 
        //TEMPERATURE
        highestTemp = getTemps(temps, NUM_TEMPS); // Temperatures
        //CURRENT
         if(currentBool == 1){ //Add current to buffer
            currentBuff[currentIndex] = getCurrent();
            currentIndex ++;
            if(currentIndex >= NUM_CURRENT){ //Average buffer to get finalized current value
                cellBalancing(voltages, NUM_VOLTAGES); //Balance 
                current = avgBuff(currentBuff, currentIndex);
                currentIndex = 0;
            }
            currentBool = 0;
        }
        /*END MEASUREMENTS*/
        
        /*FAULT CHECKING*/
        //TEMPERATURE
        for(int i = 0; i <NUM_TEMPS; i++){
            if(temps[i] >= 40 || temps[i] <= 10){
                numFaults++;
            }
        }
        //CURRENT
        if(current >= 10){
            numFaults++;
        }
        //VOLTAGES
        /**********/
        //COUNT FAULTS
        if(numFaults >= 10){
            DISCHARGE_EN = 0;
        }
        /*END FAULT CHECKING*/
        
       /*WRITE DATA TO DISP*/
        //UART
        if(uartBool == 1){ //UART
            writeValuesToUart(voltages, NUM_VOLTAGES, totalVoltage, temps, NUM_TEMPS, highestTemp, current, UART_LINES);
            uartBool = 0;
        }
        //I2C
        /**********/
        /*END WRITING DATA TO DISPLAY*/
    }
}

/******************************************************************************/
//int startup()
//Run once on start up. Ensures that batteries are in a safe
//operating condition prior to initial startup. Then calculates initial soc
//to give column counting algorithm an accurate start point
/******************************************************************************/
int startUp(int *highestTemp, int temps[], float voltages[], float *totalVoltage, float *current, int *soc){
    highestTemp = getTemps(temps, NUM_TEMPS);
    for(int i = 0; i < NUM_TEMPS; i++){
        if(temps[i] < 5 || temps[i] > 40){
            //Two Possibilities:
            //Open circuit temp sensor -- do not allow battery to charge or discharge
            //Batteries are too hot -- possible short circuit
            return 0;
        }
    }
    
    current = getCurrent();
    if(current < -2 || current > 2){
       //Current Sensor Issue
        return 0;
    }
    
    measureVoltages(voltages, &totalVoltage, NUM_VOLTAGES);
    for(int i = 0; i < NUM_VOLTAGES; i++){
        if(voltages[i] > 4.2 || voltages[i] < 3.1){
            //Batteries are over or under charged
            return 0;
        }
    }
    totalVoltage = sumVoltages(voltages, NUM_VOLTAGES); 
    soc = (int)(100.0*(MAX_VOLTAGE / totalVoltage)); //Calculate Initial SOC
    return 1;
}

char running(){
 return 1;   
}

/******************************************************************************/
//ISR()
//All interrupts go through this function
//All interrupts have the same priority
/******************************************************************************/
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
  
/******************************************************************************/
//void setup()
//Calls the setup functions for the modules that are used
/******************************************************************************/
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
        TRISBbits.TRISB6 = 1; //i2c requires these be inputs
        TRISBbits.TRISB7 = 1; //i2c requires these be inputs
        i2cSetup();
   
    //SPI: PIC is Master -> LTC6804-2 is Slave
       TRISCbits.TRISC5 = 0; //Data Out Line
       TRISCbits.TRISC4 = 1; //Data In Line
       TRISCbits.TRISC3 = 0; //Driving Clock Line
       TRISDbits.TRISD3 = 0; //CS Pin -- Active when pulled to ground
       spiSetup();

    //LTC
        LTC6804_initialize();
}