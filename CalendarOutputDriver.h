#ifndef _CALENDAROUTPUTDRIVER_H_
#define _CALENDAROUTPUTDRIVER_H_
#include "Adafruit_NeoPixel.h"
#include "Common.h"

// Helpers for the pixels functions
#define RED 255, 0, 0
#define GREEN 0, 255, 0
#define BLUE 0, 0, 255

class CalendarOutputDriver
{
    
    public:
        CalendarOutputDriver() : pixels(NUMPIXELS, PIXELS_CONTROL, NEO_GRB + NEO_KHZ800) ,pixels2(1,PIXELS_CONTROL2, NEO_GRB + NEO_KHZ800)
        {
            debugLogger.log(mDebugStr, __FUNCTION__);
            pinMode(FINAL_SONG_CONTROL, OUTPUT);
            pinMode(START_SONG_CONTROL, OUTPUT);
            pinMode(WRONG_DOOR_CONTROL, OUTPUT);
            pinMode(CORRECT_DOOR_CONTROL, OUTPUT);
            digitalWrite(FINAL_SONG_CONTROL, LOW);
            digitalWrite(START_SONG_CONTROL, LOW);
            digitalWrite(WRONG_DOOR_CONTROL, LOW);
            digitalWrite(CORRECT_DOOR_CONTROL, LOW);
        }

        int begin()
        {
            debugLogger.log(mDebugStr, __FUNCTION__);
            pixels.begin();
            digitalWrite(FINAL_SONG_CONTROL, LOW);
            digitalWrite(START_SONG_CONTROL, LOW);
            digitalWrite(WRONG_DOOR_CONTROL, LOW);
            digitalWrite(CORRECT_DOOR_CONTROL, LOW);
        }

        void trigg_correct_day(uint16_t day)
        {
            debugLogger.log(mDebugStr, __FUNCTION__);
            digitalWrite(CORRECT_DOOR_CONTROL, HIGH);
            fade_to_red_single(mDayToPixelMap[day], GREEN);
            delay(200);
            digitalWrite(CORRECT_DOOR_CONTROL, LOW);                   
        }

        void trigg_incorrect_day(uint16_t day)
        {
            debugLogger.log(mDebugStr, __FUNCTION__);
            digitalWrite(WRONG_DOOR_CONTROL, HIGH);
            flash_single(mDayToPixelMap[day], RED, GREEN, GREEN);
            delay(200);
            digitalWrite(WRONG_DOOR_CONTROL, LOW);
        }

        void trigg_start_calendar()
        {
            debugLogger.log(mDebugStr, __FUNCTION__);
            digitalWrite(START_SONG_CONTROL, HIGH);
            animation_one();
            begin_fade();
            digitalWrite(START_SONG_CONTROL, LOW);
        }

        void trigg_end_calendar()
        {
            debugLogger.log(mDebugStr, __FUNCTION__);
            digitalWrite(FINAL_SONG_CONTROL, HIGH);
            delay(5000);
            ending_animation();
            digitalWrite(FINAL_SONG_CONTROL, LOW);
        }

    private:
        const uint8_t PIXELS_CONTROL = 16;
        const uint8_t PIXELS_CONTROL2 = 7;
        const uint8_t FINAL_SONG_CONTROL = A5;
        const uint8_t START_SONG_CONTROL = A4;
        const uint8_t WRONG_DOOR_CONTROL = A2;
        const uint8_t CORRECT_DOOR_CONTROL = 15;
        const uint16_t NUMPIXELS = 24; 
        
        // This is a map of day numbers to pixel
        static const uint16_t mDayToPixelMap[25];
        const char* mDebugStr = "Calendar Output Driver: %s"; 
        Adafruit_NeoPixel pixels;
        Adafruit_NeoPixel pixels2;
        void turn_off_all();
        void fade_to_red_single(uint16_t led, uint8_t s_r, uint8_t s_g, uint8_t s_b);
        void turn_on_single(uint16_t led, uint8_t r, uint8_t g, uint8_t b);          
        void flash_single(uint16_t led, uint8_t f_r, uint8_t f_g, uint8_t f_b,
                          uint8_t f_2_r, uint8_t f_2_g, uint8_t f_2_b, uint8_t a_r, uint8_t a_g, uint8_t a_b);
        void animation_one();
        void begin_fade();
        void turn_on_all(uint8_t red, uint8_t green, uint8_t  blue);
        void split_half(uint8_t l_red, uint8_t l_green, uint8_t  l_blue, uint8_t r_red, uint8_t r_green, uint8_t  r_blue);
        void turn_on_lower_right(boolean shouldShow, uint8_t r, uint8_t g, uint8_t b);
        void turn_on_middle_right(boolean shouldShow, uint8_t r, uint8_t g, uint8_t b);
        void turn_on_upper_right(boolean shouldShow, uint8_t r, uint8_t g, uint8_t b);
        void turn_on_lower_left(boolean shouldShow, uint8_t r, uint8_t g, uint8_t b);
        void turn_on_middle_left(boolean shouldShow, uint8_t r, uint8_t g, uint8_t b);
        void turn_on_upper_left(boolean shouldShow, uint8_t r, uint8_t g, uint8_t b);
        void multi_color_fade_in_out();
        void happy_new_year_flash_1();
        void we_wish_you_animation();
        void ending_animation();

        // Main pocket pixel
        void fade_in();
        void fade_out();
               
};

#endif
