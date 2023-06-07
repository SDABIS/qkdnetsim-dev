




#ifndef QKD_RANDOM_GENERATOR_H
#define QKD_RANDOM_GENERATOR_H

#include "ns3/random-variable-stream.h"
#include <vector>
#include "Quantis.hpp"

#define IS_DEMO true
#define MAX_BUFFER_SIZE 16*1024*1024

namespace ns3 {

class QKDRandomGenerator{
    private:
        void _printCardsInfo(QuantisDeviceType deviceType);
        bool m_activeQRNG;
        Ptr<UniformRandomVariable> randomgenerator;
        uint32_t buffer_size;
        //idQ::Quantis *dispositivoCuantico;

    public:
        QKDRandomGenerator();
        QKDRandomGenerator(bool activeQRNG);
        ~QKDRandomGenerator();
        void Dispose();
        void printCardsInfo();
        void ActivateQuantumDevice();
        std::vector<uint8_t> generateStream(uint32_t limite);

};

}

#endif /* QKD_RANDOM_GENERATOR_H */