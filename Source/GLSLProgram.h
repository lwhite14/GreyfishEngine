#pragma once

#ifdef WIN32
#pragma warning( disable : 4290 )
#endif

#include <glad/glad.h>

#include <string>
#include <map>
#include <glm/glm.hpp>
#include <stdexcept>

class GLSLProgramException : public std::runtime_error {
public:
    GLSLProgramException(const std::string &msg) :
            std::runtime_error(msg) {}
};

namespace GLSLShader {
    enum GLSLShaderType {
        VERTEX = GL_VERTEX_SHADER,
        FRAGMENT = GL_FRAGMENT_SHADER,
        GEOMETRY = GL_GEOMETRY_SHADER,
        TESS_CONTROL = GL_TESS_CONTROL_SHADER,
        TESS_EVALUATION = GL_TESS_EVALUATION_SHADER,
        COMPUTE = GL_COMPUTE_SHADER
    };
};

class GLSLProgram {
private:
    GLuint handle;
    bool linked;
    std::map<std::string, int> uniformLocations;

    inline GLint GetUniformLocation(const char *name);
	void DetachAndDeleteShaderObjects();
    bool FileExists(const std::string &fileName);
    std::string GetExtension(const char *fileName);

public:
    GLSLProgram();
	~GLSLProgram();

	// Make it non-copyable.
	GLSLProgram(const GLSLProgram &) = delete;
	GLSLProgram & operator=(const GLSLProgram &) = delete;

	void CompileShader(const char *fileName);
    void CompileShader(const char *fileName, GLSLShader::GLSLShaderType type);
    void CompileShader(const std::string &source, GLSLShader::GLSLShaderType type,
                       const char *fileName = NULL);

    void Link();
    void Validate();
    void Use();

    int GetHandle();
    bool IsLinked();

    void BindAttribLocation(GLuint location, const char *name);
    void BindFragDataLocation(GLuint location, const char *name);

    void SetUniform(const char *name, float x, float y, float z);
    void SetUniform(const char *name, const glm::vec2 &v);
    void SetUniform(const char *name, const glm::vec3 &v);
    void SetUniform(const char *name, const glm::vec4 &v);
    void SetUniform(const char *name, const glm::mat4 &m);
    void SetUniform(const char *name, const glm::mat3 &m);
    void SetUniform(const char *name, float val);
    void SetUniform(const char *name, int val);
    void SetUniform(const char *name, bool val);
    void SetUniform(const char *name, GLuint val);

    void FindUniformLocations();
    void PrintActiveUniforms();
    void PrintActiveUniformBlocks();
    void PrintActiveAttribs();

    const char *getTypeString(GLenum type);
};

int GLSLProgram::GetUniformLocation(const char *name) {
	auto pos = uniformLocations.find(name);

	if (pos == uniformLocations.end()) {
		GLint loc = glGetUniformLocation(handle, name);
		uniformLocations[name] = loc;
		return loc;
	}

	return pos->second;
}

