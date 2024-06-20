

#include<string>
#include"Camera.h"
#include<glad/glad.h>
#include<stb/stb_image.h>
#include"shaderClass.h"
#include <filesystem>
//Potrzebne do operacji na plikach
namespace fs = std::filesystem;

//Rozmiary okna
const unsigned int width = 1200;
const unsigned int height = 1200;

//Wierzcholki Cubemapy
float skyboxVertices[] =
{
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f
};

//Indeksy dla Cubemapy
unsigned int skyboxIndices[] =
{
    // Prawo
    1, 2, 6,
    6, 5, 1,
    // Lewo
    0, 4, 7,
    7, 3, 0,
    // Gora
    4, 5, 6,
    6, 7, 4,
    // Dol
    0, 3, 2,
    2, 1, 0,
    // Tyl
    0, 1, 5,
    5, 4, 0,
    // Przod
    3, 7, 6,
    6, 2, 3
};


//Szescian na srodku, ktory dziala jak lustro
float cubeVertices[] = {
    // Pozycje         //wektory normalne
    -0.1f, -0.1f, -0.1f,  0.0f,  0.0f, -1.0f, // Tylna sciana
     0.1f, -0.1f, -0.1f,  0.0f,  0.0f, -1.0f,
     0.1f,  0.1f, -0.1f,  0.0f,  0.0f, -1.0f,
    -0.1f,  0.1f, -0.1f,  0.0f,  0.0f, -1.0f,

    -0.1f, -0.1f,  0.1f,  0.0f,  0.0f,  1.0f, // Przednia sciana
     0.1f, -0.1f,  0.1f,  0.0f,  0.0f,  1.0f,
     0.1f,  0.1f,  0.1f,  0.0f,  0.0f,  1.0f,
    -0.1f,  0.1f,  0.1f,  0.0f,  0.0f,  1.0f,

    -0.1f, -0.1f, -0.1f, -1.0f,  0.0f,  0.0f, // Lewa sciana
    -0.1f,  0.1f, -0.1f, -1.0f,  0.0f,  0.0f,
    -0.1f,  0.1f,  0.1f, -1.0f,  0.0f,  0.0f,
    -0.1f, -0.1f,  0.1f, -1.0f,  0.0f,  0.0f,

     0.1f, -0.1f, -0.1f,  1.0f,  0.0f,  0.0f, // Prawa sciana
     0.1f,  0.1f, -0.1f,  1.0f,  0.0f,  0.0f,
     0.1f,  0.1f,  0.1f,  1.0f,  0.0f,  0.0f,
     0.1f, -0.1f,  0.1f,  1.0f,  0.0f,  0.0f,

    -0.1f, -0.1f, -0.1f,  0.0f, -1.0f,  0.0f, // Dolna sciana
     0.1f, -0.1f, -0.1f,  0.0f, -1.0f,  0.0f,
     0.1f, -0.1f,  0.1f,  0.0f, -1.0f,  0.0f,
    -0.1f, -0.1f,  0.1f,  0.0f, -1.0f,  0.0f,

    -0.1f,  0.1f, -0.1f,  0.0f,  1.0f,  0.0f, // gorna sciana
     0.1f,  0.1f, -0.1f,  0.0f,  1.0f,  0.0f,
     0.1f,  0.1f,  0.1f,  0.0f,  1.0f,  0.0f,
    -0.1f,  0.1f,  0.1f,  0.0f,  1.0f,  0.0f,
};


unsigned int cubeIndices[] = {
    0, 1, 2, 2, 3, 0,    // Tylna sciana
    4, 5, 6, 6, 7, 4,    // Przednia sciana
    8, 9, 10, 10, 11, 8, // Lewa sciana
    12, 13, 14, 14, 15, 12, // Prawa sciana
    16, 17, 18, 18, 19, 16, // Dolna sciana
    20, 21, 22, 22, 23, 20  // gorna sciana
};


