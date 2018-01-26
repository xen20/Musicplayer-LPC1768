#include "SeeedStudioTFTv2.h"
#include "mbed.h"
#include "Screen_functions.h"
#include "arrays.h"
#include "Arial12x12.h"
#include "Inputs.h"
#include <string>
#include <vector>

#define PIN_XP          p20
#define PIN_XM          p19
#define PIN_YP          p16
#define PIN_YM          p17
#define PIN_MOSI        p11
#define PIN_MISO        p12
#define PIN_SCLK        p13
#define PIN_CS_TFT      p14
#define PIN_DC_TFT      p21
#define PIN_BL_TFT      p15
/*pins for the touchscreen defined above*/

SeeedStudioTFTv2 TFT(PIN_XP, PIN_XM, PIN_YP, PIN_YM, PIN_MOSI, PIN_MISO,
                     PIN_SCLK, PIN_CS_TFT, PIN_DC_TFT, PIN_BL_TFT); //TFT screen object

point p; //this object is necessary to measure the touchscreen input (using p.x and p.y instances, for example)

TouchScreen::TouchScreen(){
    } //ctor

TouchScreen::~TouchScreen(){
    } //dtor

void TouchScreen::screen_initialize() /*function clears screen, defines screen parameters
                                        prints the actual menu that user interfaces with*/
{
    TFT.claim(stdout);
    TFT.set_orientation(1);
    TFT.background(Black);
    TFT.foreground(White);
    TFT.cls();
    TFT.set_font((unsigned char*) Arial12x12);
    TFT.calibrate();
    TFT.Bitmap(0,0,320,240, (unsigned char*)bg);
}

void TouchScreen::screen_interface() //performs arbitration based on touchscreen touch location
{
    if(TFT.getTouch(p)==TFT.YES) {
        TFT.getPixel(p);
        if(((p.x < 80) && (p.x > 40)) && ((p.y > 190) && (p.y < 230))) {
            player_on_off = true;
            wait_ms(500); //avoids repetetive touch by user
        } else if(((p.x < 280) && (p.x > 240)) && ((p.y > 190) && (p.y < 230))) {
            player_on_off = false;
            wait_ms(500);
        } else if(((p.x < 50) && (p.x > 20)) && ((p.y > 0) && (p.y < 25))) {
            volume -= 2;
            if(volume  < 0) {
                volume = 1;
            }
        } else if(((p.x < 80) && (p.x > 51)) && ((p.y > 0) && (p.y < 25))) {
            volume += 2;
            if(volume >= 16) {
                volume = 0;
            }
        } else if (((p.x < 305) && (p.x > 285)) && ((p.y > 65) && (p.y < 90))) {
            song_index--;
            wait_ms(500);
            if(song_index < 0) {
                song_index=0;
            }
        } else if (((p.x < 305) && (p.x > 285)) && ((p.y > 95) && (p.y < 125))) {
            song_index++;
            wait_ms(500);
            if(song_index > songnames.size()) {
                song_index=0;
            }
        }
    }
}

void TouchScreen::print_file_names(std::vector<string> songnames_) //prints song names from SD card to list on touchscreen
{
    int y_pos = 30;
    songnames = songnames_;
    for(vector<string>::iterator it=songnames.begin(); it < songnames.end(); it++) {
        y_pos+=12;
        TFT.locate(42,y_pos);
        TFT.printf("%s",(*it).c_str());
    }
}


//Serial serial(USBTX, USBRX);
/*void TouchScreen::touch()
{
    while(1) {
        p.x=0;
        p.y=0;
        if (TFT.getTouch(p)==TFT.YES) {
            TFT.getTouch(p);
            TFT.getPixel(p);
            serial.printf("\n\r Point x:%d, y:%d", p.x, p.y);
            wait_ms(500);
        }
    }
}*/
//touch function helps when debugging touchscreen - prints touched pixel to serial.