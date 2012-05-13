#include "cTileLevel.hpp"
#include "GFX_cImage.hpp"
#include "GFX_TextureUtilities.hpp"

enum cTileLevel::e_TileType : unsigned int
{
    NOTHING       =  0xff000000,
    DIGGABLE_SOIL =  SDL_BYTEORDER == SDL_BIG_ENDIAN ? 0xff3a2e2e : 0xff2e2e3a,
    STONE_WALL    =  0xffffffff
};

cTileLevel::cTileLevel(string levelName)
: m_pppTiles(0)
{
    m_LevelMap = new GFX::cImage(levelName);

    int h = m_LevelMap->GetHeight();
    int w = m_LevelMap->GetWidth();

    m_xTiles = w;
    m_yTiles = h;
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

    for (i = 0; i < m_xTiles; ++i)
        m_pppTiles[i] = new cTile*[m_yTiles]; //reverse: I store map in row-major order


    for (i = 0; i < m_xTiles; i++)
    {
        for (int j = 0; j < m_yTiles; j++)
        {
            unsigned int c = m_LevelMap->GetPixel(i, j);
            switch (c)
            {
            case e_TileType::NOTHING:
                m_pppTiles[i][j] = new cCavy((float)(i*TILEWIDTH), (float)(j*TILEWIDTH));
                break;
            case e_TileType::DIGGABLE_SOIL:
                m_pppTiles[i][j] = new cDiggy((float)(i*TILEWIDTH), (float)(j*TILEWIDTH));
                break;
            case e_TileType::STONE_WALL:
                m_pppTiles[i][j] = new cCavy((float)(i*TILEWIDTH), (float)(j*TILEWIDTH));
                break;
            default:  //assume nothing
                m_pppTiles[i][j] = new cCavy((float)(i*TILEWIDTH), (float)(j*TILEWIDTH));
                break;
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
            if (m_pppTiles[i][j]->GetLife()<=0.0f) {
                cTile* temp = new cCavy(m_pppTiles[i][j]->GetPos().x, m_pppTiles[i][j]->GetPos().y);
                DELETESINGLE(m_pppTiles[i][j]);
                m_pppTiles[i][j] = temp;
                temp = 0;
            }
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
