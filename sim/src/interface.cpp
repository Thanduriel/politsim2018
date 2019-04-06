#include <gdnative_api_struct.gen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game/world.hpp"

extern "C" {

typedef struct user_data_struct {
	char data[256];
} user_data_struct;

const godot_gdnative_core_api_struct *api = NULL;
const godot_gdnative_ext_nativescript_api_struct *nativescript_api = NULL;

GDCALLINGCONV void *simple_constructor(godot_object *p_instance, void *p_method_data);
GDCALLINGCONV void simple_destructor(godot_object *p_instance, void *p_method_data, void *p_user_data);
godot_variant simple_get_data(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, godot_variant **p_args);

void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *p_options) {
	api = p_options->api_struct;

	// now find our extensions
	for (int i = 0; i < api->num_extensions; i++) {
		switch (api->extensions[i]->type) {
			case GDNATIVE_EXT_NATIVESCRIPT: {
				nativescript_api = (godot_gdnative_ext_nativescript_api_struct *)api->extensions[i];
			}; break;
			default: break;
		};
	};	
}

void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *p_options) {
	api = NULL;
	nativescript_api = NULL;
}

godot_variant World_Test(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, godot_variant **p_args)
{
	godot_variant ret;
	api->godot_variant_new_int(&ret, 42);
	
	return ret;
}

typedef GDCALLINGCONV godot_variant (*gd_instance_method)(godot_object *, void *, void *, int, godot_variant **);

void register_function(void* p_handle, const char* name, gd_instance_method f) {
	godot_method_attributes attrs = { GODOT_METHOD_RPC_MODE_DISABLED };
	godot_instance_method method = { NULL, NULL, NULL };
	method.method = f;
	nativescript_api->godot_nativescript_register_method(p_handle, "World", name, attrs, method);
}



GDCALLINGCONV void *world_constructor(godot_object *p_instance, void *p_method_data) {
	Game::World *world = static_cast<Game::World*>(api->godot_alloc(sizeof(Game::World)));
	return world;
}

godot_variant world_init(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, godot_variant **p_args) {
	Game::World *world = static_cast<Game::World*>(p_user_data);

	godot_dictionary map_data = api->godot_variant_as_dictionary(*p_args);
	int width = 1, height = 1;

	godot_string s;
	api->godot_string_format(&s, *p_args);
	api->godot_print(&s);

	std::vector<Game::Map::Tile> tiles;
	Game::Map map(width, height, tiles);
	world->Init(std::move(map));

	godot_variant ret;
	
	return ret;
}

GDCALLINGCONV void simple_destructor(godot_object *p_instance, void *p_method_data, void *p_user_data) {
	printf("SIMPLE._byebye()\n");
	api->godot_free(p_user_data);
}

godot_variant simple_get_data(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, godot_variant **p_args) {
	godot_string data;
	godot_variant ret;
	user_data_struct * user_data = (user_data_struct *) p_user_data;

	api->godot_string_new(&data);
	api->godot_string_parse_utf8(&data, user_data->data);
	api->godot_variant_new_string(&ret, &data);
	api->godot_string_destroy(&data);

	return ret;
}

void GDN_EXPORT godot_nativescript_init(void *p_handle) {
	godot_instance_create_func create = { NULL, NULL, NULL };
	create.create_func = &world_constructor;

	godot_instance_destroy_func destroy = { NULL, NULL, NULL };
	destroy.destroy_func = &simple_destructor;

	nativescript_api->godot_nativescript_register_class(p_handle, "World", "Reference", create, destroy);

	register_function(p_handle, "get_data", &simple_get_data);
	register_function(p_handle, "World_Test", &World_Test);
	register_function(p_handle, "init", world_init);
}

}
