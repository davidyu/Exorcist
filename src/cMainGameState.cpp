#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_mixer.h>
#include "cMainGameState.hpp"
#include "STATE_cGameTransition.hpp"
#include "CORE_cGame.hpp"
#include "GFX_cImage.hpp"
#include "GFX_cTexture.hpp"
#include "GFX_G2D_cTextureRegion.hpp"
#include "GFX_G2D_cSpriteBatch.hpp"
#include "GFX_G2D_cAnimation.hpp"
#include "GFX_cTextureRegistry.hpp"
#include "GFX_TextureUtilities.hpp"
#include "GFX_MotionBlur.hpp"
#include "global_inc.hpp"
    #include <iostream>
    using namespace std;

#define WINDOW_WIDTH game->GetSDLState().window_w
#define WINDOW_HEIGHT game->GetSDLState().window_h


using namespace GFX;
using namespace GFX::G2D;

namespace
{
    cTexture* p_tex;
}

cMainGameState::cMainGameState()
: m_pMotionTex(0)
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
    cout << "Entering State\n";
//    if (!p_tex) {
//        p_tex = new cTexture("art/tilez.png");
//        p_tex->RegisterGL();
//    }
    if (!m_pMotionTex) {
        m_pMotionTex = new cTextureWrapper();
        m_pMotionTex->SetBytesPerPixel(3);
        m_pMotionTex->SetTextureFormat(GL_RGB);
        m_pMotionTex->SetUV(0.0f, 1.0f, 1.0f, 0.0f);

        CreateMotionBlurTexture(*m_pMotionTex, 512, 512, 0);
    }

    texs.push_back(cTexture("art/bg.png"));
    texs.back().RegisterGL();

    texs.push_back(cTexture("art/Particle.png"));
    texs.back().RegisterGL();

    test = false;


    return true;
}
bool cMainGameState::OnExit(CORE::cGame* game)
{
    texs.clear();
    DELETESINGLE(p_tex);
    cout << "Leaving state\n";

}
void cMainGameState::Pause(CORE::cGame* game) {}
void cMainGameState::Resume(CORE::cGame* game) {}


void cMainGameState::Update(CORE::cGame* game, float delta)
{
    HandleInput(game);
}

float posx = 0.0f;
void cMainGameState::Render(CORE::cGame* game, float percent_tick)
{
    static G2D::cSpriteBatch batch = G2D::cSpriteBatch();

    static float rot = 0.0f;


    rot += percent_tick*0.1f;
    rot = fmod(rot, 360.0f);

    const int width = m_pMotionTex->GetTextureWidth();
    const int height = m_pMotionTex->GetTextureHeight();

    glEnable(GL_ALPHA_TEST) ;
    glAlphaFunc(GL_GREATER, 0.1f);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA,
			GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
//    glViewport(0, 0, width, height);
    /* Begin Main Drawing Procedure */

    batch.Begin();
//        batch.SetColor(0.2f, 0.2f, 0.2f, 1.0f);
        batch.DrawTexture(texs[0], 0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT);
        batch.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
//        batch.DrawTexturePos2Dim2Origin2Scale2Rot(reg, 50.0f, 0.0f, 200.0f, 100.0f, 100.0f, 50.0f, 1.0f, 1.0f, rot);
    batch.End();

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
//    glBlendFunc(GL_DST_COLOR, GL_ONE);
    glBlendFunc(GL_DST_COLOR,GL_SRC_COLOR); // 2X Multiplicative
////    glBlendFunc(GL_ZERO, GL_SRC_COLOR); // Multiplicative
//    glBlendFunc(GL_ONE, GL_ONE); // Additive -> Wrong for Particle.png

//    glBlendFunc(GL_DST_COLOR, GL_ZERO); // Additive -> Wrong for Particle.png
//    glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
////    glBlendEquation(GL_MAX);
    ImmediateRenderTexturePos2Dim2(texs[1], posx, -300.0f, 1000.0f, 1000.0f);

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
//    ImmediateRenderTexturePos2Dim2(texs[0], posx, -300.0f, 1000.0f, 1000.0f);


    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

//    ImmediateRenderTexturePos2Dim2Origin2Scale2Rot(*m_pMotionTex, 0.0f, 0.0f, 640.0f, 480.0f, 320.0f, 240.0f, 1.0f, -1.0f, 0.0f);
//

    /* End Main Drawing Procedure */

    glColor4f(1.0f, 1.0f, 1.0f, expf(-5.08e-3f*percent_tick));//MATH::clampf(exp2f(-9.0f/percent_tick), 0.0f, 0.999f));
//    RenderFullViewportTexture(*m_pMotionTex, width, height);
//    CopyBackbufferToTexture(*m_pMotionTex, width, height);
    RenderFullViewportTexture(*m_pMotionTex, WINDOW_WIDTH, WINDOW_HEIGHT);
    CopyBackbufferToTexture(*m_pMotionTex, WINDOW_WIDTH, WINDOW_HEIGHT);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// put the texture on the screen
	RenderFullViewportTexture(*m_pMotionTex, WINDOW_WIDTH, WINDOW_HEIGHT);


}

void cMainGameState::HandleInput(CORE::cGame* game)
{
    CORE::Input& input = game->GetInput();

    if (input.GetKeyState(SDLK_ESCAPE)) game->EndGame();
    if (input.GetKeyState(SDLK_RIGHT)) {posx += 1.0f; }
    if (input.OnKeyDown(SDLK_a)) { test = !test;}
    if (input.OnMouseButtonDown(SDL_BUTTON_LEFT)) { test = !test;}
    if (input.OnKeyDown(SDLK_b)) {
        STATE::cGameTransition* trans = game->transition_factory.CreateObject("transSquareSpin");
        STATE::iGameState* newstate = game->state_factory.CreateObject("play");

        game->GetStateManager().ReplaceStateUsingTransition(newstate, trans);
    }
    float x, y;
}
