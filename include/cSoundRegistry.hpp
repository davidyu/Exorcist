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

    }
    static void Destroy()
    {
        Mix_FreeChunk( drill );
        Mix_FreeChunk( beast );
        Mix_FreeChunk( swim );
        Mix_FreeChunk( gameover );
    }
    static Mix_Chunk* drill;
    static Mix_Chunk* beast;
    static Mix_Chunk* swim;
    static Mix_Chunk* gameover;
};

#endif // CSOUNDREGISTRY_H
