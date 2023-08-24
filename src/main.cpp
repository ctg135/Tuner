#include <Arduino.h>

#include "notes.h"
note current;

#include <arduinoFFT.h>
 
#define SAMPLES 128             //SAMPLES-pt FFT. Must be a base 2 number. Max 128 for Arduino Uno.
#define SAMPLING_FREQUENCY 2048 //Ts = Based on Nyquist, must be 2 times the highest expected frequency.
#define REPEATS 4

arduinoFFT FFT = arduinoFFT();

unsigned int samplingPeriod;
unsigned long microSeconds;

double vReal[SAMPLES]; //create vector of size SAMPLES to hold real values
double vImag[SAMPLES]; //create vector of size SAMPLES to hold imaginary values

void recordSamples(); //function for recording samples


#include <U8glib-HAL.h>

U8GLIB_SSD1306_128X64 u8g(13, 11, 10, 9, 8);

void display();

void setup() {
  Serial.begin(115200); //Baud rate for the Serial Monitor
  samplingPeriod = round(1000000*(1.0/SAMPLING_FREQUENCY)); //Period in microseconds 
}

double sum=0;
int count=0;
int count_sample;
double frequncy_summ, peak_fr;

void loop() {
  frequncy_summ = 0;
  for (int count_sample = 0; count_sample < REPEATS; count_sample++){
    recordSamples();
    /*Perform FFT on samples*/
    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);

    /*Find peak frequency and print peak*/
    peak_fr = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);

    frequncy_summ += peak_fr;
  }
  peak_fr = frequncy_summ/REPEATS;

  // Filter for frequency
  // It`s maded for specipfic microphone, that overcharges frequency
  if (peak_fr > 250) peak_fr = peak_fr*0.98; 
  else if (peak_fr < 250 && peak_fr > 215) peak_fr = peak_fr*99.65/100;
  else if (peak_fr < 215 && peak_fr > 210) peak_fr = peak_fr*0.995;
  else if (peak_fr < 210 && peak_fr > 160) peak_fr = peak_fr*0.985;
  else if (peak_fr < 160 && peak_fr > 125) peak_fr = peak_fr*0.988;
  else if (peak_fr < 125 && peak_fr > 105) peak_fr = peak_fr*0.979;
  else if (peak_fr < 105 && peak_fr > 93) peak_fr = peak_fr*0.99;
  else if (peak_fr < 93 && peak_fr > 85) peak_fr = peak_fr*0.98;
  else if (peak_fr < 85 && peak_fr > 80) peak_fr = peak_fr*1000.11/1000;
  else if (peak_fr < 80 && peak_fr > 65) peak_fr = peak_fr*0.98;

  // Getting note with frequency
  current = getNoteByFrequency(peak_fr);
  Serial.println(peak_fr);     //Print out the most dominant frequency.

  // Display on monitor
  display(); 
}

void recordSamples(){
  /*Sample SAMPLES times*/
  for(int i=0; i<SAMPLES; i++)
  {
    microSeconds = micros();    //Returns the number of microseconds since the Arduino board began running the current script. 
  
    vReal[i] = analogRead(0); //Reads the value from analog pin 0 (A0), quantize it and save it as a real term.
    vImag[i] = 0; //Makes imaginary term 0 always

    /*remaining wait time between samples if necessary*/
    while(micros() < (microSeconds + samplingPeriod))
    {
      //do nothing
    }
  }
}

void display(){
  
  String out = "";
  out += current.letter;
  out += current.octave;

  int pos = (current.cent / 0.1 + 30) * 2 + 4;
  u8g.firstPage();
  do
  {
    u8g.setFont(u8g_font_unifont);
    u8g.setPrintPos(30, 28);
    u8g.print(out.c_str());
    u8g.drawLine(4, 40, 124, 40); // Main line
    u8g.drawLine(57, 34, 57, 46); // Left border
    u8g.drawLine(71, 34, 71, 46); // Right border
    u8g.drawLine(64, 31, 64, 34); // Center Line up
    u8g.drawLine(64, 45, 64, 48); // Center Line down
    u8g.drawBox(pos, 36, 2, 8);
  } while( u8g.nextPage() );
}