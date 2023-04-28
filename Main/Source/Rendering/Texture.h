#pragma once

class Texture {
    std::wstring name;
    uint32_t textureID;
    uint32_t width;
    uint32_t height;
    uint32_t channels;
    uint32_t internalFormat;
    uint32_t dataFormat;
};