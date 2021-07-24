#pragma once

#include "../exp/instant.h"

typedef struct Condition_ Condition;
typedef struct Reservation_ Reservation;

typedef struct InputController_ {
  Instant (*_input_instant_from)(struct InputController_ *self);
  Instant (*_input_instant_to)(struct InputController_ *self);
  Condition *(*_input_condition)(struct InputController_ *self);
  char const *(*_input_reservation_id)(struct InputController_ *self);
  char const *(*_input_visitor_id)(struct InputController_ *self);
} InputController;

static inline Instant input_controller_input_instant_from(
    InputController *self) {
  return self->_input_instant_from(self);
}

static inline Instant input_controller_input_instant_to(InputController *self) {
  return self->_input_instant_to(self);
}

static inline Condition *input_controller_input_condition(
    InputController *self) {
  return self->_input_condition(self);
}

static inline char const *input_controller_input_reservation_id(
    InputController *self) {
  return self->_input_reservation_id(self);
}

static inline char const *input_controller_input_visitor_id(
    InputController *self) {
  return self->_input_visitor_id(self);
}

typedef struct ReserveViewController_ {
  void (*show_reservations)(Reservation **found);
  void (*not_found)(void);
} ReserveViewController;

typedef struct RegisterViewController_ {
  void (*error_on_register)(char const *reason);
  void (*success_on_register)(void);
} RegisterViewController;

typedef struct CancelViewController_ {
  void (*error_on_cancel)(char const *reason);
  void (*success_on_cancel)(void);
} CancelViewController;

typedef struct DoneViewController_ {
  void (*error_on_done)(char const *reason);
  void (*success_on_done)(void);
} DoneViewController;
