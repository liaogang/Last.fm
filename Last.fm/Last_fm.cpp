//
//  Last.fm.user
//  Last.fm
//
//  Created by liaogang on 15/1/4.
//  Copyright (c) 2015年 liaogang. All rights reserved.
//

#include "Last_fm.h"
#include <cstdio> //sprintf
#include <cstring> // strcat
#include "base64.h"
#include <assert.h>
#include <iostream>
#include <json/json.h>
#include "gzstream.h"
#include "md5.h"
using namespace std;

string utf8code(string &str)
{
    unsigned char buffer2[125]={0};
    
    size_t length = str.length();
    int ii=0;
    for ( size_t i = 0; i < length; i++)
    {
        unsigned char a = str[i];
        
        if (isalpha(a))
        {
            buffer2[ii++]=a;
        }
        else
        {
            buffer2[ii++]='%';
            sprintf((char*)buffer2+ii,"%X",a);
            ii+=2;
        }
    }
    
    return string((const char*)buffer2,(size_t)strlen((const char*)buffer2));
}


LFArtist::LFArtist(){}
LFArtist::LFArtist(Json::Value &parent, bool complete):LFArtistBasic(parent[jsonID()])
{
    Json::Value jvArtist = parent[jsonID()];
    
    assert(jvArtist.type() == Json::objectValue );
    

    
    if (complete)
    {
        stats = LFStats(jvArtist);
        
        similarArtist = LFSimilarArtist(jvArtist);
        
        tags = LFTags(jvArtist);
        
        bio = LFBio(jvArtist);
    }
}



LFTrack::LFTrack(){}

LFTrack::LFTrack(Json::Value &parent)
{
    Json::Value dicTrack = parent[jsonID()];
    
    id = dicTrack["id"].asString();
    name = dicTrack["name"].asString();
    mbid = dicTrack["mbid"].asString();
    url = dicTrack["url"].asString();
    duration = dicTrack["duration"].asString();
    //streamable = dicTrack["streamable"].asString();
    listeners = dicTrack["listeners"].asString();
    playcount = dicTrack["playcount"].asString();
    artist = LFArtist(dicTrack,false) ;
    toptags = dicTrack["toptags"].asString();
};




/**
 Last.fm

 
 test
 player
 API Key: 6ef0a182fcb172b557c0ca096594f288
 Secret: is 3b1a4e1e970ed3a30c28cd65bb88579c
 
 http://ws.audioscrobbler.com/2.0/?api_key=6ef0a182fcb172b557c0ca096594f288&country=&lang=zh&method=Track.getInfo&artist=Michael%20Jackson&track=Earth%20Song&format=json
 
 */

const char lastFmPath[]="/2.0/";
const char lastFmHost[] = "ws.audioscrobbler.com" ;
const char lastFmApiKey[] = "6ef0a182fcb172b557c0ca096594f288" ;
const char lastFmLang[10] ="zh";

/// a param and it's value.
struct paramPair
{
    string param;
    string value;
    paramPair(string p , string v):param(p),value(v)
    {
        
    }
};


/// return server's response content if have. else nullptr is returned;
MemBuffer* lastFmSendRequestWithoutAuth(bool mkMd5, paramPair arrParamPairs[] , int numParamPairs)
{
    assert(numParamPairs>=1);
    
    char strParams[256]={0};
    
    strcat(strParams, "api_key=");
    strcat(strParams, lastFmApiKey);
   
    string strMD5("api_key=");
    strMD5+=lastFmApiKey;
    
    for( int i = 0; i< numParamPairs; i++)
    {
        paramPair pPP = arrParamPairs[i];
        
        strcat(strParams, "&");
        strcat(strParams, pPP.param.c_str());
        strcat(strParams, "=");
        strcat(strParams, pPP.value.c_str());
        
        strMD5+=pPP.param;
        strMD5+=pPP.value;
    }
    
    strcat(strParams, "&format=json");
    
    strMD5+="format";
    strMD5+="json";
    
    if (mkMd5)
    {
        strMD5=md5(strMD5);

        strcat( strParams,   "&api_sig=");
        strcat( strParams,   strMD5.c_str());
    }
    
    
    const int senderHeaderLenMax = 2048;
    
 

    unsigned char senderHeader[senderHeaderLenMax];
const char senderHeaderFormatter[] =
"GET %s?%s HTTP/1.1\r\n\
Connection: Keep-Alive\r\n\
Accept-Encoding: gzip\r\n\
Accept-Language: zh-CN,en,*\r\n\
Host: %s\r\n\
\r\n";
    
    
    sprintf( (char*) senderHeader, senderHeaderFormatter , lastFmPath , strParams , lastFmHost );
    
    int senderHeaderLen = strlen((char*)senderHeader);
    
    printf("%s\n",senderHeader);
    
    int socketClient;
    if(CreateTcpSocketClient(lastFmHost  , &socketClient) )
    {
        if(  sendDataToSocket(socketClient, senderHeader, senderHeaderLen) == senderHeaderLen )
        {
            MemBuffer *socketBuf = recvSocketData(socketClient);
            closesocket(socketClient);
            return socketBuf;
        }
        
    }
    
    return nullptr;
}



