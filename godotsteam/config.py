def can_build(env, platform):
	return platform=="x11" or platform=="windows" or platform=="osx"

def configure(env):
	pass

def get_doc_classes():
	return [
		"Steam",
	]

def get_doc_path():
	return "doc_classes"
