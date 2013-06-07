//******************************************************************************
//
// SCD@iai.heig-vd 04.12.07
// LCD.c
//
// cible : MSP430FG4617149 sur carte de laboratoire uC MCN
// compilé sous : IAR embedded workbench v3.42A kickstart
//
// BUT:
//  gestion LCD
//
//******************************************************************************

extern void LCD_clear(void);
extern void LCD_init(void);
extern void LCD_setCursorPos(unsigned char n);
extern unsigned char LCD_getCursorPos(void);
extern void LCD_print(char* str);
extern void LCD_dot(char pos);