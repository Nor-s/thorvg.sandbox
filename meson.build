project('thorvg_sandbox', 'cpp',
    default_options:['cpp_std=c++20']
)

platform = get_option('platform')

src_dir = '/'.join(meson.current_source_dir().split('\\'))

if platform == 'web'
    add_project_arguments('-DEXAMPLE_DIR=""', language : 'cpp')
    tvg_config_path = 'build/wasm/subprojects/thorvg'
    wasm_link_args = [
      '-sWASM=1',
      '-sUSE_SDL=2',
      '-sUSE_WEBGL2=1',
      '-sFORCE_FILESYSTEM=1',
      '-sALLOW_MEMORY_GROWTH=1',
      '-sFULL_ES3=1',
      '-sMAX_WEBGL_VERSION=2',
      '-sMIN_WEBGL_VERSION=2',
      '--preload-file', '@0@/resources@/'.format(src_dir),
    ]
    # tvg_proj = subproject('thorvg', default_options:['engines=sw,gl', 'log=true', 'loaders=all','savers=all', 'extra=opengl_es,lottie_expressions','partial=false','static=true','threads=false','b_lto=true'])
    tvg_proj = subproject('thorvg', default_options:['engines=sw,gl', 'log=true', 'loaders=all','savers=all', 'extra=opengl_es,lottie_expressions','static=true'])
else
    add_project_arguments('-DEXAMPLE_DIR="@0@/resources"'.format(src_dir),
                      language : 'cpp')
    tvg_config_path = 'build/native/subprojects/thorvg'
    wasm_link_args = []
    if host_machine.system() == 'windows'
        add_project_arguments('-DNOMINMAX', language: 'cpp')
        tvg_proj = subproject('thorvg', default_options:['engines=sw,gl', 'log=true', 'loaders=all','savers=all', 'extra=' ])
    else
        tvg_proj = subproject('thorvg', default_options:['engines=sw,gl', 'log=true', 'loaders=all','savers=all', 'extra=lottie_expressions' ])
    endif
endif


# For Dependency
tvg_lib = tvg_proj.get_variable('thorvg_lib')
tvg_lib_dep = tvg_proj.get_variable('thorvg_lib_dep')
tvg_headers = tvg_proj.get_variable('headers')
tvg_compiler_flags = tvg_proj.get_variable('compiler_flags')
tvg_override_options = tvg_proj.get_variable('override_options')

cmake = import('cmake')
sdl2_dep = dependency('sdl2')
imgui_proj = subproject('imgui')
imgui_dep = imgui_proj.get_variable('imgui_dep')
entt_proj = cmake.subproject('entt')
entt_dep = entt_proj.dependency('EnTT')

example_dep = []
subdir('src/editor/examples')

dep_list =[
    sdl2_dep,
    imgui_dep,
    entt_dep,
    tvg_lib_dep,
    example_dep
]

# Src Files
tvg_sandbox_src =[
    'src/main.cpp',

    'src/core/canvas/canvas.cpp',
    'src/core/gpu/gl/extraGl.cpp',
    'src/core/gpu/gl/extraGl.h',
    'src/core/gpu/gl/glUtil.cpp',
    'src/core/gpu/gl/glUtil.h',

    'src/editor/app.cpp',
    'src/editor/imgui/imguiCanvas.cpp',

    'src/editor/imgui/imguiContentBrowser.cpp',
    'src/editor/imgui/imguiContentBrowser.h',

    'src/editor/window/window.cpp',
    'src/editor/window/glWindow.cpp',
]


tvg_sandbox_inc = include_directories('src', tvg_config_path)

# Define Lib
executable('thorvg_sandbox', 
    tvg_sandbox_src,
    dependencies: dep_list,
    link_args: wasm_link_args,
    include_directories : [tvg_headers, tvg_sandbox_inc],

    cpp_args               : tvg_compiler_flags,
    gnu_symbol_visibility  : 'hidden',
    override_options       : tvg_override_options,
)


# subdir('test')