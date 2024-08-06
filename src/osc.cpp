#include "osc.hpp"

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void OSC::_bind_methods() {
}

OSC::OSC()
{
}

OSC::~OSC()
{
}

void OSC::_ready() {
    UtilityFunctions::print("hello from OSC");
}

void OSC::_process(double delta) {
}