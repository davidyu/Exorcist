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

#define NUMDESTS 6

cDarkOne::cDarkOne(const Vec2f& pos, const cRectf& bbox, const cTileLevel& level)
: m_State(IDLING)
, m_NextBehaviorChange(0.0f)
, m_IsPlayerControlled(false)
, m_TurnCooldown(0.0f)
, m_DirPreference(0) // Dark one prefers to go on x axis or y?
, m_Dir(NORTH)
, m_Destinations()
, m_DestIndex(0)
{
    m_Anims.SetTicksPerFrame(300.0f);
    m_Anims.PushAnimation(cAnimation(30.0f,
                         cTextureRegion::SplitTextureHorizontalTexNumXYWH(Art("sheet"), 4, 256, 0, 64, 64)));
    m_Anims.PushAnimation(cAnimation(30.0f,
                         cTextureRegion::SplitTextureHorizontalTexNumXYWH(Art("sheet"), 4, 256, 64, 64, 64)));
    m_Anims.PushAnimation(cAnimation(30.0f,
                         cTextureRegion::SplitTextureHorizontalTexNumXYWH(Art("sheet"), 4, 256, 128, 64, 64)));
    m_Anims.PushAnimation(cAnimation(30.0f,
                         cTextureRegion::SplitTextureHorizontalTexNumXYWH(Art("sheet"), 4, 256, 192, 64, 64)));


    m_Anims.PushAnimation(cAnimation(30.0f,
                         cTextureRegion::SplitTextureHorizontalTexNumXYWH(Art("sheet"), 4, 256, 256, 64, 64)));

    m_Anims.SetTicksPerFrame(200.0f);
    m_Anims.PushAnimation(cAnimation(30.0f,
                         cTextureRegion::SplitTextureHorizontalTexNumXYWH(Art("sheet"), 2, 384, 320, 64, 64)));
    m_Anims.SetTicksPerFrame(100.0f);
    m_Anims.PushAnimation(cAnimation(30.0f,
                         cTextureRegion::SplitTextureHorizontalTexNumXYWH(Art("sheet"), 4, 256, 384, 64, 64)));



    const float DIST = 400.0f;
    int x, y;
    Vec2f v;
    for (int i=0; i<NUMDESTS; ++i) {
        v = m_Pos+Vec2f(RandFloat(-DIST, DIST), RandFloat(-DIST, DIST));
        level.GetTilePosClosestToPos(v, x, y);
        while (x==-1||y==-1) {
            v = m_Pos+Vec2f(RandFloat(-DIST, DIST), RandFloat(-DIST, DIST));
            level.GetTilePosClosestToPos(v, x, y);
        }
        m_Destinations.push_back(v);
    }


}

cDarkOne::~cDarkOne()
{
    //dtor
}

#define TOL 15.0f

void cDarkOne::Update(CORE::cGame* game, float delta, cMainGameState* state)
{
    if (!m_IsPlayerControlled) {

        if (m_NextBehaviorChange<0.0f) {
            DetermineNewBehavior();
        }

        if (m_State==WANDERING){
            m_TurnCooldown -= delta;
            DetermineDirection();
            if (m_Dir!=NONE) {
                Walk(delta);
            }
        }
        m_NextBehaviorChange -= delta;
    } else {
        HandleInput(game, delta);
        if (m_State==WANDERING){
            if (m_Dir!=NONE) {
                Walk(delta);
            }
        }
    }

    m_Pos += m_Vel;
    m_Vel *= expf(-0.05*delta);
}

void cDarkOne::GetNextDestination()
{
    m_DestIndex = ++m_DestIndex % NUMDESTS;
}

