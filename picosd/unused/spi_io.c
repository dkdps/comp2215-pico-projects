/*
 *  File: spi_io.c.example
 *  Author: Nelson Lombardo
 *  Year: 2015
 *  e-mail: nelson.lombardo@gmail.com
 *  License at the end of file.
 */

#include "spi_io.h"
#include "hardware/resets.h"
#include "hardware/clocks.h"
#include "hardware/spi.h"

// SPI Pins
#define SCLK 10
#define SDO 11
#define SDI 12
#define CS 15 

// Data Pins
#define DAT1 13
#define DAT2 14

#define SPI_PORT spi1

/******************************************************************************
 Module Public Functions - Low level SPI control functions
******************************************************************************/


void SPI_Init (void) {

    stdio_init_all();
    spi_init(SPI_PORT, 500000) // SPI 1 at 500KHz

    // Initialise GPIO pins for SPI
    gpio_set_function(SCLK, GPIO_FUNC_SPI);
    gpio_set_function(SDO, GPIO_FUNC_SPI);
    gpio_set_function(SDI, GPIO_FUNC_SPI);
    gpio_set_function(CSn, GPIO_FUNC_SPI);

    // Configure Chip Select
   
    gpio_init(CS);
    gpio_set_dir(CS, GPIO_OUT);
    gpio_put(CS, 1);  // Set High to indicate no SPI communication.
}

BYTE SPI_RW (BYTE d) {
    while(!(SPI0_S & SPI_S_SPTEF_MASK));
    SPI0_D = d;
    while(!(SPI0_S & SPI_S_SPRF_MASK));
    return((BYTE)(SPI0_D));
}

void SPI_Release (void) {
    WORD idx;
    for (idx=512; idx && (SPI_RW(0xFF)!=0xFF); idx--);
}

inline void SPI_CS_Low (void) {
    gpio_put(CS, 0);//CS LOW
}

inline void SPI_CS_High (void){
    gpio_put(CS, 1); //CS HIGH
}

inline void SPI_Freq_High (void) {
    //TODO:what is the max frequency?
    spi_set_baudrate(SPI_PORT, 400000);
}

inline void SPI_Freq_Low (void) {
    spi_set_baudrate(SPI_PORT, 400000);
}

void SPI_Timer_On (WORD ms) {
    SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;  // Make sure clock is enabled
    LPTMR0_CSR = 0;                     // Reset LPTMR settings
    LPTMR0_CMR = ms;                    // Set compare value (in ms)
    // Use 1kHz LPO with no prescaler
    LPTMR0_PSR = LPTMR_PSR_PCS(1) | LPTMR_PSR_PBYP_MASK;
    // Start the timer and wait for it to reach the compare value
    LPTMR0_CSR = LPTMR_CSR_TEN_MASK;
}

inline BOOL SPI_Timer_Status (void) {
    return (!(LPTMR0_CSR & LPTMR_CSR_TCF_MASK) ? TRUE : FALSE);
}

inline void SPI_Timer_Off (void) {
    LPTMR0_CSR = 0;                     // Turn off timer
}

#ifdef SPI_DEBUG_OSC
inline void SPI_Debug_Init(void)
{
    SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK; // Port A enable
    PORTA_PCR12 = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK  | PORT_PCR_PS_MASK;
    GPIOA_PDDR |= (1 << 12); // Pin is configured as general-purpose output, for the GPIO function.
    GPIOA_PDOR &= ~(1 << 12); // Off
}
inline void SPI_Debug_Mark(void)
{
    GPIOA_PDOR |= (1 << 12); // On
    GPIOA_PDOR &= ~(1 << 12); // Off
}
#endif

/*
The MIT License (MIT)

Copyright (c) 2015 Nelson Lombardo

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
