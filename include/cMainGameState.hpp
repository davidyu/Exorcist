#ifndef MAINGAMESTATE_H
#define MAINGAMESTATE_H

#include "STATE_iGameState.hpp"
#include "GFX_G2D_cSpriteBatch.hpp"
#include "GFX_cTexture.hpp"
//
#include "global_inc.hpp"


namespace CORE
{
    class cGame;
    class Input;
}

//static STATE::iGameState* CreateInstance() {}

class cMainGameState : public STATE::iGameState
{
    public:
        cMainGameState();
        virtual ~cMainGameState();

        static STATE::iGameState* CreateInstance();
        virtual STATE::iGameState* Clone(); // Ideally this function should return a copy, not a blank instance

        bool OnEnter(CORE::cGame* game);
        bool OnExit(CORE::cGame* game);
        void Pause(CORE::cGame* game);
        void Resume(CORE::cGame* game);

        void HandleInput(CORE::cGame* game);
        void Update(CORE::cGame* game, float delta);

        void Render(CORE::cGame* game, float percent_tick);

    private:
        GFX::G2D::cSpriteBatch m_batch;
        GFX::cTextureWrapper* m_pMotionTex;
        std::vector<GFX::cTexture> texs;
        bool test;
};

#endif // MAINGAMESTATE_H
