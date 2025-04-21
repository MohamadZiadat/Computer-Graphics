#include <Camera.h>
#include <chrono>
#include <thread>
#include <algorithm>

glm::vec3 rotationCenter = glm::vec3(0, 0, 0); // Default to center of the cube

void Camera::SetOrthographic(float near, float far)
{
    m_Near = near;
    m_Far = far;

    // Rest Projection and View matrices
    m_Projection = glm::ortho(m_Left, m_Right, m_Bottom, m_Top, near, far);
    m_View = glm::lookAt(m_Position, m_Position + m_Orientation, m_Up);
}

/////////////////////
// Input Callbacks //
/////////////////////




void KeyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
    Camera* camera = (Camera*) glfwGetWindowUserPointer(window);
    if (!camera) {
        std::cout << "Warning: Camera wasn't set as the Window User Pointer! KeyCallback is skipped" << std::endl;
        return;
    }

    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        switch (key)
        {
            case GLFW_KEY_UP:
                camera->handleUpArrow();
                break;
            case GLFW_KEY_DOWN:
                camera->handleDownArrow();
                break;
            case GLFW_KEY_LEFT:
                camera->handleLeftArrow();
                break;
            case GLFW_KEY_RIGHT:
                camera->handleRightArrow();
                break;
            case GLFW_KEY_R:
                camera->handleRKey();
                break;
            case GLFW_KEY_L:
                camera->handleLKey();
                break;
            case GLFW_KEY_U:
                camera->handleUKey();
                break;
            case GLFW_KEY_D:
                camera->handleDKey();
                break;
            case GLFW_KEY_B:
                camera->handleBKey();
                break;
            case GLFW_KEY_F:
                camera->handleFKey();
                break;
            case GLFW_KEY_SPACE:
                camera->handleSpaceKey();
                break;
            case GLFW_KEY_Z:
                camera->handleZKey();
                break;
            case GLFW_KEY_A:
                camera->handleAKey();
                break;
            case GLFW_KEY_P:
                camera->handlePKey();
                break;    
            case GLFW_KEY_I:
                camera->handleIKey();
                break;
            case GLFW_KEY_O:
                camera->handleOKey();
                break;
            case GLFW_KEY_M:
                std::thread([camera]() { camera->handleMKey(); }).detach();
                break;
            case GLFW_KEY_S:
                std::thread([camera]() { camera->handleSKey(); }).detach();
                break;
            default:
                break;
        }
    }
}


void MouseButtonCallback(GLFWwindow* window, double currMouseX, double currMouseY)
{   
    Camera* camera = (Camera*) glfwGetWindowUserPointer(window);
    if (!camera) {
        std::cout << "Warning: Camera wasn't set as the Window User Pointer! KeyCallback is skipped" << std::endl;
        return;
    }


    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        std::cout << "MOUSE LEFT Click" << std::endl;
    }
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
           std::cout << "MOUSE RIGHT Click" << std::endl;

            if (camera->rubiksCube.pickingMode) {
            // Get mouse position
            double mouseX, mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);

            // Flip Y-coordinate for OpenGL origin
            int flippedY = camera->m_Height - static_cast<int>(mouseY);

            // Read the color from the back buffer
            unsigned char color_picked[4] = {0, 0, 0, 0};
            glReadPixels(static_cast<int>(mouseX), flippedY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color_picked);

            // Decode the color to get the shape ID
            int color_id = color_picked[0] ;
            int shapeID = color_id  ;

                if(shapeID < 0 ||shapeID > 26) 
                    camera->rubiksCube.selectedCube = nullptr ; 

            for (SmallCube * cube : camera->rubiksCube.smallCubes) { 
                    if(cube->index == shapeID) {
                        camera->rubiksCube.selectedCube = cube ; 
                        std :: cout << " the selceed cube is " << cube->index << std::endl ; 
                            break ; 
                    }
            }
            
            ////////////

            // Output the color and shape ID
            std::cout << "Selected Color: [R: " << static_cast<int>(color_picked[0])
                      << ", G: " << static_cast<int>(color_picked[1])
                      << ", B: " << static_cast<int>(color_picked[2]) << "]" << std::endl;
            std::cout << "Shape ID: " << shapeID << std::endl;
        
            }
     }
}




