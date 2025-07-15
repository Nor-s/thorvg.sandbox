wasm_build_test_src =[
    'main.cpp'
]

wasm_build_dep_list =[
    sdl2_dep,
    imgui_dep,
    # tvg_dep,
        tvg_lib_dep,
]

executable('wasm_build_test', 
    wasm_build_test_src,
    dependencies: wasm_build_dep_list,
    link_args: wasm_link_args,
    # cpp_args: tvg_compiler_flags,
    include_directories : tvg_headers,
    # link_with : tvg_lib,
)