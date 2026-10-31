#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#include <stdio.h>

 
typedef struct Numbers{
     char    *key;
     char    *value;
}Numbers;

void    put_str(char *str)
{
    int i;
    if (!str)
        return ;
    i = 0;
    while (str[i] != '\0')
    {
        write(1, &str[i], 1);
        i++;
    }
}

int compare_str(char *str1, char *str2)
{
    int i;

    i = 0;
    while (str1[i] && str2[i])
    {
        if (str1[i] != str2[i])
            return (0);
        i++;
    }
    return (str1[i] == '\0' && str2[i] == '\0');
}

char *find_value(Numbers *ob, char *input)
{
    int i;

    i = 0;
    while (ob[i].key != NULL)
    {
        if (compare_str(ob[i].key, input))
            return (ob[i].value);
        i++;
    }
    return (NULL);
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

void run(struct Numbers *ob, char* input){

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

int ft_strlen(char* str)
{
    int i;
    i = 0;
    while(str[i] != '\0')
    {
        // printf("%c\n", str[i]);
        ++i;

    }
    return (i);    
}

int power_of_ten(int power)
{
    int result;
    result = 1;
    while(power >= 1)
    {
        result *= 10;
        --power;
    }
    return (result);
}

char *get_number_with_zeros(char num, int zero_count)
{
    char *str;
    int i;

    // printf("Received digit: %c\n", num);
    // printf("Received zero_count: %d\n", zero_count);

    str = malloc(zero_count + 2);
    if (!str)
        return (NULL);

    str[0] = num;
    i = 1;

    while (i <= zero_count)
    {
        str[i] = '0';
        // printf("Filled one zero\n");
        i++;
    }

    str[i] = '\0';

    // printf("Final string: [%s]\n", str);
    return (str);
}

int print_number_in_letters(Numbers *ob, char *num_str, int count1)
{
    int count;
    int c;
    int j;
    int index;
    char *passable_num;
    char *first_num;
    char *printable_num;

    if(count1 == -1){
        count = ft_strlen(num_str);
        c = count;
    }else{
        count = count1;
        c = count;
    }

    while (c > 0)
    {
        index = count - c;
        if (num_str[index] == '0')
        {
            --c;
            continue;
        }
        if (c <= 2)
        {
            if (c == 2 && num_str[index] == '1')
            {
                passable_num = malloc(3);
                if (!passable_num)
                    return (0);

                passable_num[0] = '1';
                passable_num[1] = num_str[index + 1];
                passable_num[2] = '\0';
                c--;
            }
            else
            {
                passable_num = get_number_with_zeros(num_str[index], c - 1);
                if (!passable_num)
                    return (0);
            }

            printable_num = find_value(ob, passable_num);
            if (!printable_num)
            {
                free(passable_num);
                return (0);
            }
            put_str(printable_num);
            put_str(" ");
            free(passable_num);
        }
        else if(c <= 3)
        {
            passable_num = get_number_with_zeros('1', c - 1);
            if (!passable_num)
                return (0);
            printable_num = find_value(ob, passable_num);
            first_num = get_number_with_zeros(num_str[index], 0);
            if (!first_num || !printable_num)
            {
                free(passable_num);
                free(first_num);
                return (0);
            }
            put_str(find_value(ob, first_num));
            put_str(" ");
            put_str(printable_num);
            put_str(" ");
            free(passable_num);
            free(first_num);
        }
        else
        {
            j = c % 3;
            if (j == 0)
                j = 3;

            if (!print_number_in_letters(ob, &num_str[index], j))
                return (0);

            c -= j;

            if (c > 0)
            {
                passable_num = get_number_with_zeros('1', c);
                if (!passable_num)
                    return (0);

                printable_num = find_value(ob, passable_num);
                if (!printable_num)
                {
                    free(passable_num);
                    return (0);
                }

                put_str(printable_num);
                put_str(" ");
                free(passable_num);
            }
            continue;
        }
        c--;
    }
    return (1);
}

int valid_input(char *str){
    int i;

    if(str[0] == '0')
        return 0;

    i = 0;
    while(str[i] != '\0'){
        if(!(str[i] >= '0' && str[i] <= '9')){
            return 0;
        }
        i++;
    }
    return 1;
}

int main(int argc, char **argv)
{
    // printf("%s\n","Start");
    Numbers *Num;
    int fd;
    char *dict_file;
    char *input;
    if (argc < 2) 
    return (put_str("Error\n"), 1);
    else if (argc < 3)
    {
        dict_file = "numbers.dict";
        input = argv[1];
        // printf("%s\n", "Dict file assigned");
    }
    else if (argc < 4)
    {
        //use first arg as dict
        dict_file = argv[1];
        input = argv[2];
    }
    else
    return (put_str("Error\n"), 1);
    
    // printf("%s\n","Input validation");
    if(!valid_input(input)){
        put_str("Argument error\n");
        return -1;
    }
    
    fd = open(dict_file, O_RDONLY);
    if (fd < 0)
        return (1);
    Num = malloc(sizeof(Numbers) * 100);
    if (!Num){
        close(fd);
        return (1);
    }
    // printf("%s\n","Got numbers array");
    dict_reader(Num, fd);
    // printf("%s\n","Filled numbers array");
    int i = print_number_in_letters(Num, input,-1);
    // printf("%s\n","Printed");

    free(Num);
    close(fd);


    return (0);

}
