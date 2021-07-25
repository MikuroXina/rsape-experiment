#include "./condition.h"
#include "./reservation.h"

#include <stdlib.h>

typedef bool (*Matcher)(Condition const *self, Reservation const *r);
typedef void (*Destructor)(Condition *self);

typedef struct Condition_ {
  Matcher matcher;
  Destructor destructor;
} Condition;

static void do_nothing_destructor(Condition *_) {}

typedef struct {
  Condition cond;
  Instant t;
} ConditionInstant;

static bool later_than_matcher(Condition const *self, Reservation const *r) {
  ConditionInstant const *c = (ConditionInstant const *)self;
  return instant_less_than(c->t, r->start);
}

Condition *cond_later_than(Instant t) {
  ConditionInstant *c = malloc(sizeof(ConditionInstant));
  c->cond.matcher = later_than_matcher;
  c->cond.destructor = do_nothing_destructor;
  c->t = t;
  return (Condition *)c;
}

static bool earlier_than_matcher(Condition const *self, Reservation const *r) {
  ConditionInstant const *c = (ConditionInstant const *)self;
  return instant_less_than(r->start, c->t);
}

Condition *cond_earlier_than(Instant t) {
  ConditionInstant *c = malloc(sizeof(ConditionInstant));
  c->cond.matcher = earlier_than_matcher;
  c->cond.destructor = do_nothing_destructor;
  c->t = t;
  return (Condition *)c;
}

typedef struct {
  Condition cond;
  HourMinute from;
  HourMinute to;
} ConditionTimeOfDay;

static bool in_time_of_day_matcher(Condition const *self,
    Reservation const *r) {
  ConditionTimeOfDay const *c = (ConditionTimeOfDay const *)self;
  struct tm start_tm = to_local_time(r->start);
  struct tm end_tm = to_local_time(r->end);
  int start_minutes = start_tm.tm_hour * 60 + start_tm.tm_min;
  int end_minutes = end_tm.tm_hour * 60 + end_tm.tm_min;
  int from_miniutes = (int)(c->from.hour * 60 + c->from.miniute);
  int to_miniutes = (int)(c->to.hour * 60 + c->to.miniute);
  return start_tm.tm_yday != end_tm.tm_yday ||
         (from_miniutes < end_minutes && start_minutes < to_miniutes);
}

Condition *cond_in_time_of_day(HourMinute from, HourMinute to) {
  ConditionTimeOfDay *c = malloc(sizeof(ConditionTimeOfDay));
  c->cond.matcher = in_time_of_day_matcher;
  c->cond.destructor = do_nothing_destructor;
  c->from = from;
  c->to = to;
  return (Condition *)c;
}

typedef struct {
  Condition cond;
  DayOfWeek day;
} ConditionDayOfWeek;

static bool is_day_of_week_matcher(Condition const *self,
    Reservation const *r) {
  ConditionDayOfWeek const *c = (ConditionDayOfWeek const *)self;
  struct tm tm_t = to_local_time(r->start);
  return (int)c->day == tm_t.tm_wday;
}

Condition *cond_is_day_of_week(DayOfWeek day) {
  ConditionDayOfWeek *c = malloc(sizeof(ConditionDayOfWeek));
  c->cond.matcher = is_day_of_week_matcher;
  c->cond.destructor = do_nothing_destructor;
  c->day = day;
  return (Condition *)c;
}

static bool all_matcher(Condition const *_c, Reservation const *_r) {
  return true;
}

Condition *cond_all() {
  Condition *c = malloc(sizeof(Condition));
  c->matcher = all_matcher;
  c->destructor = do_nothing_destructor;
  return c;
}

typedef struct {
  Condition cond;
  Condition *l;
  Condition *r;
} ConditionBinary;

static void condition_binary_destructor(Condition *self) {
  ConditionBinary *c = (ConditionBinary *)self;
  drop_condition(c->l);
  drop_condition(c->r);
}

static bool cond_or_matcher(Condition const *self, Reservation const *r) {
  ConditionBinary const *c = (ConditionBinary const *)self;
  return is_matched(c->l, r) || is_matched(c->r, r);
}

Condition *cond_or(Condition *l, Condition *r) {
  ConditionBinary *c = malloc(sizeof(ConditionBinary));
  c->cond.matcher = cond_or_matcher;
  c->cond.destructor = condition_binary_destructor;
  c->l = l;
  c->r = r;
  return (Condition *)c;
}

static bool cond_and_matcher(Condition const *self, Reservation const *r) {
  ConditionBinary const *c = (ConditionBinary const *)self;
  return is_matched(c->l, r) && is_matched(c->r, r);
}

Condition *cond_and(Condition *l, Condition *r) {
  ConditionBinary *c = malloc(sizeof(ConditionBinary));
  c->cond.matcher = cond_and_matcher;
  c->cond.destructor = condition_binary_destructor;
  c->l = l;
  c->r = r;
  return (Condition *)c;
}

typedef struct {
  Condition cond;
  Condition *term;
} ConditionUnary;

static void condition_unary_destructor(Condition *self) {
  ConditionUnary *c = (ConditionUnary *)self;
  drop_condition(c->term);
}

static bool cond_not_matcher(Condition const *self, Reservation const *r) {
  ConditionUnary const *c = (ConditionUnary const *)self;
  return !is_matched(c->term, r);
}

Condition *cond_not(Condition *term) {
  ConditionUnary *c = malloc(sizeof(ConditionUnary));
  c->cond.matcher = cond_not_matcher;
  c->cond.destructor = condition_unary_destructor;
  c->term = term;
  return (Condition *)c;
}

void drop_condition(Condition *c) {
  c->destructor(c);
  free(c);
}

bool is_matched(Condition const *self, Reservation const *r) {
  return self->matcher(self, r);
}
