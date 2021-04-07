project(
        'c-list',
        'c',
        version: '3',
        license: 'Apache',
        default_options: [
                'c_std=c11',
        ],
)
project_description = 'Circular Intrusive Double Linked List Collection'

add_project_arguments('-D_GNU_SOURCE', language: 'c')
mod_pkgconfig = import('pkgconfig')

use_gnu = false
ccid = meson.get_compiler('c').get_id()
if ccid == 'gcc' or ccid == 'clang'
        use_gnu = true
endif

subdir('src')
