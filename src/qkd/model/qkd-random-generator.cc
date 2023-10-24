

#include "qkd-random-generator.h"
#include "ns3/packet.h"
#include "ns3/assert.h"
#include "ns3/log.h" 
#include <string>
#include <cstdarg>
#include <iostream>
#include <sstream>
  
#include "ns3/header.h"
#include "ns3/tcp-header.h"
#include "ns3/udp-header.h" 
#include "ns3/icmpv4.h"

#include "ns3/dsdv-packet.h" 
#include "ns3/dsdvq-packet.h" 

#include "ns3/aodv-packet.h"
#include "ns3/aodvq-packet.h"

#include "ns3/olsr-header.h" 

#include "ns3/node.h"
#include "ns3/qkd-internal-tag.h"
#include "ns3/virtual-ipv4-l3-protocol.h"
 
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/copy.hpp>

namespace ns3 {

/*NS_LOG_COMPONENT_DEFINE ("QKDRandomGenerator");
NS_OBJECT_ENSURE_REGISTERED (QKDRandomGenerator);
TypeId 
QKDRandomGenerator::GetTypeId (void)
{
  
  static TypeId tid = TypeId ("ns3::QKDRandomGenerator")
    .SetParent<Object> () 
    .AddAttribute ("MaxReq", "The maximum amount of key size to be requested to the QRNG.",
                   UintegerValue (10000),
                   MakeUintegerAccessor (&QKDRandomGenerator::m_maxReq),
                   MakeUintegerChecker<uint32_t> (1));
  return tid;
}*/

QKDRandomGenerator::QKDRandomGenerator(){
  //NS_LOG_FUNCTION (this);
    randomgenerator = CreateObject<UniformRandomVariable> ();
}

QKDRandomGenerator::~QKDRandomGenerator(){
  //NS_LOG_FUNCTION (this);
    //Dispose();
}

void 
QKDRandomGenerator::Dispose(){
  //NS_LOG_FUNCTION (this);
    randomgenerator->Dispose();
}

std::vector<uint8_t> 
QKDRandomGenerator::generateStream(uint32_t total){
    std::vector<uint8_t> keyMaterial; 
    keyMaterial.reserve(total);
    for(uint32_t i = 0; i < total; i++){
        keyMaterial.push_back(int(randomgenerator->GetValue(0,256)));
    }
    return keyMaterial;
}

}