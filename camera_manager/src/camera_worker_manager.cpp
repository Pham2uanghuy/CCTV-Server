#include "camera_worker_manager.hpp"
#include <iostream>

void CameraWorkerManager::startMultiCameraWorkers(const std::vector<CameraConfigManager::CameraConfig> &configs)
{
    std::lock_guard<std::mutex> lock(mtx);
    for (auto &cfg : configs)
    {
        if (workers.find(cfg.id) == workers.end())
        {
            auto cam = std::make_shared<CameraConnection>(cfg);
            if (cam->connectionToCamera())
            {
                cam->startCaptureThread();
                workers[cfg.id] = cam;
                std::cout << "[WorkerManager] started camera" << cfg.id << "\n";
            }
            else
            {
                std::cout << "[WorkManager] failed to connect camrea" << cfg.id << "\n";
            }
        }
    }
}

void CameraWorkerManager::stopCameraWorker(const std::string &cameraId)
{
    std::lock_guard<std::mutex> lock(mtx);
    auto it = workers.find(cameraId);
    if (it != workers.end())
    {
        it->second->stopCaptureThread();
        workers.erase(it);
        std::cout << "[WorkManager] stop camera" << cameraId << "\n";
    }
}

void CameraWorkerManager::shutdownAll()
{
    std::lock_guard<std::mutex> lock(mtx);
    for (auto &[id, cam] : workers)
    {
        cam->stopCaptureThread();
    }
    workers.clear();
}