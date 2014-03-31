#include "stdafx.h"
#include "RoomScene.h"
#include "Home.h"
#include "Room.h"
#include "ManagerScreen.h"
#include "ManagerDialog.h"
#include "Player.h"


RoomScene::RoomScene( std::shared_ptr< Room > room ):
    mRoom( room ),
    mItems( room->currentItemSet() ),
    countAllItems( room->currentItemSet().size() )
{
    DASSERT( mRoom );
    DASSERT( this->init() );
    DASSERT( !mItems.empty()
        && "Для комнаты не обнаружено ни одной вещи." );
}




RoomScene::~RoomScene() {
}




CCScene*
RoomScene::scene() {

    CCScene* scene = nullptr;
    do {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF( !scene );

        scene->addChild( this );

    } while ( false );

    return scene;
}




bool
RoomScene::init() {

    bool bRet = false;
    do {
        CC_BREAK_IF( !CCLayerColor::initWithColor( ccc4( 0, 0, 0, 255 ) ) );

        const CCSize visibleSize =
            CCDirector::sharedDirector()->getVisibleSize();
        const CCPoint origin =
            CCDirector::sharedDirector()->getVisibleOrigin();

        
        // комната
        // # Покажем комнату на фоне интерфейса (демка, всё же).
        
        const std::string name = mRoom->name + ".png";
        const auto sizeRoomAtlas = mRoom->home()->sizeRoomAtlas();
        const CCRect rect( 0, 0, sizeRoomAtlas.width, sizeRoomAtlas.height );
        CCSprite* roomSprite = CCSprite::create( name.c_str(), rect );
        CC_BREAK_IF( !roomSprite );
        const auto scale = mRoom->home()->scaleRoom();
        roomSprite->setScaleX( scale.width );
        roomSprite->setScaleY( scale.height );
        roomSprite->setAnchorPoint( ccp( 0, 1 ) );
        roomSprite->setPosition( ccp(
            origin.x,
            origin.y + visibleSize.height
        ) );
        addChild( roomSprite, 0 );

        
        // предметы в комнате
        for (auto itr = mItems.begin(); itr != mItems.end(); ++itr) {
            Room::Item& item = *itr;
            const CCRect inAtlas(
                item.slot.textureCoord.x,     item.slot.textureCoord.y,
                item.slot.textureSize.width,  item.slot.textureSize.height
            );
            CCSprite* itemSprite = CCSprite::create( name.c_str(), inAtlas );
            itemSprite->setScaleX( scale.width );
            itemSprite->setScaleY( scale.height );
            const ccBlendFunc blend = { GL_ONE, GL_ONE };
            itemSprite->setBlendFunc( blend );
            itemSprite->setAnchorPoint( ccp( 0, 1 ) );
            const float x =
                (origin.x + item.slot.location.x) * scale.width;
            const float y =
                visibleSize.height -
                (origin.y + item.slot.location.y) * scale.height;
            itemSprite->setPosition( ccp( x, y ) );
            addChild( itemSprite, 1 );
            item.sprite = itemSprite;
#ifdef _DEBUG
            LOG << item << std::endl;
#endif
        } // for (auto itr = items.cbegin(); ...


        // интерфейс
        CCSprite* bgSprite = CCSprite::create( "bg.png" );
        CC_BREAK_IF( !bgSprite );
        bgSprite->setPosition( ccp(
             origin.x + visibleSize.width  / 2,
             origin.y + visibleSize.height / 2
        ) );
        addChild( bgSprite, 1 );


        // оставшееся для исследования комнаты время
        mRestTime = CCLabelTTF::create( "", "Arial", TITLE_FONT_SIZE / 2 );
        mRestTime->setColor( ccc3( 247, 227, 156 ) );
        static const coord_t  coord( 120, visibleSize.height - 1097 );
        mRestTime->setPosition( ccp(
            origin.x + coord.x,
            origin.y + coord.y
        ) );
        addChild( mRestTime, 1 );
        updateRestTime();


        // # Каждую секунду будем уменьшать время на исследование комнаты.
        schedule( schedule_selector( RoomScene::update ),  1.0f );

        auto d = CCDirector::sharedDirector();
        d->getTouchDispatcher()->addTargetedDelegate( this, 0, true );


        bRet = true;

    } while ( false );

    return bRet;
}




void
RoomScene::update( float ) {

    auto player = Player::instance();

    // проверяем, что собрано заданное количество предметов
    if ( player->completed() ) {
        completedSuccess();
        return;
    }

    // бежит время
    player->decreaseRestTime( 1 );
    updateRestTime();
    if (player->restTime() == 0) {
        completedFailure();
    }
}




bool
RoomScene::ccTouchBegan( CCTouch* touch, CCEvent* event ) {

    const auto p = touch->getLocationInView();
    auto itrItem = item( p );
#ifdef _DEBUG
    LOG << "RoomScene::ccTouchBegan() " << p.x << " " << p.y <<
        " -> '";
    if (itrItem != mItems.end()) { LOG << *itrItem; }
    LOG << "'" << std::endl;
#endif

    // предмет найден?
    if (itrItem != mItems.end()) {
        collectedItem( itrItem );
        return true;
    }

    // возвращение на карту дома?
    const bool tlc = pressTopLeftCorner( p );
    if ( tlc ) {
        auto ms = ManagerScreen::instance();
        ms->goToHome();
    }

    return true;
}




