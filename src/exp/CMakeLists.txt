add_library(exp
  STATIC
    condition.c
    reservation.c
)

enable_testing()

add_executable(TestCondition condition.test.c)
target_link_libraries(TestCondition exp gtest_main)

include(GoogleTest)
gtest_discover_tests(TestCondition)
