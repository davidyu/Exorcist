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
#include "cDarkOne.hpp"
#include "cDoor.hpp"
#include "cSoundRegistry.hpp"

#define WINDOW_WIDTH game->GetSDLState().window_w
#define WINDOW_HEIGHT game->GetSDLState().window_h

int DARKOFFSET = 2;
//int DOOROFFSET = 1;

//using namespace std;
using namespace GFX;
using namespace GFX::G2D;

int cMainGameState::m_LevelIndex = 0;
vector<cTileLevel*> cMainGameState::m_pLevels;

void cMainGameState::InitLevels()
{
    m_pLevels.push_back(new cTileLevel("art/level_1.png"));
    m_pLevels.push_back(new cTileLevel("art/level_2.png"));
    m_pLevels.push_back(new cTileLevel("art/level_3.png"));
    m_pLevels.push_back(new cTileLevel("art/level_4.png"));
    m_pLevels.push_back(new cTileLevel("art/level_5.png"));
    m_pLevels.push_back(new cTileLevel("art/level_6.png"));
}

void cMainGameState::ClearLevels()
{
    for (int i=0; i<m_pLevels.size(); ++i) {
        DELETESINGLE(m_pLevels[i]);
    }
    m_pLevels.clear();
}


cMainGameState::cMainGameState()
: m_batch()
, m_pMotionTex(0)
, m_pLightTex(0)
, m_HasFlared(true)
, m_pAnimStaticOverlay(0)
, m_P2Index(DARKOFFSET)
, m_Win(0)
, m_Wintime(0.0f)
, m_Quit(true)
, m_Player(0)
, m_Entrance(0)
, m_Exit(0)
, m_IsStatic(false)
, m_LightSwitch(0.0f)
, m_P2Life(true)
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
    int i;

    cout << "Entering Main Game state\n";

    MATH::cRectf vpr = MATH::cRectf(0.0f, 0.0f, 800.0f, 700.0f);
    m_Camera = new cOrthoCamera(vpr);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
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

    if (!m_Player)
    {
        m_Player = new cBros;
        m_Entities.EntityList.push_back(m_Player); //push player before push door
    }
    DARKOFFSET = m_Entities.EntityList.size();


    //DOOROFFSET = 1; //just in case!!!

    m_pLevels[m_LevelIndex]->Init(this);

    texs.push_back(cTexture("art/bg.png"));
    texs.back().RegisterGL();

    texs.push_back(cTexture("art/light.png"));
    texs.back().RegisterGL();

    m_pAnimStaticOverlay = new cAnimation(50.0f, cTextureRegion::SplitTextureHorizontalTexNumXYWH(Art("static"), 4, 0, 0, 512, 512));

    //cout << m_Entities.EntityList.size() << endl;
    //m_P2Index = DARKOFFSET;

    if (DARKOFFSET < m_Entities.EntityList.size())
    {
        dynamic_cast<cDarkOne*>(m_Entities.EntityList[m_Entities.EntityList.size()-1])->SetPlayerControl(true);
    }


    m_Quit = false;

    return true;
}

bool cMainGameState::OnExit(CORE::cGame* game)
{
    texs.clear();
    DELETESINGLE(m_pMotionTex);
    DELETESINGLE(m_pLightTex);
    DELETESINGLE(m_pAnimStaticOverlay);
    DELETESINGLE(m_Camera);

    int i;
    m_Entities.ClearEntities();
    m_Player = 0;
    m_Entrance = 0;
    m_Exit = 0;
    //delete m_Player;
    //delete m_Entrance;
    //delete m_Exit;


    cout << "Leaving Main Game state\n";

}
void cMainGameState::Pause(CORE::cGame* game) {}
void cMainGameState::Resume(CORE::cGame* game) {}


