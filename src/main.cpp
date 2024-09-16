
#include "models/GLTF/GLTF.h"
#include "utils/Base64.h"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>

struct Test {
    Test() {}
    ~Test() { std::cout << "destroy" << std::endl; }
};

int main() {
    // auto gameManager = new Managers::GameManager();
    // if (gameManager->Init()) {
    //     gameManager->Run();
    // }
    CoreUtils::Base64 b{};
    std::string s = "AAABAAIAAAAAAAAAAAAAAAAAAAAAAIA/AAAAAAAAAAAAAAAAAACAPwAAAAA=";
    // // auto c = reinterpret_cast<const unsigned char *>(s.c_str());
    // // std::cout << c << std::endl;
    // {
    //     auto t = std::make_unique<Test *>();
    //     // delete t;
    // }
    auto t = b.decode(s);

    // auto arr = t.c_str();
    // std::cout << &arr << std::endl;
    // // // auto Temp = t.c_str();
    // auto t1 = (uint16_t *)(t.c_str());
    // std::cout << &t1 << std::endl;
    // std::cout << t1[1] << std::endl;
    // std::cout << " " << std::endl;
    Models::GLTF gltf{};
    std::string path = "./assets/simple.gltf";
    gltf.Load(path);
    // std::cout << static_cast<unsigned short *>(arr)[1] << std::endl;
    // unsigned short val = *(Temp + 1) << 8 | *(Temp + 2);
    // std::cout << val << std::endl;
    // unsigned short *arr2 = reinterpret_cast<unsigned short>((unsigned short *)arr);

    // std::cout <<  << std::endl;

    // auto t1 = b.decode(t);
    // auto r1 = base64::from_base64(s);
    // auto arr = r1.c_str();
    // std::cout << arr << std::endl;
    // std::string text = "Hello cplusplus forum!";

    // std::ofstream outfile("binary.txt", std::ofstream::binary);
    // unsigned int size = text.size();                              // get the size
    // outfile.write(reinterpret_cast<char *>(&size), sizeof(size)); // write it to the file
    // outfile.write(text.c_str(), text.size());                     // write the actual text
    // outfile.close();
    return 0;
}