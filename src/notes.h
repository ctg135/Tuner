#include <Arduino.h>

#define A440 440.0

//Structure for note detection
struct note{
  double cent;
  int octave;
  String letter;
};

String notesArray[] = {"A ", "A#", "B ", "C ", "C#", "D ", "D#", "E ", "F ", "F#", "G ", "G#"};
int octave;
double noteFreq, cents;

note getNoteByFrequency(double frequency){
  // Calculating tone
  int tone = round(12 * log(frequency/A440) / log(2));
  // Calculating frequency of tone
  noteFreq = A440 * powf(2, tone/12.0);
  cents = (frequency-noteFreq)*100/noteFreq;

  octave = (tone+57)/12;

  while(tone >= 12){
    tone -= 12;
  }

  while(tone < 0){
    tone += 12;
  }

  note detected;
  detected.cent = cents;
  detected.letter = notesArray[tone];
  detected.octave = octave;
  return detected;
}

