/****************************************************************************************
* EE344, rsLab4Project, Lab4Main
*   Program meant to run with Lab4Main to generate a sinewave to the
*   DAC32 pin on the K65FTWR while displaying messages to user via LCD
*   and Keypad peripherals on the K65TWR
*
* Robert Sanborn, 11/2/2018
*
****************************************************************************************/

#ifndef LAB4MAIN_H_
#define LAB4MAIN_H_

/* Define Constants for denoting Alarm State */

#define ALARM_OFF (INT8C)(0x14)
#define ALARM_ON  (INT8C)(0x11)

#include "K65TWR_ClkCfg.h"  //>>>these shall not be in here, not all dependences. -1bp
#include "K65TWR_GPIO.h"
#include "Key.h"
#include "LCD.h"
#include "SysTickDelay.h"

/**********************************************************************************
* Public Function Prototypes
**********************************************************************************/


/**********************************************************************************
* AlarmState    public function that returns the program state
*
* Return Value: lrmstate, the status of the program and thus the alarm
*
* Arguments:    state, the status of the program and thus the alarm
**********************************************************************************/
INT8C AlarmState(void);



#endif /* LAB4MAIN_H_ */
