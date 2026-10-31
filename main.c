#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#include <stdio.h>

 
typedef struct Numbers{
     char    *key;
     char    *value;
}Numbers;

void    print_error(char *str){
	int i;

	i = 0;
    while (str[i] != '\0' ){
        write(1,&str[i],1);
        i++;
    }
}



int parse(struct Numbers *ob, char *buf){
    int i;
    int j;
    int k;

    i = 0;
    j = 0;
    while (buf[i] != '\0'){
        while (buf[i] == '\n')
            i++;
        if (buf[i] == '\0')
            break ;
        ob[j].key = &buf[i];
        while (buf[i] >= '0' && buf[i] <= '9')
            i++;
        if (ob[j].key == &buf[i])
            return (0);
        k = i;
        while (buf[i] == ' ')
            i++;
        if (buf[i] != ':')
            return (0);
        buf[k] = '\0';
        i++;
        while (buf[i] == ' ')
            i++;
        ob[j].value = &buf[i];
        while (buf[i] != '\n' && buf[i] != '\0')
            i++;
        k = i;
        while (k > 0 && buf[k - 1] == ' ')
            k--;
        if (buf[i] == '\n')
            i++;
        buf[k] = '\0';
        j++;
    }
    ob[j].key = NULL;
    ob[j].value = NULL;
    return (1);
}



void dict_reader(Numbers *ob, int fd){
    int l;
    char * buffer;
 
    buffer = malloc(10000);

    if(!buffer){
        return ;
    }
    l = read(fd, buffer,10000);
    buffer[l] = '\0';
    parse(ob,buffer);
}


int main(){
    Numbers *Num;
    int fd;

    fd = open("numbers.dict", O_RDONLY);
    if (fd < 0)
        return (1);
    Num = malloc(sizeof(Numbers) * 100);
    if (!Num){
        close(fd);
        return (1);
    }
    dict_reader(Num, fd);

    int i = 0;
    while (Num[i].key != NULL){
        printf("%s %s",Num[i].key, Num[i].value);
        i++;
    }

    free(Num);
    close(fd);


    return (0);

}
