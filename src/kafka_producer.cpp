#include "kafka_producer.hpp"
#include <iostream>

KafkaProducer::KafkaProducer(const std::string &brokers, const std::string &topic)
{
    std::string errstr;

    RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    conf->set("bootstrap.servers", brokers, errstr);
    conf->set("queue.buffering.max.kbytes", "512000", errstr);
    conf->set("queue.buffering.max.messages", "100000", errstr);
    conf->set("compression.type", "snappy", errstr);

    producer = RdKafka::Producer::create(conf, errstr);
    if (!producer)
    {
        std::cerr << "Failed to create producer: " << errstr << std::endl;
        exit(1);
    }

    topic_obj = RdKafka::Topic::create(producer, topic, nullptr, errstr);

    delete conf;
}

void KafkaProducer::send(const std::vector<unsigned char> &data)
{
    RdKafka::ErrorCode resp = producer->produce(
        topic_obj,
        RdKafka::Topic::PARTITION_UA,
        RdKafka::Producer::RK_MSG_COPY,
        (void *)data.data(),
        data.size(),
        nullptr,
        nullptr);

    if (resp != RdKafka::ERR_NO_ERROR)
    {
        std::cerr << "Produce failed: " << RdKafka::err2str(resp) << std::endl;
    }

    producer->poll(0);
}

void KafkaProducer::flush()
{
    producer->flush(5000);
}
