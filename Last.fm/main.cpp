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
    string artist = "陈洁仪";
    string track = "";
    
    //track_getInfo( artist, track );
    
    
    
    
    /*
    string artist2 = "Michael Jackson";
    
    cout<<utf8code(artist2)<<endl;
    
    artist_getInfo( artist2);
    */
    
    
    LFUser user;
    if(auth(user) )
    {
        cout<<" is connected to : "<<user.name << " , sessionKey:"<<user.sessionKey<<endl;
    }
    else
        cout<<"not connected."<<endl;
    
    return 0;
}
