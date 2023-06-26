

#include "qkd-random-generator.h"


namespace ns3 {

//NS_LOG_COMPONENT_DEFINE ("QKDRandomGenerator");

QKDRandomGenerator::QKDRandomGenerator(){
    m_activeQRNG = false;
    buffer_size = 100000;
    randomgenerator = CreateObject<UniformRandomVariable> ();
}

QKDRandomGenerator::QKDRandomGenerator(bool activeQRNG){
    m_activeQRNG = activeQRNG;
    buffer_size = 100000;
    randomgenerator = CreateObject<UniformRandomVariable> ();
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
}

std::vector<uint8_t> 
QKDRandomGenerator::generateStream(uint32_t limite){
    std::vector<uint8_t> keyMaterial; 
    keyMaterial.reserve(limite);
    if(m_activeQRNG){
      if(IS_DEMO){
        std::cout << "Comenzamos a pedir material, tamaño total: " << limite << std::endl << std::endl;
      }
      idQ::Quantis dispositivoCuantico(QUANTIS_DEVICE_USB,0);
      if(limite < 10000){
        if(IS_DEMO){
          std::cout << "Llamada única" << std::endl;
        }
        uint8_t buffer[limite];
        dispositivoCuantico.Read(&buffer,limite);
        std::vector<uint8_t> my_vector(&buffer[0], &buffer[limite]);
        keyMaterial = my_vector;
        //std::cout << "vector: [" << keyMaterial.data() << "]";
      }else{
        uint32_t tamanho = buffer_size;
        uint32_t max = MAX_BUFFER_SIZE;
        if(tamanho > max){
          tamanho = max;
        }
        if(IS_DEMO){
          std::cout << "Llamadas de " << tamanho << " en " << tamanho << std::endl;
          std::cout << "---------------------------------------" << std::endl;
        }
        uint8_t buffer[tamanho];
        uint32_t recorrido = limite/tamanho;
        for(uint32_t i = 0; i < recorrido; i++){
          if(IS_DEMO){
            std::cout << "iteraccion numero " << i << " de " << recorrido << "." << std::endl;
          }
          dispositivoCuantico.Read(&buffer,tamanho);
          if(IS_DEMO){
            std::cout << "dispositivo leido con exito" << std::endl;
          }
          for(uint32_t j = 0; j < tamanho; j++){
            keyMaterial.push_back(buffer[j]);
          }
        }
        if(tamanho * recorrido != limite){
          if(IS_DEMO){
            std::cout << "iteraccion extra para completar el tamaño de la clave pedido" << std::endl;
            std::cout << "---------------------------------------" << std::endl;
          }
          uint32_t ultimoNumero = limite - ((limite / tamanho) * tamanho);
          uint8_t buffer[ultimoNumero];
          dispositivoCuantico.Read(&buffer,ultimoNumero);
          for(uint32_t j = 0; j < ultimoNumero; j++){
            keyMaterial.push_back(buffer[j]);
          }
        }
      }  
    }else{
        for(uint32_t i = 0; i < limite; i++){
            keyMaterial.push_back(int(randomgenerator->GetValue(0,256)));
        }
    }
    return keyMaterial;
}

// IMPRIMIR INFORMACION DE LOS DISPOSITIVOS
void QKDRandomGenerator::_printCardsInfo(QuantisDeviceType deviceType){

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
void QKDRandomGenerator::printCardsInfo(){
  std::cout << "Displaying cards info:" << std::endl;

  std::cout << std::endl
       << "* Searching for PCI devices..." << std::endl;
  _printCardsInfo(QUANTIS_DEVICE_PCI);

  std::cout << std::endl
       << "* Searching for USB devices..." << std::endl;
  _printCardsInfo(QUANTIS_DEVICE_USB);
  std::cout << std::dec;
}

}