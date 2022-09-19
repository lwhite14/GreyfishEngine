#ifndef MODEL_H
#define MODEL_H

#include "../SceneObjects/Component.h"
#include "ObjMesh.h"

class Model : public Component
{
private:
	ObjMesh* m_objMesh;
    SceneObject* m_associatedObject;
    Texture* m_texture;
    glm::vec3 m_matAmbient;
    glm::vec3 m_matDiffuse;
    glm::vec3 m_matSpecular;
    float m_matShininess;

    float m_matAmbientArr[3];
    float m_matDiffuseArr[3];
    float m_matSpecularArr[3];
    float m_matShininessArr;

public:
	Model();
    Model(ObjMesh* objMesh, SceneObject* associatedObject, Texture* texture);
	void Update(float deltaTime);
	void Render(GLSLProgram* prog);
	void DrawUI();
	void Serialization(YAML::Emitter& out);

    void SetMatAmbient(glm::vec3 matAmbient);
    void SetMatDiffuse(glm::vec3 matDiffuse);
    void SetMatSpecular(glm::vec3 matSpecular);
    void SetMatShininess(float matShininess);

    void SetAssociatedObject(SceneObject* associatedObject);
};

#endif //MODEL_H