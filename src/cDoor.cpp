#include "cDoor.hpp"
#include "GFX_cTextureRegistry.hpp"
#include "GFX_G2D_cTextureRegion.hpp"
#include "GFX_TextureUtilities.hpp"
#include "CORE_cGame.hpp"
#include "cMainGameState.hpp"

using namespace GFX::G2D;
using namespace GFX;

cDoor::cDoor(float x, float y, unsigned int direction, unsigned int type)
: cEntity(),
  m_Direction(direction),
  m_Type(type),
  m_State(e_State::WAITING)
{
    m_Pos = Vec2f(x, y);
    m_Dim = Vec2f(64, 64);

    //offset - needed because these things take 64 x 64 pixels but only 32 x 32 on the map!
    if (m_Direction == e_Direction::SOUTH)
        m_Pos += Vec2f(-m_Dim.x/2, -m_Dim.y/2);
    else if (m_Direction == e_Direction::WEST)
        m_Pos += Vec2f(0, -m_Dim.y/2);
    else if (m_Direction == e_Direction::EAST)
        m_Pos += Vec2f(-m_Dim.x/2, 0);

    m_Anims.SetTicksPerFrame(1000.0f);
    m_Anims.PushAnimation(cAnimation(30.0f,
                         cTextureRegion::SplitTextureHorizontalTexNumXYWH(Art("sheet_alt"), 1, 0, 0, 64, 64)));
    m_Anims.PushAnimation(cAnimation(30.0f,
                         cTextureRegion::SplitTextureHorizontalTexNumXYWH(Art("sheet_alt"), 1, 64, 0, 64, 64)));
    m_Anims.PushAnimation(cAnimation(30.0f,
                         cTextureRegion::SplitTextureHorizontalTexNumXYWH(Art("sheet_alt"), 1, 128, 0, 64, 64)));
    m_Anims.PushAnimation(cAnimation(30.0f,
                         cTextureRegion::SplitTextureHorizontalTexNumXYWH(Art("sheet_alt"), 1, 192, 0, 64, 64)));

    m_BBox = cRectf(0, 0, 64.0f, 64.0f);
    m_Anims.SetCurrentIndex(m_Direction);
}

cDoor::~cDoor()
{
    //dtor
}

void cDoor::Update(CORE::cGame* game, float delta, cMainGameState* state)
{
    if (m_State == e_State::ENTERED)
    {
        //win!
    }
}

void cDoor::Render(CORE::cGame* game, float delta, cMainGameState* state)
{
    const cTextureWrapper& frame = m_Anims.GetCurrentFrame();
    ImmediateRenderTexturePos2Dim2(frame, GetPos().x, GetPos().y, 64, 64);
}
