#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include "../include/camera_module.hpp"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: ./test_camera <camera_url>" << std::endl;
        return -1;
    }

    std::string url = argv[1];
    CameraModule cam(url);

    std::vector<unsigned char> jpegBuffer;

    std::cout << "Testing camera module..." << std::endl;

    while (true)
    {
        if (!cam.getFrame(jpegBuffer))
        {
            std::cerr << "Failed to read frame!" << std::endl;
            continue;
        }

        // Giải mã lại JPEG để hiển thị
        cv::Mat frame = cv::imdecode(jpegBuffer, cv::IMREAD_COLOR);
        if (frame.empty())
        {
            std::cerr << "Decode failed!" << std::endl;
            continue;
        }

        cv::rotate(frame, frame, cv::ROTATE_90_CLOCKWISE);

        cv::imshow("Camera Test", frame);

        char c = (char)cv::waitKey(1);
        if (c == 'q' || c == 27)
            break;
    }

    return 0;
}
