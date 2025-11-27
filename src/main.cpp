#include <iostream>
#include <thread>
#include <chrono>
#include "camera_module.hpp"
#include "kafka_producer.hpp"

using namespace std::chrono_literals;

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: ./app <camera_url>" << std::endl;
        return -1;
    }

    std::string camUrl = argv[1];

    CameraModule camera(camUrl);
    KafkaProducer producer("localhost:9092", "cctv-frames");

    std::vector<unsigned char> jpegBuffer;

    std::cout << "Streaming and sending to Kafka..." << std::endl;

    while (true)
    {
        if (!camera.getFrame(jpegBuffer))
        {
            std::cerr << "Retrying camera..." << std::endl;
            std::this_thread::sleep_for(1s);
            continue;
        }

        producer.send(jpegBuffer);
    }

    producer.flush();
    return 0;
}
