def can_build(platform):
  return platform=="x11" or platform=="windows" or platform=="osx"

def configure(env):
  env.Append(CPPPATH=["#modules/godotsteam/sdk/public/"])
  if env["platform"]== "x11":
    env.Append(LIBS=["steam_api"])
    env.Append(RPATH=["."])
    if env["bits"]=="32":
      env.Append(RPATH=env.Literal('\\$$ORIGIN/linux32'))
      env.Append(LIBPATH=["#modules/godotsteam/sdk/redistributable_bin/linux32"])
    else: # 64 bit
      env.Append(RPATH=env.Literal('\\$$ORIGIN/linux64'))
      env.Append(LIBPATH=["#modules/godotsteam/sdk/redistributable_bin/linux64"])
  elif env["platform"] == "windows":
    # mostly VisualStudio
    if env["CC"] == "cl":
      if env["bits"]=="32":
        env.Append(LINKFLAGS=["steam_api.lib"])
        env.Append(LIBPATH=["#modules/godotsteam/sdk/redistributable_bin"])
      else: # 64 bit
        env.Append(LINKFLAGS=["steam_api64.lib"])
        env.Append(LIBPATH=["#modules/godotsteam/sdk/redistributable_bin/win64"])
    # mostly "gcc"
    else:
      if env["bits"]=="32":
        env.Append(LIBS=["steam_api"])
        env.Append(LIBPATH=["#modules/godotsteam/sdk/redistributable_bin"])
      else: # 64 bit
        env.Append(LIBS=["steam_api64"])
        env.Append(LIBPATH=["#modules/godotsteam/sdk/redistributable_bin/win64"])
  elif env["platform"] == "osx":
    env.Append(LIBS=["steam_api"])
    env.Append(LIBPATH=['#modules/godotsteam/sdk/redistributable_bin/osx32'])

def get_doc_classes():
    return [
        "Steam",
        "FriendGameInfo",
    ]

def get_doc_path():
    return "docs_classes"
