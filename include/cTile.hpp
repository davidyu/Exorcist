#ifndef CTILE_H
#define CTILE_H

#include "global_inc.hpp"

#define TILEWIDTH 40.0f

namespace CORE
{
    class cGame;
}

namespace GFX
{
    namespace G2D
    {
        class cSpriteBatch;
    }
}
class cMainGameState;

using namespace MATH;

class cTile
{
    public:
        cTile(float x, float y, bool collidable);
        virtual ~cTile();
        void Update(CORE::cGame* game, float delta, cMainGameState* state);
        void Render(CORE::cGame* game, float delta, GFX::G2D::cSpriteBatch& batch);

        void SetDrilled(bool b)
        { m_IsDrilled = b; }
        float GetLife() const
        { return m_Life; }
        bool IsCollidable() const
        { return m_IsCollidable; }
        cRectf GetBoundingBox() const
        { return cRectf::GetShiftedRect(m_BBox, m_Pos); }
    protected:
        Vec2f m_Pos;
        cRectf m_BBox;
        float m_Life;
        bool m_IsDrilled;
        bool m_IsCollidable;

};

#endif // CTILE_H
