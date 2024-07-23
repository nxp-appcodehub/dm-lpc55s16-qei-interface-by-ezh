/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "peripherals.h"
#include "board.h"
#include "fsl_smartdma.h"
#include "fsl_power.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define COUNT_MAX       100  /* QEI count maximum value in decimal */
#define SCAN_PERIOD	  300*150 /* Scan period in 30us @150MHz */

/*******************************************************************************
 * Variables
 ******************************************************************************/
smartdma_qei_param_t smartdmaParam;                  /*!< SMARTDMA function parameters. */
volatile uint8_t g_samrtdma_stack[32];
volatile uint32_t g_phase_gpio_register[8] = {
		(uint32_t)& GPIO->B[1][5], /*PHASE_A, P1_5,Pin Data Register*/
		(uint32_t)& GPIO->B[0][16],/*PHASE_B, P0_16,Pin Data Register*/
};
volatile uint32_t QEI_Data[16]={
		0,/*index 0, reserved*/
		0,/*index 1, reserved*/
		0,/*index 2, QEI counter*/
		0,/*index 3, direction of QEI*/
		0,/*index 4, reserved*/
		0,/*index 5, reserved*/
		0,/*index 6, reserved*/
		0,/*index 7, reserved*/
		0,/*index 8, flag to show that QEI has up-to-date data*/
		0,/*index 9, reserved*/
		COUNT_MAX,/*index 10, QEI count maximum value in decimal*/
		0,
};

volatile uint32_t QEIScanPeriod = SCAN_PERIOD;

volatile uint16_t shift_code=0x1111;
volatile uint8_t  shift_number=0;
volatile uint8_t  code_mode_number=0;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern void Led_Display(uint16_t Code);

/*******************************************************************************
 * Code
 ******************************************************************************/
static void SmartDMA_QEI_callback(void *param){

	if(QEI_Data[8] == 1){ //if QEI have up-to-date data
		PRINTF("Counter: %d \r\n",QEI_Data[2]);
		if(QEI_Data[3] == 0xB){//if direction is backward
			PRINTF("Direction:Anti-Clockwise\r\n\r\n");
			if(shift_number++ %2 == 0){
				shift_code = (shift_code >> 1 | (shift_code << 15));	// Rotate Right 1 bit
			}
			Led_Display(shift_code);
		}else if(QEI_Data[3] == 0xF){//if direction is forward
			PRINTF("Direction:Clockwise \r\n\r\n");
			if(shift_number++ %2 == 0){
				shift_code = (shift_code << 1 | (shift_code >> 15));	// Rotate Left	1 bit
			}
			Led_Display(shift_code);
		}else{
			PRINTF("Direction:stop \r\n");
		}
		QEI_Data[8] = 0;//clear the flag after fetch by arm core

	}
 }

void pio1_18_gint_callback(void)
{
	switch(code_mode_number++ %4){
		case 0:	shift_code=0x0001;		break;
		case 1:	shift_code=0x0101;		break;
		case 2:	shift_code=0x00FF;		break;
		case 3:	shift_code=0x1111;		break;
		default:break;
	}
	Led_Display(shift_code);}

/*!
 * @brief Main function
 */
int main(void)
{
    char ch;

    /* Init board hardware. */
    /* set BOD VBAT level to 1.65V */
    POWER_SetBodVbatLevel(kPOWER_BodVbatLevel1650mv, kPOWER_BodHystLevel50mv, false);
    /* attach main clock divide to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    BOARD_InitBootPeripherals();
    Led_Display(shift_code);
#if !defined(DONT_ENABLE_FLASH_PREFETCH)
    /* enable flash prefetch for better performance */
    SYSCON->FMCCR |= SYSCON_FMCCR_PREFEN_MASK;
#endif
    PRINTF("lpc5516 qei demo.\r\n");

	SMARTDMA_InitWithoutFirmware();
	SMARTDMA_InstallFirmware(SMARTDMA_QEI_MEM_ADDR,s_smartdmaQEIFirmware,
								SMARTDMA_QEI_FIRMWARE_SIZE);
	SMARTDMA_InstallCallback(SmartDMA_QEI_callback, NULL);
	NVIC_EnableIRQ(Reserved46_IRQn);
	NVIC_SetPriority(Reserved46_IRQn, 3);

	smartdmaParam.smartdma_stack 	 = (uint32_t*)g_samrtdma_stack;
	smartdmaParam.p_gpio_reg  		 = (uint32_t*)g_phase_gpio_register;
	smartdmaParam.p_qei_data		 = (uint32_t*)QEI_Data;
	smartdmaParam.p_scan_period		 = (uint32_t*)&QEIScanPeriod;
	SMARTDMA_Boot(kSMARTDMA_qei, &smartdmaParam, 0x2);

    while (1)
    {
    }
}
