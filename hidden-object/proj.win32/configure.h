#pragma once

#include <string>


// Путь к папке ресурсов.
static const std::string  PATH_RESOURCE = "Resources";


// Путь к папке долговременного хранилища и названия таблиц.
static const std::string  PATH_STORE = PATH_RESOURCE + "/store";
static const std::string  PLAYER_TABLE_STORE = "player";


// Название XML-файла, в котором лежит информация о доме.
static const std::string  HOME_FILE_NAME = "home.xml";


// Тестовый логин подключившегося игрока.
static const std::string  LOGIN = "test@email.com";


// Минимальное кол-во энергии, которое получает игрок при запуске игры.
#ifdef _DEBUG
// # Чтобы сразу зайти в одну из комнат.
static const size_t MIN_ENERGY = 30;
#else
static const size_t MIN_ENERGY = 0;
#endif


// Отношение количества начисляемой игроку энерги ко времени, сек.
// Например, при == 10, энергия начисляется каждую секунду.
static const size_t COST_ENERGY = 10;


// Сколько предметов игрок должен найти в комнате, чтобы комната
// считалась пройденной.
// # Значение введено для тестирование, чтобы не выискивать все предметы, а
//   быстро получить результат.
static const size_t COMPLETED_COUNT_ITEMS = 4;


// Для отладки.
#ifdef _DEBUG
#define ASSERT(EXPR)   assert(EXPR);
#define DASSERT(EXPR)  if (!(EXPR)) __debugbreak();

#define QUOTE_(WHAT)      #WHAT
#define QUOTE(WHAT)       QUOTE_(WHAT)
#define DBG(format, ...)  printf("%s: "format, __FILE__":"QUOTE(__LINE__), ## __VA_ARGS__)

#define LOG  std::cout

#else
#define ASSERT(EXPR)      ((void)0)
#define DASSERT(EXPR)     ((void)0)
#define DBG(format, ...)  ((void)0)
#define LOG           ((void)0)

#endif
