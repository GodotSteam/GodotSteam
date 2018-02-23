//#include "Godot.hpp"
#include "godotsteam.h"

// GDNative initialize
extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *o){
	godot::Godot::gdnative_init(o);
}
// GDNative terminate
extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *o){
	godot::Godot::gdnative_terminate(o);
}
// NativeScript initialize
extern "C" void GDN_EXPORT godot_nativescript_init(void *handle){
	godot::Godot::nativescript_init(handle);
	godot::register_class<Steam>();
}