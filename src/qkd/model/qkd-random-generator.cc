

#include "qkd-random-generator.h"


namespace ns3 {

//NS_LOG_COMPONENT_DEFINE ("QKDRandomGenerator");

QKDRandomGenerator::QKDRandomGenerator(){
    m_activeQRNG = false;
    randomgenerator = CreateObject<UniformRandomVariable> ();
}

QKDRandomGenerator::QKDRandomGenerator(bool activeQRNG){
    m_activeQRNG = activeQRNG;
    randomgenerator = CreateObject<UniformRandomVariable> ();
    if(activeQRNG){//TODO comprobar si lo inicializa
        //dispositivoCuantico = idQ::Quantis(QUANTIS_DEVICE_USB,0);
    }
}

QKDRandomGenerator::~QKDRandomGenerator(){
    Dispose();
}

void 
QKDRandomGenerator::Dispose(){
    randomgenerator->Dispose();
    if(m_activeQRNG == true){
        //dispositivoCuantico.~Quantis();
    }
}

void
QKDRandomGenerator::ActivateQuantumDevice(){
    m_activeQRNG = true;
    //TODO
}

std::vector<uint8_t> 
QKDRandomGenerator::generateStream(uint32_t limite){
    std::vector<uint8_t> keyMaterial; 
    keyMaterial.reserve(limite);
    //TODO prueba
    /*if(m_activeQRNG == true){
        idQ::Quantis dispositivoCuantico(QUANTIS_DEVICE_USB,0);
        int aux = dispositivoCuantico.ReadInt(0,255);
        std::cout << "numero aleatorio generado" << aux << std::endl;
        uint8_t buffer[32];
        dispositivoCuantico.Read(&buffer,32);
        std::cout << "buffer[32]" << buffer << std::endl;

        std::cout << "Read de 16.777.217" << buffer << std::endl;
        uint8_t buffer1[16777217];
        dispositivoCuantico.Read(&buffer1,16777217);
        std::cout << "buffer1[16777217]" << buffer1 << std::endl;

        uint8_t buffer2[16777216];
        dispositivoCuantico.Read(&buffer2,16777216);
        std::cout << "buffer1[16777216]" << buffer2 << std::endl;
    }*/

    if(m_activeQRNG){
        /*std::cout << "QUANTIS ChargingApp" << std::endl;
        std::string params = "u0";
        idQ::random_device rd(params);
        uint32_t randomNumber = 0;
        for (unsigned int i = 0; i < limite/4; ++i){
            //aux <<  char(int((rd() % 255) + 1));
            randomNumber = rd();
            for(unsigned int j = 0; j < 4; j++){
                keyMaterial.push_back(randomNumber % 256);
                randomNumber = randomNumber / 256;
            }
            //newKeyMaterial.push_back(int((rd() % 256)));
        }*/
        //TODO hacer comprobacion de tamaño
        /*uint8_t buffer[limite];
        idQ::Quantis dispositivoCuantico(QUANTIS_DEVICE_USB,0);
        dispositivoCuantico.Read(&buffer,limite);
        std::vector<uint8_t> my_vector(&buffer[0], &buffer[limite]);
        keyMaterial = my_vector;*/

    }else{
        //std::cout << "RANDOM ChargingApp" << std::endl;
        for(uint32_t i = 0; i < limite; i++){
            keyMaterial.push_back(int(randomgenerator->GetValue(0,256)));
        }
        //std::cout << "FIN RANDOM ChargingApp" << std::endl;
    }
    return keyMaterial;
}

// IMPRIMIR INFORMACION DE LOS DISPOSITIVOS
static void _printCardsInfo(QuantisDeviceType deviceType)
{

  try
  {
    // Devices count
    int devicesCount = idQ::Quantis::Count(deviceType);
    std::cout << "  Found " << devicesCount << " card(s)" << std::endl;

    // Device details
    for (int i = 0; i < devicesCount; i++)
    {
      // Creates a quantis object
      idQ::Quantis quantis(deviceType, i);

      // Display device info
      std::cout << "  - Details for device #" << i << std::endl;
      int driverVersion = quantis.GetDriverVersion();
      std::cout << "      driver version: " << static_cast<int>(driverVersion / 10)
           << "." << driverVersion % 10 << std::endl;
      std::cout << "      core version: " << std::hex << quantis.GetBoardVersion() << std::endl;
      std::cout << "      serial number: " << std::hex << quantis.GetSerialNumber() << std::endl;
      std::cout << "      manufacturer: " << std::hex << quantis.GetManufacturer() << std::endl;

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
        std::cout << "      module " << j << ": " << strMask << " " << strStatus << std::endl;
      }
    }
  }
  catch (std::runtime_error &ex)
  {
    std::cerr << "Error while getting cards information: " << ex.what() << std::endl;
  }
}

// IMPRIMIR INFORMACION DE LOS DISPOSITIVOS
static void printCardsInfo()
{
  std::cout << "Displaying cards info:" << std::endl;

  std::cout << std::endl
       << "* Searching for PCI devices..." << std::endl;
  _printCardsInfo(QUANTIS_DEVICE_PCI);

  std::cout << std::endl
       << "* Searching for USB devices..." << std::endl;
  _printCardsInfo(QUANTIS_DEVICE_USB);
}

}