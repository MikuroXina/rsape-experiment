#pragma once

#include <stdbool.h>
#include <time.h>

typedef struct Instant_ {
  unsigned long seconds;
} Instant;

static inline Instant instant_from_time(time_t t) {
  Instant i = {(unsigned long)t};
  return i;
}

static inline time_t instant_as_time(Instant i) {
  return (time_t)i.seconds;
}

static inline bool instant_less_than(Instant l, Instant r) {
  return l.seconds < r.seconds;
}

static inline struct tm to_local_time(Instant i) {
  time_t t = instant_as_time(i);
  struct tm t_tm;
  localtime_r(&t, &t_tm);
  return t_tm;
}

typedef struct Duration_ {
  unsigned long seconds;
} Duration;

static inline Duration duration_from_time(time_t t) {
  Duration d = {(unsigned long)t};
  return d;
}

static inline time_t duration_as_time(Duration d) {
  return (time_t)d.seconds;
}

static inline Instant instant_add(Instant l, Duration r) {
  Instant i = {l.seconds + r.seconds};
  return i;
}

typedef struct HourMinute_ {
  unsigned hour;
  unsigned miniute;
} HourMinute;

typedef enum DayOfWeek_ {
  Sunday,
  Monday,
  Tuesday,
  Wednesday,
  Thursday,
  Friday,
  Saturaday,
  MAX_DAY_OF_WEEK
} DayOfWeek;
