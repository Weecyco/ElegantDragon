#include <GL/glew.h>

#include "Shader.h"
#include "graphicUtilities.h"

#include <iostream>
#include <fstream>
#include <string>

Shader::Shader(const std::string& filepath)
	: m_filepath(filepath), m_renderSHID(0)
{
	ShaderSet Shaders = parseShader(filepath);
    m_renderSHID = createShader(Shaders.vertShader, Shaders.fragShader);
    
}

Shader::~Shader()
{
    GL_SAFE(glDeleteProgram(m_renderSHID));
}

void Shader::bind() const
{
    GL_SAFE(glUseProgram(m_renderSHID));
}

void Shader::unbind()
{
    GL_SAFE(glUseProgram(0));
}

//integer / sampler integer
void Shader::setUniform1i(const std::string& name, int i0)
{
    GL_SAFE(glUniform1i(getUniformLocation(name), i0));
}

void Shader::setUniform1f(const std::string& name, float v0)
{
    GL_SAFE(glUniform1f(getUniformLocation(name), v0));
}

void Shader::setUniform2f(const std::string& name, float v0, float v1)
{
    GL_SAFE(glUniform2f(getUniformLocation(name), v0, v1));
}

void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GL_SAFE(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
}

void Shader::setUniformMatrix4f(const std::string& name, float* Mat, unsigned char rowMaj_GLBool)
{
    GL_SAFE(glUniformMatrix4fv(getUniformLocation(name), 1, rowMaj_GLBool, Mat));
}


//split into separate func since there will be more types and formates of uniforms that still \
get their location the same way
int Shader::getUniformLocation(const std::string& name)
{
    //if (UniformMap.count(name) > 0)
    if (UniformMap.find(name) != UniformMap.end())
    {
        return UniformMap[name];
    }
    GL_SAFE(int location = glGetUniformLocation(m_renderSHID, name.c_str()));
    if (location == -1)
    {
        std::cout << "[WARNING][SHADER] Shader name" << name << " not found in " << m_renderSHID << std::endl;
    }
    //std::cout << "Uniform cached: " << name << std::endl;
    UniformMap[name] = location;
	return location;
}

ShaderSet Shader::parseShader(const std::string filepath)
{
    std::ifstream shadeFileStream(filepath);
    PROJ_ASSERT_W_MSG(shadeFileStream.good(), "Failed to create stream for file: " + filepath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line = {};
    ShaderType type = ShaderType::NONE;
    std::string vertShader;
    std::string fragShader;

    while (getline(shadeFileStream, line))
    {
        //std::cout << line << std::endl;
        //Tag detect
        std::string tagHead = "#tag";
        if (line.substr(0, tagHead.length()).compare(tagHead) == 0)
        {
            //tag name
            std::string tName = line.substr(tagHead.length(), line.length() - tagHead.length());
            //have it auto append " " as a prefix? might be useful if i want to use other arguments
            //for now just brute force
            if (tName.compare(" Vertex") == 0)
                type = ShaderType::VERTEX;
            else if (tName.compare(" Fragment") == 0)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            switch (type)
            {
            case ShaderType::VERTEX:
                vertShader.append(line);
                vertShader.append("\n");
                break;
            case ShaderType::FRAGMENT:
                fragShader.append(line);
                fragShader.append("\n");
                break;
            default:
                std::cout << "[ERROR][SHADER][PARSER] Unknown Shader Type";
                break;
            }
        }
    }
    return ShaderSet(vertShader, fragShader);
}

//Generallized for both vertex and fragment shaders
unsigned int Shader::compileShader(unsigned int type, const std::string& source)
{
    GL_SAFE(unsigned int shdrID = glCreateShader(type)); //Creates our shader pointer
    const char* src = source.c_str(); //note: you and also just do &source[0] same thing
    GL_SAFE(glShaderSource(shdrID, 1, &src, nullptr)); //sets the Shader (null ptr to specify to use the whole thing)
    GL_SAFE(glCompileShader(shdrID));

    int returnVal = 0;
    GL_SAFE(glGetShaderiv(shdrID, GL_COMPILE_STATUS, &returnVal));

    if (returnVal == GL_FALSE)
    {
        int length;
        GL_SAFE(glGetShaderiv(shdrID, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        GL_SAFE(glGetShaderInfoLog(shdrID, length, &length, message));
        std::cout << "Failed to comple " << (type == GL_VERTEX_SHADER ? "vertex" : (type == GL_FRAGMENT_SHADER ? "fragment" : "UNKNOWN")) << " shader!" << std::endl;
        std::cout << message << std::endl;
        GL_SAFE(glDeleteShader(shdrID));

    }

    return shdrID;
}

//The strings are literally the shader source code (usually grab them from a file)
//Returns a unique id for the sharder program to use else where in the code
unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    //program is really just the id of the buffer space for the shader
    //can also be GLuint, they're the same
    GL_SAFE(unsigned int program = glCreateProgram()); //shader program being sent to the gpu
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    PROJ_ASSERT(vs != 0);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
    PROJ_ASSERT(fs != 0);

    //These are now basically assigning the program the shaders, linking the shaders, checking if it works, and 
    GL_SAFE(glAttachShader(program, vs));
    GL_SAFE(glAttachShader(program, fs));
    GL_SAFE(glLinkProgram(program));
    GL_SAFE(glValidateProgram(program));

    GL_SAFE(glDeleteShader(vs));
    GL_SAFE(glDeleteShader(fs));//you normally should detach shaders as well, but it makes debug harder with minimal performance gain

    //int* returnVal = 0;
    //glGetProgramiv(program, GL_LINK_STATUS, returnVal);
    //TO DO: Handle returns later

    return program;
}