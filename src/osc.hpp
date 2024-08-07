#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/udp_server.hpp>
// #include <godot_cpp/core/class_db.hpp>

namespace godot {

class OSC : public Node
{
    GDCLASS(OSC, Node);

protected:
	static void _bind_methods();

public:

    UDPServer *server;
    Dictionary messageHandlers;
    String _outIP = "127.0.0.1";
    int _inPort = 9999;
    int _outPort = 8888;

    OSC();
    ~OSC();

    // void _init(int inPort, int outPort, String outIP);
    // static OSC create(int inPort, int outPort, String outIP);
    void init(int inPort, int outPort, String outIP);
    void _ready() override;
    void _process(double delta) override;

    void send(PackedByteArray buffer);
    void stop();
    void onMessage(String address, Callable callback);
};

}