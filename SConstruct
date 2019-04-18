#!python

import os, sys, platform
#################################################
# Project name can be changed, using GodotSteam
project_name = "godotsteam"
# Set up the environment
env = Environment()
host_platform = platform.system()
# Get the platform specified
target_platform = ARGUMENTS.get('p', ARGUMENTS.get('platform', False))
# If bits is not specified, use this fallback
if ARGUMENTS.get("bits", ARGUMENTS.get("b", False)):
	target_arch = ARGUMENTS.get("bits", ARGUMENTS.get("b", False))
	print("Bits used! Arch is %s") % (target_arch)
else:
	if sys.maxsize > (2**32):
		target_arch = "64"
	else:
		target_arch = "32"
	print("Bits not used! Considering using the bits or b argument. System reported as %s-bit") % (target_arch)
# Default to debug build, must be same setting as used for cpp_bindings
target = ARGUMENTS.get('target', 'debug')
# Using Clang instead of GCC
if ARGUMENTS.get('use_llvm', 'no') == 'yes':
	env['CXX'] = 'clang++'
# Local dependency paths, adapt them to your setup
cpp_bindings = ARGUMENTS.get('cpp_bindings', '../godot-cpp')
godot_headers = ARGUMENTS.get('headers', '../godot-cpp/godot_headers')
result_path = "bin/"
result_name = project_name
steam_lib_path = "include/sdk/redistributable_bin"
steam_lib = "steam_api"
# Add all files that have a .cpp extension from the path
def add_sources(sources, dir, extension):
		for f in os.listdir(dir):
				if f.endswith('.' + extension):
						sources.append(dir + '/' + f)
# If platform is Linux
if target_platform == 'linux' or target_platform == 'x11':
	result_name += '.linux.' + target_arch
	# Set compiler variables
	env['CXX']='gcc-7'
	# If using Clang
	if ARGUMENTS.get('use_llvm', 'no') == 'yes':
		env['CXX'] = 'clang++'
	env.Append(CCFLAGS = [ '-fPIC', '-g', '-O3', '-std=c++14' ])
	env.Append(LINKFLAGS = [ '-Wl,-R,\'$$ORIGIN\'', '-static-libgcc', '-static-libstdc++' ])
	# If 32-bit
	if target_arch == '32':
		env.Append(CCFLAGS = [ '-m32' ])
		env.Append(LINKFLAGS = [ '-m32' ])
		# Set correct Steam library
		steam_lib_path += "/linux32"
	# If 64-bit
	elif target_arch == '64':
		env.Append(CCFLAGS = [ '-m64' ])
		env.Append(LINKFLAGS = [ '-m64' ])
		# Set correct Steam library
		steam_lib_path += "/linux64"
	# Attach the CPP bindings lib
	env.Append(LIBS = [ 'libgodot-cpp.' + target_platform + '.' + target_arch + '.a' ])
# If platform is Windows
if target_platform == 'windows':
	result_name += '.windows.' + target_arch
	# If the host is actually Windows
	if host_platform == 'Windows':
		result_name += '.dll'
		# Set compiler variables
		env.Append(LINKFLAGS = [ '/WX' ])
		if target == 'debug':
			env.Append(CCFLAGS = ['-EHsc', '-D_DEBUG', '/MTd' ])
		else:
			env.Append(CCFLAGS = ['-O2', '-EHsc', '-DNDEBUG', '/MT' ])
	# If the host is not actually Windows
	else:
		# Set compiler variables
		if target_arch == '32':
			env['CXX']='i686-w64-mingw32-g++'
		elif target_arch == '64':
			env['CXX']='x86_64-w64-mingw32-g++'
		env.Append(CCFLAGS = [ '-g', '-O3', '-std=c++14', '-Wwrite-strings' ])
		env.Append(LINKFLAGS = [ '--static', '-Wl,--no-undefined', '-static-libgcc', '-static-libstdc++' ])
	# Set correct Steam library
	steam_lib = "steam_api.lib"
	# If using 64-bit
	if target_arch == "64":
		steam_lib = "steam_api64.lib"
		steam_lib_path += "\win64"
		# Mostly VisualStudio
		if env["CC"] == "cl":
			env.Append(LINKFLAGS=[ steam_lib ])
	# Attach the CPP bindings lib
	env.Append(LIBS = [ 'godot-cpp.' + target_platform + '.' + target_arch + '.lib' ])
# If platform is OSX
if target_platform == 'osx':
	result_name += '.osx.' + target_arch + '.dylib'
	# If using Clang
	if ARGUMENTS.get('use_llvm', 'no') == 'yes':
		env['CXX'] = 'clang++'
	# Set compiler variables
	env.Append(CCFLAGS = [ '-g','-O3', '-std=c++14', '-arch', 'x86_64' ])
	env.Append(LINKFLAGS = [ '-arch', 'x86_64', '-framework', 'Cocoa', '-Wl,-undefined,dynamic_lookup' ])
	env.Append(RPATH=env.Literal("\\$$ORIGIN"))
	# Set correct Steam library
	steam_lib_path += "/osx32"
	# Attach the CPP bindings lib
	env.Append(LIBS = [ 'libgodot-cpp.' + target_platform + '.' + target_arch + '.a' ])
# Append the last paths
env.Append(CPPPATH = [ '.', 'src', 'include', godot_headers, cpp_bindings + '/include', cpp_bindings + '/include/core', 'include/sdk/public' ])
env.Append(LIBS = [ steam_lib ])
env.Append(LIBPATH = [ cpp_bindings + '/include', cpp_bindings + '/bin', 'lib', steam_lib_path ])
# Finally add the CPP sources
sources = []
add_sources(sources, 'src', 'cpp')
# Where to output the new shared library
library = env.SharedLibrary(target=(result_path + '/' + result_name), source=sources)
Default(library)
