/** TODO This file is part of dataheap2 (https://github.com/tud-zih-energy/dataheap2)
* TOD dataheap2 - A wrapper for the Open Trace Format 2 library
*
* Copyright (c) 2018, Technische Universität Dresden, Germany
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* * Redistributions of source code must retain the above copyright notice, this
*   list of conditions and the following disclaimer.
*
* * Redistributions in binary form must reproduce the above copyright notice,
*   this list of conditions and the following disclaimer in the documentation
*   and/or other materials provided with the distribution.
*
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**/

#include <amqpcpp.h>
#include <ev.h>
#include <time.h>
#include <iomanip>
#include <memory>
#include <string>

#include <json.hpp>

#include "core/core.h"

namespace dataheap2 {
namespace core {

RabbitMqCore::RabbitMqCore(struct ev_loop *loop) : handler(loop) {}

void RabbitMqCore::connectToServer(const std::string &server_address) {
  rpc_connection = std::make_unique<AMQP::TcpConnection>(
      &handler, AMQP::Address(server_address));
  rpc_channel = std::make_unique<AMQP::TcpChannel>(rpc_connection.get());
  rpc_channel->onError([](const char *message) {
    // report error
    std::cout << "channel error: " << message << std::endl;
  });

  rpc_channel
      ->declareQueue(AMQP::exclusive) //  rpc queue
      .onSuccess(
          [this](const std::string &name, int msgcount, int consumercount) {
            callback_queue = name;

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

              std::cout << "message received: " << message.body() << std::endl;

              handleRpcRespone(message);

              // acknowledge the message
              rpc_channel->ack(deliveryTag);
            };

            rpc_channel->consume(name)
                .onReceived(messageCb)
                .onSuccess(startCb)
                .onError(errorCb);

            // request initial config
            rpcGetConfig();
          });
}

void RabbitMqCore::rpcGetConfig() {
  nlohmann::json json_request = {{"function", "getConfig"}};

  sendRpcMessage(json_request.dump());
}

void RabbitMqCore::sendRpcMessage(const std::string &message) {
  AMQP::Envelope env(message.data(), message.size());
  // env.setCorrelationID(correlation); //TODO(Franz): implement correlation ids
  env.setReplyTo(callback_queue);

  rpc_channel->publish("",                 // default direct exchange
                       RABBITMQ_RPC_QUEUE, // queue name
                       env);
}

void RabbitMqCore::handleRpcRespone(const AMQP::Message &message) {
  // TODO(Franz): handle rpc response and server commands
  const std::string responseMessage(message.body(),
                                    static_cast<size_t>(message.bodySize()));

  std::cout << "Message body: " << responseMessage << std::endl;

  try {
    auto json_reponse = nlohmann::json::parse(responseMessage);

      std::cout << "Document parsed" << std::endl;

      const std::string &functionName = json_reponse["function"];
      const nlohmann::json &responseData = json_reponse["response"];

      if (functionName == "getConfig") {
        rpcResponseGetConfig(responseData);
      } //  else if () {}
      else {
        std::cout << "error in rpc response: function " << functionName
                  << " unknown" << std::endl;
      }
  } catch (nlohmann::json::parse_error& e) {
    std::cout << "error in rpc response: parsing message: " << e.what() << std::endl;
  } catch (nlohmann::json::type_error& e) {
    std::cout << "error in rpc response: accessing parameter: " << e.what() << std::endl;
  }
}

} // namespace core
} // namespace dataheap2