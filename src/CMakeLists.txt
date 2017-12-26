
find_package(Allegro)

set(koflinux_SRCS
  batle.c
  display.c
  effects.c
  initfiles.c
  inits.c
  intro.c
  loads.c
  logger.c
  main.c
  message.c
  monitor.c
  moves.c
  mp3.c
  options.c
  palettes.c
  refresh.c
  screenshot.c
  sprites.c
  startmenu.c
  wavsounds.c
)

add_executable(koflinux ${koflinux_SRCS})
#add_definitions(-DSO=LINUX)
target_link_libraries(koflinux ${ALLEGRO_LIBRARY})

