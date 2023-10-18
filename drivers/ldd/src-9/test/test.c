#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFF_SIZE	10

static int getTemperature(const char* const dev, char* const buff, int buffSize);

int main(int argc, char* argv[])
{
	char tempBuff[BUFF_SIZE];

	if (argc < 2)
	{
		printf("Device file not found\n");
		return -1;
	}

	if (getTemperature(argv[1], tempBuff, sizeof(tempBuff)) == 0)
	{
		printf("Temperature: %s", tempBuff);
	}

	return 0;
}

static int getTemperature(const char* const dev, char* const buff, int buffSize)
{
	int fd, ret = -1;
	fd = open(dev, O_RDWR);
	if (fd >= 0)
	{
		int res = read(fd, buff, buffSize);
		if (res > 0)
		{
			ret = 0;
		}
		else 
		{
			ret = -2;
		}
		close (fd);
	}

	return ret;
}
