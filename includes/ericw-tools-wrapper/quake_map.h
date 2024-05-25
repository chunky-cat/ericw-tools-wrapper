#pragma once

#include <map>
#include <vector>
#include <string>
#include <memory>
#include <common/qvec.hh>
#include <qbsp/map.hh>

namespace ericwtoolswrapper
{
    struct QFace
    {
        std::vector<qvec3f> verts;
        std::vector<qvec3f> normals;
        std::vector<qvec2d> uvs;
        std::vector<int> indices;
        int texID;
    };

    class QEnt
    {
    public:
        const std::string &ClassName() { return classNames[classIndex]; }
        const std::map<std::string, std::string> &Attributes() { return attributes; }

    protected:
        int classIndex = -1;
        static int getClassIndex(const std::string &name);
        std::map<std::string, std::string> attributes;

    private:
        static std::vector<std::string> classNames;

        friend class QBspMap;
    };

    class QPointEnt : public QEnt
    {
    public:
        const qvec3f &Origin() { return origin; }
        float Angle() { return angle; }

    private:
        qvec3d origin;
        float angle;
        friend class QBspMap;
    };

    using QFacePtr = std::shared_ptr<QFace>;

    class QModelEnt : public QEnt
    {
    public:
        const std::vector<QFacePtr> &Faces() { return faces; }

    private:
        void fetchModel();
        void buildBSPTree(const bsp2_dnode_t &node);
        void getSurfaceIDsFromLeaf(int leafID);
        int getVertIndexFromEdge(int surfEdge);
        int modelIndex;
        std::vector<QFacePtr> faces;
        std::vector<int> surfaces;
        friend class QBspMap;
    };

    class QBspMap;

    using QPointEntPtr = std::shared_ptr<QPointEnt>;
    using QModelEntPtr = std::shared_ptr<QModelEnt>;
    using QBspMapPtr = std::shared_ptr<QBspMap>;

    class QBspMap
    {
    public:
        static QBspMapPtr FromString(const std::string &entString);
        const std::vector<QPointEntPtr> &PointEnts() { return pointEntities; }
        const std::vector<QModelEntPtr> &ModelEnts() { return modelEntities; }
        const QModelEntPtr WorldSpawn() { return worldspawn; }

    private:
        std::vector<QPointEntPtr> pointEntities;
        std::vector<QModelEntPtr> modelEntities;
        std::vector<qvec3f> vertPool;
        void parseEntity(std::stringstream &ss);
        QModelEntPtr worldspawn;
    };
}