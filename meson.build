# Projeto de exemplo para uso do GenICam com C
project('Aplicação', 'c')

# Inicia as flags de compilação como um vetor vazio
c_args = []

# Adiciona flags de compilação para o GCC e Clang
cc = meson.get_compiler('c')
if cc.get_id() == 'gcc' or cc.get_id() == 'clang'
 c_args += '-Wno-deprecated'
endif

# Importa as pastas de cabeçalho
inc = include_directories('include', '/usr/local/include')

# Importa os arquivos de código fonte
app_src = files(
 'src/main.c'
)

# Local para importar as dependencias

# Aplicação Central
executable('main',
   sources : [
     app_src
   ],
   include_directories : inc,
   dependencies: [

   ],
   install: true,
   c_args : c_args
 )