#include "CalendarOutputDriver.h"

const uint16_t CalendarOutputDriver::mDayToPixelMap[25] = {255, 13, 0, 16, 2, 9, 22, 10, 19, 8, 14, 21, 23, 5, 7,
                                                    3, 1, 17, 20, 6, 15, 11, 12, 18, 4};



void CalendarOutputDriver::turn_off_all()
{
    pixels.clear();
    pixels.show();
    while(!pixels.canShow())
    {} 
}

void CalendarOutputDriver::fade_to_red_single(uint16_t led, uint8_t s_r, uint8_t s_g, uint8_t s_b)
{
    for (uint16_t j = 0; j <= 255; j++)
    {
      pixels.setPixelColor(led, pixels.Color(s_r,s_g,s_b));
      pixels.show();
      delay(5);
      // Make sure numbers don't go negative
      if (s_r < 255)
      {
         s_r++;
      }
      if (s_g > 0)
      {
          s_g--;
      }
      if (s_b > 0)
      {
          s_b--;
      }
    }
    while(!pixels.canShow())
    {}  
}

void CalendarOutputDriver::turn_on_single(uint16_t led, uint8_t r, uint8_t g, uint8_t b)
{
    pixels.setPixelColor(led, pixels.Color(r, g, b));
    pixels.show();
    while(!pixels.canShow())
    {}  
}

void CalendarOutputDriver::flash_single(uint16_t led, uint8_t f_r, uint8_t f_g, uint8_t f_b,
                          uint8_t f_2_r, uint8_t f_2_g, uint8_t f_2_b, uint8_t a_r, uint8_t a_g, uint8_t a_b)
{
    for (int i = 0; i < 5; i++)
    {
      pixels.setPixelColor(led, pixels.Color(f_r, f_g, f_b));
      pixels.show();
      delay(50);   
      pixels.setPixelColor(led, pixels.Color(f_2_r, f_2_g, f_2_b));      
      pixels.show();
      delay(50); 
    }
    turn_on_single(led, a_r, a_g, a_b);                          
}

void CalendarOutputDriver::animation_one()
{
    // Start from beggining and move through all the lights starting with blue color
    for (int i =0 ; i < NUMPIXELS; i++)
    {
      for (uint16_t j = 0; j <= 255; j+= 25)
      {
        pixels.setPixelColor(i, pixels.Color(0,0,j));
        pixels.show();
        delay(5);
      }
  
    }

    // Flash blue slowly twice
    for (int i = 0; i < 2; i++)
    {
      for (int i =0 ; i < NUMPIXELS; i++)
      {
        pixels.setPixelColor(i, pixels.Color(0,0,255));
      }
      pixels.show();
      delay(200);

      for (int i =0 ; i < NUMPIXELS; i++)
      {
        pixels.setPixelColor(i, pixels.Color(0,0,0));
      }
      pixels.show();
      delay(200);
    }

    // Flash purple fast 4 times
    for (int i = 0; i < 4; i++)
    {
      for (int i =0 ; i < NUMPIXELS; i++)
      {
        pixels.setPixelColor(i, pixels.Color(150,0,200));
      }
      pixels.show();
      delay(100);

      for (int i =0 ; i < NUMPIXELS; i++)
      {
        pixels.setPixelColor(i, pixels.Color(0,0,0));
      }
      pixels.show();
      delay(100);
    }

    // Flash 4 really quick 6 times
    for (int i = 0; i < 6; i++)
    {
      for (int i =0 ; i < NUMPIXELS; i++)
      {
        pixels.setPixelColor(i, pixels.Color(255, 0, 0));
      }
      pixels.show();
      delay(75);

      for (int i =0 ; i < NUMPIXELS; i++)
      {
        pixels.setPixelColor(i, pixels.Color(0,0,0));
      }
      pixels.show();
      delay(75);
    }

    // Do the red train starting from the other side
    for (int i = NUMPIXELS ; i >= 0; i--)
    {
      for (uint16_t j = 0; j <= 255; j+= 25)
      {
        pixels.setPixelColor(i, pixels.Color(j,0,0));
        pixels.show();
        delay(5);
      } 
    }  
}

