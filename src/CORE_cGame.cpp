#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "CORE_cGame.hpp"
#include "STATE_iGameState.hpp"

#include "GFX_cTextureRegistry.hpp"
#include "cSoundRegistry.hpp"
#include "cMainGameState.hpp"


/*temp*/ #include <iostream>
         #include <cassert>
        using namespace std;

using namespace CORE;
using namespace STATE;

cGenericFactory<STATE::iGameState> cGame::state_factory;
cGenericFactory<STATE::cGameTransition> cGame::transition_factory;

cGame::cGame()
: m_running(true)
, m_state_manager(this)
, m_sdl_state()
, m_timer()
{
}

cGame::~cGame()
{
    //dtor
}
#include <stdlib.h>
bool cGame::Initialise()
{
    if (!SetupSDL()) {
        return false;
    }
    if (!SetupGL()) {
        return false;
    }

    m_running = true;
    m_input.Initialise();
    GFX::cTextureRegistry::Init();
    cSoundRegistry::Init();
    cMainGameState::InitLevels();


    return true;
}

bool cGame::SetupSDL()
{
    if (!SDL_Init( SDL_INIT_EVERYTHING )){
        cout << "SOMETHING BAD HAPPENED IN SDL_INIT\n";
        return false;
    }
    // Setup SDL Window and Render
    m_sdl_state = new cSDLState();
    m_sdl_state->SetGL();
    m_sdl_state->window = SDL_CreateWindow(m_sdl_state->window_title,
        m_sdl_state->window_x,
        m_sdl_state->window_y,
        m_sdl_state->window_w , m_sdl_state->window_h,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if (!m_sdl_state->window) {
        cout << "Window creation failed\n";
        Terminate();
        return false;
    }

    // Fullscreen?
    SDL_SetWindowFullscreen(m_sdl_state->window, m_sdl_state->is_fullscreen);
    // Renderer
    m_sdl_state->renderer = SDL_CreateRenderer(m_sdl_state->window,
                                    0, m_sdl_state->render_flags);
    if (!m_sdl_state->renderer) {
        cout << "Renderer creation failed\n";
        Terminate();
        return false;
    }

    // GL Context
    m_sdl_state->glctx = SDL_GL_CreateContext(m_sdl_state->window);
    if (!m_sdl_state->glctx) {
        cout << SDL_GetError();
//        cout << "GL Context creation failed\n";
        Terminate();
        return false;
    }

    SDL_GL_MakeCurrent(m_sdl_state->window, m_sdl_state->glctx);

    SDL_GL_SetSwapInterval(1); // 1 for Vsync?

    if (!IMG_Init( IMG_INIT_PNG )){
        printf("IMG_Init: %s\n", IMG_GetError());
        return false;
    }

    if( Mix_OpenAudio( MIX_DEFAULT_FREQUENCY, AUDIO_S16SYS, 2, 4096 ) == -1 ) {
        fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
        return false;
    }
    //    if (!Mix_Init()){
//        printf("Mix_Init: %s\n", Mix_GetError());
//        return false;
//    }

    return true;
}
bool cGame::SetupGL()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, GetSDLState().window_w, GetSDLState().window_h, 0, -10.0, 10.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_TEXTURE_2D);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);          // Really Nice Perspective

    return true;
}

bool cGame::Terminate()
{
    if (m_sdl_state) {

        if (m_sdl_state->renderer) {
            SDL_DestroyRenderer(m_sdl_state->renderer);
        }
        if (m_sdl_state->glctx)    {
            SDL_GL_DeleteContext(m_sdl_state->glctx);
        }
        if (m_sdl_state->window)   {
            SDL_DestroyWindow(m_sdl_state->window);
        }
        SDL_free(m_sdl_state);
    }

    m_state_manager.ClearAll();
    m_input.Terminate();

    SDL_Quit();
    IMG_Quit();
    Mix_Quit();

    return true;
}

void cGame::MainLoop()
{
    iGameState* state = 0;
    float delta = 0.0f, percent_tick = 0.0f; // Dummy vars for now; substitute timer values in later

    m_timer.Start();

    while (m_running)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    EndGame();
                    break;
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                case SDL_MOUSEMOTION:
                case SDL_MOUSEBUTTONUP:
                case SDL_MOUSEBUTTONDOWN:
                case SDL_JOYAXISMOTION:
                case SDL_JOYBUTTONDOWN:
                case SDL_JOYBUTTONUP:
                    m_input.HandleEvent(event);
                    break;
                default:
                    break;
            }
        }

        // Game Loop

        // Update
        // Update Input-- set old keystates and current ones
        delta = static_cast<float>(m_timer.GetTicksDelta());
//        std::cout << delta << std::endl;
        m_input.UpdateAll();
        state = m_state_manager.GetCurrent();
        state->Update(this, delta);

        // Render Sequence
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        SDL_GL_MakeCurrent(m_sdl_state->window, m_sdl_state->glctx);
        state = m_state_manager.GetCurrent();
        state->Render(this, delta);

        SDL_GL_SwapWindow(m_sdl_state->window);
    }
    cMainGameState::ClearLevels();
    cSoundRegistry::Destroy();
}

void cGame::EndGame()
{   m_running = false; }

Input& cGame::GetInput()
{    return m_input; }

cGameStateManager& cGame::GetStateManager()
{    return m_state_manager;    }

CORE::cTimer& cGame::GetTimer()
{    return m_timer; }