void CursorPosCallback(GLFWwindow* window, double currMouseX, double currMouseY)
{
    Camera* camera = (Camera*) glfwGetWindowUserPointer(window);
    if (!camera) {
        std::cout << "Warning: Camera wasn't set as the Window User Pointer! KeyCallback is skipped" << std::endl;
        return;
    }

        float deltaX = camera->m_OldMouseX - currMouseX;
        float deltaY = camera->m_OldMouseY - currMouseY;
        camera->m_OldMouseX = currMouseX;
        camera->m_OldMouseY = currMouseY;


        // Check if left mouse button is pressed to trigger rotation
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
        
        // Output mouse motion (for debugging)


        if(camera->rubiksCube.pickingMode) {
            
            SmallCube *   cube  = camera->rubiksCube.selectedCube ; 
            if (cube) {

                    // Calculate rotation angles based on mouse motion
                float rotationAngleY = deltaX * camera->m_RotationSensitivity;
                float rotationAngleX = deltaY * camera->m_RotationSensitivity;


                // Create rotation matrices for X and Y axes

                glm::mat4 translationToOrigin = glm::translate(-cube->getPosition()); // Translate by (-x1, -y1, -z1)

                glm::mat4 rotationMatrixX = glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngleX), glm::vec3(1.0f, 0.0f, 0.0f));
                glm::mat4 rotationMatrixY = glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngleY), glm::vec3(0.0f, 1.0f, 0.0f));

                glm::mat4 translateBack = glm::translate(cube->getPosition());  
                 

                glm::mat4 finalMatrix =  translateBack *   rotationMatrixY    *  rotationMatrixX    * translationToOrigin;  
                cube->setModelMatrix(finalMatrix * cube->getModelMatrix());  



                // Apply the rotation to the selected cube


            }
        }

        else {
            float rotationAngleY = deltaX * camera->m_RotationSensitivity;
            float rotationAngleX = deltaY * camera->m_RotationSensitivity;

            // Optional: Clamp the X axis rotation to avoid excessive tilting
            rotationAngleX = glm::clamp(rotationAngleX, -89.0f, 89.0f);

            // Precompute the rotation matrices only once
            glm::mat4 rotationMatrixX = glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngleX), glm::vec3(1.0f, 0.0f, 0.0f));
            glm::mat4 rotationMatrixY = glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngleY), glm::vec3(0.0f, 1.0f, 0.0f));

            // Combine the rotation matrices
            glm::mat4 finalRotationMatrix = rotationMatrixY * rotationMatrixX;

           //   camera->rubiksCube.RotationMatrix =  finalRotationMatrix  * camera->rubiksCube.RotationMatrix ; 



            // Apply the combined rotation matrix to all cubes in the Rubik's Cube
            for (SmallCube* cube : camera->rubiksCube.getSmallCubes()) {
                cube->setRotationMatrix(  finalRotationMatrix * cube->getRotationMatrix())  ;       
            }
        }

            
    }

  // Mouse rigth motion , when you click and mve the mouse  
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
            

        // Handle motion for the right mouse button
        
        if(camera->rubiksCube.pickingMode) {

            SmallCube *   cube  = camera->rubiksCube.selectedCube ; 

            if (cube) {

                        // Calculate translation based on mouse motion
                    float sensitivity = 0.02 *camera->m_Position.z /15  ; // Adjust translation sensitivity
                    glm::vec3 translation(
                        deltaX * sensitivity , // Horizontal mouse motion
                        -deltaY * sensitivity, // Vertical mouse motion (inverted to match screen coordinates)
                        0.0f // No translation along the Z-axis
                    );

                    

                    // Update the cube's model matrix
                    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), translation);
                    cube->setModelMatrix(translationMatrix * cube->getModelMatrix() );


            }


        }

        else {
            float sensitivity = 0.005f; // Adjust movement speed

            // Update camera position based on mouse motion
            camera->m_Position += glm::normalize(glm::cross(camera->m_Orientation, camera->m_Up)) * deltaX * sensitivity; // Left/right
            camera->m_Position += camera->m_Up * deltaY * sensitivity;                                                   // Up/down

            // Update the view matrix
            camera->UpdateViewMatrix();

        }

    
    }
}

