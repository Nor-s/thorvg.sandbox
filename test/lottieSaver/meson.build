lottie_saver_test_src =[
    'main.cpp'
]

executable('lottieSaver', 
    lottie_saver_test_src,
    dependencies: [tvg_lib_dep, lottie_dep],
    include_directories : [tvg_headers, tvg_sandbox_inc, '.'],
    cpp_args               : tvg_compiler_flags,
    gnu_symbol_visibility  : 'hidden',
    override_options       : tvg_override_options,
)