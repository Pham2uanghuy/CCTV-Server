#include "kafka_producer.hpp"
#include <iostream>
#include <vector>
#include <string>

int main()
{
    // Kafka broker và topic
    std::string brokers = "localhost:9092"; // đổi theo Kafka của bạn
    std::string topic = "test";

    try
    {
        KafkaProducer producer(brokers, topic);

        std::string message = "Hello Kafka!";
        std::vector<unsigned char> data(message.begin(), message.end());

        producer.send(data);
        producer.flush();

        std::cout << "Message sent successfully!" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
