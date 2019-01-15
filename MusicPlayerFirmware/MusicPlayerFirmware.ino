#include <Adafruit_VS1053.h>
#include <SD.h>
#include "SimpleDebug.h"

#define RESET -1
#define SHIELD_CS 7
#define SHIELD_DCS 6
#define SD_CARD_CS 4
#define DREQ 3

#define SOUND_1_PIN A5
#define SOUND_2_PIN A4
#define SOUND_3_PIN A3
#define SOUND_4_PIN A1
#define DEBUG


const char* TRACK_NAMES[4] = {"track001.mp3", "track002.mp3", "track003.mp3", "track004.mp3"};

enum SwStates {OFF = 0 , ON = 1};
enum SoundTriggeredStates {NOT_TRIGERRED = 0, TRIGERRED = 1};
enum ShouldTriggerPlay {NO = 0, YES = 1};

struct SoundsSwitchStates 
{
  uint8_t soundOneLastState : 1;
  uint8_t soundTwoLastState : 1;
  uint8_t soundThreeLastState : 1;
  uint8_t soundFourLastState : 1;
  uint8_t soundOneCurrentState : 1;
  uint8_t soundTwoCurrentState : 1;
  uint8_t soundThreeCurrentState : 1;
  uint8_t soundFourCurrentState : 1;  
};

enum Sounds {TRACK_ONE, TRACK_TWO, TRACK_THREE, TRACK_FOUR, NONE};

struct SoundControl
{
  Sounds soundToBeTriggered;
  bool restartCurrentlyPlayed;
};

SoundControl soundControl;
Sounds currentlyPlaying;

SoundsSwitchStates soundsSwStates = {0};

SerialOutputDevice debugToSerial;
SimpleDebug<50> debugLogger(debugToSerial);
Adafruit_VS1053_FilePlayer musicPlayer = Adafruit_VS1053_FilePlayer(RESET, SHIELD_CS, SHIELD_DCS, DREQ, SD_CARD_CS);


void setup() {  
  #ifdef DEBUG
  Serial.begin(9600);
  debugLogger.enable();  
  #else if
  debugLogger.disable();
  #endif

  pinMode(SOUND_1_PIN, INPUT);
  pinMode(SOUND_2_PIN, INPUT);
  pinMode(SOUND_3_PIN, INPUT);
  pinMode(SOUND_4_PIN, INPUT);  
    
  // Run required checks without which we can't proceed
  if (!musicPlayer.begin())
  {
     debugLogger.log("Couldn't find VS1053, do you have the right pins defined?");
     while (1);
  }

  musicPlayer.setVolume(0, 0);
  
  debugLogger.log("VS1053 found");
  #ifdef DEBUG
  // Sound test
  musicPlayer.sineTest(0x44, 500);
  #endif
  
  if (!SD.begin(SD_CARD_CS))
  {
    debugLogger.log("SD failed, or not present");
    while (1);
  }
  
  debugLogger.log("SD OK!");
  
  if (!musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT))
  {
    debugLogger.log("DREQ pin is not an interrupt pin");
  }

  // Initialized sound control structures
  soundControl.soundToBeTriggered = NONE;
  soundControl.restartCurrentlyPlayed = false;
  currentlyPlaying = NONE;

  delay(3000);    // Wait 5seconds before checking the sounds so everything can boot up  

}

void loop() {
  read_sound_switches(soundsSwStates);
  determine_sound_to_play(soundControl, soundsSwStates, currentlyPlaying);
  play_sound(currentlyPlaying, soundControl);
  update_sound_switches_states(soundsSwStates);
  delay(200);
}


uint8_t read_debounced_pin(uint8_t pin)
{
  static uint8_t debounceTimeMs = 10;
  uint8_t firstReading = digitalRead(pin);
  delay(debounceTimeMs);
  uint8_t secondReading = digitalRead(pin);
  return ((firstReading != secondReading) ? !firstReading : firstReading);    
}

void read_sound_switches(SoundsSwitchStates& sw)
{
  sw.soundOneCurrentState = read_debounced_pin(SOUND_1_PIN);
  debugLogger.log("Pin #1 is %d\n", sw.soundOneCurrentState);
  sw.soundTwoCurrentState = read_debounced_pin(SOUND_2_PIN);
  debugLogger.log("Pin #2 is %d\n", sw.soundTwoCurrentState);
  sw.soundThreeCurrentState = read_debounced_pin(SOUND_3_PIN);
  debugLogger.log("Pin #3 is %d\n", sw.soundThreeCurrentState);
  sw.soundFourCurrentState = read_debounced_pin(SOUND_4_PIN);
  debugLogger.log("Pin #4 is %d\n", sw.soundFourCurrentState);
}

void update_sound_switches_states(SoundsSwitchStates& sw)
{
  sw.soundOneLastState = sw.soundOneCurrentState;
  sw.soundTwoLastState = sw.soundTwoCurrentState;
  sw.soundThreeLastState = sw.soundThreeCurrentState;
  sw.soundFourLastState = sw.soundFourCurrentState;
}

boolean is_sound_sw_triggered(uint8_t state_old, uint8_t state_new)
{
  boolean ret = false;
  if (state_new == ON && state_old == OFF)
  {
    ret = true;
  }
  return ret;
}

Sounds which_sound_triggered(const SoundsSwitchStates& sw)
{
  Sounds temp;
  if (is_sound_sw_triggered(sw.soundOneLastState, sw.soundOneCurrentState))
  {
    temp = TRACK_ONE;
  }
  else if (is_sound_sw_triggered(sw.soundTwoLastState, sw.soundTwoCurrentState))
  {
    temp = TRACK_TWO;
  }
  else if (is_sound_sw_triggered(sw.soundThreeLastState, sw.soundThreeCurrentState))
  {
    temp = TRACK_THREE;
  }
  else if (is_sound_sw_triggered(sw.soundFourLastState, sw.soundFourCurrentState))
  {
    temp = TRACK_FOUR;
  }
  else
  {
    temp = NONE;
  }
  debugLogger.log("Sound determined to trigger is %d\n", temp);
  return temp;
}

void determine_sound_to_play(SoundControl& control,const SoundsSwitchStates& sw, const Sounds& currentlyPlaying)
{
  // Find out which sound was triggered by change in the SW
  Sounds temp = which_sound_triggered(sw);
  control.soundToBeTriggered = temp;
  // Determine if the change happened with the sound that was already being played
  control.restartCurrentlyPlayed = (control.soundToBeTriggered == currentlyPlaying) ? true : false;   
  debugLogger.log("Determine function soundToBeTriggered: %d, restartCurrentlyPlayed: %d\n", control.soundToBeTriggered);
  debugLogger.log("Determine function restartCurrentlyPlayed: %d\n", control.restartCurrentlyPlayed);
}


void play_sound(Sounds& currentlyPlaying, const SoundControl& control)
{
  // If there is currently no sound being played reset currently playing to NONE
  if (musicPlayer.stopped())
  {
    currentlyPlaying = NONE;
  }

 // Determine if song needs to be restarted or replaced with a different one
 if ((control.soundToBeTriggered != currentlyPlaying || control.restartCurrentlyPlayed) && control.soundToBeTriggered != NONE) 
    {
      if (!musicPlayer.stopped())
      {
        musicPlayer.stopPlaying(); 
      }
      delay(100);
      bool playingFile = musicPlayer.startPlayingFile(TRACK_NAMES[control.soundToBeTriggered]);
      debugLogger.log("Return from trying to play the file %d\n", playingFile);
    }
}