void cMainGameState::Update(CORE::cGame* game, float delta)
{
    HandleInput(game);

    if (m_Win==1) {
        m_Wintime+=delta;
    } else if (m_Win==2) {
        m_Wintime+=delta;
    }
    if (m_Wintime>= 2000.0f) {
        if (m_Win==1) {
            IncrementLevelIndex();
        }
        game->GetStateManager().ReplaceStateUsingTransition(game->state_factory.CreateObject("play")
                                                        ,   game->transition_factory.CreateObject("transFade"));
    }

    m_pLevels[m_LevelIndex]->Update(game, delta, this);

    m_Entities.EntityList[0]->Update(game, delta, this);

    if (m_Exit->GetBBox().IsCollidedRect(m_Entities.EntityList[0]->GetBBox()))
    {
        IncrementLevelIndex();
        game->GetStateManager().ReplaceStateUsingTransition(game->state_factory.CreateObject("play")
                                                        ,   game->transition_factory.CreateObject("transFade"));
    }

    if (m_Entrance->GetBBox().IsCollidedRect(m_Entities.EntityList[0]->GetBBox()))
    {
        //prev level!
    }

    m_IsStatic = false;

    if (DARKOFFSET == m_Entities.EntityList.size()) //no dark ones
        return;

    for (int i=DARKOFFSET; i<m_Entities.EntityList.size(); ++i) {
        m_Entities.EntityList[i]->Update(game, delta, this);
        if (dynamic_cast<cDarkOne*>(m_Entities.EntityList[i])->IsPlayerControlled()
         && dynamic_cast<cDarkOne*>(m_Entities.EntityList[i])->GetState()!=cDarkOne::DYING
         && m_Entities.EntityList[i]->GetBBox().IsCollidedRect(m_Entities.EntityList[0]->GetBBox())) {
            dynamic_cast<cBros*>(m_Entities.EntityList[0])->Kill();
            SetWinner(2);
        }
        if (dynamic_cast<cDarkOne*>(m_Entities.EntityList[i])->GetState()!=cDarkOne::DYING
        && m_Entities.EntityList[i]->GetBBox().IsCollidedRect(dynamic_cast<cBros*>(m_Entities.EntityList[0])->GetFlareBox())) {
            m_IsStatic = true;
            Mix_PlayChannel(-1, cSoundRegistry::stat, 0);
        }
    }
}

void cMainGameState::Render(CORE::cGame* game, float percent_tick)
{
    if (m_Quit) return;
//    glEnable(GL_ALPHA_TEST) ;

    m_pMotionTex->SetUV(0.0f, 0.0f, 1.0f, 1.0f);

    BuildLightMask(game, percent_tick);

    //camera stuff - find offset and move it there
    Vec2f diff = m_Player->GetPos() - m_Camera->GetPos();

    //offsets
    diff -= Vec2f(m_Camera->GetViewportWidth()/2, m_Camera->GetViewportHeight()/2);
    diff += Vec2f(m_Player->GetBBox().Width()/2, m_Player->GetBBox().Height()/2);

    glLoadIdentity();
    //m_Camera->LoadIdentity();
    //m_Camera->Translatef(diff.x, diff.y);
    m_Camera->TranslateTof(diff.x, diff.y); //for culling
    glTranslatef(-diff.x, -diff.y, 0);      //for actual rendering


    RenderMain(game, percent_tick);
    static float statetime = 0.0f;
    statetime += percent_tick;
    if (statetime>10000000.0f) {
        statetime = 0.0f;
    }
    if (m_IsStatic) {
        ImmediateRenderTexturePos2Dim2(m_pAnimStaticOverlay->GetKeyFrame(statetime, true)
                                 , m_Camera->GetPos().x, m_Camera->GetPos().y, WINDOW_WIDTH,WINDOW_WIDTH);
    }
    RenderLightMask(game, percent_tick);

    for (int i=0; i<m_Entities.EntityList.size(); ++i) {
        m_Entities.EntityList[i]->Render(game, percent_tick, this);
    }

    if (!m_HasFlared) {
        Flare(game, percent_tick);
    }
    const float e = expf(-11.08e-3f*percent_tick);
    glColor4f(1.0f, 1.0f, 1.0f, e);
    RenderMotionBlur(game, percent_tick);

    BuildMotionBlurFrame(game, percent_tick);
    m_pMotionTex->SetUV(0.0f, 1.0f, 1.0f, 0.0f);

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    RenderMotionBlur(game, percent_tick);
}


