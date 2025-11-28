#include "camera_config_manager.hpp"
#include "camera_worker_manager.hpp"

int main() {
    CameraConfigManager configMgr;
    if (!configMgr.loadCameraConfig("config/cameras.json")) {
        std::cerr << "Failed to load camera config\n";
        return 1;
    }

    CameraWorkerManager workerMgr;
    workerMgr.startMultiCameraWorkers(configMgr.getCameraList());

    std::this_thread::sleep_for(std::chrono::minutes(1));

    workerMgr.shutdownAll();
    return 0;
}
