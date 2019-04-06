extends TileMap

func read_map():
	var map_file = File.new()
	map_file.open("res://map.json", File.READ)
	var map_data = map_file.get_as_text()
	map_file.close()
	return JSON.parse(map_data).result

onready var map = read_map()

func get_texture_by_name(name):
	return load("res://assets/tiles/%s.png" % name)
	
func get_texture_by_id(id):
	return get_texture_by_name(map.texture[id])

func _ready():
	tile_set.clear()
	for t in range(map.texture.size()):
		tile_set.create_tile(t)
		tile_set.tile_set_texture(t, get_texture_by_id(t))
	
	for x in range(map.width):
		for y in range(map.height):
			set_cell(x, y, map.tiles[y][x].texture)