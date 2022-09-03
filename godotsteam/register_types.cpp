#include "register_types.h"
#include "core/object/class_db.h"
#include "core/config/engine.h"
#include "godotsteam.h"

void initialize_godotsteam_module(ModuleInitializationLevel level){
	if(level != MODULE_INITIALIZATION_LEVEL_SCENE){
		GDREGISTER_CLASS(Steam);
	}
}

void uninitialize_godotsteam_module(ModuleInitializationLevel level){
	if(level != MODULE_INITIALIZATION_LEVEL_SCENE){
		return;
	}
}
