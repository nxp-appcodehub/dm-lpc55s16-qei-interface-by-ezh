/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***********************************************************************************************************************
 * Included files
 **********************************************************************************************************************/
#include "sct_filter.h"

const sctimer_config_t SCT0_initConfig = {
    .enableCounterUnify = false,
    .clockMode = kSCTIMER_System_ClockMode,
    .clockSelect = kSCTIMER_Clock_On_Rise_Input_0,
    .enableBidirection_l = false,
    .enableBidirection_h = false,
    .prescale_l = 0U,
    .prescale_h = 0U,
    .outInitState = (uint8_t)(SCT0_OUTPUT_0 | SCT0_OUTPUT_1),
    .inputsync = (uint8_t)(SCT0_INPUTSYNC_0 | SCT0_INPUTSYNC_1)};
uint32_t SCT0_event[8];

void sct_filter_init(uint16_t ch0_filter_value, uint16_t ch1_filter_val)
{
    SCTIMER_Init(SCT0_PERIPHERAL, &SCT0_initConfig);
    /*SCT_L*/
    /* Initialization of state 0 */
    SCTIMER_CreateAndScheduleEvent(SCT0_PERIPHERAL, kSCTIMER_InputHighEvent, 0, kSCTIMER_Input_0, kSCTIMER_Counter_L, &SCT0_event[0]);
    SCTIMER_SetupCounterLimitAction(SCT0_PERIPHERAL, kSCTIMER_Counter_L, SCT0_event[0]);
    SCTIMER_SetupCounterStartAction(SCT0_PERIPHERAL, kSCTIMER_Counter_L, SCT0_event[0]);
    SCTIMER_SetupNextStateAction(SCT0_PERIPHERAL, 1, SCT0_event[0]);
    SCTIMER_CreateAndScheduleEvent(SCT0_PERIPHERAL, kSCTIMER_InputLowEvent, 0, kSCTIMER_Input_0, kSCTIMER_Counter_L, &SCT0_event[2]);
    SCTIMER_SetupCounterLimitAction(SCT0_PERIPHERAL, kSCTIMER_Counter_L, SCT0_event[2]);
    SCTIMER_SetupCounterStartAction(SCT0_PERIPHERAL, kSCTIMER_Counter_L, SCT0_event[2]);
    SCTIMER_SetupNextStateAction(SCT0_PERIPHERAL, 2, SCT0_event[2]);
    SCTIMER_IncreaseState(SCT0_PERIPHERAL);
    /* Initialization of state 1 */
    SCTIMER_CreateAndScheduleEvent(SCT0_PERIPHERAL, kSCTIMER_MatchEventOnly, ch0_filter_value, kSCTIMER_Out_0, kSCTIMER_Counter_L, &SCT0_event[1]);
    SCTIMER_SetupCounterLimitAction(SCT0_PERIPHERAL, kSCTIMER_Counter_L, SCT0_event[1]);
    //For test
    SCTIMER_SetupOutputSetAction(SCT0_PERIPHERAL, kSCTIMER_Out_2, SCT0_event[1]);
    //For test
    SCTIMER_SetupOutputSetAction(SCT0_PERIPHERAL, kSCTIMER_Out_8, SCT0_event[1]);
    SCTIMER_SetupNextStateAction(SCT0_PERIPHERAL, 0, SCT0_event[1]);
    SCTIMER_ScheduleEvent(SCT0_PERIPHERAL, SCT0_event[2]);
    SCTIMER_IncreaseState(SCT0_PERIPHERAL);
    /* Initialization of state 2 */
    SCTIMER_ScheduleEvent(SCT0_PERIPHERAL, SCT0_event[0]);
    SCTIMER_CreateAndScheduleEvent(SCT0_PERIPHERAL, kSCTIMER_MatchEventOnly, ch0_filter_value, kSCTIMER_Out_0, kSCTIMER_Counter_L, &SCT0_event[3]);
    SCTIMER_SetupCounterLimitAction(SCT0_PERIPHERAL, kSCTIMER_Counter_L, SCT0_event[3]);
    //For test
    SCTIMER_SetupOutputClearAction(SCT0_PERIPHERAL, kSCTIMER_Out_2, SCT0_event[3]);
    //For test
    SCTIMER_SetupOutputClearAction(SCT0_PERIPHERAL, kSCTIMER_Out_8, SCT0_event[3]);
    SCTIMER_SetupNextStateAction(SCT0_PERIPHERAL, 0, SCT0_event[3]);

    /*SCT_H*/
    /* Initialization of state 0 */
    SCTIMER_CreateAndScheduleEvent(SCT0_PERIPHERAL, kSCTIMER_InputHighEvent, 0, kSCTIMER_Input_1, kSCTIMER_Counter_H, &SCT0_event[4]);
    SCT0_PERIPHERAL->EV[SCT0_event[4]].CTRL |= SCT_EV_CTRL_HEVENT(1U);
    SCT0_PERIPHERAL->EV[SCT0_event[4]].STATE = (1UL << 0);
    SCT0_PERIPHERAL->LIMIT |= SCT_LIMIT_LIMMSK_H(1UL << SCT0_event[4]);
    SCT0_PERIPHERAL->START |= SCT_START_STARTMSK_H(1UL << SCT0_event[4]);
    SCTIMER_SetupNextStateAction(SCT0_PERIPHERAL, 1, SCT0_event[4]);
    SCTIMER_CreateAndScheduleEvent(SCT0_PERIPHERAL, kSCTIMER_InputLowEvent, 0, kSCTIMER_Input_1, kSCTIMER_Counter_H, &SCT0_event[6]);
    SCT0_PERIPHERAL->EV[SCT0_event[6]].CTRL |= SCT_EV_CTRL_HEVENT(1U);
    SCT0_PERIPHERAL->EV[SCT0_event[6]].STATE = (1UL << 0);
    SCT0_PERIPHERAL->LIMIT |= SCT_LIMIT_LIMMSK_H(1UL << SCT0_event[6]);
    SCT0_PERIPHERAL->START |= SCT_START_STARTMSK_H(1UL << SCT0_event[6]);
    SCTIMER_SetupNextStateAction(SCT0_PERIPHERAL, 2, SCT0_event[6]);

    /* Initialization of state 1 */
    SCTIMER_CreateAndScheduleEvent(SCT0_PERIPHERAL, kSCTIMER_MatchEventOnly, ch1_filter_val, kSCTIMER_Out_0, kSCTIMER_Counter_H, &SCT0_event[5]);
    SCT0_PERIPHERAL->EV[SCT0_event[5]].CTRL |= SCT_EV_CTRL_HEVENT(1U);
    SCT0_PERIPHERAL->EV[SCT0_event[5]].STATE = (1UL << 1);
    SCT0_PERIPHERAL->LIMIT |= SCT_LIMIT_LIMMSK_H(1UL << SCT0_event[5]);
    //For test
    SCTIMER_SetupOutputSetAction(SCT0_PERIPHERAL, kSCTIMER_Out_6, SCT0_event[5]);
    //For test
    SCTIMER_SetupOutputSetAction(SCT0_PERIPHERAL, kSCTIMER_Out_9, SCT0_event[5]);
    SCTIMER_SetupNextStateAction(SCT0_PERIPHERAL, 0, SCT0_event[5]);
    SCT0_PERIPHERAL->EV[SCT0_event[6]].STATE |= (1UL << 1);
    /* Initialization of state 2 */
    SCT0_PERIPHERAL->EV[SCT0_event[4]].STATE |= (1UL << 2);
    SCTIMER_CreateAndScheduleEvent(SCT0_PERIPHERAL, kSCTIMER_MatchEventOnly, ch1_filter_val, kSCTIMER_Out_0, kSCTIMER_Counter_H, &SCT0_event[7]);
    SCT0_PERIPHERAL->EV[SCT0_event[7]].STATE = (1UL << 2);
    SCT0_PERIPHERAL->EV[SCT0_event[7]].CTRL |= SCT_EV_CTRL_HEVENT(1U);
    SCT0_PERIPHERAL->LIMIT |= SCT_LIMIT_LIMMSK_H(1UL << SCT0_event[7]);
    //For test
    SCTIMER_SetupOutputClearAction(SCT0_PERIPHERAL, kSCTIMER_Out_6, SCT0_event[7]);
    //For test
    SCTIMER_SetupOutputClearAction(SCT0_PERIPHERAL, kSCTIMER_Out_9, SCT0_event[7]);
    SCTIMER_SetupNextStateAction(SCT0_PERIPHERAL, 0, SCT0_event[7]);

    SCTIMER_StartTimer(SCT0_PERIPHERAL, (uint32_t)kSCTIMER_Counter_L | (uint32_t)kSCTIMER_Counter_H);
}
