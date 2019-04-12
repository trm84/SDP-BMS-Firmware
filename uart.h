/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File: uart
 * Author: Tyler Matthews
 * Comments: UART is used to communicate to a console via the FTDI chip
 * Revision history: 
 */

//Includes
    #include <xc.h> // include processor files - each processor file is guarded.  
    #include "timer.h"
    #include <stdio.h>

//Defines

//Variables
    char str[50]; //Character Buffer
    int n; //Array Location
    
//Prototypes
    void writeValuesToUart(float voltageArr[], int voltageArrLength, float totalVoltage, int temperatureArr[], int temperatureArrLength, int temperatureHigh, float current, float soc, int uartLines);
    void uartSetup();
    void writeVoltages(float volts[], int length, float totalVoltage);
    void writeTemps(int temps[], int highestTemp, int numTemps);
    void clearScreen(int numLines);
    void uartEnable();
    void uartDisable();
    void writeCurrent(float current);
    void writeSOC(float soc);