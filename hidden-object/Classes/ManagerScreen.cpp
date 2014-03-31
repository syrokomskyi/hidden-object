#include "stdafx.h"
#include "ManagerScreen.h"
#include "ManagerDialog.h"
#include "AppDelegate.h"
#include "HomeScene.h"
#include "RoomScene.h"
#include "Home.h"
#include "Player.h"


ManagerScreen*  ManagerScreen::mInstance = nullptr;




ManagerScreen::ManagerScreen( const Resource& selectedResource ):
    selectedResource( selectedResource ),
    mHomeScene(),
    mRoomScene()
{
    DASSERT( !selectedResource.directory.empty() );
}




ManagerScreen::~ManagerScreen() {
}




void
ManagerScreen::goToHome() {

    ManagerDialog::instance()->closeAboutRoom();
    ManagerDialog::instance()->closeNeedEnergy();

    std::shared_ptr< Home >  home( new Home(
        PATH_RESOURCE + "/" + selectedResource.directory + "/" + HOME_FILE_NAME
    ) );
    mHomeScene = std::shared_ptr< HomeScene >( new HomeScene( home ) );

    auto d = CCDirector::sharedDirector();
    d->getTouchDispatcher()->removeDelegate( mRoomScene.get() );

    CCScene* scene = mHomeScene->scene();
    if ( d->getRunningScene() ) {
        d->setDepthTest( true );
        CCTransitionScene* transition = CCTransitionFade::create( 2.0f, scene );
        d->replaceScene( transition );

    } else {
        // первая сцена
        d->runWithScene( scene );
    }
}




void
ManagerScreen::goToRoom( const std::string& name ) {

    DASSERT( mHomeScene );
    DASSERT( !name.empty() );

    ManagerDialog::instance()->closeResultRoom();

    auto room = mHomeScene->home()->room( name );
    DASSERT( room );

    auto d = CCDirector::sharedDirector();
    d->getTouchDispatcher()->removeDelegate( mHomeScene.get() );

    mRoomScene = std::shared_ptr< RoomScene >( new RoomScene( room ) );

    CCScene* scene = mRoomScene->scene();
    d->setDepthTest( true );
    CCTransitionScene* transition = CCTransitionFade::create( 1.0f, scene );
    d->replaceScene( transition );

    // время на исследование комнаты
    auto player = Player::instance();
    player->timelimit( room->timelimit );
    player->restTime(  room->timelimit );
    player->findItems( 0 );
}
