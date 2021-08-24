#pragma once
#include <string>
#include <unordered_map>

struct ShaderSet
{
    ShaderSet(std::string inVS, std::string inFS)
    {
        vertShader = inVS;
        fragShader = inFS;
    }
    std::string vertShader;
    std::string fragShader;
};

class Shader
{
private:
    std::string m_filepath; // for debug
    unsigned int m_renderSHID;
    std::unordered_map<std::string, int> UniformMap; //Caching doesn't appear to make a difference?

    //Hmm what if I added a vector of shaders and their types and have the prgram select???
    //nvm. what's in the Shader sets are the strings and the vs and fs in the create func are best deleted afterwards to save space

    //some kind of caching system for the Uniform????(he says it like caeshing)
    //probably gonna be use remembering what locations are associated with which names
public:
    Shader(const std::string& filepath);
    ~Shader();

    void bind() const;
    static void unbind();

    //sets uniforms
    void setUniform1i(const std::string& name, int i0);
    void setUniform1f(const std::string& name, float v0);
    void setUniform2f(const std::string& name, float v0, float v1);
    void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

    void setUniformMatrix4f(const std::string& name, float* Mat, unsigned char rowMaj_GLBool);

private:
    int getUniformLocation(const std::string& name);

    ShaderSet parseShader(const std::string filepath);
    unsigned int compileShader(unsigned int type, const std::string& source);
    unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);
};