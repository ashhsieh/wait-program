#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include "Wait.h"

Wait::Wait(int argc, char **argv)
    : POSIXApplication(argc, argv)
{
    parser().setDescription("Wait execution for some time");
    parser().registerPositional("PROCESS", "Wait execution for the given number of seconds");
}

Wait::~Wait()
{
}

Wait::Result Wait::exec()
{
    int sec = 0;

    // Convert input to seconds
    if ((sec = wait(arguments().get("PROCESS"))) == -1)
    {
        ERROR("invalid process `" << arguments().get("PROCESS") << "'");
        return InvalidArgument;
    }

    // Wait now
    if (waitid(P_PID, (pid_t)sec, WEXITED) != 0)
    {
        ERROR("failed to wait: " << strerror(errno));
        return IOError;
    }

    // Done
    return Success;
}