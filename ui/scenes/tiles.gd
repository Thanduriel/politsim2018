extends TileMap

onready var global = get_node("/root/global")

const tiles_path = "res://assets/tiles/%s"

func get_texture_by_name(name):
	var path = tiles_path % (name + ".png")
	if ResourceLoader.exists(path):
		return load(path)
	
	path = tiles_path % (name + ".tres")
	return load(path)
	
func get_texture_by_id(id):
	return get_texture_by_name(global.map.texture[id])

func _ready():
	global.initialise()
	var map = global.map
	tile_set.clear()
	for t in range(map.texture.size()):
		tile_set.create_tile(t)
		tile_set.tile_set_texture(t, get_texture_by_id(t))
	
	var screen_width = get_viewport_rect().size.x
	var scale_factor = screen_width / (cell_size.x * map.width)
	scale *= scale_factor
	
	for x in range(map.width):
		for y in range(map.height):
			set_cell(x, y, map.tiles[y][x].texture)