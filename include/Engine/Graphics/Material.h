#pragma once
#include <map>
#include <string>
#include <windef.h>

// Materials within the material file
struct Material {
    char map_Kd[MAX_PATH];
};

// Manages .mtl files
class MaterialFile {
    public:
        MaterialFile(const char *mtlPath);
        std::map<std::string, Material *> m_materials;
};