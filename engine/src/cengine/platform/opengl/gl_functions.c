#include "gl_functions.h"

#include <Windows.h>

PFNGLCREATEVERTEXARRAYSPROC glCreateVertexArrays;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
PFNGLCREATEBUFFERSPROC glCreateBuffers;
PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLBUFFERDATAPROC glBufferData;
PFNGLDELETEBUFFERSPROC glDeleteBuffers;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;

PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLDELETEPROGRAMPROC glDeleteProgram;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLGETPROGRAMIVPROC glGetProgramiv;
PFNGLCREATESHADERPROC glCreateShader;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLGETSHADERIVPROC glGetShaderiv;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLDELETESHADERPROC glDeleteShader;
PFNGLUNIFORM2FVPROC glUniform2fv;
PFNGLUNIFORM3FVPROC glUniform3fv;
PFNGLUNIFORM4FVPROC glUniform4fv;
PFNGLUNIFORM1IPROC glUniform1i;
PFNGLUNIFORM1FPROC glUniform1f;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv;
PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;

PFNGLCREATETEXTURESPROC glCreateTextures;
PFNGLTEXTURESTORAGE2DPROC glTextureStorage2D;
PFNGLTEXTUREPARAMETERIPROC glTextureParameteri;
PFNGLTEXTURESUBIMAGE2DPROC glTextureSubImage2D;
PFNGLBINDTEXTUREPROC glBindTexture;
PFNGLDELETETEXTURESPROC glDeleteTextures;
PFNGLACTIVETEXTUREPROC glActiveTexture;
PFNGLBINDTEXTUREUNITPROC glBindTextureUnit;

void load_gl_functions() {
    glCreateVertexArrays =
        (PFNGLCREATEVERTEXARRAYSPROC)wglGetProcAddress("glCreateVertexArrays");
    glBindVertexArray =
        (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray");
    glBindBuffer =
        (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
    glDeleteVertexArrays =
        (PFNGLDELETEVERTEXARRAYSPROC)wglGetProcAddress("glDeleteVertexArrays");
    glVertexAttribPointer =
        (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
    glEnableVertexAttribArray =
        (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
    glCreateBuffers =
        (PFNGLCREATEBUFFERSPROC)wglGetProcAddress("glCreateBuffers");
    glBufferData =
        (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
    glDeleteBuffers =
        (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");
    glCreateProgram =
        (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
    glUseProgram =
        (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
    glDeleteProgram =
        (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");
    glCreateShader =
        (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
    glShaderSource =
        (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
    glCompileShader =
        (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
    glGetShaderiv =
        (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
    glGetShaderInfoLog =
        (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
    glAttachShader =
        (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
    glGetProgramiv =
        (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
    glGetProgramInfoLog =
        (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");
    glDeleteShader =
        (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
    glLinkProgram =
        (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
    glUniform2fv =
        (PFNGLUNIFORM2FVPROC)wglGetProcAddress("glUniform2fv");
    glUniform3fv =
        (PFNGLUNIFORM3FVPROC)wglGetProcAddress("glUniform3fv");
    glUniform4fv =
        (PFNGLUNIFORM4FVPROC)wglGetProcAddress("glUniform4fv");
    glUniform1i =
        (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");
    glUniform1f =
        (PFNGLUNIFORM1FPROC)wglGetProcAddress("glUniform1f");
    glGetUniformLocation =
        (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
    glUniformMatrix4fv =
        (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
    glUniformMatrix3fv =
        (PFNGLUNIFORMMATRIX3FVPROC)wglGetProcAddress("glUniformMatrix3fv");
    glUniformMatrix2fv =
        (PFNGLUNIFORMMATRIX2FVPROC)wglGetProcAddress("glUniformMatrix2fv");
    glCreateTextures =
        (PFNGLCREATETEXTURESPROC)wglGetProcAddress("glCreateTextures");
    glTextureStorage2D =
        (PFNGLTEXTURESTORAGE2DPROC)wglGetProcAddress("glTextureStorage2D");
    glTextureParameteri =
        (PFNGLTEXTUREPARAMETERIPROC)wglGetProcAddress("glTextureParameteri");
    glTextureSubImage2D =
        (PFNGLTEXTURESUBIMAGE2DPROC)wglGetProcAddress("glTextureSubImage2D");
    glBindTexture =
        (PFNGLBINDTEXTUREPROC)wglGetProcAddress("glBindTexture");
    glDeleteTextures =
        (PFNGLDELETETEXTURESPROC)wglGetProcAddress("glDeleteTextures");
    glActiveTexture =
        (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");
    glBindTextureUnit =
        (PFNGLBINDTEXTUREUNITPROC)wglGetProcAddress("glBindTextureUnit");
}