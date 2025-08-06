source_file = [
    meson.current_source_dir().join('canvas/canvas.cpp'),
    meson.current_source_dir().join('gpu/gl/extraGl.cpp'),
    meson.current_source_dir().join('gpu/gl/extraGl.h'),
    meson.current_source_dir().join('gpu/gl/glUtil.cpp'),
    meson.current_source_dir().join('gpu/gl/glUtil.h'),
]

core_dep += [declare_dependency(
    include_directories : include_directories('.'),
    sources             : source_file,
)]
