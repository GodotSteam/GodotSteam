# config.py

def can_build(env, platform):
	return platform=="linuxbsd" or platform=="windows" or platform=="macos" or platform=="server"

def configure(env):
	pass

def get_doc_classes():
	return [
		"SteamMultiplayerPeer",
	]

def get_doc_path():
	return "doc_classes"
