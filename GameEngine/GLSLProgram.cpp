#include "GLSLProgram.h"
#include "EngineErrors.h"
#include <vector>
#include <fstream>
namespace GameEngine {

	GLSLProgram::GLSLProgram() : m_numAttributes(0), m_programID(0), m_vertexShaderID(0), m_fragmentShaderID(0)
	{
	}


	GLSLProgram::~GLSLProgram()
	{
	}

	void GLSLProgram::compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath) {
		//Vertex and fragment shaders are successfully compiled.
		//Now time to link them together into a program.
		//Get a program object.
		m_programID = glCreateProgram();

		// Create the shader object and store its ID
		m_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		if (m_vertexShaderID == 0) {
			fatalError("Vertex shader failed to be created!");
		}

		// Create the shader object and store its ID
		m_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if (m_fragmentShaderID == 0) {
			fatalError("Fragment shader failed to be created!");
		}

		// Compile each shader
		compileShader(vertexShaderFilePath, m_vertexShaderID);
		compileShader(fragmentShaderFilePath, m_fragmentShaderID);
	}

	void GLSLProgram::linkShaders() {

		//Attach shaders to program
		glAttachShader(m_programID, m_vertexShaderID);
		glAttachShader(m_programID, m_fragmentShaderID);

		//Link program
		glLinkProgram(m_programID);

		GLint isLinked = 0;
		glGetProgramiv(m_programID, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &maxLength);

			// maxLength includes the NULL character
			std::vector<char> errorLog(maxLength);
			glGetProgramInfoLog(m_programID, maxLength, &maxLength, &errorLog[0]);

			// Finished using program
			glDeleteProgram(m_programID);

			// Finished using shaders
			glDeleteShader(m_vertexShaderID);
			glDeleteShader(m_fragmentShaderID);

			std::printf("%s\n", &(errorLog[0]));
			fatalError("Shaders failed to link!");
		}

		// Detach shaders after a successful link
		glDetachShader(m_programID, m_vertexShaderID);
		glDetachShader(m_programID, m_fragmentShaderID);
		glDeleteShader(m_vertexShaderID);
		glDeleteShader(m_fragmentShaderID);
	}

	void GLSLProgram::addAttribute(const std::string& attributeName) {
		glBindAttribLocation(m_programID, m_numAttributes++, attributeName.c_str());
	}

	GLint GLSLProgram::getUniformLocation(const std::string& uniformName) {
		GLint location = glGetUniformLocation(m_programID, uniformName.c_str());
		if (location == GL_INVALID_INDEX) {
			fatalError("Uniform " + uniformName + " not found in shader!");
		}
		return location;
	}

	void GLSLProgram::use() {
		glUseProgram(m_programID);
		// Enable all attributes added with addAttribute
		for (int i = 0; i < m_numAttributes; i++) {
			glEnableVertexAttribArray(i);
		}
	}

	void GLSLProgram::unuse() {
		glUseProgram(0);
		for (int i = 0; i < m_numAttributes; i++) {
			// Disable all attributes
			glDisableVertexAttribArray(i);
		}
	}

	void GLSLProgram::compileShader(const std::string& filePath, GLuint id) {

		std::ifstream shaderFile(filePath);
		if (shaderFile.fail()) {
			perror(filePath.c_str());
			fatalError("Failed to open " + filePath);
		}

		std::string fileContents = "";
		std::string line;

		while (std::getline(shaderFile, line)) {
			fileContents += line + "\n";
		}

		shaderFile.close();

		const char* contentsPtr = fileContents.c_str();

		// Tell OpenGL that fileContents is a shader file
		glShaderSource(id, 1, &contentsPtr, nullptr);

		// Compile the shader
		glCompileShader(id);

		// Check for errors
		GLint success = 0;
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);

		if (success == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

			// maxLength include the NULL character
			std::vector<char> errorLog(maxLength);
			glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

			// Finished using shader
			glDeleteShader(id);

			// Print error and exit
			std::printf("%s\n", &(errorLog[0]));
			fatalError("Shader " + filePath + " failed to compile!");
		}
	}
}