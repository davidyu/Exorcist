#ifndef CTILELEVEL_H
#define CTILELEVEL_H

#include <string>

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
        cTileLevel(string levelName);
        virtual ~cTileLevel();

        void Init(cMainGameState* state);
        void Clear();

        void Update(CORE::cGame* game, float delta, cMainGameState* state);
        void Render(CORE::cGame* game, float delta, GFX::G2D::cSpriteBatch& batch, cRectf* renderRect);

        cTile* GetTileXY(int x, int y);
        void GetTilePosClosestToPos(const Vec2f& p, int& x, int& y) const;
        vector<cTile*> GetCollidedTiles(const cRectf& r);
        bool IsWithinRangeXY(int x, int y);
    private:
        cTile*** m_pppTiles; // Yeah this is sooo smart >_>
        int m_xTiles, m_yTiles;
        bool m_IsDrilled; //why here?
        enum e_TileType : unsigned int;
        enum e_EntityType : unsigned int;

        bool m_Init;

        GFX::cImage* m_LevelMap;


};

#endif // CTILELEVEL_H
