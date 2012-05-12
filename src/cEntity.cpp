#include "cEntity.hpp"

cEntity::cEntity()
        :m_AnimFPS(10), m_AnimLooping(true)
{
    m_Anim.SetTicksPerFrame((int) (1000.0f * 1.0f / m_AnimFPS));
}

cEntity::cEntity(Vec2f& pos, Vec2f& dim, Vec2f& vel)
        :m_Pos(pos), m_Dim(dim), m_Vel(vel), m_AnimLooping(true),
         m_AnimFPS(10)
{
    m_Anim.SetTicksPerFrame((int) (1000.0f * 1.0f / m_AnimFPS));
}

cEntity::cEntity(float x, float y, float w, float h, float vx, float vy)
        :m_AnimFPS(10), m_AnimLooping(true)
{
    m_Anim.SetTicksPerFrame((int) (1000.0f * 1.0f / m_AnimFPS));
    m_Pos = Vec2f(x, y);
    m_Dim = Vec2f(w, h);
    m_Vel = Vec2f(vx, vy);
}

cEntity::~cEntity() {}

void cEntity::Update(CORE::cGame* game, float delta, cMainGameState* state)
{
    m_Anim.GetKeyFrame(delta, m_AnimLooping);
}
void cEntity::Render(CORE::cGame* game, float delta, cMainGameState* state) {}
void cEntity::Collided(cEntity& e) {}
