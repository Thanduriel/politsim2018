extends Label

onready var global = $"/root/global"

func _process(delta):
	var day = global.World.get_day()
	var week = day / 7
	text = "Day " + str(day) + ", KW " + str(week)