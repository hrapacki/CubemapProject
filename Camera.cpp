#include "Camera.h"

//Konstruktor, wymagane: wymiary okna i pozycja kamery.
Camera::Camera(int width, int height, glm::vec3 position)
{
    this->width = width;
    this->height = height;
    this->Position = position;
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
    glm::mat4 view = glm::lookAt(Position, Position + Orientation, Up);
    glm::mat4 projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);
    cameraMatrix = projection * view;
}

void Camera::Matrix(Shader& shader, const char* uniform)
{
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}
//Kontrola ruchu kamery
void Camera::Inputs(GLFWwindow* window)
{
    //Ruch WSAD
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        Position += speed * Orientation;//PRZoD
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        Position += speed * -glm::normalize(glm::cross(Orientation, Up));//LEWO
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        Position += speed * -Orientation;//TYl
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        Position += speed * glm::normalize(glm::cross(Orientation, Up));//PRAWO

    //Rotacja kamery przy u¿yciu lewego przycisku myszy
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        //Kursor staje sie niewidoczny
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        static bool firstClick = true;
        //Poprawa ruchu kamery przy kliknieciu myszka
        if (firstClick)
        {
            //ustawiamy kursor na srodek by kamera nie wykonywala naglych skokow
            glfwSetCursorPos(window, (width / 2), (height / 2));
            firstClick = false;
        }

        //dostaje pozycje myszy
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        //Liczymy ro¿nice polo¿enia od srodka
        float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
        float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

        //Obrot wektora orientacji
        Orientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));
        Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

        //ustawiamy kursor na srodku
        glfwSetCursorPos(window, (width / 2), (height / 2));
    }
    else
    {
        //kursor znowu widoczny
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        static bool firstClick = true;
        firstClick = true;
    }
}
