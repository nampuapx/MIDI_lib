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

#include "midi_lib_port.h"


 
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

 //MIDI_Channel_msg
enum{
    Voice_CC_or_MIDI_Channel_Mode       = 0xb0, // second byte <=0x77 CC, >=0x78 Channel_Mode
    MIDI_Channel_Voice_Note_off         = 0x80,
    MIDI_Channel_Voice_Note_on          = 0x90,
    MIDI_Channel_Poly_Key_Pressure      = 0xA0, // Aftertouch key (poly)
  //MIDI_Channel_Mode_or_Voice_CC       = 0xb0, // second byte <=0x77 CC, >=0x78 Channel_Mode   
    MIDI_Channel_Voice_Program_Change   = 0xC0,     
    MIDI_Channel_Pressure               = 0xD0, //Aftertouch chanel
    MIDI_Channel_Voice_Pitch_Bend       = 0xE0
};


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
        
	if(midi_byte & 0x80){
		//status incoming
			if(midi_byte >= MIDI_REAL_TIME_Clock){
			//System Real-Time Messages /////////////////////////////////////////
					switch(midi_byte){
						case MIDI_REAL_TIME_Clock:
							MIDI_recive_clock_pulse_handler();     //midi clock

							bpm_divider %=24;
							if(!bpm_divider){
								MIDI_bpm_pulse_handler();        //bpm pulse (24 MIDI Clock per 1 bpm step)
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
				data_count = 0;

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
		// Data Bytes icoming

			data_count %= command_length;
			data[data_count] = midi_byte;
			data_count++;

			if(data_count == command_length){
				//mesg_done

				switch(RunningStatus & 0xf0) {
					case Voice_CC_or_MIDI_Channel_Mode:
						if(data[0]<0x78){
						// CC
							MIDI_recive_CC_handler(RunningStatus & 0x0f,data[0],data[1]);
						}else{
						// MIDI Channel Mode Messages
						}
						break;

					case MIDI_Channel_Voice_Note_on:
						MIDI_recive_note_on_handler(RunningStatus & 0x0f,data[0],data[1]);
						break;
					case MIDI_Channel_Voice_Note_off:
						MIDI_recive_note_off_handler(RunningStatus & 0x0f,data[0],data[1]);
						break;
	
				}//switch
			}//if
		}//else
}
#endif
