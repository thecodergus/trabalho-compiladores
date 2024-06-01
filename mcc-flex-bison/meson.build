project('mcc', 'c',
    default_options: [ 'buildtype=release',
                       'c_std=c11',
                       'warning_level=3' ],
    meson_version: '>=0.44.0'
)

# ------------------------------------------------------------------ Generators

flex = find_program('flex')
lgen = generator(flex,
                 output: ['@BASENAME@.c', '@BASENAME@.h'],
                 arguments: [ '--outfile=@OUTPUT0@',
                              '--header-file=@OUTPUT1@',
                              '@INPUT@' ])

bison = find_program('bison')
pgen = generator(bison,
                 output: ['@BASENAME@.tab.c', '@BASENAME@.tab.h'],
                 arguments: [ '-Wall',
                              '--output=@OUTPUT0@',
                              '--defines=@OUTPUT1@',
                              '@INPUT@' ])


# --------------------------------------------------------------------- Library

mcc_inc = include_directories('include')

mcc_def = [ '-D_POSIX_C_SOURCE=200809L' ]

mcc_src = [ 'src/ast.c',
            'src/ast_print.c',
            'src/ast_visit.c',
            'src/symbol_table.c',
            'src/symbol_table_print.c',
            'src/semantic_checks.c',
            'src/tac.c',
            'src/assembly.c',
            'src/cfg.c',
            lgen.process('src/scanner.l'),
            pgen.process('src/parser.y') ]

mcc_lib = library('mcc', mcc_src,
                  c_args: mcc_def,
                  include_directories: [mcc_inc, include_directories('src')])

# ---------------------------------------------------------------- Applications

mcc_apps = [ 'mcc', 'mc_ast_to_dot' , 'mc_symbol_table', 'mc_ir', 'mc_asm', 'mc_cfg_to_dot']

foreach app : mcc_apps
    executable(app, 'app/' + app + '.c',
               c_args: mcc_def,
               include_directories: mcc_inc,
               link_with: mcc_lib)
endforeach

# ----------------------------------------------------------------------- Tests

mcc_tests = [ 'parser_test', 'symbol_table_test', 'semantic_checks_test', 'intermediate_rep_test']

cutest_inc = include_directories('vendor/cutest')

foreach test : mcc_tests
    t = executable(test, 'test/unit/' + test + '.c', 'vendor/cutest/CuTest.c',
                   c_args: mcc_def,
                   include_directories: [mcc_inc, cutest_inc],
                   link_with: mcc_lib)
    test(test, t)
endforeach
