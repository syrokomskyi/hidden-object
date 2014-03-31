#include "stdafx.h"
#include "AppDelegate.h"
#include "ManagerDialog.h"
#include "ManagerScreen.h"
#include "Player.h"
#include "HomeScene.h"
#include "Home.h"


AppDelegate::AppDelegate() {
    // @see AppDelegate::applicationDidFinishLaunching()
}




AppDelegate::~AppDelegate() {
    //SimpleAudioEngine::end();
}




bool
AppDelegate::applicationDidFinishLaunching() {

    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView*  pEGLView  = CCEGLView::sharedOpenGLView();
    pDirector->setOpenGLView( pEGLView );

    pEGLView->setDesignResolutionSize(
        designResolutionSize.width,
        designResolutionSize.height,
        kResolutionNoBorder
    );

    // выбираем размер ресурсов в зависимости от разрешения экрана
    {
	    CCSize frameSize = pEGLView->getFrameSize();
        selectedResource =
            (frameSize.height > smallResource.size.height)  ? largeResource :
            smallResource;
        pDirector->setContentScaleFactor( std::min(
            selectedResource.size.height / designResolutionSize.height,
            selectedResource.size.width  / designResolutionSize.width
        ) );
#ifdef _DEBUG
        LOG << "Size of resources for this resolution: " <<
            selectedResource.directory << std::endl;
#endif
        std::vector< std::string >  searchPath;
        const std::string prefix =
            PATH_RESOURCE + "/" + selectedResource.directory + "/";
        searchPath.push_back( prefix );
        // # Список папок-комнат возьмём из декларации дома.
        const Home home( prefix + HOME_FILE_NAME );
        const auto allRooms = home.allRooms();
        for (auto itr = allRooms.cbegin(); itr != allRooms.cend(); ++itr) {
            const std::string&  name = *itr;
            DASSERT( !name.empty() );
#ifdef _DEBUG
            LOG << "\tRoom '" << name << "'" << std::endl;
#endif
            searchPath.push_back( prefix + name );
        }
        CCFileUtils::sharedFileUtils()->setSearchPaths( searchPath );
    }

    pDirector->setDisplayStats( true );

    pDirector->setAnimationInterval( 1.0f / 600.0f );

    // инициируем менеджеров
    ManagerDialog::mInstance = new ManagerDialog( selectedResource );
    ManagerScreen::mInstance = new ManagerScreen( selectedResource );

    // инициируем игрока
    std::unique_ptr< Home >  home( new Home(
        PATH_RESOURCE + "/" + selectedResource.directory + "/" + HOME_FILE_NAME
    ) );
    Player::mInstance = new Player( LOGIN, std::move( home ) );

    // первая сцена: карта дома
    ManagerScreen::instance()->goToHome();

    return true;
}




// This function will be called when the app is inactive.
// When comes a phone call, it's be invoked too.
void AppDelegate::applicationDidEnterBackground() {

    CCDirector::sharedDirector()->stopAnimation();
    //SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}




// This function will be called when the app is active again.
void AppDelegate::applicationWillEnterForeground() {

    CCDirector::sharedDirector()->startAnimation();
    //SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
