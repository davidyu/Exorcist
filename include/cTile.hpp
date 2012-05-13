#ifndef CTILE_H
#define CTILE_H

#include "global_inc.hpp"
#include "GFX_G2D_cTextureRegion.hpp"
#include "GFX_cTextureRegistry.hpp"

#define TILEWIDTH 32.0f

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
using namespace GFX::G2D;

class cTile
{
public:
    enum { EMPTY, COLLIDABLE, BLOCK, COLLECTIBLE};

    cTile(float x, float y, int type);
     ~cTile();
    void Update(CORE::cGame* game, float delta, cMainGameState* state);
    void Render(CORE::cGame* game, float delta, GFX::G2D::cSpriteBatch& batch);

    void DecreaseLife(float dec)
    { if (m_Type!=BLOCK) m_Life -= dec; }
    void SetDrilled(bool b)
    { m_IsDrilled = b; }
    void SetFrameIndex(int i)
    { m_FrameIndex = i; }
    void SetType(int type)
    { m_Type = type; }
    float GetLife() const
    { return m_Life; }
    bool IsCollidable() const
    { return m_Type==COLLIDABLE||m_Type==BLOCK; }
    cRectf GetBBox() const
    { return cRectf::GetShiftedRect(m_BBox, m_Pos); }
    const Vec2f& GetPos()
    { return m_Pos; }

protected:
    Vec2f m_Pos;
    cRectf m_BBox;

    float m_Life;
    bool m_IsDrilled;
    int m_Type;

    vector<GFX::cTextureWrapper> m_Frames;
    int m_FrameIndex;

};

// Diggable tile
class cBlock : public cTile
{
public:
    cBlock(float x, float y) : cTile(x, y, true)
    {
        m_Frames.push_back(cTextureRegion(Art("tileset"), 256, 0, 64, 64));
        m_Type = BLOCK;
    }
    ~cBlock(){}
};

// Diggable tile
class cDiggy : public cTile
{
public:
    cDiggy(float x, float y) : cTile(x, y, COLLIDABLE)
    {
        m_Frames.push_back(cTextureRegion(Art("tileset"), 64, 0, 64, 64));
        m_Frames.push_back(cTextureRegion(Art("tileset"), 128, 0, 64, 64));
        m_Frames.push_back(cTextureRegion(Art("tileset"), 192, 0, 64, 64));
    }
    ~cDiggy(){}
};

// Empty tile
class cCavy : public cTile
{
public:
    cCavy(float x, float y) : cTile(x, y, EMPTY)
    {
        m_Frames.push_back(cTextureRegion(Art("tileset"), 0, 0, 64, 64));
    }
    ~cCavy(){}
};

#endif // CTILE_H
