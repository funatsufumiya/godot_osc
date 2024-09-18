#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

#include "osc_message.hpp"
#include "osc.hpp"

using namespace godot;

void OSCMessage::_bind_methods() {
    ClassDB::bind_static_method("OSCMessage", D_METHOD("new_from", "value"), &OSCMessage::new_from);
    ClassDB::bind_method(D_METHOD("init", "value"), &OSCMessage::init);
    ClassDB::bind_method(D_METHOD("send", "sender"), &OSCMessage::send);
    ClassDB::bind_method(D_METHOD("toPackedByteArray"), &OSCMessage::toPackedByteArray);
    ClassDB::bind_method(D_METHOD("toString"), &OSCMessage::toString);
    ClassDB::bind_method(D_METHOD("getValue", "id"), &OSCMessage::getValue);
    ClassDB::bind_method(D_METHOD("getValues"), &OSCMessage::getValues);
    ClassDB::bind_method(D_METHOD("add", "value"), &OSCMessage::add);
    ClassDB::bind_method(D_METHOD("padSize", "bytes"), &OSCMessage::padSize);
    // ClassDB::bind_method(D_METHOD("_parseMessage", "theBytes"), &OSCMessage::_parseMessage);
    // ClassDB::bind_method(D_METHOD("_parseAddrPattern", "theBytes", "theLength", "theIndex"), &OSCMessage::_parseAddrPattern);
    // ClassDB::bind_method(D_METHOD("_parseTypetag", "theBytes", "theLength", "theIndex"), &OSCMessage::_parseTypetag);
    // ClassDB::bind_method(D_METHOD("_parseArguments", "theBytes"), &OSCMessage::_parseArguments);
    // ClassDB::bind_method(D_METHOD("_align", "theInt"), &OSCMessage::_align);
}

OSCMessage::OSCMessage()
{
}

OSCMessage::~OSCMessage()
{
}

OSCMessage* OSCMessage::new_from(Variant value) {
    OSCMessage* oscMessage = memnew(OSCMessage);
    oscMessage->init(value);
    return oscMessage;
}

void OSCMessage::init(Variant value) {
    if (value.get_type() == Variant::Type::PACKED_BYTE_ARRAY) {
        _parseMessage(value);
    } else if (value.get_type() == Variant::Type::STRING) {
        _myAddrPattern = value;
    }
}

String OSCMessage::address() {
    return _myAddrPattern;
}

bool OSCMessage::isValid() {
    return _isValid;
}

int OSCMessage::dataLength() {
    return _myArguments.size();
}

Variant OSCMessage::getValue(int id) {
    return _myArguments[id];
}

Array OSCMessage::getValues() {
    return _myArguments;
}

OSCMessage* OSCMessage::add(Variant value) {
    _myArguments.push_back(value);
    switch (value.get_type()) {
        case Variant::Type::FLOAT:
            _myTypetag.push_back(0x66);
            break;
        case Variant::Type::STRING:
            _myTypetag.push_back(0x73);
            break;
        case Variant::Type::INT:
            _myTypetag.push_back(0x69);
            break;
        case Variant::Type::PACKED_BYTE_ARRAY:
            _myTypetag.push_back(0x62);
            break;
    }
    return this;
}

// int OSCMessage::padSize(int bytes) {
//     return (4 - (bytes & 03)) & 3;
// }

int OSCMessage::padSize(int bytes) {
    int remainder = bytes % 4;
    switch (remainder) {
        case 0: return 4;
        case 1: return 3;
        case 2: return 2;
        case 3: return 1;
        default: return 0; // should never happen
    }
}

