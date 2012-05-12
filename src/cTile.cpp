#include "cTile.hpp"
#include "GFX_cTextureRegistry.hpp"
#include "GFX_TextureUtilities.hpp"
#include "GFX_G2D_cSpriteBatch.hpp"
#include "GFX_TextureUtilities.hpp"
#include "cTileLevel.hpp"

cTile::cTile(float x, float y, bool collidable)
: m_Pos(Vec2f(x, y))
, m_BBox(cRectf(0.0f, 0.0f, TILEWIDTH, TILEWIDTH))
, m_Life(0.0f)
, m_IsDrilled(false)
, m_IsCollidable(collidable)
{
    //ctor
}

cTile::~cTile()
{
    //dtor
}

void cTile::Update(CORE::cGame* game, float delta, cMainGameState* state)
{

}

void cTile::Render(CORE::cGame* game, float delta, GFX::G2D::cSpriteBatch& batch)
{
    static GFX::cTextureWrapper& tex = Art("tile0");
//    batch.DrawTexturePos2Dim2(tex, m_Pos.x, m_Pos.y, TILEWIDTH, TILEWIDTH);
    ImmediateRenderTexturePos2Dim2(tex, m_Pos.x ,m_Pos.y, TILEWIDTH, TILEWIDTH);

}
