# godot_osc

OSC addon for Godot 4.2.2

## Usage

```gdscript
@onready var osc:OSC = OSC.new_from(12345, 12346, "127.0.0.1")

func _ready():
	add_child(osc)

	osc.onMessage("/test", func(addr: String, args: Array):
		print("[OSC] /test")
		print("Address: ", addr)
		print("Args: ", args)
	)

	# Message output
	osc.send("/address", [123])

func _exit_tree():
	osc.stop()
	remove_child(osc)

func _process(delta):
	pass
```

see [`project/`](project) and [`osc_receiver.gd`](project/osc_receiver.gd) for example usage.

## Build and Run

- `git submodule update --init --recursive --recommend-shallow --depth 1`
- `scons`
- `scons target=template_release`
- `godot project/project.godot` (only first time)
- `godot --path project/`