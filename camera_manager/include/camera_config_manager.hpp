#ifndef CAMERA_CONFIG_MANAGER_HPP
#define CAMERA_CONFIG_MANAGER_HPP

#include <string>
#include <vector>

class CameraConfigManager
{
public:
    struct CameraConfig
    {
        std::string id;
        std::string rtspUrl;
        std::string username;
        std::string password;
    };

    bool loadCameraConfig(const std::string &filePath);
    bool validateCameraConfig();
    void reloadCameraList();
    std::vector<CameraConfig> getCameraList();

private:
    std::vector<CameraConfig> cameras;
};

#endif