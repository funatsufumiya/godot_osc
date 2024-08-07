// # GDScript:
// class_name OSC_MSG

// var addrPatern:String
// var separator = ','.to_ascii_buffer()[0]
// var KOMMA = 0x2c
// var ZEROBYTE = 0x00
// var _myAddrInt = -1
// var _myAddrPattern = ""
// var _myTypetag = []
// var _myData = []
// var _myArguments = []
// var isArray = false
// var _myArrayType = 0x00
// var _isValid = false

// var address :  String :
// 	get:
// 		return _myAddrPattern
// 	set(value):
// 		_myAddrPattern = value
		
// var isValid :  bool :
// 	get:
// 		return _isValid

// var dataLength :  int :
// 	get:
// 		return _myArguments.size()

// func getValue(id:int):
// 	return _myArguments[id]

// func getValues():
// 	return _myArguments

// func add(value):
// 	_myArguments.push_back(value)
// 	match typeof(value):
// 		TYPE_FLOAT:
// 			_myTypetag.push_back(0x66)
// 		TYPE_STRING:
// 			_myTypetag.push_back(0x73)
// 		TYPE_INT:
// 			_myTypetag.push_back(0x69)
// 		TYPE_PACKED_BYTE_ARRAY:
// 			_myTypetag.push_back(0x62)
// #
// #	if("PackedByteArray" == type_string(typeof(value))):
// #		_myTypetag.push_back(0x62)
// #	elif("String" == type_string(typeof(value))):
// #		_myTypetag.push_back(0x73)
// #	elif("int" == type_string(typeof(value))):
// #		_myTypetag.push_back(0x69)
// #	elif("float" == type_string(typeof(value))):
		
// 	return self

// func padSize(bytes:int): 
// 	return ( 4- (bytes & 03)) & 3


// func toPackedByteArray():
// 	var output = PackedByteArray()
// 	var addrPad = padSize(_myAddrPattern.length() + 1)
// 	output.append_array ( _myAddrPattern.to_utf8_buffer() )
// 	output.append(ZEROBYTE)
// 	while(addrPad>0):
// 		output.append(ZEROBYTE)
// 		addrPad-=1
// 	output.append(KOMMA)
// 	for i in range(0,_myTypetag.size()):
// 		output.append(_myTypetag[i])
// 	var typePad = padSize(_myTypetag.size() + 1);
// 	while(typePad>0):
// 		output.append(ZEROBYTE)
// 		typePad-=1
// 	for i in range(0,_myArguments.size()):
// 		match _myTypetag[i]:
// 			0x69:#'i'.to_ascii_buffer()[0]: # Interger
// 				var data = PackedByteArray()
// 				data.resize(4)
// 				data.encode_s32(0, _myArguments[i])
// 				data.reverse()
// 				output.append_array(data)
// 			0x66:#'f'.to_ascii_buffer()[0]: # Float
// 				var data = PackedByteArray()
// 				data.resize(4)
// 				data.encode_float(0, _myArguments[i])
// 				data.reverse()
// 				output.append_array(data)
// 			0x73:#'s'.to_ascii_buffer()[0]: # String
// 				output.append_array ( _myArguments[i].to_utf8_buffer() )
// 				output.append(ZEROBYTE)
// 				var dataPad = padSize(_myArguments[i].length() + 1)
// 				while(dataPad>0):
// 					output.append(ZEROBYTE)
// 					dataPad-=1
// 			0x62:#'b'.to_ascii_buffer()[0]: # Blob
// 				output.append_array ( _myArguments[i] )
// 				var dataPad = padSize(_myArguments[i].length())
// 				while(dataPad>0):
// 					output.append(ZEROBYTE)
// 					dataPad-=1
// 	return output

// func send(sender:OSC):
// 	sender.send(toPackedByteArray())
// 	return self
	
// func toString():
// 	var data = ""
// 	for arg in _myArguments:
// 		data += str(arg) + " "
		
// 	return self.address + " " + data

// func _init(value):
	
	
	
// 	if(TYPE_PACKED_BYTE_ARRAY == typeof(value)):
// 		_parseMessage(value)
// 	elif(TYPE_STRING
// 	 == typeof(value)):
// 		self.address = value

// func _parseMessage(theBytes):
// 	var myLength = theBytes.size()
// 	var myIndex = 0;
// 	myIndex = _parseAddrPattern(theBytes, myLength, myIndex);
// 	if (myIndex != -1):
// 		myIndex = _parseTypetag(theBytes, myLength, myIndex);
// 	if (myIndex != -1):
// 		_myData = theBytes.slice(myIndex)
// 		_myArguments = _parseArguments(_myData);
// 		_isValid = true;
	
