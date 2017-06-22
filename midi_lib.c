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

#include "midi_lib.h"





#define		STATUS_BYTE_MASK	 	0x80
#define		CHANNEL_NUM_MASK		0x0f
#define		STATUS_MASK				0xf0



uint8_t RunningStatus=0 ,bpm_divider=0;
uint8_t  data[2];



void put_MIDI_CC(uint8_t chanel, uint8_t CC_num, uint8_t CC_value){

  put_MIDI_command_3b((Voice_CC_or_MIDI_Channel_Mode |(chanel & 0x0f)),CC_num,CC_value);
}

void put_MIDI_note_off(uint8_t chanel, uint8_t note_num, uint8_t note_vel){

 put_MIDI_command_3b((MIDI_Channel_Voice_Note_off |(chanel & 0x0f)),note_num,note_vel);
}

void put_MIDI_note_on(uint8_t chanel, uint8_t note_num, uint8_t note_vel){

 put_MIDI_command_3b((MIDI_Channel_Voice_Note_on |(chanel & 0x0f)),note_num,note_vel);
}




void put_MIDI_clock(void){

    put_MIDI_real_time(MIDI_REAL_TIME_Clock);
}
void put_MIDI_start(void){

    put_MIDI_real_time(MIDI_REAL_TIME_Start);
}
void put_MIDI_stop(void){

    put_MIDI_real_time(MIDI_REAL_TIME_Stop);
}

///////////////////////////////////////////////////////////////////////////////
#ifdef MIDI_INPUT_ENABLE
void midi_parser_byte(uint8_t midi_byte){

static uint8_t  data_count,command_length;
        
	if(midi_byte & STATUS_BYTE_MASK){
		//status incoming
			if(midi_byte >= MIDI_REAL_TIME_Clock){
			//System Real-Time Messages /////////////////////////////////////////
					switch(midi_byte){
						case MIDI_REAL_TIME_Clock:
							MIDI_recive_clock_pulse_handler();     //midi clock

							bpm_divider %= MIDI_CLOCK_PER_BEAT;
							if(!bpm_divider){
								MIDI_bpm_pulse_handler();
							}
							bpm_divider++;
							break;

						case MIDI_REAL_TIME_Start:
							MIDI_recive_start_handler();           //midi_start
							break;

						case MIDI_REAL_TIME_Stop:
							MIDI_recive_stop_handler();            //midi_stop
							break;

						default:
							break;
					}
			///End of real time//////////////////////////////////////////////////
			}else{
				//not real time
				RunningStatus = midi_byte;
				data_count = 0; //new Running Status, data pointer reset

				switch(RunningStatus){
					case MIDI_Channel_Voice_Program_Change:
					case MIDI_Channel_Pressure:
						command_length = 1;
						break;

					default:
						command_length = 2;
						break;
				}
			}

		}else{
		// Data Bytes incoming

			data_count %= command_length;
			data[data_count] = midi_byte;
			data_count++;

			if(data_count == command_length){
				//mesg_done

				switch(RunningStatus & STATUS_MASK) {
					case Voice_CC_or_MIDI_Channel_Mode:
						if(data[0] < CC_OR_CHAN_MODE_THR){
						// CC
							MIDI_recive_CC_handler(RunningStatus & CHANNEL_NUM_MASK, data[0], data[1]);
						}else{
						// MIDI Channel Mode Messages
						}
						break;

					case MIDI_Channel_Voice_Note_on:
						MIDI_recive_note_on_handler(RunningStatus & CHANNEL_NUM_MASK, data[0], data[1]);
						break;
					case MIDI_Channel_Voice_Note_off:
						MIDI_recive_note_off_handler(RunningStatus & CHANNEL_NUM_MASK, data[0], data[1]);
						break;
	
				}//switch
			}//if
		}//else
}
#endif
