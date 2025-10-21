#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <random>

struct CANMessage {
    int sender_id;
    double setpoint;
    double current_pos;
    double arrival_time;
};

class CANBus {
public:
    CANBus(int min_delay, int max_delay, double loss_chance)
        : gen_(rd_()), delay_dist_(min_delay, max_delay), loss_dist_(0.0, 1.0), packet_loss_chance_(loss_chance) {
    }

    void sendMessage(int sender_id, double setpoint, double current_pos, double current_time) {
        if (loss_dist_(gen_) < packet_loss_chance_) {
            // Packet lost
            return;
        }
        double delay = delay_dist_(gen_) / 1000.0; // convert ms to seconds
        message_queue.push({ sender_id, setpoint, current_pos, current_time + delay });
    }

    bool receiveMessage(double current_time, CANMessage& received_msg) {
        if (!message_queue.empty() && message_queue.front().arrival_time <= current_time) {
            received_msg = message_queue.front();
            message_queue.pop();
            return true;
        }
        return false;
    }

private:
    std::queue<CANMessage> message_queue;
    double packet_loss_chance_;

    // For random number generation
    std::random_device rd_;
    std::mt19937 gen_;
    std::uniform_int_distribution<> delay_dist_;
    std::uniform_real_distribution<> loss_dist_;
};