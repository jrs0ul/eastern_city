/*   Copyright (C) 2017 by jrs0ul
*   jrs0ul@gmail.com          */

#ifndef GL_EXTENSIONS_H_
#define GL_EXTENSIONS_H_

#ifdef WIN32
    #ifdef _MSC_VER
        #include <SDL.h>
        #include <SDL_opengl.h>
    #else
        #include <SDL/SDL.h>
        #include <SDL/SDL_opengl.h>
    #endif
#else
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_opengl.h>
#endif

#ifndef __APPLE__ //OSX already has VBO and shaders
//---------------VBO----------------------------------------
extern PFNGLGENBUFFERSARBPROC               glGenBuffers;
extern PFNGLDELETEBUFFERSPROC               glDeleteBuffers;
extern PFNGLBINDBUFFERPROC                  glBindBuffer;
extern PFNGLBUFFERDATAPROC                  glBufferData;
extern PFNGLMAPBUFFERPROC                   glMapBuffer;
extern PFNGLUNMAPBUFFERPROC                 glUnmapBuffer;
//----------------ATRIBUTES--------------------------
extern PFNGLVERTEXATTRIBPOINTERPROC         glVertexAttribPointer;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC     glEnableVertexAttribArray;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC    glDisableVertexAttribArray;

//----------------GLSL---------------------------------
extern PFNGLCREATESHADERPROC                glCreateShader;
extern PFNGLSHADERSOURCEPROC                glShaderSource;
extern PFNGLCOMPILESHADERPROC               glCompileShader;
extern PFNGLGETSHADERIVPROC                 glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC            glGetShaderInfoLog;
extern PFNGLDELETESHADERPROC                glDeleteShader;

extern PFNGLCREATEPROGRAMPROC               glCreateProgram;
extern PFNGLDELETEPROGRAMPROC               glDeleteProgram;
extern PFNGLATTACHSHADERPROC                glAttachShader;
extern PFNGLLINKPROGRAMPROC                 glLinkProgram;
extern PFNGLGETPROGRAMIVPROC                glGetProgramiv;
extern PFNGLUSEPROGRAMPROC                  glUseProgram;
extern PFNGLGETUNIFORMLOCATIONPROC          glGetUniformLocation;
extern PFNGLGETATTRIBLOCATIONPROC           glGetAttribLocation;
extern PFNGLUNIFORM1FPROC                   glUniform1f;
extern PFNGLUNIFORM2FPROC                   glUniform2f;
extern PFNGLUNIFORM3FPROC                   glUniform3f;
extern PFNGLUNIFORM4FPROC                   glUniform4f;
extern PFNGLUNIFORM1IPROC                   glUniform1i;
extern PFNGLUNIFORMMATRIX4FVPROC            glUniformMatrix4fv;
#endif

extern PFNGLGETINFOLOGARBPROC               glGetInfoLogARB;
//------------------FBO--------------------------------
extern PFNGLGENFRAMEBUFFERSPROC             glGenFramebuffers;
extern PFNGLGENRENDERBUFFERSEXTPROC         glGenRenderbuffersEXT;
extern PFNGLDELETEFRAMEBUFFERSPROC          glDeleteFramebuffers;
extern PFNGLBINDFRAMEBUFFERPROC             glBindFramebuffer;
extern PFNGLBINDRENDERBUFFEREXTPROC         glBindRenderbufferEXT;
extern PFNGLRENDERBUFFERSTORAGEEXTPROC      glRenderbufferStorageEXT;
extern PFNGLFRAMEBUFFERTEXTURE2DPROC        glFramebufferTexture2D;
extern PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC   glCheckFramebufferStatusEXT;
extern PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC  glFramebufferRenderbufferEXT;
extern PFNGLDELETERENDERBUFFERSEXTPROC      glDeleteRenderbuffersEXT;

#ifndef __APPLE__
//-----------------Multitexturing------------------------------------
#ifdef WIN32 
extern PFNGLACTIVETEXTUREPROC            glActiveTexture;
#endif
#endif
extern const GLubyte * GLExtensionString;

extern void LoadExtensions();
extern void UnloadExtensions();

bool hasExtension(const char * name);

#endif //GL_EXTENSIONS_H_
