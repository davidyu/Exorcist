#ifndef CENTITY_H
    #define CENTITY_H

#include <vector>

#include "global_inc.hpp"

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
    //CAnimation animation -- to do!

public:
    cEntity();
    virtual ~cEntity();
    virtual void Update(CORE::cGame* game, float delta, cMainGameState* state);
    virtual void Render(CORE::cGame* game, float delta, cMainGameState* state); //animation logic in here
    virtual void Collide(cEntity *entity); //what to do after colliding with entity?

    Vec2f& GetPos() { return m_Pos; }
    Vec2f& GetVel() { return m_Vel; }

protected:
    Vec2f m_Pos;
    Vec2f m_Dim; //size
    Vec2f m_Vel;
};

#endif // CENTITY_H
