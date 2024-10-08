class_name OSCReceiver
extends Node

@export var osc_in_port: int = 1234
@export var osc_out_port: int = 12349
@export var host_ip: String = "127.0.0.1"
# @onready var osc:OSC = OSC.new()
@onready var osc:OSC = OSC.new_from(osc_in_port, osc_out_port, host_ip)

# Called when the node enters the scene tree for the first time.
func _ready():
	# osc.init(osc_in_port, osc_out_port, host_ip) # inPort, outPort, outIP

	add_child(osc)

	# Message input Handler 
	# osc.onMessage("/test", func(msg:OSCMessage):
	osc.onMessage("/test", func(addr: String, args: Array):
		print("[OSC] /test")
		# print(msg.toString())
		print("Address: ", addr)
		print("Args: ", args)

		# match msg.getValues() :
		# 	[var x, var y] when x is float and y is float:
		# 		# print("X: ", x, ", Y: ", y)
		# 		if mover:
		# 			mover.osc_move_message_received.emit(x, y)
		# 	[var x, var y, var s] when x is float and y is float:
		# 		# print("X: ", x, ", Y: ", y)
		# 		if mover:
		# 			mover.osc_move_message_received.emit(x, y)
	)

	# osc.onMessage("/hello", func(msg:OSCMessage):
	osc.onMessage("/hello", func(addr: String, args: Array):
		print("[OSC] /hello")
		# print(msg.toString())
		print("Address: ", addr)
		print("Args: ", args)
	)

	# Message output
	osc.send("/address", [123])

func _exit_tree():
	osc.stop()
	remove_child(osc)


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pass
