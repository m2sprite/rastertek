#include <unistd.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "primitive_typedefs.h"
#include "opengl_typedefs.h"
#include "opengl_matrix_ops.h"

const i32 KEY_ESCAPE = 0;

typedef struct {
  PFNGLCREATESHADERPROC glCreateShader;
  PFNGLSHADERSOURCEPROC glShaderSource;
  PFNGLCOMPILESHADERPROC glCompileShader;
  PFNGLGETSHADERIVPROC glGetShaderiv;
  PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
  PFNGLCREATEPROGRAMPROC glCreateProgram;
  PFNGLATTACHSHADERPROC glAttachShader;
  PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation;
  PFNGLLINKPROGRAMPROC glLinkProgram;
  PFNGLGETPROGRAMIVPROC glGetProgramiv;
  PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
  PFNGLDETACHSHADERPROC glDetachShader;
  PFNGLDELETESHADERPROC glDeleteShader;
  PFNGLDELETEPROGRAMPROC glDeleteProgram;
  PFNGLUSEPROGRAMPROC glUseProgram;
  PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
  PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
  PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
  PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
  PFNGLGENBUFFERSPROC glGenBuffers;
  PFNGLBINDBUFFERPROC glBindBuffer;
  PFNGLBUFFERDATAPROC glBufferData;
  PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
  PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
  PFNGLDELETEBUFFERSPROC glDeleteBuffers;
  PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
  PFNGLUNIFORM1IPROC glUniform1i;
  PFNGLACTIVETEXTUREPROC glActiveTexture;
  PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
  PFNGLUNIFORM2FVPROC glUniform2fv;
  PFNGLUNIFORM3FVPROC glUniform3fv;
  PFNGLUNIFORM4FVPROC glUniform4fv;
  PFNGLMAPBUFFERPROC glMapBuffer;
  PFNGLUNMAPBUFFERPROC glUnmapBuffer;
  PFNGLXSWAPINTERVALEXTPROC glXSwapIntervalEXT;
  PFNGLUNIFORM1FPROC glUniform1f;
  PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
  PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
  PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
  PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
  PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers;
  PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer;
  PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage;
  PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer;
  PFNGLDRAWBUFFERSPROC glDrawBuffers;
  PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers;
  PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate;
} open_gl_function_pointers;

typedef struct {
  f32 WorldMatrix[16];
  f32 ProjectionMatrix[16];
  f32 OrthoMatrix[16];
} open_gl_matrix_struct;

const b32 FULL_SCREEN = false;
const b32 VSYNC_ENABLED = true;
const f32 SCREEN_NEAR = 0.3f;
const f32 SCREEN_DEPTH = 1000.0f;

void GLBeginScene(f32 red, f32 green, f32 blue, f32 alpha)
{
  glClearColor(red, green, blue, alpha);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  return;
}

void GLEndScene(Display *MainVideoDisplay, Window MainWindow)
{
  glXSwapBuffers(MainVideoDisplay, MainWindow);
  return;
}

b32 GLRender(Display *MainVideoDisplay, Window MainWindow)
{
  GLBeginScene(1.0f, 1.0f, 0.0f, 1.0f);
  GLEndScene(MainVideoDisplay, MainWindow);
  return true;
}

