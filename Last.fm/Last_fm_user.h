//
//  Last.fm.h
//  Last.fm
//
//  Created by liaogang on 15/1/4.
//  Copyright (c) 2015年 liaogang. All rights reserved.
//

#ifndef __Last_fm__Last_fm_user__
#define __Last_fm__Last_fm_user__

#include <string>
#include <vector>

using namespace std;

class LFUser
{
public:
    LFUser():isConnected(false){};
    
    string name;
    string sessionKey;
    
    bool isConnected;
};

struct LFTrackRecord
{
public:
    string artist;
    string track;
    time_t time;
public:
    bool updatedLastFm;
    LFTrackRecord()
    {
        
    }
    
    LFTrackRecord(string &a,string &t,time_t tm):artist(a),track(t),time(tm)
    {
        
    }
};

struct LFTrackRecords
{
public:
    vector<LFTrackRecord> records;
    
};




bool auth(LFUser &user);



LFTrackRecords& loadTrackRecords();
void addScrobbleLocal(string &artist,string &track);
void saveTrackRecords();

#endif

