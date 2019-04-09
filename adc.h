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
 * File: adc
 * Author: Tyler Matthews
 * Comments: adc are used to measure current and temperatures
 * Revision history: 
 */

//Includes
    #include <xc.h> // include processor files - each processor file is guarded.  
    #include "timer.h"
    #include <math.h>

//Defines
    #define GPIO1 00000
    #define GPIO2 00001
    #define GPIO3 00010
    #define GPIO4 00011
    #define TEMPFET LATBbits.LATB5
    #define TEMP1 01100 
    #define TEMP2 01010
    #define TEMP3 01000
    #define TEMP4 01001
    #define TEMP5 01011
    #define CSENSE 10101

//Prototypes
    void adcSetup();
    int adcRead(char ch);
    
    float avgBuff(float buff[], int size);
    
    int getTemps(int temperatures[], int numTemps);
    float getCurrent();
    
    int calculateTemp(int temp);
    float calculateCurrent(float adcValue);

//Variables -- AN12, AN10, AN8, AN9, AN11 ... RB5
    char tempChannels[5] = {0x0C, 0x0A, 0x08, 0x09, 0x0B}; //TEMP1, TEMP2, ...., TEMP5
    
    const int Beta = 3977;
    const float t0 = 298.15;
    
    
    const char temperatures[] = {148, 118, 103, 92, 84, 78, 72, 67, 63, 60,
    56, 53, 51, 48, 45, 43, 41, 39, 37, 34, 33, 31, 29, 27, 25, 23,
    22, 20, 18, 16, 15, 13, 11, 9, 7, 5, 3, 1, -1, -3, -5, -8, -11,
    -14, -17, -21, -26, -32, -42, -273};