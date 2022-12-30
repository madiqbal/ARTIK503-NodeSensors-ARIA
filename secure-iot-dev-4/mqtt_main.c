
#include <tinyara/config.h>
#include <stdio.h>
#include <time.h>

#include <errno.h>
#include <fcntl.h>
#include <tinyara/analog/adc.h>
#include <tinyara/analog/ioctl.h>

#include <artik_module.h>
#include <artik_time.h>
#include <artik_gpio.h>

#define SW702 	42
#define SW703 	44
#define LED702 	49
#define LED703	45

char *cmd_connect[] = {"","mqtt","connect","mqtts://ec2-54-234-178-65.compute-1.amazonaws.com","8883","iot-dev-3",""};
//char *cmd_connect[] = {"","mqtt","connect","mqtts://asepmas.myislab.com","8883","asep","09874321"};

char *cmd_publish_ec2[] = {"","mqtt","publish","/ec2/h1/r1/gas",""};
char *cmd_publish_aws[] = {"","mqtt","publish","/aws/h1/r1/gas",""};

static char **cmd_publish = cmd_publish_ec2;

static int hold = 0;

static artik_list *requested_node;

struct gpio_node {
	artik_list node;
	artik_gpio_handle handle;
};

static int gpio_io(artik_gpio_dir_t dir, artik_gpio_id id, int new_value)
{
	artik_gpio_module *gpio;
	artik_gpio_config config;
	artik_gpio_handle handle;
	char name[16] = "";
	int ret = 0;

	gpio = (artik_gpio_module *)artik_request_api_module("gpio");
	if (!gpio) {
		fprintf(stderr, "GPIO module is not available\n");
		return -1;
	}

	memset(&config, 0, sizeof(config));
	config.dir = dir;
	config.id = id;
	snprintf(name, 16, "gpio%d", config.id);
	config.name = name;

	if (gpio->request(&handle, &config) != S_OK) {
		fprintf(stderr, "Failed to request GPIO %d\n", config.id);
		ret = -1;
		goto exit;
	}

	if (dir == GPIO_OUT) {
		ret = gpio->write(handle, new_value);
		if (ret != S_OK) {
			fprintf(stderr, "Failed to write GPIO %d [err %d]\n", config.id, ret);
		} else {
			fprintf(stdout, "Write %d to GPIO %d\n", new_value, config.id);
		}
	} else {
		ret = gpio->read(handle);
		if (ret < 0) {
			fprintf(stderr, "Failed to read GPIO %d [err %d]\n", config.id, ret);
		} else {
			fprintf(stdout, "The value read in GPIO %d is %d\n", config.id, ret);
		}
	}

	gpio->release(handle);

exit:
	artik_release_api_module(gpio);
	return ret;
}

static void gpio_event(void *user_data, int value)
{
	unsigned int id = (unsigned int)user_data;
	hold = 1;
	if(id == SW702){
		fprintf(stdout, "Select topic for EC2\n");
		cmd_publish = cmd_publish_ec2;
		gpio_io(GPIO_OUT, LED702, 1);
		gpio_io(GPIO_OUT, LED703, 0);
	}
	else if (id == SW703){
		fprintf(stdout, "Select topic for AWS\n");
		gpio_io(GPIO_OUT, LED702, 0);
		gpio_io(GPIO_OUT, LED703, 1);
		cmd_publish = cmd_publish_aws;
	}
	hold = 0;
}

int gpio_watch(int num)
{

	artik_gpio_module *gpio = (artik_gpio_module *)
					artik_request_api_module("gpio");

	artik_gpio_config config;
	artik_gpio_handle handle;
	char name[16] = "";
	int ret = 0;

	memset(&config, 0, sizeof(config));
	snprintf(name, 16, "gpio%d", config.id);
	config.name = name;
	config.id = num;
	config.dir = GPIO_IN;
	config.initial_value = 0;
	config.edge = GPIO_EDGE_FALLING;

	ret = gpio->request(&handle, &config);
	if (ret != S_OK) {
		fprintf(stderr, "GPIO id %u already on watching or wrong GPIO id\n", config.id);
		goto exit;
	}

	struct gpio_node *node = (struct gpio_node *)artik_list_add(&requested_node,
			(ARTIK_LIST_HANDLE)config.id, sizeof(struct gpio_node));
	if (!node)
		return E_NO_MEM;

	node->handle = handle;

	ret = gpio->set_change_callback(handle, gpio_event, (void *)config.id);
	if (ret != S_OK) {
		fprintf(stderr, "Failed, could not set GPIO change callback (%d)\n",
				ret);
		goto exit;
	}

	fprintf(stdout, "GPIO id %u on watching\n", config.id);

exit:
	artik_release_api_module(gpio);

	return ret;
}


int getADC(int fd){
	int ret = 0;
	struct adc_msg_s sample;
	size_t readsize;
	ssize_t nbytes;
	while(1){
		ret = ioctl(fd, ANIOC_TRIGGER, 0);

		readsize = sizeof(struct adc_msg_s);
		nbytes = read(fd, &sample, readsize);

		int nsamples = sizeof(struct adc_msg_s);
		if (sample.am_channel == 0) {
			ret = sample.am_data;
			break;
		}
	}
	return ret;
}

void set_time(void)
{
    artik_time date;
	artik_time_module *time_modu;

	date.month = 3;
	date.day = 28;
	date.year = 2019;
	date.hour = 1;
	date.minute = 1;
	date.second = 1;


	time_modu = (artik_time_module *)artik_request_api_module("time");
	time_modu->set_time(date,ARTIK_TIME_UTC);

    return;
}


int mqtt_main(int argc, char *argv[])
{
	int fd, tt;
	char val[5];
	fd = open("/dev/adc0", O_RDONLY);

	set_time();

	gpio_io(GPIO_OUT, LED702, 1);
	gpio_io(GPIO_OUT, LED703, 0);
	gpio_watch(SW702);
	gpio_watch(SW703);


	while(1){
		printf("Secure MQTT Application - Temperature\n");

		sleep(1);

		mqtt_cmd_connect(7, cmd_connect);

		while(1){
			sleep(2);
			if(hold)
				continue;

			sprintf(val, "%d",getADC(fd));
			cmd_publish[4] = val;
			if(mqtt_cmd_publish(5, cmd_publish) < 0){
				mqtt_cmd_disconnect(7, cmd_connect);
				break;
			}
		}
	}

	close(fd);
	return 0;
}
