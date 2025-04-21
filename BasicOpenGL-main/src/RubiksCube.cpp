#include "RubiksCube.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <GLFW/glfw3.h>
#include <Camera.h>
#include <thread>  // Required for sleep in animation
#include <chrono>

const int size = 3;


RubiksCube::RubiksCube() : locks{false, false, false, false, false, false}, Sensitivity(1.0f), RotationAngle(90), RotationDirection(1)
{
    generateSmallCubes(); 
}

RubiksCube::~RubiksCube()
{
    for (SmallCube *cube : smallCubes)
        delete cube;
}

void RubiksCube::generateSmallCubes()
{

    smallCubes.clear();

    int index = 0;
    const double offset = (size - 1) / 2.0; // Offset to handle negative indices
    for (double x = -offset; x <= offset; x += 1.0)
    {
        for (double y = -offset; y <= offset; y += 1.0)
        {
            for (double z = -offset; z <= offset; z += 1.0)
            {
                glm::vec3 position(x, y, z);
                SmallCube *smallCube = new SmallCube(position, index);

                // Update the model matrix for the cube
                glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position);
                smallCube->setModelMatrix(modelMatrix);

                // Add cube to smallCubes vector
                smallCubes.push_back(smallCube);

            // if(size ==3&& index==size*size*size/2){
            //     this->centerCube = smallCube;
            // }
            //  else
            if (index==size*size*size/2 && size % 2 != 0)
             {
                 this->centerCube = new SmallCube(glm::vec3(0,0,0),size*size*size/2);
             }

                index++;
            }
        }
    }
}

std::vector<SmallCube *> RubiksCube::getSmallCubes()
{
    return this->smallCubes;
}

glm::vec3 RubiksCube::getPosition()
{

    return this->centerCube->getPosition();
}

void RubiksCube::render(Shader &shader, VertexArray &va, IndexBuffer &ib, glm::mat4 proj, glm::mat4 view, GLFWwindow *window)
{
    // -------- Normal Rendering Pass (Visible) --------
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (SmallCube *cube : smallCubes)
    {
        glm::mat4 model = cube->getRotationMatrix() * cube->getModelMatrix();
        glm::mat4 mvp = proj * view * model;

        shader.Bind();
        glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        shader.SetPickingMode(false);
        shader.SetUniform4f("u_Color", color);
        shader.SetUniformMat4f("u_MVP", mvp);

        va.Bind();
        ib.Bind();
        GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));

        shader.Unbind();
    }

    glfwSwapBuffers(window);

    if (pickingMode)
    {
        // -------- Picking Pass (Invisible) --------
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (SmallCube *cube : smallCubes)
        {
            glm::mat4 model = cube->getRotationMatrix() * cube->getModelMatrix();
            glm::mat4 mvp = proj * view * model;

            shader.Bind();
            glm::vec3 uniqueColor = glm::vec3(cube->index, cube->index, cube->index);
            glm::vec4 pickingColor = glm::vec4(uniqueColor / 255.0f, 1.0f);
            shader.SetPickingMode(true);
            shader.SetUniform4f("u_Color", pickingColor);
            shader.SetUniformMat4f("u_MVP", mvp);

            va.Bind();
            ib.Bind();
            GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));

            shader.Unbind();
        }

        // Ensure picking happens here
        glFlush();
        glFinish();
    }
}

void RubiksCube::rotateRightWall()
{

    std::cout << "Rotating Right Wall by " << (RotationAngle * RotationDirection) << " degrees\n";
    glm::vec3 centerPosition;
    if (size == 3)
        centerPosition =  glm::vec3(-1,0,0);
    else if(size%2!=0)
    centerPosition=glm::vec3(-((size - 1) / 2.0f),0,0);
    else
        centerPosition = glm::vec3(-((size - 1) / 2.0f), -0, -0);



    for (size_t i = 0; i < smallCubes.size(); ++i)
    {
        SmallCube *cube = smallCubes[i];

        if (std::abs(cube->getPosition().x - centerPosition.x) < 0.001f)
        {

            glm::mat4 translationToOrigin = glm::translate(glm::mat4(1.0f), -centerPosition);
            float effectiveRotation = glm::radians((float)RotationAngle * glm::sign(RotationDirection));
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), effectiveRotation, glm::vec3(1.0f, 0.0f, 0.0f));
            glm::mat4 translateBack = glm::translate(glm::mat4(1.0f), +centerPosition);

            glm::mat4 finalMatrix = translateBack * rotationMatrix * translationToOrigin;
            cube->setModelMatrix(finalMatrix * cube->getModelMatrix());
        }
        
        }

    if (std::abs(RotationAngle) == 45)
    {
        this->locks[0] = !this->locks[0];
    }
}

