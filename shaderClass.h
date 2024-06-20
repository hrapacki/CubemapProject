#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

//Klasa do zarzadzania shaderami
class Shader
{
public:
	//ID do referencji
	GLuint ID;
	// Konstruktor
	Shader(const char* vertexFile, const char* fragmentFile);

	void Activate();
	void Delete();
};


#endif