#ifndef MAINGAMESTATE_H
#define MAINGAMESTATE_H

#include "STATE_iGameState.hpp"
#include "GFX_G2D_cSpriteBatch.hpp"
#include "GFX_cTexture.hpp"
#include "cBros.hpp"
#include "GFX_cOrthoCamera.hpp"
//
#include "global_inc.hpp"
#include "cTileLevel.hpp"


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

        void SelectP2DarkOne();

        cBros* GetPlayer() { return m_Player; }

        cTileLevel* GetLevel()
        { return &m_Levels[m_LevelIndex]; }

        static void IncrementLevelIndex()
        { m_LevelIndex++; }

        static int GetLevelIndex()
        { return m_LevelIndex; }

    private:

        static int m_LevelIndex;

        void RegisterSpriteFrames();

        GFX::cOrthoCamera* m_Camera;
        GFX::G2D::cSpriteBatch m_batch;
        GFX::cTextureWrapper* m_pMotionTex;
        GFX::cTextureWrapper* m_pLightTex;
        GFX::G2D::cAnimation* m_pAnimStaticOverlay;
        std::vector<GFX::cTexture> texs;

        vector<cTileLevel> m_Levels;


        cBros* m_Player;
        int m_P2Index;

};

#endif // MAINGAMESTATE_H
