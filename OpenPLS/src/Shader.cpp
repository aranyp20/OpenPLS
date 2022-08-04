
#include "Shader.h"




Shader::Shader(const std::string& filepath) : ID(0)
{
    ShaderProgramSource source = ParseShader(filepath);
    ID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
    glDeleteProgram(ID);
}

void Shader::Bind() const
{
    glUseProgram(ID);
}


//Simple uniform set-ups
void Shader::SetUniform(const std::string& name, const float f)
{
    glUniform1f(getUniformLocation(name), f);
}

void Shader::SetUniform(const std::string& name, const vec4& v)
{
    glUniform4f(getUniformLocation(name), v.x, v.y, v.z, v.w);
}

void Shader::SetUniform(const std::string& name, const vec3& v)
{
    glUniform3f(getUniformLocation(name), v.x, v.y, v.z);
}

void Shader::SetUniform(const std::string& name, const mat4& m)
{
    
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_TRUE, m);

}

//Complex uniform set-ups 
void Shader::SetUniform(const std::string& name, const Material& material)
{
    SetUniform(name + ".kd", material.kd);
    SetUniform(name + ".ks", material.ks);
    SetUniform(name + ".ka", material.ka);
    SetUniform(name + ".shininess", material.shininess);
}

void Shader::SetUniform(const std::string& name, const Light& light)
{
    SetUniform(name + ".La", light.La);
    SetUniform(name + ".Le", light.Le);
    SetUniform(name + ".wLightPos", light.wLightPos);
}

unsigned int Shader::getUniformLocation(const std::string& name)
{
    unsigned int location = glGetUniformLocation(ID, name.c_str());
    if (location < 0)std::cout << "Uniform not found!" << std::endl;
    return location;
}


unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

ShaderProgramSource Shader::ParseShader(const std::string& filePath)
{
    std::ifstream stream(filePath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType shaderType = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                shaderType = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                shaderType = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)shaderType] << line << '\n';
        }
    }



    return { ss[0].str(),ss[1].str() };
}



unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    if (!CompileErrorHandler(id))
    {
        type == GL_VERTEX_SHADER ? (std::cout << "(Vertex)" << std::endl) : (std::cout << "(Fragment)" << std::endl);
        return 0;
    }


    return id;
}

bool Shader::CompileErrorHandler(unsigned int shaderID) const
{
    int result;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(sizeof(char) * length);
        glGetShaderInfoLog(shaderID, length, &length, message);
        std::cout  << " Shader compile error: " << message << std::endl;
        glDeleteShader(shaderID);
      
        return false;
    }
    return true;
}