b32 LoadOpenGLExtensionList(open_gl_function_pointers *OpenGLFunctions)
{
  OpenGLFunctions->glCreateShader = (PFNGLCREATESHADERPROC)glXGetProcAddress((unsigned char*)"glCreateShader");
  if(!OpenGLFunctions->glCreateShader)
  {
    return false;
  }

  OpenGLFunctions->glShaderSource = (PFNGLSHADERSOURCEPROC)glXGetProcAddress((unsigned char*)"glShaderSource");
  if(!OpenGLFunctions->glShaderSource)
  {
    return false;
  }

  OpenGLFunctions->glCompileShader = (PFNGLCOMPILESHADERPROC)glXGetProcAddress((unsigned char*)"glCompileShader");
  if(!OpenGLFunctions->glCompileShader)
  {
    return false;
  }

  OpenGLFunctions->glGetShaderiv = (PFNGLGETSHADERIVPROC)glXGetProcAddress((unsigned char*)"glGetShaderiv");
  if(!OpenGLFunctions->glGetShaderiv)
  {
    return false;
  }

  OpenGLFunctions->glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)glXGetProcAddress((unsigned char*)"glGetShaderInfoLog");
  if(!OpenGLFunctions->glGetShaderInfoLog)
  {
    return false;
  }

  OpenGLFunctions->glCreateProgram = (PFNGLCREATEPROGRAMPROC)glXGetProcAddress((unsigned char*)"glCreateProgram");
  if(!OpenGLFunctions->glCreateProgram)
  {
    return false;
  }

  OpenGLFunctions->glAttachShader = (PFNGLATTACHSHADERPROC)glXGetProcAddress((unsigned char*)"glAttachShader");
  if(!OpenGLFunctions->glAttachShader)
  {
    return false;
  }

  OpenGLFunctions->glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)glXGetProcAddress((unsigned char*)"glBindAttribLocation");
  if(!OpenGLFunctions->glBindAttribLocation)
  {
    return false;
  }

  OpenGLFunctions->glLinkProgram = (PFNGLLINKPROGRAMPROC)glXGetProcAddress((unsigned char*)"glLinkProgram");
  if(!OpenGLFunctions->glLinkProgram)
  {
    return false;
  }

  OpenGLFunctions->glGetProgramiv = (PFNGLGETPROGRAMIVPROC)glXGetProcAddress((unsigned char*)"glGetProgramiv");
  if(!OpenGLFunctions->glGetProgramiv)
  {
    return false;
  }

  OpenGLFunctions->glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)glXGetProcAddress((unsigned char*)"glGetProgramInfoLog");
  if(!OpenGLFunctions->glGetProgramInfoLog)
  {
    return false;
  }

  OpenGLFunctions->glDetachShader = (PFNGLDETACHSHADERPROC)glXGetProcAddress((unsigned char*)"glDetachShader");
  if(!OpenGLFunctions->glDetachShader)
  {
    return false;
  }

  OpenGLFunctions->glDeleteShader = (PFNGLDELETESHADERPROC)glXGetProcAddress((unsigned char*)"glDeleteShader");
  if(!OpenGLFunctions->glDeleteShader)
  {
    return false;
  }

  OpenGLFunctions->glDeleteProgram = (PFNGLDELETEPROGRAMPROC)glXGetProcAddress((unsigned char*)"glDeleteProgram");
  if(!OpenGLFunctions->glDeleteProgram)
  {
    return false;
  }

  OpenGLFunctions->glUseProgram = (PFNGLUSEPROGRAMPROC)glXGetProcAddress((unsigned char*)"glUseProgram");
  if(!OpenGLFunctions->glUseProgram)
  {
    return false;
  }

  OpenGLFunctions->glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)glXGetProcAddress((unsigned char*)"glGetUniformLocation");
  if(!OpenGLFunctions->glGetUniformLocation)
  {
    return false;
  }

  OpenGLFunctions->glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)glXGetProcAddress((unsigned char*)"glUniformMatrix4fv");
  if(!OpenGLFunctions->glUniformMatrix4fv)
  {
    return false;
  }

  OpenGLFunctions->glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)glXGetProcAddress((unsigned char*)"glGenVertexArrays");
  if(!OpenGLFunctions->glGenVertexArrays)
  {
    return false;
  }

  OpenGLFunctions->glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)glXGetProcAddress((unsigned char*)"glBindVertexArray");
  if(!OpenGLFunctions->glBindVertexArray)
  {
    return false;
  }

  OpenGLFunctions->glGenBuffers = (PFNGLGENBUFFERSPROC)glXGetProcAddress((unsigned char*)"glGenBuffers");
  if(!OpenGLFunctions->glGenBuffers)
  {
    return false;
  }

  OpenGLFunctions->glBindBuffer = (PFNGLBINDBUFFERPROC)glXGetProcAddress((unsigned char*)"glBindBuffer");
  if(!OpenGLFunctions->glBindBuffer)
  {
    return false;
  }

  OpenGLFunctions->glBufferData = (PFNGLBUFFERDATAPROC)glXGetProcAddress((unsigned char*)"glBufferData");
  if(!OpenGLFunctions->glBufferData)
  {
    return false;
  }

  OpenGLFunctions->glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)glXGetProcAddress((unsigned char*)"glEnableVertexAttribArray");
  if(!OpenGLFunctions->glEnableVertexAttribArray)
  {
    return false;
  }

  OpenGLFunctions->glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)glXGetProcAddress((unsigned char*)"glVertexAttribPointer");
  if(!OpenGLFunctions->glVertexAttribPointer)
  {
    return false;
  }

  OpenGLFunctions->glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)glXGetProcAddress((unsigned char*)"glDeleteBuffers");
  if(!OpenGLFunctions->glDeleteBuffers)
  {
    return false;
  }

  OpenGLFunctions->glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)glXGetProcAddress((unsigned char*)"glDeleteVertexArrays");
  if(!OpenGLFunctions->glDeleteVertexArrays)
  {
    return false;
  }

  OpenGLFunctions->glUniform1i = (PFNGLUNIFORM1IPROC)glXGetProcAddress((unsigned char*)"glUniform1i");
  if(!OpenGLFunctions->glUniform1i)
  {
    return false;
  }

  OpenGLFunctions->glActiveTexture = (PFNGLACTIVETEXTUREPROC)glXGetProcAddress((unsigned char*)"glActiveTexture");
  if(!OpenGLFunctions->glActiveTexture)
  {
    return false;
  }

  OpenGLFunctions->glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)glXGetProcAddress((unsigned char*)"glGenerateMipmap");
  if(!OpenGLFunctions->glGenerateMipmap)
  {
    return false;
  }

  OpenGLFunctions->glUniform2fv = (PFNGLUNIFORM2FVPROC)glXGetProcAddress((unsigned char*)"glUniform2fv");
  if(!OpenGLFunctions->glUniform2fv)
  {
    return false;
  }

  OpenGLFunctions->glUniform3fv = (PFNGLUNIFORM3FVPROC)glXGetProcAddress((unsigned char*)"glUniform3fv");
  if(!OpenGLFunctions->glUniform3fv)
  {
    return false;
  }

  OpenGLFunctions->glUniform4fv = (PFNGLUNIFORM4FVPROC)glXGetProcAddress((unsigned char*)"glUniform4fv");
  if(!OpenGLFunctions->glUniform4fv)
  {
    return false;
  }

  OpenGLFunctions->glMapBuffer = (PFNGLMAPBUFFERPROC)glXGetProcAddress((unsigned char*)"glMapBuffer");
  if(!OpenGLFunctions->glMapBuffer)
  {
    return false;
  }

  OpenGLFunctions->glUnmapBuffer = (PFNGLUNMAPBUFFERPROC)glXGetProcAddress((unsigned char*)"glUnmapBuffer");
  if(!OpenGLFunctions->glUnmapBuffer)
  {
    return false;
  }

  OpenGLFunctions->glXSwapIntervalEXT = (PFNGLXSWAPINTERVALEXTPROC)glXGetProcAddress((unsigned char*)"glXSwapIntervalEXT");
  if(!OpenGLFunctions->glXSwapIntervalEXT)
  {
    return false;
  }

  OpenGLFunctions->glUniform1f = (PFNGLUNIFORM1FPROC)glXGetProcAddress((unsigned char*)"glUniform1f");
  if(!OpenGLFunctions->glUniform1f)
  {
    return false;
  }

  OpenGLFunctions->glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)glXGetProcAddress((unsigned char*)"glGenFramebuffers");
  if(!OpenGLFunctions->glGenFramebuffers)
  {
    return false;
  }

  OpenGLFunctions->glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)glXGetProcAddress((unsigned char*)"glDeleteFramebuffers");
  if(!OpenGLFunctions->glDeleteFramebuffers)
  {
    return false;
  }

  OpenGLFunctions->glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)glXGetProcAddress((unsigned char*)"glBindFramebuffer");
  if(!OpenGLFunctions->glBindFramebuffer)
  {
    return false;
  }

  OpenGLFunctions->glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)glXGetProcAddress((unsigned char*)"glFramebufferTexture2D");
  if(!OpenGLFunctions->glFramebufferTexture2D)
  {
    return false;
  }

  OpenGLFunctions->glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)glXGetProcAddress((unsigned char*)"glGenRenderbuffers");
  if(!OpenGLFunctions->glGenRenderbuffers)
  {
    return false;
  }

  OpenGLFunctions->glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)glXGetProcAddress((unsigned char*)"glBindRenderbuffer");
  if(!OpenGLFunctions->glBindRenderbuffer)
  {
    return false;
  }

  OpenGLFunctions->glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)glXGetProcAddress((unsigned char*)"glRenderbufferStorage");
  if(!OpenGLFunctions->glRenderbufferStorage)
  {
    return false;
  }

  OpenGLFunctions->glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)glXGetProcAddress((unsigned char*)"glFramebufferRenderbuffer");
  if(!OpenGLFunctions->glFramebufferRenderbuffer)
  {
    return false;
  }

  OpenGLFunctions->glDrawBuffers = (PFNGLDRAWBUFFERSARBPROC)glXGetProcAddress((unsigned char*)"glDrawBuffers");
  if(!OpenGLFunctions->glDrawBuffers)
  {
    return false;
  }

  OpenGLFunctions->glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)glXGetProcAddress((unsigned char*)"glDeleteRenderbuffers");
  if(!OpenGLFunctions->glDeleteRenderbuffers)
  {
    return false;
  }

  OpenGLFunctions->glBlendFuncSeparate = (PFNGLBLENDFUNCSEPARATEPROC)glXGetProcAddress((unsigned char*)"glBlendFuncSeparate");
  if(!OpenGLFunctions->glBlendFuncSeparate)
  {
    return false;
  }

  return true;
}

