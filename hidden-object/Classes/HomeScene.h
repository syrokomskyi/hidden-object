#pragma once

#include "structure.h"


class Home;


class HomeScene : public cocos2d::CCLayerColor {
public:
    HomeScene( std::shared_ptr< Home > );

    virtual ~HomeScene();


    // Here's a difference. Method 'init' in cocos2d-x returns bool,
    // instead of returning 'id' in cocos2d-iphone.
    virtual bool init();


    // there's no 'id' in cpp, so we recommand to return the
    // exactly class pointer
    cocos2d::CCScene* scene();


    virtual bool ccTouchBegan( CCTouch*, CCEvent* );
    virtual void ccTouchMoved( CCTouch*, CCEvent* );
    virtual void ccTouchEnded( CCTouch*, CCEvent* );
    virtual void ccTouchCancelled( CCTouch*, CCEvent* );


    inline std::shared_ptr< const Home > home() const { return mHome; }
    inline std::shared_ptr< Home >       home()       { return mHome; }


    // @return Ќазвание комнаты, в которой лежит заданна€ точка.
    //         ѕуста€ строка, если комната не найдена.
    std::string nameRoom( const coord_t& ) const;


private:
    void updateEnergy( float );


private:
    std::shared_ptr< Home >  mHome;
    
    CCLabelTTF*  mEnergy;

    // —писок названий комнат и областей, которые им соответствуют.
    std::map< std::string, area_t >  mRoomMap;
};
