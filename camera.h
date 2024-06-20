#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

//Includy wymagane do operacji na wektorach i macierzach, zarzadzania ruchem.
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include"shaderClass.h"

//Obiektem klasy kamera bedziemy mogli sie poruszac
class Camera
{
public:
    //wektory kamery
    glm::vec3 Position;
    glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 cameraMatrix = glm::mat4(1.0f);

    bool firstClick = true;
    // Rozdzielczosc okna
    int width;
    int height;
    //predkosc poruszania
    float speed = 0.001f;
    //czulosc myszy
    float sensitivity = 100.0f;
    //Konstruktor 
    Camera(int width, int height, glm::vec3 position);
    //Aktualizacja macierzy kamery
    void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
    //Przeslanie Macierzy do shadera
    void Matrix(Shader& shader, const char* uniform);
    //Obsluga ruchu
    void Inputs(GLFWwindow* window);
};

#endif
