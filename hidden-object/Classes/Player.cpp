#include "stdafx.h"
#include "Player.h"
#include "Home.h"
#include "Store.h"


Player*  Player::mInstance = nullptr;




Player::Player(
    const std::string&             login,
    std::unique_ptr< const Home >  home
):
    login( login ),
    mStore( new Store() ),
    mHome( std::move( home ) ),
    mEnergy( 0 ),
    mLastTimeAddEnergy( 0 ),
    mRoom(),
    mRestTime( 0 ),
    mFindItems( 0 )
{
    DASSERT( mHome );
    DASSERT( !login.empty() );

    mEnergy = mStore->loadLast< energy_t >(
        PLAYER_TABLE_STORE, "energy", login
    );

    mLastTimeAddEnergy = mStore->loadLast< size_t >(
        PLAYER_TABLE_STORE, "lastTimeAddEnergy", login
    );
    if (mLastTimeAddEnergy == 0) { mLastTimeAddEnergy = currentTime() / 1000; }

    // рассчитаем энергию исходя из последнего запуска игры под этим логином
    addEnergy();

    // поможем игроку, предоставив запас энергии, если у него её нет
    if (mEnergy < MIN_ENERGY) {
        mEnergy = MIN_ENERGY;
    }
}




Player::~Player() {
}




void
Player::addEnergy() {

    const auto ct = currentTime() / 1000;
    // # Контролируем хак: ручной перевод времени на компьютере игрока.
    const auto dt = (ct > mLastTimeAddEnergy) ? (ct - mLastTimeAddEnergy) : 0;
    const auto delta = dt / COST_ENERGY;
    if (delta > 0) {
        mEnergy += delta;
        mLastTimeAddEnergy = ct;
    }
}




void
Player::subEnergy() {

    const auto delta = (mEnergy >= needEnergy()) ? needEnergy() : 0;
    if (delta > 0) {
        mEnergy -= delta;
    }
}




size_t
Player::minTime() const {
    DASSERT( !mRoom.empty()
        && "Комната не указана." );
    const size_t min = mStore->loadMin< size_t >(
        PLAYER_TABLE_STORE, "exploreTime", mRoom, login
    );
    return min;
}




energy_t
Player::needEnergy() const {
    DASSERT( !mRoom.empty()
        && "Комната не указана." );
    return mHome->needEnergy( mRoom );
}




bool
Player::enoughEnergy() const {
    DASSERT( !mRoom.empty()
        && "Комната не указана." );
    return mHome->enoughEnergy( mRoom );
}




void
Player::save() {

    const auto exploreTime = timelimit() - restTime();
    std::ostringstream  ss;
    ss <<
        currentTime() << ", " <<
        "'" << login << "', " <<
        mEnergy << ", " <<
        mLastTimeAddEnergy << ", " <<
        "'" << mRoom << "', " <<
        exploreTime << ", " <<
        mFindItems
    ;
    mStore->save( PLAYER_TABLE_STORE,  ss.str() );
}
