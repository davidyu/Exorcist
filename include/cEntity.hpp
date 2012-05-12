#ifndef CENTITY_H
    #define CENTITY_H

#include <vector>

#include "global_inc.hpp"
#include "GFX_G2D_cAnimation.hpp"

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
    int                  m_AnimFPS;
    bool                 m_AnimLooping;

public:
    cEntity();
    cEntity(Vec2f& pos, Vec2f& dim, Vec2f& vel);
    cEntity(float x, float y, float w, float h, float vx, float vy);
    virtual ~cEntity();
    virtual void Update(CORE::cGame* game, float delta, cMainGameState* state);
    virtual void Render(CORE::cGame* game, float delta, cMainGameState* state); //animation logic in here
    virtual void Collided(cEntity& e);

    Vec2f& GetPos() { return m_Pos; }
    Vec2f& GetVel() { return m_Vel; }

protected:
    Vec2f m_Pos;
    Vec2f m_Dim; //size
    Vec2f m_Vel;
};

#endif // CENTITY_H
