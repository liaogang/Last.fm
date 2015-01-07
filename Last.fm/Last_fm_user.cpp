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
    const char userProfile[] = "userProfile.data";
    
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

