gtkdeps = [dependency('gtk+-3.0')]
cairodeps = [dependency('cairo')]

executable(
  meson.project_name (),

  'main.c',
  'window.c',
  'overlay.c',
  'ruler.c',

  dependencies: [gtkdeps, cairodeps]
  )
