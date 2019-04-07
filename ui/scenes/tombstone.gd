extends Sprite

var self_destruct_in = 5

func _process(delta):
	self_destruct_in -= delta
	if self_destruct_in < 0:
		queue_free()