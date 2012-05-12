#ifndef CTILELEVEL_H
#define CTILELEVEL_H

#include "global_inc.hpp"
#include "cTile.hpp"

#define XTILES 100
#define YTILES 100

namespace CORE
{
    class cGame;
}
namespace MATH
{
    class cRectf;
}
class cMainGameState;

class cTileLevel
{
    public:
        cTileLevel(int xTiles, int yTiles);
        virtual ~cTileLevel();
        void Init();

        void Update(CORE::cGame* game, float delta, cMainGameState* state);
        void Render(CORE::cGame* game, float delta, GFX::G2D::cSpriteBatch& batch, const MATH::cRectf& renderRect);

        cTile& GetTileXY(int x, int y);
    private:
        cTile*** tiles; // Yeah this is sooo smart >_>
        int m_xTiles, m_yTiles;
        bool m_IsDrilled;

};

#endif // CTILELEVEL_H
