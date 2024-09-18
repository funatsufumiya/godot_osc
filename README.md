# godot_osc

OSC addon for Godot 4.2.2 - 4.3

## Usage

```gdscript
@onready var osc:OSC = OSC.new_from(12345, 12346, "0.0.0.0") # port_in, port_out, ip

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

## Install

NOTE: [godot_osc_bin](https://github.com/funatsufumiya/godot_osc_bin) makes installation even easier.

- create a `addons` folder in your godot project
- place `osc.gdextension` below:

    ```text
    [configuration]

    entry_symbol = "gdextension_init"
    compatibility_minimum = 4.2

    [libraries]

    macos.debug = "bin/libosc.macos.debug.framework"
    macos.release = "bin/libosc.macos.release.framework"
    windows.debug.x86_64 = "bin/libosc.windows.debug.x86_64.dll"
    windows.release.x86_64 = "bin/libosc.windows.release.x86_64.dll"
    linux.debug.x86_64 = "bin/libosc.linux.debug.x86_64.so"
    linux.release.x86_64 = "bin/libosc.linux.release.x86_64.so"
    linux.debug.arm64 = "bin/libosc.linux.debug.arm64.so"
    linux.release.arm64 = "bin/libosc.linux.release.arm64.so"
    linux.debug.rv64 = "bin/libosc.linux.debug.rv64.so"
    linux.release.rv64 = "bin/libosc.linux.release.rv64.so"
    ```

- create a `bin` folder in your godot project
- place dlls from [Releases](https://github.com/funatsufumiya/godot_osc/releases) into `bin` folder

## Build and Run

(This process is needed only if you build this plugin by your own)

```bash
$ git submodule update --init --recursive --recommend-shallow --depth 1
$ scons
$ scons target=template_release
$ godot project/project.godot # (only first time)
$ godot --path project/ # run demo
```