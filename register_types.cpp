#include "register_types.h"
#include "core/object/class_db.h"
#include "core/config/engine.h"
#include "godotsteam.h"

static Steam* SteamPtr = NULL;

void initialize_godotsteam_module(ModuleInitializationLevel level){
	if(level == MODULE_INITIALIZATION_LEVEL_SERVERS){
		GDREGISTER_CLASS(Steam);
		SteamPtr = memnew(Steam);
		Engine::get_singleton()->add_singleton(Engine::Singleton("Steam", Steam::get_singleton()));
	}
}


void uninitialize_godotsteam_module(ModuleInitializationLevel level){
	if(level == MODULE_INITIALIZATION_LEVEL_SERVERS){
		Engine::get_singleton()->remove_singleton("Steam");
		memdelete(SteamPtr);
	}
}
