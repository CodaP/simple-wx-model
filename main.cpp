#define GLFW_INCLUDE_GLCOREARB
#include <iostream>
#include <GLFW/glfw3.h>
#include <eccodes.h>
#include <cstdint>
#include <assert.h>

const int WIDTH = 361*4;
const int HEIGHT = 180*4;
const int ARRAY_SIZE = WIDTH*HEIGHT;

static double gh[ARRAY_SIZE];

static void glCheckError(){
    GLenum error = glGetError();
    if(error){
        std::cout << "OpenGl Error: " << error << std::endl;
        //printf("%s\n",gluErrorString(error));
        exit(1);
    }
}


const GLchar * FragmentSource =
"#version 330 core\n"
"uniform sampler2D sampler;"
"in vec2 UV;"
"out vec3 color;"
"void main(){"
"   float gpm = (texture( sampler, UV ).r);"
"   color = vec3(gpm,gpm,gpm);"
"}"
;

const GLchar * VertexSource =
"#version 330 core\n"
"layout(location = 0) in vec3 position;"
"out vec2 UV;"
"void main(){"
"  gl_Position.xyz = position;"
"  gl_Position.w = 1.0;"
"  UV.x = position.x/2.0+0.5;"
"  UV.y = position.y/2.0+0.5;"
"}"
;


GLuint LoadShaders(){

    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Vertex Shader
    glShaderSource(VertexShaderID, 1, &VertexSource, NULL);
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
    glShaderSource(FragmentShaderID, 1, &FragmentSource, NULL);
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

static void setup_textures(GLuint textures[1]){
    float min = 100000000;
    float max = 0;
    for(int i=0;i<ARRAY_SIZE;i++){
        if((gh[i] < min) && (gh[i] != 0)){
            min = gh[i];
        }
        if(gh[i] > max){
            max = gh[i];
        }
    }
    printf("min,max: %f, %f\n",min,max);
    static float gh_float[361*4*180*4];
    for(int i=0;i<ARRAY_SIZE;i++){
        gh_float[i] = (gh[i]-min)/(max-min);
    }
    glGenTextures(1, textures);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, HEIGHT, WIDTH, 0, GL_RED, GL_FLOAT, gh_float);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glCheckError();
    std::cout << "set-up textures" << std::endl;
}

static void setup_vertices(GLuint vertex_buffers[1]){
    static const GLfloat g_vertex_buffer_data[] = { 
       -1.0f, -1.0f, 0.0f,
       1.0f, -1.0f, 0.0f,
       1.0f,  1.0f, 0.0f,
       -1.0f, -1.0f, 0.0f,
       1.0f,  1.0f, 0.0f,
       -1.0f, 1.0f, 0.0f
    };
    glGenBuffers(1,vertex_buffers);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glCheckError();
    std::cout << "buffered-data" << std::endl;
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffers[0]);
    glVertexAttribPointer(
       0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
       3,                  // size
       GL_FLOAT,           // type
       GL_FALSE,           // normalized?
       0,                  // stride
       (void*)0            // array buffer offset
    );
    glCheckError();
    std::cout << "set-up vertices" << std::endl;
}

static void read_grib_file(){
    FILE * f = fopen("gfs.t18z.pgrb2.0p25.anl","r");
    if(f == NULL){
        fprintf(stderr, "Error: opening grib file\n");
        return;
    }
    int error;
    char value[5000];
    size_t vlen=5000;
    grib_handle * gid;
    while((gid = codes_grib_handle_new_from_file(NULL, f, &error))!=NULL){;
        if(error != CODES_SUCCESS){
            fprintf(stderr, "Error: opening grib handle\n");
            fclose(f);
            return;
        }

        bzero(value,5000);
        if( codes_is_defined(gid, "shortName") == 0 ){
            printf("No shortName for this\n");
        }
        else{
            vlen=5000;
            if(codes_get_string(gid, "shortName",value,&vlen) != 0){
                printf("Error reading\n");
            }
            else{
                if(strcmp("gh", value) == 0){
                    long level;
                    codes_get_long(gid, "level", &level);
                    if(level == 500){
                        size_t len = sizeof(gh);
                        assert(codes_is_defined(gid, "values"));
                        if(codes_get_double_array(gid, "values", gh, &len)){
                            perror("fdsa");
                        }
                        codes_handle_delete(gid);
                        return;
                    }
                }
            }
        }
        codes_handle_delete(gid);
    }
}

static void setup_shaders(){
    GLuint program = LoadShaders();
    glUseProgram(program);
    glUniform1i(glGetUniformLocation(program, "sampler"), 0);
}

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

int main(void) {
    read_grib_file();
    if (!glfwInit()) {
        // Initialization failed
        std::cerr << "Init Failed" << std::endl;
        return 1;
    }
    glfwSetErrorCallback(glfw_error_callback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(640, 480, "Simple Wx Model", NULL, NULL);
    if (!window)
    {
        // Window or OpenGL context creation failed
        std::cerr << "Window failed" << std::endl;
        return 1;
    }
    glfwMakeContextCurrent(window);
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
    GLuint textures[1];
    GLuint vertices[1];
    setup_shaders();
    setup_textures(textures);
    setup_vertices(vertices);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glfwSwapBuffers(window);
    while (!glfwWindowShouldClose(window))
    {
        // Keep running
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
