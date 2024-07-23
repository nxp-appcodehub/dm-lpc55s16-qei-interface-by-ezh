/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_spi.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_debug_console.h"

#include <stdbool.h>
#include "fsl_power.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_SPI_MASTER          SPI8


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
#define BUFFER_SIZE (2)
static uint8_t srcBuff[BUFFER_SIZE];
static uint8_t destBuff[BUFFER_SIZE];
/*******************************************************************************
 * Code
 ******************************************************************************/
void Led_Display(uint16_t Code){
	spi_transfer_t xfer            = {0};

 /* Init Buffer*/
	srcBuff[0] = Code;
	srcBuff[1] = Code >> 8;

	/*Start Transfer*/
	xfer.txData      = srcBuff;
	xfer.rxData      = NULL;
	xfer.dataSize    = sizeof(srcBuff);
	xfer.configFlags = kSPI_FrameAssert;
	SPI_MasterTransferBlocking(EXAMPLE_SPI_MASTER, &xfer);
    GPIO_PinWrite(PINSFUNC_QEI_CS_GPIO, PINSFUNC_QEI_CS_PORT, PINSFUNC_QEI_CS_PIN, 0);
    GPIO_PinWrite(PINSFUNC_QEI_CS_GPIO, PINSFUNC_QEI_CS_PORT, PINSFUNC_QEI_CS_PIN, 1);
 }
