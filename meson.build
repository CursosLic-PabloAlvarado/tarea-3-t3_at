project('tarea3','cpp',
        default_options : ['cpp_std=c++20'],  
        version : '0.0.2')

# Find Jack dependencies
jack_dep = dependency('jack')
sndfile_dep = dependency('sndfile')
boost_dep = dependency('boost', modules : ['program_options','system'])
google_benchmkark_dep = dependency('benchmark', required : true) 

all_deps = [jack_dep,sndfile_dep,boost_dep]
sources = files('main.cpp', 'jack_client.cpp','passthrough_client.cpp',
                'sndfile_thread.cpp','waitkey.cpp', 'dsp_client.cpp', 'biquad.cpp', 'cascade.cpp')

# Añadir las banderas de optimización y símbolos de depuración
# opt_flags = ['-O3', '-g', '-mavx']

opt_flags = ['-O3', '-g', '-mavx', '-march=native', '-funroll-loops', '-ftree-vectorize', '-fomit-frame-pointer', '-flto']

executable('tarea3', sources,
           dependencies: all_deps,
           cpp_args: opt_flags)


               
benchmark_tarea3=executable('benchmark_tarea3',                                 
                files('benchmark.cpp','biquad.cpp', 'cascade.cpp'),                
                dependencies:[boost_dep,google_benchmkark_dep],     
                cpp_args : ['-std=c++20'] + opt_flags,                          
                build_by_default : false                            
        )                                                    
                                                                                    
# Register benchmark target                                                     
benchmark('benchmark_suite',benchmark_tarea3)      