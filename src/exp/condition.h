#pragma once

#include <stdbool.h>

#include "./instant.h"

typedef struct Reservation_ Reservation;
typedef struct Condition_ Condition;

Condition *cond_later_than(Instant t);
Condition *cond_earlier_than(Instant t);
Condition *cond_in_time_of_day(HourMinute from, HourMinute to);
Condition *cond_is_day_of_week(DayOfWeek day);
Condition *cond_all(void);
Condition *cond_or(Condition *l, Condition *r);
Condition *cond_and(Condition *l, Condition *r);
Condition *cond_not(Condition *term);

void drop_condition(Condition *c);

bool is_matched(Condition const *self, Reservation const *r);
