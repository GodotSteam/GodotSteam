import os.path

module_path = os.path.dirname(os.path.realpath(__file__))

def can_build(env, platform):
	# Check if headers are present
	if os.path.exists("%s/sdk/public/steam/steam_api.h" % module_path):
		return True
	return False

def configure(env):
	pass

def get_doc_classes():
	return [
		"Steam",
	]

def get_doc_path():
	return "doc_classes"
