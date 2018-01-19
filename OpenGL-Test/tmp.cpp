//
//  main.cpp
//  OpenGL-Test
//
//  Created by James Walsh on 14/11/2017.
//  Copyright © 2017 James Walsh. All rights reserved.
//

#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <vector>
#include "SDL2/SDL.h"
#include "SDL2_ttf/SDL_ttf.h"
#include <glew.h>
#include <OpenGL/gl3.h>

using namespace std;

TTF_Font *font;                                         // font reference
SDL_Window *win = NULL;                                 // window reference
SDL_Renderer *renderer = NULL;                          // renderer reference
SDL_GLContext context;                                  // context reference

GLuint LoadShaderPair() {
//    string vertexFilePath = "TextShader.vertexshader";
//    string fragmentFilePath = "TextShader.fragmentshader";
    
    string vertexFilePath = "test.vertexshader";
    string fragmentFilePath = "test.fragmentshader";
    
    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    
    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertexFilePath, std::ios::in);
    if(VertexShaderStream.is_open())
    {
        std::string Line = "";
        while(getline(VertexShaderStream, Line))
            VertexShaderCode += "\n" + Line;
        VertexShaderStream.close();
    }
    else
    {
        cerr << "Could not load vertex sharer" << endl;
    }
    
    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragmentFilePath, std::ios::in);
    if(FragmentShaderStream.is_open()){
        std::string Line = "";
        while(getline(FragmentShaderStream, Line))
            FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close();
    }
    else
    {
        cerr << "Could not load fragment sharer" << endl;
    }
    
    GLint Result = GL_FALSE;
    int InfoLogLength;
    
    // Compile Vertex Shader
    cout << "Compiling shader: " << vertexFilePath << endl;
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);
    
    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> VertexShaderErrorMessage(10000);
    glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
    fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);
    
    // Compile Fragment Shader
    cout << "Compiling shader: " << fragmentFilePath << endl;
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);
    
    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> FragmentShaderErrorMessage(10000);
    glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
    fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);
    
    // Link the program
    fprintf(stdout, "Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);
    
    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> ProgramErrorMessage( std::max(InfoLogLength, int(1)) );
    glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);
    
    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
    
    return ProgramID;
}

void setupWindow () {
    int posX = 0, posY = 0, width = 320, height = 240;  // position and dimensions of the window

    // Initialize SDL and set SDL context variables
    SDL_Init(SDL_INIT_VIDEO);
//    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
//    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

    // Initialize SDL TTF
    TTF_Init();
    
    // Create the window and context using SDL
    win = SDL_CreateWindow("Test Text Rendering", posX, posY, width, height, (SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE));
    context = SDL_GL_CreateContext(win);
    
    //Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if( glewError != GLEW_OK ) {
        printf( "Error initializing GLEW! %s\n", glewGetErrorString( glewError ) );
    }
    
    // set the viewport of the window
    glViewport(0, 0, (float)width, (float)height);
    
    // set open gl render variables
    glEnable(GL_DEPTH_TEST);        // Enable depth test
    glDepthFunc(GL_LESS);           // Accept fragment if it closer to the camera than the former one
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    // Dont know what this does!!!
    // But nothing works without it
    // Didnt need it b4
    // BD 11 Oct 2017
    // Generate and bind a new vertex array object
    
    // create the renderer with SDL
    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    
    // load the font with SDL TTF
    font = TTF_OpenFont("/Library/Fonts/Arial.ttf", 10);
    
    // print some diagnostic info
    cout << "OpenGL version: "  << glGetString(GL_VERSION) << endl;
    cout << "OpenGL GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
    cout << "OpenGL renderer: "  << glGetString(GL_RENDERER) << endl;
}

void printText () {
    SDL_Texture *textTexture = NULL;  // texture reference
    SDL_Surface *textSurface = NULL;  // surface reference
    
    // load the text shaders
//    GLuint programID = LoadShaderPair();
//    glUseProgram(programID);
    
    // create the color for the text
    SDL_Color color={100,100,100};
    
    // set the text on the surface
    if((textSurface=TTF_RenderText_Solid(font,"Hello World!",color)) == NULL){
        fprintf(stderr, "Failed to render the text: %s\n",SDL_GetError());
    }
    // render the surface to the texture
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    // copy the texture to the window
    SDL_Rect renderQuad = {20,20,textSurface->w,textSurface->h};
    SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);
    SDL_RenderPresent(renderer);
    
    // free the surface once the text has been rendered to the texture
    SDL_FreeSurface(textSurface);
}

int main_old(int argc, const char * argv[]) {
    setupWindow();
    
    while (1) {
        // check for quit signal
        SDL_Event e;
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                break;
            }
        }
        
        // clear the window
        SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear(renderer);
    
        printText();
        
        
    
        // render the current window
//        SDL_RenderPresent(renderer);
//        SDL_Delay( 5000 );
    }
    
    // cleanup once the quit signal has been received
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    
    SDL_Quit();
    return 0;
}
