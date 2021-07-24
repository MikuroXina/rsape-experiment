#pragma once

#include "instant.h"

typedef enum ReservationState_ { Active, Stale, Done } ReservationState;

typedef struct Reservation_ {
  char *id;
  ReservationState state;
  Instant start;
  Instant end;
  char const *visitor_id;
} Reservation;

Reservation *new_reservation(ReservationState state,
    Instant start,
    Instant end,
    char const *visitor_id);

void drop_reservation(Reservation *r);
