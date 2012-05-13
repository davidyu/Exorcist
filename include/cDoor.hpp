#ifndef CDOOR_H
#define CDOOR_H

#include "global_inc.hpp"
#include "cEntity.hpp"

namespace CORE
{
    class cGame;
}

using namespace MATH;

class cMainGameState;

class cDoor: public cEntity
{
public:
    cDoor(float x, float y, unsigned int direction, unsigned int type);
    virtual ~cDoor();

    void Update(CORE::cGame* game, float delta, cMainGameState* state);
    void Render(CORE::cGame* game, float delta, cMainGameState* state);
    void HandleInput(CORE::cGame* game, float delta, cMainGameState* state);

    inline bool IsExit() { return m_Type == e_Type::EXIT; }
    inline bool IsEntrance() { return m_Type == e_Type::ENTRANCE; }

    enum e_Direction : unsigned int { SOUTH, WEST, NORTH, EAST};
    enum e_Type : unsigned int{ ENTRANCE, EXIT };
private:
    enum e_State : unsigned int     { ENTERED, WAITING };
    const unsigned int m_Direction;
    unsigned int m_State;
    unsigned int m_Type;
};

#endif //CDOOR_H
