extends Button

onready var tile_map = get_node("/root/main/tiles")
onready var cursor = get_node("/root/main/cursor")
onready var player = get_node("/root/main/sound/Button")

export var action_type = "test"

func pressed():
	tile_map.send_action(action_type)
	cursor.set_texture($"./CenterContainer/TextureRect".get_texture())
	player.play()