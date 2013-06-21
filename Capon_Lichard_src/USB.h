// BUT:  fonctions lecture/�criture par USB
//******************************************************************************

#ifndef USB_H
#define USB_H

// renvoie vrai si l'USB est connect�
extern unsigned char USB_isConnected(void);

// renvoie vrai si on peut lire un byte sur l'USB
extern unsigned char USB_readyToRead(void);

// lecture d'un byte
extern unsigned char USB_read(void);

// renvoie vrai si on peut �crire un byte � destination du PC
extern unsigned char USB_readyToWrite(void);

// �criture d'un byte � vers le PC
extern void USB_write(unsigned char n);

// initialise signaux de controle pour communication avec FTDI
// (RD# sur P5.0, WR sur P5.1, TXE# sur P5.5, RXF# sur P5.6, PWREN sur P5.7)
extern void USB_init(void);

// lecture d'une chaine de caract�res de la console USB 
// se r�f�rer � la fonction C gets
extern char *usb_gets (char *a);

// affichage d'une chaine de caract�res sur la console USB
// se r�f�rer � la fonction C puts
extern int usb_puts (char *a);

#endif