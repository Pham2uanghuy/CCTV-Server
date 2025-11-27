#include "camera_module.hpp"
#include <iostream>

CameraModule::CameraModule(const std::string &baseUrl)
{
    // add /video url not inclucde
    if (baseUrl.rfind("/video") == baseUrl.size() - 6)
        url = baseUrl;
    else
        url = baseUrl + "/video";

    std::cout << "Opening camera stream: " << url << std::endl;

    cap.open(url, cv::CAP_FFMPEG);
    if (!cap.isOpened())
    {
        std::cerr << "Failed to open camera stream!" << std::endl;
        exit(1);
    }
}

bool CameraModule::getFrame(std::vector<unsigned char> &jpegBuffer)
{
    cv::Mat frame;

    if (!cap.read(frame))
    {
        std::cerr << "Failed to read frame!" << std::endl;
        return false;
    }

    std::vector<int> params = {cv::IMWRITE_JPEG_QUALITY, 80};

    return cv::imencode(".jpg", frame, jpegBuffer, params);
}
