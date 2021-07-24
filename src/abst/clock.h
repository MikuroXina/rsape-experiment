#pragma once

#include "../exp/instant.h"

typedef struct Clock_ {
  Instant (*_get_instant)(struct Clock_ *self);
} Clock;

static inline Instant clock_get_instant(Clock *self) {
  return self->_get_instant(self);
}
