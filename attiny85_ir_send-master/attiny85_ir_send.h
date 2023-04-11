/*
 * IRremote
 * Version 0.11 August, 2009
 * Copyright 2009 Ken Shirriff
 * For details, see http://arcfn.com/2009/08/multi-protocol-infrared-remote-library.html
 *
 * Interrupt code based on NECIRrcv by Joe Knapp
 * http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1210243556
 * Also influenced by http://zovirl.com/2008/11/12/building-a-universal-remote-with-an-arduino/
 *
 * Modified by Kristian Lauszus to work with ATTiny85
 * For details, see http://blog.tkjelectronics.dk
 * Attiny85 adaptation by Nick Gammon http://arduino.cc/forum/index.php?topic=91488.msg687101#msg687101)
 * Spilt into two lib by philteta@gmail.com www.tetalab.org (IRTinyTX-lib-v1)
 *
 *Modified by Anor Neto send all types of ir codes with attiny
 * January 20015  https://github.com/anorneto/attiny85_ir_send
 */

#ifndef ATTTINY_85_IR_SEND_H
#define ATTTINY_85_IR_SEND_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define IRLED PB1 // (Used as OC0B) - Physical Pin 6 (Digital 1) on ATtiny85

// Enssures the compiler knows the cpu frequency, and uses 8Mhz by default 
#ifndef F_CPU
//#define F_CPU 16000000UL  // 16 MHz
#define F_CPU 8000000UL  // 8 MHz
#endif


#define TOPBIT 0x80000000

// Pulse parms are *50-100 for the Mark and *50+100 for the space
// First MARK is the one after the long gap
// pulse parameters in usec
#define NEC_HDR_MARK	9000
#define NEC_HDR_SPACE	4500
#define NEC_BIT_MARK	560
#define NEC_ONE_SPACE	1600
#define NEC_ZERO_SPACE	560

#define SONY_HDR_MARK	2400
#define SONY_HDR_SPACE	600
#define SONY_ONE_MARK	1200
#define SONY_ZERO_MARK	600

#define RC5_T1		889
#define RC5_RPT_LENGTH	46000

#define RC6_HDR_MARK  2666
#define RC6_HDR_SPACE 889
#define RC6_T1    444

#define SAMSUNG_HDR_MARK  5000
#define SAMSUNG_HDR_SPACE 5000
#define SAMSUNG_BIT_MARK  560
#define SAMSUNG_ONE_SPACE 1600
#define SAMSUNG_ZERO_SPACE  560

#define SHARP_BIT_MARK 245
#define SHARP_ONE_SPACE 1805
#define SHARP_ZERO_SPACE 795
#define SHARP_TOGGLE_MASK 0x3FF

#define PANASONIC_HDR_MARK 3502
#define PANASONIC_HDR_SPACE 1750
#define PANASONIC_BIT_MARK 502
#define PANASONIC_ONE_SPACE 1244
#define PANASONIC_ZERO_SPACE 400

#define JVC_HDR_MARK 8000
#define JVC_HDR_SPACE 4000
#define JVC_BIT_MARK 600
#define JVC_ONE_SPACE 1600
#define JVC_ZERO_SPACE 550

class IRsend
{
public:
  IRsend() {}
  void sendRaw(unsigned int buf[], int len, int hz);
  void sendNEC(unsigned long data, int nbits);
  void sendSony(unsigned long data, int nbits);
  void sendRC5(unsigned long data, int nbits);
  void sendRC6(unsigned long data, int nbits);
  void sendSAMSUNG(unsigned long data, int nbits);
  void sendSharp(unsigned int address, unsigned int command);
  void sendSharpRaw(unsigned long data, int nbits);
  void sendPanasonic(unsigned int address, unsigned long data);
  void sendJVC(unsigned long data, int nbits, int repeat); 
  /* *Note instead of sending the REPEAT constant if you want the JVC repeat signal sent, 
  send the original code value and change the repeat argument from 0 to 1. 
  JVC protocol repeats by skipping the header NOT by sending a separate code value like NEC does. */
  void enableIROut(uint8_t khz);
private:
  void mark(int16_t usec);
  void space(int16_t usec);
};

#endif
