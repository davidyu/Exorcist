#ifndef CTEXTUREREGISTRY_H
#define CTEXTUREREGISTRY_H

#include <unordered_map>
#include <string>
#include "GFX_cTexture.hpp"
#include "GFX_G2D_cTextureRegion.hpp"

#define Art(key) GFX::cTextureRegistry::GetTexture(key)

namespace GFX
{
    using namespace GFX::G2D;

    class cTextureRegistry
    {
    public:
        cTextureRegistry() {}
        virtual ~cTextureRegistry() {}

        static void Init();
        static void RegisterTexture(std::string str_key, const cTextureWrapper& tex);
        static cTextureWrapper& GetTexture(std::string str_key);

        static std::unordered_map<std::string, cTextureWrapper> textures_map;

    };
    inline void cTextureRegistry::RegisterTexture(std::string str_key, const cTextureWrapper& tex)
    { textures_map.insert(std::pair<std::string, cTextureWrapper>(str_key, tex)); }

    inline cTextureWrapper& cTextureRegistry::GetTexture(std::string str_key)
    { return textures_map[str_key]; }

    inline void cTextureRegistry::Init()
    {
        cTexture tiles_e = cTexture("art/tileset.png"); tiles_e.RegisterGL();
        RegisterTexture("tileset", tiles_e);

        cTexture sheet = cTexture("art/spritesheet.png"); sheet.RegisterGL();
        RegisterTexture("sheet", sheet);

        cTexture sheet_alt = cTexture("art/spritesheet_alt.png"); sheet_alt.RegisterGL();
        RegisterTexture("sheet_alt", sheet_alt);

        cTexture sta = cTexture("art/static.png"); sta.RegisterGL();
        RegisterTexture("static", sta);

        cTexture tit = cTexture("art/title.png"); tit.RegisterGL();
        RegisterTexture("title", tit);

        cTexture int1 = cTexture("art/int1.png"); int1.RegisterGL();
        RegisterTexture("int1", int1);

        cTexture int2 = cTexture("art/int2.png"); int2.RegisterGL();
        RegisterTexture("int2", int2);
    }
}


#endif // CTEXTUREREGISTRY_H
