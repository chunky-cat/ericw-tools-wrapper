#include <ericw-tools-wrapper/wrapper.h>

#include <qbsp/qbsp.hh>
#include <qbsp/map.hh>

#include <common/settings.hh>
#include <common/log.hh>

#include <vector>
#include <iostream>
#include <format>

// forward declarations
void Wrapped_CreateHulls();
void Wrapped_WriteEntitiesToString();
void Wrapped_BSPX_CreateBrushListWrapped();
void Wrapped_LoadSecondaryTexturesWrapped();
void BeginBSPFile();

namespace ericwtoolswrapper
{
    char *convert_opts(const std::string &s)
    {
        char *pc = new char[s.size() + 1];
        std::strcpy(pc, s.c_str());
        return pc;
    }

    QBspMapPtr QBsp_CreateBSP(std::string mapFile, QBspOptions opts)
    {
        logging::preinitialize();
        logging::mask = logging::flag::NONE;

        std::vector<std::string> vopts;
        vopts.push_back("nothing");

        if (opts.bsp2)
            vopts.push_back("-bsp2");
        if (opts.notexures)
            vopts.push_back("-notex");
        if (opts.quite)
            vopts.push_back("-quiet");
        if (opts.noclip)
            vopts.push_back("-noclip");
        if (opts.nosubdivide)
            vopts.push_back("-nosubdivide");

        vopts.push_back(mapFile);
        std::vector<char *> vc;
        std::transform(vopts.begin(), vopts.end(), std::back_inserter(vc), convert_opts);

        map.reset();
        InitQBSP(vc.size(), (const char **)&vc[0]);
        //  load brushes and entities
        LoadMapFile();
        // handle load time operation on the .map
        ProcessMapBrushes();
        Wrapped_LoadSecondaryTexturesWrapped();
        BeginBSPFile();

        // create hulls!
        Wrapped_CreateHulls();

        Wrapped_WriteEntitiesToString();
        Wrapped_BSPX_CreateBrushListWrapped();

        auto m = QBspMap::FromString(map.bsp.dentdata);
        return m;
    }
}