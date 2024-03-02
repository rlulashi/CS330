// Robert Lulashi
// Southern New Hampshire University
// CS 330 Final Project
// **Final output is two pillars standing on opposite ends of a platform with a cylinder in the middle and
// a light source overlooking..// 

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"Texture.h"
#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Camera.h"


#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>

const float PI = 3.14592653959f;

const unsigned int width = 800;
const unsigned int height = 800;



// Function to generate vertices for a horizontal cylinder
void GenerateCylinderVertices(GLfloat* cylinderVertices, GLuint* cylinderIndices, float radius, float height, int sectors) {
    // Center of the bottom circle
    cylinderVertices[0] = 0.05f;
    cylinderVertices[1] = -0.05f;
    cylinderVertices[2] = 0.05f;

    // Center of the top circle
    cylinderVertices[3] = 0.025f;
    cylinderVertices[4] = -0.05 + height;
    cylinderVertices[5] = 0.025f;

    // Vertices for bottom circle
    for (int i = 0; i < sectors; ++i) {
        float theta = 2 * PI * i / sectors;
        cylinderVertices[(i + 2) * 3] = radius * cos(theta);
        cylinderVertices[(i + 2) * 3 + 1] = -0.05f;
        cylinderVertices[(i + 2) * 3 + 2] = radius * sin(theta);
    }

    // Vertices for top circle
    for (int i = 0; i < sectors; ++i) {
        float theta = 2 * PI * i / sectors;
        cylinderVertices[(sectors + i + 2) * 3] = radius * cos(theta);
        cylinderVertices[(sectors + i + 2) * 3 + 1] = -0.05 + height;
        cylinderVertices[(sectors + i + 2) * 3 + 2] = radius * sin(theta);
    }

    // Indices for triangles
    for (int i = 0; i < sectors; ++i) {
        // Bottom circle
        cylinderIndices[i * 3] = 0;
        cylinderIndices[i * 3 + 1] = (i + 1) % sectors + 2;
        cylinderIndices[i * 3 + 2] = i + 2;

        // Side faces
        cylinderIndices[(i + sectors) * 6] = (i + 2) % sectors + 2;
        cylinderIndices[(i + sectors) * 6 + 1] = (i + 1) % sectors + 2;
        cylinderIndices[(i + sectors) * 6 + 2] = (i + 1) % sectors + 2 + sectors;
        cylinderIndices[(i + sectors) * 6 + 3] = (i + 1) % sectors + 2 + sectors;
        cylinderIndices[(i + sectors) * 6 + 4] = (i + 2) % sectors + 2 + sectors;
        cylinderIndices[(i + sectors) * 6 + 5] = (i + 2) % sectors + 2;
    }
}
 


// Vertices coordinates
GLfloat vertices[] =
{ //     COORDINATES     /        COLORS        /    TexCoord    /       NORMALS     //
    -1.0f, 0.0f,  1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
    -1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 1.0f,		0.0f, 1.0f, 0.0f,
     1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 1.0f,		0.0f, 1.0f, 0.0f,
     1.0f, 0.0f,  1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 0.0f,		0.0f, 1.0f, 0.0f
};

// Indices for vertices order
GLuint indices[] =
{
    0, 1, 2,
    0, 2, 3
};

GLfloat cubeVertices[] =
{ //     COORDINATES     //
    -1.0f,  0.0f,  0.1f,    	
    -1.0f,  0.75f, 0.1f,    
    -1.0f,  0.0f, -0.1f,    		
    -1.0f,  0.75f,-0.1f,   		
    -0.85f, 0.0f,  0.1f,    		
    -0.85f, 0.75f, 0.1f,   		
    -0.85f, 0.0f, -0.1f,    		
    -0.85f, 0.75f,-0.1f,   		
};

GLuint cubeIndices[] =
{
    0, 1, 2,
    0, 2, 3,
    0, 4, 7,
    0, 7, 3,
    3, 7, 6,
    3, 6, 2,
    2, 6, 5,
    2, 5, 1,
    1, 5, 4,
    1, 4, 0,
    4, 5, 6,
    4, 6, 7
};

GLfloat cube2Vertices[] =
{ //     COORDINATES     //
    1.0f,  0.0f,  0.1f,
    1.0f,  0.75f, 0.1f,
    1.0f,  0.0f, -0.1f,
    1.0f,  0.75f,-0.1f,
    0.85f, 0.0f,  0.1f,
    0.85f, 0.75f, 0.1f,
    0.85f, 0.0f, -0.1f,
    0.85f, 0.75f,-0.1f,
};

