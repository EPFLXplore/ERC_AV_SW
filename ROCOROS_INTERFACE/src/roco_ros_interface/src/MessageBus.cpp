/*
 * MessageBus.cpp
 *
 *  Created on: 10 Dec 2019
 *      Author: Arion
 */

/*
 * Dynamic allocation should be avoided. If needed, use pvPortMalloc and pvPortFree instead of new and delete respectively.
 */

#include "MessageBus.h"
#include <unistd.h>

// Template explicit instantiation
#define REGISTER(P) 												\
	template bool MessageBus::define<P>(uint8_t);					\
	template bool MessageBus::handle<P>(std::function<void(uint8_t, P*, void*)>, void*);		\
	template bool MessageBus::forward<P>(MessageBus*);				\
	template bool MessageBus::send<P>(P*);


#include "Protocol/ProtocolRegisters.h"

#undef REGISTER



/*
 * Packet definitions
 *
 * Explicit template instantiation is needed.
 */

/*
 * Registers a message identifier for the specified structure/class.
 *
 * Allows I/O communication for the given message ID according to the bus specification.
 * Using std::typeindex allows more flexibility insofar as the message ID is directly inferred
 * using the template of send_packet.
 *
 * The two first bits of the identifier are used for storing dispatching metadata.
 *
 * Warning: this method is not thread-safe.
 */


// WARNING: using a hash as a UUID is a bad idea in general but it was to only way to get SWIG working.
template<typename T> bool MessageBus::define(uint8_t identifier) {
	size_t struct_size = sizeof(T);
	size_t hash = typeid(T).hash_code();

	uint32_t insertion_point = hash % 256;

	if(definitions_by_id[identifier & 0b00111111].hash != 0) {
		return false; // Packet ID already in use
	}

	if(struct_size > max_packet_size) {
		return false; // Packet size too large
	}

	while(definitions_by_type[insertion_point] != nullptr) {
		if(definitions_by_type[insertion_point]->hash == hash) {
			return false; // Packet type already defined
		}

		insertion_point++;

		if(insertion_point == 256) {
			insertion_point = 0;
		}
	}

	PacketDefinition* def = &definitions_by_id[identifier & 0b00111111];

	def->id = identifier;
	def->size = (uint8_t) struct_size;
	def->hash = hash;

	definitions_by_type[insertion_point] = def;

	return true;
}


/*
 * Registers a handler for this event bus.
 *
 * Accepts a function reference as message handler.
 *
 * Warning: this method is not thread-safe.
 */
template<typename T> bool MessageBus::handle(std::function<void(uint8_t, T*, void*)> handler, void* publisher) {
	size_t hash = typeid(T).hash_code();

	PacketDefinition* def = retrieve(hash);

	if(def != nullptr) {
		uint8_t packetID = def->id;

		if(handlers[packetID] != nullptr) {
			handlers[packetID] = [this, packetID, &handler, &publisher](uint8_t sender, void* packet, void* pub) {
				handlers[packetID](sender, (T*) packet, pub);
				handler(sender, (T*) packet, pub);
			};
		} else {
			handlers[packetID] = [handler](uint8_t sender, void* packet, void* pub) { handler(sender, (T*) packet, pub); };
			publishers[packetID] = publisher;
		}

		return true;
	}

	return false;
}

/*
 * Registers a forwarder for this event bus.
 *
 * Every time a packet matching to given type is received, forwards it to the other message bus.
 *
 * Warning: this method is not thread-safe.
 */
template<typename T> bool MessageBus::forward(MessageBus* bus) {
	size_t hash = typeid(T).hash_code();

	PacketDefinition* def = retrieve(hash);

	if(def != nullptr) {
		uint8_t packetID = def->id;

		if(forwarders[packetID] != nullptr) {
			return false; // A forwarder is already registered for this packet type
		}

		forwarders[packetID] = bus;

		return true;
	}

	return false;
}

/*
 * Sends the given message using the implemented transmission protocol.
 */
template<typename T> bool MessageBus::send(T *message) {
	size_t hash = typeid(T).hash_code();

	PacketDefinition* def = retrieve(hash);

	return send(def, (uint8_t*) message);
}

bool MessageBus::send(PacketDefinition* def, uint8_t* data) {
	if(def != nullptr) {
		uint32_t data_bytes_written = 0;
//		append(&def->id, 1);
		while(data_bytes_written < def->size) {
			append(&def->id, 1); // Write the packet ID for each transmission frame.
							     // This is only to facilitate the packet reconstruction and should not increment data_bytes_written.

			uint32_t new_bytes = append(data + data_bytes_written, def->size - data_bytes_written); // Send the data

			if(new_bytes == 0) {
				return false;
			} else {
				data_bytes_written += new_bytes;
			}
		}
		transmit();
		usleep(this->mid_sends_interval);
		return true;
	}

	return false;
}

/*
 * Handles the reception of a message.
 *
 * Provided an external thread calls this method with a buffer to the next incoming message,
 * dispatches the message to the appropriate message handlers.
 */
void MessageBus::receive(uint8_t sender_id, uint8_t *pointer, uint32_t length) {
	if(length > 0) {
		ReconstructionBuffer* indexable_buffer = &reconstruction_buffers[sender_id & 0b00111111];
		uint8_t packet_id = 0xFF;

		if(indexable_buffer->index == 0) {
			packet_id = *pointer++; // Packet ID is the first element in a frame
			indexable_buffer->current_packet_id = packet_id;
			indexable_buffer->index++;
			length--;
		} else {
			packet_id = indexable_buffer->current_packet_id;
		}

		PacketDefinition* def = &definitions_by_id[packet_id & 0b00111111];

		if(def->id != packet_id || indexable_buffer->index + length > max_packet_size) {
			// Invalid packet
			indexable_buffer->index = 0; // Corrupted packet
			indexable_buffer->current_packet_id = 0;


			//console.printf("Corrupted ID: %d\r\n", packet_id);

			receive(sender_id, pointer, length);

			return;
		}

		for(uint16_t i = 0; i < length; i++) {
			indexable_buffer->buffer[indexable_buffer->index++] = *pointer++;
		}

		int16_t excess = indexable_buffer->index - (def->size + 1);

		if(excess >= 0) {
			// Packet is complete.
			//console.printf("Correct ID: %d\r\n", packet_id);

			if(handlers[packet_id & 0b00111111] != nullptr) {
				handlers[packet_id & 0b00111111](sender_id, indexable_buffer->buffer + 1, publishers[packet_id & 0b00111111]);
			}


			if(forwarders[packet_id & 0b00111111] != nullptr) {
				forwarders[packet_id & 0b00111111]->send(def, indexable_buffer->buffer + 1);
			}

			if(excess > 0) {
				for(uint16_t i = 0; i < excess; i++) {
					indexable_buffer->buffer[i] = indexable_buffer->buffer[i + (def->size + 1)];
				}

				indexable_buffer->current_packet_id = indexable_buffer->buffer[0];
			}

			indexable_buffer->index = excess;
		}
	}
}

/*
 * Searches a packet definition matching the given type in the hash table.
 */
PacketDefinition* MessageBus::retrieve(size_t hash) {
	uint32_t searchPoint = hash % 256;
	uint32_t searchStart = searchPoint;

	while(definitions_by_type[searchPoint] != nullptr) {
		if(definitions_by_type[searchPoint]->hash == hash) {
			return definitions_by_type[searchPoint];
		}

		searchPoint++;

		if(searchPoint == 256) {
			searchPoint = 0;
		}

		if(searchStart == searchPoint) {
			break; // No packet definition matching the given template type
		}
	}

	return nullptr;
}
