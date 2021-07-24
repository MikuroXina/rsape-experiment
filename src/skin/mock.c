#include "./mock.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "../abst/clock.h"
#include "../abst/config.h"
#include "../abst/database.h"
#include "../exp/reservation.h"

typedef struct {
  Clock clock;
  Instant static_time;
} MockClock;

static Instant get_instant_mock_clock(Clock *c) {
  MockClock *m = (MockClock *)c;
  return m->static_time;
}

Clock *new_mock_clock(Instant static_time) {
  MockClock *m = malloc(sizeof(MockClock));
  m->clock._get_instant = get_instant_mock_clock;
  m->static_time = static_time;
  return (Clock *)m;
}

void drop_mock_clock(Clock *c) {
  free(c);
}

typedef struct {
  Config config;
  int reservation_limit;
  Duration cancel_limit;
} MockConfig;

static int mock_config_reservation_limit(Config *c) {
  MockConfig *m = (MockConfig *)c;
  return m->reservation_limit;
}

static Duration mock_config_cancel_limit(Config *c) {
  MockConfig *m = (MockConfig *)c;
  return m->cancel_limit;
}

Config *new_mock_config(int reservation_limit, Duration cancel_limit) {
  MockConfig *m = malloc(sizeof(MockConfig));
  m->config._reservation_limit = mock_config_reservation_limit;
  m->config._cancel_limit = mock_config_cancel_limit;
  m->reservation_limit = reservation_limit;
  m->cancel_limit = cancel_limit;
  return (Config *)m;
}

void drop_mock_config(Config *c) {
  free(c);
}

typedef struct {
  Reservation *p;
  size_t len;
  size_t cap;
} ReservationVec;

static ReservationVec *new_reservation_vec() {
  ReservationVec *vec = malloc(sizeof(ReservationVec));
  vec->p = malloc(sizeof(Reservation));
  vec->len = 0;
  vec->cap = 1;
  return vec;
}

static void drop_reservation_vec(ReservationVec *vec) {
  free(vec->p);
  free(vec);
}

static void push_reservation_vec(ReservationVec *vec, Reservation node) {
  if (vec->cap <= vec->len + 1) {
    vec->cap *= 2;
    vec->p = realloc(vec->p, vec->cap);
  }
  vec->p[vec->len] = node;
  ++vec->len;
}

static Reservation const *find_id_reservation_vec(ReservationVec const *vec,
    char const *id) {
  long idx = -1;
  for (size_t i = 0; i < vec->len; ++i) {
    if (strcmp(vec->p[i].id, id) == 0) {
      idx = (long)i;
    }
  }
  if (idx < 0) {
    return NULL;
  }
  return vec->p + idx;
}

static void remove_reservation_vec(ReservationVec *vec, size_t idx) {
  for (size_t i = idx; i + 1 < vec->len; ++i) {
    vec->p[i] = vec->p[i + 1];
  }
  drop_reservation(vec->p + vec->len);
  --vec->len;
}

typedef struct {
  Database database;
  ReservationVec *active;
  ReservationVec *stale;
  ReservationVec *done;
} MockDatabase;

static bool mock_register_reservation(Database *d, Reservation const *r) {
  MockDatabase *m = (MockDatabase *)d;
  push_reservation_vec(m->active, *r);
  return true;
}

static bool mock_stale_reservation(Database *d, char const *id) {
  MockDatabase *m = (MockDatabase *)d;
  Reservation const *found = find_id_reservation_vec(m->active, id);
  if (found == NULL) {
    return true;
  }
  push_reservation_vec(m->stale, *found);
  remove_reservation_vec(m->active, (size_t)(found - m->active->p));
  return true;
}

static bool mock_done_reservation(Database *d, char const *id) {
  MockDatabase *m = (MockDatabase *)d;
  Reservation const *found = find_id_reservation_vec(m->active, id);
  if (found == NULL) {
    return true;
  }
  push_reservation_vec(m->done, *found);
  remove_reservation_vec(m->active, (size_t)(found - m->active->p));
  return true;
}

Database *new_mock_database() {
  MockDatabase *m = malloc(sizeof(MockDatabase));
  m->database._register_reservation = mock_register_reservation;
  m->database._stale_reservation = mock_stale_reservation;
  m->database._done_reservation = mock_done_reservation;
  m->active = new_reservation_vec();
  m->stale = new_reservation_vec();
  m->done = new_reservation_vec();
  return (Database *)m;
}

void drop_mock_database(Database *d) {
  MockDatabase *m = (MockDatabase *)d;
  drop_reservation_vec(m->done);
  drop_reservation_vec(m->stale);
  drop_reservation_vec(m->active);
  free(m);
}

Reservation const *active_reservations(Database const *d) {
  MockDatabase const *m = (MockDatabase const *)d;
  return m->active->p;
}

Reservation const *stale_reservations(Database const *d) {
  MockDatabase const *m = (MockDatabase const *)d;
  return m->stale->p;
}

Reservation const *done_reservations(Database const *d) {
  MockDatabase const *m = (MockDatabase const *)d;
  return m->done->p;
}
