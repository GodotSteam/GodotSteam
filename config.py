# config.py

def can_build(env, platform):
    return platform=="linuxbsd" or platform=="windows" or platform=="macos" or platform=="server"

def configure(env):
    pass