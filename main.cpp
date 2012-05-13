/* Include windows.h properly on Windows */
#if defined(WIN32) || defined(_WINDOWS)
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#ifndef HELLO_MAGIC_MAIN
#  undef main
#endif


#include <iostream>
using namespace std;
#include "CORE_cGame.hpp"
#include "STATE_iGameState.hpp"

#include "cMainGameState.hpp"
#include "cMenuState.hpp"
#include "STATE_cTransSquareSpin.hpp"
#include "GFX_cTextureRegistry.hpp"

using namespace CORE;

void RegisterStates()
{
    CORE::cGame::state_factory.RegisterClass("play", cMainGameState::CreateInstance);
    CORE::cGame::state_factory.RegisterClass("menu", cMenuState::CreateInstance);
//
    cGame::transition_factory.RegisterClass("transFade", STATE::cTransSquareSpin::CreateInstance);
}

// Main function must have these params for SDL... kinda lame for encapsulation here..
int main(int argc, char* args[])
{
    RegisterStates();

    iApplication* game = new cGame();
    cMenuState* state0 = dynamic_cast<cMenuState*>(cGame::state_factory.CreateObject("menu"));
    cMenuState* state1 = dynamic_cast<cMenuState*>(cGame::state_factory.CreateObject("menu"));
    cMenuState* state2 = dynamic_cast<cMenuState*>(cGame::state_factory.CreateObject("menu"));
    STATE::iGameState* state3 = cGame::state_factory.CreateObject("play");

    if (game->Initialise()) {
        state0->SetTexture(&Art("title"));
        state1->SetTexture(&Art("int1"));
        state2->SetTexture(&Art("int2"));
        dynamic_cast<cGame*>(game)->GetStateManager().PushState(state3);
        dynamic_cast<cGame*>(game)->GetStateManager().PushState(state2);
        dynamic_cast<cGame*>(game)->GetStateManager().PushState(state1);
        dynamic_cast<cGame*>(game)->GetStateManager().PushState(state0);
        game->MainLoop();
        game->Terminate();
    }

    delete game;

    return 0;
}
