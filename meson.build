project('tar', 'c',
  version: '0.1',
  default_options: ['warning_level=3', 'c_std=gnu2x', 'werror=true']
)

srcs = files(
  # files go here
  # (or in 'folder'/meson.build)
)

# optional subdir('folder')
subdir('src')

include = include_directories(
  'include'
)

executable('tar', srcs,
  include_directories: include,
  install: true,
)
