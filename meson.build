project(
    'Tiler',
    ['cpp'],
    default_options : ['cpp_std=c++17'],
    version: '0.1.0',
    license: 'LGPLv3',
    meson_version: '>=0.45.1',
)

add_project_arguments(
    [
        '-pthread',
        '-Wl,--as-needed',
        'warning_level=3',
    ],
    language: 'c++',
)

miral_dep = dependency('miral')

subdir('window_manager')

srcs = files(
    'main.cpp',
    'tiler_shell.h',
    'tiler_shell.cpp',
    'event_filter.h',
    'event_filter.cpp',
    'setup_env.cpp',
) + window_manager_srcs

example = executable(
    'tiler',
    srcs,
    dependencies: [miral_dep],
    install: true,
)
