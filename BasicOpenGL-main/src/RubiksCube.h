#ifndef RUBIKSCUBE_H
#define RUBIKSCUBE_H

#include <vector>
#include "smallCube.h"
#include <glm/glm.hpp>
#include <Shader.h>
#include <VertexArray.h>
#include <IndexBuffer.h>
#include <GLFW/glfw3.h>

class RubiksCube
{

public:
    int RotationDirection; // global varibale for rotation direction
    int RotationAngle;     // global variable for rotation
    float Sensitivity;     //  for arrow pressing
    bool pickingMode;

    RubiksCube();  // Constructor
    ~RubiksCube(); // Destructor

    void initializeCubes();
    void generateSmallCubes(); // Generate all the small cubes
    void render(Shader &shader, VertexArray &va, IndexBuffer &ib, glm::mat4 proj, glm::mat4 view, GLFWwindow *window);

    bool locks[6];
    std::vector<SmallCube *> smallCubes; // Store all the small cubes in the Rubik's Cube
    SmallCube *centerCube;
    SmallCube *selectedCube;

    // Rotaions
    void rotateRightWall();
    void rotateLeftWall();
    void rotateUpWall();
    void rotateDownWall();
    void rotateBackWall();
    void rotateFrontWall();

    // locks
    bool canRotateRightWall();
    bool canRotateLeftWall();
    bool canRotateUpWall();
    bool canRotateDownWall();
    bool canRotateBackWall();
    bool canRotateFrontWall();

    // arows
    void UpArrow();
    void DownArrow();
    void LeftArrow();
    void RightArrow();

    //  getters
    glm ::vec3 getPosition();
    std::vector<SmallCube *> getSmallCubes();

private:
};

#endif
