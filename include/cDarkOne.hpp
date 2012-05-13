#ifndef CDARKONE_H
#define CDARKONE_H

#include "cEntity.hpp"
#include "global_inc.hpp"
#include "cTileLevel.hpp"

#define WALKSPEED 0.06f

class cDarkOne : public cEntity
{
    public:
        enum { IDLING, WANDERING, BLINKING, SMILING, DYING };

        cDarkOne(const Vec2f& pos, const cRectf& bbox, const cTileLevel& level);
        virtual ~cDarkOne();

        void Update(CORE::cGame* game, float delta, cMainGameState* state);
        void Render(CORE::cGame* game, float delta, cMainGameState* state);

        void DetermineNewBehavior();
        void DetermineDirection();
        void GetNextDestination();
        void Walk(float delta);
        void Kill();

        void HandleInput(CORE::cGame* game, float delta);
        void HandleJoy(CORE::cGame* game, float delta);

        bool IsPlayerControlled()


        { return m_IsPlayerControlled; }
        void SetPlayerControl(bool b)
        { m_IsPlayerControlled = b; }
        int GetState()
        { return m_State; }
    private:
        int m_State;
        float m_NextBehaviorChange;
        bool m_IsPlayerControlled;
        float m_TurnCooldown;
        int m_DirPreference; // Dark one prefers to go on x axis or y?
        int m_Dir;

        vector<Vec2f> m_Destinations;
        int m_DestIndex;
};

#endif // CDARKONE_H
