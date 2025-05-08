//===========================================================================//
// GodotSteam - register_types.cpp
//===========================================================================//
//
// Copyright (c) 2015-Current | GP Garcia and Contributors (view contributors.md)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//===========================================================================//

#include "register_types.h"

#include <gdextension_interface.h>

#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/godot.hpp>

#include "godotsteam.h"

using namespace godot;


static Steam *SteamPtr = nullptr;


void initialize_godotsteam(ModuleInitializationLevel level){
	if(level == MODULE_INITIALIZATION_LEVEL_SERVERS){
		ClassDB::register_class<Steam>();
		SteamPtr = memnew(Steam);
		Engine::get_singleton()->register_singleton("Steam", Steam::get_singleton());

		// Setup Project Settings
		// GDExtension does not have GLOBAL_DEF or GLOBAL_GET, so we gotta do this manually here
		const String app_id_setting = "steam/initialization/app_id";
		const Variant initial_app_id = 0;
		if (!ProjectSettings::get_singleton()->has_setting(app_id_setting)) {
			ProjectSettings::get_singleton()->set(app_id_setting, initial_app_id);
		}
		// Fetching the value right away seems required, otherwise set_initial_value and set_as_basic don't work
		int app_id = ProjectSettings::get_singleton()->get_setting_with_override(app_id_setting);
		ProjectSettings::get_singleton()->set_initial_value(app_id_setting, initial_app_id);
		ProjectSettings::get_singleton()->set_as_basic(app_id_setting, true);

		const String auto_init_setting = "steam/initialization/initialize_on_startup";
		const Variant initial_auto_init = false;
		if (!ProjectSettings::get_singleton()->has_setting(auto_init_setting)) {
			ProjectSettings::get_singleton()->set(auto_init_setting, initial_auto_init);
		}
		// Fetching the value right away seems required, otherwise set_initial_value and set_as_basic don't work
		bool auto_init = ProjectSettings::get_singleton()->get_setting_with_override(auto_init_setting);
		ProjectSettings::get_singleton()->set_initial_value(auto_init_setting, initial_auto_init);
		ProjectSettings::get_singleton()->set_as_basic(auto_init_setting, true);

		const String embed_callbacks_setting = "steam/initialization/embed_callbacks";
		const Variant initial_embed_callbacks = false;
		if (!ProjectSettings::get_singleton()->has_setting(embed_callbacks_setting)) {
			ProjectSettings::get_singleton()->set(embed_callbacks_setting, initial_embed_callbacks);
		}
		// Fetching the value right away seems required, otherwise set_initial_value and set_as_basic don't work
		bool embed_callbacks = ProjectSettings::get_singleton()->get_setting_with_override(embed_callbacks_setting);
		ProjectSettings::get_singleton()->set_initial_value(embed_callbacks_setting, initial_embed_callbacks);
		ProjectSettings::get_singleton()->set_as_basic(embed_callbacks_setting, true);

		if (auto_init) {
			// Prevent initializing Steam from the editor itself
			if (Engine::get_singleton()->is_editor_hint()) {
				return;
			}
			Steam::get_singleton()->steamInitEx(app_id, embed_callbacks);
		}
	}
}

void uninitialize_godotsteam(ModuleInitializationLevel level){
	if(level == MODULE_INITIALIZATION_LEVEL_SERVERS){
		Engine::get_singleton()->unregister_singleton("Steam");
		memdelete(SteamPtr);
	}
}

extern "C" {
	GDExtensionBool GDE_EXPORT godotsteam_init(GDExtensionInterfaceGetProcAddress p_interface, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization){
		godot::GDExtensionBinding::InitObject init_obj(p_interface, p_library, r_initialization);

		init_obj.register_initializer(initialize_godotsteam);
		init_obj.register_terminator(uninitialize_godotsteam);
		init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SERVERS);

		return init_obj.init();
	}
}
