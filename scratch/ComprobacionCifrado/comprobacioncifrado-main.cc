#include "ns3/core-module.h" 

#include "ns3/qkd-crypto.h"
#include "ns3/qkd-key.h"
#include "ns3/qkd-manager.h"

#include <crypto++/base64.h>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("QKD_CHANNEL_TEST");

void OTPcipher()
{
    Ptr<QKDCrypto> crypto = CreateObject<QKDCrypto>();
    crypto->SetAttribute("EncryptionEnabled",BooleanValue(true));

    std::string msg = "holamundo";

    std::cout << "mensaje: " << msg << std::endl;

    std::string key = "abcdefghi";

    Ptr<QKDKey> quantumKey = CreateObject<QKDKey>(1,key);


    std::string C = crypto->OTP(msg,quantumKey);

    CryptoPP::Base64Encoder encoder;
    
    std::stringstream ss;
    for(unsigned int i = 0; i < C.size(); i++){
        ss << char(encoder.Put(C[i]));
    }

    //mensaje cifrado en base64
    byte decoded[C.size()];
    std::memcpy(decoded, C.data(), C.length());

    std::string encoded;

    CryptoPP::Base64Encoder encoder2;
    encoder2.Put(decoded, sizeof(decoded));
    encoder2.MessageEnd();

    CryptoPP::word64 size = encoder2.MaxRetrievable();
    if(size)
    {
        encoded.resize(size);		
        encoder2.Get((byte*)&encoded[0], encoded.size());
    }

    std::cout << "mensaje cifrado: " << encoded << std::endl;

    std::cout << "mensaje cifrado en ASCII: ";

    for(unsigned int i = 0; i < C.size(); i++){
        std::cout << int(C[i]) << "-";
    }

    std::cout << std::endl;

    std::string D = crypto->OTP(C,quantumKey);

    std::cout << "mensaje descifrado: " << D << std::endl;

    crypto->Dispose();
}

void AEScipher()
{

    std::cout << "--------------- AES ---------------" << std::endl;
    Ptr<QKDCrypto> crypto = CreateObject<QKDCrypto>();
    crypto->SetAttribute("EncryptionEnabled",BooleanValue(true));

    std::string msg = "holamundo";
    std::cout << "mensaje: " << msg << std::endl;
    
    std::string key = "aaaaaaaaaaaaaaaa";
    Ptr<QKDKey> quantumKey = CreateObject<QKDKey>(1,key);
    std::string C = crypto->AESEncrypt(msg,quantumKey);

    //mensaje cifrado en base64
    byte decoded[C.size()];
    std::memcpy(decoded, C.data(), C.length());

    std::string encoded;

    CryptoPP::Base64Encoder encoder2;
    encoder2.Put(decoded, sizeof(decoded));
    encoder2.MessageEnd();

    CryptoPP::word64 size = encoder2.MaxRetrievable();
    if(size)
    {
        encoded.resize(size);		
        encoder2.Get((byte*)&encoded[0], encoded.size());
    }

    std::cout << "mensaje cifrado: " << encoded << std::endl;
    //std::cout << "mensaje cifrado en plano: " << C << std::endl;
    std::cout << "mensaje cifrado en ASCII: ";

    for(unsigned int i = 0; i < C.size(); i++){
        std::cout << int(C[i]) << "-";
    }
    std::cout << std::endl;

    std::string D = crypto->AESDecrypt(C,quantumKey);
    std::cout << "mensaje descifrado: " << D << std::endl;

    crypto->Dispose();
}

int main (int argc, char *argv[])
{
    OTPcipher();

    AEScipher();

}