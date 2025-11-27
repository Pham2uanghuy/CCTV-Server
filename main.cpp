#include <iostream>
#include <opencv2/opencv.hpp>
#include <librdkafka/rdkafkacpp.h>
#include <chrono>
#include <thread>

using namespace std;
using namespace cv;

class KafkaProducer
{
public:
    KafkaProducer(const string &brokers, const string &topic)
    {
        string errstr;

        // Config
        RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
        conf->set("bootstrap.servers", brokers, errstr);
        conf->set("queue.buffering.max.kbytes", "512000", errstr);
        conf->set("queue.buffering.max.messages", "100000", errstr);
        conf->set("compression.type", "snappy", errstr);

        producer = RdKafka::Producer::create(conf, errstr);
        if (!producer)
        {
            cerr << "Failed to create producer: " << errstr << endl;
            exit(1);
        }

        this->topic_str = topic;
        topic_obj = RdKafka::Topic::create(producer, topic, nullptr, errstr);

        delete conf;
    }

    void send(const vector<uchar> &data)
    {
        RdKafka::ErrorCode resp = producer->produce(
            topic_obj,
            RdKafka::Topic::PARTITION_UA,
            RdKafka::Producer::RK_MSG_COPY,
            (void *)data.data(),
            data.size(),
            nullptr, // key
            nullptr  // timestamp
        );

        if (resp != RdKafka::ERR_NO_ERROR)
        {
            cerr << "Produce failed: " << RdKafka::err2str(resp) << endl;
        }

        producer->poll(0);
    }

    void flush()
    {
        producer->flush(5000);
    }

private:
    RdKafka::Producer *producer;
    RdKafka::Topic *topic_obj;
    string topic_str;
};

int main()
{
    string rtsp_url = "rtsp://192.168.151.18:8554/stream";
    string brokers = "localhost:9092";
    string topic = "cctv-frames";

    KafkaProducer kafka(brokers, topic);

    VideoCapture cap(rtsp_url);
    if (!cap.isOpened())
    {
        cerr << "Cannot open RTSP stream" << endl;
        return -1;
    }

    cout << "RTSP opened. Streaming and sending to Kafka..." << endl;

    Mat frame;
    vector<uchar> buf;

    while (true)
    {
        if (!cap.read(frame))
        {
            cerr << "Failed to read frame, retrying..." << endl;
            this_thread::sleep_for(1s);
            continue;
        }

        // Optional resize để giảm kích thước
        // resize(frame, frame, Size(640, 360));

        // Encode JPEG
        vector<int> params = {IMWRITE_JPEG_QUALITY, 80};
        imencode(".jpg", frame, buf, params);

        // Send to Kafka
        kafka.send(buf);

        // Throttle (optional)
        // this_thread::sleep_for(20ms);
    }

    kafka.flush();
    return 0;
}
