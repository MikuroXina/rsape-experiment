#include "./stub.h"

#include <stdlib.h>
#include <string.h>

#include "../abst/controller.h"
#include "../abst/database.h"
#include "../exp/condition.h"
#include "../exp/reservation.h"

typedef struct {
  Repository repo;
  Reservation **reservations;
  size_t reservations_len;
  Reservation **query_buf;
} StubRepository;

static Reservation *stub_get_by_id(Repository *r, char const *id) {
  StubRepository *s = (StubRepository *)r;
  for (size_t i = 0; i < s->reservations_len; ++i) {
    if (strcmp(s->reservations[i]->id, id) == 0) {
      return s->reservations[i];
    }
  }
  return NULL;
}

static Reservation **stub_query_reservations(Repository *r,
    Condition const *cond) {
  StubRepository *s = (StubRepository *)r;
  s->query_buf = calloc(s->reservations_len, sizeof(Reservation *));
  if (s->query_buf != NULL) {
    free(s->query_buf);
  }
  size_t buf_idx = 0;
  for (size_t i = 0; i < s->reservations_len; ++i) {
    if (is_matched(cond, s->reservations[i])) {
      s->query_buf[buf_idx] = s->reservations[i];
      ++buf_idx;
    }
  }
  return s->query_buf;
}

Repository *new_stub_repository(Reservation **reservations,
    size_t reservations_len) {
  StubRepository *s = malloc(sizeof(StubRepository));
  s->repo._get_by_id = stub_get_by_id;
  s->repo._query_reservations = stub_query_reservations;
  s->reservations = reservations;
  s->reservations_len = reservations_len;
  s->query_buf = NULL;
  return (Repository *)s;
}

void drop_stub_repository(Repository *r) {
  StubRepository *s = (StubRepository *)r;
  if (s->query_buf != NULL) {
    free(s->query_buf);
  }
  free(s);
}

typedef struct {
  InputController con;
  Instant from;
  Instant to;
  Condition *condition;
  char const *reservation_id;
  char const *visitor_id;
} StubInputController;

static Instant stub_instant_from(InputController *i) {
  StubInputController *s = (StubInputController *)i;
  return s->from;
}

static Instant stub_instant_to(InputController *i) {
  StubInputController *s = (StubInputController *)i;
  return s->to;
}

static Condition *stub_cond(InputController *i) {
  StubInputController *s = (StubInputController *)i;
  return s->condition;
}

static char const *stub_reservation_id(InputController *i) {
  StubInputController *s = (StubInputController *)i;
  return s->reservation_id;
}

static char const *stub_visitor_id(InputController *i) {
  StubInputController *s = (StubInputController *)i;
  return s->visitor_id;
}

InputController *new_stub_input_controller(Instant from,
    Instant to,
    Condition *condition,
    char const *reservation_id,
    char const *visitor_id) {
  StubInputController *s = malloc(sizeof(StubInputController));
  s->con._input_instant_from = stub_instant_from;
  s->con._input_instant_to = stub_instant_to;
  s->con._input_condition = stub_cond;
  s->con._input_reservation_id = stub_reservation_id;
  s->con._input_visitor_id = stub_visitor_id;
  s->from = from;
  s->to = to;
  s->condition = condition;
  s->reservation_id = reservation_id;
  s->visitor_id = visitor_id;
  return (InputController *)s;
}

void drop_stub_input_controller(InputController *i) {
  free(i);
}
