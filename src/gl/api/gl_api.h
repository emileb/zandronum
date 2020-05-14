#if defined(__unix__) || defined(__APPLE__)
#ifndef __ANDROID__
#define APIENTRY
#define APIENTRYP *
#endif
#endif // __unix__ || __APPLE__

#ifndef __EXTERN
#define __EXTERN extern
#endif

__EXTERN PFNGLBLENDEQUATIONPROC glBlendEquation;

// ARB_SHADER_OBJECTS
__EXTERN PFNGLDELETESHADERPROC glDeleteShader;
__EXTERN PFNGLDELETEPROGRAMPROC glDeleteProgram;
__EXTERN PFNGLDETACHSHADERPROC glDetachShader;
__EXTERN PFNGLCREATESHADERPROC glCreateShader;
__EXTERN PFNGLSHADERSOURCEPROC glShaderSource;
__EXTERN PFNGLCOMPILESHADERPROC glCompileShader;
__EXTERN PFNGLCREATEPROGRAMPROC glCreateProgram;
__EXTERN PFNGLATTACHSHADERPROC glAttachShader;
__EXTERN PFNGLLINKPROGRAMPROC glLinkProgram;
__EXTERN PFNGLUSEPROGRAMPROC glUseProgram;
__EXTERN PFNGLVALIDATEPROGRAMPROC glValidateProgram;
__EXTERN PFNGLVERTEXATTRIB1FPROC glVertexAttrib1f;
__EXTERN PFNGLVERTEXATTRIB2FPROC glVertexAttrib2f;
__EXTERN PFNGLVERTEXATTRIB4FPROC glVertexAttrib4f;
__EXTERN PFNGLVERTEXATTRIB2FVPROC glVertexAttrib2fv;
__EXTERN PFNGLVERTEXATTRIB3FVPROC glVertexAttrib3fv;
__EXTERN PFNGLVERTEXATTRIB4FVPROC glVertexAttrib4fv;
__EXTERN PFNGLVERTEXATTRIB4UBVPROC glVertexAttrib4ubv;
__EXTERN PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
__EXTERN PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation;

__EXTERN PFNGLUNIFORM1FPROC glUniform1f;
__EXTERN PFNGLUNIFORM2FPROC glUniform2f;
__EXTERN PFNGLUNIFORM3FPROC glUniform3f;
__EXTERN PFNGLUNIFORM4FPROC glUniform4f;
__EXTERN PFNGLUNIFORM1IPROC glUniform1i;
__EXTERN PFNGLUNIFORM2IPROC glUniform2i;
__EXTERN PFNGLUNIFORM3IPROC glUniform3i;
__EXTERN PFNGLUNIFORM4IPROC glUniform4i;
__EXTERN PFNGLUNIFORM1FVPROC glUniform1fv;
__EXTERN PFNGLUNIFORM2FVPROC glUniform2fv;
__EXTERN PFNGLUNIFORM3FVPROC glUniform3fv;
__EXTERN PFNGLUNIFORM4FVPROC glUniform4fv;
__EXTERN PFNGLUNIFORM1IVPROC glUniform1iv;
__EXTERN PFNGLUNIFORM2IVPROC glUniform2iv;
__EXTERN PFNGLUNIFORM3IVPROC glUniform3iv;
__EXTERN PFNGLUNIFORM4IVPROC glUniform4iv;

__EXTERN PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv;
__EXTERN PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv;
__EXTERN PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;

__EXTERN PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
__EXTERN PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
__EXTERN PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
__EXTERN PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform;
__EXTERN PFNGLGETUNIFORMFVPROC glGetUniformfv;
__EXTERN PFNGLGETUNIFORMIVPROC glGetUniformiv;
__EXTERN PFNGLGETSHADERSOURCEPROC glGetShaderSource;
__EXTERN PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameteriv;

__EXTERN PFNGLGENQUERIESARBPROC glGenQueries;
__EXTERN PFNGLDELETEQUERIESARBPROC glDeleteQueries;
__EXTERN PFNGLBEGINQUERYARBPROC glBeginQuery;
__EXTERN PFNGLENDQUERYARBPROC glEndQuery;
__EXTERN PFNGLGETQUERYOBJECTUIVARBPROC glGetQueryObjectuiv;

__EXTERN PFNGLACTIVETEXTUREPROC glActiveTexture;
__EXTERN PFNGLMULTITEXCOORD2FPROC glMultiTexCoord2f;
__EXTERN PFNGLMULTITEXCOORD2FVPROC glMultiTexCoord2fv;

__EXTERN PFNGLBINDBUFFERPROC glBindBuffer;
__EXTERN PFNGLDELETEBUFFERSPROC glDeleteBuffers;
__EXTERN PFNGLGENBUFFERSPROC glGenBuffers;
__EXTERN PFNGLBUFFERDATAPROC glBufferData;
__EXTERN PFNGLBUFFERSUBDATAPROC glBufferSubData;
__EXTERN PFNGLMAPBUFFERPROC glMapBuffer;
__EXTERN PFNGLUNMAPBUFFERPROC glUnmapBuffer;
__EXTERN PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
__EXTERN PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
__EXTERN PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;

__EXTERN PFNGLMAPBUFFERRANGEPROC glMapBufferRange;
__EXTERN PFNGLFLUSHMAPPEDBUFFERRANGEPROC glFlushMappedBufferRange;

__EXTERN PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
__EXTERN PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
__EXTERN PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
__EXTERN PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
__EXTERN PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers;
__EXTERN PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers;
__EXTERN PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer;
__EXTERN PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage;
__EXTERN PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer;

__EXTERN PFNGLTEXBUFFERARBPROC glTexBufferARB;

#undef __EXTERN
