#include <string>
#include <fstream>
#include <vector>
#include "Platform.h"

int stringToInt(std::string s){
    int n=0;
    for(int i=s.length()-1,j=1;i>=0;i--,j*=10){
        n+=j*(int)(s[i]-'0');
    }
    return n;
}

bool loadMap(int &numPlatforms, std::vector<Platform> &platforms,std::string f){         // Loads all platforms from text file
    std::ifstream file(f);
    if(!file.is_open()){
        printf("Cannot open TextFile");
        return 0;
    }
    std::string line;
    getline(file,line);
    numPlatforms=stringToInt(line);
    for(int i=0;i<numPlatforms;i++){
        getline(file,line);
        std::string s=line.substr(0,line.find(" "));
        line.erase(0,line.find(" ")+1);
        int x=stringToInt(s);
        s=line.substr(0,line.find(" "));
        line.erase(0,line.find(" ")+1);
        int y=stringToInt(s);
        s=line.substr(0,line.find(" "));
        line.erase(0,line.find(" ")+1);
        int w=stringToInt(s);
        s=line.substr(0,line.find(" "));
        int h=stringToInt(s);
        platforms.push_back(Platform(x,y,w,h));
    }
    file.close();
    return 1;
}
