#include "Engine/Engine.h"
#include <stdio.h>
#include <string.h>

MaterialFile::MaterialFile(const char *mtlPath) {

    // Parsing .mtl file:
    FILE *file = fopen(mtlPath, "r");
    Material *current = nullptr;
    while (true) {
        char lineHeader[256];
        int result = fscanf(file, "%s", lineHeader);
        if (result == EOF) break;

        // New Material
        if (strcmp(lineHeader, "newmtl") == 0) {
            char name[MAX_PATH] = {};
            fscanf(file, "%s\n", name);
            current = new Material();
            m_materials[name] = current;
        }

        // map_Kd
        else if (strcmp(lineHeader, "map_Kd") == 0) {
            fscanf(file, "%s\n", current->map_Kd);
        }
    }
    fclose(file);

}