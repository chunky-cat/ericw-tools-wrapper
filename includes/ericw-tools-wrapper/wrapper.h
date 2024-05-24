#pragma once

#include <string>

namespace ericwtoolswrapper
{
    struct QBspOptions
    {
        bool quite = true;
        bool notexures = true;
        bool bsp2 = true;
        bool noclip = true;
    };

    constexpr QBspOptions default_opts;
    void QBsp_CreateBSP(std::string mapFile, QBspOptions otps = default_opts);
}