// func _parseAddrPattern(theBytes, theLength, theIndex):
// 	if (theLength > 4 && theBytes[4] == KOMMA):
// 		_myAddrInt = theBytes.slice(0, 4).to_int32_array()
// 	for i in range(theIndex,theLength):
// 		if (theBytes[i] == ZEROBYTE):
// 			_myAddrPattern = theBytes.slice(theIndex, theIndex+i).get_string_from_utf8()
// 			return i + _align(i)
// 	return -1;

// func _parseTypetag(theBytes, theLength, theIndex):
// 	if (theBytes[theIndex] == KOMMA):
// 		theIndex+=1
// 		for i in range(theIndex,theLength):
// 			if (theBytes[i] == ZEROBYTE):
// 				_myTypetag = theBytes.slice(theIndex, i)
// 				return i + _align(i);
// 	return -1

// func _parseArguments(theBytes):
// 	var myArguments = []
// 	var myTagIndex = 0
// 	var myIndex = 0
// 	myArguments.resize(_myTypetag.size())
// 	myArguments.fill(0) # Initialize the 10 elements to 0.
	
// 	isArray = _myTypetag.size() > 0
	
// 	while (myTagIndex < _myTypetag.size()):
// 		# check if we still save the arguments as an array
// 		if (myTagIndex == 0):
// 			_myArrayType = _myTypetag[myTagIndex]
// 		else:
// 			if (_myTypetag[myTagIndex] != _myArrayType):
// 				isArray = false
// 		match _myTypetag[myTagIndex]:
// 			0x69:#'i'.to_ascii_buffer()[0]: # Interger
// 				myArguments[myTagIndex] = theBytes.slice(myIndex, myIndex+4)
// 				myArguments[myTagIndex].reverse()
// 				myArguments[myTagIndex] = myArguments[myTagIndex].to_int32_array()[0]
// 				myIndex += 4;
// 			0x66:#'f'.to_ascii_buffer()[0]: # Float
// 				myArguments[myTagIndex] = theBytes.slice(myIndex, myIndex+4)
// 				myArguments[myTagIndex].reverse()
// 				myArguments[myTagIndex] = myArguments[myTagIndex].to_float32_array()[0]
// 				myIndex += 4;
// 			0x73:#'s'.to_ascii_buffer()[0]: # String
// 				myArguments[myTagIndex] = theBytes.slice(myIndex).get_string_from_utf8()
// 				var newIndex = myIndex + myArguments[myTagIndex].length()
// 				myIndex = newIndex + _align(newIndex)
// 			0x62:#'b'.to_ascii_buffer()[0]: # Blob
// 				var myLen = theBytes.slice(myIndex, myIndex+4)
// 				myLen.reverse()
// 				myLen = myLen.to_int32_array()[0]
// 				myIndex += 4;
// 				myArguments[myTagIndex] = theBytes.slice(myIndex, myIndex+myLen)
// 				myIndex += myLen + (_align(myLen) % 4)
// 		myTagIndex+=1
// 	_myData = _myData.slice(0, myIndex)
// 	return myArguments

// func _align(theInt:int):
// 	return (4 - (theInt % 4))


// # C++:

#include <godot_cpp/core/class_db.hpp>

#include "osc_message.hpp"

using namespace godot;

