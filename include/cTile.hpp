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

using namespace MATH;

class cTile
{
    public:
        cTile(float x, float y);
        virtual ~cTile();
        void Render(CORE::cGame* game, float delta, GFX::G2D::cSpriteBatch& batch);
    protected:
        Vec2f m_Pos;

};

#endif // CTILE_H
