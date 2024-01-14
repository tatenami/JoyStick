#include <fcntl.h>
#include <unistd.h>
#include <linux/joystick.h>
#include <linux/types.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

void Init(unsigned char BData[], signed int SData[]){
    int a;
    for(a = 0; a < 15; a++){
        BData[a] = 0;
        if(a < 6){
            SData[a] = 0;
        }
    }
}

int main(){
    int fd = open("/dev/input/js0",O_RDONLY);
    struct js_event JS;
    int a,stick;
    char on[3] = "on", off[5] = "off";
    unsigned char BData[17];
    signed int SData[6];
    if(read(fd, &JS, sizeof(JS)) == -1){
        printf("Error");
        return 1;
    }
    Init(BData,SData);

    while(1){
        if(read(fd, &JS, sizeof(struct js_event)) >= sizeof(struct js_event)){
            switch(JS.type & 0x7f){
                case JS_EVENT_BUTTON:
                BData[JS.number] = JS.value;
                    switch(JS.value){
                    case 0:
                        printf("[change] button:%d state:%s\n",JS.number,off);
                        break;
                    case 1:
                        printf("[change] button:%d state:%s\n",JS.number,on);
                        break;
                    }
                    for(a = 0; a < 17; a++){
                        printf("buttun:%d = %d\n",a,BData[a]);
                    }
                    printf("\n");                   
                    break;
                case JS_EVENT_AXIS:
                SData[JS.number] = JS.value;
                    stick = JS.value;
                    if(JS.number == 1 || JS.number == 4){
                        stick = -stick;
                    }
                    if(JS.number == 2 || JS.number == 5){
                        break;
                    }
                    printf("[change] axis:%d value:%d\n",JS.number,stick);
                    for(a = 0; a < 6; a++){
                        printf("axis:%d = %d\n",a,SData[a]);
                    }
                    printf("\n");
                    break;
            }
        }
    }
    return 0;
}