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

namespace GFX
{
    namespace G2D
    {
        class cAnimation;
    }
}
class cTileLevel;
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
        void RenderMain(CORE::cGame* game, float percent_tick);
        void RenderMotionBlur(CORE::cGame* game, float percent_tick);
        void RenderLightMask(CORE::cGame* game, float percent_tick);
        void BuildLightMask(CORE::cGame* game, float percent_tick);
        void BuildMotionBlurFrame(CORE::cGame* game, float percent_tick);

    private:

        void RegisterSpriteFrames();

        GFX::G2D::cSpriteBatch m_batch;
        GFX::cTextureWrapper* m_pMotionTex;
        GFX::cTextureWrapper* m_pLightTex;
        GFX::G2D::cAnimation* m_pAnimStaticOverlay;
        std::vector<GFX::cTexture> texs;

        cTileLevel* m_pLevel;

};

#endif // MAINGAMESTATE_H
