#pragma once

#include "structure.h"


class AppDelegate;
class Home;
class Store;


// Информация об игроке.
class Player {
public:
    // Время, оставшееся на исследования комнаты.
    typedef std::pair< size_t /* min */, size_t /* sec */ >  restTime_t;


    const std::string  login;


private:
    Player(
        const std::string&             login,
        std::unique_ptr< const Home >  home
    );


public:
    static inline Player* instance() {
        DASSERT( mInstance
            && "Игрок должен быть иниццирован до обращения к instance()." );
        return mInstance;
    }


    virtual ~Player();


    // @return Текущая энергия игрока.
    inline energy_t energy() const { return mEnergy; }


    // @return Время, когда произошло последнее начисление энергии, сек.
    inline size_t lastTimeAddEnergy() const { return mLastTimeAddEnergy; }


    // Вызывается для начисление энергии.
    void addEnergy();


    // Вызывается для уменьшения энергии (при вхождении в комнату).
    void subEnergy();


    // @return Комната, в которой находится игрок или в которую собирается зайти.
    inline void room( const std::string& name ) { mRoom = name; }

    inline std::string room() const { return mRoom; }


    // @return Лимит времени на исследование комнаты, сек.
    inline void timelimit( size_t limit ) { mTimelimit = limit; }

    inline size_t timelimit() const { return mTimelimit; }


    // @return Время, оставшееся на исследование комнаты, сек.
    inline void restTime( size_t rt ) { mRestTime = rt; }

    inline size_t restTime() const { return mRestTime; }


    // @return Количество найденных в комнате предметов.
    inline void findItems( size_t count ) { mFindItems = count; }

    inline size_t findItems() const { return mFindItems; }


    // @return Комната успешно завершена.
    inline bool completed() const {
        return (findItems() >= COMPLETED_COUNT_ITEMS);
    }


    // @return Минимальное время, за которое была пройдена комната, сек.
    size_t minTime() const;


    // Уменьшает оставшееся на исследование комнаты время.
    inline void decreaseRestTime( size_t dt ) {
        if (mRestTime >= dt) { mRestTime -= dt; }
    }


    // @return Сколько нужно игроку энергии для исследования комнаты.
    energy_t needEnergy() const;


    // @return У игрока достаточно энергии для прохождения комнаты.
    bool enoughEnergy() const;


    // @return Текущее время в игре, мс.
    inline size_t currentTime() const {
        struct cc_timeval  now;
        CCTime::gettimeofdayCocos2d( &now, nullptr );
        const auto ct = (now.tv_sec * 1000 + now.tv_usec / 1000);
        return static_cast< size_t >( ct );
    }


    // Сохраняет текущий прогресс в долговременном хранилище.
    void save();


private:
    // Информация о доме.
    std::unique_ptr< const Home >  mHome;

    // Текущая энергия игрока.
    energy_t  mEnergy;

    // Время, когда произошло последнее начисление энергии, сек.
    size_t  mLastTimeAddEnergy;

    // Комната, в которой находится игрок или в которую собирается зайти.
    std::string  mRoom;

    // Лимит времени на исследование комнаты, сек.
    size_t  mTimelimit;

    // Время, оставшееся на исследование комнаты, сек.
    size_t  mRestTime;

    // Количество найденных в комнате предметов.
    size_t  mFindItems;

    // Долговременное хранилище.
    std::unique_ptr< Store >  mStore;

    static Player*  mInstance;

    friend AppDelegate;
};
