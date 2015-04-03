#include "routing.h"
#include <string.h>

static uint8_t framePayload[MAX_PAYLOAD_SIZE];

static RoutingTableEntry routingTable[ROUTING_TABLE_ENTRIES];

bool getNextHop(uint8_t dest, uint8_t *nextHop);
bool forward(uint8_t nextHop, uint8_t *framePayload, uint8_t payloadLen);

bool getNextHop(uint8_t dest, uint8_t *nextHop) {
    uint8_t i = 0;
    bool entryFound = false;
    RoutingTableEntry r;
    for (i = 0; i < ROUTING_TABLE_ENTRIES; i++) {
        r = routingTable[i];
        if (r.dest == dest) {
            entryFound = true;
            break;
        }
    }
    *nextHop = r.nextHop;
    return entryFound;
}

bool forward(uint8_t nextHop, uint8_t *framePayload, uint8_t frameLen) {

    uint8_t send_time;
    bool acked;
    uint8_t retries = 0;

    while (retries < 5) {
        sendFrame(nextHop, &framePayload, frameLen);


        send_time = rtc_get_time();
        acked = false;
        while (rtc_get_time() - send_time < 50) {
            if ((acked = ackReceived(nextHop)))
                break;
        }

        if (acked) {
            return true;
        }

        retries ++;
    }

    return false;
}

void sendPacket(PacketHeader *h, uint8_t *payload, uint8_t payloadLen) {

    // Treat the packet header as a byte array
    uint8_t *harray = (uint8_t *)h;

    // Put header and payload into framePayload
    memcpy(framePayload, harray, PACKET_HEADER_SIZE);
    memcpy(&framePayload[PACKET_HEADER_SIZE], payload, payloadLen);

    // Try to find the next hop to the destination
    uint8_t nextHop;
    bool entryFound = getNextHop(h->dest, &nextHop);

    if (entryFound) {

        // Attempt to forward the packet
        forward(nextHop, &framePayload, payloadLen + PACKET_HEADER_SIZE);

    } else {

        // Do the hard part

    }

}

void handleReceived() {

    uint8_t senderNodeId, frameLength;
    bool needsAck;

    while (framesToRead()) {

        // Get the raw frame
        getNextFrame(&senderNodeId, &frameLength, &framePayload, &needsAck);

        // Send an ack if needed
        if (needsAck) {
            sendAck(senderNodeId);
        }

        // Unpack the packet header from the raw frame
        PacketHeader *h = (PacketHeader *)framePayload;

        // Realistically, here we'll want to update our own routing tables
        // with information about h->source

        // Get the next hop to the packet's destination
        uint8_t nextHop;
        bool entryFound = getNextHop(h->dest, &nextHop);

        if (entryFound) {

            forward(nextHop, framePayload, frameLength);

        } else {

            // Do the hard part

        }

    }

}
