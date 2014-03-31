#include "stdafx.h"
#include "Home.h"
#include "Room.h"
#include "Player.h"
#include "ManagerScreen.h"


Home::Home( const std::string& fileXML ):
    mXML( true, tinyxml2::COLLAPSE_WHITESPACE )
{
    DASSERT( !fileXML.empty() );
    const tinyxml2::XMLError error = mXML.LoadFile( fileXML.c_str() );
    DASSERT( error == tinyxml2::XML_SUCCESS );
}




Home::~Home() {
}




dimension_t
Home::sizeRoomAtlas() const {

    const auto home = mXML.FirstChildElement( "home" );
    DASSERT( home );
    const auto atlas = home->FirstChildElement( "atlas" );
    DASSERT( atlas );
    const auto rootSize = atlas->FirstChildElement( "sizeRoom" );
    DASSERT( rootSize );

    return size( rootSize );
}




dimension_t
Home::sizeRoomInterface() const {

    const auto home = mXML.FirstChildElement( "home" );
    DASSERT( home );
    const auto interface = home->FirstChildElement( "interface" );
    DASSERT( interface );
    const auto rootSize = interface->FirstChildElement( "sizeRoom" );
    DASSERT( rootSize );

    return size( rootSize );
}




scale_t
Home::scaleRoom() const {
    const auto si = sizeRoomInterface();
    const auto sa = sizeRoomAtlas();
    return scale_t( si.width  / sa.width,  si.height / sa.height );
}




Home::roomSet_t
Home::allRooms() const {

    roomSet_t r;
    const auto home = mXML.FirstChildElement( "home" );
    DASSERT( home );
    const auto rooms = home->FirstChildElement( "rooms" );
    DASSERT( rooms );
    for (auto root = rooms->FirstChildElement( "room" );  root;
         root = root->NextSiblingElement( "room" )
    ) {
        const auto nameRoom = root->FirstChildElement( "name" );
        DASSERT( nameRoom );
        r.insert( nameRoom->GetText() );
    }

    return r;
}




std::shared_ptr< Room >
Home::room( const std::string& searchName ) const {

    DASSERT( !searchName.empty() );

    std::string  name;
    size_t       timelimit  = 0;
    size_t       needEnergy = 0;
    area_t       area;

    const auto home = mXML.FirstChildElement( "home" );
    DASSERT( home );
    const auto rooms = home->FirstChildElement( "rooms" );
    DASSERT( rooms );
    for (auto root = rooms->FirstChildElement( "room" );  root;
         root = root->NextSiblingElement( "room" )
    ) {
        const auto nameRoom = root->FirstChildElement( "name" );
        DASSERT( nameRoom );
        name = nameRoom->GetText();
        if (searchName == name) {

            // timelimit
            {
                const auto tn = root->FirstChildElement( "timelimit" );
                DASSERT( tn );
                timelimit = static_cast< size_t >( 60.0f * std::atof( tn->GetText() ) );
                DASSERT( timelimit > 0 );
            }

            // needEnergy
            {
                const auto tn = root->FirstChildElement( "needEnergy" );
                DASSERT( tn );
                needEnergy = static_cast< size_t >( std::atoi( tn->GetText() ) );
                DASSERT( needEnergy > 0 );
            }

            //area
            {
                const auto tn = root->FirstChildElement( "area" );
                DASSERT( tn );
                area = Home::area( tn );
            }

            // # Ќазвани€ комнат уникальны.
            break;

        } // if (searchName == name)

    } // while ...

    const auto ms = ManagerScreen::instance();
    std::unique_ptr< const Home >  h( new Home(
        PATH_RESOURCE + "/" + ms->selectedResource.directory + "/" + HOME_FILE_NAME
    ) );

    return std::shared_ptr< Room >( new Room(
        std::move( h ),  name,  timelimit,  needEnergy,  area
    ) );
}




energy_t
Home::needEnergy( const std::string& nameRoom ) const {

    const auto r = room( nameRoom );
    DASSERT( r );

    return r->needEnergy;
}




bool
Home::enoughEnergy( const std::string& nameRoom ) const {
    const auto player = Player::instance();
    return (player->energy() >= needEnergy( nameRoom ));
}




area_t
Home::area( const tinyxml2::XMLElement* rootArea ) {

    DASSERT( rootArea );

    area_t  area;

    std::istringstream  ss( rootArea->GetText() );
    std::string  part;
    coord_t  coord( 0, 0 );
    size_t count = 0;
    while ( std::getline( ss, part, ' ' ) ) {
        if ((count % 2) == 0) {
            coord.x = std::atoi( part.c_str() );
        } else {
            coord.y = std::atoi( part.c_str() );
            area.push_back( coord );
        }
        ++count;
    }

    DASSERT( ((area.size() % 2) == 0)
        && "ѕолучено непарное количество координат дл€ создани€ области." );
    DASSERT( !area.front().equals( area.back() )
        && "¬ XML не следует замыкать область: замыкание будет сделано автоматически." );

    // замыкаем область
    area.push_back( area.front() );

    return area;
}




dimension_t
Home::size( const tinyxml2::XMLElement* rootSize ) {

    DASSERT( rootSize );

    std::istringstream  ss( rootSize->GetText() );
    std::string  part;
    dimension_t  dimension( 0, 0 );
    size_t count = 0;
    while ( std::getline( ss, part, ' ' ) ) {
        if ((count % 2) == 0) {
            dimension.width = std::atoi( part.c_str() );
        } else {
            dimension.height = std::atoi( part.c_str() );
        }
        ++count;
    }

    DASSERT( (count == 2)
        && "ƒл€ задани€ размера ожидаетс€ 2 значени€." );
    DASSERT( ((dimension.width > 0) && (dimension.height > 0))
        && "–азмер должен быть больше 0." );

    return dimension;
}
