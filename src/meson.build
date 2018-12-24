project('JumpGame','cpp',
  default_options : ['cpp_std=c++14', 'warning_level=3', 'buildtype=release']
)

src=[
  'Main.cpp'
]

SDL = dependency('sdl2' ,version : '>=2.0.7')

threads = dependency('threads')

program = executable('a.out',src,dependencies : [SDL,threads] )
