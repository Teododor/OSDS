/* Create, Delete & Count Access files in lab3, respectively lab3 -> lab31*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <unistd.h> 
#include <string.h>

#define EVENT_SIZE (sizeof(struct inotify_event))
#define EVENT_BUF_LEN (1024 * (EVENT_SIZE + 16))

// HERE WE WILL SEE HOW MANY TIMES THE FILE WAS MODIFIED/ACCESSED
struct filename_counter
{
    char filename[50];
    int nr;
} v[30] ;



int main()
{
    int length, i=0, last_file = 0;
    int fd;
    int wd;
    char buffer[EVENT_BUF_LEN];

    // CREATE THE INOTIFY INSTANCE
    fd = inotify_init();

    // CHECKING FOR ERROR
    if(fd<0) 
    {
        perror("inotify_init");
    }

    // ADDING THE DIRECTORY INTO WATCH LIST. HERE, THE SUGGESTION IS TO VALIDATE THE EXISTENCE OF THE DIRECTORY BEFORE ADDINBG INTO MONITORING LIST
    wd = inotify_add_watch(fd, "/home/teodor/Desktop/OSDS - finished/lab3/lab31", IN_CREATE | IN_DELETE | IN_ACCESS);

    // READ TO DETERMINE THE EVENT CHANGE HAPPENED ON THE DIRECTORY
    do
    {
        i=0;
        length = read(fd, buffer, EVENT_BUF_LEN);

        // CHECKING FOR ERROR
        if(length < 0)
        {
            perror("read");
        }

        // ITERATING OVER ALL THE EVENTS IN BUFFER
        while(i<length)
        {
            struct inotify_event *event = (struct inotify_event *) &buffer[i];

            if(event->len)
            {
                if(event->mask & IN_CREATE)
                {
                    if(event->mask & IN_ISDIR)
                    {
                        printf("New directory %s created.\n", event->name);
                    }
                    else
                    {
                        printf("New file %s created.\n", event->name);
                    }
                }
                else if(event->mask & IN_DELETE)
                {
                    if(event->mask & IN_ISDIR)
                    {
                        printf("Directory %s deleted.\n", event->name);
                    }
                    else
                    {
                        printf("File %s deleted.\n", event->name);
                    }
                }
                else if(event->mask & IN_ACCESS)
                {
                    int j, found_file =0;
                    for(j=0; j<30; j++)
                    {
                        if(strcmp(v[j].filename, event->name) == 0)
                        {
                            v[j].nr++;
                            found_file = 1;
                            break;
                        }
                    }

                    if(found_file == 0)
                    {
                        v[last_file + 1].nr = 1;
                        strcpy(v[last_file + 1].filename, event->name);
                    }
                    if(event-> mask & IN_ISDIR)
                    {
                        if(j<30)
                        {
                            printf("Directory %s accessed (modified) %d times\n", event-> name, v[j].nr);
                        }
                        else 
                        {
                            printf("Directory %s accessed (modified) %d times\n", event-> name, v[last_file].nr);
                        }
                    }
                    else 
                    {
                        if(j<30)
                        {
                            printf("File %s accessed (modified) %d times\n", event-> name, v[j].nr);
                        }
                        else 
                        {
                            printf("File %s accessed (modified) %d times\n", event-> name, v[last_file].nr);
                        }
                    }
                    i += EVENT_SIZE + event->len;
                }
            }
        }
    }while(1);

    // REMOVING THE DIRECTORY FROM THE WATCH LIST
    inotify_rm_watch(fd, wd);

    // CLOSING THE INOTIFY INSTANCE
    close(fd);
    
};