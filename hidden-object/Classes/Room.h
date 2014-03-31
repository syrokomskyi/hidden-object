#pragma once

#include "structure.h"


class Home;


class Room {
public:
    // предмет в комнате
    struct Item;


public:
    // Название комнаты.
    // @see Соглашения в home.xml.
    const std::string  name;

    // Лимит времени на прохождение комнаты, сек.
    const size_t  timelimit;

    // Минимальная энергия, необходимая для исследования комнаты.
    const energy_t  needEnergy;

    // Область комнаты в доме.
    const area_t  area;


public:
    Room(
        std::unique_ptr< const Home >  home,
        const std::string&  name,
        size_t              timelimit,
        energy_t            needEnergy,
        const area_t&       area
    );


    virtual ~Room();


    inline const Home*  home() const { return mHome.get(); }


    // @return Предметы в комнате.
    // Предметы располагаются в случайных слотах, с учётом intersections (см.
    // exmpl.xml).
    // "Случайность" меняется после вызова nextRandomSeed().
    std::vector< Item >  currentItemSet() const;


    // Инициализирует генератор случ. чисел новым значением.
    // Влияет на расположение предметов в комнате - см. nextRandomSeed
    inline void nextRandomSeed() { mSeed = rand(); }


private:
    std::unique_ptr< const Home >  mHome;

    tinyxml2::XMLDocument  mXML;

    size_t  mSeed;
};








struct Room::Item {

    struct Slot {
        // вторая часть строкового идентификатора предмета (clipName + addName)
        std::string  addName;

        // числовой идентификатор предмета
        int  id;

        // координаты предмета в комнате
        coord_t  location;

        // расположение и размеры текстуры предмета в текстурном атласе
        coord_t      textureCoord;
        dimension_t  textureSize;
    };


    // ключ локализации для данного предмета
    std::string  name;

    // первая часть строкового идентификатора предмета
    std::string  clipName;

    // расположение и размер текстурного региона предмета в атласе
    // для отображения на панели поиска
    coord_t      searchTextureCoord;
    dimension_t  searchTextureSize;

    // выбранное расположение предмета в комнате
    // @see currentItemSet()
    Slot  slot;

    // спрайт на экране
    CCSprite*  sprite;
};








std::ostream& operator<<( std::ostream&,  const Room::Item& );
