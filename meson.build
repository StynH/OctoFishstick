project('OctoFishstick', 'c',
  meson_version: '>=0.60',
  default_options: ['c_std=c23', 'buildtype=debugoptimized', 'warning_level=3'],
)

cc = meson.get_compiler('c')
if cc.get_id() == 'clang'
  add_project_arguments('-Wall', '-Wextra', '-Wpedantic', '-Werror', language: 'c')
endif

deps = [
  dependency('gtk4', required: true),
  dependency('glib-2.0', required: true),
  dependency('gobject-2.0', required: true),
  dependency('cairo', required: true),
  dependency('json-c', required: true),
  dependency('libcurl', required: true),
  dependency('gee-0.8', required: true),
]

livechart_inc = include_directories('external/live-chart-c')
livechart_dir = join_paths(meson.project_source_root(), 'external', 'live-chart-c')
livechart     = cc.find_library('livechart', dirs: [livechart_dir], required: true)

libm = cc.find_library('m', required: false)

sources = files(
  'src/actions.c',
  'src/api.c',
  'src/resources.c',
  'src/app.c',
  'src/main.c',
  'src/ticker.c',
  'src/views/add_ticker_dialog.c',
  'src/window.c',
)

all_deps = deps + [livechart]
if libm.found()
  all_deps += libm
endif

executable('OctoFishstick', sources,
  dependencies: all_deps,
  include_directories: [livechart_inc],
  install: false,
)