PackedByteArray OSCMessage::toPackedByteArray() {
    PackedByteArray output;
    int addrPad = padSize(_myAddrPattern.length() + 1);
    output.append_array(_myAddrPattern.to_utf8_buffer());
    output.append(0x00);
    while (addrPad > 0) {
        output.append(0x00);
        addrPad -= 1;
    }
    output.append(0x2c);
    for (int i = 0; i < _myTypetag.size(); i++) {
        output.append(_myTypetag[i]);
    }
    int typePad = padSize(_myTypetag.size() + 1);
    while (typePad > 0) {
        output.append(0x00);
        typePad -= 1;
    }
    for (int i = 0; i < _myArguments.size(); i++) {
        Variant v = _myTypetag[i];
        char t = (uint8_t)v;
        if (t == 0x69) { // 'i'.to_ascii_buffer()[0]: // Interger
            PackedByteArray data;
            data.resize(4);
            data.encode_s32(0, _myArguments[i]);
            data.reverse();
            output.append_array(data);
        } else if (t == 0x66) { // 'f'.to_ascii_buffer()[0]: // Float
            PackedByteArray data;
            data.resize(4);
            data.encode_float(0, _myArguments[i]);
            data.reverse();
            output.append_array(data);
        } else if (t == 0x73) { // 's'.to_ascii_buffer()[0]: // String
            String s = (String)_myArguments[i];
            output.append_array(s.to_utf8_buffer());
            output.append(0x00);
            int dataPad = padSize(s.length() + 1);
            while (dataPad > 0) {
                output.append(0x00);
                dataPad -= 1;
            }
        } else if (t == 0x62) { // 'b'.to_ascii_buffer()[0]: // Blob
            // FIXME: is this the right type for blob?
            // String b = (String)_myArguments[i];
            PackedByteArray b = (PackedByteArray)_myArguments[i];
            output.append_array(b);
            // int dataPad = padSize(b.length());
            int dataPad = padSize(b.size());
            while (dataPad > 0) {
                output.append(0x00);
                dataPad -= 1;
            }
        }
    }
    return output;
}

OSCMessage* OSCMessage::send(OSC* sender) {
    sender->sendBuffer(toPackedByteArray());
    return this;
}

String OSCMessage::toString() {
    String data = "";
    for (int i = 0; i < _myArguments.size(); i++) {
        data += String::num(_myArguments[i]) + " ";
    }
    return this->address() + " " + data;
}

TypedArray<PackedByteArray> OSCMessage::_parseMessage(PackedByteArray theBytes) {
    int myLength = theBytes.size();
    int myIndex = 0;
    myIndex = _parseAddrPattern(theBytes, myLength, myIndex);

    bool is_bundle = false;
    // PackedByteArray bundle_timetag;
    TypedArray<PackedByteArray> rest_messages;

    int bundle_first_message_end_index = -1;

    if (_myAddrPattern == "#bundle") {
        is_bundle = true;

        // NOTE: only parse first message in bundle. The rest will be returned as is

//         Parts of an OSC Bundle
// Data	Size	Purpose
// OSC-string “#bundle”	8 bytes	How to know that this data is a bundle
// OSC-timetag	8 bytes	Time tag that applies to the entire bundle
// Size of first bundle element	int32 = 4 bytes	
// First bundle element’s contents	As many bytes as given by “size of first bundle element”	First bundle element
// Size of second bundle element	int32 = 4 bytes	
// Second bundle element’s contents	As many bytes as given by “size of second bundle element”	Second bundle element
// etc.		Addtional bundle elements
        
        // UtilityFunctions::print("[debug] OSC bundle received");
        // UtilityFunctions::print("[debug] theBytes: ");
        // UtilityFunctions::print(theBytes);

        // skip the timetag
        // bundle_timetag = theBytes.slice(myIndex, myIndex + 8);
        myIndex += 8;
        // skip the size of the first bundle element
        PackedByteArray data = theBytes.slice(myIndex, myIndex + 4);
        data.reverse();
        int myLen = data.to_int32_array()[0];
        myIndex += 4;

        // UtilityFunctions::print("[debug] OSC bundle message length: " + String::num(myLen));
        // UtilityFunctions::print("[debug] OSC bundle data: ");
        // UtilityFunctions::print(theBytes.slice(myIndex, myIndex + myLen));

        int idx = _parseAddrPattern(theBytes.slice(myIndex, myIndex + myLen), myLen, 0);
        // UtilityFunctions::print("[debug] OSC bundle message address: " + _myAddrPattern);
        myIndex += idx;

        bundle_first_message_end_index = myIndex + myLen;
    }

    if (myIndex != -1) {
        myIndex = _parseTypetag(theBytes, myLength, myIndex);
    }
    if (myIndex != -1) {
        _myData = theBytes.slice(myIndex);
        _myArguments = _parseArguments(_myData);
        _isValid = true;
    }

    if (is_bundle) {
        // skip size of the rest of the bundle elements
        TypedArray<PackedByteArray> rest_messages = TypedArray<PackedByteArray>();
        myIndex = bundle_first_message_end_index;
        while (myIndex < myLength) {
            PackedByteArray data = theBytes.slice(myIndex, myIndex + 4);
            data.reverse();
            int myLen = data.to_int32_array()[0];
            myIndex += 4;
            rest_messages.push_back(theBytes.slice(myIndex, myIndex + myLen));
            myIndex += myLen;
        }

        // UtilityFunctions::print("[debug] OSC bundle rest message num " + String::num(rest_messages.size()));
        
        return rest_messages;
    } else {
        return TypedArray<PackedByteArray>();
    }
}

