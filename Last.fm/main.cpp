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

#include <time.h>

int main(int argc, const char * argv[])
{
    string artist = "Michael Jackson";
    string track = "Earth Song";
    
    //track_getInfo( artist, track );
    
    
    
    
    /*
    string artist2 = "陈洁仪";
    LFArtist lfArtist ;
    if(artist_getInfo( artist2 ,lfArtist) )
    {
        cout<<lfArtist.name<<endl;
    }
    */
    
    
    LFUser user;
    if(auth(user) )
    {
        cout<<" is connected to : "<<user.name << " , sessionKey:"<<user.sessionKey<<endl<<endl;
    }
    else
        cout<<"not connected."<<endl;
    
    
    /*
    vector<string> artists ({artist});
    vector<string> tracks ({track});
    vector<string> times ({"2015:01:08:02"});
    
    time_t t;
    time(&t);
    
    if (track_scrobble(user.sessionKey, artists , tracks , times))
        cout<<"ok"<<endl;
    else
        cout<<"error"<<endl;
    */
 
    
    vector<LFTrack> tracks;
    if(user_getRecentTracks(user.name , tracks) )
    {
        cout<<"ok"<<endl;
        
        for (int i = 0; i < tracks.size() ; i++) {
            LFTrack track = tracks[i];
            
            cout<<"track:"<<track.name<<" , artist:"<<track.artist.name<<track.artist.text<<endl;
            
        }
        
        
    }
    else
        cout<<"error"<<endl;
    
    
    
    
    /*
    if(track_updateNowPlaying(user.sessionKey, artist, track) )
        cout<<"ok"<<endl;
    else
        cout<<"error"<<endl;
    */
    
    
    
    /*
    if( track_love( user.sessionKey, artist, track) )
    {
        cout<<"user: "<<user.name<<" loved a track."<<endl;
    }
    else
        cout<<"user: "<<user.name<<" loved a track failed!"<<endl;
    */
    
    
    
    
    
    
    
    
    return 0;
}
