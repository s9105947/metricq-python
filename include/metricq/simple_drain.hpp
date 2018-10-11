// Copyright (c) 2018, ZIH,
// Technische Universitaet Dresden,
// Federal Republic of Germany
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright notice,
//       this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright notice,
//       this list of conditions and the following disclaimer in the documentation
//       and/or other materials provided with the distribution.
//     * Neither the name of metricq nor the names of its contributors
//       may be used to endorse or promote products derived from this software
//       without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#pragma once

#include <metricq/drain.hpp>
#include <metricq/ostream.hpp>
#include <metricq/types.hpp>

#include <string>
#include <unordered_map>
#include <vector>

namespace metricq
{
class SimpleDrain : public Drain
{
public:
    SimpleDrain(const std::string& token, const std::string& queue) : Drain(token, queue)
    {
    }

    void data_callback(const std::string& id, const metricq::DataChunk& chunk) override
    {
        auto& d = data_.at(id);
        for (const auto& tv : chunk)
        {
            d.emplace_back(tv);
        }
    }

    /**
     * warning this (re)move the entire map
     */
    std::unordered_map<std::string, std::vector<TimeValue>>& get()
    {
        return data_;
    };

    /**
     * warning this (re)moves the vector
     */
    std::vector<TimeValue>& at(const std::string& metric)
    {
        return data_.at(metric);
    }

protected:
    void setup_complete() override
    {
        Drain::setup_complete();
        for (const auto& metric : metrics_)
        {
            data_[metric];
        }
    }

private:
    std::unordered_map<std::string, std::vector<TimeValue>> data_;
};
}