#include "./debug.h"

#include <stdio.h>
#include <time.h>

#include "../exp/reservation.h"
#include "../play/play.h"
#include "../skin/console.h"
#include "../skin/mock.h"
#include "../skin/stub.h"

void debug_run() {
  Reservation *reservations[2] = {
      new_reservation(
          Active, instant_from_time(200), instant_from_time(200), "hoge"),
      new_reservation(
          Stale, instant_from_time(800), instant_from_time(900), "fuga"),
  };
  Repository *repo = new_stub_repository(reservations, 2);
  Database *db = new_mock_database();
  ConsoleViewController *view = new_console_view_controller();
  InputController *input = new_console_input_controller();
  Clock *clock = new_mock_clock(instant_from_time(time(NULL)));
  Config *config = new_mock_config(3, duration_from_time(108000));

  while (true) {
    puts("終了: 0");
    puts("確認: 1");
    puts("登録: 2");
    puts("取消: 3");
    puts("完了: 4");
    puts("実行する Play の数字を入力して下さい");
    int select = 0;
    scanf("%d", &select);
    getchar();
    if (select == 0) {
      break;
    }
    switch (select) {
      case 1:
        confirm_reservation(repo, input, view->reserve_view);
        break;
      case 2:
        register_reservation(repo, db, input, view->register_view, config);
        break;
      case 3:
        cancel_reservation(repo, db, input, view->cancel_view, clock, config);
        break;
      case 4:
        done_reservation(repo, db, input, view->done_view, clock);
        break;
    }
  }

  drop_mock_config(config);
  drop_mock_clock(clock);
  drop_console_input_controller(input);
  drop_console_view_controller(view);
  drop_mock_database(db);
  drop_stub_repository(repo);
}
