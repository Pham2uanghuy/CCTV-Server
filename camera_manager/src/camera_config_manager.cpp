#include <camera_config_manager.hpp>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

bool CameraConfigManager::loadCameraConfig(const std::string &filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
        return false;

    json j;
    file >> j;
    cameras.clear();

    for (auto &item : j) {
        CameraConfig cfg;
        cfg.id = item.value("id", "");
        cfg.rtspUrl = item.value("rtspUrl", "");
        cfg.username = item.value("username", "");
        cfg.password = item.value("password", "");
        cameras.push_back(cfg);
    }

    return validateCameraConfig();
}


bool CameraConfigManager::validateCameraConfig() {
    for (auto &cfg: camera) {
        if (cfg.id.empty() || cfg.rtsUrl.empty()) return false;
    }
    return true;
}

void CameraConfigManager::reloadCameraList() {

}

std::vector<CameraConfigManager::CameraConfig> CameraConfigManager::getCameraList() {
    return cameras;
}

