#include <unistd.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "primitive_typedefs.h"
#include "flat_opengl.cpp"

const i32 KEY_ESCAPE = 0;
const b32 FULL_SCREEN = false;
const b32 VSYNC_ENABLED = true;
const f32 SCREEN_NEAR = 0.3f;
const f32 SCREEN_DEPTH = 1000.0f;

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