b32 InitializeOpenGL(Display *MainVideoDisplay, Window win, i32 ScreenWidth, i32 ScreenHeight, f32 ScreenNear, f32 ScreenDepth, b32 Vsync, open_gl_matrix_struct *Matricies, open_gl_function_pointers *Functions)
{
  GLXDrawable Drawable;
  f32 FieldOfView, ScreenAspect;
  b32 Result;

  Result = LoadOpenGLExtensionList(Functions);
  if(!Result)
  {
    return false;
  }

  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST);
  glFrontFace(GL_CW);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  Drawable = glXGetCurrentDrawable();

  if(Vsync)
  {
    Functions->glXSwapIntervalEXT( MainVideoDisplay, Drawable, 1);
  }
  else
  {
    Functions->glXSwapIntervalEXT( MainVideoDisplay, Drawable, 0);
  }

  GLBuildIdentityMatrix(Matricies->WorldMatrix);

  FieldOfView = 3.14159265358979323846f / 4.0f;
  ScreenAspect = (f32)ScreenWidth/(f32)ScreenHeight;
  GLBuildPerspectiveFovMatrix( Matricies->ProjectionMatrix, FieldOfView, ScreenAspect, ScreenNear, ScreenDepth);
  GLBuildOrthoMatrix(Matricies->OrthoMatrix, (f32)ScreenHeight, (f32)ScreenHeight, ScreenNear, ScreenDepth);

  return true;
}

