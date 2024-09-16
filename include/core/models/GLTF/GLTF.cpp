#include "./GLTF.h"
#include "models/GLTF/GltfObjects.h"
#include "utils/Base64.h"
#include "utils/RapidJsonUtils.h"
#include <SDL_log.h>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>
#include <ostream>
#include <rapidjson/document.h>
#include <rapidjson/reader.h>
#include <sstream>

namespace Models {
GLTF::GLTF() {}
GLTF::~GLTF() {}
void GLTF::Load(std::string &path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "not found file " << path << std::endl;
        return;
    }

    std::stringstream fileStream;
    fileStream << file.rdbuf();
    std::string contents = fileStream.str();
    rapidjson::StringStream jsonStr(contents.c_str());
    rapidjson::Document doc;
    doc.ParseStream(jsonStr);

    if (!doc.IsObject()) {
        std::cout << "not valid json " << std::endl;
        return;
    }
    ParseBuffers(std::move(doc));

    auto buffer = m_buffers[0];
    auto values = buffer->Values<std::float_t>(8);
    std::cout << values[0] << std::endl;
    // bufferObj.Test();

    // auto scenes = doc["scenes"].GetArray();
    // for (auto &scene : scenes) {
    //     for (auto &nodes : scene.GetObject()) {
    //         std::cout << nodes.name.GetString() << std::endl;
    //         for (auto &node : nodes.value.GetArray()) {
    //             std::cout << node.GetInt() << std::endl;
    //         }
    //     }
    // }
    // std::cout < < < < std::endl;
}

void GLTF::ParseBuffers(rapidjson::Document &&doc) {
    auto buffers = doc["buffers"].GetArray();
    CoreUtils::Base64 base64{};

    for (auto &buffer : buffers) {
        auto bufferObj = std::make_shared<GltfObject::Buffer>();
        for (auto &nodes : buffer.GetObject()) {
            std::string nameField = nodes.name.GetString();
            if (nameField.compare("uri") == 0) {
                std::string value = nodes.value.GetString();
                if (value.find(',') != std::string::npos) {
                    std::string strBuffer = value.substr(value.find(',') + 1, value.length());
                    auto tmpBuffer = base64.decode(strBuffer);
                    bufferObj->SetBuffer(tmpBuffer);
                }
            }
            if (nameField.compare("byteLength") == 0) {
                auto value = nodes.value.GetUint64();
                bufferObj->SetLength(value);
            }
        }
        m_buffers.push_back(bufferObj);
    }
}
} // namespace Models