/// SCROLL Motion
void ScrollCallback(GLFWwindow* window, double scrollOffsetX, double scrollOffsetY)
{
    Camera* camera = (Camera*) glfwGetWindowUserPointer(window);
    if (!camera) {
        std::cout << "Warning: Camera wasn't set as the Window User Pointer! ScrollCallback is skipped" << std::endl;
        return;
    }

    // Define sensitivity for scroll motion
    float sensitivity = 0.5f;

    // Update camera position along the Z-axis in the direction of the camera's orientation
    camera->m_Position += camera->m_Orientation * static_cast<float>(scrollOffsetY) * sensitivity;

    // Update the view matrix
    camera->UpdateViewMatrix();

   

    std::cout << "SCROLL Motion" << std::endl;
}

void Camera::EnableInputs(GLFWwindow* window)
{
    // Set camera as the user pointer for the window
    glfwSetWindowUserPointer(window, this);

    // Handle key inputs
    glfwSetKeyCallback(window, (void(*)(GLFWwindow *, int, int, int, int)) KeyCallback);

    // Handle cursor buttons
    glfwSetMouseButtonCallback(window, (void(*)(GLFWwindow *, int, int, int)) MouseButtonCallback);

    // Handle cursor position and inputs on motion
    glfwSetCursorPosCallback(window , (void(*)(GLFWwindow *, double, double)) CursorPosCallback);

    // Handle scroll inputs
    glfwSetScrollCallback(window, (void(*)(GLFWwindow *, double, double)) ScrollCallback);
}



void Camera::UpdateViewMatrix()
{
    // Calculate the forward and right directions after rotation
    glm::vec3 right = glm::normalize(glm::cross(m_Up, m_Orientation)); // Right vector (cross of Up and Forward)
    glm::vec3 up = glm::cross(right, m_Orientation); // Recalculate Up vector

    // Apply pitch and yaw rotations to the orientation (camera's forward vector)
    m_Orientation = glm::rotate(m_Orientation, glm::radians(m_RotationAngleX), right); // Pitch (around X-axis)
    m_Orientation = glm::rotate(m_Orientation, glm::radians(m_RotationAngleY), m_Up); // Yaw (around Y-axis)

    // Update the view matrix
    m_View = glm::lookAt(m_Position, m_Position + m_Orientation, up);
}


void Camera::SetPerspective(float fov, float near, float far)
{
    // Set the FOV, near, and far planes
    m_FOV = fov;
    m_PerspectiveNear = near;
    m_PerspectiveFar = far;

    // Calculate the aspect ratio
    float aspectRatio = static_cast<float>(m_Width) / static_cast<float>(m_Height);

    // Update the Projection matrix using glm::perspective
    m_Projection = glm::perspective(glm::radians(m_FOV), aspectRatio, m_PerspectiveNear, m_PerspectiveFar);

    // Update the View matrix
    UpdateViewMatrix();

    // Debugging output
    std::cout << "Perspective set: FOV=" << m_FOV 
              << ", Near=" << m_PerspectiveNear 
              << ", Far=" << m_PerspectiveFar 
              << ", AspectRatio=" << aspectRatio << std::endl;
}


void Camera::SetPosition(glm::vec3 vec3) { 

    this->m_Position = vec3 ; 
    UpdateViewMatrix() ; 
}


void Camera :: handleRKey() {

    std::cout << "R key pressed "<< std::endl;

    if (rubiksCube.canRotateRightWall()) 
         rubiksCube.rotateRightWall() ;
    
}

void  Camera :: handleLKey() {

    std::cout << "L key pressed " << std::endl;

    if (rubiksCube.canRotateLeftWall()) 
            rubiksCube.rotateLeftWall() ;
    
}

void  Camera :: handleUKey() {

    std::cout << "U key pressed " << std::endl;

    if (rubiksCube.canRotateUpWall()) 
            rubiksCube.rotateUpWall() ;

}


void  Camera :: handleDKey() {

    std::cout << "D key pressed " << std::endl;

    if (rubiksCube.canRotateDownWall()) 
            rubiksCube.rotateDownWall() ;

}