int main()
{
    // Inicjalizacja GLFW i ustawienie wersji OpenGL
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "Grafika Projekt ", NULL, NULL);
    glfwMakeContextCurrent(window);

    gladLoadGL();
    glViewport(0, 0, width, height);

    // Inicjalizacja shaderow dla cubemapy i szescianu
    Shader skyboxShader("skybox.vert", "skybox.frag");
    Shader reflectShader("reflect.vert", "reflect.frag");

    skyboxShader.Activate();
    glUniform1i(glGetUniformLocation(skyboxShader.ID, "skybox"), 0);

    //Wlaczenie glebi
    glEnable(GL_DEPTH_TEST);
    //Renderujemy tylko widoczne obiekty
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW);
    //Tworzenie kamery i ustalenie jej pozycji
    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 0.5f));

    

    // Inicjalizacja VAO(Vertex Array Object), VBO(Vertex Buffer Object), i EBO(Element Buffer Object) dla cubemapy
    unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glGenBuffers(1, &skyboxEBO);
    //powiazujemy VAO
    glBindVertexArray(skyboxVAO);
    //powiazujemy VBO i przesylamy dane o wierzcholkach
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    //powiazujemy VEO i przesylamy dane o indeksach
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Pobieramy zdjecia, z ktorych ma sie skladac cubemapa
    std::string parentDir = (fs::current_path().fs::path::parent_path()).string();
    std::string facesCubemap[6] =
    {
        parentDir + "/GrafikaProejkt/skybox/right.jpg",
        parentDir + "/GrafikaProejkt/skybox/left.jpg",
        parentDir + "/GrafikaProejkt/skybox/top.jpg",
        parentDir + "/GrafikaProejkt/skybox/bottom.jpg",
        parentDir + "/GrafikaProejkt/skybox/front.jpg",
        parentDir + "/GrafikaProejkt/skybox/back.jpg"
    };
    // Tworzenie tekstury dla cubemapy
    unsigned int cubemapTexture;
    glGenTextures(1, &cubemapTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    //Liniowa filtracja
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //CLAMP_TO_EDGE - przycinamy wspolrzedne tekstury do krawedzi
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    for (unsigned int i = 0; i < 6; i++)
    {
        int width, height, nrChannels;
        unsigned char* data = stbi_load(facesCubemap[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            //Blokuje odwracanie przy ladowaniu
            stbi_set_flip_vertically_on_load(false);
            //przekazujemy dane do cubemapy
            glTexImage2D
            (
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, //Ktora sciana
                0, //Poziom
                GL_RGB, //Format
                width, //szerokosc tekstury
                height, //wysokosc tekstury
                0, //Granica
                GL_RGB, //Format
                GL_UNSIGNED_BYTE, //Typ danych
                data //pobrane dane
            );
            stbi_image_free(data);
        }
    }

    // Inicjalizacja VAO, VBO, i EBO dla szescianu
    unsigned int cubeVAO, cubeVBO, cubeEBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);
    //powiazujemy VAO
    glBindVertexArray(cubeVAO);
    //powiazujemy VBO i przesylamy dane o wierzcholkach
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    //powiazujemy VEO i przesylamy dane o indeksach
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    while (!glfwWindowShouldClose(window))
    {
        // Czyscimy bufory
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //Kontrola ruchu kamery
        camera.Inputs(window);

        // Rysowanie cubemapy
        //Funkcja glebokosci dla Cubemapy ustawiona na mniej lub rowne
        glDepthFunc(GL_LEQUAL);
        //Rysowane beda widoczne sciany
        glEnable(GL_CULL_FACE);
        //Aktywacja shadera
        skyboxShader.Activate();
        //Ustawiamy macierze, oraz przesylamy o nich informacje do shadera
        glm::mat4 view = glm::lookAt(camera.Position, camera.Position + camera.Orientation, camera.Up);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 10.0f);
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        //Powiazujemy VAO
        glBindVertexArray(skyboxVAO);
        //Aktywacja Tekstury
        glActiveTexture(GL_TEXTURE0);
        //Powiazujemy teksture z cubemapa i ja rysujemy
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        

        // Rysowanie  szescianu
        //Culling wylaczony - inaczej szescian ma niewidzialne sciany
        glDisable(GL_CULL_FACE);
        //Aktywacja Shadera
        reflectShader.Activate();
        //Ustawiamy macierze, oraz przesylamy o nich informacje do shadera
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(reflectShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(reflectShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(reflectShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniform3fv(glGetUniformLocation(reflectShader.ID, "cameraPos"), 1, glm::value_ptr(camera.Position));
        //Powiazujemy VAO
        glBindVertexArray(cubeVAO);
        //Aktuwujemy teksture
        glActiveTexture(GL_TEXTURE0);
        //Powiazujemy i rysujemy szescian
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glEnable(GL_CULL_FACE);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //Na koniec programu usuwamy wszystko bufory i shadery
    reflectShader.Delete();
    skyboxShader.Delete();
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVBO);
    glDeleteBuffers(1, &skyboxEBO);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}