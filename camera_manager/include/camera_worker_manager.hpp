#ifndef CAMERA_WORKER_MANAGER_HPP
#define CAMERA_WORKER_MANAGER_HPP

#include "camera_connection.hpp"
#include "map"
#include <memory>
#include <mutex>
#include <vector>

class CameraWorkerManager
{
public:
    void startMultiCameraWorkers(const std::vector<CameraConfigManager::CameraConfig> &configs);
    void stopCameraWorker(const std::string &cameraId);
    void shutdownAll();

private:
    std::map<std::string, std::shared_ptr<CameraConnection>> workers;
    std::mutex mtx;
};

#endif