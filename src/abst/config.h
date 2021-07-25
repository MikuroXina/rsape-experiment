#pragma once

#include "../exp/instant.h"

typedef struct Config_ {
  int (*_reservation_limit)(struct Config_ const *self);
  Duration (*_cancel_limit)(struct Config_ const *self);
} Config;

static inline int config_reservation_limit(Config const *self) {
  return self->_reservation_limit(self);
}

static inline Duration config_cancel_limit(Config const *self) {
  return self->_cancel_limit(self);
}
