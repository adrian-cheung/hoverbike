#include "Paths.h"

std::string Paths::Asset(const std::string& subPathFromAssetsDir) {
    return assetRoot + subPathFromAssetsDir;
}

std::string Paths::Image(const std::string& subPathFromImagesDir) {
    return assetRoot + "images/"s + subPathFromImagesDir + ".png";
}

std::string Paths::Map(const std::string& subPathFromMapsDir) {
    return assetRoot + "maps/"s + subPathFromMapsDir + ".txt";
}