void cMainGameState::RenderMain(CORE::cGame* game, float percent_tick)
{
    //static cBros bro;
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
//    m_pLevels[m_LevelIndex].Render(game, percent_tick, m_batch, MATH::cRectf(0.0f, 0.0f, 800.0f, 700.0f));
    m_batch.End();

    cRectf *r = m_Camera->GetViewportRect();
    //cout << r.Left() << "," << r.Right() << endl;
    m_pLevels[m_LevelIndex]->Render(game, percent_tick, m_batch, r);


    if (m_Entrance)
        m_Entrance->Render(game, percent_tick, this);
    if (m_Exit)
        m_Exit->Render(game, percent_tick, this);

    m_Entities.EntityList[0]->Render(game, percent_tick, this);

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

    RenderFullViewportTexture(*m_pLightTex, WINDOW_WIDTH, WINDOW_HEIGHT);
    glBlendFunc(GL_SRC_ALPHA,
			GL_ONE_MINUS_SRC_ALPHA);
}
void cMainGameState::BuildLightMask(CORE::cGame* game, float percent_tick)
{
    glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glColor4f(1.0f,1.0f,1.0f,1.0f);

    m_LightSwitch += percent_tick;
    if (m_LightSwitch>30.0f) {
        ImmediateRenderTexturePos2Dim2(texs[1], DIGGER->GetPos().x+32.0f-200.0f, DIGGER->GetPos().y+0.0f-200.0f, 400.0f, 400.0f);
        m_LightSwitch=0.0f;
    }
            glBindTexture(GL_TEXTURE_2D, m_pLightTex->GetID());
    CopyBackbufferToTexture(*m_pLightTex, WINDOW_WIDTH, WINDOW_HEIGHT);
}

void cMainGameState::BuildMotionBlurFrame(CORE::cGame* game, float percent_tick)
{
    glBindTexture(GL_TEXTURE_2D, m_pMotionTex->GetID());
    CopyBackbufferToTexture(*m_pMotionTex, WINDOW_WIDTH, WINDOW_HEIGHT);
    // Copy back buffer calls glCopyTex2D which flips the texture upside down.
}

void cMainGameState::HandleInput(CORE::cGame* game)
{
    CORE::Input& input = game->GetInput();
    float    x, y;
    //input.GetJoyExtentIDWhichExtent2(0,0, x, y);

    if (input.GetKeyState(SDLK_ESCAPE)) game->EndGame();
    if (input.GetKeyState(SDLK_p)/*||(input.OnJoyButtonDown(0,3))*/) {
        SelectP2DarkOne();
    }
    if (input.OnKeyDown(SDLK_b)) {
        STATE::cGameTransition* trans = game->transition_factory.CreateObject("transSquareSpin");
        STATE::iGameState* newstate = game->state_factory.CreateObject("play");

        game->GetStateManager().ReplaceStateUsingTransition(newstate, trans);
    }
}

void cMainGameState::Flare(CORE::cGame* game, float percent_tick)
{
    const float hw = DIGGER->GetPos().x+WINDOW_WIDTH/2.0f+100.0f;
    const float hh = DIGGER->GetPos().y+WINDOW_HEIGHT/2.0f;
    const float MAXFLARETIME=1000.0f;

    m_FlareTime += percent_tick;
    glColor4f(1.0f, 1.0f, 1.0f, 1.2f-m_FlareTime/MAXFLARETIME);
    glBegin(GL_QUADS);
        glBindTexture(GL_TEXTURE_2D, 0);
        glVertex2f(hw, hh);
        glVertex2f(hw, -hh);
        glVertex2f(-hw, -hh);
        glVertex2f(-hw, hh);
    glEnd();

    if (m_FlareTime>MAXFLARETIME) {
        m_HasFlared = true;
    }
}

void cMainGameState::SelectP2DarkOne()
{
    cRectf r = *m_Camera->GetViewportRect();
    bool isSelected = false;
    int newIndex=-1;
    int count = 0;
    cRectf bbox;
    if (m_Entities.EntityList.size()==1||!m_P2Life) return;

    while (!isSelected&&count<100&&newIndex!=m_P2Index
           &&(newIndex==-1||dynamic_cast<cDarkOne*>(m_Entities.EntityList[newIndex])->cDarkOne::DYING)) {
        newIndex = MATH::RandInt(DARKOFFSET,m_Entities.EntityList.size()-1);
        bbox = m_Entities.EntityList[newIndex]->GetBBox();
//        if (m_Entities.EntityList[m_P2Index]==)
        if (r.IsCollidedRect(bbox)) {
            dynamic_cast<cDarkOne*>(m_Entities.EntityList[m_P2Index])->SetPlayerControl(false);
            dynamic_cast<cDarkOne*>(m_Entities.EntityList[newIndex])->SetPlayerControl(true);
            m_P2Index = newIndex;
            isSelected = true;
        }
        ++count;
    }
}
