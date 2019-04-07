extends Label

onready var global = $"/root/global"

func _process(delta):
	var time = global.World.get_time()
	var day = global.World.get_day()
	var week = day / 7
	var hour = time * 24
	var minute = int(hour * 60) % 60
	text = ("%02d" % int(hour)) + ":" + ("%02d" % minute) + "\n" \
		+ "Day " + str(day) + ", KW " + str(week)