extends Node

const libpolitsim = preload("res://lib/politsim.gdns")

var map
var World

var initialised = false

func read_map():
	var map_file = File.new()
	map_file.open("res://map.json", File.READ)
	var map_data = map_file.get_as_text()
	map_file.close()
	return JSON.parse(map_data).result

func _ready():
	map = read_map()
	
func initialise():
	if not initialised:
		World = libpolitsim.new()
		World.init(map)
		initialised = true