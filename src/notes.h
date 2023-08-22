#include <Arduino.h>

#define A440 440.0

//Structure for note detection
struct note{
  double cent;
  int octave;
  String letter;
};

String notesArray[] = {"A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#"};

note getNoteByFrequency(double frequency){
  int tone = 12 * log(frequency/A440) / log(2);
  double noteFreq = A440 * powf(2, double(tone)/double(12));
  double cents = abs(frequency - noteFreq) * 100 / noteFreq;
double note2Freq = A440 * powf(2, double(tone - 1)/double(12));
  double cents2 = abs(frequency - note2Freq) * 100 / note2Freq;

  if (cents2 < cents){
    noteFreq = note2Freq;
    cents = -cents2;
    tone = tone - 1;
  }
  else{
    cents = cents;
  }

  Serial.print("Tone : ");
  Serial.println(tone);


  Serial.print("Ideal: ");
  Serial.println(noteFreq);


  Serial.print("Cents: ");
  Serial.println(cents);

  int octave = 4;

  while(tone >= 12){
    tone -= 12;
    octave++;
  }

  while(tone < 0){
    tone += 12;
    octave--;
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