void RubiksCube::rotateLeftWall()
{

    std::cout << "Rotating Left Wall by " << (RotationAngle * RotationDirection) << " degrees\n";

    // Get the position of the centerCube to keep the wall's rotation relative to it
    glm::vec3 centerPosition;
    // Get the position of the centerCube to keep the wall's rotation relative to it
    if (size == 3)
        centerPosition =  glm::vec3(1,0,0);
    else if(size%2!=0)
    centerPosition=glm::vec3(((size - 1) / 2.0f),0,0);
    else
        centerPosition = glm::vec3(((size - 1) / 2.0f), -0, -0);


    for (size_t i = 0; i < smallCubes.size(); i++)
    {
        SmallCube *cube = smallCubes[i];

        if (std::abs(cube->getPosition().x - centerPosition.x) < 0.001f)
        { 

            glm::mat4 translationToOrigin = glm::translate(glm::mat4(1.0f), -centerPosition); // Translate by (-x1, -y1, -z1)
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians((float)RotationAngle * RotationDirection), glm::vec3(1.0f, 0.0f, 0.0f));
            glm::mat4 translateBack = glm::translate(glm::mat4(1.0f), +centerPosition);

            glm::mat4 finalMatrix = translateBack * rotationMatrix * translationToOrigin;
            cube->setModelMatrix(finalMatrix * cube->getModelMatrix());
        }
    }

    if (std::abs(RotationAngle) == 45)
    {
        this->locks[1] = !this->locks[1];
    }
}

void RubiksCube::rotateUpWall()
{
     glm::vec3 centerPosition;
    if (size == 3)
        centerPosition = glm::vec3(0,-1,0);
    else if (size!=3 && size % 2 != 0)
        centerPosition = glm::vec3(0,-((size - 1) / 2.0f),0);
    else
        centerPosition = glm::vec3(0, -((size - 1) / 2.0f), 0);
    for (size_t i = 0; i < smallCubes.size(); i++)
    {
        SmallCube *cube = smallCubes[i];

        if (std::abs(cube->getPosition().y - centerPosition.y) < 0.001f)
        {

            glm::mat4 translationToOrigin = glm::translate(glm::mat4(1.0f), -centerPosition);
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians((float)RotationAngle * RotationDirection), glm::vec3(0.0f, 1.0f, 0.0f));
            glm::mat4 translateBack = glm::translate(glm::mat4(1.0f), +centerPosition);

            glm::mat4 finalMatrix = translateBack * rotationMatrix * translationToOrigin;
            cube->setModelMatrix(finalMatrix * cube->getModelMatrix());
        }
    }

    // Locking condition check
    if (std::abs(RotationAngle) == 45)
    {
        this->locks[2] = !this->locks[2];
    }
}

void RubiksCube::rotateDownWall()
{
    std::cout << "Rotating Down Wall by " << (RotationAngle * RotationDirection) << " degrees\n";
 glm::vec3 centerPosition;
    if (size == 3)
        centerPosition = glm::vec3(0,1,0);
    else if (size!=3 && size % 2 != 0)
        centerPosition = glm::vec3(0,((size - 1) / 2.0f),0);
    else
        centerPosition = glm::vec3(0, ((size - 1) / 2.0f), -0);
    for (size_t i = 0; i < smallCubes.size() ; i++)
    {
        SmallCube *cube = smallCubes[i];

        if (std::abs(cube->getPosition().y - centerPosition.y) < 0.001f)
        { 

            glm::mat4 translationToOrigin = glm::translate(glm::mat4(1.0f), -centerPosition);
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians((float)RotationAngle * RotationDirection), glm::vec3(0.0f, 1.0f, 0.0f));
            glm::mat4 translateBack = glm::translate(glm::mat4(1.0f), +centerPosition);

            glm::mat4 finalMatrix = translateBack * rotationMatrix * translationToOrigin;
            cube->setModelMatrix(finalMatrix * cube->getModelMatrix());
        }
    }

    // Locking condition check
    if (std::abs(RotationAngle) == 45)
    {
        this->locks[3] = !this->locks[3];
    }
}

void RubiksCube::rotateBackWall()
{
    std::cout << "Rotating Back Wall by " << (RotationAngle * RotationDirection) << " degrees\n";
    
    glm::vec3 centerPosition;
    if (size!=3 && size % 2 != 0)
        centerPosition = glm::vec3(0,0,-((size - 1) / 2.0f));
    else if( size == 3)
    centerPosition = glm::vec3(0,0,-1);
    else
        centerPosition = glm::vec3(0, 0, -((size - 1) / 2.0f));

    for (size_t i = 0; i < smallCubes.size(); i++)
    {
        SmallCube *cube = smallCubes[i];

        if (std::abs(cube->getPosition().z - centerPosition.z) < 0.001f)
        {

            glm::mat4 translationToOrigin = glm::translate(glm::mat4(1.0f), -centerPosition);
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians((float)RotationAngle * RotationDirection), glm::vec3(0.0f, 0.0f, 1.0f));
            glm::mat4 translateBack = glm::translate(glm::mat4(1.0f), +centerPosition);

            glm::mat4 finalMatrix = translateBack * rotationMatrix * translationToOrigin;
            cube->setModelMatrix(finalMatrix * cube->getModelMatrix());
        }
    }

   

    // Locking condition check
    if (std::abs(RotationAngle) == 45)
    {
        this->locks[4] = !this->locks[4];
    }
}

