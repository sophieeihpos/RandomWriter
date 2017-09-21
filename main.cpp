#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <iterator>

using namespace std;

ifstream fileStream;
stringstream sStream;
map<vector<string>,vector<string>> wordsMap;
signed int numWords;

string getText(){
    string currentText="";
    string currentLine;

    getline(sStream,currentText,' ');
    while(sStream.fail()){
        if(fileStream.is_open() && !fileStream.eof()){
            getline(fileStream,currentLine);
            if(fileStream.fail()){
                currentText="-1";
                break;
            }
            sStream= stringstream(currentLine);
            getline(sStream,currentText,' ');
            while(currentText==""){
                currentText=getText();
            }
        }else{
            currentText="-1";
            break;
        }
    }
    return currentText;
}

bool creatMap(int NGrams, string filePath){
    fileStream.open(filePath);
    vector<string> keys;
    vector<string> values;
    string currentText;

    if(fileStream.fail()){
        return 0;
    }

    int i=0;
    while(i<NGrams-1){
        currentText=getText();
        if(currentText=="-1"){
            cout<<"Essay too short for analysis."<<endl;
        }else{
            keys.push_back(currentText);
            i++;
        }

    }

    while(!(fileStream.eof() && sStream.eof())){

        currentText=getText();
        if(currentText=="-1"){
            break;
        }
        if(wordsMap.find(keys)==wordsMap.end()){
            values=vector<string>({currentText});
        }else{
            values=vector<string>(wordsMap[keys]);
            values.push_back(currentText);
        }
        wordsMap[keys]=values;
        keys.erase(keys.begin());
        keys.push_back(currentText);
    }

    fileStream.close();
    return 1;
}

void getDataFromConsole(){
    string filePath;
    int NGRAMS;
    string real;

    cout<<"Welcome to CS106B Random Writer ('N-Grams').\n"
          "This program makes random text based on a document.\n"
          "Give me an input file and 'N' value for groups of words,\n"
          " and I will create random text for you."<<endl;

    do{
        cin.clear();
        cout<<"\nSample essay file path?"<<endl;
        cin>>filePath;
        replace(filePath.begin(),filePath.end(),'\\','/');
        fileStream.open(filePath.c_str());
    } while(fileStream.fail());

    fileStream.close();

    do{
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        cout<<"Value of N?"<< endl;
        cin>>NGRAMS;
    }while(cin.fail());

    do{
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        cout<<"\nNumber of words to generate (0 to quit) ?"<<endl;
        cin>>numWords;
    }while(cin.fail());

    if(numWords==0){
        exit(0);
    }
    creatMap(NGRAMS,filePath);
}

int getRandomInt(int size){
    srand((int)time(0));
    int r = rand() % size;
    return r;
}

void writePassage(){
    string passage="...";
    int mapSize=wordsMap.size();

    map<vector<string>,vector<string>>::iterator mapIter=wordsMap.begin();
    advance(mapIter, getRandomInt(mapSize));
    pair<vector<string>,vector<string>> currentPair=*mapIter;
    vector<string> currentKeys=currentPair.first;
    vector<string> currentValues=currentPair.second;
    for(signed int i=0;i<currentKeys.size();i++){
        passage+=currentKeys[i]+" ";
    }

    string value;
    for(signed int i=0;i<numWords;i++){
        if(mapIter!=wordsMap.end()){
            value=currentValues[getRandomInt(currentValues.size())];
            passage+=value+" ";
            currentKeys.erase(currentKeys.begin());
            currentKeys.push_back(value);
            mapIter=wordsMap.find(currentKeys);
            if(mapIter!=wordsMap.end()){
                currentPair=*mapIter;
                currentValues=currentPair.second;
            }
        }
    }

    passage+="...";

    cout<<passage<<endl;

}

int main()
{
    getDataFromConsole();
    writePassage();
    return 0;
}
