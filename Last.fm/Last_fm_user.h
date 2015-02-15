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

void setUserProfilePath(string path);

bool auth(LFUser &user);

#endif
