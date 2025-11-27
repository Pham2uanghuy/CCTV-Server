#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <camera_url>" << std::endl;
        return -1;
    }

    std::string url = argv[1];
    std::cout << "Opening camera stream: " << url << std::endl;

    cv::VideoCapture cap;

    // detect protocol
    if (url.find("rtsp://") == 0)
    {
        std::cout << "Detected RTSP stream" << std::endl;
        cap.open(url, cv::CAP_FFMPEG); // FFMPEG backend for RTSP
    }
    else if (url.find("http://") == 0 || url.find("https://") == 0)
    {
        std::cout << "Detected HTTP MJPEG stream" << std::endl;
        cap.open(url);
    }
    else
    {
        std::cerr << "Unsupported URL protocol" << std::endl;
        return -1;
    }

    if (!cap.isOpened())
    {
        std::cerr << "Failed to open camera stream" << std::endl;
        return -1;
    }

    cv::Mat frame;
    while (true)
    {
        if (!cap.read(frame))
        {
            std::cerr << "Failed to read frame, retrying..." << std::endl;
            cv::waitKey(500);
            continue;
        }

        cv::imshow("Camera Viewer", frame);

        char c = (char)cv::waitKey(1);
        if (c == 'q' || c == 27)
            break; // q hoặc ESC để thoát
    }

    cap.release();
    cv::destroyAllWindows();

    return 0;
}
