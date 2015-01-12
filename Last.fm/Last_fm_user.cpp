//
//  Last.fm.user
//  Last.fm
//
//  Created by liaogang on 15/1/4.
//  Copyright (c) 2015年 liaogang. All rights reserved.
//

#include "Last_fm_user.h"
#include "Last_fm_api.h"

typedef char TCHAR;

//int
FILE& operator<<(FILE& f,const int t)
{
    fwrite(&t,sizeof(int),1,&f);
    return f;
}

FILE& operator>>(FILE& f,int& t)
{
    fread(&t,sizeof(int),1,&f);
    return f;
}


//write zero terminated str array
FILE& operator<<(FILE& f,const TCHAR * str)
{
    int l=(int)strlen(str)+1;
    f<<l;
    fwrite(str,sizeof(TCHAR),l,&f);
    return f;
}

FILE& operator>>(FILE& f,TCHAR * str)
{
    int l=0;
    f>>l;
    fread(str,sizeof(TCHAR),l,&f);
    return f;
}

//tstring
FILE& operator<<(FILE& f,const string &str)
{
    int l=(int)str.length();
    f<<l+1;
    fwrite(str.c_str(),sizeof(char),l,&f);
    char nullstr='\0';
    fwrite(&nullstr,sizeof(char),1,&f);
    return f;
}

FILE& operator>>(FILE& f,string &str)
{
    char buf[256];
    f>>buf;
    str=buf;
    return f;
}

/// load it from cached file if has, else create a new session again.
bool auth(LFUser &user)
{
    const char userProfile[] = "lastFm.cfg";
    
    bool userProfileLoaded = false;
    
    FILE *file = fopen(userProfile, "r");
    if (file)
    {
        fseek(file, 0, SEEK_END );
        if(ftell(file)>sessionKeyLength)
        {
            fseek(file, 0, SEEK_SET);
            
            *file >> user.name >> user.sessionKey;
    
            userProfileLoaded = true;
        }
        fclose(file);
    }
    
    
    if (userProfileLoaded == false)
    {
        string token;
        if(auth_getToken( token ) )
        {
            openWebInstance(token);
            
            while (true)
            {
                if(auth_getSession(token,user.sessionKey ,user.name))
                {
                    userProfileLoaded = true;
                    
                    // save sessionKey and userName to file.
                    
                    FILE *file = fopen(userProfile, "w");
                    if (file)
                    {
                        *file << user.name << user.sessionKey;
                        
                        fclose(file);
                    }
                    
                    
                    
                    break;
                }
                else
                {
                    printf("please press auth in the web broswer after login in or wait a minute\n\n");
                }
                
                sleep(10);
            }
        }
    }
    
    user.isConnected = userProfileLoaded;
    
    return userProfileLoaded;
}






FILE& operator<<(FILE& f,const time_t &t)
{
    fwrite(&t, sizeof(time_t), 1, &f);
    return f;
}

FILE& operator>>(FILE& f,time_t& t)
{
    fread(&t, sizeof(time_t), 1, &f);
    return f;
}

template <class T>
FILE& operator<<(FILE& f,const vector<T> &t)
{
    int length = (int)t.size();
    f<<length;
    for (int i = 0; i< length; i++)
    {
        f<<t[i];
    }
    return f;
}

template <class T>
FILE& operator>>(FILE& f,vector<T> &t)
{
    int length ;
    f>>length;
    
    for (int i = 0; i< length; i++)
    {
        T tt;
        f>>tt;
        t.push_back(tt);
    }
    
    return f;
}

/// LFTrackRecord

FILE& operator<<(FILE& f,const LFTrackRecord &t)
{
    return f<<t.artist<<t.track<<t.time;
}

FILE& operator>>(FILE& f,LFTrackRecord& t)
{
    return f>>t.artist>>t.track>>t.time;
}

/// LFTrackRecords

FILE& operator<<(FILE& f,const LFTrackRecords &t)
{
    return f<<t.records;
}

FILE& operator>>(FILE& f,LFTrackRecords& t)
{
    return f>>t.records;
}






const char localRecord[] = "localRecord.cfg";
static  LFTrackRecords records;
static bool recordsDirty = false;
LFTrackRecords& loadTrackRecords()
{
    FILE *file = fopen(localRecord, "r");
    if (file)
    {
        (*file)>>records;
        fclose(file);
    }
    
    return records;
}

void addScrobbleLocal(string &artist,string &track)
{
    time_t t;
    time(&t);
    
    records.records.push_back(LFTrackRecord(artist,track,t));
    
    recordsDirty = true;
}

void saveTrackRecords()
{
    if (recordsDirty)
    {
        FILE *file = fopen(localRecord, "w");
        if (file)
        {
            (*file)<<records;
            fclose(file);
        }
    }
    
}


