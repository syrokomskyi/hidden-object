#pragma once

#include <cocos2d.h>


// 2D-координаты
typedef cocos2d::CCPoint  coord_t;


// 2D-размер
typedef cocos2d::CCSize  dimension_t;


// 2D-масштаб
typedef cocos2d::CCSize  scale_t;


// 2D-область в виде полигона
typedef std::vector< coord_t >  area_t;


// Ёнерги€ игрока
typedef int  energy_t;







inline std::ostream& operator<<( std::ostream& out,  const coord_t& c ) {
    out.setf( std::ios::fixed, std::ios::floatfield );
    out.precision( 2 );
    out.width( 7 );
    out << c.x << " " << c.y;
    return out;
}




inline std::ostream& operator<<( std::ostream& out,  const dimension_t& s ) {
    out.setf( std::ios::fixed, std::ios::floatfield );
    out.precision( 2 );
    out.width( 7 );
    out << s.width << " " << s.height;
    return out;
}
