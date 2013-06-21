//******************************************************************************
// SCD@iai.heig-vd 04.12.07
// USB.c
//
// cible : MSP430FG4617149 sur carte de laboratoire uC MCN
// compil� sous : IAR embedded workbench v3.42A kickstart
//
// BUT: fonctions lecture/�criture par USB
//******************************************************************************

#include "msp430fg4617.h"

unsigned char strlen(char* str)
{
  unsigned char n = -1;
  do
  {
    n++;
  }while(str[n] != 0);
  return n;
}

// renvoie vrai si l'USB est connect�
unsigned char USB_isConnected(void)
{ return (!(P5IN & BIT7));
}

// renvoie vrai si on peut lire un byte sur l'USB
unsigned char USB_readyToRead(void)
{ return (!(P5IN & BIT6));
}

// *****************************************************************
// lecture d'un byte
unsigned char USB_read(void)
{ unsigned char a, b;

  // met IOs en entr�es
  P1DIR &= ~(BIT4|BIT5|BIT6|BIT7);
  P3DIR &= ~(BIT4|BIT5|BIT6|BIT7);

  P5OUT &= ~BIT0; //baisse RD#
  a = P1IN;       //lecture...
  b = P3IN;
  P5OUT |= BIT0;  //rel�ve RD#
  return((b&0xF0) | ((a>>4)&0x0F));
}

// *****************************************************************
// renvoie vrai si on peut �crire un byte � destination du PC
unsigned char USB_readyToWrite(void)
{ return (!(P5IN & BIT5));
}

// ****************************************************************
//�criture d'un byte � vers le PC
void USB_write(unsigned char n)
{ unsigned char a;

  P5OUT |= BIT1; //l�ve WR

  // met IOs en sorties
  P1DIR |= (BIT4|BIT5|BIT6|BIT7);
  P3DIR |= (BIT4|BIT5|BIT6|BIT7);

  // �criture
  a = P1OUT & 0x0F;     // �criture 4 LSBs sur P1.4-7
  P1OUT = a | (n<<4); 
  a = P3OUT & 0x0F;     // �criture 4 MSBs sur P3.4-7
  P3OUT = a | (n&0xF0); 

  P5OUT &= ~BIT1; // rebaisse WR

  //met IOs en entr�es
  P1DIR &= ~(BIT4|BIT5|BIT6|BIT7);
  P3DIR &= ~(BIT4|BIT5|BIT6|BIT7);
}

// initialise signaux de controle pour communication avec FTDI
// (RD# sur P5.0, WR sur P5.1, TXE# sur P5.5, RXF# sur P5.6, PWREN sur P5.7)
void USB_init(void)
{
  //fixe niveau des signaux
  P5OUT |=  BIT0; //met signal RD# � '1'
  P5OUT &= ~BIT1; //met signal WR � '0'

  //configure IOs
  P5SEL &= ~(BIT0|BIT1|BIT5|BIT6|BIT7);
  P5DIR |= (BIT0|BIT1);
  P5DIR &= ~(BIT5|BIT6|BIT7);
  P1SEL &= ~(BIT4|BIT5|BIT6|BIT7);
  P3SEL &= ~(BIT4|BIT5|BIT6|BIT7);

  //vide buffer de r�ception
  while (USB_readyToRead())
    USB_read();
}

// **************************************************************
// �quivalente � la function standard puts

int usb_puts (char *a)
{ int j,le; 
  le = strlen(a);
  for (j=0 ; j<strlen(a) ; j++)
     USB_write(a[j]);
  USB_write(13); 
  return (le);
}  

// **************************************************************
// �quivalente � la function standard gets

char *usb_gets (char *a)
{ unsigned char c;
  int i = 0;
  
  if (USB_readyToRead()) 
  {    
    do 
    { if (USB_readyToRead()) 
      { c = USB_read();
        a[i] = c;
        i++;
      }
    } while (c != 13);
    USB_write(13);
    a[i-1] = 0;
    return a;
  }
  else return 0;
}