void cDarkOne::DetermineDirection()
{
    const float xDelta = m_Destinations[0].x-m_Pos.x;
    const float yDelta = m_Destinations[0].y-m_Pos.y;

        if (m_TurnCooldown<0.0f) {
            m_TurnCooldown = RandFloat(300.0f, 4000.0f);
            m_DirPreference = RandInt(0,2);
        }
        if (fabs(xDelta)<TOL&&fabs(yDelta)<TOL) {

            GetNextDestination();
        } else if (m_DirPreference==0) {
            m_Dir = NONE;
        }
        else if (m_DirPreference==1) {
            if (fabs(xDelta)>TOL) {
                if (xDelta < 0) {
                    m_Dir = WEST;
                } else {
                    m_Dir = EAST;
                }
            } else if (fabs(yDelta)>TOL) {
                if (yDelta < 0) {
                    m_Dir = NORTH;
                } else {
                    m_Dir = SOUTH;
                }
            } else {

            }
        } else {
            if (fabs(yDelta)>TOL) {
                if (yDelta < 0) {
                    m_Dir = NORTH;
                } else {
                    m_Dir = SOUTH;
                }
            } else if (fabs(xDelta)>TOL) {
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
            m_Vel.y = -WALKSPEED*delta;
            break;
        case 1: // EAST
            m_Vel.x = WALKSPEED*delta;
            break;
        case 2: // SOUTH
            m_Vel.y = WALKSPEED*delta;
            break;
        case 3: // WEST
            m_Vel.x = -WALKSPEED*delta;
            break;
        default:
            break;
}
}
void cDarkOne::Render(CORE::cGame* game, float delta, cMainGameState* state)
{
    if (m_State==WANDERING) {
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
            default:
                break;
        }
    } else if (m_State==BLINKING) {
        m_Anims.SetCurrentIndex(4);
    } else if (m_State==SMILING) {
        m_Anims.SetCurrentIndex(5);
    } else if (m_State==IDLING) {
        m_Anims.SetCurrentIndex(6);
    }
        if (m_State==WANDERING&&m_Dir==NONE) {
            const cTextureWrapper& frame
             = m_Anims[m_Anims.GetCurrentIndex()][0];
            ImmediateRenderTexturePos2Dim2(frame, GetPos().x, GetPos().y, 64, 64);
        } else if (true) {
            const cTextureWrapper& frame
             = m_Anims.GetCurrentFrame();
             ImmediateRenderTexturePos2Dim2(frame, GetPos().x, GetPos().y, 64, 64);
        }
         m_Anims.UpdateCurrent(delta);
}

void cDarkOne::DetermineNewBehavior()
{
    m_State = RandInt(0, 7);
    if (m_State>4) {
        m_State = WANDERING;
    }
    switch (m_State) {
        case WANDERING:
            GetNextDestination();
            m_NextBehaviorChange = RandFloat(3000.0f, 12000.0f);
            break;
        case BLINKING:
            m_NextBehaviorChange = RandFloat(1000.0f, 2000.0f);
            break;
        case SMILING:
            m_NextBehaviorChange = RandFloat(1500.0f, 2000.0f);
            break;
        case IDLING:
            m_NextBehaviorChange = RandFloat(2000.0f, 3000.0f);
            break;
    }
}

void cDarkOne::HandleInput(CORE::cGame* game, float delta)
{
    CORE::Input& input = game->GetInput();

    if (m_State==WANDERING&&
    !( input.GetKeyState(SDLK_w)
    || input.GetKeyState(SDLK_d)
    || input.GetKeyState(SDLK_a)
    || input.GetKeyState(SDLK_s))) {
        m_State = WANDERING;
        m_Dir = NONE;
    }

    if (input.OnKeyDown(SDLK_w)) {
        m_Dir = NORTH;
        m_State = WANDERING;
    }
    if (input.OnKeyDown(SDLK_d)) {
        m_Dir = EAST;
        m_State = WANDERING;
    }
    if (input.OnKeyDown(SDLK_s)) {
        m_Dir = SOUTH;
        m_State = WANDERING;

    }
    if (input.OnKeyDown(SDLK_a)) {
        m_Dir = WEST;
        m_State = WANDERING;
    }
    if (input.OnKeyDown(SDLK_q)) {
        m_State = BLINKING;
    }
    if (input.OnKeyDown(SDLK_e)) {
        m_State = IDLING;
    }
    if (input.OnKeyDown(SDLK_r)) {
        m_State = SMILING;
    }

}

