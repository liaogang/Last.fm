//
//  Last.fm.user
//  Last.fm
//
//  Created by liaogang on 15/1/4.
//  Copyright (c) 2015年 liaogang. All rights reserved.
//

#include "Last_fm.h"
#include "Last_fm_api.h"
#include <cstdio> //sprintf
#include <cstring> // strcat
#include <assert.h>
#include <iostream>
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
const char lastFmApiKey[] = "6ef0a182fcb172b557c0ca096594f288";
const char lastFmSecret[] = "3b1a4e1e970ed3a30c28cd65bb88579c";
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

enum httpMethod
{
    httpMethod_post,
    httpMethod_get
};

const char *arrHttpMethod[] = {"POST","GET"};

bool cmp(paramPair a,paramPair b)
{
    int ret = strcmp( a.param.c_str() ,b.param.c_str() ) < 0;
    return ret;
}

/** return server's response content if have. else nullptr is returned.
 */
MemBuffer* lastFmSendRequest(vector<paramPair> arrParamPairs, httpMethod  method , bool mkMd5, bool acceptGzipEncoding , bool useJsonFormat )
{
    size_t numParamPairs = arrParamPairs.size();
    
    assert(numParamPairs>=1);
    
    string strParams;
   
    arrParamPairs.insert(arrParamPairs.begin(), {"api_key",lastFmApiKey});
    numParamPairs++;
    
    
//    if (useJsonFormat)
//    {
//        arrParamPairs.push_back( {"format","json"} );
//        numParamPairs++;
//    }
    
    sort(arrParamPairs.begin(), arrParamPairs.end(), cmp);
    
    
    if (mkMd5)
    {
        string strMD5;
        //
        for( int i = 0; i< numParamPairs; i++)
        {
            paramPair pPP = arrParamPairs[i];
            
            strMD5+=pPP.param;
            strMD5+=pPP.value;
        }
        
        strMD5+= lastFmSecret;
        
        arrParamPairs.push_back({"api_sig",md5(strMD5)});
        numParamPairs++;
        sort(arrParamPairs.begin(), arrParamPairs.end(), cmp);
    }
  
    
    arrParamPairs.push_back( {"format","json"} );
    numParamPairs++;
    sort(arrParamPairs.begin(), arrParamPairs.end(), cmp);
    
    for( int i = 0; i< numParamPairs; i++)
    {
        paramPair pPP = arrParamPairs[i];
        
        if(i!=0)
            strParams+='&';
        
        strParams+=pPP.param;
        strParams+='=';
        strParams+=pPP.value;
    }
    

    cout<<strParams<<endl;
    
    const int senderHeaderLenMax = 2048;
    
 
    unsigned char senderHeader[senderHeaderLenMax];
const char senderHeaderFormatter[] =
"%s %s?%s HTTP/1.1\r\n\
Connection: Keep-Alive\r\n\
%s\
Accept-Language: zh-CN,en,*\r\n\
Host: %s\r\n\
\r\n";
    
    const char acceptEncoding[] = "Accept-Encoding: gzip\r\n";
    
    sprintf( (char*) senderHeader, senderHeaderFormatter ,arrHttpMethod[method]  , lastFmPath , strParams.c_str() , acceptGzipEncoding?acceptEncoding:"" ,  lastFmHost );
    
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

    
    vector<paramPair> arrParamPair
    (
     {
         {"method","artist.getInfo"},
         {"artist", utf8code(artist)},
         {"autocorrect","1"}
     }
     );
    
    MemBuffer *buffer = lastFmSendRequest(arrParamPair ,httpMethod_get ,false,true  , true );
    
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
    vector<paramPair> arrParamPair
    (
     {
        {"artist",utf8code( artist)},
        {"autocorrect","1"},
        {"method","track.getInfo"},
        {"lang",lastFmLang},
        {"track", utf8code(track)}
     }
     );
    
   
    MemBuffer *buffer = lastFmSendRequest(arrParamPair , httpMethod_get ,false, false , true);
    
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
{    vector<paramPair> arrParamPair
    (
     {

        {"method","auth.gettoken"},
     }
     );
 
   
    MemBuffer *buffer = lastFmSendRequest( arrParamPair ,httpMethod_get ,true,false, true);
    
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
    
    string url="open \"http://www.last.fm/api/auth/?api_key=";
    url+=lastFmApiKey;
    url+="&token=";
    url+=token;
    url+="\"";
    
    // open "..&.."
    system(url.c_str());
}

bool auth_getSession(string &token,string &sessionKey,string &userName)
{    vector<paramPair> arrParamPair
    (
     {
        {"method","auth.getSession"},
        {"token",token}
     }
     );
 
    
    /// this api has a bug , if using json format.
    /// @see http://cn.last.fm/group/Last.fm+Web+Services/forum/21604/_/428269
    
    MemBuffer *buffer = lastFmSendRequest(arrParamPair ,httpMethod_get , true , false , false);
    
    if (buffer)
    {
        printf("%s\n",buffer->buffer);
        
        //parse it by json.
        Json::Reader reader;
        Json::Value root;
        reader.parse((const char*)buffer->buffer, (const char*)buffer->buffer+buffer->length , root);
        
        Json::Value v = root["session"];
        
        sessionKey = v["key"].asString();
        
        userName = v["name"].asString();
        
        deleteMemBuffer(buffer);
        
    }
    
    
    bool sessionCreated = (sessionKey.length() == sessionKeyLength) && userName.length() > 0;
    
    return sessionCreated;
}

bool track_love(string &sessionKey, string &artist , string & track )
{    vector<paramPair> arrParamPair
    (
     {
        {"artist", utf8code(artist)},
        {"method","track.love"},
        {"sk", sessionKey},
        {"track", utf8code(track) }
     }
     );
    
    MemBuffer *buffer = lastFmSendRequest(arrParamPair ,httpMethod_post ,true , false , true);
    
    if (buffer)
    {
        printf("%s\n",buffer->buffer);
        
        //parse it by json.
        Json::Reader reader;
        Json::Value root;
        reader.parse((const char*)buffer->buffer, (const char*)buffer->buffer+buffer->length , root);
        
 
        
        deleteMemBuffer(buffer);
        
        return true;
    }
    
    
    return false;

}


