#include <string>

#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_mixer.h>
#include "cMainGameState.hpp"
#include "STATE_cGameTransition.hpp"
#include "CORE_cGame.hpp"
#include "GFX_cImage.hpp"
#include "GFX_cTexture.hpp"
#include "GFX_G2D_cTextureRegion.hpp"
#include "GFX_G2D_cSpritebatch.hpp"
#include "GFX_G2D_cAnimation.hpp"
#include "GFX_cTextureRegistry.hpp"
#include "GFX_TextureUtilities.hpp"
#include "GFX_MotionBlur.hpp"
#include "global_inc.hpp"

#include "cTileLevel.hpp"
#include "cBros.hpp"

#define WINDOW_WIDTH game->GetSDLState().window_w
#define WINDOW_HEIGHT game->GetSDLState().window_h


//using namespace std;
using namespace GFX;
using namespace GFX::G2D;

namespace
{
    cTexture* p_tex;
}

cMainGameState::cMainGameState()
: m_batch()
, m_pMotionTex(0)
, m_pLightTex(0)
, m_pLevel(0)
, m_pAnimStaticOverlay(0)
 {}

cMainGameState::~cMainGameState() {}

STATE::iGameState* cMainGameState::CreateInstance()
{
    return new cMainGameState;
}
STATE::iGameState* cMainGameState::Clone()
{
    return new cMainGameState;
}


//cTexture* p_tex2;
bool cMainGameState::OnEnter(CORE::cGame* game)
{
    cout << "Entering Main Game state\n";

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
//    glScalef(1.0f, -1.0f, 1.0f);
    glLoadIdentity();

    if (!m_pMotionTex) {
        m_pMotionTex = new cTextureWrapper();
        m_pMotionTex->SetBytesPerPixel(3);
        m_pMotionTex->SetTextureFormat(GL_RGB);
//        m_pMotionTex->SetUV(0.0f, 1.0f, 1.0f, 0.0f);
//        m_pMotionTex->SetUV(0.0f, 1.0f, 1.0f, 0.0f);


        CreateMotionBlurTexture(*m_pMotionTex, 512, 512, 0);
    }

    if (!m_pLightTex) {
         m_pLightTex = new cTextureWrapper();
         m_pLightTex->SetBytesPerPixel(4);
         m_pLightTex->SetTextureFormat(GL_RGBA);
         CreateMotionBlurTexture(*m_pLightTex, 256, 256, 0);
    }

    m_pLevel = new cTileLevel(100, 100);
    m_pLevel->Init();

    //note going to override Init

    texs.push_back(cTexture("art/bg.png"));
    texs.back().RegisterGL();

    texs.push_back(cTexture("art/light.png"));
    texs.back().RegisterGL();

    m_pAnimStaticOverlay = new cAnimation(50.0f, cTextureRegion::SplitTextureHorizontalTexNumXYWH(Art("static"), 4, 0, 0, 512, 512));

    cBros* bro = new cBros;
    cEntity::EntityList.push_back(bro);
    bro = 0;


    return true;
}

bool cMainGameState::OnExit(CORE::cGame* game)
{
    texs.clear();
    DELETESINGLE(m_pMotionTex);
    DELETESINGLE(m_pLightTex);
    DELETESINGLE(m_pAnimStaticOverlay);


    cout << "Leaving Main Game state\n";

}
void cMainGameState::Pause(CORE::cGame* game) {}
void cMainGameState::Resume(CORE::cGame* game) {}


void cMainGameState::Update(CORE::cGame* game, float delta)
{
    HandleInput(game);

    m_pLevel->Update(game, delta, this);

    int i;
    for (i=0; i<cEntity::EntityList.size(); ++i) {
        cEntity::EntityList[i]->Update(game, delta, this);
    }
}

float posx = 0.0f;
bool b = true;
void cMainGameState::Render(CORE::cGame* game, float percent_tick)
{
//    glEnable(GL_ALPHA_TEST) ;
    m_pMotionTex->SetUV(0.0f, 0.0f, 1.0f, 1.0f);

    BuildLightMask(game, percent_tick);

    RenderMain(game, percent_tick);
    static float statetime = 0.0f;
    statetime += percent_tick;
    if (statetime>10000000.0f) {
        statetime = 0.0f;
    }
    ImmediateRenderTexturePos2Dim2(m_pAnimStaticOverlay->GetKeyFrame(statetime, true), posx,100,700,700);
    if (b)RenderLightMask(game, percent_tick);


    const float e = expf(-35.08e-3f*percent_tick);
    glColor4f(1.0f, 1.0f, 1.0f, e);
    RenderMotionBlur(game, percent_tick);

    BuildMotionBlurFrame(game, percent_tick);

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    RenderMotionBlur(game, percent_tick);
}



