project('mdflash', 'c',
        version: '0.1.0',
        default_options: ['warning_level=2', 'c_std=c11'])

gtk_dep = dependency('gtk+-3.0')

src = ['mdflash.c']

executable('mdflash', src,
           dependencies: gtk_dep,
           install: true)
