#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <zlib.h>
#include <vector>
#include <memory>

using namespace std;

class shrink_operands{
    private:
    shrink_operands()=default;
    std::string reader(std::vector<unsigned char> x);

    public:
    shrink_operands()=default;
    std::string compress(std::string data);
};


std::string shrink_operands::reader(std::vector<unsigned char> x){
    std::string out;

    short i=0;

    while(i<x.size()){
        out+=x.at(i);
        i++;
    };

    return out;
};



std::string shrink_operands::compress(std::string data){
    std::string junk;

    std::unique_ptr<std::vector<unsigned char>> com_data=std::make_unique<std::vector<unsigned char>>();

    z_stream stream;
    memset(&stream,0,sizeof(stream));
    int* level=new int;
    *level=Z_DEFAULT_COMPRESSION;

    int* status=new int;
    *status=deflateInit(&stream,*level);

    if(*status != Z_OK){
        // "Error Compressing";
        // this->compress(data);
        throw std::runtime_error("Error compressing data");
    };

    com_data->resize(compressBound(data.size()));


    stream.next_in = (Bytef *)data.data();
    stream.avail_in = (uInt)data.size();
    stream.next_out = com_data->data();
    stream.avail_out = com_data->size();


    *status = deflate(&stream, Z_FINISH);
    if (*status != Z_STREAM_END) {
        // "Compression Didnt complete";
         this->compress(data);
    };

    deflateEnd(&stream);

    int* compressed_size=new int;
    *compressed_size = com_data->size() - stream.avail_out;
    com_data->push_back(*compressed_size);
    junk=this->reader(*com_data);

    delete compressed_size;
    delete status;

    return junk;
};