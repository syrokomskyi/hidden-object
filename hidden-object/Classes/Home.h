#pragma once

#include "structure.h"


class Room;


// Информация о доме.
class Home : public std::enable_shared_from_this< Home > {
public:
    typedef std::set< std::string >  roomSet_t;


public:
    Home( const std::string& fileXML );

    virtual ~Home();


    // @return Размер изображения комнаты в атласе.
    dimension_t sizeRoomAtlas() const;


    // @return Размер изображения комнаты в игровом интерфейсе.
    dimension_t sizeRoomInterface() const;


    // @return Масштаб комнаты.
    scale_t scaleRoom() const;


    // @return Список всех комнат в доме.
    roomSet_t allRooms() const;


    // # Если комнаты с заданным именем не существует, возвращает nullptr.
    std::shared_ptr< Room >  room( const std::string& name ) const;


    // @return Сколько энергии необходимо игроку для исследования комнаты.
    energy_t needEnergy( const std::string& nameRoom ) const;


    // @return Игроку хватает энергии для исследования комнаты.
    bool enoughEnergy( const std::string& nameRoom ) const;


    inline const tinyxml2::XMLElement*  rootXML() const { return mXML.RootElement(); }


    // # Область замыкается автоматически (в XML не следует указывать последнюю
    //   вершину == первой).
    static area_t area( const tinyxml2::XMLElement* rootArea );


    static dimension_t size( const tinyxml2::XMLElement* rootSize );


private:
    tinyxml2::XMLDocument  mXML;
};
