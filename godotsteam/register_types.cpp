#include "register_types.h"
#include "object_type_db.h"
#include "godotsteam.h"
#include "globals.h"

static Steam* SteamPtr = NULL;

void register_godotsteam_types(){
	SteamPtr = memnew(Steam);

	Globals::get_singleton()->add_singleton(Globals::Singleton("Steam",Steam::get_singleton()));

	ObjectTypeDB::register_virtual_type<Steam>();
}

void unregister_godotsteam_types(){
	memdelete(SteamPtr);
}
