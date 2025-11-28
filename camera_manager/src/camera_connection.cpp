#include "camera_connection.hpp"
#include <chrono>
#include <iostream>

CameraConnection::CameraConnection(const CameraConfigManager::CameraConfig &cfg)
    : config(cfg), running(false) {}

CameraConnection::~CameraConnection()
{
    stopCaptureThread();
    if (cap.isOpened())
        cap.release();
}

bool CameraConnection::connectionToCamera()
{
    cap.open(config.rtspUrl);
    return cap.isOpened();
}

void CameraConnection::startCaptureThread()
{
    running = true;
    captureThread = std::thread(&CameraConnection::captureLoop, this);
}

void CameraConnection::stopCaptureThread()
{
    running = false;
    cv.notify_all();
    if (captureThread.joinable())
        captureThread.join();
}

void CameraConnection::captureLoop()
{
    cv::Mat frame;
    while (running)
    {
        if (!cap.isOpened())
        {
            reconnectionWhenLost();
            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;
        }

        if (!cap.read(frame))
        {
            std::cout << "[Camera " << config.id << "] Failed to read frame, reconnecting...\n";
            reconnectionWhenLost();
            continue;
        }

        // handling fram here
        std::this_thread::sleep_for(std::chrono::milliseconds(30)); // ~30fps
    }
}

void CameraConnection::reconnectionWhenLost()
{
    cap.release();
    exponentialBackoffReconnect();
}

void CameraConnection::exponentialBackoffReconnect()
{
    int waitSeconds = 1;
    while (running)
    {
        std::cout << "[Camera " << config.id << "] Attempting reconnect...\n";
        if (cap.open(config.rtspUrl))
        {
            std::cout << "[Camera " << config.id << "] Reconnected.\n";
            return;
        }
        std::this_thread::sleep_for(std::chrono::seconds(waitSeconds));
        waitSeconds = std::min(waitSeconds * 2, 32);
    }
}

bool CameraConnection::detectStreamFreeze()
{
    // use histogram or frame difference to detect freeze
    return false;
}

bool CameraConnection::detectCameraOffline()
{
    return !cap.isOpened();
}
