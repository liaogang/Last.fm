//
//  Last.fm.h
//  Last.fm
//
//  Created by liaogang on 15/1/4.
//  Copyright (c) 2015年 liaogang. All rights reserved.
//



#ifndef __seralize_h__
#define __seralize_h__

#include "Last_fm_local_record.h"

/// int
FILE& operator<<(FILE& f,const int t);


FILE& operator>>(FILE& f,int& t);


/// char
//write zero terminated str array
FILE& operator<<(FILE& f,const char* str);


FILE& operator>>(FILE& f,char* str);


/// string
FILE& operator<<(FILE& f,const string &str);


FILE& operator>>(FILE& f,string &str);


/// time_t
FILE& operator<<(FILE& f,const time_t &t);


FILE& operator>>(FILE& f,time_t& t);


///  vector<T>
template <class T>
FILE& operator<<(FILE& f,const vector<T> &t);


template <class T>
FILE& operator>>(FILE& f,vector<T> &t);


/// LFTrackRecord

FILE& operator<<(FILE& f,const LFTrackRecord &t);

FILE& operator>>(FILE& f,LFTrackRecord& t);

/// LFTrackRecords

FILE& operator<<(FILE& f,const LFTrackRecords &t);

FILE& operator>>(FILE& f,LFTrackRecords& t);


#endif

