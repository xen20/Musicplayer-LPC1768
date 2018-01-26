/*This class handles reading the names of files in card & passing required
parameters for playing (or stopping...) the files in the SD card.
It works in conjuction with: 
1) the wave_player class, which handles the actual output
2) the TouchScreen class, which is responsible for changing the parameters passed
to this class.
Author: Konstantin Mishin
Look .cpp file for function details*/

#ifndef MUSICPLAYER_H_
#define MUSICPLAYER_H_

#include "mbed.h"
#include <string>
#include <vector>

class MusicPlayer{
    public:
    MusicPlayer();
    virtual ~MusicPlayer();
    void player(char *directory, int *song_index_, bool *PlayStop, int *volume_);
    void read_file_names(char *dir);
    std::vector<string> filenames;
    };
    
#endif