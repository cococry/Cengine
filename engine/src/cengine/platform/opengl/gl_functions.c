#include "gl_functions.h"

#include <Windows.h>

PFNGLCREATEVERTEXARRAYSPROC glCreateVertexArrays;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
PFNGLCREATEBUFFERSPROC glCreateBuffers;
PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLBUFFERDATAPROC glBufferData;
PFNGLBUFFERSUBDATAPROC glBufferSubData;
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
PFNGLGETSTRINGPROC glGetString;
PFNGLUNIFORM1FPROC glUniform1f;
PFNGLUNIFORM1IVPROC glUniform1iv;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv;
PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;

PFNGLDRAWARRAYSPROC glDrawArrays;

PFNGLCREATETEXTURESPROC glCreateTextures;
PFNGLTEXTURESTORAGE2DPROC glTextureStorage2D;
PFNGLTEXTUREPARAMETERIPROC glTextureParameteri;
PFNGLTEXTURESUBIMAGE2DPROC glTextureSubImage2D;
PFNGLTEXIMAGE2DPROC glTexImage2D;
PFNGLPIXELSTOREIPROC glPixelStorei;
PFNGLBINDTEXTUREPROC glBindTexture;
PFNGLTEXPARAMETERIPROC glTexParameteri;
PFNGLDELETETEXTURESPROC glDeleteTextures;
PFNGLACTIVETEXTUREPROC glActiveTexture;
PFNGLBINDTEXTUREUNITPROC glBindTextureUnit;
PFNGLGENTEXTURESPROC glGenTextures;
PFNGLENABLEPROC glEnable;
PFNGLBLENDFUNCPROC glBlendFunc;

void *load_gl_function(const char *name) {
    void *func = (void *)wglGetProcAddress(name);
    if (func == 0 ||
        (func == (void *)0x1) || (func == (void *)0x2) || (func == (void *)0x3) ||
        (func == (void *)-1)) {
        HMODULE module = LoadLibraryA("opengl32.dll");
        func = (void *)GetProcAddress(module, name);
    }
    return func;
}

