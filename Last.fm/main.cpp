//
//  main.cpp
//  Last.fm
//
//  Created by liaogang on 15/1/4.
//  Copyright (c) 2015年 liaogang. All rights reserved.
//

#include <iostream>
#include "audioTag.h"
#include "Last_fm.h"
#include "Last_fm_api.h"

typedef string tstring ;
typedef char  TCHAR;

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
    int l=strlen(str)+1;
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
FILE& operator<<(FILE& f,const tstring &str)
{
    int l=str.length();
    f<<l+1;
    fwrite(str.c_str(),sizeof(TCHAR),l,&f);
    TCHAR nullstr='\0';
    fwrite(&nullstr,sizeof(TCHAR),1,&f);
    return f;
}

FILE& operator>>(FILE& f,tstring &str)
{
    TCHAR buf[256];
    f>>buf;
    str=buf;
    return f;
}

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
    
    const char userProfile[] = "userProfile.data";
    const int sessionKeyLength = 32;
    // load it from cached file if has, else create a new session again.
    bool userProfileLoaded = false;
    FILE *file = fopen(userProfile, "w");
    if (file)
    {
        if(ftell(file)>0)
        {
            unsigned char nameLength = user.name.length();
            
            *file >> user.name;
            
            fwrite("\n", sizeof(char), 1, file);
            
            fwrite(user.sessionKey.c_str(), sizeof(char), sessionKeyLength, file);
            
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
                if(auth_getSession(user.sessionKey ,user.name))
                {
                    
                    assert(user.sessionKey.length() == sessionKeyLength);
                    
                    cout<<user.name<<"is connected"<<endl;
                    
                    // save sessionKey and userName to file.
                    
                    FILE *file = fopen(userProfile, "w");
                    if (file)
                    {
                        *file << user.name;
                        
                        fwrite("\n", sizeof(char), 1, file);
                        
                        fwrite(user.sessionKey.c_str(), sizeof(char), sessionKeyLength, file);
                        
                        fclose(file);
                    }
                    
                    
                    
                    break;
                }
                else
                {
                    printf("please press auth in the web broswer after login in.\n");
                }
                
                sleep(1000);
            }
        }
    }
    
    return 0;
}
