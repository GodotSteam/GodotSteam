
def can_build(platform):
	return platform=="x11" or platform=="windows" or platform=="osx"

def configure(env):
	env.Append(CPPPATH=["#modules/godotsteam/sdk/public/"])
	if env["platform"]== "x11":
		env.Append(LIBS=["steam_api"])
		#env.Append(RPATH=["."])
		if env["bits"]=="32":
			env.Append(RPATH=env.Literal('\\$$ORIGIN'))
			env.Append(LIBPATH=["#modules/godotsteam/sdk/redistributable_bin/linux32"])
		elif env["bits"]=="64":
			env.Append(RPATH=env.Literal('\\$$ORIGIN'))
			env.Append(LIBPATH=["#modules/godotsteam/sdk/redistributable_bin/linux64"])
	elif env["platform"] == "osx":
		env.Append(LIBS=["steam_api"])
		env.Append(RPATH=env.Literal('\\$$ORIGIN'))
		env.Append(LIBPATH=["#modules/godotsteam/sdk/redistributable_bin/osx32"])
	elif env["platform"] == "windows":
		steamlib = "steam_api"
		steamlib_path = "#modules/godotsteam/sdk/redistributable_bin"
		# use 64bit steam_api on Windows 64bit?
		use_64bit_lib = True # Change to False if you have problems with "unresolved external symbols"
		
		if env["CC"] == "cl": # mostly VisualStudio
			if env["bits"] == "32":
				steamlib += ".lib" # steam_api.lib
			elif env["bits"] == "64":
				if (use_64bit_lib):
					steamlib += "64" # steam_api64 (+.lib)
					steamlib_path += "/win64" # .../sdk/redistributable_bin/win64
				steamlib += ".lib"
			env.Append(LINKFLAGS=[ steamlib ])
		else: # mostly "gcc"
			# 32bit - stays "steam_api"
			if env["bits"] == "64":
				if (use_64bit_lib):
					steamlib += "64" # steam_api64
					steamlib_path += "/win64" # .../sdk/redistributable_bin/win64
			env.Append(LIBS=[ steamlib ])
		
		env.Append(LIBPATH=[ steamlib_path ])
