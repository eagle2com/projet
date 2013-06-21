 #include "msp430fg4617.h"

void LCD_init(void);
void LCD_clear(void);
unsigned char cursorPos;

// ATTENTION, il est possible que les variables globales ne soient pas
// initialisées lors l'utilisation d'un package C par un prg assembleur.
// Les constantes, elles, le sont.

// correspondance caractères-segments pour les 128 premiers caractères ASCII
// car 0-31  et 127 pas imprimables... => enlevés
//° = ar ascii 248
const unsigned short int chr2seg[95] = { \
/*  0 0,0,0,0,0,0,0,0,\*/
/*  8 0,0,0,0,0,0,0,0,\*/
/* 16 0,0,0,0,0,0,0,0,\*/
/* 24 0,0,0,0,0,0,0,0,\*/
/* 32*/ 0,0,0,0,0,0,0,0,\
/* 40*/ 0,0,0,0,0x8000,0x0240,0x8000,0,\
/* 48*/ 0x700E,0x6000,0x324C,0x7248,0x6242,0x524A,0x524E,0x7000,\
/* 56*/ 0x724E,0x724A,0,0,0,0x0248,0,0,\
/* 64*/ 0,0x7246,0x7858,0x100E,0x7818,0x120E,0x1206,0x504E,\
/* 72*/ 0x6246,0x1818,0x6008,0x02A6,0x000E,0x6126,0x6186,0x700E,\
/* 80*/ 0x3246,0x708E,0x32C6,0x524A,0x1810,0x600E,0x0426/*0x6180*/,0x6486,\
/* 88*/ 0x05A0,0x0520,0x1428,0,0,0,0,0,\
/* 96*/ 0,0,0,0,0,0,0,0,\
/*104*/ 0,0,0,0,0,0,0,0,\
/*112*/ 0,0,0,0,0,0,0,0,\
/*120*/ 0,0,0,0,0,0,0,\
};

void LCD_init(void)
{
  LCDACTL = LCDFREQ_32 + LCD4MUX + LCDON;

  LCDAPCTL0 = 0xFE; // active tous segments sauf S0-S3
  LCDAPCTL1 = 0x01; // active semgments S32-S35

  LCDAVCTL0 = LCDCPEN; // enable charge pump pour création des niveaux
  LCDAVCTL1 = VLCD_8; //3.02V

  P5SEL |= (BIT2|BIT3|BIT4);  // initialisation des pins COM1-COM3
  P5DIR |= (BIT2|BIT3|BIT4);  
  LCD_clear();
}

//******************************************************************************
void LCD_clear(void)   //efface tous les segments
{
  unsigned char n;
  for (n=0 ; n<20 ; n++)
    *((unsigned char *)(&LCDM1+n)) = 0x0;
  cursorPos = 1;
}

//******************************************************************************
void LCD_setCursorPos(unsigned char n)
{
  if (n>8)
    n=8;
  if(n<1)
    n=1;
  cursorPos = n;
}

//******************************************************************************
unsigned char LCD_getCursorPos(void)
{
  return(cursorPos);
}

//******************************************************************************

void LCD_print(const char* str)
{
  unsigned char length, i, regNr;

  // détermine la longueur de la chaîne
  length=0;
  while(str[length] != 0)
    length++;

  // allume segments
  for (i=0 ; i<length ; i++)
    if(i<=8)
    {
      regNr = 2*(cursorPos + i);
      *((unsigned char *)(&LCDM1+regNr)) = chr2seg[str[i]-32] & 0x00FF;
      *((unsigned char *)(&LCDM1+regNr+1)) = (chr2seg[str[i]-32]>>8) & 0x00FF;
    }
}

//******************************************************************************

/* LCD_dot(pos)
* - adds a dot on the pos-th LCD cell
*/
void LCD_dot(char pos)
{
  *((unsigned char *)(&LCDM1+2*pos)) |= (0x8000 & 0x00FF);
  *((unsigned char *)(&LCDM1+2*pos+1)) |= ((0x8000>>8) & 0x00FF);
}