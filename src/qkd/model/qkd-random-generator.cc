

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

NS_LOG_COMPONENT_DEFINE ("QKDRandomGenerator");
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
}

QKDRandomGenerator::QKDRandomGenerator(){
  NS_LOG_FUNCTION (this);
    m_activeQRNG = false;
    randomgenerator = CreateObject<UniformRandomVariable> ();
}

QKDRandomGenerator::QKDRandomGenerator(bool activeQRNG){
  NS_LOG_FUNCTION (this);
    m_activeQRNG = activeQRNG;
    randomgenerator = CreateObject<UniformRandomVariable> ();
}

QKDRandomGenerator::~QKDRandomGenerator(){
  NS_LOG_FUNCTION (this);
    Dispose();
}

void 
QKDRandomGenerator::Dispose(){
    randomgenerator->Dispose();
    if(m_activeQRNG == true){
      //qrngDevice.~Quantis();
    }
}

void
QKDRandomGenerator::ActivateQuantumDevice(){
    m_activeQRNG = true;
}

std::vector<uint8_t> 
QKDRandomGenerator::generateStream(uint32_t total){
    std::vector<uint8_t> keyMaterial; 
    keyMaterial.reserve(total);
    if(m_activeQRNG){
      NS_LOG_FUNCTION (this << "Requesting " << total << " bytes: ");
      idQ::Quantis qrngDevice(QUANTIS_DEVICE_USB,0);
      if(total < m_maxReq){
        NS_LOG_FUNCTION (this << "Single call");
        uint8_t buffer[total];
        qrngDevice.Read(&buffer,total);
        std::vector<uint8_t> my_vector(&buffer[0], &buffer[total]);
        keyMaterial = my_vector;
        //NS_LOG_FUNCTION (this << "vector: [" << keyMaterial.data() << "]";
      }else{

        uint32_t size = m_maxReq;
        uint32_t max = MAX_BUFFER_SIZE;
        if(size > max){
          size = max;
        }

        uint8_t buffer[size];
        uint32_t numberOfCalls = total/size;
                  
        NS_LOG_FUNCTION (this << numberOfCalls << " requests of " << size << " bytes.");
          //NS_LOG_DEBUG (this << "---------------------------------------");
        for(uint32_t i = 0; i < numberOfCalls; i++){
          NS_LOG_FUNCTION (this << "Iteration no. " << i << " of " << numberOfCalls << ".");
          qrngDevice.Read(&buffer,size);
          NS_LOG_DEBUG (this << "Device read successfully.");
          for(uint32_t j = 0; j < size; j++){
            keyMaterial.push_back(buffer[j]);
          }
        }
        if(size * numberOfCalls != total){
          uint32_t remaining = total - ((total / size) * size);
          NS_LOG_FUNCTION (this << "Last request of " << remaining << " bytes");

          uint8_t buffer[remaining];
          qrngDevice.Read(&buffer,remaining);
          for(uint32_t j = 0; j < remaining; j++){
            keyMaterial.push_back(buffer[j]);
          }
        }
      }  
    }else{
        for(uint32_t i = 0; i < total; i++){
            keyMaterial.push_back(int(randomgenerator->GetValue(0,256)));
        }
    }
    return keyMaterial;
}

void QKDRandomGenerator::_printCardsInfo(QuantisDeviceType deviceType){

  try
  {
    // Devices count
    int devicesCount = idQ::Quantis::Count(deviceType);
    NS_LOG_FUNCTION (this << "  Found " << devicesCount << " card(s)");

    // Device details
    for (int i = 0; i < devicesCount; i++)
    {
      // Creates a quantis object
      idQ::Quantis quantis(deviceType, i);

      // Display device info
      NS_LOG_FUNCTION (this << "  - Details for device #" << i);
      int driverVersion = quantis.GetDriverVersion();
      NS_LOG_FUNCTION (this << "      driver version: " << static_cast<int>(driverVersion / 10)
           << "." << driverVersion % 10);
      NS_LOG_FUNCTION (this << "      core version: " << quantis.GetBoardVersion());
      NS_LOG_FUNCTION (this << "      serial number: " << quantis.GetSerialNumber());
      NS_LOG_FUNCTION (this << "      manufacturer: " << quantis.GetManufacturer());

      // Display device's modules info
      for (int j = 0; j < 4; j++)
      {
        std::string strMask = "not found";
        std::string strStatus = "";
        if (quantis.GetModulesMask() & (1 << j))
        {
          strMask = "found";
          if (quantis.GetModulesStatus() & (1 << j))
          {
            strStatus = "(enabled)";
          }
          else
          {
            strStatus = "(disabled)";
          }
        }
        NS_LOG_FUNCTION (this << "      module " << j << ": " << strMask << " " << strStatus);
      }
    }
  }
  catch (std::runtime_error &ex)
  {
    std::cerr << "Error while getting cards information: " << ex.what() << std::endl;
  }
}

void QKDRandomGenerator::printCardsInfo(){
  NS_LOG_FUNCTION (this << "Displaying cards info:");

  NS_LOG_FUNCTION (this 
       << "* Searching for PCI devices...");
  _printCardsInfo(QUANTIS_DEVICE_PCI);

  NS_LOG_FUNCTION (this 
       << "* Searching for USB devices...");
  _printCardsInfo(QUANTIS_DEVICE_USB);
  std::cout << std::dec;
}

}