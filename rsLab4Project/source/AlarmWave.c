/****************************************************************************************
* EE344, rsLab4Project, AlarmWave.c
*   Program generates a sinewave to an audio output while
*   displaying messages to user via LCD and Keypad peripherals on the
*   K65TWR
*
* Robert Sanborn, 10/30/2018
*
****************************************************************************************/
#include "MCUType.h"               /* Include project header file                      */
#include "Lab4Main.h"
#include "AlarmWave.h"



#define ALARM_OFF  (INT8C)(0x14)
#define ALARM_ON   (INT8C)(0x11)

#define CNTDOWN_DACSPS   3124U
#define SAMPLES_PER_HS   14600U

#define DAC_CNTRL_SETUP       0xE0U
#define DAC_HALF_SCALE_L      0x00U
#define DAC_HALF_SCALE_H      0x08U


/* Sine Wave Lookup table of values in Q12 notation for 1+sin(2*PI*t)*/
static const INT16U SineWave[64] = {
        2248,
        2447,
        2642,
        2831,
        3013,
        3185,
        3347,
        3496,
        3631,
        3750,
        3854,
        3940,
        4007,
        4056,
        4086,
        4095,
        4086,
        4056,
        4007,
        3940,
        3854,
        3750,
        3631,
        3496,
        3347,
        3185,
        3013,
        2831,
        2642,
        2447,
        2248,
        2048,
        1847,
        1648,
        1453,
        1264,
        1082,
        910,
        748,
        599,
        464,
        345,
        241,
        155,
        88,
        39,
        9,
        0,
        9,
        39,
        88,
        155,
        241,
        345,
        464,
        599,
        748,
        910,
        1082,
        1264,
        1453,
        1648,
        1847,
        2047
};



/**********************************************************************************
* PIT0_IRQHandler()
*
* Description:          utilizes PIT0 timer for altering state of DAC every
*                       3125 clk ticks, 19200 sps
*
* Return Value:         none
*
* Arguments:            none
**********************************************************************************/
void PIT0_IRQHandler(void);


/**********************************************************************************
* AlarmWaveInit         public function initializes resources for Alarm Wave
*                       such as PIT and DACO from the K65F_TWR, note that clk
*                       signal is assumed to be 180MHz
*
* Return Value:         none
* Arguments:            none
**********************************************************************************/
void AlarmWaveInit(void){
    SIM->SCGC6 |= SIM_SCGC6_PIT(1);

    PIT->MCR &= PIT_MCR_MDIS(0);     /*Start with standard timer enabled*/
    PIT->MCR |=PIT_MCR_FRZ(1);
    NVIC_ClearPendingIRQ(PIT0_IRQn);
    NVIC_EnableIRQ(PIT0_IRQn);


    SIM->SCGC2 |= SIM_SCGC2_DAC0(1);

    DAC0->C0 = DAC_CNTRL_SETUP;
}

/**********************************************************************************
* AlarmWaveControlTask  public function that controls the PIT and DAC
*                       to configure and control the alarm waveform timing
*                       based on the AlarmState
*
*
*
* Return Value:         none
* Arguments:            none
**********************************************************************************/
void AlarmWaveControlTask(void){
    DB3_TURN_ON();

    switch (AlarmState()){
        /*Turn off PIT for Alarm Off*/
        case(ALARM_OFF):
            PIT->CHANNEL[0].TCTRL = (PIT_TCTRL_TIE(0)|PIT_TCTRL_TEN(0));
            DAC0->DAT[0].DATL = (0U);
            DAC0->DAT[0].DATH = (0U);
            break;

        /*Generate and Display Wave for Alarm On*/
        case(ALARM_ON):

            /*Start PIT if transitioning from ALARM_OFF to ALARM_ON
             * If already in ALARM_ON, no change will occur*/
            PIT->CHANNEL[0].LDVAL = CNTDOWN_DACSPS;
            PIT->CHANNEL[0].TCTRL = (PIT_TCTRL_TIE(1)|PIT_TCTRL_TEN(1));
            break;

        /*Do nothing for default*/
        default:
            break;
    }


    DB3_TURN_OFF();

}


/**********************************************************************************
* PIT0_IRQHandler()
*
* Description:  utilizes PIT0 timer for measuring 1 second time periods
*
* Return Value: none
* Arguments:    none
**********************************************************************************/
void PIT0_IRQHandler(void){
    static INT8U SineIndex = 0;     //>>>better to make these global to move the code out of the handler
    static INT16U SineWaveCnt = 0U;
    static INT8U OnOffCycle = 1U;

    DB4_TURN_ON();
    PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF(1U); /*Clear the interrupt flag*/

    /* If cycle is an on cycle write values to from sine pointer to the DAC*/
    if (OnOffCycle){
        SineIndex &= 0x3F;
        DAC0->DAT[0].DATL = (INT8U)(  (SineWave[SineIndex]) & 0xFF);
        DAC0->DAT[0].DATH = (INT8U)(( (SineWave[SineIndex]) >> 8) & 0x0F);
        SineIndex++;

        /*Else have DAC0 output 0V*/
    } else {
        DAC0->DAT[0].DATL = (DAC_HALF_SCALE_L);
        DAC0->DAT[0].DATH = (DAC_HALF_SCALE_H);
    }

    SineWaveCnt++;

    /* Once interrupt has been used 19200 times, change cycle from on to off*/
    if (SineWaveCnt == SAMPLES_PER_HS){
        SineWaveCnt = 0;
        OnOffCycle = OnOffCycle ^0x01;
    } else {}

    DB4_TURN_OFF();
}

//>>>Alarm on/off time slow. 1.54sec -1req
