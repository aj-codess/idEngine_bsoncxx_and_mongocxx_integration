#pragma once

#include <iostream>
#include <string>

#include <logic.h>
#include <jsonScript.h>

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/url.hpp>
#include <nlohmann/json.hpp>

using namespace std;

class getId_controller{
    private:
    id_schema engine;
    jsonScript json;
    void url_2_body_gen(boost::beast::http::request<boost::beast::http::string_body>& req,std::string& body_2_send);
    std::string cast(boost::urls::params_view::iterator value_2_cast);

    public:
    getId_controller()=default;
    void get_indi_id(boost::beast::http::request<boost::beast::http::string_body>& req,boost::beast::http::response<boost::beast::http::string_body>& res);
    void owned_ugc(boost::beast::http::request<boost::beast::http::string_body>& req,boost::beast::http::response<boost::beast::http::string_body>& res);
    void owned_space(boost::beast::http::request<boost::beast::http::string_body>& req,boost::beast::http::response<boost::beast::http::string_body>& res);
    void usersInSpace(boost::beast::http::request<boost::beast::http::string_body>& req,boost::beast::http::response<boost::beast::http::string_body>& res);
    void ugcInSpace(boost::beast::http::request<boost::beast::http::string_body>& req,boost::beast::http::response<boost::beast::http::string_body>& res);
};



//make a json to string cast
std::string getId_controller::cast(boost::urls::params_view::iterator value_2_cast) {
    // Access the value from the iterator by dereferencing it first
    const auto& param = *value_2_cast;  // Dereference to get the key-value pair

    return std::string(param.value.data(), param.value.size());  // Convert the value to std::string

};


void getId_controller::url_2_body_gen(boost::beast::http::request<boost::beast::http::string_body>& req,std::string& payload_){

    boost::urls::url_view url(req.target());

    auto option=url.params().find("option");

    auto user_id_f_ugc=url.params().find("user_id");

    auto space_id_f_ugc=url.params().find("space_id");

    if(option != url.params().end() && (user_id_f_ugc !=url.params().end() || space_id_f_ugc!=url.params().end())){

        payload_=this->engine.generate_id(cast(option),cast(user_id_f_ugc),cast(space_id_f_ugc));

    };

};



void getId_controller::get_indi_id(boost::beast::http::request<boost::beast::http::string_body>& req,boost::beast::http::response<boost::beast::http::string_body>& res){
    
    try{

        std::string payload;

        this->url_2_body_gen(req,payload);

        res.body()=(json.id_2_json(payload)).dump();

        res.result(boost::beast::http::status::ok);

    } catch(std::exception& error){
        cout<<"Error Getting Indie id - "<<error.what()<<endl;
    };
    
};


void getId_controller::owned_ugc(boost::beast::http::request<boost::beast::http::string_body>& req,boost::beast::http::response<boost::beast::http::string_body>& res){
    try{

        boost::urls::url_view url(req.target());

        auto user_id=url.params().find("user_id");

        //res.body()=(this->engine.).dump();

        res.result(boost::beast::http::status::ok);

    } catch(std::exception& error){
        res.body()="Error getting ids";
    }
};


void getId_controller::owned_space(boost::beast::http::request<boost::beast::http::string_body>& req,boost::beast::http::response<boost::beast::http::string_body>& res){
    try{

        boost::urls::url_view url(req.target());

        auto user_id=url.params().find("user_id");

        //res.body()=(this->engine.).dump();

        res.result(boost::beast::http::status::ok);

    } catch(std::exception& error){
        res.body()="Error getting ids";
        cout<<"internal System Error -"<<error.what()<<endl;
    }
};


void getId_controller::usersInSpace(boost::beast::http::request<boost::beast::http::string_body>& req,boost::beast::http::response<boost::beast::http::string_body>& res){
    try{

        boost::urls::url_view url(req.target());

        auto user_id=url.params().find("space_id");

                //res.body()=(this->engine.).dump();

        res.result(boost::beast::http::status::ok);

    } catch(std::exception& error){
        res.body()="Internal System Error";
        cout<<"Internal System Error - "<<error.what()<<endl;
    };
};



void getId_controller::ugcInSpace(boost::beast::http::request<boost::beast::http::string_body>& req,boost::beast::http::response<boost::beast::http::string_body>& res){
    try{

        boost::urls::url_view url(req.target());

        auto user_id=url.params().find("space_id");

                //res.body()=(this->engine.).dump();

        res.result(boost::beast::http::status::ok);

    } catch(std::exception& error){
        res.body()="Internal System Error";
        cout<<"Internal System Error - "<<error.what()<<endl;
    };
};