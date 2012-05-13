#include "cDarkOne.hpp"
#include "GFX_cTextureRegistry.hpp"
#include "GFX_G2D_cTextureRegion.hpp"
#include "GFX_TextureUtilities.hpp"
#include "CORE_cGame.hpp"
#include "cMainGameState.hpp"
#include "cTileLevel.hpp"
#include "MATH_Math.hpp"

using namespace MATH;
using namespace GFX::G2D;
using namespace GFX;

cDarkOne::cDarkOne(const Vec2f& pos, const cRectf& bbox)
: m_State(IDLING)
, m_NextBehaviorChange(0.0f)
, m_IsPlayerControlled(false)
, m_TurnCooldown(0.0f)
, m_DirPreference(0) // Dark one prefers to go on x axis or y?
, m_Dir(NORTH)
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
}

cDarkOne::~cDarkOne()
{
    //dtor
}

#define TOL 3.0f

void cDarkOne::Update(CORE::cGame* game, float delta, cMainGameState* state)
{
    if (m_IsPlayerControlled) {

    }

    if (m_NextBehaviorChange<0.0f) {
        DetermineNewBehavior();
    }

    if (m_State==WANDERING){
        m_TurnCooldown -= delta;
        DetermineDirection();
        Walk(delta);

    }

    m_NextBehaviorChange -= delta;

    m_Pos += m_Vel;
    m_Vel *= expf(-0.05*delta);
}

void cDarkOne::DetermineDirection()
{
    const float xDelta = m_Destinations[0].x-m_Pos.x;
    const float yDelta = m_Destinations[0].y-m_Pos.y;

        if (m_TurnCooldown<300.0f) {
            m_TurnCooldown = 300.0f;
            m_DirPreference = RandInt(1,2);
        }
        if (fabs(xDelta)<TOL&&fabs(yDelta)<TOL) {
            m_Destinations.pop_back();
            m_Destinations.push_back(Vec2f(m_Pos)+Vec2f(RandFloat(100.0f, 400.0f), RandFloat(100.0f, 400.0f)));
        }
        else if (m_DirPreference==1) {
            if (fabs(xDelta)<TOL) {
                if (xDelta < 0) {
                    m_Dir = WEST;
                } else {
                    m_Dir = EAST;
                }
            }
            if (fabs(yDelta)<TOL) {
                if (yDelta < 0) {
                    m_Dir = NORTH;
                } else {
                    m_Dir = SOUTH;
                }
            }
        } else {
            if (fabs(yDelta)<TOL) {
                if (yDelta < 0) {
                    m_Dir = NORTH;
                } else {
                    m_Dir = SOUTH;
                }
            }
            if (fabs(xDelta)<TOL) {
                if (xDelta < 0) {
                    m_Dir = WEST;
                } else {
                    m_Dir = EAST;
                }
            }

        }
}
void cDarkOne::Walk(float delta)
{
    switch (m_Dir) {
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
}
void cDarkOne::Render(CORE::cGame* game, float delta, cMainGameState* state)
{
    switch (m_Dir) {
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

        const cTextureWrapper& frame
         = m_Anims.GetCurrentFrame();
         ImmediateRenderTexturePos2Dim2(frame, GetPos().x, GetPos().y, 64, 64);
}

void cDarkOne::DetermineNewBehavior()
{
    m_State = RandInt(0, 3);
    m_State = WANDERING;
    switch (m_State) {
        case WANDERING:
            m_Destinations.push_back(Vec2f(m_Pos)+Vec2f(RandFloat(100.0f, 400.0f), RandFloat(100.0f, 400.0f)));
            m_NextBehaviorChange = RandFloat(3000.0f, 12000.0f);
            break;
        case BLINKING:
            m_NextBehaviorChange = RandFloat(2000.0f, 4000.0f);
            break;
        case SMILING:
            m_NextBehaviorChange = RandFloat(4000.0f, 5000.0f);
            break;
        case IDLING:
            m_NextBehaviorChange = RandFloat(3000.0f, 8000.0f);
            break;
    }

}

