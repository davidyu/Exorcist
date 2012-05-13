#include "cBros.hpp"
#include "GFX_cTextureRegistry.hpp"
#include "GFX_G2D_cTextureRegion.hpp"
#include "GFX_TextureUtilities.hpp"
#include "CORE_cGame.hpp"
#include "cMainGameState.hpp"
#include "cTileLevel.hpp"

using namespace GFX::G2D;

cBros::cBros()
: cEntity()
, m_Anims()
, m_DrillRate(0.3f)
, m_Direction(0)
, m_State(STILL)
{
    m_Anims.SetTicksPerFrame(300.0f);
    m_Anims.PushAnimation(cAnimation(30.0f,
                         cTextureRegion::SplitTextureHorizontalTexNumXYWH(Art("sheet"), 4, 0, 0, 64, 64)));
    m_Anims.PushAnimation(cAnimation(30.0f,
                         cTextureRegion::SplitTextureHorizontalTexNumXYWH(Art("sheet"), 4, 0, 64, 64, 64)));
    m_Anims.PushAnimation(cAnimation(30.0f,
                         cTextureRegion::SplitTextureHorizontalTexNumXYWH(Art("sheet"), 4, 0, 128, 64, 64)));
    m_Anims.PushAnimation(cAnimation(30.0f,
                         cTextureRegion::SplitTextureHorizontalTexNumXYWH(Art("sheet"), 4, 0, 192, 64, 64)));

    m_Pos.y = 200.0f;
    m_BBox = cRectf(0.0f, 0.0f, 40.0f, 40.0f);
}

cBros::~cBros()
{
    //dtor
}

void cBros::Update(CORE::cGame* game, float delta, cMainGameState* state)
{
    HandleInput(game, delta);

    if (m_State==WALKING) {
        TryMove(game, delta, state);
    }

    m_Pos += m_Vel;
    m_Vel *= expf(-0.02*delta);

}

void cBros::TryMove(CORE::cGame* game, float delta, cMainGameState* state)
{
    int x, y;
    cTileLevel* level = state->GetLevel();
    cTile* tile = level->GetTileClosestToPos(m_Pos, x, y);
    cTile* collideTile = 0;

    switch (m_Direction) {
        case 0: // NORTH
            m_Vel.y = -0.1f*delta;
            break;
        case 1: // EAST
            m_Vel.x = 0.1f*delta;
            break;
        case 2: // SOUTH
            m_Vel.y = 0.1f*delta;
            break;
        case 3: // WEST
            m_Vel.x = -0.1f*delta;
            break;
    }

    vector<cTile*> col = level->GetCollidedTiles(GetBBoxSwept());
    for (int i=0; i<col.size(); ++i) {
        m_Pos += GetMinTranslationVectorRectRect(GetBBoxSwept(), col[i]->GetBBox());
    }
}

void cBros::Render(CORE::cGame* game, float delta, cMainGameState* state)
{
    if (m_State==WALKING||m_State==DRILLING) {
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
    m_Anims.UpdateCurrent(delta);
    ImmediateRenderTexturePos2Dim2(m_Anims.GetCurrentFrame(), GetPos().x, GetPos().y, 64, 64);

}

void cBros::HandleInput(CORE::cGame* game, float delta)
{
    CORE::Input& input = game->GetInput();

    if (m_State!=DYING&&
    !( input.GetKeyState(SDLK_UP)
    || input.GetKeyState(SDLK_RIGHT)
    || input.GetKeyState(SDLK_DOWN)
    || input.GetKeyState(SDLK_LEFT))) {
        m_State = STILL;
    }


    if (input.OnKeyDown(SDLK_UP)) {
        m_Direction = NORTH;
        m_State = WALKING;

    }
    if (input.OnKeyDown(SDLK_RIGHT)) {
        m_Direction = EAST;
        m_State = WALKING;
    }
    if (input.OnKeyDown(SDLK_DOWN)) {
        m_Direction = SOUTH;
        m_State = WALKING;

    }
    if (input.OnKeyDown(SDLK_LEFT)) {
        m_Direction = WEST;
        m_State = WALKING;
    }

}
