#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>
#include <iostream>

void glCheckError(){
    GLenum error = glGetError();
    if(error){
        std::cout << "OpenGl Error: " << error << std::endl;
        //printf("%s\n",gluErrorString(error));
        exit(1);
    }   
}

GLuint LoadShaders(const GLchar **VertexSource, const GLchar **FragmentSource){

    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Vertex Shader
    glShaderSource(VertexShaderID, 1, VertexSource, NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        printf("Error compiling vertex shader\n");
        GLchar error[1000];
        GLsizei len = 0;
        glGetShaderInfoLog(VertexShaderID, 1000, &len, error);
        printf("%s\n",error);
        exit(1);
    }

    // Compile Fragment Shader
    glShaderSource(FragmentShaderID, 1, FragmentSource, NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        printf("Error compiling fragment shader\n");
        GLchar error[1000];
        GLsizei len = 0;
        glGetShaderInfoLog(FragmentShaderID, 1000, &len, error);
        printf("%s\n",error);
        exit(1);
    }


    // Link the program
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);
    if(glGetError()){
        printf("Error Linking program\n");
        GLchar infoLog[1000];
        GLsizei len;
        glGetProgramInfoLog(ProgramID, 1000, &len, infoLog);
        printf("%s\n",infoLog);
        exit(1);
    }


    glDetachShader(ProgramID, VertexShaderID);
    glDetachShader(ProgramID, FragmentShaderID);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramID;
}
