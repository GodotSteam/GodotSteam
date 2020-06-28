#!/usr/bin/python
"""
Patch the steamworks sdk public header files for usage with mingw

run this script with the steamworks sdk already placed, and this file
in the godotsteam directory
"""

import sys, os

# taken from Open Steamworks
# TODO: potentially rename it
macro_snippet = r'''
//======= begin steamworks mingw patch
#if defined(_WIN32) && defined(__GNUC__) && !defined(_S4N_)
	#define STEAMWORKS_STRUCT_RETURN_0(returnType, functionName)	\
		virtual void functionName( returnType& ret ) = 0;			\
		inline returnType functionName()							\
		{															\
			returnType ret;											\
			this->functionName(ret);								\
			return ret;												\
		}
#else
	#define STEAMWORKS_STRUCT_RETURN_0(returnType, functionName) virtual returnType functionName() = 0;
#endif
//======= end steamworks mingw patch
'''

getsteamid_snippet = '''
//======= begin steamworks mingw patch
STEAMWORKS_STRUCT_RETURN_0(CSteamID, GetSteamID)
//======= original
//virtual CSteamID GetSteamID() = 0;
//======= end steamworks mingw patch
'''

header_relpath = os.path.join('sdk', 'public', 'steam', 'isteamuser.h')
script_path = os.path.realpath(__file__)
script_dirname = os.path.basename(os.path.split(script_path)[0])
isteamuser_path = os.path.join(os.path.dirname(script_path), header_relpath)

def check_if_in_godot_steam():
    # TODO: if this ever somehow becomes complex enough to need more configuration, switch
    # to argparse for handling arguments
    if script_dirname != 'godotsteam' and '-f' not in sys.argv:
        print('Refusing to run the script from anywhere but godotsteam/ (run with -f to force)')
        sys.exit()

def main():
    result = ''
    with open(isteamuser_path, 'r') as file:
        top, include, rest = file.read().partition('#include "steam_api_common.h"')
        result = ''.join((
            top,
            include,
            macro_snippet,
            rest.replace(
                'virtual CSteamID GetSteamID() = 0;',
                getsteamid_snippet
            )
        ))
    with open(isteamuser_path, 'w') as file:
        file.write(result)

if __name__ == '__main__':
    check_if_in_godot_steam()
    main()
