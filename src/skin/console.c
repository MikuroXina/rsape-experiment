#include "./console.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../abst/controller.h"
#include "../exp/condition.h"
#include "../exp/reservation.h"

typedef struct {
  InputController con;
  Condition *cond_buf;
  char *reservation_id_buf;
  char *visitor_id_buf;
} ConsoleInputController;

static Instant input_instant() {
  time_t now = time(NULL);
  struct tm inputting;
  localtime_r(&now, &inputting);

  printf("年 [%d]: ", inputting.tm_year + 1900);
  int year = 0;
  if (scanf("%d", &year)) {
    inputting.tm_year = year - 1900;
  }

  printf("月 [%d]: ", inputting.tm_mon + 1);
  int month = 0;
  if (scanf("%d", &month)) {
    inputting.tm_mon = month - 1;
  }

  printf("日 [%d]: ", inputting.tm_mday);
  int day = 0;
  if (scanf("%d", &day)) {
    inputting.tm_mday = day;
  }

  printf("時 [%d]: ", inputting.tm_hour);
  int hour = 0;
  if (scanf("%d", &hour)) {
    inputting.tm_hour = hour;
  }

  printf("分 [%d]: ", inputting.tm_min);
  int minute = 0;
  if (scanf("%d", &minute)) {
    inputting.tm_min = minute;
  }

  time_t inputted = mktime(&inputting);
  return instant_from_time(inputted);
}

static Instant input_instant_from(InputController *_i) {
  puts("開始日時を入力して下さい");
  return input_instant();
}

static Instant input_instant_to(InputController *_i) {
  puts("終了日時を入力して下さい");
  return input_instant();
}

static HourMinute input_hour_minute() {
  printf("時: ");
  unsigned hour = 0;
  scanf("%d", &hour);

  printf("分: ");
  unsigned minute = 0;
  scanf("%d", &minute);

  HourMinute time = {hour, minute};
  return time;
}

static Condition *input_condition(InputController *i) {
  ConsoleInputController *c = (ConsoleInputController *)i;
  if (c->cond_buf != NULL) {
    drop_condition(c->cond_buf);
  }
  puts("すべて: 0");
  puts("指定日時以降: 1");
  puts("指定日時以前: 2");
  puts("時間帯指定: 3");
  puts("曜日指定: 4");
  puts("二つの条件のどちらか (AND): 5");
  puts("二つの条件の両方 (OR): 6");
  puts("条件の否定 (NOT): 7");
  printf("検索条件の数字を入力して下さい: ");
  int select = 0;
  scanf("%d", &select);
  switch (select) {
    case 0: {
      c->cond_buf = cond_all();
      break;
    }
    case 1: {
      puts("日時を指定してください");
      Instant time = input_instant();
      c->cond_buf = cond_later_than(time);
      break;
    }
    case 2: {
      puts("日時を指定してください");
      Instant time = input_instant();
      c->cond_buf = cond_earlier_than(time);
      break;
    }
    case 3: {
      puts("時間帯の始まりの時刻を指定してください");
      HourMinute begin = input_hour_minute();
      puts("時間帯の終わりの時刻を指定してください");
      HourMinute end = input_hour_minute();
      c->cond_buf = cond_in_time_of_day(begin, end);
      break;
    }
    case 4: {
      puts("日曜: 0");
      puts("月曜: 1");
      puts("火曜: 2");
      puts("水曜: 3");
      puts("木曜: 4");
      puts("金曜: 5");
      puts("土曜: 6");
      printf("曜日の数字を入力して下さい: ");
      int day = 0;
      scanf("%d", &day);
      c->cond_buf = cond_is_day_of_week((DayOfWeek)day);
      break;
    }
    case 5: {
      puts("AND の左側の条件の入力を始めます");
      Condition *left = input_condition(i);
      puts("AND の右側の条件の入力を始めます");
      Condition *right = input_condition(i);
      c->cond_buf = cond_and(left, right);
      break;
    }
    case 6: {
      puts("OR の左側の条件の入力を始めます");
      Condition *left = input_condition(i);
      puts("OR の右側の条件の入力を始めます");
      Condition *right = input_condition(i);
      c->cond_buf = cond_or(left, right);
      break;
    }
    case 7: {
      puts("NOT で否定する条件の入力を始めます");
      Condition *term = input_condition(i);
      c->cond_buf = cond_not(term);
      break;
    }
    default: {
      Condition *term = cond_all();
      c->cond_buf = cond_not(term);
      break;
    }
  }

  return c->cond_buf;
}