void  Camera :: handleBKey() {

        std::cout << "B key pressed" << std::endl;
        if (rubiksCube.canRotateBackWall()) 
            rubiksCube.rotateBackWall() ;

}

void  Camera :: handleFKey() {

        std::cout << "F key pressed" << std::endl;
        if (rubiksCube.canRotateFrontWall()) 
                rubiksCube.rotateFrontWall() ;

}

void  Camera :: handleSpaceKey() {
    std::cout << "Space key pressed - Flip Rotation Direction" << std::endl;
            rubiksCube.RotationDirection = -rubiksCube.RotationDirection  ;  
}

void  Camera :: handleZKey() {
    std::cout << "Z key pressed - Divide Rotation Angle by 2" << std::endl;
            rubiksCube.RotationAngle = std::max(rubiksCube.RotationAngle / 2, 45);
    std :: cout <<  "the new angle is " << rubiksCube.RotationAngle *  rubiksCube.RotationDirection ; 
}

void  Camera :: handleAKey() {
    std::cout << "A key pressed - Multiply Rotation Angle by 2" << std::endl;
            rubiksCube.RotationAngle = std::min(rubiksCube.RotationAngle*2, 180);
    std :: cout <<  "the new angle is " << rubiksCube.RotationAngle *  rubiksCube.RotationDirection ; 


}

void Camera::handleMKey() {
    std::cout << "M key pressed - the Mixer Will start the work .. " << std::endl; 

    // Open the file to store mixer actions
    std::ofstream mixerFile("mixer.txt");
    if (!mixerFile.is_open()) {
        std::cerr << "Error: Unable to open mixer.txt for writing. Check file permissions or directory." << std::endl;
        return;
    }

    std::cout << "File mixer.txt opened successfully." << std::endl;

    // Define possible actions and their names
    std::vector<std::pair<char, std::string>> actions = {
    {'R', "Right wall rotation (" + std::to_string(this->rubiksCube.RotationDirection * this->rubiksCube.RotationAngle) + " degrees clockwise)"},
    {'L', "Left wall rotation (" + std::to_string(this->rubiksCube.RotationDirection * this->rubiksCube.RotationAngle) + " degrees clockwise)"},
    {'U', "Up wall rotation (" + std::to_string(this->rubiksCube.RotationDirection * this->rubiksCube.RotationAngle) + " degrees clockwise)"},
    {'D', "Down wall rotation (" + std::to_string(this->rubiksCube.RotationDirection * this->rubiksCube.RotationAngle) + " degrees clockwise)"},
    {'B', "Back wall rotation (" + std::to_string(this->rubiksCube.RotationDirection * this->rubiksCube.RotationAngle) + " degrees clockwise)"},
    {'F', "Front wall rotation (" + std::to_string(this->rubiksCube.RotationDirection * this->rubiksCube.RotationAngle) + " degrees clockwise)"}
};
    // Generate random number of actions
    int actionCount = 50 + (std::rand() % 50); // Random number between 50 and 70
    std::cout << "Number of actions to perform: " << actionCount << std::endl;

    for (int i = 0; i < actionCount; ++i) {
        // Select a random action
        const auto& action = actions[std::rand() % actions.size()];

        // Perform the action (simulate pressing a key)
        switch (action.first) {
            case 'R': handleRKey(); break;
            case 'L': handleLKey(); break;
            case 'U': handleUKey(); break;
            case 'D': handleDKey(); break;
            case 'B': handleBKey(); break;
            case 'F': handleFKey(); break;
        }

        // Write the action description to the file
        mixerFile << action.second << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));


    }

    mixerFile.close();
    std::cout << "Mixer actions have been written to mixer.txt." << std::endl;
}
void Camera::handleSKey() {
    std::cout << "S key pressed - the Solver Will start the work .. " << std::endl;

    // Open the file to store solver actions
    std::ofstream solverFile("solver.txt");
    if (!solverFile.is_open()) {
        std::cerr << "Error: Unable to open solver.txt for writing. Check file permissions or directory." << std::endl;
        return;
    }

    // Open mixer file to read previous moves
    std::ifstream mixerFile("mixer.txt");
    if (!mixerFile.is_open()) {
        std::cerr << "Error: Unable to open mixer.txt for reading. Cannot solve." << std::endl;
        return;
    }

    std::vector<std::string> actions;
    std::string actionLine;

    // Read all actions into a vector
    while (std::getline(mixerFile, actionLine)) {
        actions.push_back(actionLine);
    }
    mixerFile.close();

    std::cout << "Reversing " << actions.size() << " mixer actions..." << std::endl;

    // Reverse the actions to solve the cube
    for (auto it = actions.rbegin(); it != actions.rend(); ++it) {
        std::string reverseAction = *it;

        // Determine inverse action (counter-clockwise rotation)
        if (reverseAction.find("Right wall") != std::string::npos) handleRPrimeKey();
        else if (reverseAction.find("Left wall") != std::string::npos) handleLPrimeKey();
        else if (reverseAction.find("Up wall") != std::string::npos) handleUPrimeKey();
        else if (reverseAction.find("Down wall") != std::string::npos) handleDPrimeKey();
        else if (reverseAction.find("Back wall") != std::string::npos) handleBPrimeKey();
        else if (reverseAction.find("Front wall") != std::string::npos) handleFPrimeKey();

        solverFile << "Reversed: " << reverseAction << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Small delay for visualization
    }

    solverFile.close();
    std::cout << "Solver actions have been written to solver.txt." << std::endl;
}
void Camera::handleIKey()
{
    float step = 1.0f; // Adjust step size if needed

       rotationCenter.z += step;
}
void Camera::handleOKey(){
    float step = 1.0f; // Adjust step size if needed
    rotationCenter.z -= step;

}
void Camera::handleRPrimeKey() {
    std::cout << "R' key pressed - Rotating Right wall Counterclockwise" << std::endl;
    this->rubiksCube.RotationDirection = -1;
    this->rubiksCube.rotateRightWall();
    this->rubiksCube.RotationDirection = 1;
}

