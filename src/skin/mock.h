#pragma once

#include "../exp/instant.h"

typedef struct Clock_ Clock;

Clock *new_mock_clock(Instant static_time);
void drop_mock_clock(Clock *);

typedef struct Config_ Config;

Config *new_mock_config(int reservation_limit, Duration cancel_limit);
void drop_mock_config(Config *);

typedef struct Database_ Database;
typedef struct Reservation_ Reservation;

Database *new_mock_database(void);
void drop_mock_database(Database *);
