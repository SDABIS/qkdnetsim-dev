




#ifndef QKD_RANDOM_GENERATOR_H
#define QKD_RANDOM_GENERATOR_H

#include "ns3/random-variable-stream.h"
#include <algorithm>
#include <stdint.h>

#include "ns3/header.h"
#include "ns3/tcp-header.h"
#include "ns3/udp-header.h" 
#include "ns3/icmpv4.h"

#include "ns3/dsdv-packet.h"  
#include "ns3/aodv-packet.h" 
#include "ns3/olsr-header.h" 

#include "ns3/packet.h"
#include "ns3/packet-metadata.h"
#include "ns3/tag.h" 
#include "ns3/object.h"
#include "ns3/callback.h"
#include "ns3/assert.h"
#include "ns3/ptr.h"

#include "ns3/deprecated.h"
#include "ns3/traced-value.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/qkd-buffer.h"
#include "ns3/qkd-header.h"
#include "ns3/qkd-key.h"
#include "ns3/net-device.h"

#include <crypto++/aes.h>
#include <crypto++/modes.h>
#include <crypto++/filters.h>
#include <crypto++/hex.h>
#include <crypto++/osrng.h>
#include <crypto++/ccm.h>
#include <crypto++/vmac.h>
#include <crypto++/iterhash.h>
#include <crypto++/secblock.h>
#include <crypto++/sha.h>
#include <vector>
//#include "Quantis.hpp"


#define MAX_BUFFER_SIZE 16*1024*1024

namespace ns3 {

class QKDRandomGenerator {
    private:
        Ptr<UniformRandomVariable> randomgenerator;
        uint32_t buffer_size;
        uint32_t m_maxReq = 100000;

    public:
        static TypeId GetTypeId (void);
        QKDRandomGenerator();
        ~QKDRandomGenerator();
        void Dispose();
        void printCardsInfo();
        std::vector<uint8_t> generateStream(uint32_t amount);

};

}

#endif /* QKD_RANDOM_GENERATOR_H */