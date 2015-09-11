//
//  main.cpp
//  Last.fm
//
//  Created by liaogang on 15/1/4.
//  Copyright (c) 2015年 liaogang. All rights reserved.
//

#include <iostream>
#include "Last_fm_user.h"
#include "Last_fm_api.h"

#include <time.h>

#include "Last_fm_local_record.h"

void testLocalRecord()
{
    LFTrackRecords records = loadTrackRecords();
    

    
    
    
 
    
    
    string artist = "蔡健雅";
    string track = "Beautiful love";
    
    addScrobbleLocal(artist, track);
    addScrobbleLocal(artist, track);
    addScrobbleLocal(artist, track);
    addScrobbleLocal(artist, track);
    
    string artist2 = "Michael Jackson";
    string track2 = "Earth Song";
    
    addScrobbleLocal(artist2, track2);
    addScrobbleLocal(artist2, track2);
    addScrobbleLocal(artist2, track2);
    addScrobbleLocal(artist2, track2);
    
    saveTrackRecords();
    
}





int main(int argc, const char * argv[])
{
   /*
    testLocalRecord();
    return 0;
    */
    
    /*
    if(curlUrlFile("http://userserve-ak.last.fm/serve/252/30501527.png", "18583905.jpg")>0)
    {
        cout<<"ok"<<endl;
    }
     */
    
    
    /*
     if(curlUrlFile("http://userserve-ak.last.fm/serve/_/3095298/Goodbye++Hello.jpg", "Goodbye++Hello.jpg")>0)
    {
        cout<<"ok"<<endl;
    }
     
    return 0;
    */
    
    
    string artist = "蔡健雅";
    string track = "Beautiful love";
    
    string artist2 = "Michael Jackson";
    string track2 = "Earth Song";
    
    /*
    LFTrack lfTrack;
    if(track_getInfo( artist, track ,lfTrack))
    {
        cout<<"ok"<<endl;
    }
    return 0;
    */
    
    /*
    string artist2 = "陈洁仪";
    LFArtist lfArtist ;
    if(artist_getInfo( artist ,lfArtist) )
    {
        cout<<lfArtist.name<<endl;
    }
    return 0;
    */
    
    LFUser user;
    if(authLocal(user) )
    {
        cout<<" is connected to : "<<user.name << " , sessionKey:"<<user.sessionKey<<endl<<endl;
    }
    else
        cout<<"not connected."<<endl;
    
    
    /*
    vector<string> artists ({artist,artist2});
    vector<string> tracks ({track,track2});
    vector<string> times ({""});
    
    
    if (track_scrobble(user.sessionKey, artists , tracks , times))
        cout<<"ok"<<endl;
    else
        cout<<"error"<<endl;
    */
 
    
    
    
    /*
    vector<LFTrack> tracks;
    if(user_getRecentTracks(user.name , tracks) )
    {
        cout<<"ok"<<endl;
        
        for (int i = 0; i < tracks.size() ; i++) {
            LFTrack track = tracks[i];
            
            cout<<"track: "<<track.name<<" , artist: "<<track.artist.name<<track.artist.text<<endl;
        }
    }
    else
        cout<<"error"<<endl;
    */
    
    
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
