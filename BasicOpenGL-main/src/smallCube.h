#ifndef SMALLCUBE_H
#define SMALLCUBE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <array>

class SmallCube {
    
    public:
    int index ; 


    SmallCube(const glm::vec3& pos , int index);
    SmallCube() ; 

    glm::vec3 getPosition() const;
    glm::mat4 getModelMatrix() const;
    glm::mat4 getRotationMatrix() const;




    void setModelMatrix(const glm::mat4& matrix); // Setter for the model matrix
    void setRotationMatrix(const glm::mat4& matrix); // Setter for the model matrix



    private : 
    glm::mat4 modelMatrix;
    glm::mat4 RotationMatrix  ;



};


#endif

