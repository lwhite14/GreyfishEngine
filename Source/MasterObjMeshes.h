#ifndef MASTEROBJMESHES_H
#define MASTEROBJMESHES_H

#include <vector>
#include "Drawables/ObjMesh.h"
#include "UserInterface/Console.h"

class MasterObjMeshes
{
public:
    inline static std::vector<ObjMesh*> objMeshList;

    inline static void NewObjMesh(std::string objMeshName)
    {
        bool isName = false;
        std::string str = "";
        for (unsigned int i = 0; i < objMeshName.size(); i++)
        {
            if (objMeshName[i] == '.')
            {
                isName = true;
            }
            if (!isName)
            {
                str.push_back(objMeshName[i]);
                if ((objMeshName[i] == '/') || (objMeshName[i] == '\\'))
                {
                    str = "";
                }
            }
        }

        for (unsigned int i = 0; i < objMeshList.size(); i++)
        {
            if (objMeshList[i]->GetName() == str)
            {
                Console::AddMessage("MasterObjMeshes: ObjMesh already added.");
                return;
            }
        }

        ObjMesh* objMesh = ObjMesh::Load(objMeshName.c_str());
        objMesh->SetName(str);
        objMesh->SetFilePath(objMeshName);
        objMeshList.push_back(objMesh);
    }
};

#endif //MASTEROBJMESHES_H