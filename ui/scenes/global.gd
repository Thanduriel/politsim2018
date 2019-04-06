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
	
func initialise():
	if not initialised:
		map = read_map()
		World = libpolitsim.new()
		World.init(map)
		initialised = true