void cMainGameState::RenderMain(CORE::cGame* game, float percent_tick)
{
    static cBros bro;
    /* Begin Main Drawing Procedure */
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0f,1.0f,1.0f,1.0f);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
//    glAlphaFunc(GL_GREATER, 0.95f);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA,
			GL_ONE_MINUS_SRC_ALPHA);

    m_batch.Begin();
//        m_batch.SetColor(0.2f, 0.2f, 0.2f, 1.0f);
//        m_batch.DrawTexture(texs[0], 0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT);
        m_batch.SetColor(1.0f, 1.0f, 1.0f, 1.0f);

//        m_batch.DrawTexturePos2Dim2Origin2Scale2Rot(reg, 50.0f, 0.0f, 200.0f, 100.0f, 100.0f, 50.0f, 1.0f, 1.0f, rot);
//    m_pLevel->Render(game, percent_tick, m_batch, MATH::cRectf(0.0f, 0.0f, 800.0f, 700.0f));
    m_batch.End();

    m_pLevel->Render(game, percent_tick, m_batch, MATH::cRectf(0.0f, 0.0f, 800.0f, 700.0f));

    int i;
    for (i=0; i<cEntity::EntityList.size(); ++i) {
        cEntity::EntityList[i]->Render(game, percent_tick, this);
    }

    /* End Main Drawing Procedure */

        // Draw the motion blur once at alpha


}
void cMainGameState::RenderMotionBlur(CORE::cGame* game, float percent_tick)
{
	RenderFullViewportTexture(*m_pMotionTex, WINDOW_WIDTH, WINDOW_HEIGHT);
}
void cMainGameState::RenderLightMask(CORE::cGame* game, float percent_tick)
{
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    //    glBlendFunc(GL_DST_COLOR, GL_ONE);
    glBlendFunc(GL_DST_COLOR,GL_SRC_COLOR); // 2X Multiplicative
////    glBlendFunc(GL_ZERO, GL_SRC_COLOR); // Multiplicative
//    glBlendFunc(GL_ONE, GL_ONE); // Additive -> Wrong for Particle.png
//    ImmediateRenderTexturePos2Dim2(texs[1], posx, -300.0f, 1000.0f, 1000.0f);
    RenderFullViewportTexture(*m_pLightTex, WINDOW_WIDTH, WINDOW_HEIGHT);
    glBlendFunc(GL_SRC_ALPHA,
			GL_ONE_MINUS_SRC_ALPHA);
}
void cMainGameState::BuildLightMask(CORE::cGame* game, float percent_tick)
{
    glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glColor4f(1.0f,1.0f,1.0f,1.0f);

    ImmediateRenderTexturePos2Dim2(texs[1], DIGGER->GetPos().x+32.0f-200.0f, DIGGER->GetPos().y+0.0f-200.0f, 400.0f, 400.0f);

    glBindTexture(GL_TEXTURE_2D, m_pLightTex->GetID());
    CopyBackbufferToTexture(*m_pLightTex, WINDOW_WIDTH, WINDOW_HEIGHT);
}

void cMainGameState::BuildMotionBlurFrame(CORE::cGame* game, float percent_tick)
{
    glBindTexture(GL_TEXTURE_2D, m_pMotionTex->GetID());
    CopyBackbufferToTexture(*m_pMotionTex, WINDOW_WIDTH, WINDOW_HEIGHT);
    // Copy back buffer calls glCopyTex2D which flips the texture upside down.
    m_pMotionTex->SetUV(0.0f, 1.0f, 1.0f, 0.0f);
}

void cMainGameState::HandleInput(CORE::cGame* game)
{
    CORE::Input& input = game->GetInput();
    float    x, y;
    //input.GetJoyExtentIDWhichExtent2(0,0, x, y);

    if (input.GetKeyState(SDLK_ESCAPE)) game->EndGame();
//    if (input.GetKeyState(SDLK_RIGHT)) {posx += 1.0f; }
    if (y<-0.4f) { posx -= 1.0f; }
    if (input.OnMouseButtonUp(SDL_BUTTON_LEFT)) { b=!b;}
    if (input.OnKeyDown(SDLK_b)) {
        STATE::cGameTransition* trans = game->transition_factory.CreateObject("transSquareSpin");
        STATE::iGameState* newstate = game->state_factory.CreateObject("play");

        game->GetStateManager().ReplaceStateUsingTransition(newstate, trans);
    }

}
