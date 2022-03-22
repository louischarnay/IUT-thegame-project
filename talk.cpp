#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include "talk.h"

using namespace std;
vector<int> sockets;

void talk(Talk &param) {
    char msg;
    int err;

    cout << param.getReader() << "-" << param.getWriter() << endl;

    //parcours vector + add new writers
    bool isExisting = false;
    for (int i = 0; i < sockets.size(); ++i) {
        if(sockets.at(i) == param.getReader()){
            isExisting = true;
        }
    }
    if(!isExisting){
        sockets.push_back(param.getReader());
    }

    for(;;)
    {
        err= read(param.getReader(), &msg, 1);
        if(err <= 0)
            break;

        if(param.getWriter() == -1){
            //envoie à tous les writers sauf le reader
            for (int i = 0; i < sockets.size(); ++i) {
                if(sockets.at(i) != param.getReader()){
                    err= write(sockets.at(i), &msg, 1);
                    if(err <= 0)
                        break;
                }
            }
        } else {
            err= write(param.getWriter(), &msg, 1);
            if(err <= 0)
                break;
        }

    }

    return;
}
