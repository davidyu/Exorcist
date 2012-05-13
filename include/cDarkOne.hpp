#ifndef CDARKONE_H
#define CDARKONE_H

#include "cEntity.hpp"

class cDarkOne : public cEntity
{
    public:
        cDarkOne(const Vec2f& pos, const Vec2f& dim, const Vec2f& vel, const cRectf& bbox);
        virtual ~cDarkOne();

        void Update(CORE::cGame* game, float delta, cMainGameState* state);
        void Render(CORE::cGame* game, float delta, cMainGameState* state);
    private:
        int m_State;
        float m_NextBehaviorChange;
        bool m_IsPlayerControlled;
};

#endif // CDARKONE_H
