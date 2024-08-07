#include "osc.hpp"
#include "osc_message.hpp"

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/packet_peer_udp.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void OSC::_bind_methods() {
    // ClassDB::bind_method(D_METHOD("_init", "inPort", "outPort", "outIP"), &OSC::_init);
    // ClassDB::bind_static_method("OSC", D_METHOD("create", "inPort", "outPort", "outIP"), &OSC::create);
    ClassDB::bind_static_method("OSC", D_METHOD("new_from", "inPort", "outPort", "outIP"), &OSC::new_from);
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

OSC* OSC::new_from(int inPort, int outPort, String outIP) {
    // UtilityFunctions::print("OSC::new_from");
    // OSC* osc = new OSC();
    // osc.instantiate();

    OSC* osc = memnew(OSC);
    osc->init(inPort, outPort, outIP);

    return osc;
}

void OSC::init(int inPort, int outPort, String outIP) {
    // UtilityFunctions::print("OSC::init");
    // UtilityFunctions::print("OSC::init inPort: " + String::num_int64(inPort) + " outPort: " + String::num_int64(outPort) + " outIP: " + outIP);
    _inPort = inPort;
    _outPort = outPort;
    _outIP = outIP;

	// server = UDPServer.new()
	// server.listen(inPort)

    server = memnew(UDPServer);
    server->listen(inPort);
    UtilityFunctions::print("OSC UDP server listening on port: " + String::num_int64(inPort));
}

void OSC::_ready() {
    // UtilityFunctions::print("OSC::_ready()");
}

void OSC::_process(double delta) {
    server->poll(); // Important!
    if (server->is_connection_available()) {
        Ref<PacketPeerUDP> peer = server->take_connection();
        PackedByteArray packet = peer->get_packet();

        std::shared_ptr<OSCMessage> msg = std::make_shared<OSCMessage>();
        msg->init(packet);
        
        if (!msg->isValid()) {
            return;
        }
        if (messageHandlers.has("*")) {
            Array empty = Array();
            Array arr = messageHandlers.get("*", empty);
            // for (Callable handler : arr) {
            //     handler.call(msg);
            // }

            for (int i = 0; i < arr.size(); i++) {
                Callable handler = arr[i];
                handler.call(msg.get());
            }
        }
        if (messageHandlers.has(msg->address())) {
            Array arr = messageHandlers[msg->address()];

            for (int i = 0; i < arr.size(); i++) {
                Callable handler = arr[i];
                handler.call(msg.get());
            }
        }
    }
}

void OSC::send(PackedByteArray buffer) {
    // UtilityFunctions::print("OSC::send");
}

void OSC::stop() {
    server->stop();
    UtilityFunctions::print("OSC UDP server stopped");
}

void OSC::onMessage(String address, Callable callback) {
    // UtilityFunctions::print("OSC::onMessage");

    if (!messageHandlers.has(address)) {
        messageHandlers[address] = Array();
    }
    ((Array)messageHandlers[address]).push_back(callback);
}