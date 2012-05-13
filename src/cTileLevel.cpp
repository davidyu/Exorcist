#include "cTileLevel.hpp"
#include "GFX_cImage.hpp"
#include "GFX_TextureUtilities.hpp"
#include "cEntity.hpp"
#include "cDoor.hpp"
#include "cBros.hpp"
#include "cDarkOne.hpp"
#include "cMainGameState.hpp"

enum cTileLevel::e_TileType : unsigned int
{
    CAVY        =  0xffffffff,
    DIGGY       =  0xff000000,
    STONE_BLOCK =  0xff00ff00
};

enum cTileLevel::e_EntityType : unsigned int
{
    DOOR         = (SDL_BYTEORDER == SDL_BIG_ENDIAN) ? 0xfff7f7f7 : 0xff7f7f7f,
    DOOR_PARTNER = (SDL_BYTEORDER == SDL_BIG_ENDIAN) ? 0xff3c3c3c : 0xffc3c3c3,
    PLAYER       = (SDL_BYTEORDER == SDL_BIG_ENDIAN) ? 0xffffc90e : 0xff0ec9ff,
    DARKONE      = (SDL_BYTEORDER == SDL_BIG_ENDIAN) ? 0xffffff00 : 0xff00ffff
};

cTileLevel::cTileLevel(string levelName)
: m_pppTiles(0)
, m_Init(false)
{
    m_LevelMap = new GFX::cImage(levelName);

    int h = m_LevelMap->GetHeight();
    int w = m_LevelMap->GetWidth();

    m_xTiles = w;
    m_yTiles = h;
}

cTileLevel::~cTileLevel()
{
    Clear();
}


