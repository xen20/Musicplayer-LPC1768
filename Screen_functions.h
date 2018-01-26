/*This class performs calibartion of touchscreen, prints the actual interface, and
performs arbitration (decision) based on user input.
The variables modified by this class are then used by the MusicPlayer class in order
to play, stop, increase or decrease volume of played file.
Author: Konstantin Mishin
Look .cpp file for function details*/

#ifndef SCREEN_FUNCTIONS_H_
#define SCREEN_FUNCTIONS_H_

#include <string>
#include <vector>

class TouchScreen {
    public:
    TouchScreen();
    virtual ~TouchScreen();
    void screen_initialize();
    void screen_interface();
    //void touch();
    void print_file_names(std::vector<string> songnames_);
    private:
    std::vector<string> songnames;
    };

#endif