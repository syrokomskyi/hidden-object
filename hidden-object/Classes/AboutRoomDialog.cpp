#include "stdafx.h"
#include "AboutRoomDialog.h"
#include "Player.h"
#include "ManagerScreen.h"
#include "ManagerDialog.h"


AboutRoomDialog::AboutRoomDialog():
    mChild( nullptr )
{
    DASSERT( this->init() );
}




AboutRoomDialog::~AboutRoomDialog() {
    auto d = CCDirector::sharedDirector();
    CCScene* scene = d->getRunningScene();
    scene->removeChild( this );
}




void
AboutRoomDialog::dialog() {

    auto d = CCDirector::sharedDirector();
    CCScene* scene = d->getRunningScene();
    do {
        scene->addChild( this );

    } while ( false );
}




bool
AboutRoomDialog::init() {

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

        CCSprite* bgSprite = CCSprite::create( "aboutRoom.png" );
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
            coordName( visibleSize.width / 2,  visibleSize.height - 135 );
        name->setPosition( ccp(
            origin.x + coordName.x,
            origin.y + coordName.y
        ) );
        mChild->addChild( name, 2 );

        const auto mt = player->minTime();
        CCString s;
        if (mt == 0) {
            s.initWithFormat(
                "Need %u energy"
                "\n\nNot best time yet...",
                player->needEnergy()
            );
        } else {
            s.initWithFormat(
                "Need %u energy"
                "\n\nBest time  %02u : %02u",
                player->needEnergy(),
                (mt / 60),   (mt % 60)
            );
        }
        CCLabelTTF*  minTime =
            CCLabelTTF::create( s.getCString(), "Arial", TITLE_FONT_SIZE / 2 );
        minTime->setColor( ccBLACK );
        static const coord_t
            coordMinTime( visibleSize.width / 2,  visibleSize.height - 250 );
        minTime->setPosition( ccp(
            origin.x + coordMinTime.x,
            origin.y + coordMinTime.y
        ) );
        mChild->addChild( minTime, 2 );



        addChild( mChild, 1 );

        
        CCDirector::sharedDirector()->getTouchDispatcher()
            ->addTargetedDelegate( this, 0, true );


        bRet = true;

    } while ( false );

    return bRet;
}




bool 
AboutRoomDialog::ccTouchBegan( CCTouch* touch, CCEvent* event ) {

    const auto p = touch->getLocationInView();
#ifdef _DEBUG
    LOG << "AboutRoomDialog::ccTouchBegan() " << p.x << " " << p.y << std::endl;
#endif

    auto d = CCDirector::sharedDirector();

    // нажата кнопка отмены или "Закрыть"?
    const bool cancelOrClose = pressCancel( p ) || pressClose( p );
    const bool explore = pressExplore( p );
    if ( cancelOrClose || explore ) {
        d->getTouchDispatcher()->removeDelegate( this );
        removeChild( mChild );
    }

    // нажата кнопка "Исследовать"?
    if ( explore ) {
        // проверим, хватает ли энергии
        const auto player = Player::instance();
        if ( player->enoughEnergy() ) {
            ManagerScreen::instance()->goToRoom( player->room() );
        } else {
            ManagerDialog::instance()->showNeedEnergy();
        }
    }

    return true;
}




void 
AboutRoomDialog::ccTouchMoved( CCTouch*, CCEvent* ) {
}




void 
AboutRoomDialog::ccTouchEnded( CCTouch*, CCEvent* ) {
}




void 
AboutRoomDialog::ccTouchCancelled( CCTouch*, CCEvent* ) {
}




bool
AboutRoomDialog::pressCancel( const coord_t& point ) const {
    // # Кнопка отмены этого диалога - в форме круга.
    static const coord_t  coord( 530, 120 );
    static const float radius = 30.0f;
    return (std::abs( point.x - coord.x ) <= radius)
        && (std::abs( point.y - coord.y ) <= radius);
}




bool
AboutRoomDialog::pressClose( const coord_t& point ) const {
    // # Кнопка "Закрыть" - в форме прямоугольника.
    std::cout << point << std::endl;
    static const CCRect  box( 360, 620,  150, 30 );
    return box.containsPoint( point );
}




bool
AboutRoomDialog::pressExplore( const coord_t& point ) const {
    // # Кнопка "Исследовать" - в форме прямоугольника.
    std::cout << point << std::endl;
    static const CCRect  box( 135, 620,  150, 30 );
    return box.containsPoint( point );
}
