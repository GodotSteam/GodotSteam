#include "godotsteam.h"

extern "C" void GDE_EXPORT godot_steam_terminate(godot::ModuleInitializationLevel level){
}

extern "C" void GDE_EXPORT godot_steam_initalizer(godot::ModuleInitializationLevel level) {
    if (level != godot::MODULE_INITIALIZATION_LEVEL_SERVERS) return;
    
    godot::ClassDB::register_class<godot::Steam>();
}

extern "C" GDExtensionBool GDE_EXPORT godot_steam_init(const GDExtensionInterface *p_interface,
                                            GDExtensionClassLibraryPtr p_library,
                                            GDExtensionInitialization *r_initialization) {
    godot::GDExtensionBinding::InitObject init_obj(p_interface, p_library, r_initialization);

	init_obj.register_initializer(godot_steam_initalizer);
	init_obj.register_terminator(godot_steam_terminate);
	init_obj.set_minimum_library_initialization_level(godot::MODULE_INITIALIZATION_LEVEL_SERVERS);

    return init_obj.init();
}
