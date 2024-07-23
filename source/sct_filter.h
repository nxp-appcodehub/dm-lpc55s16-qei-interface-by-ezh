/*
 * Copyright 2014-2016 Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef _SCT_FILTER_H_
#define _SCT_FILTER_H_

/***********************************************************************************************************************
 * Included files
 **********************************************************************************************************************/
#include "fsl_common.h"
#include "fsl_clock.h"
#include "fsl_sctimer.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/

#define SCT0_PERIPHERAL SCT0
/* Definition of clock source frequency */
#define SCT0_CLOCK_FREQ CLOCK_GetFreq(kCLOCK_BusClk)
/* SCT0 interrupt vector ID (number). */
#define SCT0_IRQN SCT0_IRQn
/* SCT0 interrupt handler identifier. */
#define SCT0_IRQHANDLER SCT0_IRQHandler
/* SCTimer output 0 mask */
#define SCT0_OUTPUT_0 (1U <<  kSCTIMER_Out_0)
/* SCTimer output 1 mask */
#define SCT0_OUTPUT_1 (1U <<  kSCTIMER_Out_1)
/* Sync input 0 mask */
#define SCT0_INPUTSYNC_0 (1U << 0U)
/* Sync input 1 mask */
#define SCT0_INPUTSYNC_1 (1U << 1U)

/***********************************************************************************************************************
 * Global variables
 **********************************************************************************************************************/
extern const sctimer_config_t SCT0_initConfig;
extern uint32_t SCT0_event[8];

/***********************************************************************************************************************
 * Initialization functions
 **********************************************************************************************************************/

void sct_filter_init(uint16_t ch0_filter_value, uint16_t ch1_filter_val);

#if defined(__cplusplus)
}
#endif

#endif /* _SCT_FILTER_H_ */
