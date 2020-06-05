//
//  main.cpp
//  Shaders
//
//  Created by lianghuajian on 2020/6/5.
//  Copyright © 2020 lhj. All rights reserved.
//


#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <filesystem>

#include "math.h"
#include "Shader.hpp"
const int width = 800;

const int height = 600;

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 ourColor;\n"
"out vec4 vertexColor;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(aPos, 1.0);\n"
"    vertexColor = vec4(ourColor, 1.0);\n"
"}\0";

//const char *vertexShaderSource = "#version 330 core\n"
//"layout (location = 0) in vec3 aPos;\n"
//"out vec4 vertexColor;\n"
//"void main()\n"
//"{\n"
//"    gl_Position = vec4(aPos, 1.0);\n"
//"    vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n"
//"}\0";

// get color from vertexShader
const char *fragShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec4 vertexColor;\n"
"void main() { \n"
"   FragColor = vertexColor; \n"
"}\0";

// get color from uniform
//const char *fragShaderSource = "#version 330 core\n"
//"out vec4 FragColor;\n"
//"uniform vec4 ourColor;\n"
//"void main()\n"
//"{ \n"
//"   FragColor = ourColor; \n"
//"}\0";

static void framesize_changed_callback(GLFWwindow *window, int width, int height);

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

std::string get_working_path()
{
   char temp[1000];
   return ( getcwd(temp, sizeof(temp)) ? std::string( temp ) : std::string("") );
}

int main(int argc, const char * argv[]) {
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    GLFWwindow *window = glfwCreateWindow(width, height, "Draw a triangle", NULL, NULL);
    
    if (!window) {
        std::cout<<"Failed to create GLFW window"<<std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    
    glfwSetWindowSizeCallback(window, framesize_changed_callback);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    std::__fs::filesystem::current_path();
    // 创建顶点着色器
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // 着色器源码附加到着色器对象
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // 创建片元着色器
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // 初始化
    glShaderSource(fragmentShader, 1, &fragShaderSource, NULL); // 赋值
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // 创建着色器程序，链接上面的顶点着色器和片元着色器
    unsigned int shaderProgram;

    shaderProgram = glad_glCreateProgram();

    glad_glAttachShader(shaderProgram, vertexShader);
    glad_glAttachShader(shaderProgram, fragmentShader);
    glad_glLinkProgram(shaderProgram);
    glad_glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glad_glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::Programiv::Link_FAILED\n" << infoLog << std::endl;
    }

    // delete after link
    glad_glDeleteShader(vertexShader);
    glad_glDeleteShader(fragmentShader);
    
    // 注意这里要替换成shader.fs和shader.vs绝对路径，而不是下面这个
    Shader shader = Shader("/Users/lianghuajian/Desktop/Mine/selflearning/iOSpractice/OpenGL/2.Draw a triangle/Draw a triangle/shader.fs", "/Users/lianghuajian/Desktop/Mine/selflearning/iOSpractice/OpenGL/2.Draw a triangle/Draw a triangle/shader.vs");
    float vertices[] = {
        0.0,   0.5, 0.0, 1.0f, 0.0f, 0.0f,
        -0.5, -0.5, 0.0, 0.0f, 1.0f, 0.0f,
        0.5,  -0.5, 0.0, 0.0f, 0.0f, 1.0f
    };
    
    unsigned int VAO, VBO;
    glad_glGenVertexArrays(1, &VAO);
    glad_glGenBuffers(1, &VBO);
    
    glad_glBindVertexArray(VAO);
    glad_glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glad_glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glad_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glad_glEnableVertexAttribArray(0);
    glad_glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glad_glEnableVertexAttribArray(1);
    
    glfwSetKeyCallback(window, key_callback);
    
    while (!glfwWindowShouldClose(window)) {
        glad_glClearColor(0.2f, 0.3f, 0.4f, 0.1f);
        glad_glClear(GL_COLOR_BUFFER_BIT);
        
        shader.use();
        
//        glad_glUseProgram(shaderProgram);
        
//        float timeValue = glfwGetTime();
//        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
//        float redValue = (sin(timeValue) / 2.0f) + 0.3f;
//        int vertexColorLocation = glad_glGetUniformLocation(shader.ID, "ourColor");
//        glad_glUniform4f(vertexColorLocation, redValue, greenValue, 0.0f, 1.0f);
        
        glad_glBindVertexArray(VAO);
        glad_glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glad_glDeleteBuffers(1, &VAO);
    glad_glDeleteBuffers(1, &VBO);
//    glad_glDeleteProgram(shaderProgram);
    shader.terminate();
    
    glfwTerminate();
    
    return 0;
}

static void framesize_changed_callback(GLFWwindow *window, int width, int height) {
    glad_glViewport(0, 0, width, height);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}
