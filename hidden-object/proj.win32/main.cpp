#include "stdafx.h"
#include "main.h"
#include "AppDelegate.h"


#define USE_WIN32_CONSOLE


int APIENTRY _tWinMain(
    HINSTANCE  hInstance,
    HINSTANCE  hPrevInstance,
    LPTSTR     lpCmdLine,
    int        nCmdShow
) {
    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );

    setlocale( LC_ALL, "Russian" );
    setlocale( LC_NUMERIC, "C" );


#ifdef USE_WIN32_CONSOLE
    AllocConsole();
    freopen( "CONIN$",  "r", stdin );
    freopen( "CONOUT$", "w", stdout );
    freopen( "CONOUT$", "w", stderr );
#endif


    AppDelegate app;
    CCEGLView* eglView = CCEGLView::sharedOpenGLView();
    eglView->setFrameSize( 640, 1136 );
    eglView->setFrameZoomFactor( 0.75f );

    int ret = CCApplication::sharedApplication()->run();


#ifdef USE_WIN32_CONSOLE
    FreeConsole();
#endif

    return ret;
}
