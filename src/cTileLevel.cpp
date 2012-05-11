#include "cTileLevel.hpp"

cTileLevel::cTileLevel(int xTiles, int yTiles)
: tiles(0)
, m_xTiles(xTiles)
, m_yTiles(yTiles)
{
    //ctor
}

cTileLevel::~cTileLevel()
{
    if (tiles) {
        int i, j;
        for (i=0; i<m_xTiles; ++i) {
            for (j=0; j<m_yTiles; ++j) {
                if (tiles[i][j]) {
                    DELETESINGLE(tiles[i][j]);
                } else {
                    std::cout << "Strange things ahappenin' when deleting tiles2...\n";
                }

            }
            if (tiles[i]) {
                DELETEARRAY(tiles[i]);
            } else {
                std::cout << "Strange things ahappenin' when deleting tiles2...\n";
            }

        }
        DELETEARRAY(tiles);
    }
}


void cTileLevel::Init()
{
    int i, j;

    tiles = new cTile**[m_xTiles];
    for (i=0; i<m_xTiles; ++i) {
        tiles[i] = new cTile*[m_yTiles];
    }

    for (i=0; i<m_xTiles; ++i) {
        for (j=0; j<m_yTiles; ++j) {
            tiles[i][j] = new cTile((float)(i*40), (float)(j*40));
        }
    }

}
void cTileLevel::Update(CORE::cGame* game, float delta)
{

}

// Renders the tiles within the rectangle given
void cTileLevel::Render(CORE::cGame* game, float delta, GFX::G2D::cSpriteBatch& batch, const MATH::cRectf& renderRect)
{

    const int left = static_cast<int>(renderRect.Left())/TILEWIDTH;
    const int right = static_cast<int>(renderRect.Right())/TILEWIDTH;
    const int top = static_cast<int>(renderRect.Top())/TILEWIDTH;
    const int bottom = static_cast<int>(renderRect.Bottom())/TILEWIDTH;
    int i, j, count =0;

//    cout << left << COMMA << right << COMMA << top << COMMA << bottom << endl;

    for (i=left; i<=right; ++i) {
        for (j=top; j<=bottom; ++j) {
            tiles[i][j]->Render(game, delta, batch);
            ++count;

        }
    }

}
