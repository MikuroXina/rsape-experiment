#pragma once

#include "../exp/instant.h"

#include <stddef.h>

typedef struct Condition_ Condition;
typedef struct Repository_ Repository;
typedef struct Reservation_ Reservation;

Repository *new_stub_repository(Reservation **reservations,
    size_t reservations_len);
void drop_stub_repository(Repository *);

typedef struct InputController_ InputController;

InputController *new_stub_input_controller(Instant from,
    Instant to,
    Condition *condition,
    char const *reservation_id,
    char const *visitor_id);
void drop_stub_input_controller(InputController *);