void load_gl_functions() {
    glBlendFunc =
        (PFNGLBLENDFUNCPROC)load_gl_function("glBlendFunc");
    glEnable =
        (PFNGLENABLEPROC)load_gl_function("glEnable");
    glCreateVertexArrays =
        (PFNGLCREATEVERTEXARRAYSPROC)load_gl_function("glCreateVertexArrays");
    glBindVertexArray =
        (PFNGLBINDVERTEXARRAYPROC)load_gl_function("glBindVertexArray");
    glBindBuffer =
        (PFNGLBINDBUFFERPROC)load_gl_function("glBindBuffer");
    glDeleteVertexArrays =
        (PFNGLDELETEVERTEXARRAYSPROC)load_gl_function("glDeleteVertexArrays");
    glVertexAttribPointer =
        (PFNGLVERTEXATTRIBPOINTERPROC)load_gl_function("glVertexAttribPointer");
    glEnableVertexAttribArray =
        (PFNGLENABLEVERTEXATTRIBARRAYPROC)load_gl_function("glEnableVertexAttribArray");
    glCreateBuffers =
        (PFNGLCREATEBUFFERSPROC)load_gl_function("glCreateBuffers");
    glBufferData =
        (PFNGLBUFFERDATAPROC)load_gl_function("glBufferData");
    glBufferSubData =
        (PFNGLBUFFERSUBDATAPROC)load_gl_function("glBufferSubData");
    glGetString =
        (PFNGLGETSTRINGPROC)load_gl_function("glGetString");
    glDeleteBuffers =
        (PFNGLDELETEBUFFERSPROC)load_gl_function("glDeleteBuffers");
    glCreateProgram =
        (PFNGLCREATEPROGRAMPROC)load_gl_function("glCreateProgram");
    glUseProgram =
        (PFNGLUSEPROGRAMPROC)load_gl_function("glUseProgram");
    glDeleteProgram =
        (PFNGLDELETEPROGRAMPROC)load_gl_function("glDeleteProgram");
    glCreateShader =
        (PFNGLCREATESHADERPROC)load_gl_function("glCreateShader");
    glShaderSource =
        (PFNGLSHADERSOURCEPROC)load_gl_function("glShaderSource");
    glCompileShader =
        (PFNGLCOMPILESHADERPROC)load_gl_function("glCompileShader");
    glGetShaderiv =
        (PFNGLGETSHADERIVPROC)load_gl_function("glGetShaderiv");
    glGetShaderInfoLog =
        (PFNGLGETSHADERINFOLOGPROC)load_gl_function("glGetShaderInfoLog");
    glAttachShader =
        (PFNGLATTACHSHADERPROC)load_gl_function("glAttachShader");
    glGetProgramiv =
        (PFNGLGETPROGRAMIVPROC)load_gl_function("glGetProgramiv");
    glGetProgramInfoLog =
        (PFNGLGETPROGRAMINFOLOGPROC)load_gl_function("glGetProgramInfoLog");
    glDeleteShader =
        (PFNGLDELETESHADERPROC)load_gl_function("glDeleteShader");
    glLinkProgram =
        (PFNGLLINKPROGRAMPROC)load_gl_function("glLinkProgram");
    glUniform2fv =
        (PFNGLUNIFORM2FVPROC)load_gl_function("glUniform2fv");
    glUniform3fv =
        (PFNGLUNIFORM3FVPROC)load_gl_function("glUniform3fv");
    glUniform4fv =
        (PFNGLUNIFORM4FVPROC)load_gl_function("glUniform4fv");
    glUniform1i =
        (PFNGLUNIFORM1IPROC)load_gl_function("glUniform1i");
    glUniform1iv =
        (PFNGLUNIFORM1IVPROC)load_gl_function("glUniform1iv");
    glUniform1f =
        (PFNGLUNIFORM1FPROC)load_gl_function("glUniform1f");
    glGetUniformLocation =
        (PFNGLGETUNIFORMLOCATIONPROC)load_gl_function("glGetUniformLocation");
    glUniformMatrix4fv =
        (PFNGLUNIFORMMATRIX4FVPROC)load_gl_function("glUniformMatrix4fv");
    glUniformMatrix3fv =
        (PFNGLUNIFORMMATRIX3FVPROC)load_gl_function("glUniformMatrix3fv");
    glUniformMatrix2fv =
        (PFNGLUNIFORMMATRIX2FVPROC)load_gl_function("glUniformMatrix2fv");
    glCreateTextures =
        (PFNGLCREATETEXTURESPROC)load_gl_function("glCreateTextures");
    glTextureStorage2D =
        (PFNGLTEXTURESTORAGE2DPROC)load_gl_function("glTextureStorage2D");
    glTextureParameteri =
        (PFNGLTEXTUREPARAMETERIPROC)load_gl_function("glTextureParameteri");
    glTextureSubImage2D =
        (PFNGLTEXTURESUBIMAGE2DPROC)load_gl_function("glTextureSubImage2D");
    glTexImage2D =
        (PFNGLTEXIMAGE2DPROC)load_gl_function("glTexImage2D");
    glBindTexture = (PFNGLBINDTEXTUREPROC)load_gl_function("glBindTexture");
    glActiveTexture =
        (PFNGLACTIVETEXTUREPROC)load_gl_function("glActiveTexture");
    glBindTextureUnit =
        (PFNGLBINDTEXTUREUNITPROC)load_gl_function("glBindTextureUnit");
    glDrawArrays =
        (PFNGLDRAWARRAYSPROC)load_gl_function("glDrawArrays");
    glGenTextures =
        (PFNGLGENTEXTURESPROC)load_gl_function("glGenTextures");
    glTexParameteri =
        (PFNGLTEXPARAMETERIPROC)load_gl_function("glTexParameteri");
    glPixelStorei =
        (PFNGLPIXELSTOREIPROC)load_gl_function("glPixelStorei");
}