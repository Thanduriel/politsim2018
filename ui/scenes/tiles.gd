extends TileMap

onready var global = get_node("/root/global")
onready var cursor = get_node("/root/main/cursor")

const tiles_path = "res://assets/tiles/%s"

var action_send = true
var action = null

func send_action(actionName):
	action_send = false
	action = actionName
	
func cancle_action():
	cursor.set_texture(null)
	action_send = true

func simlib_call(action, tile_id):
	if global.money > 0:
		global.money -= 1
		global.politic += 0.2
	print("call: ", action, tile_id);

func _input(event):
	if event is InputEventMouseButton \
	and event.button_index == BUTTON_RIGHT \
	and event.is_pressed() \
	and !action_send:
		cancle_action()
	if event is InputEventMouseButton \
	and event.button_index == BUTTON_LEFT \
	and event.is_pressed() \
	and !action_send:
		var tile_pos = world_to_map(event.position / scale)
		if tile_pos.x < global.map.width \
		and tile_pos.x >= 0 \
		and tile_pos.y < global.map.height \
		and tile_pos.y >= 0:
			var tile_id = tile_pos.y * global.map.width + tile_pos.x
			cancle_action()
			simlib_call(action, tile_id)

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