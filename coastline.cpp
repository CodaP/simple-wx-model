#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>
#include "coastline_data.h"
#include "glutil.h"
#include <iostream>

static const GLchar * VertexSource = 
"#version 330 core\n"
"layout(location = 0) in vec2 lonlat;"
"void main(){"
"   gl_Position.x = lonlat.x/180.0;"
"   gl_Position.y = lonlat.y/90.0;"
"   gl_Position.z = 0.0;"
"   gl_Position.w = 1.0;"
"}"
;

static const GLchar * FragmentSource =
"#version 330 core\n"
"out vec3 color;"
"void main(){"
"   color = vec3(1.0,0.0,1.0);"
"}"
;


static GLuint setup_coast_buffer(){
    GLuint coast_buffer;
    glGenBuffers(1,&coast_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, coast_buffer);
    GLsizei size = 0;
    for(uint8_t i=0;i<COAST_POINTS_SIZE;i++){
        size += sizeof(float)*coast_point_lengths[i]*2;
    }   
    glBufferData(GL_ARRAY_BUFFER, size, coast_points, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glCheckError();
    glBindBuffer(GL_ARRAY_BUFFER, coast_buffer);
    glVertexAttribPointer(
       0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
       2,                  // size
       GL_FLOAT,           // type
       GL_FALSE,           // normalized?
       0,                  // stride
       (void*)0            // array buffer offset
    );  
    glCheckError();
    return coast_buffer;
}

static GLuint VertexArrayID;
static GLuint program;

void init_coast(){
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    setup_coast_buffer();
    program = LoadShaders(&VertexSource, &FragmentSource);
    glUseProgram(program);
}

void draw_coast(){
    glBindVertexArray(VertexArrayID);
    glUseProgram(program);
    uint32_t offset = 0;
    for(uint32_t i=0;i<COAST_POINTS_SIZE;i++){
        const uint32_t len = coast_point_lengths[i];
        glDrawArrays(GL_LINE_STRIP, offset, len);
        offset += len;
    }
}

