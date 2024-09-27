///// HEADER INCLUDES
/////////////////////////////////////////////////
//
// Include GodotSteam header
#include "godotsteam_id.h"


///// BIND METHODS
/////////////////////////////////////////////////
//
void SteamID::_bind_methods() {
	ClassDB::bind_method(D_METHOD("from_int", "id"), &SteamID::from_int);

	ClassDB::bind_method(D_METHOD("get_account_id"), &SteamID::get_account_id);
	ClassDB::bind_method(D_METHOD("get_un_account_instance"), &SteamID::get_un_account_instance);
	ClassDB::bind_method(D_METHOD("get_account_type"), &SteamID::get_account_type);
	ClassDB::bind_method(D_METHOD("get_universe"), &SteamID::get_universe);

	ClassDB::bind_method(D_METHOD("to_int"), &SteamID::to_int);
	
	BIND_ENUM_CONSTANT(ACCOUNT_TYPE_INVALID);
	BIND_ENUM_CONSTANT(ACCOUNT_TYPE_INDIVIDUAL);
	BIND_ENUM_CONSTANT(ACCOUNT_TYPE_MULTISEAT);
	BIND_ENUM_CONSTANT(ACCOUNT_TYPE_GAME_SERVER);
	BIND_ENUM_CONSTANT(ACCOUNT_TYPE_ANON_GAME_SERVER);
	BIND_ENUM_CONSTANT(ACCOUNT_TYPE_PENDING);
	BIND_ENUM_CONSTANT(ACCOUNT_TYPE_CONTENT_SERVER);
	BIND_ENUM_CONSTANT(ACCOUNT_TYPE_CLAN);
	BIND_ENUM_CONSTANT(ACCOUNT_TYPE_CHAT);
	BIND_ENUM_CONSTANT(ACCOUNT_TYPE_CONSOLE_USER);
	BIND_ENUM_CONSTANT(ACCOUNT_TYPE_ANON_USER);

	BIND_ENUM_CONSTANT(UNIVERSE_INVALID);
	BIND_ENUM_CONSTANT(UNIVERSE_PUBLIC);
	BIND_ENUM_CONSTANT(UNIVERSE_BETA);
	BIND_ENUM_CONSTANT(UNIVERSE_INTERNAL);
	BIND_ENUM_CONSTANT(UNIVERSE_DEV);
}