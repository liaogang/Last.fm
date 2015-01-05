//
//  main.cpp
//  Last.fm
//
//  Created by liaogang on 15/1/4.
//  Copyright (c) 2015年 liaogang. All rights reserved.
//

#include <iostream>
#include "audioTag.h"
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
    
    string userName;
    string sessionKey;
    string token;
    if(auth_getToken( token ) )
    {
        openWebInstance(token);
        
        
        while (true)
        {
            if(auth_getSession( sessionKey , userName))
            {
                cout<<userName<<"is connected"<<endl;
                
                /// save sessionKey and userName to file.
                
                
                break;
            }
            else
            {
                printf("please press auth in the web broswer after login in.\n");
            }
            
            sleep(1000);
        }
    }
    

    return 0;
}
