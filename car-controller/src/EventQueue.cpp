#include "EventQueue.h"

#include <linux/input.h>

#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <getopt.h>
#include <ctype.h>
#include <signal.h>
#include <limits.h>
#include <sys/time.h>
#include <sys/types.h>

#include <stdexcept>
#include <string>

#define FILENAME "/dev/input/event2"

EventQueue::EventQueue()
{
    if ((fd = open(FILENAME, O_RDONLY)) < 0) {
        throw std::runtime_error("Failed to open input device file: " + std::string(strerror(errno)));
    }
}

EventQueue::~EventQueue()
{
    close(fd);
}

Event EventQueue::getEvent()
{
    input_event ev;
    int rd;

    rd = read(fd, &ev, sizeof(ev));
    if (rd < (int)sizeof(ev))
    {
        throw std::runtime_error("Error reading event, expected " + std::to_string(sizeof(ev)) + " bytes, got " + std::to_string(rd) + '\n');
    }

    Event event;
    if (ev.type == EV_SYN)
    {
        event = getEvent();
    }
    else
    {
        event.button = ev.code - BTN_0;
        event.pressed = ev.value;
    }

    return event;
}