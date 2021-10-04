gtkdeps = [dependency('gtk+-3.0')]
xlibdeps = [dependency('x11'), dependency('xcomposite')]
cairodeps = [dependency('cairo')]

executable(
  meson.project_name (),

  'main.c',
  'window.c',
  'overlay_window.c',

  dependencies: [gtkdeps, xlibdeps]
  )
