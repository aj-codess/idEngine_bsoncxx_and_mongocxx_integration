#pragma once

#include <iostream>
#include <string>

#include <logic.h>
#include <jsonScript.h>

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <nlohmann/json.hpp>

using namespace std;

class persistent_class{
    private:
    persistent_class()=default;
    id_schema engine;
    jsonScript json;
    persist get_persistent();

    public:
    void payload(boost::beast::http::response<boost::beast::http::string_body>& res);
};


void persistent_class::payload(boost::beast::http::response<boost::beast::http::string_body>& res){

    res.body()=(this->json.convert_persistent(this->get_persistent())).dump();

    res.result(boost::beast::http::status::ok);

};



persist persistent_class::get_persistent(){
    persist payload;

    try{
        payload=this->engine.get_persistent();
    } catch(std::exception& error){
        cout<<error.what()<<endl;
    }

    return payload;
};