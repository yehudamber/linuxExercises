#include <perror.h>

#include <unistd.h>

constexpr auto fd = 3;

int main()
{
    ::close(fd); // verifying that fd isn't used
    if(::write(fd, "", 1) < 0)
    {
        perror("write failed");
    }
}
