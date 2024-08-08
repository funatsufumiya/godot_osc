# godot_osc

OSC addon for Godot 4.2.2

## Usage

see [`project/`](project) and [`osc_receiver.gd`](project/osc_receiver.gd) for example usage.

## Build and Run

- `git submodule update --init --recursive --recommend-shallow --depth 1`
- `scons`
- `scons target=template_release`
- `godot project/project.godot` (only first time)
- `godot --path project/`