add_subdirectory(exp)
add_subdirectory(play)
add_subdirectory(skin)

add_executable(reserve_system main.c)
target_link_libraries(reserve_system exp play skin)
target_include_directories(reserve_system
  PRIVATE exp
  PRIVATE play
  PRIVATE abst
  PRIVATE skin
)
