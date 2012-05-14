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
#include "cEntityManager.hpp"

#include "cDoor.hpp"

#define DIGGER m_Entities.EntityList[0]

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
        void Flare(CORE::cGame* game, float percent_tick);

        void SetEntrance(cDoor* d)
        { m_Entrance = d; }

        void SetExit (cDoor* d)
        { m_Exit = d; }

        void SetWinner(int i)
        { if (!m_Win) m_Win = i; }
        void SetHasFlared(bool b)
        { m_HasFlared=b; m_FlareTime=0.0f; }
        void SelectP2DarkOne();

        cBros* GetPlayer() { return m_Player; }

        cTileLevel* GetLevel()
        { return m_pLevels[m_LevelIndex]; }

        cEntityManager& GetEntities()
        { return m_Entities; }

        static void IncrementLevelIndex()
        { m_LevelIndex = ++m_LevelIndex%static_cast<int>(m_pLevels.size()); }

        static void DecrementLevelIndex()
        {
            m_LevelIndex--;
            if (m_LevelIndex < 0) m_LevelIndex = 0;
        }

        static void InitLevels();
        static void ClearLevels();

        static int GetLevelIndex()
        { return m_LevelIndex; }
         bool m_P2Life;

    private:

        static int m_LevelIndex;

        void RegisterSpriteFrames();

        GFX::cOrthoCamera* m_Camera;
        GFX::G2D::cSpriteBatch m_batch;
        GFX::cTextureWrapper* m_pMotionTex;
        GFX::cTextureWrapper* m_pLightTex;
        GFX::G2D::cAnimation* m_pAnimStaticOverlay;
        std::vector<GFX::cTexture> texs;
        cEntityManager m_Entities;

        bool m_HasFlared;
        float m_FlareTime;

        static vector<cTileLevel*> m_pLevels;

        cBros* m_Player;
        cDoor* m_Entrance;
        cDoor* m_Exit;

        int m_P2Index;

        int m_Win;
        float m_Wintime;
        bool m_Quit;
        bool m_IsStatic;
        float m_LightSwitch;


};

#endif // MAINGAMESTATE_H
