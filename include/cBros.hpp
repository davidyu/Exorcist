#ifndef CBROS_H
#define CBROS_H

#include "global_inc.hpp"

namespace CORE
{
    class cGame;
}

using namespace MATH;

class cMainGameState;

class cBros
{
public:
    cBros();
    virtual ~cBros();
    void Update(CORE::cGame* game, float delta, cMainGameState* state);
    void Render(CORE::cGame* game, float delta, cMainGameState* state);
    void HandleInput(CORE::cGame* game, float delta);

    Vec2f& GetPos() { return m_Pos; }
    Vec2f& GetVel() { return m_Vel; }

private:


    Vec2f m_Pos;
    Vec2f m_Vel;
};

#endif // CBROS_H