b32 InitializeWindow( i32 *ScreenWidth,i32 *ScreenHeight,  Display *MainVideoDisplay, Window *MainWindow, GLXContext *MainRenderingContext)
{
  b32 Result;
  Window RootWindow;
  XVisualInfo *VisualInfo;
  GLint AttributeList[15];
  Colormap ColorMap;
  XSetWindowAttributes SetWindowAttributes;
  Screen *DefaultScreen;
  i32 MajorVersion;

  Atom WmState, FullScreenState, MotifHints;
  XEvent FullScreenEvent;
  i32 MotifHintList[5];
  // NOTE(m2sprite): for some reason the Status variable name is taken by something
  i32 status, PosX, PosY, DefaultScreenWidth, DefaultScreenHeight;


  RootWindow = DefaultRootWindow(MainVideoDisplay);

  AttributeList[0] = GLX_RGBA;// support 24bit color and an alpha channel
  AttributeList[1] = GLX_DEPTH_SIZE;// support for 24 bit depth buffer;
  AttributeList[2] = 24;
  AttributeList[3] = GLX_STENCIL_SIZE;
  AttributeList[4] = 8;
  AttributeList[5] = GLX_DOUBLEBUFFER; //Support for double buffering.
  AttributeList[6] = GLX_RED_SIZE; //8 bits for each channel
  AttributeList[7] = 8;
  AttributeList[8] = GLX_GREEN_SIZE;
  AttributeList[9] = 8;
  AttributeList[10] = GLX_BLUE_SIZE;
  AttributeList[11] = 8;
  AttributeList[12] = GLX_ALPHA_SIZE;
  AttributeList[13] = 8;
  AttributeList[14] = None;

  VisualInfo = glXChooseVisual(MainVideoDisplay, 0, AttributeList);
  if(!VisualInfo)
  {
    return false;
  }

  ColorMap = XCreateColormap(MainVideoDisplay, RootWindow, VisualInfo->visual, AllocNone);

  // Fill out the structure for setting the window attributes.
  SetWindowAttributes.colormap = ColorMap;
  SetWindowAttributes.event_mask = KeyPressMask|KeyReleaseMask|ButtonPressMask|ButtonReleaseMask;

  if(FULL_SCREEN)
  {
    DefaultScreen = XDefaultScreenOfDisplay(MainVideoDisplay);
    *ScreenWidth = XWidthOfScreen(DefaultScreen);
    *ScreenHeight = XHeightOfScreen(DefaultScreen);
  } else
  {
    *ScreenWidth = 1024;
    *ScreenHeight = 768;
  }

  *MainWindow = XCreateWindow(MainVideoDisplay, RootWindow, 0,0, *ScreenWidth, *ScreenHeight, 0, VisualInfo->depth,
                          InputOutput, VisualInfo->visual, CWColormap | CWEventMask, &SetWindowAttributes);

  XMapWindow(MainVideoDisplay, *MainWindow);
  XStoreName(MainVideoDisplay, *MainWindow, "Engine");
  if(FULL_SCREEN)
  {
    WmState = XInternAtom(MainVideoDisplay, "_NET_WM_STATE",False);
    FullScreenState = XInternAtom(MainVideoDisplay, "_NET_WM_STATE_FULLSCREEN", False);
    memset(&FullScreenEvent, 0, sizeof(FullScreenEvent));

    FullScreenEvent.type = ClientMessage;
    FullScreenEvent.xclient.window = *MainWindow;
    FullScreenEvent.xclient.message_type = WmState;
    FullScreenEvent.xclient.format = 32;
    FullScreenEvent.xclient.data.l[0] = 1;
    FullScreenEvent.xclient.data.l[1] = FullScreenState;
    FullScreenEvent.xclient.data.l[2] = 0;
    FullScreenEvent.xclient.data.l[3] = 0;
    FullScreenEvent.xclient.data.l[4] = 0;

    status = XSendEvent(MainVideoDisplay, DefaultRootWindow(MainVideoDisplay), False, SubstructureRedirectMask| SubstructureNotifyMask, &FullScreenEvent);
    if(status != 1)
    {
      return false;
    }

    MotifHints = XInternAtom(MainVideoDisplay, "MOTIF_WM_HINTS", False);
    MotifHintList[0] = 2; //Remove border;
    MotifHintList[1] = 0;
    MotifHintList[2] = 0;
    MotifHintList[3] = 0;
    MotifHintList[4] = 0;

    status = XChangeProperty(MainVideoDisplay, *MainWindow, MotifHints, MotifHints ,32, PropModeReplace, (unsigned char *)&MotifHintList,5);
    if(status != 1)
    {
      return false;
    }

    status = XFlush(MainVideoDisplay);
    if(status != 1)
    {
      return false;
    }

    sleep(1);
  }

  *MainRenderingContext = glXCreateContext(MainVideoDisplay, VisualInfo, NULL, GL_TRUE);
  if(*MainRenderingContext == NULL)
  {
    return false;
  }

  Result = glXMakeCurrent(MainVideoDisplay, *MainWindow, *MainRenderingContext);
  if(!Result)
  {
    return false;
  }

  glGetIntegerv(GL_MAJOR_VERSION, &MajorVersion);
  if(MajorVersion < 4)
  {
    return false;
  }

  //Confirm that we have a direct renderring context
  Result = glXIsDirect(MainVideoDisplay, *MainRenderingContext);
  if(!Result)
  {
    return false;
  }

  if(!FULL_SCREEN)
  {
    DefaultScreen = XDefaultScreenOfDisplay(MainVideoDisplay);
    DefaultScreenWidth = XWidthOfScreen(DefaultScreen);
    DefaultScreenHeight = XHeightOfScreen(DefaultScreen);
    PosX =  (DefaultScreenWidth - *ScreenWidth)/2;
    PosY =  (DefaultScreenHeight - *ScreenHeight)/2;

    status = XMoveWindow(MainVideoDisplay, *MainWindow, PosX, PosY);
    if(status != 1)
    {
      return false;
    }
  }
  return true;
}

