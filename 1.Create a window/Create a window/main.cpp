//
//  main.cpp
//  OpenGL creates window
//
//  Created by lianghuajian on 2020/5/30.
//  Copyright © 2020 lhj. All rights reserved.
//

#include <iostream>
// 下面两者顺序不能颠倒
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

static void gl_setup();

// call as window resize
static void framebuffer_size_callback(GLFWwindow *window, int width, int height);

// call as window receive input, like press 'esc'
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main(int argc, const char * argv[]) {
    
    std::cout << "Hello, World!\n";
    gl_setup();
    
    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    
    if (window == NULL) {
        std::cout << "Failed to create GFLW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);// set window as current context for the calling thread
    // glfwMakeContextCurrent调用后才能调用下面语句，loader需要知道当前context地址后（glfwGetProcAddress），才能动态定位当前OpenGL版本的函数
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    glViewport(0, 0, 800, 600);// OpenGL渲染窗口的尺寸大小，如果比window小，则内容显示不全
    
    glfwSetKeyCallback(window, key_callback);
    
    while (!glfwWindowShouldClose(window)) {
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);// 清空屏幕所用的颜色
        
        glClear(GL_COLOR_BUFFER_BIT);// 清空颜色缓冲。还可以清除GL_DEPTH_BUFFER_BIT和GL_STENCIL_BUFFER_BIT模版
        
        glfwSwapBuffers(window);// 交换颜色缓冲。双缓冲区，如果只有单缓冲区，图像可能回出现闪缩，新画面只能从上到下，从左到右像素描绘而成，而不是瞬间显示给用户
        glfwPollEvents();
    }
    
    glfwTerminate();
    
    return 0;
}

static void gl_setup() {
    glfwInit();//初始化
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

static void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}
