#ifndef MASTEROBJMESHES_H
#define MASTEROBJMESHES_H

#include <vector>
#include "Drawables/ObjMesh.h"

class MasterObjMeshes
{
public:
    inline static std::vector<ObjMesh*> objMeshList;

    inline static void NewObjMesh(std::string objMeshName)
    {
        std::string str = "Assets/Models/";
        for (unsigned int i = 0; i < objMeshName.size(); i++)
        {
            if (objMeshName[i] != '\0') { str.push_back(objMeshName[i]); }
            else { break; }
        }
        ObjMesh* objMesh = ObjMesh::Load(str.c_str());
        objMesh->SetName(objMeshName);
        objMeshList.push_back(objMesh);
    }
};

#endif //MASTEROBJMESHES_H