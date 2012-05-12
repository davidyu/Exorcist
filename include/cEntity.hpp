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
    cEntity(Vec2f& pos, Vec2f& dim, Vec2f& vel);
    cEntity(float x, float y, float w, float h, float vx, float vy);
    virtual ~cEntity();
    virtual void Update(CORE::cGame* game, float delta, cMainGameState* state);
    virtual void Render(CORE::cGame* game, float delta, cMainGameState* state); //animation logic in here

    Vec2f& GetPos() { return m_Pos; }
    Vec2f& GetVel() { return m_Vel; }

protected:
    Vec2f m_Pos;
    Vec2f m_Dim; //size
    Vec2f m_Vel;
};

#endif // CENTITY_H
