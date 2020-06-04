//
//  main.cpp
//  Draw a triangle
//
//  Created by lianghuajian on 2020/5/31.
//  Copyright © 2020 lhj. All rights reserved.
//

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char *fragShaderSource1 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()"
"{\n"
"   FragColor = vec4(1.0f, 0.1f, 0.2f, 1.0f);"
"}\0";

const char *fragShaderSource2 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()"
"{\n"
"   FragColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);"
"}\0";

const int width = 800;

const int height = 600;

static void framesize_changed_callback(GLFWwindow *window, int width, int height);

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
// pratices
static void drawAMatrix(int shaderProgram, GLFWwindow *window);
static void drawTwoTriangle(int shaderProgram, GLFWwindow *window);
static void drawTwoTriangleWithDifferentColor(int shaderProgram1, int shaderProgram2, GLFWwindow *window);
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
    unsigned int fragmentShader1;
    fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER); // 初始化
    glShaderSource(fragmentShader1, 1, &fragShaderSource1, NULL); // 赋值
    glCompileShader(fragmentShader1);
    
    unsigned int fragmentShader2;
    fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader2, 1, &fragShaderSource2, NULL);
    glad_glCompileShader(fragmentShader2);
    
    // 创建着色器程序，链接上面的顶点着色器和片元着色器
    unsigned int shaderProgram1;
    unsigned int shaderProgram2;
    shaderProgram1 = glad_glCreateProgram();
    shaderProgram2 = glad_glCreateProgram();
    glad_glAttachShader(shaderProgram1, vertexShader);
    glad_glAttachShader(shaderProgram1, fragmentShader1);
    glad_glLinkProgram(shaderProgram1);
    glad_glGetProgramiv(shaderProgram1, GL_LINK_STATUS, &success);
    if (!success) {
        glad_glGetProgramInfoLog(shaderProgram1, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::Programiv::Link_FAILED\n" << infoLog << std::endl;
    }
    
    glad_glAttachShader(shaderProgram2, vertexShader);
    glad_glAttachShader(shaderProgram2, fragmentShader2);
    glad_glLinkProgram(shaderProgram2);
    glad_glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
    if (!success) {
        glad_glGetProgramInfoLog(shaderProgram1, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::Programiv::Link_FAILED\n" << infoLog << std::endl;
    }
    
    // delete after link
    glad_glDeleteShader(vertexShader);
    glad_glDeleteShader(fragmentShader1);
    
    drawAMatrix(shaderProgram1, window);
//    drawTwoTriangle(shaderProgram1, window);
//    drawTwoTriangleWithDifferentColor(shaderProgram1 , shaderProgram2, window);
    return 0;
}

// 初始化和绘制流程
// 1.创建并绑定顶点数组对象VAO glGenVertexArrays
// 2.把顶点数组复制到VBO glad_glBindBuffer(GL_ARRAY_BUFFER, VBO);
// 3.设定顶点属性指针 glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
// 4.开始绘制 glUseProgram -> glBindVertexArray -> glDrawArrays -> glfwSwapBuffers

static void drawTwoTriangleWithDifferentColor(int shaderProgram1, int shaderProgram2, GLFWwindow *window) {
    
    float vertices1[] = {
        -0.9, -0.5, 0.0f,
        0.0, -0.5, 0.0f,
        -0.45, 0.5,0.0f
    };
    
    float vertices2[] = {
        0.0, -0.5, 0.0f,
        0.9, -0.5, 0.0f,
        0.45, 0.5,0.0f
    };
    
    unsigned int VAOs[2],VBOs[2];
    
    glad_glGenVertexArrays(2, VAOs);
    glad_glGenBuffers(2, VBOs);
    
    glad_glBindVertexArray(VAOs[0]);
    glad_glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glad_glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glad_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glad_glEnableVertexAttribArray(0);
    
    glad_glBindVertexArray(VAOs[1]);
    glad_glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glad_glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glad_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glad_glEnableVertexAttribArray(0);
    
    glad_glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
    
    while (!glfwWindowShouldClose(window)) {
        glad_glClearColor(0.2f,0.3f,0.4f,1.0f);
        glad_glClear(GL_COLOR_BUFFER_BIT);
        
        glad_glUseProgram(shaderProgram1);
        // window may change every sec
        glad_glBindVertexArray(VAOs[0]);
        // params1:图元
        // params2:顶点数组的起始索引
        // params3:打算绘制的顶点数
        glad_glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glad_glUseProgram(shaderProgram2);
        glad_glBindVertexArray(VAOs[1]);
        glad_glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glad_glDeleteBuffers(2, VAOs);
    glad_glDeleteBuffers(2, VBOs);
    glad_glDeleteProgram(shaderProgram1);
    glad_glDeleteProgram(shaderProgram2);
    
    glfwTerminate();
}

static void drawTwoTriangle(int shaderProgram, GLFWwindow *window) {
    
    float vertices1[] = {
        -0.9, -0.5, 0.0f,
        0.0, -0.5, 0.0f,
        -0.45, 0.5,0.0f
    };
    
    float vertices2[] = {
        0.0, -0.5, 0.0f,
        0.9, -0.5, 0.0f,
        0.45, 0.5,0.0f
    };
    
    unsigned int VAOs[2],VBOs[2];
    
    glad_glGenVertexArrays(2, VAOs);
    glad_glGenBuffers(2, VBOs);
    
    glad_glBindVertexArray(VAOs[0]);
    glad_glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glad_glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glad_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glad_glEnableVertexAttribArray(0);
    
    glad_glBindVertexArray(VAOs[1]);
    glad_glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glad_glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glad_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glad_glEnableVertexAttribArray(0);
    
    glad_glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
    
    while (!glfwWindowShouldClose(window)) {
        glad_glClearColor(0.2f,0.3f,0.4f,1.0f);
        glad_glClear(GL_COLOR_BUFFER_BIT);
        
        glad_glUseProgram(shaderProgram);
        // window may change every sec
        glad_glBindVertexArray(VAOs[0]);
        // params1:图元
        // params2:顶点数组的起始索引
        // params3:打算绘制的顶点数
        glad_glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glad_glBindVertexArray(VAOs[1]);
        glad_glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glad_glDeleteBuffers(2, VAOs);
    glad_glDeleteBuffers(2, VBOs);
    glad_glDeleteProgram(shaderProgram);
    
    glfwTerminate();
}

static void drawAMatrix(int shaderProgram, GLFWwindow *window) {
    // 顶点数据
    float vertices[] = {
        0.5, 0.5, 0.0f,
        0.5, -0.5, 0.0f,
        -0.5, -0.5, 0.0f,
        -0.5, 0.5, 0.0f
    };
    
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };
    
    unsigned int VAO,VBO,VEO;
    glad_glGenVertexArrays(1, &VAO);
    glad_glGenBuffers(1, &VBO);
    glad_glGenBuffers(1, &VEO);
    
    glad_glBindVertexArray(VAO);
    // 创建顶点缓冲对象管理顶点数据，一次性的发送大批数据到显卡。
    glad_glBindBuffer(GL_ARRAY_BUFFER, VBO); // 绑定缓冲对象到顶点缓冲类型GL_ARRAY_BUFFER
    // 参数1:目标缓冲的类型，GL_ARRAY_BUFFER指顶点缓冲数据
    // 参数4:希望显卡如何管理给定的数据，GL_STATIC_DRAW表示数据不会或几乎不会改变
    glad_glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //把之前定义的顶点数据复制到缓冲的内存VBO
    glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VEO);
    glad_glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // 第一个参数：0，把顶点属性传到location为0的地方，即我们上面定义的vertexShaderSource
    // 第二个参数：顶点属性的大小，上面定义的属性是vec3，由三个值组成，大小是3
    // 第三个参数：数据的类型，上面定义的属性是vec3，由三个值组成，大小是3
    // 第四个参数：是否希望数据被标准化（数据被映射到0和1之间）
    // 第五个参数：步长，由于下一组位置数据在3个float之后，我们把步长设置为3 * sizeof(float)，紧密排列
    // 第六个参数：位置数据在缓冲中起始位置的偏移量（Offset）
    glad_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // 第一个参数：顶点属性位置
    glad_glEnableVertexAttribArray(0);
    
    glad_glBindBuffer(GL_ARRAY_BUFFER,0);
    
    glad_glBindVertexArray(0);
    
    glfwSetKeyCallback(window, key_callback);
    
    glad_glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    while (!glfwWindowShouldClose(window)) {
        glad_glClearColor(0.2f,0.3f,0.4f,1.0f);
        glad_glClear(GL_COLOR_BUFFER_BIT);
        
        glad_glUseProgram(shaderProgram);
        // window may change every sec
        glad_glBindVertexArray(VAO);
        // params1:图元
        // params2:顶点数组的起始索引
        // params3:打算绘制的顶点数
//        glad_glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glad_glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glad_glDeleteBuffers(1, &VAO);
    glad_glDeleteBuffers(1, &VBO);
    glad_glDeleteBuffers(1, &VEO);
    glad_glDeleteProgram(shaderProgram);
    
    glfwTerminate();
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void framesize_changed_callback(GLFWwindow *window, int width, int height) {
    glad_glViewport(0, 0, width, height);
}