void 
RoomScene::ccTouchMoved( CCTouch*, CCEvent* ) {
}




void 
RoomScene::ccTouchEnded( CCTouch*, CCEvent* ) {
}




void 
RoomScene::ccTouchCancelled( CCTouch*, CCEvent* ) {
}




RoomScene::items_t::iterator
RoomScene::item( const coord_t& point ) {

    // учитываем размещение, заданное в init()
    const CCSize visibleSize =
        CCDirector::sharedDirector()->getVisibleSize();
    const CCPoint origin =
        CCDirector::sharedDirector()->getVisibleOrigin();
    const auto scale = mRoom->home()->scaleRoom();
    const coord_t  p(
        (origin.x + point.x) / scale.width,
        (origin.y + point.y) / scale.height
    );

    for (auto itr = mItems.begin(); itr != mItems.end(); ++itr) {
        const Room::Item&  item  = *itr;
        const coord_t&     coord = item.slot.location;
        const dimension_t& size  = item.slot.textureSize;
        const CCRect  box( coord.x, coord.y, size.width, size.height );
        if ( box.containsPoint( p ) ) {
            // точка указывает на этот предмет
            return itr;
        }
    }

    // на этом месте предмета нет
    return mItems.end();
}




void
RoomScene::collectedItem( items_t::iterator& itr ) {

    DASSERT( itr != mItems.cend() );
    DASSERT( itr->sprite
        && "Спрайт для предмета не указан." );

    // сбросим координаты предмета, чтобы он не был найден повторно (пока
    // убираем его со сцены)
    itr->slot.location.x = itr->slot.location.y =
        std::numeric_limits< float >::min();

    // уберём предмет со сцены
    const CCSize visibleSize =
        CCDirector::sharedDirector()->getVisibleSize();
    const auto scale = mRoom->home()->scaleRoom();
    const coord_t  endCoord(
        itr->slot.location.x * scale.width,
        visibleSize.height
    );
    CCActionInterval* move = CCMoveBy::create( 2.0f, endCoord );
    /*
    CCDelayTime* delay = CCDelayTime::create( 0.5f );
    CCAction* remove = CCCallFuncO::create(
        this,  callfuncO_selector( RoomScene::removeItemSeq ),  itr->sprite
    );
    CCFiniteTimeAction* seq =
        CCSequence::create( move, CCCA( delay ), remove, nullptr );
    itr->sprite->runAction( seq );
    */
    CCFiniteTimeAction* remove =
        CCCallFuncN::create( this, callfuncN_selector( RoomScene::removeItemSeq ) );
    itr->sprite->runAction( CCSequence::create( move, remove, nullptr ) );
}




#if 0
void
RoomScene::removeItemSeq( CCObject* o ) {

    DASSERT( o );

    // # Спрайт удалим при выходе из комнаты.
    // @todo fine Удалять спрайт сразу.
    //o->release();

    for (auto itr = mItems.begin(); itr != mItems.end(); ++itr) {
        if (itr->sprite == o) {
            mItems.erase( itr );
            break;
        }
    }

    // отметим, сколько предметов игрок собрал в этой комнате
    const size_t count = countAllItems - mItems.size();
    auto player = Player::instance();
    player->findItems( count );

#ifdef _DEBUG
    std::cout << "Find items in this room: " << count << std::endl;
#endif
}
#endif


void
RoomScene::removeItemSeq( CCNode* sender ) {

    DASSERT( sender );

    // удаляем спрайт из списка и со сцены
    CCSprite* sprite = static_cast< CCSprite* >( sender );
    for (auto itr = mItems.begin(); itr != mItems.end(); ++itr) {
        if (itr->sprite == sprite) {
            mItems.erase( itr );
            break;
        }
    }
    removeChild( sprite, true );

    // отметим, сколько предметов игрок собрал в этой комнате
    const size_t count = countAllItems - mItems.size();
    auto player = Player::instance();
    player->findItems( count );

#ifdef _DEBUG
    std::cout << "Find items in this room: " << count << std::endl;
#endif
}




bool
RoomScene::pressTopLeftCorner( const coord_t& point ) const {
    static const dimension_t  size( 50, 40 );
    return (point.x <= size.width) && (point.y <= size.height);
}




void
RoomScene::updateRestTime() {

    const auto player = Player::instance();
    const auto rt = player->restTime();
    CCString s;
    s.initWithFormat( "%02u : %02u",  (rt / 60),   (rt % 60) );
    mRestTime->setString( s.getCString() );
}




void
RoomScene::completedSuccess() {

    // остановим бег времени
    pauseSchedulerAndActions();

    // запомним результат
    auto player = Player::instance();
    player->subEnergy();
    player->save();

    // покажем диалог
    ManagerDialog::instance()->showResultRoom();
}




void
RoomScene::completedFailure() {

    // остановим бег времени
    pauseSchedulerAndActions();

    // # Провал не запоминаем. Тем не менее, энергию заберём.
    auto player = Player::instance();
    player->subEnergy();

    // покажем диалог
    ManagerDialog::instance()->showResultRoom();
}