void cTileLevel::Init(cMainGameState* state)
{
    if (m_Init) Clear();
    int i, j;
    m_pppTiles = new cTile**[m_xTiles];

    for (i = 0; i < m_xTiles; ++i)
        m_pppTiles[i] = new cTile*[m_yTiles];


    for (i = 0; i < m_xTiles; i++)
    {
        for (int j = 0; j < m_yTiles; j++)
        {
            unsigned int c = m_LevelMap->GetPixel(i, j);
            //unsigned int b = m_LevelMap->GetPixel(i, j);
            //foreground
            switch(c)
            {
            case e_EntityType::DOOR:
                cDoor* d;
                if (i+1 < m_xTiles && m_LevelMap->GetPixel(i+1,j) == e_EntityType::DOOR_PARTNER)
                {
                    if (j+1 < m_yTiles && m_LevelMap->GetPixel(i,j+1) == e_EntityType::PLAYER)
                    {
                        d = new cDoor(i*TILEWIDTH, j*TILEWIDTH, cDoor::e_Direction::NORTH, cDoor::e_Type::ENTRANCE);
                        state->GetPlayer()->SetPos(i*TILEWIDTH, (j+1)*TILEWIDTH);
                        state->SetEntrance(d);
                    }

                    else
                    {
                        d = new cDoor(i*TILEWIDTH, j*TILEWIDTH, cDoor::e_Direction::NORTH, cDoor::e_Type::EXIT);
                        state->SetExit(d);
                    }
                }
                else if (i-1 > 0 && m_LevelMap->GetPixel(i-1,j) == e_EntityType::DOOR_PARTNER)
                {
                    if (j-1 > 0 && m_LevelMap->GetPixel(i,j-1) == e_EntityType::PLAYER)
                    {
                        d = new cDoor(i*TILEWIDTH, j*TILEWIDTH, cDoor::e_Direction::SOUTH, cDoor::e_Type::ENTRANCE);
                        state->GetPlayer()->SetPos(i*TILEWIDTH, (j-1)*TILEWIDTH);
                        state->SetEntrance(d);
                    }
                    else
                    {
                        d = new cDoor(i*TILEWIDTH, j*TILEWIDTH, cDoor::e_Direction::SOUTH, cDoor::e_Type::EXIT);
                        state->SetExit(d);
                    }

                }
                else if (j+1 < m_yTiles && m_LevelMap->GetPixel(i,j+1) == e_EntityType::DOOR_PARTNER)
                {
                    if (i-1 > 0 && m_LevelMap->GetPixel(i-1,j) == e_EntityType::PLAYER)
                    {
                        d = new cDoor(i*TILEWIDTH, j*TILEWIDTH, cDoor::e_Direction::EAST, cDoor::e_Type::ENTRANCE);
                        state->GetPlayer()->SetPos((i-1)*TILEWIDTH, j*TILEWIDTH);
                        state->SetEntrance(d);
                    }
                    else
                    {
                        d = new cDoor(i*TILEWIDTH, j*TILEWIDTH, cDoor::e_Direction::EAST, cDoor::e_Type::EXIT);
                        state->SetExit(d);
                    }

                }
                else if (j-1 > 0 && m_LevelMap->GetPixel(i,j-1) == e_EntityType::DOOR_PARTNER)
                {
                    if (i+1 < m_xTiles && m_LevelMap->GetPixel(i+1,j) == e_EntityType::PLAYER)
                    {
                        d = new cDoor(i*TILEWIDTH, j*TILEWIDTH, cDoor::e_Direction::WEST, cDoor::e_Type::ENTRANCE);
                        state->GetPlayer()->SetPos((i+1)*TILEWIDTH, j*TILEWIDTH);
                        state->SetEntrance(d);
                    }
                    else
                    {
                        d = new cDoor(i*TILEWIDTH, j*TILEWIDTH, cDoor::e_Direction::WEST, cDoor::e_Type::EXIT);
                        state->SetExit(d);
                    }
                }

                break;
            case e_EntityType::DARKONE:
                state->GetEntities().EntityList.push_back(new cDarkOne(Vec2f(i*TILEWIDTH, j*TILEWIDTH)
                                                          , cRectf(14.0, 0.0f, 36, 64), *state->GetLevel()));
                break;
            }


            //background
            switch (c)
            {
            case e_TileType::CAVY:
                m_pppTiles[i][j] = new cCavy((float)(i*TILEWIDTH), (float)(j*TILEWIDTH));
                break;
            case e_TileType::DIGGY:
                m_pppTiles[i][j] = new cDiggy((float)(i*TILEWIDTH), (float)(j*TILEWIDTH));
                break;
            case e_TileType::STONE_BLOCK:
                m_pppTiles[i][j] = new cBlock((float)(i*TILEWIDTH), (float)(j*TILEWIDTH));
                break;
            default:  //assume cavy
                m_pppTiles[i][j] = new cCavy((float)(i*TILEWIDTH), (float)(j*TILEWIDTH));
                break;
            }

        }
    }
    m_Init = true;
}

void cTileLevel::Clear()
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
void cTileLevel::Render(CORE::cGame* game, float delta, GFX::G2D::cSpriteBatch& batch, MATH::cRectf* renderRect)
{

    int left = static_cast<int>(renderRect->Left())/TILEWIDTH;
    int right = static_cast<int>(renderRect->Right())/TILEWIDTH;
    int top = static_cast<int>(renderRect->Top())/TILEWIDTH;
    int bottom = static_cast<int>(renderRect->Bottom())/TILEWIDTH;
    int i, j, count =0;

    left = (left < 0) ? 0 : left;
    top  = (top  < 0) ? 0 : top;
    right = (right > m_xTiles) ? m_xTiles : right;
    bottom  = (bottom  > m_yTiles) ? m_yTiles : bottom;

    //cout << left << COMMA << right << COMMA << top << COMMA << bottom << endl;

    for (i=left; i<=right; ++i) {
        for (j=top; j<=bottom; ++j) {
            if (!IsWithinRangeXY(i, j)) {
                continue;
            }
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

void cTileLevel::GetTilePosClosestToPos(const Vec2f& p, int& x, int& y) const
{
    const float fx = p.x/TILEWIDTH;
    const float fy = p.y/TILEWIDTH;
    x = static_cast<int>(fx);
    y = static_cast<int>(fy);

    if (fx>=static_cast<float>(m_xTiles)||fx<0.0f) {
        x = -1;
    }if (fy>=static_cast<float>(m_yTiles)||fy<0.0f) {
        y = -1;
    }
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
