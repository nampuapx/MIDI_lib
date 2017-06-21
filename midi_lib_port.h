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

#ifndef MIDI_PARSER_OUT_FUNCS_H
#define	MIDI_PARSER_OUT_FUNCS_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include "midi_lib_config.h"
//#define uint8_t unsigned char
 
 #ifdef MIDI_OUTGOING_ENABLE 
 
 
 
void put_MIDI_real_time(uint8_t real_time_command);
void put_MIDI_command_3b(uint8_t status_byte, uint8_t byte_1, uint8_t byte_2);
void put_MIDI_command_2b(uint8_t status_byte, uint8_t byte_1);



void MIDI_port_serial_byte_out(uint8_t byte);
void MIDI_port_serial_n_bytes_out(uint8_t * byte, uint8_t amount);
void put_MIDI_real_time(uint8_t real_time_command);

#endif


/////////////////////////////////////////////////////////////////////////
/////// handlers /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void MIDI_recive_clock_pulse_handler(void);
void MIDI_bpm_pulse_handler(void);
void MIDI_recive_start_handler(void);
 
void MIDI_recive_stop_handler(void);
 
 
 
void MIDI_recive_CC_handler(uint8_t chanel, uint8_t CC_num, uint8_t CC_value);
void MIDI_recive_note_on_handler(uint8_t chanel, uint8_t key_num, uint8_t key_vel);
void MIDI_recive_note_off_handler(uint8_t chanel, uint8_t key_num, uint8_t key_vel);
 
 
 
 
 
 
 
 
 
 
 
 
#ifdef	__cplusplus
}
#endif

#endif	/* MIDI_PARSER_OUT_FUNCS_H */

