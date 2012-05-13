#ifndef CENTITY_H
    #define CENTITY_H

#include <vector>

#include "global_inc.hpp"
#include "GFX_G2D_cAnimationRegistry.hpp"
#include "MATH_Primitives.hpp"

namespace CORE
{
    class cGame;
}
using namespace MATH;

class cMainGameState;

class cEntity
{
public:
protected:
    GFX::G2D::cAnimation m_Anim;
    int                  m_AnimFPS;         //use this to initially set ticks_per_frame for m_Anim
    bool                 m_AnimLooping;

public:
    enum {NORTH, EAST, SOUTH, WEST, NONE };

    cEntity();
    cEntity(const Vec2f& pos, const cRectf& bbox);
    cEntity(float x, float y, float w, float h, float vx, float vy, float offx, float offy, float offw, float offh);
    virtual ~cEntity();
    virtual void Update(CORE::cGame* game, float delta, cMainGameState* state);
    virtual void Render(CORE::cGame* game, float delta, cMainGameState* state); //animation logic in here
    virtual void Collided(cEntity& e);

    void SetAnimFPS(int afps);
    void SetPos(int x, int y)
    { m_Pos = Vec2f(x, y); }
    Vec2f&  GetPos()
    { return m_Pos;  }
    Vec2f&  GetVel()
    { return m_Vel;  }
    Vec2f&  GetDim()
    { return m_Dim;  }
    cRectf GetBBox()
    { return cRectf::GetShiftedRect(m_BBox, m_Pos); }
    cRectf GetBBoxSwept() {
        cRectf temp = cRectf(minf(m_Pos.x+m_BBox.Left()+m_Vel.x, m_Pos.x+m_BBox.Left())
                           , minf(m_Pos.y+m_BBox.Top()+m_Vel.y, m_Pos.y+m_BBox.Top())
                           , maxf(m_BBox.Width()+m_Vel.x, m_BBox.Width())
                           , maxf(m_BBox.Height()+m_Vel.y, m_BBox.Height()));
        return temp;
    }

protected:
    GFX::G2D::cAnimationRegistry m_Anims;
    Vec2f m_Pos;
    Vec2f m_Dim; //size
    Vec2f m_Vel;
    cRectf m_BBox; //bounding box for collision detection
};

#endif // CENTITY_H
