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

        if (FILE* file = fopen(textureName.c_str(), "r"))
        {
            fclose(file);
        }
        else
        {
            Console::AddWarningMessage("Texture: '" + str + "' location is not valid, file has either been moved or deleted.");
            return;
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
        texture->SetFilePath(textureName);
        textureList.push_back(texture);
    }

    inline static void RemoveTexture(std::string fileName) 
    {
        for (unsigned int i = 0; i < textureList.size(); i++)
        {
            if (textureList[i]->GetName() == fileName)
            {
                textureList.erase(textureList.begin() + i);
            }
        }
    }
};

#endif //MASTERTEXTURES_H