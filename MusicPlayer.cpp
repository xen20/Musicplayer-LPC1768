#include "MusicPlayer.h"
#include "SDFileSystem.h"
#include "wave_player.h"
#include "mbed.h"
#include "Inputs.h"

SDFileSystem sd(p5, p6, p7, p8, "sd"); //SD card and corresponding pins.

AnalogOut DACout(p18); //LPC 1768 analogout pin

wave_player waver(&DACout); //object that contains .wav decoding functions

MusicPlayer::MusicPlayer(){
    } //ctor

MusicPlayer::~MusicPlayer(){
    } //dtor


void MusicPlayer::player(char *directory, int *song_index_, bool *PlayStop, int *volume_)
{
    std::string songname = filenames[song_index]; //retrieve song name from vector array by index
    std::string location =  directory+songname; /*combine directory name (e.g "/sd/"), with songname,
                                                  in order to finally give the logical location of song on card*/

    FILE *wave_file; 
    wave_file=fopen(location.c_str(),"r");
    waver.play(wave_file, &player_on_off, &volume); //passes parameters to function that performs output to analog pin
    fclose(wave_file);
} 

void MusicPlayer::read_file_names(char *dir) //reads file names from SD card
{
    DIR *dp;
    struct dirent *dirp;
    dp = opendir(dir);
    //read all directory and file names in current directory into filename vector
    while((dirp = readdir(dp)) != NULL) {
        filenames.push_back(string(dirp->d_name));
    }
    closedir(dp);
}