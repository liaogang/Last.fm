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
#include <stdarg.h> //va_list
#include "base64.h"
#include <assert.h>
#include <iostream>

using namespace std;
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


/// a param and it's value.
struct paramPair
{
    char *param;
    char *value;
    paramPair(char *p ,char *v):param(p),value(v)
    {
        
    }
};


/// return server's response content if have. else nullptr is returned;
unsigned char* lastFmSendRequestWithoutAuth(int numParamPairs, /*paramPair*/ ...)
{
    assert(numParamPairs>=1);
    
    va_list vl;
    
    va_start(vl,numParamPairs);
    
    char strParams[256]={0};
    
    strcat(strParams, "api_key");
    strcat(strParams, "=");
    strcat(strParams, lastFmApiKey);
    
    for( int i = 0; i< numParamPairs; i++)
    {
        paramPair *pPP;
        
        pPP = va_arg(vl, paramPair*);
        
        strcat(strParams, "&");
        strcat(strParams, pPP->param);
        strcat(strParams, "=");
        strcat(strParams, pPP->value);
        
    }
    
    strcat(strParams, "&format=json");
    
    va_end(vl);
    
    
    const int senderHeaderLenMax = 2048;
    
 

    unsigned char senderHeader[senderHeaderLenMax];
const char senderHeaderFormatter[] =
"GET %s?%s HTTP/1.1\r\n\
Connection: Keep-Alive\r\n\
Host: %s\r\n\
\r\n";
    
    
    sprintf( (char*) senderHeader, senderHeaderFormatter , lastFmPath , strParams , lastFmHost );
    
    int senderHeaderLen = strlen((char*)senderHeader);
    
    //int senderHeaderLen = sizeof(senderHeaderFormatter)/sizeof(senderHeaderFormatter[0]) - 3 * 2 * sizeof(char) + strlen(lastFmPath) + strlen(strParams) + strlen(lastFmHost);
    
    printf("%s\n",senderHeader);
    
    int socketClient;
    if(CreateTcpSocketClient(lastFmHost  , &socketClient) )
    {
        if(  sendDataToSocket(socketClient, senderHeader, senderHeaderLen) == senderHeaderLen )
        {
            SocketBuffer *socketBuf = recvSocketData(socketClient);
            closesocket(socketClient);
            
            if (socketBuf->length > 0)
                return socketBuf->buffer;
        }
        
    }
    
    return nullptr;
}


/**
     Get the metadata for a track on Last.fm using the artist/track name or a musicbrainz id. 
     @ref: http://cn.last.fm/api/show/track.getInfo
     @param:
 mbid (Optional) : The musicbrainz id for the track
 track (Required (unless mbid)] : The track name
 artist (Required (unless mbid)] : The artist name
 username (Optional) : The username for the context of the request. If supplied, the user's playcount for this track and whether they have loved the track is included in the response.
 autocorrect[0|1] (Optional) : Transform misspelled artist and track names into correct artist and track names, returning the correct version instead. The corrected artist and track name will be returned in the response.
 api_key (Required) : A Last.fm API key.
 
 */

void track_getInfo(char *artist , char *track)
{
    //paramPair
    
    /*
    method=Track.getInfo
    artist=Michael%20Jackson
    track=Earth%20Song
    */
    
    paramPair m("method","Track.getInfo");
    ;
    
    unsigned char *buffer1[125]={0};
    unsigned int i = 0;
    for (; i < strlen(artist); i++) {
        unsigned char a = artist[i];
        sprintf((char*)buffer1,"%X",a);
    }

    printf("\n");
    
    unsigned char *buffer2[125]={0};
    unsigned int i = 0;
    for (; i < strlen(artist); i++) {
        unsigned char a = artist[i];
        sprintf((char*)buffer2,"%X",a);
    }
    
    
    
    paramPair a("artist",(char*)buffer1/*(char*)artist22.c_str()*/);
    
   const unsigned char title2[] =   "earch song";
    
    std::string title22= base64_encode( (unsigned char*)track, strlen((char*)track)) ;
    
    paramPair t("track",(char*)buffer2  );
    
    unsigned char *buffer = lastFmSendRequestWithoutAuth(3,&m,&a,&t);
    
    printf("%s\n",buffer);
    
    //parse it by json.
    
    
    free(buffer);
}