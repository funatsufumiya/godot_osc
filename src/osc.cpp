#include "osc.hpp"

#include <godot_cpp/variant/utility_functions.hpp>
// #include <godot_cpp/core/class_db.hpp>

using namespace godot;

void OSC::_bind_methods() {
    // ClassDB::bind_method(D_METHOD("_init"), &OSC::_init);
}

OSC::OSC()
{
    UtilityFunctions::print("OSC::constructor");
}

OSC::~OSC()
{
}

void OSC::_ready() {
    UtilityFunctions::print("OSC::_ready()");
}

void OSC::_process(double delta) {
}