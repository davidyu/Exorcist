#include "cTileLevel.hpp"

cTileLevel::cTileLevel(int xTiles, int yTiles)
: m_pppTiles(0)
, m_xTiles(xTiles)
, m_yTiles(yTiles)
{
    //ctor
}

cTileLevel::~cTileLevel()
{
    if (m_pppTiles) {
        int i, j;
        for (i=0; i<m_xTiles; ++i) {
            for (j=0; j<m_yTiles; ++j) {
                if (m_pppTiles[i][j]) {
                    DELETESINGLE(m_pppTiles[i][j]);
                } else {
                    std::cout << "Strange things ahappenin' when deleting m_pppTiles2...\n";
                }

            }
            if (m_pppTiles[i]) {
                DELETEARRAY(m_pppTiles[i]);
            } else {
                std::cout << "Strange things ahappenin' when deleting m_pppTiles2...\n";
            }

        }
        DELETEARRAY(m_pppTiles);
    }
}


void cTileLevel::Init()
{
    int i, j;

    m_pppTiles = new cTile**[m_xTiles];
    for (i=0; i<m_xTiles; ++i) {
        m_pppTiles[i] = new cTile*[m_yTiles];
    }

    for (i=0; i<m_xTiles; ++i) {
        for (j=0; j<m_yTiles; ++j) {
            m_pppTiles[i][j] = new cTile((float)(i*40), (float)(j*40), true);
        }
    }

}
void cTileLevel::Update(CORE::cGame* game, float delta, cMainGameState* state)
{
    int i, j;

    for (i=0; i<m_xTiles; ++i) {
        for (j=0; j<m_yTiles; ++j) {
            m_pppTiles[i][j]->Update(game, delta, state);
        }
    }
}

// Renders the m_pppTiles within the rectangle given
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
            m_pppTiles[i][j]->Render(game, delta, batch);
            ++count;
        }
    }
}

cTile& cTileLevel::GetTileClosestToPos(const Vec2f& p)
{
    int x = static_cast<float>(p.x)/TILEWIDTH;
    int y = static_cast<float>(p.y)/TILEWIDTH;

    if (x<m_xTiles&&y<m_yTiles) {
        cout << "Out of bounds m_pppTiles\n";
        assert(false); // T|hrow?
    }
    return *m_pppTiles[x][y];
}
