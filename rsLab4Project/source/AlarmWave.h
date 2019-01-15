/****************************************************************************************
* EE344, rsLab4Project, AlarmWave
*   Program generates a sinewave to the DAC32 for the K65FTWR
*
* Robert Sanborn, 11/2/2018
*
****************************************************************************************/
#ifndef ALARMWAVE_H_
#define ALARMWAVE_H_

/**********************************************************************************
* Public Function Prototypes
**********************************************************************************/

/**********************************************************************************
* AlarmWaveControlTask  public function that controls the PIT and DAC
*                       to configure and control the alarm waveform timing
*                       based on the AlarmState
*
* Return Value:         none
* Arguments:            none
**********************************************************************************/
void AlarmWaveControlTask(void);


/**********************************************************************************
* AlarmWaveInit         public function initializes resources for Alarm Wave
*
* Return Value:         none
* Arguments:            none
**********************************************************************************/
void AlarmWaveInit(void);




#endif /* ALARMWAVE_H_ */
