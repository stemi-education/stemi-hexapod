// XTronical DAC Audio Library, currently supporting ESP32
// May work with other processors and/or DAC's with or without modifications
// (c) XTronical 2018, Licensed under GNU GPL 3.0 and later, under this license absolutely no warranty given
// See www.xtronical.com for documentation
// This software is currently under active development (Jan 2018) and may change and break your code with new versions
// No responsibility is taken for this. Stick with the version that works for you, if you need newer commands from later versions
// you will have to alter your original code


#include "esp32-hal-timer.h";
#include "XT_DAC_Audio.h";

// The following array "uint32_t cp0_regs[18];" of register values is used to store the current state of the 
// Floating Point Unit (FPU) on the ESP32. If we try to use a float type inside an interrupt using an ESP32
// then is crashes the CPU. We can use doubles and then all floating point numbers are calculated by the CPU 
// using software, but this is time consuming compared to using hardware floating point. The crashing is not 
// a hardware issue but just one of the compiler, it's been known about for some time but is awaiting a fix. 
// In the meantime we sort the FPU ourselves within the interupt routine. See the interupt routine code.
// This solution was found at http://bbs.esp32.com/viewtopic.php?t=1292

uint32_t cp0_regs[18];

// because of the difficulty in passing parameters to interrupt handlers we have a global
// object of this type that points to the object the user creates.
XT_DAC_Audio_Class *XTDacAudioClassGlobalObject;       

// interrupt stuff
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;


// The main interrupt routine called 50,000 times per second
void IRAM_ATTR onTimer() 
{  
  uint16_t IntPartOfCount;

 // get FPU state, we need to do this until the issue of using floats inside an interupt is fixed
  uint32_t cp_state = xthal_get_cpenable();
  
  if(cp_state) {
    // Save FPU registers
    xthal_save_cp0(cp0_regs);
  } else {
    // enable FPU
    xthal_set_cpenable(1);
  }
  
  if(XTDacAudioClassGlobalObject->CurrentWav!=0)
  {
    if(XTDacAudioClassGlobalObject->CurrentWav->Completed==false)
    {      
	  // increase the counter, if it goes to a new integer digit then write to DAC
      XTDacAudioClassGlobalObject->CurrentWav->Count+=XTDacAudioClassGlobalObject->CurrentWav->IncreaseBy;                                 
      IntPartOfCount=floor(XTDacAudioClassGlobalObject->CurrentWav->Count);
      if(IntPartOfCount>XTDacAudioClassGlobalObject->CurrentWav->LastIntCount)
      {
        // gone to a new integer of count, we need to send a new value to the DAC
        XTDacAudioClassGlobalObject->CurrentWav->LastIntCount=IntPartOfCount;           // update the last int count to this new one
        dacWrite(XTDacAudioClassGlobalObject->DacPin,XTDacAudioClassGlobalObject->CurrentWav->Data[XTDacAudioClassGlobalObject->CurrentWav->DataIdx]);             												// write out the data
        XTDacAudioClassGlobalObject->CurrentWav->DataIdx++;
        if(XTDacAudioClassGlobalObject->CurrentWav->DataIdx>=XTDacAudioClassGlobalObject->CurrentWav->DataSize)  // end of data, flag end
          XTDacAudioClassGlobalObject->CurrentWav->Completed=true;  
      }  
    }
  }

  // return fpu to previous state
   if(cp_state) {
    // Restore FPU registers
    xthal_restore_cp0(cp0_regs);
  } else {
    // turn it back off
    xthal_set_cpenable(0);
  }
}
                                                        
XT_DAC_Audio_Class::XT_DAC_Audio_Class(uint8_t DacPin, uint8_t TimerNo)
{
  // Using a prescaler of 80 gives a counting frequency of 1,000,000 (1MHz) and using
  // and calling the function every 20 counts of the freqency (the 20 below) means
  // that we will call our onTimer function 50,000 times a second

  timer = timerBegin(TimerNo, 80, true);                // use timer 0, prescaler is 80 (divide by 8000), count up
  timerAttachInterrupt(timer, &onTimer, true);          // P3= edge trggered
  timerAlarmWrite(timer, 20, true);                     // will trigger 50,000 times per second, so 50kHz is max freq.
  timerAlarmEnable(timer);                              // enable
  XTDacAudioClassGlobalObject=this;						// set variable to use in ISR
  this->DacPin=DacPin;
  dacWrite(DacPin,0x7f);								// Set speaker to mid point to stop any clicks during sample playback
}



void XT_DAC_Audio_Class::PlayWav(XT_Wav_Class *Wav)
{
  Wav->Count=0;
  Wav->LastIntCount=0;
  Wav->DataIdx=44;
  Wav->Completed=false; 
  CurrentWav=Wav;
}



XT_Wav_Class::XT_Wav_Class(unsigned char *WavData)
{
  // create a new wav class object
  
  SampleRate=(WavData[25]*256)+WavData[24];  
  DataSize=(WavData[42]*65536)+(WavData[41]*256)+WavData[40]+44;
  IncreaseBy=float(SampleRate)/50000;
  Data=WavData;
  Count=0;
  LastIntCount=0;
  DataIdx=44;
  Completed=true;
}


 