void CalendarOutputDriver::fade_red_into_green_all()
{
    // Slowly turn red into green
    uint16_t k = 255;
    for (uint16_t j = 0; j <= 255; j++)
    {
      for(int i=0; i < NUMPIXELS; i++)
      {
        pixels.setPixelColor(i, pixels.Color(k,j,0));      
      }
      pixels.show();
      delay(13);
      k--;
    }  
}

void CalendarOutputDriver::turn_on_all(uint8_t red, uint8_t green, uint8_t  blue)
{
    for(int i = 0; i < NUMPIXELS; i++)
    {
        pixels.setPixelColor(i, pixels.Color(red, green, blue));      
    }
    pixels.show();
    while(!pixels.canShow())
    {}  
}

void CalendarOutputDriver::split_half(uint8_t l_red, uint8_t l_green, uint8_t  l_blue, uint8_t r_red, uint8_t r_green, uint8_t  r_blue)
{
    for(int i = 0; i < NUMPIXELS/2; i++)
    {
        pixels.setPixelColor(i, pixels.Color(l_red, l_green, l_blue));      
    }
    for(int i = NUMPIXELS/2; i < NUMPIXELS; i++)
    {
        pixels.setPixelColor(i, pixels.Color(r_red, r_green, r_blue));      
    } 
    pixels.show();
    while(!pixels.canShow())
    {} 
}

void CalendarOutputDriver::turn_on_lower_right(boolean shouldShow, uint8_t r, uint8_t g, uint8_t b)
{
    pixels.setPixelColor(0, r, g, b);
    pixels.setPixelColor(1, r, g, b);
    pixels.setPixelColor(2, r, g, b);
    pixels.setPixelColor(3, r, g, b);
    if (shouldShow)
    {
      pixels.show();
      while(!pixels.canShow())
      {}
    }  
}

void CalendarOutputDriver::turn_on_middle_right(boolean shouldShow, uint8_t r, uint8_t g, uint8_t b)
{
    pixels.setPixelColor(4, r, g, b);
    pixels.setPixelColor(5, r, g, b);
    pixels.setPixelColor(6, r, g, b);
    pixels.setPixelColor(7, r, g, b);
    if (shouldShow)
    {
      pixels.show();
      while(!pixels.canShow())
      {}
    } 
}

void CalendarOutputDriver::turn_on_upper_right(boolean shouldShow, uint8_t r, uint8_t g, uint8_t b)
{
    pixels.setPixelColor(8, r, g, b);
    pixels.setPixelColor(9, r, g, b);
    pixels.setPixelColor(10, r, g, b);
    pixels.setPixelColor(11, r, g, b);
    if (shouldShow)
    {
      pixels.show();
      while(!pixels.canShow())
      {}
    }  
}

void CalendarOutputDriver::turn_on_lower_left(boolean shouldShow, uint8_t r, uint8_t g, uint8_t b)
{
    pixels.setPixelColor(20, r, g, b);
    pixels.setPixelColor(21, r, g, b);
    pixels.setPixelColor(22, r, g, b);
    pixels.setPixelColor(23, r, g, b);
    if (shouldShow)
    {
      pixels.show();
      while(!pixels.canShow())
      {}
    } 
}
        
void CalendarOutputDriver::turn_on_middle_left(boolean shouldShow, uint8_t r, uint8_t g, uint8_t b)
{
    pixels.setPixelColor(16, r, g, b);
    pixels.setPixelColor(17, r, g, b);
    pixels.setPixelColor(18, r, g, b);
    pixels.setPixelColor(19, r, g, b);
    if (shouldShow)
    {
      pixels.show();
      while(!pixels.canShow())
      {}
    }  
}

