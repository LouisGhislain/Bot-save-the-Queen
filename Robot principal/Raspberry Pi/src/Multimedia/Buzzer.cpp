#include "../../include/Robot.h"

// Define the buzzer pin
#define BUZZER_PIN 22  // GPIO22

// Define musical notes frequencies in Hz
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523

/*! \brief Play a note on the buzzer
*
* This function plays a note on the buzzer for a specified duration.
*
* \param frequency Frequency of the note in Hz 
* \param duration Duration to play the note in milliseconds
 */
void Robot::playNote(int frequency, int duration) {
    softToneCreate(BUZZER_PIN);
    softToneWrite(BUZZER_PIN, frequency);
    delay(duration);               // In milliseconds
    softToneWrite(BUZZER_PIN, 0);  // Stop the tone
    softToneStop(BUZZER_PIN);
}

/*! \brief Play a melody on the buzzer
*
* This function plays a simple melody on the buzzer.
*
*/
void Robot::playMelody() {
    // Example melody: Beginning of "Twinkle Twinkle Little Star"
    int notes[] = {NOTE_C4, NOTE_C4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4};
    int durations[] = {500, 500, 500, 500, 500, 500, 1000};
    int noteCount = 7;
    
    softToneCreate(BUZZER_PIN);
    
    for (int i = 0; i < noteCount; i++) {
        softToneWrite(BUZZER_PIN, notes[i]);
        delay(durations[i]);
    }
    
    // Stop the tone
    softToneWrite(BUZZER_PIN, 0);
    softToneStop(BUZZER_PIN);
}

// Original function modified to use softTone
void Robot::buzzBuzzer() {
    playNote(146.83, 10000);
}
