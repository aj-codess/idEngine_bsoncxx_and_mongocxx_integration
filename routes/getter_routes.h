#pragma once

#include <iostream>
#include <string>

#include <boost/asio.hpp>
#include <boost/beast.hpp>

#include <url_dep.h>
#include <get_id_controller.h>
#include <persistent_controller.h>

using namespace std;

class get_router{
    public:
    get_router()=default;
    void route(boost::beast::http::request<boost::beast::http::string_body>& req,boost::beast::http::response<boost::beast::http::string_body>& res);

    private:
    getId_controller id_getter;
    persistent_class persistent_getter;
    url_dep url;
    void not_found(boost::beast::http::response<boost::beast::http::string_body>& res);
};


void get_router::not_found(boost::beast::http::response<boost::beast::http::string_body>& res){

    res.result(boost::beast::http::status::not_found);
            
    res.body() = "The resource was not found.";

}


void get_router::route(boost::beast::http::request<boost::beast::http::string_body>& req,boost::beast::http::response<boost::beast::http::string_body>& res){

    if(this->url.path_finder(req,"/id")==true){

        this->id_getter.get_indi_id(req,res);

    } else if(this->url.path_finder(req,"/id/owned_ugc")==true){

        this->id_getter.owned_ugc(req,res);
        
    }else if(this->url.path_finder(req,"/id/owned_space")==true){

        this->id_getter.owned_space(req,res);
        
    }else if(this->url.path_finder(req,"/space/users")==true){

        this->id_getter.usersInSpace(req,res);
        
    }
    else if(this->url.path_finder(req,"/space/ugc")==true){

        this->id_getter.ugcInSpace(req,res);
        
    } else if(this->url.path_finder(req,"/persistent_data")){
        
        this->persistent_getter.payload(res);
    
    } else{

        this->not_found(res);

    };

};