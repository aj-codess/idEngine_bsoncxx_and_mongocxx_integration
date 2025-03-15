#pragma once

#include <iostream>
#include <string>

#include <boost/asio.hpp>
#include <boost/url.hpp>
#include <boost/beast.hpp>

class url_dep{
    public:
    url_dep()=default;
    bool path_finder(boost::beast::http::request<boost::beast::http::string_body>& req,std::string path_condition);
    bool sniff(boost::beast::http::request<boost::beast::http::string_body>& req,std::string path);

    private:

};


bool url_dep::path_finder(boost::beast::http::request<boost::beast::http::string_body>& req,std::string path_condition){
    
    boost::urls::url_view url(req.target());

    std::string url_path=std::string(url.encoded_path());

    return (url_path==path_condition);

};


bool url_dep::sniff(boost::beast::http::request<boost::beast::http::string_body>& req,std::string path){

    boost::urls::url_view url(req.target());

    std::string url_path=std::string(url.encoded_path());

    if(url_path.find(path) != std::string::npos){

        return true;

    };

    return false;

};