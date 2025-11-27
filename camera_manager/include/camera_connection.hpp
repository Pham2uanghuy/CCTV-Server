#ifndef CAMERA_CONNECTION_HPP
#define CAMERA_CONNECTION_HPP

#include "camera_config_manager.hpp"
#include <opencv2/opencv.hpp>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

class CameraConnection
{
public:
    CameraConnection(const CameraConfigManager::CameraConfig &config);
    ~CameraConnection();

    bool connectionToCamera();
    void startCaptureThread();
    void stopCaptureThread();

private:
    void captureLoop();
    void reconnectionWhenLost();
    void exponentialBackoffReconnect();
    bool detectStreamFreeze();
    bool detectCameraOffline();

    CameraConfigManager::CameraConfig config;
    cv::VideoCapture cap;
    std::thread captureThread;
    std::atomic<bool> running;
    std::mutex mtx;
    std::condition_variable cv;
};
#endif