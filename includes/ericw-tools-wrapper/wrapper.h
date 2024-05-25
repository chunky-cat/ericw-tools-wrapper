#pragma once

#include <string>
#include <ericw-tools-wrapper/quake_map.h>

namespace ericwtoolswrapper
{
    struct QBspOptions
    {
        bool quite = true;
        bool notexures = true;
        bool bsp2 = true;
        bool noclip = true;
        bool nosubdivide = true;
    };

    constexpr QBspOptions default_opts;
    QBspMapPtr QBsp_CreateBSP(std::string mapFile, QBspOptions otps = default_opts);
}