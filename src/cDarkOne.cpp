#include "cDarkOne.hpp"
#include "GFX_cTextureRegistry.hpp"
#include "GFX_G2D_cTextureRegion.hpp"
#include "GFX_TextureUtilities.hpp"
#include "CORE_cGame.hpp"
#include "cMainGameState.hpp"
#include "cTileLevel.hpp"

cDarkOne::cDarkOne(const Vec2f& pos, const Vec2f& dim, const Vec2f& vel, const cRectf& bbox)
: m_State(0)
, m_NextBehaviorChange(0.0f)
, m_IsPlayerControlled(false)
{
    //ctor
}

cDarkOne::~cDarkOne()
{
    //dtor
}


void cDarkOne::Update(CORE::cGame* game, float delta, cMainGameState* state)
{
    if (m_IsPlayerControlled) {

    }

    if (m_NextBehaviorChange<0.0f) {
        m_NextBehaviorChange = RandFloat(3000.0f, 12000.0f);
    }

    m_NextBehaviorChange -= delta;

    m_Pos += m_Vel;
    m_Vel *= expf(-0.05*delta);
}

void cDarkOne::Render(CORE::cGame* game, float delta, cMainGameState* state)
{

}
