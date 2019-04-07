extends Node

const libpolitsim = preload("res://lib/politsim.gdns")
const map_s = '{"width":12,"height":6,"texture":["facility","field","kurve_no","kurve_so","kurve_wn","kurve_ws","police_station","skate_park","tennis","weg_ns","weg_wo","industrie","weggabelung","strasse_no","strasse_ns","strasse_so","strasse_wn","strasse_wo","strasse_ws","residence_high","dienstleister","theater","slums","bundestagl","bundestagr","bank","platzl","platzr","handwerker","arcade","skyscraper"],"tiles":[[{"type":"residence","quality":"high","texture":19},{"type":"street","quality":"high","texture":15},{"type":"street","quality":"high","texture":17},{"type":"street","quality":"high","texture":17},{"type":"street","quality":"high","texture":18},{"type":"work","income":6,"texture":25},{"type":"work","income":6,"texture":25},{"type":"work","income":4,"texture":20},{"type":"street","quality":"low","texture":3},{"type":"street","quality":"low","texture":10},{"type":"work","income":3,"texture":1},{"type":"work","income":3,"texture":1}],[{"type":"residence","quality":"high","texture":19},{"type":"street","quality":"high","texture":14},{"type":"hobby","name":"theater","texture":21},{"type":"residence","quality":"high","texture":19},{"type":"street","quality":"high","texture":14},{"type":"government","texture":23},{"type":"government","texture":24},{"type":"police","texture":6},{"type":"street","quality":"low","texture":9},{"type":"residence","quality":"mid","texture":30},{"type":"work","income":3,"texture":1},{"type":"work","income":3,"texture":1}],[{"type":"hobby","name":"tennis","texture":8},{"type":"street","quality":"high","texture":14},{"type":"residence","quality":"high","texture":19},{"type":"work","income":4,"texture":20},{"type":"street","quality":"high","texture":13},{"type":"street","quality":"high","texture":26},{"type":"street","quality":"high","texture":27},{"type":"street","quality":"low","texture":10},{"type":"street","quality":"low","texture":12},{"type":"residence","quality":"mid","texture":30},{"type":"work","income":2,"texture":11},{"type":"work","income":3,"texture":1}],[{"type":"residence","quality":"mid","texture":30},{"type":"street","quality":"high","texture":14},{"type":"hobby","name":"tennis","texture":8},{"type":"hobby","name":"arcade","texture":29},{"type":"residence","quality":"mid","texture":30},{"type":"street","quality":"high","texture":14},{"type":"work","income":4,"texture":20},{"type":"residence","quality":"mid","texture":30},{"type":"street","quality":"low","texture":2},{"type":"street","quality":"low","texture":5},{"type":"work","income":2,"texture":11},{"type":"work","income":2,"texture":11}],[{"type":"work","income":5,"texture":28},{"type":"street","quality":"high","texture":13},{"type":"street","quality":"high","texture":17},{"type":"street","quality":"high","texture":17},{"type":"street","quality":"high","texture":17},{"type":"street","quality":"high","texture":16},{"type":"hobby","name":"arcade","texture":29},{"type":"residence","quality":"poor","texture":22},{"type":"hobby","name":"skat_park","texture":7},{"type":"street","quality":"low","texture":2},{"type":"street","quality":"low","texture":10},{"type":"street","quality":"low","texture":10}],[{"type":"work","income":5,"texture":28},{"type":"work","income":5,"texture":28},{"type":"work","income":4,"texture":20},{"type":"residence","quality":"mid","texture":30},{"type":"residence","quality":"mid","texture":30},{"type":"hobby","name":"skat_park","texture":7},{"type":"residence","quality":"poor","texture":22},{"type":"residence","quality":"poor","texture":22},{"type":"residence","quality":"poor","texture":22},{"type":"residence","quality":"poor","texture":22},{"type":"work","income":1,"texture":0},{"type":"work","income":1,"texture":0}]]}'

var map
var World
export var money = 42
export var politic = 0.3

var initialised = false

func read_map():
	#var map_file = File.new()
	#map_file.open("res://map.json", File.READ)
	#var map_data = map_file.get_as_text()
	#map_file.close()
	return JSON.parse(map_s).result
	
func initialise():
	if not initialised:
		map = read_map()
		World = libpolitsim.new()
		World.init(map)
		initialised = true