void CalendarOutputDriver::turn_on_upper_left(boolean shouldShow, uint8_t r, uint8_t g, uint8_t b)
{
    pixels.setPixelColor(12, r, g, b);
    pixels.setPixelColor(13, r, g, b);
    pixels.setPixelColor(14, r, g, b);
    pixels.setPixelColor(15, r, g, b);
    if (shouldShow)
    {
      pixels.show();
      while(!pixels.canShow())
      {}
    }
}
        
void CalendarOutputDriver::multi_color_fade_in_out()
{
    uint16_t k_16;
    uint16_t j_16;
    
    k_16 = 255;
    for (j_16 = 0; j_16 <= 255; j_16++)
    {
      for(int i = 0; i < NUMPIXELS; i++)
      {
        pixels.setPixelColor(i, pixels.Color(k_16, 0, j_16));      
      }
      pixels.show();
      delay(5);
      k_16--;
    }
  
    k_16 = 255;
    for (j_16 = 0; j_16 <= 255; j_16++)
    {
      for(int i = 0; i < NUMPIXELS; i++)
      {
        pixels.setPixelColor(i, pixels.Color(0, j_16, k_16));      
      }
      pixels.show();
      delay(5);
      k_16--;
    }  
}

void CalendarOutputDriver::happy_new_year_flash_1()
{
    turn_on_all(0, 255, 0);
    delay(75);
    turn_off_all();
    delay(75);
    turn_on_all(255, 0, 0);
    delay(75);
    turn_off_all();
    delay(75);
    turn_on_all(91,0,100);
    delay(75);
    turn_off_all();
    delay(75);
    turn_on_all(219, 225, 45);
    delay(75);
    turn_off_all();
    delay(75);
    turn_on_all(91,0,100);
    delay(150);
    turn_on_all(219, 225, 45);
    delay(150);
    turn_on_all(0, 0, 255);
    delay(150);
    turn_on_all(0, 255 , 0);
    delay(150);
    split_half(255, 0, 0, 0, 255, 0);
    delay(100);
    split_half(0, 255, 0, 255, 0, 0);
    delay(100);
    turn_on_all(255, 0 , 0);
    delay(500);  
}

