#include <ericw-tools-wrapper/quake_map.h>
#include <iostream>

namespace ericwtoolswrapper
{

        void QModelEnt::fetchModel()
    {
        auto &m = map.bsp.dmodels[modelIndex];
        for (int fid = m.firstface; fid < m.numfaces; fid++)
        {
            auto mface = std::make_shared<QFace>();
            const auto &face = map.bsp.dfaces[fid];
            int edgestep = face.firstedge;
            for (int eid = 0; eid < face.numedges; eid++)
            {
                int surfEdgeID = map.bsp.dsurfedges[face.firstedge + eid];
                qvec3f v{0};
                if (surfEdgeID < 0)
                {
                    v = map.bsp.dvertexes[map.bsp.dedges[abs(map.bsp.dsurfedges[edgestep])][0]]; //[map.edgeLump.edges[Mathf.Abs(map.edgeLump.ledges[edgestep])].vert1];
                }
                else
                {
                    v = map.bsp.dvertexes[map.bsp.dedges[map.bsp.dsurfedges[edgestep]][1]]; //[map.edgeLump.edges[Mathf.Abs(map.edgeLump.ledges[edgestep])].vert1];
                }
                mface->verts.push_back(v);
                edgestep++;
            }
            faces.push_back(mface);

            mface->indices.resize((face.numedges - 2) * 3);
            int tristep = 1;
            for (int i = 1; i < mface->verts.size() - 1; i++)
            {
                mface->indices[tristep - 1] = 0;
                mface->indices[tristep] = i;
                mface->indices[tristep + 1] = i + 1;
                tristep += 3;
            }
        }
    }

}