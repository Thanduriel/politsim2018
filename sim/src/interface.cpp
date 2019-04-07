#include <gdnative_api_struct.gen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
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
	for (unsigned i = 0; i < api->num_extensions; i++) {
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
	new (world) Game::World();
	return world;
}

godot_string gs_str(const char* s) {
	return api->godot_string_chars_to_utf8(s);
}

godot_variant gv_str(const char* s) {
	godot_string str = gs_str(s);
	godot_variant ret;
	api->godot_variant_new_string(&ret, &str);
	return ret;
}

godot_string g_dict_get_s(const godot_dictionary* d, const char* s) {
	godot_variant gs = gv_str(s);
	godot_variant v = api->godot_dictionary_get(d, &gs);
	return api->godot_variant_as_string(&v);
}

bool g_s_eq(const godot_string* a, const godot_string* b) {
	return api->godot_string_operator_equal(a, b);
}

// pls no
godot_variant world_init(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, godot_variant **p_args) {
	Game::World *world = static_cast<Game::World*>(p_user_data);

	godot_dictionary map_data = api->godot_variant_as_dictionary(*p_args);

	godot_variant s_width = gv_str("width");
	godot_variant width_v = api->godot_dictionary_get(&map_data, &s_width);
	int width = static_cast<int>(api->godot_variant_as_int(&width_v));

	godot_variant s_height = gv_str("height");
	godot_variant height_v = api->godot_dictionary_get(&map_data, &s_height);
	int height = static_cast<int>(api->godot_variant_as_int(&height_v));

	godot_variant s_tiles = gv_str("tiles");
	godot_variant tiles_v = api->godot_dictionary_get(&map_data, &s_tiles);
	godot_array a_tiles = api->godot_variant_as_array(&tiles_v);
	godot_variant s_type = gv_str("type"), s_income = gv_str("income");

	godot_string s_residence = gs_str("residence"), s_street = gs_str("street"), s_work = gs_str("work"), s_hobby = gs_str("hobby");
	godot_string s_high = gs_str("high"), s_mid = gs_str("mid"), s_low = gs_str("low"), s_poor = gs_str("poor");
	godot_string s_skate = gs_str("skat_park"), s_theater = gs_str("theater"), s_arcade = gs_str("arcade"), s_tennis = gs_str("tennis");
	godot_string s_government = gs_str("government");

	std::vector<Game::Map::Tile> tiles;

	for (godot_int y = 0; y < height; y++) {
		godot_variant v_row = api->godot_array_get(&a_tiles, y);
		godot_array row = api->godot_variant_as_array(&v_row);

		for (godot_int x = 0; x < width; x++) {
			godot_variant v_tile = api->godot_array_get(&row, x);
			godot_dictionary tile = api->godot_variant_as_dictionary(&v_tile);
			
			godot_string type = g_dict_get_s(&tile, "type");
			Game::Map::Tile t;

			if (g_s_eq(&type, &s_residence)) {
				t.type = Game::Map::Tile::Type::Residence;
				godot_string quality = g_dict_get_s(&tile, "quality");
				if (g_s_eq(&quality, &s_high)) t.info.quality = Game::Map::Tile::Info::Quality::High;
				else if (g_s_eq(&quality, &s_mid)) t.info.quality = Game::Map::Tile::Info::Quality::Mid;
				else if (g_s_eq(&quality, &s_low)) t.info.quality = Game::Map::Tile::Info::Quality::Low;
				else if (g_s_eq(&quality, &s_poor)) t.info.quality = Game::Map::Tile::Info::Quality::Low;
			} else if (g_s_eq(&type, &s_street)) {
				t.type = Game::Map::Tile::Type::Street;
			} else if (g_s_eq(&type, &s_work)) {
				t.type = Game::Map::Tile::Type::Work;
				godot_variant v_income = api->godot_dictionary_get(&tile, &s_income);
				t.info.income = static_cast<int>(api->godot_variant_as_int(&v_income));
			} else if (g_s_eq(&type, &s_hobby)) {
				t.type = Game::Map::Tile::Type::Hobby;
				godot_string name = g_dict_get_s(&tile, "name");
				if (g_s_eq(&name, &s_skate)) t.info.hobby = Game::Map::Tile::Info::Hobby::Skate;
				else if (g_s_eq(&name, &s_theater)) t.info.hobby = Game::Map::Tile::Info::Hobby::Theater;
				else if (g_s_eq(&name, &s_arcade)) t.info.hobby = Game::Map::Tile::Info::Hobby::Arcade;
				else if (g_s_eq(&name, &s_tennis)) t.info.hobby = Game::Map::Tile::Info::Hobby::Tennis;
			} else if (g_s_eq(&type, &s_government)) {
				t.type = Game::Map::Tile::Type::Government;
			} else {

				printf("unknown tile type\n");
				api->godot_print(&type);
			}

			tiles.push_back(t);
		}
	}

	world->Init(Game::Map(width, height, std::move(tiles)));

	godot_variant ret{};
	return ret;
}

godot_variant world_get_actors(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, godot_variant **p_args) 
{
	Game::World& world = *static_cast<Game::World*>(p_user_data);

	godot_variant ret;
	godot_pool_vector2_array pool_array;
	api->godot_pool_vector2_array_new(&pool_array);
	for (const Game::Actor& actor : world.GetActors())
		api->godot_pool_vector2_array_push_back(&pool_array, reinterpret_cast<const godot_vector2*>(&actor.position));
	api->godot_variant_new_pool_vector2_array(&ret, &pool_array);

	return ret;
}

godot_variant world_get_day(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, godot_variant **p_args) {
	Game::World& world = *static_cast<Game::World*>(p_user_data);
	godot_variant ret;
	api->godot_variant_new_int(&ret, world.GetDay());
	return ret;
}

godot_variant world_get_time(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, godot_variant **p_args) {
	Game::World& world = *static_cast<Game::World*>(p_user_data);
	godot_variant ret;
	api->godot_variant_new_real(&ret, world.GetTime());
	return ret;
}

godot_variant world_get_money(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, godot_variant **p_args) {
	Game::World& world = *static_cast<Game::World*>(p_user_data);
	godot_variant ret;
	api->godot_variant_new_int(&ret, world.GetMoney());
	return ret;
}

godot_variant world_get_politic(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, godot_variant **p_args) {
	Game::World& world = *static_cast<Game::World*>(p_user_data);
	godot_variant ret;
	api->godot_variant_new_real(&ret, world.GetPoliticBar());
	return ret;
}

godot_variant world_update(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, godot_variant **p_args) 
{
	Game::World& world = *static_cast<Game::World*>(p_user_data);

	world.Update(static_cast<float>(api->godot_variant_as_real(p_args[0])));

	godot_variant ret{};
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
	register_function(p_handle, "get_actor_positions", world_get_actors);
	register_function(p_handle, "get_time", world_get_time);
	register_function(p_handle, "get_money", world_get_money);
	register_function(p_handle, "get_politic", world_get_politic);
	register_function(p_handle, "get_day", world_get_day);
	register_function(p_handle, "update", world_update);
}

}