void CalendarOutputDriver::we_wish_you_animation()
{
    turn_on_lower_right(true, 220, 220, 220);
    delay(300);
    turn_on_upper_right(true, 220, 220, 220);
    delay(300);
    turn_on_upper_left(true, 220, 220, 220);
    delay(300);
    turn_on_lower_left(true, 220, 220, 220);
    delay(300);
    split_half(220, 220, 220, 255, 0, 0);
    delay(400);
    turn_off_all();
    delay(50);
    split_half(255, 0, 0, 220, 220, 220);
    delay(400);
    turn_off_all();
    delay(50);
    turn_on_all(220, 220, 220);
    delay(50);
  
    turn_on_lower_right(true, 0, 255, 0);
    delay(300);
    turn_on_upper_right(true, 0, 255, 0);
    delay(300);
    turn_on_upper_left(true, 0, 255, 0);
    delay(300);
    turn_on_lower_left(true, 0, 255, 0);
    delay(300);
    split_half(220, 220, 220, 0, 255, 0);
    delay(400);
    turn_off_all();
    delay(50);
    split_half(0, 255, 0, 220, 220, 220);
    delay(400);
    turn_off_all();
    delay(50);
    turn_on_all(0, 255, 0);
    delay(50);
  
    turn_on_lower_right(true, 255, 0, 0);
    delay(300);
    turn_on_upper_right(true, 255, 0, 0);
    delay(300);
    turn_on_upper_left(true, 255, 0, 0);
    delay(300);
    turn_on_lower_left(true, 255, 0, 0);
    delay(300);
    split_half(255, 0, 0, 0, 255, 0);
    delay(400);
    turn_off_all();
    delay(50);
    split_half(0, 255, 0, 255, 0, 0);
    delay(400);
    turn_off_all();
    delay(50);
    turn_on_all(255, 0, 0);
    delay(50);
    turn_off_all();
  
    int j = NUMPIXELS;
    int i;
    for (i =0 ; i < NUMPIXELS/2; i++)
    {
      pixels.setPixelColor(i, 247, 16, 224);  
      if (i - 1 >= 0)
      {
        pixels.setPixelColor(i-1, 0, 0, 0);  
      }
      pixels.setPixelColor(j, 247, 117, 16);
      if (j + 1 < 24)
      {
        pixels.setPixelColor(j+1, 0, 0, 0);  
      }
      pixels.show();
      j--;
      delay(75);
    }
    turn_off_all();
    delay(50);
    
    for (i =0; i < 2; i++)
    {
    split_half(247, 117, 16, 247, 16, 224);
    delay(50);
    turn_off_all();
    delay(50);
    }
  
    j = NUMPIXELS;
    for (i =0 ; i < NUMPIXELS/2; i++)
    {
      pixels.setPixelColor(i, 16, 247, 63);  
      if (i - 1 >= 0)
      {
        pixels.setPixelColor(i-1, 0, 0, 0);  
      }
      pixels.setPixelColor(j, 220, 220, 220);
      if (j + 1 < 24)
      {
        pixels.setPixelColor(j+1, 0, 0, 0);  
      }
      pixels.show();
      j--;
      delay(50);
    }
    turn_off_all();
    delay(25);
    
    for (i = 0; i < 2; i++)
    {
    split_half(220, 220, 220, 16, 247, 63);
    delay(50);
    turn_off_all();
    delay(50);
    } 
}