static const char *input_reservation_id(InputController *i) {
  ConsoleInputController *c = (ConsoleInputController *)i;
  if (c->reservation_id_buf != NULL) {
    free(c->reservation_id_buf);
  }
  puts("予約 ID を入力して下さい");
  char *str = calloc(37, sizeof(char));
  scanf("%37s", str);
  c->reservation_id_buf = str;
  return str;
}

static const char *input_visitor_id(InputController *i) {
  ConsoleInputController *c = (ConsoleInputController *)i;
  if (c->visitor_id_buf != NULL) {
    free(c->visitor_id_buf);
  }
  puts("受診者 ID を入力して下さい");
  char *str = calloc(37, sizeof(char));
  scanf("%37s", str);
  c->visitor_id_buf = str;
  return str;
}

InputController *new_console_input_controller() {
  ConsoleInputController *c = malloc(sizeof(ConsoleInputController));
  c->con._input_instant_from = input_instant_from;
  c->con._input_instant_to = input_instant_to;
  c->con._input_condition = input_condition;
  c->con._input_reservation_id = input_reservation_id;
  c->con._input_visitor_id = input_visitor_id;
  c->cond_buf = NULL;
  c->reservation_id_buf = NULL;
  c->visitor_id_buf = NULL;
  return (InputController *)c;
}

void drop_console_input_controller(InputController *i) {
  ConsoleInputController *c = (ConsoleInputController *)i;
  if (c->cond_buf != NULL) {
    drop_condition(c->cond_buf);
  }
  if (c->reservation_id_buf != NULL) {
    free(c->reservation_id_buf);
  }
  if (c->visitor_id_buf != NULL) {
    free(c->visitor_id_buf);
  }
  free(c);
}

static void show_reservations(Reservation **found) {
  size_t count = 0;
  for (Reservation **it = found; *it != NULL; ++it) {
    ++count;
  }

  printf("%ld 件の予約が見つかりました\n", count);
  puts(
      " ID                                   | 状態   | 開始時刻             "
      "    | 終了時刻                 | 受診者 ID");
  for (Reservation **it = found; *it != NULL; ++it) {
    Reservation *r = *it;
    char const *state_mes = "";
    switch (r->state) {
      case Active:
        state_mes = "予約中";
        break;
      case Stale:
        state_mes = "取消済";
        break;
      case Done:
        state_mes = "完了済";
        break;
    }
    time_t start = instant_as_time(r->start), end = instant_as_time(r->end);
    char start_buf[26] = {0};
    char end_buf[26] = {0};
    ctime_r(&start, start_buf);
    ctime_r(&end, end_buf);
    start_buf[24] = end_buf[24] = 0;
    printf(" %s | %s | %s | %s | %s\n", r->id, state_mes, start_buf, end_buf,
        r->visitor_id);
  }
}

static void not_found() {
  puts("条件に当てはまる予約は見つかりませんでした");
}

static void error_on_register(char const *reason) {
  puts("予約を登録できませんでした");
  puts(reason);
}

static void success_on_register() {
  puts("予約を登録しました");
}

static void error_on_cancel(char const *reason) {
  puts("予約を取り消しできませんでした");
  puts(reason);
}

static void success_on_cancel() {
  puts("予約を取り消しました");
}

static void error_on_done(char const *reason) {
  puts("予約を完了できませんでした");
  puts(reason);
}

static void success_on_done() {
  puts("予約を完了しました");
}

ConsoleViewController *new_console_view_controller(void) {
  ConsoleViewController *c = malloc(sizeof(ConsoleViewController));
  c->reserve_view = malloc(sizeof(ReserveViewController));
  c->reserve_view->show_reservations = show_reservations;
  c->reserve_view->not_found = not_found;
  c->register_view = malloc(sizeof(RegisterViewController));
  c->register_view->error_on_register = error_on_register;
  c->register_view->success_on_register = success_on_register;
  c->cancel_view = malloc(sizeof(CancelViewController));
  c->cancel_view->error_on_cancel = error_on_cancel;
  c->cancel_view->success_on_cancel = success_on_cancel;
  c->done_view = malloc(sizeof(DoneViewController));
  c->done_view->error_on_done = error_on_done;
  c->done_view->success_on_done = success_on_done;
  return c;
}

void drop_console_view_controller(ConsoleViewController *c) {
  free(c->done_view);
  free(c->cancel_view);
  free(c->register_view);
  free(c->reserve_view);
  free(c);
}
