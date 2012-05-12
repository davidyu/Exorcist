#include "cEntity.hpp"

cEntity::cEntity() {}

cEntity::cEntity(Vec2f& pos, Vec2f& dim, Vec2f& vel)
        :m_Pos(pos), m_Dim(dim), m_Vel(vel)
{}

cEntity::cEntity(float x, float y, float w, float h, float vx, float vy)
{
    m_Pos = Vec2f(x, y);
    m_Dim = Vec2f(w, h);
    m_Vel = Vec2f(vx, vy);
}



cEntity::~cEntity() {}

void cEntity::Update(CORE::cGame* game, float delta, cMainGameState* state) {}
void cEntity::Render(CORE::cGame* game, float delta, cMainGameState* state) {}
