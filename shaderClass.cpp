#include"shaderClass.h"

//Zwracamy zawartosc pliku jako string, potrzebne do shaderow
std::string get_file_contents(const char* filename)
{
	//Otwieramy plik w trybie binarnym i czytamy zawartosc
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

// Konstruktor, potrzebne 2 shadery, .vert i .frag
Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	//Odczytujemy oba pliki
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	//zmieniamy je na tablice znakow
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	//Tworzymy 2 Shadery na podstawie otrzymanych danych
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	//Tworzymy program Shadera
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	//Usuwamy shadery
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}
//Aktywacja i Deaktywacja shaderow, 
void Shader::Activate()
{
	glUseProgram(ID);
}

void Shader::Delete()
{
	glDeleteProgram(ID);
}
