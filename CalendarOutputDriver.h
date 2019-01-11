#ifndef _CALENDAROUTPUTDRIVER_H_
#define _CALENDAROUTPUTDRIVER_H_
#include "Adafruit_NeoPixel.h"
#include "Common.h"

#define PIXELS_CONTROL 27
#define FINAL_SONG_CONTROL A3
#define START_SONG_CONTROL A3
#define WRONG_DOOR_CONTROL A2
#define CORRECT_DOOR_CONTROL A1

#define NUMPIXELS 24

// Helpers for the pixels functions
#define RED 255, 0, 0
#define GREEN 0, 255, 0
#define BLUE 0, 0, 255

class CalendarOutputDriver
{
    
    public:
        CalendarOutputDriver() : pixels(NUMPIXELS, PIXELS_CONTROL, NEO_GRB + NEO_KHZ800)
        {
            debugLogger.log(mDebugStr, __FUNCTION__);
            pinMode(FINAL_SONG_CONTROL, OUTPUT);
            pinMode(START_SONG_CONTROL, OUTPUT);
            pinMode(WRONG_DOOR_CONTROL, OUTPUT);
            pinMode(CORRECT_DOOR_CONTROL, OUTPUT);
        }

        int begin()
        {
            debugLogger.log(mDebugStr, __FUNCTION__);
            pixels.begin();
        }

        void trigg_correct_day(uint16_t day)
        {
            debugLogger.log(mDebugStr, __FUNCTION__);
            digitalWrite(CORRECT_DOOR_CONTROL, HIGH);
            fade_to_red_single(mDayToPixelMap[day], GREEN);
            digitalWrite(CORRECT_DOOR_CONTROL, LOW);                   
        }

        void trigg_incorrect_day(uint16_t day)
        {
            debugLogger.log(mDebugStr, __FUNCTION__);
            digitalWrite(WRONG_DOOR_CONTROL, HIGH);
            flash_single(mDayToPixelMap[day], RED, GREEN, GREEN);
            digitalWrite(WRONG_DOOR_CONTROL, LOW);
        }

        void trigg_start_calendar()
        {
            debugLogger.log(mDebugStr, __FUNCTION__);
            digitalWrite(START_SONG_CONTROL, HIGH);
            animation_one();
            fade_red_into_green_all();
            digitalWrite(START_SONG_CONTROL, LOW);
        }

        void trigg_end_calendar()
        {
            debugLogger.log(mDebugStr, __FUNCTION__);
            digitalWrite(FINAL_SONG_CONTROL, HIGH);
            ending_animation();
            digitalWrite(FINAL_SONG_CONTROL, LOW);
        }

    private:
        // This is a map of day numbers to pixels
        static const uint16_t mDayToPixelMap[25] = {255, 13, 0, 16, 2, 9, 22, 10, 19, 8, 14, 21, 23, 5, 7,
                                                    3, 1, 17, 20, 6, 15, 11, 12, 18, 4};
        static const char *mDebugStr = "Calendar Output Driver: %s";
        Adafruit_NeoPixel pixels;
        void turn_off_all();
        void fade_to_red_single(uint16_t led, uint8_t s_r, uint8_t s_g, uint8_t s_b);
        void turn_on_single(uint16_t led, uint8_t r, uint8_t g, uint8_t b);          
        void flash_single(uint16_t led, uint8_t f_r, uint8_t f_g, uint8_t f_b,
                          uint8_t f_2_r, uint8_t f_2_g, uint8_t f_2_b, uint8_t a_r, uint8_t a_g, uint8_t a_b);
        void animation_one();
        void fade_red_into_green_all();
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
               
};

#endif
