#include <amqpcpp.h>

#include "sink/sink.h"

namespace dataheap2 {
namespace sink {

RabbitMqDatasink::RabbitMqDatasink(struct ev_loop *loop) : RabbitMqCore(loop) {}

void RabbitMqDatasink::rpcResponseGetConfig(const nlohmann::json &config) {
  std::cout << "Start parsing config" << std::endl;

  const std::string &data_server_address = config["dataServerAddress"];
  data_queue = config["dataQueue"];

  data_connection = std::make_unique<AMQP::TcpConnection>(
      &handler, AMQP::Address(data_server_address));
  data_channel = std::make_unique<AMQP::TcpChannel>(data_connection.get());
  data_channel->onError([](const char *message) {
    // report error
    std::cout << "data channel error: " << message << std::endl;
  });

  data_channel
      ->declareQueue(data_queue) //  rpc queue
      .onSuccess(
          [this](const std::string &name, int msgcount, int consumercount) {
            // callback function that is called when the consume operation
            // starts
            auto startCb = [](const std::string &consumertag) {
              std::cout << "consume operation started" << std::endl;
            };

            // callback function that is called when the consume operation
            // failed
            auto errorCb = [](const char *message) {
              std::cout << "consume operation failed" << std::endl;
            };

            // callback operation when a message was received
            auto messageCb = [this](const AMQP::Message &message,
                                    uint64_t deliveryTag, bool redelivered) {
              handleAmqpData(message);

              // acknowledge the message
              data_channel->ack(deliveryTag);
            };

            data_channel->consume(name)
                .onReceived(messageCb)
                .onSuccess(startCb)
                .onError(errorCb);
          });

  if (config.find("sourceConfig") != config.end()) {
    loadSinkConfig(config["sourceConfig"]);
  } else {
    loadSinkConfig(nlohmann::json());
  }
}

void RabbitMqDatasink::handleAmqpData(const AMQP::Message &message) {
  if(message.bodySize() < 2) {
    return;
  }
  
  const auto &metric_name = message.routingkey();
  auto message_string = std::string(message.body(), message.bodySize());
  auto message_coding = message_string.front();

  message_string.erase(0, 1);

  switch (message_coding) {
    case MESSAGE_CODING_SINGLE:
    {
      DataPoint datapoint;
      datapoint.ParseFromString(message_string);
      handleIncomingDatapoint(metric_name, datapoint);
      break;
    }
    case MESSAGE_CODING_CHUNK:
    {
      DataChunk datachunk;
      datachunk.ParseFromString(message_string);
      handleIncomingDatachunk(metric_name, datachunk);
      break;
    }
  }

}
} // namespace sink
} // namespace dataheap2
