/*Title: Music player program for the LPC 1768.
  Author: Konstantin Mishin
  Date: latest revision, 19/03/17.
  Details: This program utilizes the LPC 1768 microcontroller in order
  to play .wav files using its single analog output pin18.
  Program is controlled with ILI9341-based TFT touchscreen. (mikroelektronika mikroTFT)
  Music file source is SPI-based SD card reader(sparkfun 7-pin kind).
  Output simple speaker with a nice custom amplifier.
  Look inside classes for detailed operation of functions.*/

#include "mbed.h"
#include "Screen_functions.h"
#include "MusicPlayer.h"
#include "Inputs.h"

bool player_on_off; 
int volume;
int song_index;

Ticker screenInterrupt; //object for taking inputs from screen via ticker interrupt

MusicPlayer wavPlayer; //object handles playing the actual file

TouchScreen touchScreen; //object handles screen interface

int main()
{
    touchScreen.screen_initialize();
    wavPlayer.read_file_names("/sd/");
    touchScreen.print_file_names(wavPlayer.filenames);
    screenInterrupt.attach(&touchScreen, &TouchScreen::screen_interface, 0.1);
    while(1) {
        while(player_on_off) {
            wavPlayer.player("/sd/", &song_index, &player_on_off, &volume);
        }
    }
}