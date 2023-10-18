#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

static float getTemp(const char* const devName);
static void displayTemp(const char* const devName, float value);

int main(int argc, char *argv[])
{
    if (argc < 3)
	{
		printf("Device file NOT found\n");
		return 1;
	}
	
    float temperature = getTemp(argv[1]);
	printf("Temperature = %.2f C\n", temperature);

	displayTemp(argv[2], temperature);

	return 0; 
}

float getTemp(const char* const devName)
{
    int fd;  
    uint8_t result[2] = {0};  
    uint8_t high = 0;  
    uint8_t low = 0; 
	float value = 0.0; 

    fd = open(devName, 0);  
    if (fd >= 0) 
    {
        int i = read(fd, &result, sizeof(&result));
        if (i >= 0)
        {
            high = result[1];
            low = result[0];
			
			value = ((high << 8 ) + low) * 0.0625;
        }  
        close(fd);
    }
    return value;
}

static void displayTemp(const char* const devName, float value)
{
	char msg[13];
	char buf[6];
	sprintf(buf, "%f", value);
	
	msg[0] = 'T';
	msg[1] = 'e';
	msg[2] = 'm';
	msg[3] = 'p';
	msg[4] = '=';
	msg[5] = buf[0];
	msg[6] = buf[1];
	msg[7] = '.';
	msg[8] = buf[3];
	msg[9] = buf[4];
	msg[10] = ' ';
	msg[11] = 'C';

	int fd = open(devName, O_RDWR);
	if (fd >= 0)
	{
		write(fd, &msg, sizeof(msg));
		close(fd);
	}
}