void OSCMessage::_bind_methods() {
    ClassDB::bind_method(D_METHOD("init", "value"), &OSCMessage::init);
    ClassDB::bind_method(D_METHOD("send", "sender"), &OSCMessage::send);
    ClassDB::bind_method(D_METHOD("toPackedByteArray"), &OSCMessage::toPackedByteArray);
    ClassDB::bind_method(D_METHOD("toString"), &OSCMessage::toString);
    ClassDB::bind_method(D_METHOD("getValue", "id"), &OSCMessage::getValue);
    ClassDB::bind_method(D_METHOD("getValues"), &OSCMessage::getValues);
    ClassDB::bind_method(D_METHOD("add", "value"), &OSCMessage::add);
    ClassDB::bind_method(D_METHOD("padSize", "bytes"), &OSCMessage::padSize);
    ClassDB::bind_method(D_METHOD("_parseMessage", "theBytes"), &OSCMessage::_parseMessage);
    ClassDB::bind_method(D_METHOD("_parseAddrPattern", "theBytes", "theLength", "theIndex"), &OSCMessage::_parseAddrPattern);
    ClassDB::bind_method(D_METHOD("_parseTypetag", "theBytes", "theLength", "theIndex"), &OSCMessage::_parseTypetag);
    ClassDB::bind_method(D_METHOD("_parseArguments", "theBytes"), &OSCMessage::_parseArguments);
    ClassDB::bind_method(D_METHOD("_align", "theInt"), &OSCMessage::_align);
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

int OSCMessage::padSize(int bytes) {
    return (4 - (bytes & 03)) & 3;
}

PackedByteArray OSCMessage::toPackedByteArray() {
    PackedByteArray output;
    int addrPad = padSize(_myAddrPattern.length() + 1);
    output.append_array(_myAddrPattern.to_utf8());
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
        switch (_myTypetag[i]) {
            case 0x69: // 'i'.to_ascii_buffer()[0]: // Interger
                PackedByteArray data;
                data.resize(4);
                data.encode_s32(0, _myArguments[i]);
                data.reverse();
                output.append_array(data);
                break;
            case 0x66: // 'f'.to_ascii_buffer()[0]: // Float
                PackedByteArray data;
                data.resize(4);
                data.encode_float(0, _myArguments[i]);
                data.reverse();
                output.append_array(data);
                break;
            case 0x73: // 's'.to_ascii_buffer()[0]: // String
                output.append_array(_myArguments[i].to_utf8());
                output.append(0x00);
                int dataPad = padSize(_myArguments[i].length() + 1);
                while (dataPad > 0) {
                    output.append(0x00);
                    dataPad -= 1;
                }
                break;
            case 0x62: // 'b'.to_ascii_buffer()[0]: // Blob
                output.append_array(_myArguments[i]);
                int dataPad = padSize(_myArguments[i].length());
                while (dataPad > 0) {
                    output.append(0x00);
                    dataPad -= 1;
                }
                break;
        }
    }
    return output;
}

OSCMessage* OSCMessage::send(OSC* sender) {
    sender->send(toPackedByteArray());
    return this;
}

String OSCMessage::toString() {
    String data = "";
    for (int i = 0; i < _myArguments.size(); i++) {
        data += String::num(_myArguments[i]) + " ";
    }
    return this->address() + " " + data;
}

void OSCMessage::_parseMessage(PackedByteArray theBytes) {
    int myLength = theBytes.size();
    int myIndex = 0;
    myIndex = _parseAddrPattern(theBytes, myLength, myIndex);
    if (myIndex != -1) {
        myIndex = _parseTypetag(theBytes, myLength, myIndex);
    }
    if (myIndex != -1) {
        _myData = theBytes.slice(myIndex);
        _myArguments = _parseArguments(_myData);
        _isValid = true;
    }
}

int OSCMessage::_parseAddrPattern(PackedByteArray theBytes, int theLength, int theIndex) {
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
        switch (_myTypetag[myTagIndex]) {
            case 0x69: // 'i'.to_ascii_buffer()[0]: // Interger
                myArguments[myTagIndex] = theBytes.slice(myIndex, myIndex + 4);
                myArguments[myTagIndex].reverse();
                myArguments[myTagIndex] = myArguments[myTagIndex].to_int32_array()[0];
                myIndex += 4;
                break;
            case 0x66: // 'f'.to_ascii_buffer()[0]: // Float
                myArguments[myTagIndex] = theBytes.slice(myIndex, myIndex + 4);
                myArguments[myTagIndex].reverse();
                myArguments[myTagIndex] = myArguments[myTagIndex].to_float32_array()[0];
                myIndex += 4;
                break;
            case 0x73: // 's'.to_ascii_buffer()[0]: // String
                myArguments[myTagIndex] = theBytes.slice(myIndex).get_string_from_utf8();
                int newIndex = myIndex + myArguments[myTagIndex].length();
                myIndex = newIndex + _align(newIndex);
                break;
            case 0x62: // 'b'.to_ascii_buffer()[0]: // Blob
                int myLen = theBytes.slice(myIndex, myIndex + 4);
                myLen.reverse();
                myLen = myLen.to_int32_array()[0];
                myIndex += 4;
                myArguments[myTagIndex] = theBytes.slice(myIndex, myIndex + myLen);
                myIndex += myLen + (_align(myLen) % 4);
                break;
        }
        myTagIndex += 1;
    }
    _myData = _myData.slice(0, myIndex);
    return myArguments;
}

int OSCMessage::_align(int theInt) {
    return (4 - (theInt % 4));
}