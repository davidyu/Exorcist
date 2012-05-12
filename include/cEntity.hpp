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
    static std::vector<cEntity *> EntityList;
protected:
    GFX::G2D::cAnimation m_Anim;
    int                  m_AnimFPS;         //use this to initially set ticks_per_frame for m_Anim
    bool                 m_AnimLooping;

public:
    enum {NORTH, EAST, SOUTH, WEST };

    cEntity();
    cEntity(Vec2f& pos, Vec2f& dim, Vec2f& vel, cRectf& bbox);
    cEntity(float x, float y, float w, float h, float vx, float vy, float offx, float offy, float offw, float offh);
    virtual ~cEntity();
    virtual void Update(CORE::cGame* game, float delta, cMainGameState* state);
    virtual void Render(CORE::cGame* game, float delta, cMainGameState* state); //animation logic in here
    virtual void Collided(cEntity& e);

    void SetAnimFPS(int afps);

    Vec2f&  GetPos()  { return m_Pos;  }
    Vec2f&  GetVel()  { return m_Vel;  }
    Vec2f&  GetDim()  { return m_Dim;  }
    cRectf& GetBBox() { return m_BBox; }

protected:
    Vec2f m_Pos;
    Vec2f m_Dim; //size
    Vec2f m_Vel;
    cRectf m_BBox; //bounding box for collision detection
};

#endif // CENTITY_H
