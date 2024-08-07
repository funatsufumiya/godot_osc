#include "osc.hpp"

#include <godot_cpp/variant/utility_functions.hpp>
// #include <godot_cpp/core/class_db.hpp>

using namespace godot;

void OSC::_bind_methods() {
    // ClassDB::bind_method(D_METHOD("_init", "inPort", "outPort", "outIP"), &OSC::_init);
    // ClassDB::bind_static_method("OSC", D_METHOD("create", "inPort", "outPort", "outIP"), &OSC::create);
    ClassDB::bind_method(D_METHOD("init", "inPort", "outPort", "outIP"), &OSC::init);
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

// void OSC::_init(int inPort, int outPort, String outIP) {
//     _inPort = inPort;
//     _outPort = outPort;
//     _outIP = outIP;
// }

// OSC OSC::create(int inPort, int outPort, String outIP) {
//     // UtilityFunctions::print("OSC::create");
//     OSC osc;
//     osc._inPort = inPort;
//     osc._outPort = outPort;
//     osc._outIP = outIP;

//     return osc;
// }

void OSC::init(int inPort, int outPort, String outIP) {
    // UtilityFunctions::print("OSC::set");
    _inPort = inPort;
    _outPort = outPort;
    _outIP = outIP;
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