#ifndef CBROS_H
#define CBROS_H

#include "global_inc.hpp"
#include "cEntity.hpp"

namespace CORE
{
    class cGame;
}

using namespace MATH;

class cMainGameState;

class cBros: public cEntity
{
public:
    cBros();
    virtual ~cBros();
    void Update(CORE::cGame* game, float delta, cMainGameState* state);
    void Render(CORE::cGame* game, float delta, cMainGameState* state);
    void HandleInput(CORE::cGame* game, float delta);
private:
    enum { STILL, WALKING, DRILLING, DYING };
    GFX::G2D::cAnimationRegistry m_Anims;
    float m_DrillRate;
    int m_State;

};

#endif // CBROS_H
