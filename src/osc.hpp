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

    // std::unique_ptr<UDPServer> server;
    UDPServer* server;
    Dictionary messageHandlers;
    // std::map<String, Vector<Callable>> messageHandlers;
    String _outIP = "127.0.0.1";
    int _inPort = 9999;
    int _outPort = 8888;

    OSC();
    ~OSC();

    // void _init(int inPort, int outPort, String outIP);
    static OSC* new_from(int inPort, int outPort, String outIP);
    void init(int inPort, int outPort, String outIP);
    void set_max_pending_connections(int p_max_pending_connections);
    void _ready() override;
    void _process(double delta) override;
    void _process_packet(PackedByteArray packet);

    void sendBuffer(PackedByteArray buffer);
    void send(String address, Array arguments);
    void stop();
    void onMessage(String address, Callable callback);
};

}