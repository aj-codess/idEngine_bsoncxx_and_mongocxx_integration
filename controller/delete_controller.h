#pragma once

#include <iostream>
#include <string>

#include <nlohmann/json.hpp>

#include <logic.h>

using namespace std;


class delete_controller{
    private:
    id_schema generator;
    std::string string_cast(nlohmann::json value);

    public:
    delete_controller()=default;
    bool global_del(boost::beast::http::request<boost::beast::http::string_body>& req);
};



std::string delete_controller::string_cast(nlohmann::json value){
    std::string cast;

    try{

        cast=value.get<std::string>();

    } catch(const std::exception& e){

        cout<<"unable to cast data - "<<e.what()<<endl;

    };

    return cast;
};



bool delete_controller::global_del(boost::beast::http::request<boost::beast::http::string_body>& req){
    bool isDeleted;

    try{

    nlohmann::json json_body=nlohmann::json::parse(req.body());

    auto user_id=json_body.find("user_id");

    auto user_pos=json_body.find("user_pos");

    auto space_id=json_body.find("space_id");

    auto space_pos=json_body.find("space_pos");

    auto ugc_id=json_body.find("ugc_id");

    if( user_id != json_body.end() && user_pos != json_body.end()){
        
        isDeleted=this->generator.del_user(this->string_cast(json_body["user_id"]),string_cast(json_body["user_pos"]));

    } else if(space_id != json_body.end() && space_pos != json_body.end()){

        isDeleted=this->generator.del_space(this->string_cast(json_body["space_id"]),string_cast(json_body["space_pos"]));

    } else if(user_id != json_body.end() && ugc_id != json_body.end()){

        isDeleted=this->generator.del_ugc(this->string_cast(json_body["user_id"]),string_cast(json_body["ugc_id"]));

    } else{

        isDeleted=this->generator.del_space_ugc(this->string_cast(json_body["space_id"]),string_cast(json_body["space_ugc"]));

    };


    } catch(std::exception& error){
        cout<<"Error Deleting - "<<error.what()<<endl;
    };

    return isDeleted;

};