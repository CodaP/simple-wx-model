#include <iostream>
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>
#include "draw_gh_program.h"
#include "glutil.h"

double gh[ARRAY_SIZE];

static const GLchar * FragmentSource =
"#version 330 core\n"
"uniform sampler2D sampler;"
"in vec2 UV;"
"out vec3 color;"
"void main(){"
"   float offset;"
"   if(UV.x < .5){"
"       offset = .5;"
"   } else {"
"       offset = -.5;"
"   }"
"   float gpm = (texture( sampler, vec2(UV.x+offset,UV.y) ).r);"
"   color = vec3(gpm,0.0,1.0 - gpm);"
"}"
;

static const GLchar * VertexSource =
"#version 330 core\n"
"layout(location = 0) in vec3 position;"
"out vec2 UV;"
"void main(){"
"  gl_Position.xyz = position;"
"  gl_Position.w = 1.0;"
"  UV.x = position.x/2.0+0.5;"
"  UV.y = 0.5 - position.y/2.0;"
"}"
;

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
    static float gh_float[ARRAY_SIZE];
    for(int lon=0;lon<WIDTH;lon++){
        for(int lat=0;lat<HEIGHT;lat++){
            gh_float[lat*WIDTH+lon] = (gh[lat*WIDTH+lon]-min)/(max-min);
        }
    }
    glGenTextures(1, textures);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, WIDTH, HEIGHT, 0, GL_RED, GL_FLOAT, gh_float);
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

static GLuint setup_shaders(){
    GLuint program = LoadShaders(&VertexSource, &FragmentSource);
    glUseProgram(program);
    glUniform1i(glGetUniformLocation(program, "sampler"), 0);
    return program;
}

static GLuint VertexArrayID;
static GLuint program;

void init_gh(){
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    GLuint textures[1];
    GLuint vertices[1];
    program = setup_shaders();
    setup_textures(textures);
    setup_vertices(vertices);
}

void draw_gh(){
    glBindVertexArray(VertexArrayID);
    glUseProgram(program);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

