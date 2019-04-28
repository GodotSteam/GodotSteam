def can_build(platform):
	return platform=="x11" or platform=="windows" or platform=="osx"

def configure(env):
	env.Append(CPPPATH=["#modules/godotsteam/sdk/public/"])

	# If compiling Linux
	if env["platform"]== "x11":
		env.Append(LIBS=["libsteam_api"])
		env.Append(RPATH=["."])
		if env["bits"]=="32":
			env.Append(RPATH=env.Literal('\\$$ORIGIN/linux32'))
			env.Append(LIBPATH=["#modules/godotsteam/sdk/redistributable_bin/linux32"])
		else: # 64 bit
			env.Append(RPATH=env.Literal('\\$$ORIGIN/linux64'))
			env.Append(LIBPATH=["#modules/godotsteam/sdk/redistributable_bin/linux64"])

	# If compiling Windows
	elif env["platform"] == "windows":
		# Mostly VisualStudio
		if env["CC"] == "cl":
			if env["bits"]=="32":
				env.Append(LINKFLAGS=["steam_api.lib"])
				env.Append(LIBPATH=["#modules/godotsteam/sdk/redistributable_bin"])
			else: # 64 bit
				env.Append(LINKFLAGS=["steam_api64.lib"])
				env.Append(LIBPATH=["#modules/godotsteam/sdk/redistributable_bin/win64"])
		
		# Mostly "GCC"
		else:
			if env["bits"]=="32":
				env.Append(LIBS=["steam_api"])
				env.Append(LIBPATH=["#modules/godotsteam/sdk/redistributable_bin"])
			else: # 64 bit
				env.Append(LIBS=["steam_api64"])
				env.Append(LIBPATH=["#modules/godotsteam/sdk/redistributable_bin/win64"])

	# If compiling OSX
	elif env["platform"] == "osx":
		env.Append(LIBS=["steam_api"])
		env.Append(LIBPATH=['#modules/godotsteam/sdk/redistributable_bin/osx32'])
