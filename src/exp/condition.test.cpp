extern "C" {
#include "./condition.h"
#include "./instant.h"
#include "./reservation.h"
}

#include <gtest/gtest.h>

using std::unique_ptr;

struct ReservationDeleter final {
  void operator()(Reservation *p) { drop_reservation(p); }
} del_r;

struct ConditionDeleter final {
  void operator()(Condition *p) { drop_condition(p); }
} del_c;

TEST(ConditionTest, All) {
  std::unique_ptr<Reservation, ReservationDeleter &> r1(
      new_reservation(
          Active, instant_from_time(200), instant_from_time(400), ""),
      del_r);
  std::unique_ptr<Reservation, ReservationDeleter &> r2(
      new_reservation(
          Stale, instant_from_time(500), instant_from_time(700), ""),
      del_r);
  std::unique_ptr<Reservation, ReservationDeleter &> r3(
      new_reservation(
          Done, instant_from_time(800), instant_from_time(1000), ""),
      del_r);
  std::unique_ptr<Condition, ConditionDeleter &> cond(cond_all(), del_c);
  EXPECT_TRUE(is_matched(cond.get(), r1.get()));
  EXPECT_TRUE(is_matched(cond.get(), r2.get()));
  EXPECT_TRUE(is_matched(cond.get(), r3.get()));
}
