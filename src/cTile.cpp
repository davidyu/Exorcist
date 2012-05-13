#include "cTile.hpp"
#include "GFX_cTextureRegistry.hpp"
#include "GFX_TextureUtilities.hpp"
#include "GFX_G2D_cSpriteBatch.hpp"
#include "GFX_TextureUtilities.hpp"
#include "cTileLevel.hpp"

cTile::cTile(float x, float y, int type)
: m_Pos(Vec2f(x, y))
, m_BBox(cRectf(0.0f, 0.0f, TILEWIDTH, TILEWIDTH))
, m_Life(0.0f)
, m_IsDrilled(false)
, m_Type(type)
, m_Frames()
, m_FrameIndex(0)
{
    //ctor
}

cTile::~cTile()
{
    //dtor
}

void cTile::Update(CORE::cGame* game, float delta, cMainGameState* state)
{
    if (m_IsDrilled) {
        m_IsDrilled = false;
    } else {
        m_Life = 1.0f;
    }

    if (m_Type==COLLIDABLE) {
        if (m_Life<0.6f) {
            m_FrameIndex = 2;
        } else if (m_Life<0.9f) {
            m_FrameIndex = 1;

        } else {
            m_FrameIndex = 0.0f;
        }
    }
}

void cTile::Render(CORE::cGame* game, float delta, GFX::G2D::cSpriteBatch& batch)
{
    GFX::cTextureWrapper& tex = m_Frames[m_FrameIndex];
//    batch.DrawTexturePos2Dim2(tex, m_Pos.x, m_Pos.y, TILEWIDTH, TILEWIDTH);
    ImmediateRenderTexturePos2Dim2(tex, m_Pos.x ,m_Pos.y, TILEWIDTH, TILEWIDTH);
}