GLuint cube2Indices[] =
{
    0, 1, 2,
    0, 2, 3,
    0, 4, 7,
    0, 7, 3,
    3, 7, 6,
    3, 6, 2,
    2, 6, 5,
    2, 5, 1,
    1, 5, 4,
    1, 4, 0,
    4, 5, 6,
    4, 6, 7
};

GLfloat lightVertices[] =
{ //     COORDINATES     //
    -0.1f, -0.1f,  0.1f,
    -0.1f, -0.1f, -0.1f,
     0.1f, -0.1f, -0.1f,
     0.1f, -0.1f,  0.1f,
    -0.1f,  0.1f,  0.1f,
    -0.1f,  0.1f, -0.1f,
     0.1f,  0.1f, -0.1f,
     0.1f,  0.1f,  0.1f
};

GLuint lightIndices[] =
{
    0, 1, 2,
    0, 2, 3,
    0, 4, 7,
    0, 7, 3,
    3, 7, 6,
    3, 6, 2,
    2, 6, 5,
    2, 5, 1,
    1, 5, 4,
    1, 4, 0,
    4, 5, 6,
    4, 6, 7
};
 


int main()
{
    // Initialize GLFW
    glfwInit();

    // Tell GLFW what version of OpenGL we are using 
    // In this case we are using OpenGL 4.4
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    // Tell GLFW we are using the CORE profile
    // So that means we only have the modern functions
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a GLFWwindow object of 800 by 800 pixels, naming it "Future Deadlift Platform"
    GLFWwindow* window = glfwCreateWindow(800, 800, "Future Deadlift Platform", NULL, NULL);
    // Error check if the window fails to create
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // Introduce the window into the current context
    glfwMakeContextCurrent(window);

    //Load GLAD so it configures OpenGL
    gladLoadGL();
    // Specify the viewport of OpenGL in the Window
    // In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
    glViewport(0, 0, 800, 800);



    // Generates Shader object using shaders defualt.vert and default.frag
    Shader shaderProgram("default.vert", "default.frag");


    // Vertices and indices for the cylinder
    const int sectors = 100; // number of sectors in the cylinder
    const float radius = 0.15f;
    const float cylinderHeight = 0.5f;
    GLfloat cylinderVertices[(sectors * 2 + 2) * 3]; // top circle, bottom circle, and side vertices
    GLuint cylinderIndices[sectors * 6 + sectors * 6]; // indices for triangles

    // Generate cylinder vertices and indices
    GenerateCylinderVertices(cylinderVertices, cylinderIndices, radius, cylinderHeight, sectors);


  



    // Generates Vertex Array Object and binds it
    VAO VAO1;
    VAO1.Bind();
    // Generates Vertex Buffer Object and links it to vertices
    VBO VBO1(vertices, sizeof(vertices));
    // Generates Element Buffer Object and links it to indices
    EBO EBO1(indices, sizeof(indices));
    // Links VBO attributes such as coordinates and colors to VAO
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
    // Unbind all to prevent accidentally modifying them
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    // Generates Vertex Array Object and binds it
    VAO cylinderVAO;
    cylinderVAO.Bind();
    // Generates Vertex Buffer Object and links it to vertices
    VBO cylinderVBO(cylinderVertices, sizeof(cylinderVertices));
    // Generates Element Buffer Object and links it to indices
    EBO cylinderEBO(cylinderIndices, sizeof(cylinderIndices));
    cylinderVAO.LinkAttrib(cylinderVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

    // Unbind all to prevent accidentally modifying them
    cylinderVAO.Unbind();
    cylinderVBO.Unbind();  
    cylinderEBO.Unbind();




    VAO cubeVAO;
    cubeVAO.Bind();
    // Generates Vertex Buffer Object and links it to vertices
    VBO cubeVBO(cubeVertices, sizeof(cubeVertices));
    // Generates Element Buffer Object and links it to indices
    EBO cubeEBO(cubeIndices, sizeof(cubeIndices));
    // Links VBO attributes such as coordinates and colors to VAO
    cubeVAO.LinkAttrib(cubeVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
    // Unbind all to prevent accidentally modifying them
    cubeVAO.Unbind();
    cubeVBO.Unbind();
    cubeEBO.Unbind();


    VAO cube2VAO;
    cube2VAO.Bind();
    // Generates Vertex Buffer Object and links it to vertices
    VBO cube2VBO(cube2Vertices, sizeof(cube2Vertices));
    // Generates Element Buffer Object and links it to indices
    EBO cube2EBO(cube2Indices, sizeof(cube2Indices));
    // Links VBO attributes such as coordinates and colors to VAO
    cube2VAO.LinkAttrib(cube2VBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
    // Unbind all to prevent accidentally modifying them
    cube2VAO.Unbind();
    cube2VBO.Unbind();
    cube2EBO.Unbind();


    // Shader for light cube
    Shader lightShader("light.vert", "light.frag");
    // Generates Vertex Array Object and binds it
    VAO lightVAO;
    lightVAO.Bind();
    // Generates Vertex Buffer Object and links it to vertices
    VBO lightVBO(lightVertices, sizeof(lightVertices));
    // Generates Element Buffer Object and links it to indices
    EBO lightEBO(lightIndices, sizeof(lightIndices));
    // Links VBO attributes such as coordinates and colors to VAO
    lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
    // Unbind all to prevent accidentally modifying them
    lightVAO.Unbind();
    lightVBO.Unbind();
    lightEBO.Unbind();



    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 0.75f, 0.0f); // yellowish tint to somewhat mimic sunlight
    glm::vec3 lightPos = glm::vec3(0.5f, 1.0f, -1.0f);
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos);

    glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 pyramidModel = glm::mat4(1.0f);
    pyramidModel = glm::translate(pyramidModel, pyramidPos);


    lightShader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
    glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    shaderProgram.Activate();
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
    glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);




    // Texture
    Texture brickPng("floor.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    brickPng.texUnit(shaderProgram, "tex0", 0);
    //Texture plankSpec("barbell.png", GL_TEXTURE_2D, GL_TEXTURE1, GL_RED, GL_UNSIGNED_BYTE);
    //plankSpec.texUnit(shaderProgram, "tex1", 1);

    // Enables the Depth Buffer
    glEnable(GL_DEPTH_TEST);

    // Creates camera object
    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

    // Main while loop
    while (!glfwWindowShouldClose(window))
    {
        // Specify the color of the background
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        // Clean the back buffer and assign the new color to it
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Handles camera inputs
        camera.Inputs(window);
        // Updates and exports the camera matrix to the Vertex Shader
        camera.updateMatrix(45.0f, 0.1f, 100.0f);

        // Tell OpenGL which Shader Program we want to use
        shaderProgram.Activate();
        // Exports the camera Position to the Fragment Shader for specular lighting
        glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
        // Export the camMatrix to the Vertex Shader of the pyramid
        camera.Matrix(shaderProgram, "camMatrix");
        // Binds texture so that is appears in rendering
        brickPng.Bind();
       // plankSpec.Bind();


        // Bind the VAO so OpenGL knows to use it
        VAO1.Bind();
        // Draw primitives, number of indices, datatype of indices, index of indices
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

        cylinderVAO.Bind();
        // Draw primitives, number of indices, datatype of indices, index of indices
        glDrawElements(GL_TRIANGLES, sizeof(cylinderIndices) / sizeof(int), GL_UNSIGNED_INT, 0);
        
        // Bind the VAO so OpenGL knows to use it
        cubeVAO.Bind();
        // Draw primitives, number of indices, datatype of indices, index of indices
        glDrawElements(GL_TRIANGLES, sizeof(cubeIndices) / sizeof(int), GL_UNSIGNED_INT, 0);

        // Bind the VAO so OpenGL knows to use it
        cube2VAO.Bind();
        // Draw primitives, number of indices, datatype of indices, index of indices
        glDrawElements(GL_TRIANGLES, sizeof(cube2Indices) / sizeof(int), GL_UNSIGNED_INT, 0);


        // Tells OpenGL which Shader Program we want to use
        lightShader.Activate();
        // Export the camMatrix to the Vertex Shader of the light cube
        camera.Matrix(lightShader, "camMatrix");
        // Bind the VAO so OpenGL knows to use it
        lightVAO.Bind();
        // Draw primitives, number of indices, datatype of indices, index of indices
        glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);


        // Swap the back buffer with the front buffer
        glfwSwapBuffers(window);
        // Take care of all GLFW events
        glfwPollEvents();
    }

    // Delete all the objects we've created
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    brickPng.Delete();
   // plankSpec.Delete();
    cylinderVAO.Delete();
    cylinderVBO.Delete();
    cylinderEBO.Delete();
    cubeVAO.Delete();
    cubeVBO.Delete();
    cubeEBO.Delete();
    cube2VAO.Delete();
    cube2VBO.Delete();
    cube2EBO.Delete();
    shaderProgram.Delete();
    lightVAO.Delete();
    lightVBO.Delete();
    lightEBO.Delete();
    lightShader.Delete();
    // Delete window before ending the program
    glfwDestroyWindow(window);
    // Terminate GLFW before ending the program
    glfwTerminate();
    return 0;
}
