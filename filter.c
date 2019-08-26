#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DISP_W 178
#define DISP_H 128
#define CRAP_AT_START_OF_FRAME 44
#define COMPLETE_FRAME_SIZE (frame_height * frame_width * 1.5)
#define WEIRD_OFFSET 5

int main(int argc, char **argv)
{
    // Read "header": First Byte=pixel width, 2nd Byte=frame height, 3rd=frame width
    // frame_width other than 178 is currently unsupported.
    unsigned char ch, step = 1, frame_height = 128, frame_width = 178;
    unsigned long checker;
    char *ptr;
    if (argc > 1)
    {
        if (strcmp(argv[1], "-h") == 0)
        {
            const char out[] = "usage: display <step> <frame height> <frame width>\n";
            write(STDOUT_FILENO, &out, 51);
            return (0);
        }
        checker = strtoul(argv[1], &ptr, 10);
        if (checker > 0xFF)
        {
            const char error[] = "argument over 255 not allowed!\n";
            write(STDERR_FILENO, &error, 31);
            return (0);
        }
        step = (unsigned char)checker;
    }
    if (argc > 2)
    {
        checker = strtoul(argv[2], &ptr, 10);
        if (checker > 0xFF)
        {
            const char error[] = "argument over 255 not allowed!\n";
            write(STDERR_FILENO, &error, 31);
            return (0);
        }
        frame_height = (unsigned char)checker;
        if (frame_height > DISP_H)
        {
            const char error[] = "W: Frame height invalid!\n";
            write(STDERR_FILENO, &error, 25);
        }
    }
    if (argc > 3)
    {
        checker = strtoul(argv[2], &ptr, 10);
        if (checker > 0xFF)
        {
            const char error[] = "argument over 255 not allowed!\n";
            write(STDERR_FILENO, &error, 31);
            return (0);
        }
        frame_width = (unsigned char)checker;
        if (frame_width > DISP_W)
        {
            const char error[] = "W: Frame height invalid!\n";
            write(STDERR_FILENO, &error, 25);
        }
    }
    //send "header" to the display driver
    write(STDOUT_FILENO, &step, 1);
    write(STDOUT_FILENO, &frame_height, 1);
    write(STDOUT_FILENO, &frame_width, 1);

    //send luminence stream to the brick, ignore chroma
    unsigned int count = 0;
    while (read(STDIN_FILENO, &ch, 1) > 0)
    {
        count++;
        if (count > COMPLETE_FRAME_SIZE + WEIRD_OFFSET)
        {
            count = 0;
            continue;
        }
        if (count < CRAP_AT_START_OF_FRAME)
            continue;
        if (count <= frame_height * DISP_W + CRAP_AT_START_OF_FRAME && (count - CRAP_AT_START_OF_FRAME) % step == 0)
        {
            write(STDOUT_FILENO, &ch, 1);
        }
    }

    return 0;
}