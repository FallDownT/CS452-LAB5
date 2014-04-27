//Example code used from http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_06

#include "Angel.h"
#include <iostream>
#include <cstdlib>
#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>
#include <cstdio>
#include <iostream>

using namespace std;
typedef Angel::vec4 point4;

// Model and view matrices uniform location
GLuint  mMatrix, program;
mat4 model;

GLuint vaoID,vboID,eboID, vbo_cube_texcoords;
GLuint texture_id;
GLint uniform_mytexture;
GLint attribute_texcoord;

// Create camera view variables
point4 at( 0.0, 0.0, 0.0, 1.0 );
point4 eye( 0.0, 0.0, 2.0, 1.0 );
vec4   up( 0.0, 1.0, 0.0, 0.0 );

GLfloat size=1;

GLfloat vertexarray[]={
	size,size,-size,
	size,-size,-size,
	-size,-size,-size,
	-size,size,-size,
	size,size,size,
	size,-size,size,
	-size,-size,size,
	-size,size,size
};

GLfloat cube_texcoords[2*4*6] = {
    0.0, 0.0,
    0.0, 1.0,
    1.0, 1.0,
    1.0, 0.0,

    0.0, 0.0,
    0.0, 1.0,
    1.0, 1.0,
    1.0, 0.0,

    0.0, 0.0,
    0.0, 1.0,
    1.0, 1.0,
    1.0, 0.0,

    0.0, 0.0,
    0.0, 1.0,
    1.0, 1.0,
    1.0, 0.0,

    0.0, 0.0,
    0.0, 1.0,
    1.0, 1.0,
    1.0, 0.0,

    0.0, 0.0,
    0.0, 1.0,
    1.0, 1.0,
    1.0, 0.0,
  };
											
GLubyte elems[]={
	7,3,4,0,1,3,2,
	7,6,4,5,1,6,2,1
};

// -----------------------------------------------------------------------

void init(){

  GLubyte textureData[]={
    0xFF,0x33,0x7b,	0xFF,0xfa,0x55,
    0x00,0x11,0x00,	0x6f,0x40,0xc1
  };

  glGenVertexArrays(1,&vaoID);
  glBindVertexArray(vaoID);

  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, // target
	       0,  // level, 0 = base, no minimap,
	       GL_RGB, // internalformat
	       10,  // width
	       10,  // height
	       0,  // border, always 0 in OpenGL ES
	       GL_RGB,  // format
	       GL_UNSIGNED_BYTE, // type
	       textureData);


  glGenBuffers(1, &vboID);
  GLuint in_position = glGetAttribLocation( program, "in_position" );
  glBindBuffer(GL_ARRAY_BUFFER,vboID);
  glBufferData(GL_ARRAY_BUFFER,sizeof(vertexarray),vertexarray,GL_STATIC_DRAW);
  glVertexAttribPointer(in_position,3,GL_FLOAT,GL_FALSE,0,(void*)0);

  glGenBuffers(1, &vbo_cube_texcoords);
  GLuint texcoord = glGetAttribLocation( program, "texcoord" );
  glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_texcoords);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube_texcoords), cube_texcoords, GL_STATIC_DRAW);
  glVertexAttribPointer(texcoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
  
  glGenBuffers(1,&eboID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,eboID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(elems),elems,GL_STATIC_DRAW);

  program=InitShader( "vertexshader.glsl", "fragmentshader.glsl" );
  glEnableVertexAttribArray(in_position);
  glEnableVertexAttribArray(texcoord);
    
  mMatrix = glGetUniformLocation(program,"modelMatrix");
    
  glEnable( GL_DEPTH_TEST );
    
  glClearColor( 0.0, 0.0, 1.0, 1.0 ); // black background 
}

// -----------------------------------------------------------------------

void display(SDL_Window* screen){
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  mat4 view = LookAt( eye, at, up );

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  uniform_mytexture = glGetUniformLocation(program, "texture");
  glUniform1i(uniform_mytexture, 0);

  // Draw elements of vaoID
  glUniformMatrix4fv( mMatrix, 1, GL_TRUE, model );
  glDrawElements( GL_TRIANGLE_FAN,sizeof(elems),GL_UNSIGNED_BYTE,0 );
  glFlush();
  SDL_GL_SwapWindow(screen);
}

// -----------------------------------------------------------------------

void input(SDL_Window* screen){

  SDL_Event event;

  while (SDL_PollEvent(&event)){//Handling the keyboard
    switch (event.type){
    case SDL_QUIT:exit(0);break;
    case SDL_KEYDOWN:
      switch(event.key.keysym.sym){
      case SDLK_ESCAPE:
		exit(EXIT_SUCCESS);
      case SDLK_w:
		model = model * Translate(0.0,1.0,0.0);
		break;
	  case SDLK_s:
		model = model * Translate(0.0,-1.0,0.0);
		break;
	  case SDLK_d:
		model = model * Translate(1.0,0.0,0.0);
		break;
	  case SDLK_a:
		model = model * Translate(-1.0,0.0,0.0);
		break;
      case SDLK_e:
		model = model * Scale(-1.0,-1.0,-1.0);
		break;
      //case SDLK_q:scalar-=.1f;break;
      }
    }
  }
}

// -----------------------------------------------------------------------

int main(int argc, char **argv){

  //SDL window and context management
  SDL_Window *window;

  if(SDL_Init(SDL_INIT_VIDEO)<0){//initilizes the SDL video subsystem
    fprintf(stderr,"Unable to create window: %s\n", SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);//die on error
  }

  //create window
  window = SDL_CreateWindow(
    "Lab # 5", //Window title
    SDL_WINDOWPOS_UNDEFINED, //initial x position
    SDL_WINDOWPOS_UNDEFINED, //initial y position
    500,	//width, in pixels
    500,	//height, in pixels
    SDL_WINDOW_OPENGL	//flags to be had
  );

  //check window creation
  if(window==NULL){
    fprintf(stderr,"Unable to create window: %s\n",SDL_GetError());
  }


  //creates opengl context associated with the window
  SDL_GLContext glcontext=SDL_GL_CreateContext(window);

  //initializes glew
  glewExperimental=GL_TRUE;
  if(glewInit()){
    fprintf(stderr, "Unable to initalize GLEW");
    exit(EXIT_FAILURE);
  }
  
  init();

  while(true){
    input(window);//keyboard controls
    display(window);//displaying
  }

  SDL_GL_DeleteContext(glcontext);
  SDL_DestroyWindow(window);
  SDL_Quit();
 
  return EXIT_SUCCESS;
}

