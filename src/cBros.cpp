#include "cBros.hpp"
#include "GFX_cTextureRegistry.hpp"
#include "GFX_G2D_cTextureRegion.hpp"
#include "GFX_TextureUtilities.hpp"
#include "CORE_cGame.hpp"
#include "cMainGameState.hpp"
#include "cTileLevel.hpp"
#include "cDarkOne.hpp"
#include "cSoundRegistry.hpp"

using namespace GFX::G2D;
using namespace GFX;
#define FLAREWIDTH 100.0f
#define WALKSPEEDBRO 0.07f

cBros::cBros()
: cEntity()
, m_DrillRate(0.0009f)
, m_Direction(0)
, m_State(STILL)
, m_FlaresLeft(5)
, m_FlareBox(cRectf(-FLAREWIDTH, -FLAREWIDTH, FLAREWIDTH*2.0f, FLAREWIDTH*2.0f))
{
    //SetAnimFPS(2);
    m_Anims.SetTicksPerFrame(400.0f);
    m_Anims.PushAnimation(cAnimation(30.0f,
                         cTextureRegion::SplitTextureHorizontalTexNumXYWH(Art("sheet"), 4, 0, 0, 64, 64)));
    m_Anims.PushAnimation(cAnimation(30.0f,
                         cTextureRegion::SplitTextureHorizontalTexNumXYWH(Art("sheet"), 4, 0, 64, 64, 64)));
    m_Anims.PushAnimation(cAnimation(30.0f,
                         cTextureRegion::SplitTextureHorizontalTexNumXYWH(Art("sheet"), 4, 0, 128, 64, 64)));
    m_Anims.PushAnimation(cAnimation(30.0f,
                         cTextureRegion::SplitTextureHorizontalTexNumXYWH(Art("sheet"), 4, 0, 192, 64, 64)));
    m_Anims.SetTicksPerFrame(300.0f);
    m_Anims.PushAnimation(cAnimation(30.0f,
                         cTextureRegion::SplitTextureHorizontalTexNumXYWH(Art("sheet"), 4, 0, 256, 64, 64)));
    m_Anims.PushAnimation(cAnimation(30.0f,
                         cTextureRegion::SplitTextureHorizontalTexNumXYWH(Art("sheet"), 4, 0, 320, 64, 64)));

    m_Pos.y = 0.0f;
    m_BBox = cRectf(16.0f, 7.0f, 32.0f, 50.0f);
}

cBros::~cBros()
{
    //dtor
}

void cBros::Update(CORE::cGame* game, float delta, cMainGameState* state)
{
    HandleInput(game, delta, state);

    if (m_State==WALKING) {
        TryMove(game, delta, state);
    }

    m_Pos += m_Vel;
    m_Vel *= expf(-0.04*delta);

}

void cBros::TryMove(CORE::cGame* game, float delta, cMainGameState* state)
{
    int x, y;
    cTileLevel* level = state->GetLevel();

    switch (m_Direction) {
        case 0: // NORTH
            m_Vel.y = -WALKSPEEDBRO*delta;
            break;
        case 1: // EAST
            m_Vel.x = WALKSPEEDBRO*delta;
            break;
        case 2: // SOUTH
            m_Vel.y = WALKSPEEDBRO*delta;
            break;
        case 3: // WEST
            m_Vel.x = -WALKSPEEDBRO*delta;
            break;
    }

    vector<cTile*> col = level->GetCollidedTiles(GetBBoxSwept());
    for (int i=0; i<col.size(); ++i) {
        m_Pos += GetMinTranslationVectorRectRect(GetBBoxSwept(), col[i]->GetBBox());
        col[i]->SetDrilled(true);
        col[i]->DecreaseLife(m_DrillRate*delta);
        m_DrillChannel = Mix_PlayChannel(1, cSoundRegistry::drill, 0);
    }
}

