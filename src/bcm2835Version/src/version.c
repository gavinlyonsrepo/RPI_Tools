// Example program for bcm2835 library
// print put current version

#include <bcm2835.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    if (!bcm2835_init())       return 1;
    printf("bcm2835 library version :%u\r\n" ,bcm2835_version());
    bcm2835_close();
    return 0;
}

