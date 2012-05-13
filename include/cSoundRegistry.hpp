#ifndef CSOUNDREGISTRY_H
#define CSOUNDREGISTRY_H

#include <SDL2/SDL_mixer.h>

class cSoundRegistry
{
    public:
    static void Init()
    {
        drill = Mix_LoadWAV("sound/drilling.wav");
        drill->volume = 50;

        killdig = Mix_LoadWAV("sound/digdeath.wav");
        killdig->volume = 50;

        flare = Mix_LoadWAV("sound/flare.wav");
        flare->volume = 50;

        killit = Mix_LoadWAV("sound/itdeath.wav");
        killit->volume = 50;

        stat = Mix_LoadWAV("sound/static.wav");
        stat->volume = 8;

    }
    static void Destroy()
    {
        Mix_FreeChunk( drill );
        Mix_FreeChunk( killdig );
        Mix_FreeChunk( flare );
        Mix_FreeChunk( killit );
        Mix_FreeChunk( stat );
    }
    static Mix_Chunk* drill;
    static Mix_Chunk* killdig;
    static Mix_Chunk* flare;
    static Mix_Chunk* killit;
    static Mix_Chunk* stat;
};

#endif // CSOUNDREGISTRY_H
