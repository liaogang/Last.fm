//
//  Last.fm.h
//  Last.fm
//
//  Created by liaogang on 15/1/4.
//  Copyright (c) 2015年 liaogang. All rights reserved.
//

#ifndef __Last_fm__Last_fm__
#define __Last_fm__Last_fm__

#include <stdio.h>
#include <string>
#include "socketTool.h"
#include <assert.h>

using namespace std;

/**
 Get the metadata for an artist. Includes biography, truncated at 300 characters.
 */
void artist_getInfo(string &artist);


/**
 Get the metadata for a track on Last.fm using the artist/track name or a musicbrainz id.
 */
void track_getInfo(string &artist , string & track);



bool auth_getToken( string &token );


void openWebInstance(const string &token);


const int sessionKeyLength = 32;


bool auth_getSession(string &token,string &sessionKey,string &userName);


string utf8code(string &str);


#endif /* defined(__Last_fm__Last_fm__) */