void Camera::handleLPrimeKey() {
    std::cout << "L' key pressed - Rotating Left wall Counterclockwise" << std::endl;
    this->rubiksCube.RotationDirection = -1;
    this->rubiksCube.rotateLeftWall();
    this->rubiksCube.RotationDirection = 1;
}

void Camera::handleUPrimeKey() {
    std::cout << "U' key pressed - Rotating Up wall Counterclockwise" << std::endl;
    this->rubiksCube.RotationDirection = -1;
    this->rubiksCube.rotateUpWall();
    this->rubiksCube.RotationDirection = 1;
}

void Camera::handleDPrimeKey() {
    std::cout << "D' key pressed - Rotating Down wall Counterclockwise" << std::endl;
    this->rubiksCube.RotationDirection = -1;
    this->rubiksCube.rotateDownWall();
    this->rubiksCube.RotationDirection = 1;
}

void Camera::handleBPrimeKey() {
    std::cout << "B' key pressed - Rotating Back wall Counterclockwise" << std::endl;
    this->rubiksCube.RotationDirection = -1;
    this->rubiksCube.rotateBackWall();
    this->rubiksCube.RotationDirection = 1;
}

void Camera::handleFPrimeKey() {
    std::cout << "F' key pressed - Rotating Front wall Counterclockwise" << std::endl;
    this->rubiksCube.RotationDirection = -1;
    this->rubiksCube.rotateFrontWall();
    this->rubiksCube.RotationDirection = 1;
}



void Camera::handlePKey(){
std::cout << "the key p pressed" <<  std :: endl ; 
this->rubiksCube.pickingMode = !this->rubiksCube.pickingMode ;  
std :: cout <<  "the Picking mode is " << rubiksCube.pickingMode  << std:: endl ;

}


void  Camera :: handleUpArrow() {
    
    std::cout << "UP Arrow " << std::endl;
    rubiksCube.UpArrow() ; 
}

    
void  Camera :: handleDownArrow() {
    
   std::cout << "Down Arrow " << std::endl;
    rubiksCube.DownArrow() ; 

}

void Camera :: handleLeftArrow() {
    
     std::cout << "Left Arrow " << std::endl;
    rubiksCube.LeftArrow() ; 
}

void Camera :: handleRightArrow() {

    std::cout << "Right Arrow " << std::endl;
    rubiksCube.RightArrow() ; 
}