void RubiksCube::rotateFrontWall()
{
    std::cout << "Rotating Front Wall by " << (RotationAngle * RotationDirection) << " degrees\n";

    glm::vec3 centerPosition;
    if (size!=3 && size % 2 != 0)
        centerPosition = glm::vec3(0,0,((size - 1) / 2.0f));
    else if( size == 3)
    centerPosition = glm::vec3(0,0,1);

    else
        centerPosition = glm::vec3(0, 0, ((size - 1) / 2.0f));

    for (size_t i = 0; i < smallCubes.size(); i++)
    {
        SmallCube *cube = smallCubes[i];

        // Check if the cube is part of the down wall (relative to the centerCube)
        if (std::abs(cube->getPosition().z - centerPosition.z) < 0.001f)
        {

            glm::mat4 translationToOrigin = glm::translate(glm::mat4(1.0f), -centerPosition);
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians((float)RotationAngle * RotationDirection), glm::vec3(0.0f, 0.0f, 1.0f));
            glm::mat4 translateBack = glm::translate(glm::mat4(1.0f), +centerPosition);

            glm::mat4 finalMatrix = translateBack * rotationMatrix * translationToOrigin;
            cube->setModelMatrix(finalMatrix * cube->getModelMatrix());
        }
    }
   

    // Locking condition check
    if (std::abs(RotationAngle) == 45)
    {
        this->locks[5] = !this->locks[5];
    }
}

// Locks :

bool RubiksCube::canRotateRightWall()
{
    return !(locks[2] || locks[3] || locks[4] || locks[5]); // All must be false
}

bool RubiksCube::canRotateLeftWall()
{
    return !(locks[2] || locks[3] || locks[4] || locks[5]); // Same condition as RightWall
}

bool RubiksCube::canRotateUpWall()
{
    return !(locks[0] || locks[1] || locks[4] || locks[5]); // All must be false
}

bool RubiksCube::canRotateDownWall()
{
    return !(locks[0] || locks[1] || locks[4] || locks[5]); // Fix inconsistent condition
}

bool RubiksCube::canRotateBackWall()
{
    return !(locks[0] || locks[1] || locks[2] || locks[3]); // All must be false
}

bool RubiksCube::canRotateFrontWall()
{
    return !(locks[0] || locks[1] || locks[2] || locks[3]); // All must be false
}

void RubiksCube::UpArrow()
{

    for (SmallCube *cube : this->smallCubes)
    {
        // Create a translation matrix for moving up
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -this->Sensitivity, 0.0f));

        // Apply the translation to the model matrix
        glm::mat4 updatedModelMatrix = translationMatrix * cube->getModelMatrix();

        // Set the updated model matrix
        cube->setModelMatrix(updatedModelMatrix);
    }
}

void RubiksCube::DownArrow()
{

    for (SmallCube *cube : this->smallCubes)
    {
        // Create a translation matrix for moving up
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, this->Sensitivity, 0.0f));
        std::cout << "Sensitivity value: " << this->Sensitivity << std::endl;

        // Apply the translation to the model matrix
        glm::mat4 updatedModelMatrix = translationMatrix * cube->getModelMatrix();

        // Set the updated model matrix
        cube->setModelMatrix(updatedModelMatrix);
    }
}

void RubiksCube::RightArrow()
{

    for (SmallCube *cube : this->smallCubes)
    {
        // Create a translation matrix for moving up
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-(this->Sensitivity), 0.0f, 0.0f));
        std::cout << "Sensitivity value: " << this->Sensitivity << std::endl;

        // Apply the translation to the model matrix
        glm::mat4 updatedModelMatrix = translationMatrix * cube->getModelMatrix();

        // Set the updated model matrix
        cube->setModelMatrix(updatedModelMatrix);
    }
}

void RubiksCube::LeftArrow()
{

    for (SmallCube *cube : this->smallCubes)
    {
        // Create a translation matrix for moving up
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3((this->Sensitivity), 0.0f, 0.0f));
        std::cout << "Sensitivity value: " << this->Sensitivity << std::endl;

        // Apply the translation to the model matrix
        glm::mat4 updatedModelMatrix = translationMatrix * cube->getModelMatrix();

        // Set the updated model matrix
        cube->setModelMatrix(updatedModelMatrix);
    }
}
