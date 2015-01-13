//
//  Last.fm.user
//  Last.fm
//
//  Created by liaogang on 15/1/4.
//  Copyright (c) 2015年 liaogang. All rights reserved.
//
#include "serialize.h"
#include "Last_fm_local_record.h"

typedef char TCHAR;

/// int
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

/// char
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

/// string
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

/// time_t
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

///  vector<T>
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





