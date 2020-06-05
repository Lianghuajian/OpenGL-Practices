//
//  Shader.hpp
//  Shaders
//
//  Created by lianghuajian on 2020/6/5.
//  Copyright © 2020 lhj. All rights reserved.
//

#ifndef Shader_hpp
#define Shader_hpp

#include <stdio.h>

#include <iostream>
#include <glad/glad.h>
#include <fstream>
#include <sstream>

class Shader
{
public:
    unsigned int ID;
    
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
    {
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            
            vShaderFile.close();
            fShaderFile.close();
            
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch(std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        
        const char* vShaderCode = vertexCode.c_str();
        
        const char* fShaderCode = fragmentCode.c_str();
        
        unsigned int vertex, fragment;
        int success;
        char infoLog[512];
        
        vertex = glad_glCreateShader(GL_VERTEX_SHADER);
        glad_glShaderSource(vertex, 1, &vShaderCode, NULL);
        glad_glCompileShader(vertex);
        glad_glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success) {
             glGetShaderInfoLog(vertex, 512, NULL, infoLog);
             std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        
        fragment = glad_glCreateShader(GL_FRAGMENT_SHADER);
        glad_glShaderSource(fragment, 1, &fShaderCode, NULL);
        glad_glCompileShader(fragment);
        glad_glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        
        ID = glad_glCreateProgram();
        glad_glAttachShader(ID, vertex);
        glad_glAttachShader(ID, fragment);
        glad_glLinkProgram(ID);
        glad_glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
        
        glad_glDeleteShader(vertex);
        glad_glDeleteShader(fragment);
    }
    
    void use()
    {
        glad_glUseProgram(ID);
    }
    
    void terminate()
    {
        glad_glDeleteProgram(ID);
    }
    
    void setBool(const std::string &name, bool value) const
    {
        glad_glUniform1i(glad_glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    
    void setInt(const std::string &name, int value) const
    {
        glad_glUniform1i(glad_glGetUniformLocation(ID, name.c_str()), value);
    }
    
    void setFloat(const std::string &name, float value) const
    {
        glad_glUniform1f(glad_glGetUniformLocation(ID, name.c_str()), value);
    }
};

#endif /* Shader_hpp */
