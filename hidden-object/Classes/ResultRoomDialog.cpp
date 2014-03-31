#include "stdafx.h"
#include "ResultRoomDialog.h"
#include "Player.h"
#include "ManagerScreen.h"


ResultRoomDialog::ResultRoomDialog():
    mChild( nullptr )
{
    DASSERT( this->init() );
}




ResultRoomDialog::~ResultRoomDialog() {
    auto d = CCDirector::sharedDirector();
    CCScene* scene = d->getRunningScene();
    scene->removeChild( this );
}




void
ResultRoomDialog::dialog() {

    auto d = CCDirector::sharedDirector();
    CCScene* scene = d->getRunningScene();
    do {
        scene->addChild( this );

    } while ( false );
}




bool
ResultRoomDialog::init() {

    bool bRet = false;
    do {
        CC_BREAK_IF( !CCLayerColor::init() );

        const CCSize visibleSize =
            CCDirector::sharedDirector()->getVisibleSize();
        const CCPoint origin =
            CCDirector::sharedDirector()->getVisibleOrigin();

        auto d = CCDirector::sharedDirector();
        
        // виз. форма диалога
        mChild = CCNode::create();

        CCSprite* bgSprite = CCSprite::create( "resultRoom.png" );
        CC_BREAK_IF( !bgSprite );
        bgSprite->setPosition( ccp(
            origin.x + visibleSize.width  / 2,
            origin.y + visibleSize.height * 2 / 3
        ) );
        mChild->addChild( bgSprite, 1 );

        const auto player = Player::instance();

        const std::string& nameRoom = player->room();
        CCLabelTTF*  name =
            CCLabelTTF::create( nameRoom.c_str(), "Arial", TITLE_FONT_SIZE / 2 );
        name->setColor( ccBLACK );
        static const coord_t
            coordName( visibleSize.width / 2,  visibleSize.height - 155 );
        name->setPosition( ccp(
            origin.x + coordName.x,
            origin.y + coordName.y
        ) );
        mChild->addChild( name, 2 );

        const auto mt = player->minTime();
        const auto time = player->timelimit() - player->restTime();
        CCString youTime;
        youTime.initWithFormat(
            "You time  %02u : %02u",
            (time / 60),  (time % 60)
        );
        CCString lastRecordTime;
        if (mt == 0) {
            lastRecordTime.initWithFormat(
                "Not best time yet..."
            );
        } else {
            lastRecordTime.initWithFormat(
                "Best time  %02u : %02u",
                (mt / 60),    (mt % 60)
            );
        }
        std::string result = "Time is out.";
        if ( player->completed() ) {
            result =
                static_cast< std::string >( youTime.getCString() ) +
                "\n\n" +
                static_cast< std::string >( lastRecordTime.getCString() ) +
                ( (time < mt) ? "\n\nNew record!" : "" ) +
                "\n\nRoom completed.";
        }
        CCLabelTTF*  lbResult =
            CCLabelTTF::create( result.c_str(), "Arial", TITLE_FONT_SIZE / 2 );
        lbResult->setColor( ccBLACK );
        static const coord_t
            coord( visibleSize.width / 2,  visibleSize.height - 350 );
        lbResult->setPosition( ccp(
            origin.x + coord.x,
            origin.y + coord.y
        ) );
        mChild->addChild( lbResult, 2 );


        addChild( mChild, 1 );

        
        CCDirector::sharedDirector()->getTouchDispatcher()
            ->addTargetedDelegate( this, 0, true );


        bRet = true;

    } while ( false );

    return bRet;
}




bool 
ResultRoomDialog::ccTouchBegan( CCTouch* touch, CCEvent* event ) {

    const auto p = touch->getLocationInView();
#ifdef _DEBUG
    LOG << "ResultRoomDialog::ccTouchBegan() " << p.x << " " << p.y << std::endl;
#endif

    auto d = CCDirector::sharedDirector();

    // нажата кнопка отмены или "Закрыть"?
    const bool cancel = pressCancel( p );
    if ( cancel ) {
        d->getTouchDispatcher()->removeDelegate( this );
        removeChild( mChild );
        ManagerScreen::instance()->goToHome();
    }

    return true;
}




void 
ResultRoomDialog::ccTouchMoved( CCTouch*, CCEvent* ) {
}




void 
ResultRoomDialog::ccTouchEnded( CCTouch*, CCEvent* ) {
}




void 
ResultRoomDialog::ccTouchCancelled( CCTouch*, CCEvent* ) {
}




bool
ResultRoomDialog::pressCancel( const coord_t& point ) const {
    // # Кнопка отмены этого диалога - в форме круга.
    static const coord_t  coord( 530, 150 );
    static const float radius = 30.0f;
    return (std::abs( point.x - coord.x ) <= radius)
        && (std::abs( point.y - coord.y ) <= radius);
}
