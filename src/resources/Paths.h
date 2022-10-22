#ifndef HOVERGAME_PATHS_H
#define HOVERGAME_PATHS_H

#include "Includes.h"

class Paths {
public:
    static std::string Asset(const std::string& subPathFromAssetsDir);
    static std::string Image(const std::string& subPathFromImagesDir);

private:
    static constexpr const char* assetRoot = "../assets/";
};


#endif //HOVERGAME_PATHS_H
