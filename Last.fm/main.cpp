//
//  main.cpp
//  Last.fm
//
//  Created by liaogang on 15/1/4.
//  Copyright (c) 2015年 liaogang. All rights reserved.
//

#include <iostream>
#include "audioTag.h"
#include "Last_fm_user.h"
#include "Last_fm_api.h"



int main(int argc, const char * argv[])
{
    string artist = "Michael";
    string track = "Earth";
    
    //track_getInfo( artist, track );
    
    
    
    
    
    string artist2 = "陈洁仪";
    LFArtist lfArtist ;
    if(artist_getInfo( artist2 ,lfArtist) )
    {
        cout<<lfArtist.name<<endl;
    }
    
    
    /*
    LFUser user;
    if(auth(user) )
    {
        cout<<" is connected to : "<<user.name << " , sessionKey:"<<user.sessionKey<<endl;
    }
    else
        cout<<"not connected."<<endl;
    
    
    if( track_love( user.sessionKey, artist, track) )
    {
        cout<<"user: "<<user.name<<" loved a track."<<endl;
    }
    else
        cout<<"user: "<<user.name<<" loved a track failed!"<<endl;
    */
    
    
    return 0;
}
