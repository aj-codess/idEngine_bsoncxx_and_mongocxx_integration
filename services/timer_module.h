#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <chrono>
#include <ctime>
#include <mutex>
#include <memory>

using namespace std;

const std::string server_name="ID_ENGINE";

class timer{
    private:
    time_t now_time();
    std::mutex mtx;

    public:
    timer()=default;
    std::string gen_id(std::function<void(time_t)> callback);
    time_t get_timeOnly();
};



std::string timer::gen_id(std::function<void(time_t)> callback){

    std::lock_guard<std::mutex> locker(mtx);

    time_t time=this->now_time();

    if(callback){

        callback(time);

    };

    return server_name+std::to_string(time);

};


time_t timer::now_time(){

    auto current_time=std::chrono::system_clock::now();

    auto get_duration=current_time.time_since_epoch();

    auto get_in_milli=std::chrono::duration_cast<std::chrono::milliseconds>(get_duration).count();

    return get_in_milli;

};


time_t timer::get_timeOnly(){

    return this->now_time();

};