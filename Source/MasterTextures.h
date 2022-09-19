#ifndef MASTERTEXTURES_H
#define MASTERTEXTURES_H

#include <map>
#include <string>
#include <vector>
#include "Texture.h"
#include "UserInterface/Console.h"

class MasterTextures
{
public:
    inline static std::vector<Texture*> textureList;

    inline static void NewTexture(std::string textureName)
    {
        bool isName = false;
        std::string str = "";
        for (unsigned int i = 0; i < textureName.size(); i++)
        {
            if (textureName[i] == '.') 
            {
                isName = true;
            }
            if (!isName) 
            {
                str.push_back(textureName[i]);
                if ((textureName[i] == '/') || (textureName[i] == '\\'))
                {
                    str = "";
                }
            }

        }

        for (unsigned int i = 0; i < textureList.size(); i++)
        {
            if (textureList[i]->GetName() == str)
            {
                Console::AddMessage("MasterTextures: Texture already added.");
                return;
            }
        }

        Texture* texture = new Texture(textureName.c_str());
        texture->SetName(str);
        textureList.push_back(texture);
    }

};

#endif //MASTERTEXTURES_H