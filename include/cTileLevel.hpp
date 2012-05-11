#ifndef CTILELEVEL_H
#define CTILELEVEL_H

#include "global_inc.hpp"
#include "cTile.hpp"

#define XTILES 100
#define YTILES 100
#define TILEWIDTH 20

namespace CORE
{
    class cGame;
}
namespace MATH
{
    class cRectf;
}
class cTileLevel
{
    public:
        cTileLevel();
        virtual ~cTileLevel();
        void Init();

        void Update(CORE::cGame* game, float delta);
        void Render(CORE::cGame* game, float delta, const MATH::cRectf& renderRect);

        cTile& GetTileXY(int x, int y);
    private:
        std::array<std::array<cTile, YTILES>, XTILES> tiles;

};

#endif // CTILELEVEL_H
