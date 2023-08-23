#include <Arduino.h>

#define A440 440.0

//Structure for note detection
struct note{
  double cent;
  int octave;
  String letter;
};

String notesArray[] = {"A ", "A#", "B ", "C ", "C#", "D ", "D#", "E ", "F ", "F#", "G ", "G#"};

note getNoteByFrequency(double frequency){
  int tone = round(12 * log(frequency/A440) / log(2));
  double noteFreq = A440 * powf(2, double(tone)/double(12));
  double cents = (frequency-noteFreq)*100/noteFreq;

  // if (frequency <= 220) cents /= 2;
  // if (frequency <= 110) cents /= 2;

  Serial.print("Tone : ");
  Serial.println(tone);


  Serial.print("Ideal: ");
  Serial.println(noteFreq);


  Serial.print("Cents: ");
  Serial.println(cents);

  int octave = (tone+57)/12;

  while(tone >= 12){
    tone -= 12;
    // octave++;
  }

  while(tone < 0){
    tone += 12;
    // octave--;
  }

  Serial.print("Letter: ");
  Serial.print(notesArray[tone]);
  Serial.println(octave);

  note detected;
  detected.cent = cents;
  detected.letter = notesArray[tone];
  detected.octave = octave;
  return detected;
}

