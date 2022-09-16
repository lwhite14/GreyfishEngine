#ifndef MASTERTEXTURES_H
#define MASTERTEXTURES_H

#include <map>
#include <string>
#include <vector>
#include "Texture.h"

class MasterTextures
{
public:
    inline static std::vector<Texture*> textureList;

    inline static void NewTexture(std::string textureName)
    {
        std::string str = "Assets/Images/";
        for (unsigned int i = 0; i < textureName.size(); i++)
        {
            if (textureName[i] != '\0') { str.push_back(textureName[i]); }
            else { break; }
        }
        Texture* texture = new Texture(str.c_str());
        texture->SetName(textureName);
        textureList.push_back(texture);
    }

};

#endif //MASTERTEXTURES_H