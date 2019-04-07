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

var hover_tile = Vector2(0,0)
var time = 0
export var hover_time = 1
var label = null
func show_name(tile):
	if (!label):
		label = Label.new()
		label.text = global.map.tiles[tile.y][tile.x].type
		var info = global.map.tiles[tile.y][tile.x].get("name")
		if info:
			label.text += "\n" + info
		else:
			info = global.map.tiles[tile.y][tile.x].get("quality")
			if info:
				label.text += "\n" + info
			else:
				info = global.map.tiles[tile.y][tile.x].get("income")
				if info:
					label.text += "\nincome: " + str(info)
		add_child(label)
		var s = cell_size;
		label.rect_position = Vector2(tile.x * s.x, tile.y * s.y)
		var d = cell_size.x * global.map.width - (label.rect_position.x + label.rect_size.x)
		if d < 0:
			label.rect_position.x += d

func hide_name():
	if (label):
		label.free()
		label = null

func _process(delta):
	var tile = world_to_map(get_global_mouse_position() / scale)
	if tile.x == hover_tile.x \
	and tile.y == hover_tile.y:
		time += delta
		if time >= hover_time:
			show_name(tile)
		else:
			hide_name()
	else:
		time = 0
		hover_tile = tile
		hide_name()
		
	pass