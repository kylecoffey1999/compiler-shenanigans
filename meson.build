project(
  'Compiler Shenanigans',
  'c',
  'cpp',
  version : '0.1.0',
  default_options : [
	'warning_level=3',
	'c_std=c11',
	'cpp_std=c++20',
  ],
)

c_algorithms_dep = dependency('c-algorithms')
cbitset_dep = dependency('cbitset')
fmt_dep = dependency('fmt')
gc_dep = dependency('gc', required : false)
sds_dep = dependency('sds')
vec_dep = dependency('vec')

if not gc_dep.found()
  cmake = import('cmake')
  gc_proj = cmake.subproject('bdwgc')
  gc_dep = gc_proj.dependency('gc')
endif

subdir('c')
subdir('plainc')