void CalendarOutputDriver::ending_animation()
{
    // Start of the song quick fade
    uint16_t i_16 = 0;
    uint16_t k_16 = 0;
    uint16_t j_16 = 0;
    int i = 0;
    int j = 0;
    int k = 0;
  
    
    multi_color_fade_in_out();
    happy_new_year_flash_1();
    we_wish_you_animation();
    turn_off_all();
    delay(3);
    multi_color_fade_in_out();
    happy_new_year_flash_1();
    turn_off_all();
    
    // fade in lower rows
    for (j_16 = 0; j_16 <= 255; j_16 += 10)
    {
        turn_on_lower_right(false, j_16, 0, 0);
        turn_on_lower_left(false, 0 , j_16, 0);
        pixels.show();
        delay(14);
    }
  
    // Fade in middle row fade out lower
    i_16 = 255;
    for (j_16 = 0; j_16 <= 255; j_16 += 10)
    {
        turn_on_lower_right(false, i_16, 0, 0);
        turn_on_lower_left(false, 0 , i_16, 0);
        turn_on_middle_right(false, j_16, 0, 0);
        turn_on_middle_left(false, 0, j_16, 0);
        pixels.show();
        delay(14);
        i_16 -= 10;
    }
  
  
    // Fade in upper row  fade out middle row 
    i_16 = 255;
    for (j_16 = 0; j_16 <= 255; j_16 += 10)
    {
        turn_on_middle_right(false, i_16, 0, 0);
        turn_on_middle_left(false, 0 , i_16, 0);
        turn_on_upper_right(false, j_16, 0, 0);
        turn_on_upper_left(false, 0, j_16, 0);
        pixels.show();
        delay(14);
        i_16 -= 10;
    }
  
    // Fade out upper fade in middle
    i_16 = 255;
    for (j_16 = 0; j_16 <= 255; j_16 += 10)
    {
        turn_on_upper_right(false, i_16, 0, 0);
        turn_on_upper_left(false, 0 , i_16, 0);
        turn_on_middle_right(false, j_16, 0, 0);
        turn_on_middle_left(false, 0, j_16, 0);
        pixels.show();
        delay(14);
        i_16 -= 10;
    }
  
    // Fade out middle fade in lower
    i_16 = 255;
    for (j_16 = 0; j_16 <= 255; j_16 += 10)
    {
        turn_on_middle_right(false, i_16, 0, 0);
        turn_on_middle_left(false, 0 , i_16, 0);
        turn_on_lower_right(false, j_16, 0, 0);
        turn_on_lower_left(false, 0, j_16, 0);
        pixels.show();
        delay(14);
        i_16 -= 10;
    }
  
    // fade out lower row
    for (j = 255; j >= 0; j -= 10)
    {
        turn_on_lower_right(false, j, 0, 0);
        turn_on_lower_left(false, 0 , j, 0);
        pixels.show();
        delay(14);
    }
  
    /* Second run of fading */
    
    // fade in lower rows
    for (j_16 = 0; j_16 <= 255; j_16 += 10)
    {
        turn_on_lower_right(false, 0, j_16, 0);
        turn_on_lower_left(false, j_16 , 0, 0);
        pixels.show();
        delay(15);
    }
  
    // Fade in middle row fade out lower
    i_16 = 255;
    for (j_16 = 0; j_16 <= 255; j_16 += 10)
    {
        turn_on_lower_right(false, 0, i_16, 0);
        turn_on_lower_left(false, i_16 , 0, 0);
        turn_on_middle_right(false, 0, j_16, 0);
        turn_on_middle_left(false, j_16, 0, 0);
        pixels.show();
        delay(14);
        i_16 -= 10;
    }
  
  
    // Fade in upper row  fade out middle row 
    i_16 = 255;
    for (j_16 = 0; j_16 <= 255; j_16 += 10)
    {
        turn_on_middle_right(false, 0, i_16, 0);
        turn_on_middle_left(false, i_16 , 0, 0);
        turn_on_upper_right(false, 0, j_16, 0);
        turn_on_upper_left(false, j_16, 0, 0);
        pixels.show();
        delay(14);
        i_16 -= 10;
    }
  
    // Fade out upper fade in middle
    i_16 = 255;
    for (j_16 = 0; j_16 <= 255; j_16 += 10)
    {
        turn_on_upper_right(false, 0, i_16, 0);
        turn_on_upper_left(false, i_16 , 0, 0);
        turn_on_middle_right(false, 0, j_16, 0);
        turn_on_middle_left(false, j_16, 0, 0);
        pixels.show();
        delay(14);
        i_16 -= 10;
    }
  
    // Fade out middle fade in lower
    i_16 = 255;
    for (j_16 = 0; j_16 <= 255; j_16 += 10)
    {
        turn_on_middle_right(false, 0, i_16, 0);
        turn_on_middle_left(false, i_16 , 0, 0);
        turn_on_lower_right(false, 0, j_16, 0);
        turn_on_lower_left(false, j_16, 0, 0);
        pixels.show();
        delay(14);
        i_16 -= 10;
    }
  
    // fade out lower row
    for (j = 255; j >= 0; j -= 10)
    {
        turn_on_lower_right(false, 0, j, 0);
        turn_on_lower_left(false, j , 0, 0);
        pixels.show();
        delay(14);
    }
  
    // Slowly fade in all in green color
    for (i_16 = 0; i_16 <= 255; i_16 += 1)
    {
      turn_on_all(0, i_16, 0);
      delay(5);
    }
  
    // Slowly fade green into red
    j = 255;
    for (i_16 = 0; i_16 <= 255; i_16 += 1)
    {
        turn_on_all(i_16, j, 0);
        delay(6);
        j-=1;
    }
    
    happy_new_year_flash_1();
    we_wish_you_animation();
    turn_on_all(255, 0, 0);
    delay(300);
    turn_on_lower_right(true, 220, 220, 220);
    delay(300);
    turn_on_upper_right(true, 220, 220, 220);
    delay(300);
    turn_on_upper_left(true, 220, 220, 220);
    delay(300);
    turn_on_lower_left(true, 220, 220, 220);
    delay(300);
    split_half(220, 220, 220, 255, 0, 0);
    delay(400);
    turn_off_all();
    delay(50);
    split_half(255, 0, 0, 220, 220, 220);
    delay(400);
    turn_off_all();
    delay(50);
    j_16 = 0; 
    k_16 = 0;
    for (i_16 = 0; i_16 < 247; i_16 += 3)
    {
      pixels.setPixelColor(5, i_16, j_16, k_16);
      pixels.setPixelColor(3, i_16, j_16, k_16);
      pixels.setPixelColor(11, i_16, j_16, k_16);
      pixels.setPixelColor(12, i_16, j_16, k_16);
      pixels.setPixelColor(18, i_16, j_16, k_16);
      pixels.setPixelColor(20, i_16, j_16, k_16);
      pixels.show();
      delay(10);
      if (j_16 < 132)
      {
        j_16++;
      }
      if (k_16 < 16)
      {
        k_16++;
      }
    }
  
    turn_off_all();
    delay(5);
    
    // Rotate the circle
    for (i = 0; i < 3; i++)
    {
      pixels.setPixelColor(20, 0, 0 ,0);
      pixels.setPixelColor(3, 247, 132, 16);
      pixels.show();
      delay(75);
      pixels.setPixelColor(3, 0, 0, 0);
      pixels.setPixelColor(5, 247, 132, 16);
      pixels.show();
      delay(75);
      pixels.setPixelColor(5, 0, 0, 0);
      pixels.setPixelColor(11, 247, 132, 16);
      pixels.show();
      delay(75);
      pixels.setPixelColor(11, 0, 0, 0);
      pixels.setPixelColor(12, 247, 132, 16);
      pixels.show();
      delay(75);
      pixels.setPixelColor(12, 0, 0, 0);
      pixels.setPixelColor(18, 247, 132, 16);
      pixels.show();
      delay(75);
      pixels.setPixelColor(18, 0, 0, 0);
      pixels.setPixelColor(20, 247, 132, 16);
      pixels.show();
      delay(75);
    }
  
    // Fade in the yellow
    for (i_16 = 0; i_16 < 247; i_16 += 3)
    {
      turn_on_all(i_16, j_16, k_16);
      pixels.show();
      delay(5);
      if (j_16 < 132)
      {
        j_16++;
      }
      if (k_16 < 16)
      {
        k_16++;
      }
    }
  
    // Fade in the green
    for (i_16 = 0; i_16 < 255; i_16++)
    {
      turn_on_all(0, i_16, 0);
      delay(5); 
    }
  
    delay(150);
    
    turn_on_lower_right(true, 255, 0, 0);
    delay(350);
    turn_on_upper_left(true, 255, 0, 0);
    delay(350);
    turn_on_lower_left(true, 255, 0, 0);
    delay(350);
    turn_on_upper_right(true, 255, 0, 0);  
    delay(500);
    turn_on_middle_left(true, 255, 0, 0);
    delay(100);
    turn_on_middle_right(true, 255, 0, 0);
    for (i = 255; i >= 0; i--)
    {
      turn_on_all(i, 0, 0);
      delay(5);
    }
    for (i_16 = 0; i_16 <= 255; i_16++)
    {
      turn_on_all(0, i_16, 0);
      delay(5);  
    }
    
    turn_on_all(0, 255, 0);
    delay(200);
    turn_off_all();
    delay(200);
    turn_on_all(0, 255, 0);
    delay(100);
    turn_off_all();
    delay(100);
    turn_on_all(0, 255, 0);
    delay(75);
    turn_off_all();
    delay(75);
    for (i = 0; i < 10; i++)
    {
      turn_on_all(0, 255, 0);
      delay(25);
      turn_off_all();
      delay(25);
    }  
}




