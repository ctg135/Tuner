
"""
Test for micro
Script plays sound of frequency, arduino detects sound and
prints calculated frequency to Serial. Then scripts calculate difference of values
"""


import serial
import pandas as pd
import numpy
import pygame

def tone_num(tone: int):
    '''
    Function for array NOTES
    '''
    res = tone
    while res >= 12:  res -= 12
    while res < 0:  res += 12
    return res

NOTES = ["A ", "A#", "B ", "C ", "C#", "D ", "D#", "E ", "F ", "F#", "G ", "G#"]

START_TONE = -33  # Start tone to check
END_TONE = 3     # End tone to check
TIMES_REPEAT = 50 # Sample count for result
SAMPLE_RATE = 44100 # For audio playing

# Filenames to write data
FILE_NAME_DATA = "data_working.xlsx"
FILE_NAME_STAT = "stat_working.xlsx"

port = "COM3"  # Replace with the appropriate COM port name
baudrate = 115200  # Replace with the desired baud rate
ser = serial.Serial(port, baudrate=baudrate)

# Data collect variables
data = {'Note': [], 'Octave': [], 'Ideal': [], 'Hz': [], 'Diff': [], 'Cents': []}
statistics = {'Note': [], 'Octave': [], 'Ideal': [], 'Hz AVG': [], 'Diff AVG': [], 'Cents AVG': [], 'Hz MAX': [], 'Diff MAX': [], 'Cents MAX': [], 'Hz MIN': [], 'Diff MIN': [], 'Cents MIN': []}

"""
df = pd.read_excel('data.xlsx')
for octave in df['Octave'].drop_duplicates():
    for note in df['Note'].drop_duplicates():
        statistics['Note'].append(note)
        statistics['Octave'].append(octave)
        statistics['Ideal'].append(df.query('Note == @note and Octave == @octave')['Ideal'].mean())
        statistics['Hz AVG'].append(df.query('Note == @note and Octave == @octave')['Hz'].mean())
        statistics['Hz MAX'].append(df.query('Note == @note and Octave == @octave')['Hz'].max())
        statistics['Hz MIN'].append(df.query('Note == @note and Octave == @octave')['Hz'].min())
        statistics['Cents AVG'].append(df.query('Note == @note and Octave == @octave')['Cents'].mean())
        statistics['Cents MAX'].append(df.query('Note == @note and Octave == @octave')['Cents'].max())
        statistics['Cents MIN'].append(df.query('Note == @note and Octave == @octave')['Cents'].min())
        statistics['Diff AVG'].append(df.query('Note == @note and Octave == @octave')['Diff'].mean())
        statistics['Diff MAX'].append(df.query('Note == @note and Octave == @octave')['Diff'].max())
        statistics['Diff MIN'].append(df.query('Note == @note and Octave == @octave')['Diff'].min())

df_s = pd.DataFrame(data=statistics)
df_s.to_excel('stats.xlsx')
while(True): pass
"""

pygame.mixer.init(44100,-16,2,512)

print(f'Test from {NOTES[tone_num(START_TONE)]}{int((START_TONE+57)/12)} to {NOTES[tone_num(END_TONE)]}{int((END_TONE+57)/12)}')

for i in range(START_TONE, END_TONE):
    frequency =  440 * 2**(i/12)
    # Start sound
    arr = numpy.array([4096 * numpy.sin(2.0 * numpy.pi * frequency * x / SAMPLE_RATE) for x in range(0, SAMPLE_RATE)]).astype(numpy.int16)
    arr2 = numpy.c_[arr,arr]
    sound = pygame.sndarray.make_sound(arr2)
    sound.play(-1)
    # for utils
    ser.readline()
    # common values
    note = NOTES[tone_num(i)]
    octave = int((i+57)/12)
    # pogress of script
    print(' ', note, octave, end='\r')
    # Read and write TIMES_REPEAT values to table
    for v in range(TIMES_REPEAT):
        value = ser.readline().decode("utf-8")
        value_f = float(value)
        cents = (value_f - frequency)/frequency * 100
        if cents > 15 or cents < -15: continue
        data['Note'].append(note)
        data['Octave'].append(octave)
        data['Ideal'].append(frequency)
        data['Hz'].append(value_f)
        data['Diff'].append(value_f - frequency)
        data['Cents'].append(cents)
    # Stop sound
    sound.stop()

print()
# Stop serial connection
ser.close() 


df = pd.DataFrame(data=data)
df.to_excel(FILE_NAME_DATA)
print(df)
print('Stat')
# making statistics table
for octave in df['Octave'].drop_duplicates():
    for note in df['Note'].drop_duplicates():
        statistics['Note'].append(note)
        statistics['Octave'].append(octave)
        statistics['Ideal'].append(df.query('Note == @note and Octave == @octave')['Ideal'].mean())
        statistics['Hz AVG'].append(df.query('Note == @note and Octave == @octave')['Hz'].mean())
        statistics['Hz MAX'].append(df.query('Note == @note and Octave == @octave')['Hz'].max())
        statistics['Hz MIN'].append(df.query('Note == @note and Octave == @octave')['Hz'].min())
        statistics['Cents AVG'].append(df.query('Note == @note and Octave == @octave')['Cents'].mean())
        statistics['Cents MAX'].append(df.query('Note == @note and Octave == @octave')['Cents'].max())
        statistics['Cents MIN'].append(df.query('Note == @note and Octave == @octave')['Cents'].min())
        statistics['Diff AVG'].append(df.query('Note == @note and Octave == @octave')['Diff'].mean())
        statistics['Diff MAX'].append(df.query('Note == @note and Octave == @octave')['Diff'].max())
        statistics['Diff MIN'].append(df.query('Note == @note and Octave == @octave')['Diff'].min())

df_s = pd.DataFrame(data=statistics)
df_s.to_excel(FILE_NAME_STAT)
print(df_s)



