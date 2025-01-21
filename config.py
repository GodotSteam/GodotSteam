import os.path

def can_build(env, platform):
	# Check if headers are present
	if os.path.exists("sdk/public/steam/steam_api.h"):
		return platform=="linuxbsd" or platform=="windows" or platform=="macos" or platform=="server"
	return False

def configure(env):
	pass

def get_doc_classes():
	return [
		"Steam",
	]

def get_doc_path():
	return "doc_classes"
