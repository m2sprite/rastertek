#include "system_class.h"

SystemClass::SystemClass()
{
  m_Input = 0;
  m_Application = 0;
}

SystemClass::SystemClass(const SystemClass& other)
{
}

SystemClass::~SystemClass()
{
}

bool SystemClass::Initialize()
{
  int screenWidth, screenHeight;
  bool result;
  m_Input = new InputClass;
  m_Input->Initialize();

  screenWidth = 0;
  screenHeight = 0;

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
}

void SystemClass::Shutdown()
{
  if(m_Application)
  {
    m_Application->Shutdown();
    delete m_Application;
    m_Application = 0;
  }
  ShutdownWindow();
  if(m_Input)
  {
    delete m_Input;
    m_Input = 0;
  }
  return;
}

void SystemClass::Frame()
{
  bool done, result;
  done = false;
  while(!done)
  {
    //Read The X11 input
    ReadInput();
    result = m_Application->Frame(m_Input);
  }
  return;
}

bool SystemClass::InitializeWindow( int& screenWidth, int& screenHeight)
{
  Window rootWindow;
  XVisualInfo *visualInfo;
  GLint attributeList[15];
  Colormap colorMap;
  XSetWindowAttributes setWindowAttributes;
  Screen *defaultScreen;
  bool result;
  int majorVersion;
  Atom wmState, fullScreenState, motifHints;
  XEvent fullScreenEvent;
  long motifHintList[5];
  int status, posX, posY, defaultScreenWidth, defaultScreenHeight;

  m_videoDisplay = XOpenDisplay(NULL);
  if(m_videoDisplay == NULL)
  {
    return false;
  }

  rootWindow = DefaultRootWindow(m_videoDisplay);

  attributeList[0] = GLX_RGBA;// support 24bit color and an alpha channel
  attributeList[1] = GLX_DEPTH_SIZE;// support for 24 bit depth buffer;
  attributeList[2] = 24;
  attributeList[3] = GLX_STENCIL_SIZE;
  attributeList[4] = 8;
  attributeList[5] = GLX_DOUBLEBUFFER; //Support for double buffering.
  attributeList[6] = GLX_RED_SIZE; //8 bits for each channel
  attributeList[7] = 8;
  attributeList[8] = GLX_GREEN_SIZE;
  attributeList[9] = 8;
  attributeList[10] = GLX_BLUE_SIZE;
  attributeList[11] = 8;
  attributeList[12] = GLX_ALPHA_SIZE;
  attributeList[13] = 8;
  attributeList[14] = None;

  // Query for a visual format that fits the attributes we want.
  visualInfo = glXChooseVisual(m_videoDisplay, 0, attributeList);
  if(visualInfo == NULL)
  {
    return false;
  }

  // Create a color map for the window the the specified visual type.
  colorMap = XCreateColormap(m_videoDisplay, rootWindow, visualInfo->visual, AllocNone);

  // Fill out the structure for setting the window attributes.
  setWindowAttributes.colormap = colorMap;
  setWindowAttributes.event_mask = KeyPressMask|KeyReleaseMask|ButtonPressMask|ButtonReleaseMask;

  if(FULL_SCREEN)
  {
    defaultScreen = XDefaultScreenOfDisplay(m_videoDisplay);
    screenWidth = XWidthOfScreen(defaultScreen);
    screenHeight = XHeightOfScreen(defaultScreen);
  } else
  {
    screenWidth = 1024;
    screenHeight = 768;
  }

  m_hwnd = XCreateWindow(m_videoDisplay, rootWindow, 0,0, screenWidth, screenHeight, 0, visualInfo->depth,
                          InputOutput, visualInfo->visual, CWColormap | CWEventMask, &setWindowAttributes);

  XMapWindow(m_videoDisplay, m_hwnd);
  XStoreName(m_videoDisplay, m_hwnd, "Engine");
if(FULL_SCREEN) {
    wmState = XInternAtom(m_videoDisplay, "_NET_WM_STATE",False);
    fullScreenState = XInternAtom(m_videoDisplay, "_NET_WM_STATE_FULLSCREEN", False);
    memset(&fullScreenEvent, 0, sizeof(fullScreenEvent));

    fullScreenEvent.type = ClientMessage;
    fullScreenEvent.xclient.window = m_hwnd;
    fullScreenEvent.xclient.message_type = wmState;
    fullScreenEvent.xclient.format = 32;
    fullScreenEvent.xclient.data.l[0] = 1;
    fullScreenEvent.xclient.data.l[1] = fullScreenState;
    fullScreenEvent.xclient.data.l[2] = 0;
    fullScreenEvent.xclient.data.l[3] = 0;
    fullScreenEvent.xclient.data.l[4] = 0;

    status = XSendEvent(m_videoDisplay, DefaultRootWindow(m_videoDisplay), False, SubstructureRedirectMask| SubstructureNotifyMask, &fullScreenEvent);
    if(status != 1)
    {
      return false;
    }

    motifHints = XInternAtom(m_videoDisplay, "MOTIF_WM_HINTS", False);
    motifHintList[0] = 2; //Remove border;
    motifHintList[1] = 0;
    motifHintList[2] = 0;
    motifHintList[3] = 0;
    motifHintList[4] = 0;

    status = XChangeProperty(m_videoDisplay, m_hwnd, motifHints,motifHints ,32, PropModeReplace, (unsigned char *)&motifHintList,5);
    if(status != 1)
    {
      return false;
    }

    status = XFlush(m_videoDisplay);
    if(status != 1)
    {
      return false;
    }

    sleep(1);
  }

  m_renderingContext = glXCreateContext(m_videoDisplay, visualInfo, NULL, GL_TRUE);
  if(m_renderingContext == NULL)
  {
    return false;
  }

  result = glXMakeCurrent(m_videoDisplay, m_hwnd, m_renderingContext);
  if(!result)
  {
    return false;
  }

  glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
  if(majorVersion < 4)
  {
    return false;
  }

  //Confirm that we have a direct renderring context
  result = glXIsDirect(m_videoDisplay, m_renderingContext);
  if(!result)
  {
    return false;
  }

  if(!FULL_SCREEN)
  {
    defaultScreen = XDefaultScreenOfDisplay(m_videoDisplay);
    defaultScreenWidth = XWidthOfScreen(defaultScreen);
    defaultScreenHeight = XHeightOfScreen(defaultScreen);
    posX =  (defaultScreenWidth - screenWidth)/2;
    posY =  (defaultScreenHeight - screenHeight)/2;

    status = XMoveWindow(m_videoDisplay, m_hwnd, posX, posY);
    if(status != 1)
    {
      return false;
    }
  }
  return true;
}

void SystemClass::ShutdownWindow()
{
  glXMakeCurrent(m_videoDisplay, None, NULL);
  glXDestroyContext(m_videoDisplay, m_renderingContext);
  XUnmapWindow(m_videoDisplay, m_hwnd);
  XDestroyWindow(m_videoDisplay, m_hwnd);
  XCloseDisplay(m_videoDisplay);
  return;
}

void SystemClass::ReadInput()
{
  XEvent event;
  long eventMask;
  bool foundEvent;
  char keyBuffer[32];
  KeySym keySymbol;

  eventMask = KeyPressMask | KeyReleaseMask;

  foundEvent = XCheckWindowEvent(m_videoDisplay, m_hwnd, eventMask, &event);
  if(foundEvent)
  {
    if(event.type == KeyPress)
    {
      XLookupString(&event.xkey, keyBuffer, sizeof(keyBuffer), &keySymbol, NULL);
      m_Input->KeyDown((int)keySymbol);
    }

    if(event.type == KeyRelease)
    {
      XLookupString(&event.xkey, keyBuffer, sizeof(keyBuffer), &keySymbol, NULL);
      m_Input->KeyUp((int)keySymbol);
    }
  }
}
