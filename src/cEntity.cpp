#include "cEntity.hpp"


cEntity::cEntity()
: m_AnimFPS(10), m_AnimLooping(true)
, m_Pos(Vec2f(0.0f, 0.0f))
{
    m_Anim.SetTicksPerFrame((int) (1000.0f * (1.0f / m_AnimFPS)));
}

cEntity::cEntity(const Vec2f& pos, const cRectf& bbox)
        :m_Pos(pos), m_Dim(Vec2f(1.0f, 1.0f)), m_Vel(Vec2f(0.0f, 0.0f)), m_BBox(bbox),
         m_AnimLooping(true), m_AnimFPS(10)
{
    m_Anim.SetTicksPerFrame((int) (1000.0f * (1.0f / m_AnimFPS)));
}

cEntity::cEntity(float x, float y, float w, float h, float vx, float vy, float offx, float offy, float offw, float offh)
        :m_AnimFPS(10), m_AnimLooping(true)
{
    m_Anim.SetTicksPerFrame((int) (1000.0f * (1.0f / m_AnimFPS)));
    m_Pos = Vec2f(x, y);
    m_Dim = Vec2f(w, h);
    m_Vel = Vec2f(vx, vy);
    m_BBox = cRectf(offx, offy, offw, offh);
}

cEntity::~cEntity() {}

void cEntity::Update(CORE::cGame* game, float delta, cMainGameState* state)
{
    m_Anim.GetKeyFrame(delta, m_AnimLooping);
}

void cEntity::Render(CORE::cGame* game, float delta, cMainGameState* state) {}
void cEntity::Collided(cEntity& e) {}

void cEntity::SetAnimFPS(int afps)
{
    m_AnimFPS = afps;
    m_Anim.SetTicksPerFrame((int) (1000.0f * (1.0f / m_AnimFPS))); //update ticks_per_frame
}
