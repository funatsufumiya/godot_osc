#include "osc.hpp"
#include "osc_message.hpp"

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/packet_peer_udp.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <memory>

using namespace godot;

void OSC::_bind_methods() {
    // ClassDB::bind_method(D_METHOD("_init", "inPort", "outPort", "outIP"), &OSC::_init);
    // ClassDB::bind_static_method("OSC", D_METHOD("create", "inPort", "outPort", "outIP"), &OSC::create);
    ClassDB::bind_static_method("OSC", D_METHOD("new_from", "inPort", "outPort", "outIP"), &OSC::new_from);
    ClassDB::bind_method(D_METHOD("init", "inPort", "outPort", "outIP"), &OSC::init);
    ClassDB::bind_method(D_METHOD("sendBuffer", "buffer"), &OSC::sendBuffer);
    ClassDB::bind_method(D_METHOD("send", "address", "arguments"), &OSC::send);
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

OSC* OSC::new_from(int inPort, int outPort, String outIP) {
    // UtilityFunctions::print("OSC::new_from");

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

        // UtilityFunctions::print("[debug] OSC packet received: " + packet.get_string_from_utf8());

        std::shared_ptr<OSCMessage> msg = std::make_shared<OSCMessage>();
        msg->init(packet);

        // UtilityFunctions::print("OSC message received: " + msg->address());
        // UtilityFunctions::print("OSC message content: " + msg->toString());
        
        if (!msg->isValid()) {
            // UtilityFunctions::print("[debug] OSC message is invalid");
            return;
        }
        if (messageHandlers.has("*")) {
            Array empty = Array();
            Array arr = messageHandlers.get("*", empty);

            for (int i = 0; i < arr.size(); i++) {
                // UtilityFunctions::print("Calling handler in *");
                Callable handler = arr[i];
                // handler.call(msg.get());
                // handler.call("call", msg.get());
                // handler.call();
                handler.call(msg->address(), msg->getValues());
            }
        }
        if (messageHandlers.has(msg->address())) {
            Array arr = messageHandlers[msg->address()];

            for (int i = 0; i < arr.size(); i++) {
                // UtilityFunctions::print("Calling handler (hash " + String::num(handler.hash()) + ") in address: " + msg->address());
                Callable handler = arr[i];
                // handler.call(msg.get());
                // handler.call("call", msg.get());
                // handler.call();
                handler.call(msg->address(), msg->getValues());
            }
        }
    }
}

void OSC::sendBuffer(PackedByteArray buffer) {
    // UtilityFunctions::print("OSC::sendBuffer");

    Ref<PacketPeerUDP> udp = memnew(PacketPeerUDP);
    udp->connect_to_host(_outIP, _outPort);
    udp->put_packet(buffer);
    // UtilityFunctions::print("OSC UDP packet sent to " + _outIP + ":" + String::num_int64(_outPort));
}

void OSC::send(String address, Array arguments) {
    // UtilityFunctions::print("OSC::send");

    OSCMessage msg;
    msg.init(address);
    for (int i = 0; i < arguments.size(); i++) {
        msg.add(arguments[i]);
    }
    sendBuffer(msg.toPackedByteArray());
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

    // UtilityFunctions::print("OSC::onMessage: callback (hash " + String::num(callback.hash()) + ") added to address: " + address);
}