extends Button

onready var tile_map = get_node("/root/main/tiles")
onready var cursor = get_node("/root/main/cursor")

export var action_type = "test"

func pressed():
		tile_map.send_action(action_type)
		cursor.set_texture(get_child(0).get_child(0).get_texture())

func _ready():
	pass # Replace with function body.

