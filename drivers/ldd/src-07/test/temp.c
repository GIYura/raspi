#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <stdint.h>

static float getTemp(const char* const dev);

int main(int argc, char *argv[])
{
    if (argc < 2)
	{
		printf("Device file NOT found\n");
		return 1;
	}
	
    float temperature = getTemp(argv[1]);
	printf("Temperature = %.2f C\n", temperature);

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
