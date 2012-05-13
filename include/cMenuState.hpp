#ifndef CMENUSTATE_H
#define CMENUSTATE_H

#include "STATE_iGameState.hpp"
#include "GFX_G2D_cSpriteBatch.hpp"
#include "GFX_cTexture.hpp"
#include "GFX_TextureUtilities.hpp"
#include "CORE_cGame.hpp"
//
#include "global_inc.hpp"
#define WINDOW_WIDTH game->GetSDLState().window_w
#define WINDOW_HEIGHT game->GetSDLState().window_h
class cMenuState : public STATE::iGameState
{
    public:
        cMenuState(){}
        virtual ~cMenuState(){}

        static STATE::iGameState* CreateInstance() { return new cMenuState; }
        virtual STATE::iGameState* Clone() { return new cMenuState; } // Ideally this function should return a copy, not a blank instance

        bool OnEnter(CORE::cGame* game)
        {
//            glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
//            glMatrixMode(GL_PROJECTION);
//            glLoadIdentity();
//            glOrtho(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT, -10.0, 10.0);
//            glMatrixMode(GL_MODELVIEW);
//            glLoadIdentity();
//            glScalef(1.0f, -1.0f, 1.0f);
        }
        bool OnExit(CORE::cGame* game) {
            DELETESINGLE(m_Tex);
        }
        void Pause(CORE::cGame* game) {}
        void Resume(CORE::cGame* game) {}

        void HandleInput(CORE::cGame* game) {
            CORE::Input& input = game->GetInput();

            if (input.OnKeyDown(SDLK_RETURN)) {
                game->GetStateManager().PopState();
            }
        }
        void Update(CORE::cGame* game, float delta) {
            HandleInput(game);
        }

        void Render(CORE::cGame* game, float percent_tick) {
            GFX::ImmediateRenderTexturePos2Dim2Origin2Scale2Rot(*m_Tex, 0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT
                                                                ,WINDOW_WIDTH/2.0f, WINDOW_HEIGHT/2.0f,1.0f,-1.0f, 0.0f);
        }
        void SetTexture(GFX::cTextureWrapper* tex)
        { m_Tex = tex; }

    private:
        GFX::cTextureWrapper* m_Tex;
};

#endif // CMENUSTATE_H
