#pragma once

#include "../exp/instant.h"

typedef struct Config_ {
  int (*_reservation_limit)(struct Config_ *self);
  Duration (*_cancel_limit)(struct Config_ *self);
} Config;

static inline int config_reservation_limit(Config *self) {
  return self->_reservation_limit(self);
}

static inline Duration config_cancel_limit(struct Config_ *self) {
  return self->_cancel_limit(self);
}
