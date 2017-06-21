/*******************************************************************************
 *
 * MIDI_lib: is midi lib
 *
 *    MIDI_lib:                      copyright 2017       Vetrov Nikolay.
 *
 *    Permission to use, copy, modify, and distribute this software in source
 *    and binary forms and its documentation for any purpose and without fee
 *    is hereby granted, provided that the above copyright notice appear
 *    in all copies and that both that copyright notice and this permission
 *    notice appear in supporting documentation.
 *
 *    THIS SOFTWARE IS PROVIDED BY THE VETROV NIKOLAY AND CONTRIBUTORS "AS IS"
 *    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *    PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL VETROV NIKOLAY OR CONTRIBUTORS BE
 *    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 *    THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/

#ifndef MIDI_H
#define	MIDI_H


#include "midi_lib_port.h"
//#include "midi_lib_config.h"


//MIDI System Common Messages & SysEx
enum{
   MIDI_SysEx_Message               = 0xf0,
   MIDI_Time_Code_Frame             = 0xf1,
   MIDI_Song_Position_Pointer       = 0xf2,
   MIDI_REAL_Song_Select            = 0xf3,
   MIDI_Tune_Request                = 0xf6,
   MIDI_SysEx_End                   = 0xf7,

 //MIDI_REAL_TIME:
   MIDI_REAL_TIME_Clock             = 0xf8,
   MIDI_REAL_TIME_Start             = 0xfa,
   MIDI_REAL_TIME_Continue          = 0xfb,
   MIDI_REAL_TIME_Stop              = 0xfc,
   MIDI_REAL_TIME_Active_Sensing    = 0xfe,
   MIDI_REAL_TIME_System_Reset      = 0xff
};

#define 	CC_OR_CHAN_MODE_THR		0x78

 //MIDI_Channel_msg
enum{
    Voice_CC_or_MIDI_Channel_Mode       = 0xb0, // second byte <=0x77 CC, >=0x78 Channel_Mode
    MIDI_Channel_Voice_Note_off         = 0x80,
    MIDI_Channel_Voice_Note_on          = 0x90,
    MIDI_Channel_Poly_Key_Pressure      = 0xA0, // Aftertouch key (poly)
  //MIDI_Channel_Mode_or_Voice_CC       = 0xb0, // second byte <0x78 CC, >=0x78 Channel_Mode
    MIDI_Channel_Voice_Program_Change   = 0xC0,
    MIDI_Channel_Pressure               = 0xD0, //Aftertouch chanel
    MIDI_Channel_Voice_Pitch_Bend       = 0xE0
};

#define		MIDI_CLOCK_PER_BEAT	24


void midi_parser_byte(uint8_t midi_byte);
    


#endif	/* MIDI_H */