void cBros::Flare(CORE::cGame* game, float delta, cMainGameState* state)
{
    m_State = FLARING;
    extern int DARKOFFSET;
    Mix_PlayChannel(1, cSoundRegistry::flare, 0);

    for (int i=DARKOFFSET; i<state->GetEntities().EntityList.size(); ++i) {
        if (state->GetEntities().EntityList[i]->GetBBox().IsCollidedRect(GetFlareBox())) {
            dynamic_cast<cDarkOne*>(state->GetEntities().EntityList[i])->Kill();
            if (dynamic_cast<cDarkOne*>(state->GetEntities().EntityList[i])->IsPlayerControlled()) {
                state->m_P2Life=false;
            }
        }
    }

    m_FlaresLeft--;
    m_State = FLARING;
    state->SetHasFlared(false);
}

void cBros::Render(CORE::cGame* game, float delta, cMainGameState* state)
{
    if (m_State==DYING) {
        m_Anims.SetCurrentIndex(5);
    } else if(m_State==FLARING) {
        m_Anims.SetCurrentIndex(4);
    } else {
        switch (m_Direction) {
            case NORTH:
                m_Anims.SetCurrentIndex(1);
                break;
            case EAST:
                m_Anims.SetCurrentIndex(3);
                break;
            case SOUTH:
                m_Anims.SetCurrentIndex(0);
                break;
            case WEST:
                m_Anims.SetCurrentIndex(2);
                break;
        }
    }
    if (m_State==DYING&&m_Anims.GetStatetime()<1500.0f) {
        const cTextureWrapper& frame
         = m_Anims[m_Anims.GetCurrentIndex()].GetKeyFrame(m_Anims.GetStatetime(), false);
         ImmediateRenderTexturePos2Dim2(frame, GetPos().x, GetPos().y, 64, 64);
    } else if (m_State==FLARING) {
        const cTextureWrapper& frame
         = m_Anims[m_Anims.GetCurrentIndex()].GetKeyFrame(m_Anims.GetStatetime(), false);
         ImmediateRenderTexturePos2Dim2(frame, GetPos().x, GetPos().y, 64, 64);
    } else if (m_State==WALKING||m_State==DRILLING) {
        const cTextureWrapper& frame
         = m_Anims.GetCurrentFrame();
         ImmediateRenderTexturePos2Dim2(frame, GetPos().x, GetPos().y, 64, 64);
    } else if (m_State==STILL) {
        const cTextureWrapper& frame
         = m_Anims[m_Anims.GetCurrentIndex()][0];
         ImmediateRenderTexturePos2Dim2(frame, GetPos().x, GetPos().y, 64, 64);
    }


    m_Anims.UpdateCurrent(delta);

}

void cBros::HandleInput(CORE::cGame* game, float delta, cMainGameState* state)
{
    if (m_State==DYING) return;
    CORE::Input& input = game->GetInput();

    if (m_State!=FLARING&&
    !( input.GetKeyState(SDLK_w)
    || input.GetKeyState(SDLK_d)
    || input.GetKeyState(SDLK_s)
    || input.GetKeyState(SDLK_a))) {
        m_State = STILL;
//        if (m_DrillChannel) Mix_HaltChannel(m_DrillChannel);
    }


    if (input.OnKeyDown(SDLK_w)) {
        m_Direction = NORTH;
        m_State = WALKING;

    }
    if (input.OnKeyDown(SDLK_d)) {
        m_Direction = EAST;
        m_State = WALKING;
    }
    if (input.OnKeyDown(SDLK_s)) {
        m_Direction = SOUTH;
        m_State = WALKING;

    }
    if (input.OnKeyDown(SDLK_a)) {
        m_Direction = WEST;
        m_State = WALKING;
    }
    if (input.OnKeyDown(SDLK_SPACE) && m_FlaresLeft>0) {
        Flare(game, delta, state);
    }

}

void cBros::Kill()
{
    if (m_State==DYING) return;
    m_DrillChannel = Mix_PlayChannel(1, cSoundRegistry::killdig, 0);
    m_State = DYING;
    m_Anims.ResetStatetime();
}
