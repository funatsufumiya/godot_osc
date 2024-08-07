#include "osc.hpp"

#include <godot_cpp/variant/utility_functions.hpp>
// #include <godot_cpp/core/class_db.hpp>

using namespace godot;

void OSC::_bind_methods() {
    // ClassDB::bind_method(D_METHOD("_init"), &OSC::_init);
    ClassDB::bind_method(D_METHOD("send", "buffer"), &OSC::send);
    ClassDB::bind_method(D_METHOD("stop"), &OSC::stop);
    ClassDB::bind_method(D_METHOD("onMessage", "address", "callback"), &OSC::onMessage);
}

OSC::OSC()
{
    // UtilityFunctions::print("OSC::constructor");
}

OSC::~OSC()
{
}

void OSC::_ready() {
    // UtilityFunctions::print("OSC::_ready()");
}

void OSC::_process(double delta) {
}

void OSC::send(PackedByteArray buffer) {
    // UtilityFunctions::print("OSC::send");
}

void OSC::stop() {
    // UtilityFunctions::print("OSC::stop");
}

void OSC::onMessage(String address, Callable callback) {
    // UtilityFunctions::print("OSC::onMessage");
}