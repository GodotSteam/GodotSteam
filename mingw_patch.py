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
'''

def check_if_in_godot_steam():
    # TODO: if this ever somehow becomes complex enough to need more configuration, switch
    # to argparse for handling arguments
    if os.path.basename(__file__) != 'godotsteam' and '-f' not in sys.argv:
        print('Refusing to run the script from anywhere but godotsteam/ (run with -f to force)')

def main():
    check_if_in_godot_steam()
    result = ''
    with open('godotsteam/sdk/public/steam/isteamuser.h', 'r') as file:
        top, include, rest = file.read().partition('#include "steam_api_common.h\n')
        result = ''.join((
            top,
            include,
            hack_snippet,
            rest.replace(
                'virtual CSteamID() = 0;',
                'STEAMWORKS_STRUCT_RETURN_0(CSteamID, GetSteamID)'
            )
        ))
    with open('godotsteam/sdk/public/steam/isteamuser.h', 'x') as file:
        file.write(result)


if __name__ == '__main__':
    main()
