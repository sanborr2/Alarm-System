/*****************************************************************************
* EE344, rsLab4Project, Lab4Main
*   Program generates a sinewave to an audio output while
*   displaying messages to user via LCD and Keypad peripherals on the
*   K65TWR
*
* Robert Sanborn, 10/30/2018
*
*****************************************************************************/
#include "MCUType.h"            /*    Include project header file           */
#include "Lab4Main.h"
#include "AlarmWave.h"

/* Define Constants for denoting state of Keyboard input*/

#define A_KEY     (INT8C)(0x11)
#define D_KEY     (INT8C)(0x14)

#define ZERO_ADDR  0x00000000U
#define HIGH_ADDR  0x001FFFFFU

#define ROW_1               1U
#define ROW_2               2U
#define COL_1               1U
#define COL_2               2U
#define COL_3               3U

#define CURSOR_ON           1U
#define CURSOR_BLINK        1U

/**********************************************************************************
* Private Function Prototypes
**********************************************************************************/

/**********************************************************************************
* ControlDisplayTask    public function that controls the state of the alarm
*                       based on keypad inputs and display to the LCD display.
*
* Return Value:         none
* Arguments:            none
**********************************************************************************/
static void ControlDisplayTask(void);

/**********************************************************************************
* CalcChkSum() -ascertains checksum for memory block indicated by starting and
*               ending addresses
*
* Description:  Loads every byte from starting address to ending address
*               and takes the first 16 bits of the sum of all said bytes.
*               Note that it if starting address is greater than ending address
*               function will only return byte stored at ending address.
*
* Return Value: c_sum, 16 bit unsigned integer, the first 16 bits of the sum
*               of all bytes between the two addresses
*
* Arguments:    startaddr is pointer to the initial address of the memory block
*               endaddr is the pointer to the last address of the memory block
**********************************************************************************/
static INT16U CalcChkSum(INT8U *startaddr, INT8U *endaddr);


/**********************************************************************************
* Private Globals
**********************************************************************************/
static INT8C AlarmStatus;


/**********************************************************************************
* Private Strings
**********************************************************************************/
static const INT8C AlarmOff[] = {"ALARM_OFF"};
static const INT8C AlarmOn[]  = {"ALARM_ON" };


/**********************************************************************************
* Program
**********************************************************************************/

void main(void){
    INT16U chksum = CalcChkSum( ((INT8U *)ZERO_ADDR), ((INT8U *)HIGH_ADDR) );   //>>>Slow before BootClk -1bp

    /*Initialize various resources needed for this module*/
    K65TWR_BootClock();
    KeyInit();
    LcdInit();
    (void)SysTickDlyInit();
    AlarmWaveInit();

    GpioDBugBitsInit();

    LcdCursor(CURSOR_ON, CURSOR_BLINK);

    /* Output check sum to LCD in the form XXXX, in hexadecimal*/
    LcdMoveCursor(ROW_2, COL_1);
    LcdDispByte((INT8U)(chksum >> 8));
    LcdDispByte((INT8U)(chksum & 0xFFFF));

    AlarmStatus = D_KEY; /* Initialize AlarmStatus to Alarm Off*/
    LcdMoveCursor(ROW_1, COL_1);
    LcdDispStrg(AlarmOff); /* Output message to LCD */

    /* Timeslice Scheduler of 10ms */
    while (1U){
        SysTickWaitEvent(10);
        KeyTask();
        ControlDisplayTask();
        AlarmWaveControlTask();
    }
}


/**********************************************************************************
* CalcChkSum() -ascertains checksum for memory block indicated by starting and
*               ending addresses
*
* Description:  Loads every byte from starting address to ending address
*               and takes the first 16 bits of the sum of all said bytes.
*               Note that it if starting address is greater than ending address
*               function will only return byte stored at ending address.
*
* Return Value: c_sum, 16 bit unsigned integer, the first 16 bits of the sum
*               of all bytes between the two addresses
*
* Arguments:    startaddr is pointer to the initial address of the memory block
*               endaddr is the pointer to the last address of the memory block
**********************************************************************************/
static INT16U CalcChkSum(INT8U *startaddr, INT8U *endaddr){
    INT16U c_sum = 0;
    INT8U membyte;
    INT8U *addr = startaddr;

    /*Iterate over all memory locations from start address to address
     * right before ending address and sum up all bytes*/
    while(addr < endaddr){
        membyte = *addr;
        addr++;
        c_sum += (INT16U)membyte;
    }
    c_sum += ( (INT16U)(*endaddr) );
    return c_sum;
}


/**********************************************************************************
* AlarmState    public function that returns the program state
*
* Return Value: status, the status of the program and thus the alarm
*
* Arguments:    none
**********************************************************************************/
INT8C AlarmState(void){ //>>>name should describe an action, status is the 'thing'
    INT8C status = AlarmStatus;
    return status;
}


/**********************************************************************************
* ControlDisplayTask    public function that controls the state of the alarm
*                       based on keypad inputs and display to the LCD display.
*                       If KeyGet returns the value for the D key on the keypad,
*                       the LCD will display ALARM OFF. If KeyGet returns
*                       the value for the A key on the keypad, the LCD will
*                       display ALARM ON.
*
* Return Value:         none
* Arguments:            none
**********************************************************************************/
static void ControlDisplayTask(void){
    INT8C keyvalue;

    /*Critical Zone*/
    __disable_irq();    //>>>this can cause missed events -1req

    DB1_TURN_ON();

    keyvalue= KeyGet();
    /*Do not alter display unless valid key press detected*/

    /*If KeyGet returns the value for the D key, display ALARM OFF.*/
    if ((keyvalue == D_KEY) && (keyvalue != AlarmStatus) ){
        LcdMoveCursor(ROW_1, COL_1);
        LcdClrLine(ROW_1);
        LcdDispStrg(AlarmOff);
        AlarmStatus = ALARM_OFF;

    /*If KeyGet returns the value for the A key, display ALARM ON.*/
    }else if ((keyvalue == A_KEY) && (keyvalue != AlarmStatus)){
        LcdMoveCursor(ROW_1, COL_1);
        LcdClrLine(ROW_1);
        LcdDispStrg(AlarmOn);
        AlarmStatus = ALARM_ON;

    } else{}

    DB1_TURN_OFF();

    __enable_irq();
}

