#include "stdafx.h"
#include "NeedEnergyDialog.h"


NeedEnergyDialog::NeedEnergyDialog():
    mChild( nullptr )
{
    DASSERT( this->init() );
}




NeedEnergyDialog::~NeedEnergyDialog() {
    auto d = CCDirector::sharedDirector();
    CCScene* scene = d->getRunningScene();
    scene->removeChild( this );
}




void
NeedEnergyDialog::dialog() {

    auto d = CCDirector::sharedDirector();
    CCScene* scene = d->getRunningScene();
    do {
        scene->addChild( this );

    } while ( false );
}




bool
NeedEnergyDialog::init() {

    bool bRet = false;
    do {
        CC_BREAK_IF( !CCLayerColor::init() );

        const CCSize visibleSize =
            CCDirector::sharedDirector()->getVisibleSize();
        const CCPoint origin =
            CCDirector::sharedDirector()->getVisibleOrigin();

        auto d = CCDirector::sharedDirector();
        
        // виз. форма диалога
        CCSprite* bgSprite = CCSprite::create( "needEnergy.png" );
        CC_BREAK_IF( !bgSprite );
        bgSprite->setPosition( ccp(
            origin.x + visibleSize.width  / 2,
            origin.y + visibleSize.height * 2 / 3
        ) );
        mChild = bgSprite;
        addChild( mChild, 1 );

        
        CCDirector::sharedDirector()->getTouchDispatcher()
            ->addTargetedDelegate( this, 0, true );


        bRet = true;

    } while ( false );

    return bRet;
}




bool 
NeedEnergyDialog::ccTouchBegan( CCTouch* touch, CCEvent* event ) {

    const auto p = touch->getLocationInView();
#ifdef _DEBUG
    LOG << "NeedEnergyDialog::ccTouchBegan() " << p.x << " " << p.y << std::endl;
#endif

    // нажата кнопка отмены?
    const bool cancel = pressCancel( p );
    if ( cancel ) {
        auto d = CCDirector::sharedDirector();
        d->getTouchDispatcher()->removeDelegate( this );
        removeChild( mChild );
    }

    // @todo нажата кнопка магазина?

    return true;
}




void 
NeedEnergyDialog::ccTouchMoved( CCTouch*, CCEvent* ) {
}




void 
NeedEnergyDialog::ccTouchEnded( CCTouch*, CCEvent* ) {
}




void 
NeedEnergyDialog::ccTouchCancelled( CCTouch*, CCEvent* ) {
}




bool
NeedEnergyDialog::pressCancel( const coord_t& point ) const {
    // #  нопка отмены этого диалога - в форме круга.
    static const coord_t  coord( 530, 248 );
    static const float radius = 30.0f;
    return (std::abs( point.x - coord.x ) <= radius)
        && (std::abs( point.y - coord.y ) <= radius);
}
