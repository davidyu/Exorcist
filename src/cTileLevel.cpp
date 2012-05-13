#include "cTileLevel.hpp"
#include "GFX_cImage.hpp"
#include "GFX_TextureUtilities.hpp"

enum cTileLevel::e_TileType : unsigned int
{
    NOTHING       = 0X00000000,
    DIGGABLE_SOIL = 0x3a2e2e00,
    STONE_WALL    = 0xffffff00
}

cTileLevel::cTileLevel(int xTiles, int yTiles)
: m_pppTiles(0)
, m_xTiles(xTiles)
, m_yTiles(yTiles)
{
    //ctor
}

cTileLevel::cTileLevel(string levelName)
: m_pppTiles(0)
{
    GFX::cImage lvl(levelName);

    int h = lvl->GetHeight();
    int w = lvl->GetWidth();

    m_xTiles = w;
    m_yTiles = h;

    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            unsigned int c = GFX::GetColourInHex(lvl->GetPixel(x, y));
            switch (c)
            {
            case NOTHING:
                break;
            case DIGGABLE_SOIL:
                break;
            case STONE_WALL:
                break;
            default:  //assume nothing
                break;
            }

        }
    }
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
            if (j>6) {
                m_pppTiles[i][j] = new cTile((float)(i*40), (float)(j*40), true);
            }
            else {
                m_pppTiles[i][j] = new cTile((float)(i*40), (float)(j*40), false);
            }

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

vector<cTile*> cTileLevel::GetCollidedTiles(const cRectf& r)
{
    const int left = static_cast<int>(r.Left())/TILEWIDTH;
    const int right = static_cast<int>(r.Right())/TILEWIDTH;
    const int top = static_cast<int>(r.Top())/TILEWIDTH;
    const int bottom = static_cast<int>(r.Bottom())/TILEWIDTH;
    vector<cTile*> colltiles;

    if (!IsWithinRangeXY(left, top)||!IsWithinRangeXY(right, bottom)) {
        return colltiles;
    }

    int i,j;
    for (i=left; i<=right; ++i) {
        for (j=top; j<=bottom; ++j) {
            if (m_pppTiles[i][j]->IsCollidable()) {
                colltiles.push_back(m_pppTiles[i][j]);
            }

        }
    }
}

cTile* cTileLevel::GetTileClosestToPos(const Vec2f& p, int& x, int& y)
{
    x = static_cast<float>(p.x)/TILEWIDTH;
    y = static_cast<float>(p.y)/TILEWIDTH;

    return GetTileXY(x, y);
}

cTile* cTileLevel::GetTileXY(int x, int y)
{
    if (!IsWithinRangeXY(x, y)) {
        x=y=0;
        return 0;
    }
    return m_pppTiles[x][y];
}

bool cTileLevel::IsWithinRangeXY(int x, int y)
{
    return !(x>=m_xTiles||x<0||y>=m_yTiles||y<0);
}
