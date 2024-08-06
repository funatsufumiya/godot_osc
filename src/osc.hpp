#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/class_db.hpp>

namespace godot {

class OSC : public Node
{
    GDCLASS(OSC, Node);

protected:
	static void _bind_methods();

public:
    OSC();
    ~OSC();

    void _ready() override;
    void _process(double delta) override;
};

}