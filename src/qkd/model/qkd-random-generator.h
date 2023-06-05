




#ifndef QKD_RANDOM_GENERATOR_H
#define QKD_RANDOM_GENERATOR_H

#include "ns3/random-variable-stream.h"
#include <vector>
#include "Quantis.hpp"

namespace ns3 {

class QKDRandomGenerator{
    private:
        bool m_activeQRNG;
        Ptr<UniformRandomVariable> randomgenerator;
        //idQ::Quantis dispositivoCuantico;

    public:
        QKDRandomGenerator();
        QKDRandomGenerator(bool activeQRNG);
        ~QKDRandomGenerator();
        void Dispose();
        void ActivateQuantumDevice();
        std::vector<uint8_t> generateStream(uint32_t limite);

};

}

#endif /* QKD_RANDOM_GENERATOR_H */