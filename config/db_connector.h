#pragma once

#include <iostream>
#include <string>
#include <memory>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>

using namespace std;

inline std::unique_ptr<mongocxx::client> client;

void connectToDatabase() {

    try{

        static mongocxx::instance instance{};

        client = std::make_unique<mongocxx::client>(mongocxx::uri("mongodb://localhost:27017"));

        cout<<"Connected To Mongo Database......"<<endl;

    } catch(std::exception& error){

        cout<<"Error Occured Connecting to Mongodb - "<<error.what()<<endl;

    };

};