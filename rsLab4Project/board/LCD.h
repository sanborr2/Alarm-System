/*****************************************************************************************
** Lcd.h
* A set of general purpose LCD utilities. This module should not be used with a preemptive
* kernel without protection of the shared LCD.
*
* Originally the 9S12 LCD module from Andrew Pace, 2/6/99, ET 454
* MOdified for the K70. Todd Morton, 2/24/2013
* Modified for the K65 Todd Morton, 11/03/2015
* Fixed LcdDispDecWord, Todd Morton, 12/13/2017
* Modified for MCUXpresso, Todd Morton, 10/29/2018
*****************************************************************************************/
#ifndef LCD_INC
#define LCD_INC
/*****************************************************************************************
* WWULCD Function prototypes
*****************************************************************************************/
/*****************************************************************************************
* LcdInit() Initializes display. Takes ~24ms to run
*****************************************************************************************/
void LcdInit(void);

/*****************************************************************************************
** LcdClrDisp
*  PARAMETERS: None
*  DESCRIPTION: Clears the LCD display and returns the cursor to row1, col1.
*****************************************************************************************/
void LcdClrDisp(void);

/*****************************************************************************************
 * LcdClrLine
*  PARAMETERS: line - Line to be cleared (1 or 2).
*  DESCRIPTION: Writes spaces to every location in a line and then returns the cursor to
*               column 1 of that line.
*****************************************************************************************/
void LcdClrLine (const INT8U line);

/*****************************************************************************************
** LcdDispChar() - Public
*  PARAMETERS: c - ASCII character to be sent to the LCD
*  DESCRIPTION: Displays a character at current LCD address. Assumes that the LCD port is
*               configured for a data write.
*****************************************************************************************/
void LcdDispChar(const INT8C c);

/*****************************************************************************************
* LcdDispByte()
*  PARAMETERS: b - byte to be displayed.
*  DESCRIPTION: Displays the byte, b, in hex.
*****************************************************************************************/
void LcdDispByte(const INT8U b);

/*****************************************************************************************
* LcdDispStrg()
*  PARAMETERS: *strg - pointer to the NULL terminated string to be displayed.
*  DESCRIPTION: Displays the string pointed to by strg.
*****************************************************************************************/
void LcdDispStrg(const INT8C *const strg);

/*****************************************************************************************
** LcdMoveCursor()
*  PARAMETERS: row - Destination row (1 or 2).
*              col - Destination column (1 - 16).
*  DESCRIPTION: Moves the cursor to [row,col].
*****************************************************************************************/
void LcdMoveCursor(const INT8U row, const INT8U col);
                                    
/*****************************************************************************************
** LcdDispDecByte()
*  PARAMETERS: b - the byte to be displayed.
*              lz - (Binary)Display leading zeros if TRUE.
*                   Delete leading zeros if FALSE.
*  DESCRIPTION: Displays the byte, b, in decimal.
*               Deletes leading zeros if lz is zero. Digits are right justified if leading
*               zeros are deleted.
*  RETURNS: None
*  Note: Not recommended for new designs. Use LcdDispDecWord().
*****************************************************************************************/
void LcdDispDecByte(const INT8U b, const INT8U lz);

/*****************************************************************************************
* LcdDispDecWord() - Outputs a decimal value of a 32-bit word.
*    Parameters: bin is the word to be sent,
*                maxlz is the maximum number of leading zeros to be shown. Range 1-10.
*    Examples:
*    One Byte = 10, maxlz = 1, Result: 10 - no leading zeros, at least one zero
*    One Byte = 10, maxlz = 3, Result: 010 - always three digits
*    One Half Word = 10, maxlz = 1, Result: 10 - no leading zeros, at least one zero
*    One Half Word = 10, maxlz = 5, Result: 00010 - always 5 digits
*****************************************************************************************/
void LcdDispDecWord(INT32U bin, INT8U maxlz);

/*****************************************************************************************
** LcdCursor(INT8U on, INT8U blink)
*  PARAMETERS: on - (Binary)Turn cursor on if TRUE, off if FALSE.
*              blink - (Binary)Cursor blinks if TRUE.
*  Changes LCD cursor state - four possible combinations.
*  RETURNS: None
*****************************************************************************************/
void LcdCursor(const INT8U on, const INT8U blink);

/*****************************************************************************************
* LcdBSpace()
*   Moves cursor back one space.
*****************************************************************************************/
void LcdBSpace(void);

/*****************************************************************************************
* LcdFSpace()
*   Moves cursor right one space.
*****************************************************************************************/
void LcdFSpace(void);

/****************************************************************************************/
#endif
