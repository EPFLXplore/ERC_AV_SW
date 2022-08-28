/*
 * MessageBus.h
 *
 *  Created on: 09 Feb 2020
 *      Author: Arion
 */

#ifndef MESSAGE_BUS_H_
#define MESSAGE_BUS_H_

#include <cstdint>
#include <cstddef>
#include <typeindex>
#include <functional>



struct PacketDefinition {
	uint8_t id;
	uint8_t size;
	size_t hash;
};

class MessageBus {
public:
	virtual ~MessageBus() {}

	template<typename T> bool define(uint8_t identifier);
	template<typename T> bool handle(std::function<void(uint8_t, T*, void*)> handler, void*);
	template<typename T> bool forward(MessageBus* bus);
	template<typename T> bool send(T *message);

	void receive(uint8_t senderID, uint8_t *pointer, uint32_t length);
	virtual uint8_t append(uint8_t* buffer, uint32_t length) = 0; // Must be atomic
	virtual void transmit() = 0;
	virtual bool send(PacketDefinition* def, uint8_t* data);
	virtual PacketDefinition* retrieve(size_t hash);

private:
	static const uint32_t max_packet_size = 255;
	static const uint32_t max_unique_senders = 8;
	static const uint32_t mid_sends_interval = 10000; // [us]

	struct ReconstructionBuffer {
		uint8_t current_packet_id;
		uint8_t buffer[max_packet_size];
		uint8_t index;
	};

	PacketDefinition definitions_by_id[64];
	PacketDefinition* definitions_by_type[256]; // Factor 4 to mitigate hash collisions
	ReconstructionBuffer reconstruction_buffers[max_unique_senders];

	std::function<void(uint8_t, void*, void*)> handlers[64];
	MessageBus* forwarders[64];
	void* publishers[64];
	
};


#endif /* MESSAGE_BUS_H_ */
