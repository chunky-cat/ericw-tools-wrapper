#include <sstream>
#include <vector>
#include <string>
#include <sstream>
#include <regex>

#include <qbsp/map.hh>

#include <ericw-tools-wrapper/quake_map.h>

namespace ericwtoolswrapper
{

    static std::vector<std::string> rexec_vec(std::string line, std::string regexstr)
    {
        std::stringstream results;
        std::regex re(regexstr, std::regex::icase);

        std::vector<std::string> matches;
        const std::sregex_token_iterator end;
        for (std::sregex_token_iterator it(line.begin(), line.end(), re, 1); it != end; it++)
        {
            matches.push_back(*it);
        }

        return matches;
    }

    std::vector<std::string> QEnt::classNames;

    int QEnt::getClassIndex(const std::string &name)
    {
        for (int i = 0; i < QEnt::classNames.size(); i++)
        {
            if (name == QEnt::classNames[i])
                return i;
        }

        QEnt::classNames.push_back(name);
        return QEnt::classNames.size() - 1;
    }

    QBspMapPtr QBspMap::FromString(const std::string &entString)
    {
        struct parsedObject
        {
            std::stringstream lines;
        };

        std::vector<parsedObject *> objects;
        parsedObject *current = nullptr;
        QBspMapPtr map = std::make_shared<QBspMap>();

        auto ss = std::stringstream(entString);
        for (std::string line; std::getline(ss, line);)
        {
            if (line == "{")
            {
                parsedObject *newobj = new parsedObject;
                objects.push_back(newobj);
                current = newobj;
                continue;
            }
            if (line == "}")
            {
                continue;
            }
            if (current != nullptr)
            {
                current->lines << line << std::endl;
            }
        }

        for (const auto &obj : objects)
        {
            map->parseEntity(obj->lines);
        }

        return map;
    }

    void QBspMap::parseEntity(std::stringstream &ss)
    {
        std::map<std::string, std::string> attributes;
        int classIndex = -1;
        int modelIndex = -1;
        qvec3d origin{0};
        bool hasOrigin = false;
        bool isWorldSpawn = false;
        float angle = 0;
        for (std::string line; std::getline(ss, line);)
        {
            auto matches = rexec_vec(line, R"(\"([\s\S]*?)\")");
            if (matches[0] == "classname")
            {
                if (matches[1] == "worldspawn")
                {
                    isWorldSpawn = true;
                    continue;
                }
                classIndex = QEnt::getClassIndex(matches[1]);
                continue;
            }

            if (matches[0] == "model")
            {
                matches[1].erase(0, 1);
                modelIndex = std::stoi(matches[1]);
                continue;
            }
            if (matches[0] == "origin")
            {
                std::stringstream l(matches[1]);
                l >> origin[0] >> origin[1] >> origin[2];
                hasOrigin = true;
                continue;
            }
            if (matches[0] == "angle")
            {
                angle = std::stof(matches[1]);
                continue;
            }
            attributes[matches[0]] = matches[1];
        }
        if (modelIndex != -1)
        {
            QModelEntPtr m = std::make_shared<QModelEnt>();
            m->classIndex = classIndex;
            m->modelIndex = modelIndex;
            m->attributes = attributes;
            modelEntities.push_back(m);
        }
        else if (modelIndex == -1 && hasOrigin)
        {
            QPointEntPtr p = std::make_shared<QPointEnt>();
            p->classIndex = classIndex;
            p->origin = origin;
            p->angle = angle;
            p->attributes = attributes;
            pointEntities.push_back(p);
        }
        else if (isWorldSpawn)
        {
            QModelEntPtr m = std::make_shared<QModelEnt>();
            m->modelIndex = 0;
            m->attributes = attributes;
            worldspawn = m;
            m->fetchModel();
        }
    }
}