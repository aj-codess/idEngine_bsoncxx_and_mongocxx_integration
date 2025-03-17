#pragma once

#include <iostream>
#include <string>

#include <boost/asio.hpp>
#include <boost/beast.hpp>

#include <url_dep.h>
#include <jsonScript.h>
#include <delete_controller.h>

class delete_routes{
    private:
    url_dep url;
    void not_found(boost::beast::http::response<boost::beast::http::string_body>& res);
    delete_controller del_contr;
    jsonScript json;

    public:
    delete_routes()=default;
    void route(boost::beast::http::request<boost::beast::http::string_body>& req,boost::beast::http::response<boost::beast::http::string_body>& res);
};



void delete_routes::route(boost::beast::http::request<boost::beast::http::string_body>& req,boost::beast::http::response<boost::beast::http::string_body>& res){

            if(this->url.path_finder(req,"/del_user_id")){

                res.result(boost::beast::http::status::ok);

                res.body() = (this->json.delete_bool(this->del_contr.global_del(req))).dump();

            } else if(this->url.path_finder(req,"/del_space_id")){
                
                res.result(boost::beast::http::status::ok);

                res.body() = (json.delete_bool(this->del_contr.global_del(req))).dump();

            } else if(this->url.path_finder(req,"/del_ugc")){

                res.result(boost::beast::http::status::ok);

                res.body() = (json.delete_bool(this->del_contr.global_del(req))).dump();

            }else if(this->url.path_finder(req,"/del_space_ugc")){

                res.result(boost::beast::http::status::ok);

                res.body() = (json.delete_bool(this->del_contr.global_del(req))).dump();

            } else{

                this->not_found(res);

            };

};



void delete_routes::not_found(boost::beast::http::response<boost::beast::http::string_body>& res){

    res.result(boost::beast::http::status::not_found);
            
    res.body() = "The resource was not found.";

};