void artist_getInfo(string &artist)
{
    const char fileNameGz[] = "gzip.resp";
    const char fileName[] = "text.resp";
    
    /////////////////////////////////////
    //parse it by json.
    Json::Reader reader;
    Json::Value root;
    
    std::ifstream istream ( fileName );
    
    reader.parse( istream, root);
    
    LFArtist lfArtist( root , true);
    
    return;
    /////////////////////////////////////////
    /*
    paramPair m("method","artist.getInfo");
    paramPair a("artist", utf8code(artist));
    paramPair at("autocorrect","1");
    */
    paramPair arrParamPair[] =
    {
        {"method","artist.getInfo"},
        {"artist", utf8code(artist)},
        {"autocorrect","1"}
    };
    
    const int arrLen=sizeof(arrParamPair)/sizeof(arrParamPair[0]);
    
    MemBuffer *buffer = lastFmSendRequestWithoutAuth(false, arrParamPair , arrLen);
    
    if (buffer)
    {

        
        FILE *file = fopen( fileNameGz, "w");
        if(file)
        {
            fwrite(buffer->buffer, sizeof(char), buffer->length, file);
            fclose(file);
        }
        
        
        igzstream in( fileNameGz );
        if ( ! in.good()) {
        }
        std::ofstream  out( fileName);
        if ( ! out.good()) {

        }
        char c;
        while ( in.get(c))
            out << c;
        in.close();
        out.close();
        
        
        
        //parse it by json.
        Json::Reader reader;
        Json::Value root;
        
        std::ifstream istream ( fileName );
        
        reader.parse( istream, root);
        
        LFArtist lfArtist( root , true);
        
        deleteMemBuffer(buffer);
    }
    
}



void track_getInfo(string &artist , string & track)
{
    paramPair arrParamPair[] =
    {
        {"artist",utf8code( artist)},
        {"autocorrect","1"},
        {"method","track.getInfo"},
        {"lang",lastFmLang},
        {"track", utf8code(track)}
    };
    
    const int arrLen=sizeof(arrParamPair)/sizeof(arrParamPair[0]);
    
    MemBuffer *buffer = lastFmSendRequestWithoutAuth(false, arrParamPair ,arrLen);
    
    if (buffer)
    {
        printf("%s\n",buffer->buffer);
        
        //parse it by json.
        Json::Reader reader;
        Json::Value root;
        reader.parse((const char*)buffer->buffer, (const char*)buffer->buffer+buffer->length , root);
        
        LFTrack lfTrack(root);
        
        deleteMemBuffer(buffer);
    }
}


bool auth_getToken( string &token )
{
    paramPair arrParamPair[] =
    {
        {"method","auth.gettoken"},
    };
    
    const int arrLen=sizeof(arrParamPair)/sizeof(arrParamPair[0]);
    
    MemBuffer *buffer = lastFmSendRequestWithoutAuth(true, arrParamPair ,arrLen);
    
    if (buffer)
    {
        printf("%s\n",buffer->buffer);
        
        //parse it by json.
        Json::Reader reader;
        Json::Value root;
        reader.parse((const char*)buffer->buffer, (const char*)buffer->buffer+buffer->length , root);
        
        token = root["token"].asString();
        
        deleteMemBuffer(buffer);
        
        return true;
    }
    
    return false;
}


void openWebInstance(const string &token)
{
    //    http://www.last.fm/api/auth/?api_key=xxxxxxxxxx&token=yyyyyy
    
    string url="open http://www.last.fm/api/auth/?api_key=";
    url+=lastFmApiKey;
    url+="&token=";
    url+=token;
    
    system(url.c_str());
    
}

bool auth_getSession(string &sessionKey,string &userName)
{
    paramPair arrParamPair[] =
    {
        {"method","auth.getSession"},
        {"token",sessionKey}
    };
    
    const int arrLen=sizeof(arrParamPair)/sizeof(arrParamPair[0]);
    
    MemBuffer *buffer = lastFmSendRequestWithoutAuth(true, arrParamPair ,arrLen);
    
    if (buffer)
    {
        printf("%s\n",buffer->buffer);
        
        //parse it by json.
        Json::Reader reader;
        Json::Value root;
        reader.parse((const char*)buffer->buffer, (const char*)buffer->buffer+buffer->length , root);
        
        Json::Value v = root["status"];
        v = v["session"];
        
        sessionKey = v["key"].asString();
        
        userName = v["name"].asString();
        
        deleteMemBuffer(buffer);
        
        return true;
    }
    
    
    return false;
}