#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <Debugger.h>
#include <Shader.h>
#include <RubiksCube.h>

class Camera
{
public:
    // View and Projection
    glm::mat4 m_View = glm::mat4(1.0f);
    glm::mat4 m_Projection = glm::mat4(1.0f);
    RubiksCube &rubiksCube;

    // View matrix parameters
    glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 m_Orientation = glm::vec3(0.0f, 0.0f, -1.0f); // Forward direction
    glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);           // Up direction

    // Perspective Projection parameters
    float m_FOV = 45.0f;             // Field of View
    float m_PerspectiveNear = 0.1f;  // Near plane
    float m_PerspectiveFar = 100.0f; // Far plane

    // Projection matrix parameters
    float m_Near = 0.1f;
    float m_Far = 100.0f;
    int m_Width;
    int m_Height;

    // Orthographic Projection parameters
    float m_Left = -1.0f;
    float m_Right = 1.0f;
    float m_Bottom = -1.0f;
    float m_Top = 1.0f;

    // Prevent the camera from jumping around when first clicking left click
    double m_OldMouseX = 0.0;
    double m_OldMouseY = 0.0;
    double m_NewMouseX = 0.0;
    double m_NewMouseY = 0.0;

    // Rotation angles (Pitch and Yaw)
    float m_RotationAngleX = 0.0f; // Rotation around X-axis (up-down)
    float m_RotationAngleY = 0.0f; // Rotation around Y-axis (left-right)

    // Sensitivity for mouse input (how fast the camera rotates based on mouse movement)
    float m_RotationSensitivity = 0.1f;

    Camera(int width, int height, RubiksCube &rubiksCube): m_Width(width), m_Height(height), rubiksCube(rubiksCube) {};

    // Update Projection matrix for Orthographic mode
    void SetOrthographic(float near, float far);

    // Update Perspective matrix for Orthographic mode
    void SetPerspective(float fov, float near, float far);
    void SetPosition(glm::vec3 vec3); // Move camera back

    // Handle camera inputs
    void EnableInputs(GLFWwindow *window);
    inline glm::mat4 GetViewMatrix() const { return m_View; }
    inline glm::mat4 GetProjectionMatrix() const { return m_Projection; }

    void UpdateViewMatrix(); // Update the view matrix with rotation

    // Rotations
    void handleRKey();
    void handleLKey();
    void handleUKey();
    void handleDKey();
    void handleBKey();
    void handleFKey();
    void handleSpaceKey();
    void handleZKey();
    void handleAKey();

    // Arrows
    void handleUpArrow();
    void handleDownArrow();
    void handleLeftArrow();
    void handleRightArrow();

    // Color Picking
    void handlePKey();

    // Bounus
    void handleIKey();
    void handleOKey();
    void handleMKey();
    void handleSKey();
    void handleLPrimeKey();
    void handleRPrimeKey();
    void handleFPrimeKey();
    void handleBPrimeKey();
    void handleUPrimeKey();
    void handleDPrimeKey();
};