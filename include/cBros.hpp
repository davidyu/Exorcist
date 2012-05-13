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
    void TryMove(CORE::cGame* game, float delta, cMainGameState* state);
    void Render(CORE::cGame* game, float delta, cMainGameState* state);
    void HandleInput(CORE::cGame* game, float delta, cMainGameState* state);

    void Flare(CORE::cGame* game, float delta, cMainGameState* state);
    cRectf GetFlareBox() { return cRectf::GetShiftedRect(m_FlareBox, m_Pos); }
    void Kill();


private:
    enum { STILL, WALKING, DRILLING, DYING, FLARING };
    float m_DrillRate;
    int m_State;
    int m_Direction;
    int m_FlaresLeft;
    cRectf m_FlareBox;

    int m_DrillChannel;

};

#endif // CBROS_H
