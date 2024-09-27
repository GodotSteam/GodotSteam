#!python
import os

def configure(env):
    from SCons.Script import BoolVariable, EnumVariable, Variables, Help

    env_vars = Variables()

    env_vars.Update(env)
    Help(env_vars.GenerateHelpText(env))

opts = Variables([], ARGUMENTS)

# Gets the standard flags CC, CCX, etc.
env = SConscript("godot-cpp/SConstruct")

# Define our options
opts.Add(PathVariable('target_path', 'The path where the lib is installed.', 'bin/'))
opts.Add(PathVariable('target_name', 'The library name.', 'godotsteam', PathVariable.PathAccept))

configure(env)
# Local dependency paths, adapt them to your setup
steam_lib_path = "godotsteam/sdk/redistributable_bin"

# Check our platform specifics
if env['platform'] in ('macos', 'osx'):
    # Set the correct Steam library
    steam_lib_path += "/osx"
    steamworks_library = 'libsteam_api.dylib'

elif env['platform'] in ('linuxbsd', 'linux'):
    env.Append(RPATH=env.Literal('\\$$ORIGIN'))
    # Set correct Steam library
    steam_lib_path += "/linux64" if env['arch'] == 'x86_64' else "/linux32"
    steamworks_library = 'libsteam_api.so'

elif env['platform'] == "windows":
    # This makes sure to keep the session environment variables on windows,
    # that way you can run scons in a vs 2017 prompt and it will find all the required tools
    # env.Append(ENV=os.environ)

    # Set correct Steam library
    steam_lib_path += "/win64" if env['arch'] == 'x86_64' else ""
    steamworks_library = 'steam_api64.dll' if env['arch'] == 'x86_64' else 'steam_api.dll'
    env.Append( CCFLAGS=['/bigobj'] )

# make sure our binding library is properly includes
env.Append(LIBPATH=[steam_lib_path])
env.Append(CPPPATH=['godotsteam/sdk/public'])
env.Append(LIBS=[
    steamworks_library.replace(".dll", "")
])

# tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append(CPPPATH=['godotsteam/'])
sources = Glob('godotsteam/*.cpp')

if env["platform"] == "macos":
    library = env.SharedLibrary(
        "bin/libgodotsteam.{}.{}.framework/libgodotsteam.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
else:
    library = env.SharedLibrary(
        "bin/libgodotsteam{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

Default(library)

# Generates help for the -h scons option.
Help(opts.GenerateHelpText(env))