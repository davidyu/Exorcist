#include "cBros.hpp"
#include "GFX_cTextureRegistry.hpp"
#include "GFX_G2D_cTextureRegion.hpp"
#include "GFX_TextureUtilities.hpp"

using namespace GFX::G2D;

cBros::cBros()
: cEntity()
, m_Anims()
, m_DrillRate(0.3f)
{
    m_Anims.SetTicksPerFrame(300.0f);
    m_Anims.PushAnimation(cAnimation(30.0f,
                         cTextureRegion::SplitTextureHorizontalTexNumXYWH(Art("sheet"), 4, 0, 0, 64, 64)));
    m_Pos.y = 200.0f;
}

cBros::~cBros()
{
    //dtor
}

void cBros::Update(CORE::cGame* game, float delta, cMainGameState* state)
{
    HandleInput(game, delta);


}

void cBros::Render(CORE::cGame* game, float delta, cMainGameState* state)
{
    m_Anims.UpdateCurrent(delta);
    ImmediateRenderTexturePos2Dim2(m_Anims.GetCurrentFrame(), GetPos().x, GetPos().y, 64, 64);

}

void cBros::HandleInput(CORE::cGame* game, float delta){}
