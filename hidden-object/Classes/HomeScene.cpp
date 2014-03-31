#include "stdafx.h"
#include "HomeScene.h"
#include "Home.h"
#include "ManagerScreen.h"
#include "ManagerDialog.h"
#include "Player.h"


HomeScene::HomeScene(
    std::shared_ptr< Home >  h
):
    mHome( h ),
    mEnergy( nullptr )
{
    DASSERT( mHome );
    DASSERT( this->init() );

    // инициализируем область каждой комнаты
    const auto home = mHome->rootXML();
    DASSERT( home );
    const auto rooms = home->FirstChildElement( "rooms" );
    DASSERT( rooms );
    for (auto root = rooms->FirstChildElement( "room" );  root;
         root = root->NextSiblingElement( "room" )
    ) {
        // name
        const auto nameRoom = root->FirstChildElement( "name" );
        DASSERT( nameRoom );
        const std::string& name = nameRoom->GetText();
        DASSERT( !name.empty() );

        // area
        const auto tn = root->FirstChildElement( "area" );
        const area_t area = Home::area( tn );
        mRoomMap.insert( std::make_pair( name, area ) );

    } // while ...

}




HomeScene::~HomeScene() {
}




CCScene*
HomeScene::scene() {

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
HomeScene::init() {

    bool bRet = false;
    do {
        CC_BREAK_IF( !CCLayerColor::initWithColor( ccc4( 0, 0, 0, 255 ) ) );

        const CCSize visibleSize =
            CCDirector::sharedDirector()->getVisibleSize();
        const CCPoint origin =
            CCDirector::sharedDirector()->getVisibleOrigin();

        
        // план дома + интерфейс
        CCSprite* bgSprite = CCSprite::create( "home.png" );
        CC_BREAK_IF( !bgSprite );
        bgSprite->setPosition( ccp(
            origin.x + visibleSize.width  / 2,
            origin.y + visibleSize.height / 2
        ) );
        addChild( bgSprite, 0 );


        // энергия игрока
        mEnergy = CCLabelTTF::create( "", "Arial", TITLE_FONT_SIZE / 2 );
        mEnergy->setColor( ccc3( 247, 227, 156 ) );
        static const coord_t  coord( 495, visibleSize.height - 75 );
        mEnergy->setAnchorPoint( ccp( 1.0f, 0.5f ) );
        mEnergy->setPosition( ccp(
            origin.x + coord.x,
            origin.y + coord.y
        ) );
        addChild( mEnergy, 1 );
        updateEnergy( 0 );


        // # Каждую секунду будем начислять игроку энергию.
        schedule( schedule_selector( HomeScene::updateEnergy ),  1.0f );

        
        // области комнат
        // # Не показываем. Отрабатываем в ccTouchBegan().


        CCDirector::sharedDirector()->getTouchDispatcher()
            ->addTargetedDelegate( this, 0, true );


        bRet = true;

    } while ( false );

    return bRet;
}




bool 
HomeScene::ccTouchBegan( CCTouch* touch, CCEvent* event ) {

    const auto p = touch->getLocationInView();
    const std::string name = nameRoom( p );
#ifdef _DEBUG
    LOG << "HomeScene::ccTouchBegan() " << p.x << " " << p.y <<
        " -> '" << name << "'" << std::endl;
#endif

    // переходим в комнату?
    if ( !name.empty() ) {
        const auto player = Player::instance();
        player->room( name );
        const auto md = ManagerDialog::instance();
        md->showAboutRoom();
    }

    return true;
}




void 
HomeScene::ccTouchMoved( CCTouch*, CCEvent* ) {
}




void 
HomeScene::ccTouchEnded( CCTouch*, CCEvent* ) {
}




void 
HomeScene::ccTouchCancelled( CCTouch*, CCEvent* ) {
}




std::string
HomeScene::nameRoom( const coord_t& point ) const {

    const auto left = [] ( const coord_t& p0, const coord_t& p1, const coord_t& p2 ) -> int {
        return ( (p1.x - p0.x) * (p2.y - p0.y) -
                 (p2.x - p0.x) * (p1.y - p0.y) );
    };

    for (auto rtr = mRoomMap.cbegin(); rtr != mRoomMap.cend(); ++rtr) {
        const auto& vs = rtr->second;
        DASSERT( vs.front().equals( vs.back() )
            && "Область должна быть замкнута." );

        // @source http://geomalgorithms.com/a03-_inclusion.html
        int wn = 0;
        for (size_t i = 0; i < (vs.size() - 1); ++i) {
            if (vs[ i ].y <= point.y) {
                if (vs[ i + 1 ].y > point.y) {
                    if (left( vs[ i ], vs[ i + 1 ], point ) > 0) {
                        ++wn;
                    }
                }
            } else {
                if (vs[ i + 1 ].y <= point.y) {
                    if (left( vs[ i ], vs[ i + 1 ], point ) < 0) {
                        --wn;
                    }
                }
            }
        }

        if (wn != 0) {
            // комната найдена
            return rtr->first;
        }

    } // for (auto atr = ...

    // точка не указывает на область комнаты
    return "";
}




void
HomeScene::updateEnergy( float ) {

    const auto player = Player::instance();
    player->addEnergy();
    CCString s;
    s.initWithFormat( "%d", player->energy() );
    mEnergy->setString( s.getCString() );
}
