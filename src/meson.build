#
# target: libclist.so
# Right now, c-list has no built sources, so there is no library to build.
# Instead, we just install the header plus pkg-config file. In case we ever add
# real code, both the pkg-config and libclist_dep ways allow backwards
# compatible addition of linker arguments.
#

install_headers('c-list.h')
libclist_dep = declare_dependency(include_directories: include_directories('.'),
                                  version: meson.project_version())
mod_pkgconfig.generate(version: meson.project_version(),
                       name: 'libclist',
                       filebase: 'libclist',
                       description: 'Circular Double Linked List Implementation')

#
# target: test-*
# Symbol visibility testing, unit testing, and limited stress tests for the
# clist API.
#

test_api = executable('test-api', ['test-api.c'], dependencies: libclist_dep)
test('API Symbol Visibility', test_api)

test_basic = executable('test-basic', ['test-basic.c'], dependencies: libclist_dep)
test('Basic API Behavior', test_basic)
