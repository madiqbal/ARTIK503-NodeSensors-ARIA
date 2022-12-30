/*---------------------------------------------------------------------------*/
/*                                                                           */
/* WARN: Please don't modify this default file!!!                            */
/*                                                                           */
/*---------------------------------------------------------------------------*/
#include <tinyara/config.h>
#include <stdio.h>
#include <unistd.h>
#ifdef CONFIG_TASH
#include <shell/tash.h>
#endif

#ifndef APP_PRIORITY
#define APP_PRIORITY 100
#endif

#ifndef APP_STACKSIZE
#define APP_STACKSIZE 2048
#endif

extern int mqtt_main(int argc, char *argv[]);
extern int lwm2m_main(int argc, char *argv[]);


int _main(int argc, char *argv[])
{
	int pid;

	StartWifiConnection();

	sleep(1);

	/*
	 * Create Task For async job
	 */
	pid = task_create("lwm2m_main", APP_PRIORITY, APP_STACKSIZE, lwm2m_main, argv);
	if (pid < 0) {
		/*
		 * Error : Can't Create task
		 */
		printf("lwm2m is not started, err = %d\n", pid);
	}

	sleep(10);

	/*
	 * Create Task For async job
	 */
	pid = task_create("mqtt_main", 50, APP_STACKSIZE, mqtt_main, argv);
	if (pid < 0) {
		/*
		 * Error : Can't Create task
		 */
		printf("hello is not started, err = %d\n", pid);
	}

	return 0;
}