void KeyDown(i32 KeySymbol, i32 *KeyboardState)
{
  if(KeySymbol == 65307) {
    KeyboardState[KEY_ESCAPE] = true;
  }
  return;
}

void KeyUp(int KeySymbol, i32 *KeyboardState)
{
  if(KeySymbol == 65307) {
    KeyboardState[KEY_ESCAPE] = false;
  }
  return;
}

b32 IsEscapePressed( i32 *KeyboardState )
{
  return KeyboardState[KEY_ESCAPE];
}

int main(void)
{
  i32 ScreenWidth = 0;
  i32 ScreenHeight = 0;

  Display *MainVideoDisplay = XOpenDisplay(0);
  if(!MainVideoDisplay) { return false; }
  GLXContext RenderingContext;
  Window MainWindow;
  b32 Result;

  //INITIALIZE INPUT
  i32 KeyboardState[256];
  for(size_t key_i = 0; key_i <256; ++key_i)
  {
    KeyboardState[key_i] = 0;
  }

  //INITIALIZE WINDOW
  Result = InitializeWindow(&ScreenWidth, &ScreenHeight, MainVideoDisplay, &MainWindow, &RenderingContext);
  if(!Result)
  {
    return -1;
  }

  //InitializeAPPLICATIION
  open_gl_matrix_struct OglMatricies;
  open_gl_function_pointers OglFunctions;
  InitializeOpenGL( MainVideoDisplay, MainWindow, ScreenWidth, ScreenHeight, SCREEN_NEAR, SCREEN_DEPTH, VSYNC_ENABLED, &OglMatricies, &OglFunctions);


  b32 Done;
  Done = false;
  while(!Done)
  {

    XEvent Event;
    i32 EventMask;
    b32 FoundEvent;
    char KeyBuffer[32];
    KeySym KeySymbol;

    EventMask = KeyPressMask | KeyReleaseMask;

    FoundEvent = XCheckWindowEvent(MainVideoDisplay, MainWindow, EventMask, &Event);

    if(FoundEvent)
    {
      if(Event.type == KeyPress)
      {
        XLookupString(&Event.xkey, KeyBuffer, sizeof(KeyBuffer), &KeySymbol, NULL);
        KeyDown(KeySymbol, KeyboardState);
      }

      if(Event.type == KeyRelease)
      {
        XLookupString(&Event.xkey, KeyBuffer, sizeof(KeyBuffer), &KeySymbol, NULL);
        KeyUp(KeySymbol, KeyboardState);
      }
    }


    if(IsEscapePressed(KeyboardState) == true)
    {
      return false;
    }

    Result = GLRender(MainVideoDisplay, MainWindow);

    if(!Result)
    {
      return false;
    }

    if(!Result)
    {
      Done = true;
    }

  }

  //FRAME
  //SHUTDOWN

  /*
  result = InitializeWindow(screenWidth, screenHeight);
  if(!result)
  {
    return false;
  }

  m_Application = new ApplicationClass;

  result = m_Application->Initialize(m_videoDisplay, m_hwnd, screenWidth, screenHeight);

  if(!result)
  {
    return false;
  }

  return true;
  */

  return(0);
}
