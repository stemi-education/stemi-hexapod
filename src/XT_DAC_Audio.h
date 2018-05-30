// XTronical DAC Audio Library, currently supporting ESP32
// May work with other processors and/or DAC's with or without modifications
// (c) XTronical 2018, Licensed under GNU GPL 3.0 and later, under this license absolutely no warranty given
// See www.xtronical.com for documentation
// This software is currently under active development (Jan 2018) and may change and break your code with new versions
// No responsibility is taken for this. Stick with the version that works for you, if you need newer commands from later versions
// you will have to alter your original code to work with the new if required.



// The Main Wave class for sound samples
class XT_Wav_Class
{
  public:      
  uint16_t SampleRate;  
  volatile uint32_t DataSize=0;                         // The last integer part of count
  volatile uint32_t DataIdx=44;
  volatile unsigned char *Data;  
  volatile float IncreaseBy=0;                          // The amount to increase the counter by per call to "onTimer"
  volatile float Count=0;                               // The counter counting up, we check this to see if we need to send
  volatile uint16_t LastIntCount=0;                     // The last integer part of count
  volatile bool Completed=true;
  XT_Wav_Class(unsigned char *WavData);
};


class XT_DAC_Audio_Class
{
  // the main class for using the DAC to play sounds
  public:
  volatile uint8_t DacPin;                              // pin to send DAC data to, presumably one of the DAC pins!
  XT_Wav_Class *CurrentWav=0;                               // Wav to play
  
  XT_DAC_Audio_Class(uint8_t DacPin, uint8_t TimerNo);
  
  void PlayWav(XT_Wav_Class *Wav);
};

                                                          