int OSCMessage::_parseAddrPattern(PackedByteArray theBytes, int theLength, int theIndex) {
    // when if #bundle\0
    if (theLength > 7 && theBytes[0] == 0x23 && theBytes[1] == 0x62 && theBytes[2] == 0x75 && theBytes[3] == 0x6e && theBytes[4] == 0x64 && theBytes[5] == 0x6c && theBytes[6] == 0x65 && theBytes[7] == 0x00) {
        _myAddrPattern = "#bundle";
        return 8;
    }

    if (theLength > 4 && theBytes[4] == 0x2c) {
        _myAddrInt = theBytes.slice(0, 4).to_int32_array();
    }
    for (int i = theIndex; i < theLength; i++) {
        if (theBytes[i] == 0x00) {
            _myAddrPattern = theBytes.slice(theIndex, theIndex + i).get_string_from_utf8();
            return i + _align(i);
        }
    }
    return -1;
}

int OSCMessage::_parseTypetag(PackedByteArray theBytes, int theLength, int theIndex) {
    if (theBytes[theIndex] == 0x2c) {
        theIndex += 1;
        for (int i = theIndex; i < theLength; i++) {
            if (theBytes[i] == 0x00) {
                _myTypetag = theBytes.slice(theIndex, i);
                return i + _align(i);
            }
        }
    }
    return -1;
}

Array OSCMessage::_parseArguments(PackedByteArray theBytes) {
    Array myArguments;
    int myTagIndex = 0;
    int myIndex = 0;
    myArguments.resize(_myTypetag.size());
    myArguments.fill(0); // Initialize the 10 elements to 0.
    isArray = _myTypetag.size() > 0;
    while (myTagIndex < _myTypetag.size()) {
        // check if we still save the arguments as an array
        if (myTagIndex == 0) {
            _myArrayType = _myTypetag[myTagIndex];
        } else {
            if (_myTypetag[myTagIndex] != _myArrayType) {
                isArray = false;
            }
        }

        Variant v = _myTypetag[myTagIndex];
        char t = (uint8_t)v;
        if (t == 0x69) { // 'i'.to_ascii_buffer()[0]: // Interger
            PackedByteArray data = theBytes.slice(myIndex, myIndex + 4);
            data.reverse();
            myArguments[myTagIndex] = data.to_int32_array()[0];
            myIndex += 4;
        } else if (t == 0x66) { // 'f'.to_ascii_buffer()[0]: // Float
            PackedByteArray data = theBytes.slice(myIndex, myIndex + 4);
            data.reverse();
            myArguments[myTagIndex] = data.to_float32_array()[0];
            myIndex += 4;
        } else if (t == 0x73) { // 's'.to_ascii_buffer()[0]: // String
            String s = theBytes.slice(myIndex).get_string_from_utf8();
            myArguments[myTagIndex] = s;
            int newIndex = myIndex + s.length();
            myIndex = newIndex + _align(newIndex);
        } else if (t == 0x62) { // 'b'.to_ascii_buffer()[0]: // Blob
            PackedByteArray data = theBytes.slice(myIndex, myIndex + 4);
            data.reverse();
            int myLen = data.to_int32_array()[0];
            myIndex += 4;
            myArguments[myTagIndex] = theBytes.slice(myIndex, myIndex + myLen);
            myIndex += myLen + (_align(myLen) % 4);
        }
        myTagIndex += 1;
    }
    _myData = _myData.slice(0, myIndex);
    return myArguments;
}

int OSCMessage::_align(int theInt) {
    return (4 - (theInt % 4));
}