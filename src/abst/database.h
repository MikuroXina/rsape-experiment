#pragma once

#include <stdbool.h>

typedef struct Condition_ Condition;
typedef struct Reservation_ Reservation;

typedef struct Repository_ {
  Reservation *(*_get_by_id)(struct Repository_ *self, char const *id);
  Reservation **(
      *_query_reservations)(struct Repository_ *self, Condition const *cond);
} Repository;

typedef struct Database_ {
  bool (*_register_reservation)(struct Database_ *self, Reservation const *r);
  bool (*_stale_reservation)(struct Database_ *self, char const *id);
  bool (*_done_reservation)(struct Database_ *self, char const *id);
} Database;

static inline Reservation *repository_get_by_id(Repository *self,
    char const *id) {
  return self->_get_by_id(self, id);
}

static inline Reservation **repository_query_reservations(Repository *self,
    Condition const *cond) {
  return self->_query_reservations(self, cond);
}

static inline bool database_register_reservation(Database *self,
    Reservation const *r) {
  return self->_register_reservation(self, r);
}

static inline bool database_stale_reservation(Database *self, char const *id) {
  return self->_stale_reservation(self, id);
}

static inline bool database_done_reservation(Database *self, char const *id) {
  return self->_done_reservation(self, id);
}
