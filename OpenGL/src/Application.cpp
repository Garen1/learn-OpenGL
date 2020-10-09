#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    //opengl 3.3, setting
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    //Must position after opengl context
    glewInit();

    //OpenGL version
    std::cout << glGetString(GL_VERSION) << std::endl;

    {
    //vertex buffer
    float positions[] = {
        -0.5f, -0.5f, 0.0f, 0.0f,//0 bot left
         0.5f, -0.5f, 1.0f, 0.0f,//1 bot right
         0.5f,  0.5f, 1.0f, 1.0f,//2 top right
        -0.5f,  0.5f, 0.0f, 1.0f //3 top left
         
    };

    //index buffer
    unsigned int indices[] = {
        0,1,2,
        0,2,3,
    };

    //vertex buffer is bind to vetex array
    VertexArray va;
    VertexBuffer vb(positions, 4 * 4 * sizeof(float));//#vertex * #floats pervertex * size
    
    //vertex attributes, layout of data, links buffer with vao
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    //index buffer
    IndexBuffer ib(indices, 6);

    Shader shader("res/shaders/Basic.shader");
    shader.Bind();
    //uniform must be positioned after shader is bound
    shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

    Texture texture("res/textures/teemo.jpg");
    texture.Bind();
    shader.setUniform1i("u_Texture", 0);//have to match texture slot 0-32

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    //unbind buffers
    va.Unbind();
    shader.Unbind();
    vb.Unbind();
    ib.Unbind();
    
    Renderer renderrer;

    //flashing effect   
    float red = 0.0f;
    float increment = 0.05f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderrer.Clear();

        shader.Bind();
        shader.SetUniform4f("u_Color", red, 0.3f, 0.8f, 1.0f);

        renderrer.Draw(va, ib, shader);
        
        if (red > 1.0f) increment = -0.05f;
        else if (red < 0.0f) increment = 0.05f;

        red += increment;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    }//personized scope

    glfwTerminate();
    return 0;
}