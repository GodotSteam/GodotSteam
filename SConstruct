#!python
import os

opts = Variables([], ARGUMENTS)

# Gets the standard flags CC, CCX, etc.
env = DefaultEnvironment()

# Define our options
opts.Add(EnumVariable('target', "Compilation target", 'debug', ['d', 'debug', 'r', 'release']))
opts.Add(EnumVariable('platform', "Compilation platform", '', ['', 'windows', 'linuxbsd', 'linux', 'osx']))
opts.Add(EnumVariable('p', "Compilation target, alias for 'platform'", '', ['', 'windows', 'linuxbsd', 'linux', 'osx']))
opts.Add(BoolVariable('use_llvm', "Use the LLVM / Clang compiler", 'no'))
opts.Add(PathVariable('target_path', 'The path where the lib is installed.', 'bin/'))
opts.Add(PathVariable('target_name', 'The library name.', 'godotsteam', PathVariable.PathAccept))
opts.Add(EnumVariable('macos_arch', "Compilation architecture", 'arm64', ['x86_64', 'arm64']))

# Local dependency paths, adapt them to your setup
godot_headers_path = "godot-cpp/godot-headers/"
cpp_bindings_path = "godot-cpp/"
cpp_library = "libgodot-cpp"
steam_lib_path = "godotsteam/sdk/redistributable_bin"
steam_lib = "steam_api"

# only support 64 at this time..
bits = 64

# Updates the environment with the option variables.
opts.Update(env)

# Process some arguments
if env['use_llvm']:
    env['CC'] = 'clang'
    env['CXX'] = 'clang++'

if env['p'] != '':
    env['platform'] = env['p']

if env['platform'] == '':
    print("No valid target platform selected.")
    quit();

# For the reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

# Check our platform specifics
if env['platform'] == "osx":
    env['target_path'] += 'osx/'
    cpp_library += '.osx'
    if env["macos_arch"] == "x86_64":
        env.Append(CCFLAGS = ['-g','-O2', '-arch', 'x86_64', '-std=c++17'])
        env.Append(LINKFLAGS = ['-arch', 'x86_64'])
    else: # Default to ARM since M1/M2 is the future for mac
        env.Append(CCFLAGS = ['-g','-O2', '-arch', 'arm64', '-std=c++17'])
        env.Append(LINKFLAGS = ['-arch', 'arm64'])

    if env['target'] in ('debug', 'd'):
        env.Append(CCFLAGS = ['-g','-O2', '-std=c++17'])
    else:
        env.Append(CCFLAGS=['-g', '-O3', '-std=c++17'])
    
    # Set the correct Steam library
    steam_lib_path += "/osx"
    steamworks_library = 'libsteam_api.dylib'


elif env['platform'] in ('linuxbsd', 'linux'):
    env['target_path'] += 'linuxbsd/'
    cpp_library += '.linux'
    env.Append(CCFLAGS=['-fPIC'])
    env.Append(CXXFLAGS=['-std=c++17'])
    if env['target'] in ('debug', 'd'):
        env.Append(CCFLAGS=['-g3', '-Og'])
    else:
        env.Append(CCFLAGS=['-g', '-O3'])
    # Set correct Steam library
    steam_lib_path += "/linux64"
    steamworks_library = 'libsteam_api.so'

elif env['platform'] == "windows":
    env['target_path'] += 'win64/'
    cpp_library += '.windows'
    # This makes sure to keep the session environment variables on windows,
    # that way you can run scons in a vs 2017 prompt and it will find all the required tools
    env.Append(ENV=os.environ)

    env.Append(CPPDEFINES=['WIN32', '_WIN32', '_WINDOWS', '_CRT_SECURE_NO_WARNINGS'])
    env.Append(CCFLAGS=['-W3', '-GR', '-FS'])
    env.Append(CXXFLAGS='/std:c++17')
    if env['target'] in ('debug', 'd'):
        env.Append(CPPDEFINES=['_DEBUG'])
        env.Append(CCFLAGS=['-EHsc', '-MDd', '-ZI'])
        env.Append(LINKFLAGS=['-DEBUG'])
    else:
        env.Append(CPPDEFINES=['NDEBUG'])
        env.Append(CCFLAGS=['-O2', '-EHsc', '-MD'])
    # Set correct Steam library
    steam_lib_path += "/win64"
    steamworks_library = 'steam_api64.lib'
    steam_lib = "steam_api64.lib"
    if env["CC"] == "cl":
        env.Append(LINKFLAGS=[ steam_lib ])

if env['target'] in ('debug', 'd'):
    cpp_library += '.debug'
else:
    cpp_library += '.release'

cpp_library += '.' + str(bits)

# make sure our binding library is properly includes
env.Append(CPPPATH=['.', godot_headers_path, cpp_bindings_path + 'include/', cpp_bindings_path + 'include/core/', cpp_bindings_path + 'include/gen/', 'godotsteam/sdk/public'])
env.Append(LIBPATH=[cpp_bindings_path + 'bin/', steam_lib_path])
env.Append(LIBS=[cpp_library, steamworks_library])

# tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append(CPPPATH=['godotsteam/'])
sources = Glob('godotsteam/*.cpp')

library = env.SharedLibrary(target=env['target_path'] + env['target_name'] , source=sources)

Default(library)

# Generates help for the -h scons option.
Help(opts.GenerateHelpText(env))
