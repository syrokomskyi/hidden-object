#pragma once

#include "structure.h"


class AboutRoomDialog : public cocos2d::CCLayerColor {
public:
    AboutRoomDialog();

    virtual ~AboutRoomDialog();


    // Here's a difference. Method 'init' in cocos2d-x returns bool,
    // instead of returning 'id' in cocos2d-iphone.
    virtual bool init();


    void dialog();


    virtual bool ccTouchBegan( CCTouch*, CCEvent* );
    virtual void ccTouchMoved( CCTouch*, CCEvent* );
    virtual void ccTouchEnded( CCTouch*, CCEvent* );
    virtual void ccTouchCancelled( CCTouch*, CCEvent* );


private:
    bool pressCancel(   const coord_t& ) const;
    bool pressClose(    const coord_t& ) const;
    bool pressExplore(  const coord_t& ) const;


private:
    CCNode*  mChild;
};
