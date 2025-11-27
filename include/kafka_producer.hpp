#ifndef KAFKA_PRODUCER_HPP
#define KAFKA_PRODUCER_HPP

#include <librdkafka/rdkafkacpp.h>
#include <string>
#include <vector>

class KafkaProducer
{
public:
    KafkaProducer(const std::string &brokers, const std::string &topic);
    void send(const std::vector<unsigned char> &data);
    void flush();

private:
    RdKafka::Producer *producer;
    RdKafka::Topic *topic_obj;
};

#endif
