source_file = [
   meson.current_source_dir().join('exampleBase.h'),
   meson.current_source_dir().join('exampleBase.cpp'),
   meson.current_source_dir().join('animationExample.h'),
   meson.current_source_dir().join('lottieExample.h'),
   meson.current_source_dir().join('particleExample.h'),
   meson.current_source_dir().join('svgExample.h'),
   meson.current_source_dir().join('boundingBoxExample.h'),
   meson.current_source_dir().join('examples.h'),
   meson.current_source_dir().join('emptyExample.h'),
   meson.current_source_dir().join('accessorExample.h'),
]

example_dep += [declare_dependency(
    include_directories : include_directories('.'),
    sources             : source_file,
)]
