#ifndef CDARKONE_H
#define CDARKONE_H

#include "cEntity.hpp"
#include "global_inc.hpp"

class cDarkOne : public cEntity
{
    public:
        enum { IDLING, WANDERING, BLINKING, SMILING };

        cDarkOne(const Vec2f& pos, const cRectf& bbox);
        virtual ~cDarkOne();

        void Update(CORE::cGame* game, float delta, cMainGameState* state);
        void Render(CORE::cGame* game, float delta, cMainGameState* state);

        void DetermineNewBehavior();
        void DetermineDirection();
        void Walk(float delta);
    private:
        int m_State;
        float m_NextBehaviorChange;
        bool m_IsPlayerControlled;
        float m_TurnCooldown;
        int m_DirPreference; // Dark one prefers to go on x axis or y?
        int m_Dir;

        vector<Vec2f> m_Destinations;
};

#endif // CDARKONE_H
