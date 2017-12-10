#pragma once
#include <string>
#include <GL/glew.h>

namespace GameEngine {

	// This class handles the compilation, linking, and usage of a GLSL shader program.
	// Reference: https://www.khronos.org/opengl/wiki/Shader_Compilation
	class GLSLProgram
	{
	public:
		GLSLProgram();
		~GLSLProgram();

		// Compiles the shaders for the GPU
		void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);
		void linkShaders();

		//Adds an attribute to our shader. SHould be called between compiling and linking.
		void addAttribute(const std::string& attributeName);

		GLint getUniformLocation(const std::string& uniformName);


		// Enable the shader
		void use();

		// Disable the shader
		void unuse();

	private:

		int m_numAttributes;

		// Compiles a single shader file
		void compileShader(const std::string& filePath, GLuint id);

		GLuint m_programID;
		GLuint m_vertexShaderID;
		GLuint m_fragmentShaderID;
	};

}