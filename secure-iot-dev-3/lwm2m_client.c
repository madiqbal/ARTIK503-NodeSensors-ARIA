/*
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>

#include <shell/tash.h>
#include <artik_module.h>
#include <artik_platform.h>
#include <artik_loop.h>
#include <artik_lwm2m.h>
#include <artik_log.h>

#include <tls/debug.h>

#include "lwm2m_common.h"

#define URI_MAX_LEN	128
#define UUID_MAX_LEN	64
#define DATA_MAX_LEN	1024
#define MAX_PACKET_SIZE 1024
#define MAX_LONG        0x7FFFFFFF

static artik_lwm2m_module *lwm2m;
static artik_lwm2m_handle client_h;

static artik_ssl_config ssl_config;
static artik_lwm2m_config config;

static int g_quit;
static bool akc_verify_peer = false;
static char akc_uri[URI_MAX_LEN] = "coaps://ec2-15-164-212-182.ap-northeast-2.compute.amazonaws.com:5684";
//static char akc_uri[URI_MAX_LEN] = "coaps://asepmas.myislab.com:5684";
static char akc_device_id[UUID_MAX_LEN] = "iot-dev-3";
static char akc_device_token[UUID_MAX_LEN] = "09874323";

unsigned char ca_file[] =
	"-----BEGIN CERTIFICATE-----\r\n"
	"MIIBqDCCAU8CBFuQf0QwCgYIKoZIzj0EAwIwXzELMAkGA1UEBhMCS1IxDjAMBgNV\r\n"
	"BAgMBUJ1c2FuMQ4wDAYDVQQHDAVCdXNhbjEOMAwGA1UECgwFSVNMQUIxETAPBgNV\r\n"
	"BAsMCEhhcmR3YXJlMQ0wCwYDVQQDDARyb290MB4XDTE4MDkwNjAxMTM0MFoXDTE5\r\n"
	"MDkwNjAxMTM0MFowXzELMAkGA1UEBhMCS1IxDjAMBgNVBAgMBUJ1c2FuMQ4wDAYD\r\n"
	"VQQHDAVCdXNhbjEOMAwGA1UECgwFSVNMQUIxETAPBgNVBAsMCEhhcmR3YXJlMQ0w\r\n"
	"CwYDVQQDDARyb290MFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEVWlj1Yfg4j6H\r\n"
	"dBvIRE9+gkxvSNZAAqOrJGgVpvodqfnQNswI9QAkCXb1dLRF2Jt6gLfcElP1X82e\r\n"
	"+AEp98c2gjAKBggqhkjOPQQDAgNHADBEAiA1ztg5KTbH+HMQ3DbZdumfsQLfAOiL\r\n"
	"SaU6BX4SOEF8qAIgHg+hp8ji6fl3VSlJ+1cGMTT8UmamNsiP3OH1vvRYsnE=\r\n"
	"-----END CERTIFICATE-----\r\n";

unsigned char crt_file[] =
	"-----BEGIN CERTIFICATE-----\r\n"
	"MIIB6DCCAY4CBFy8IKEwCgYIKoZIzj0EAwIwbjELMAkGA1UEBhMCS1IxDjAMBgNV\r\n"
	"BAgMBUJ1c2FuMQ4wDAYDVQQHDAVCdXNhbjEOMAwGA1UECgwFSVNMQUIxETAPBgNV\r\n"
	"BAsMCEhhcmR3YXJlMRwwGgYDVQQDDBNhc2VwbWFzLm15aXNsYWIuY29tMB4XDTE4\r\n"
	"MDQyMTA3NDk1M1oXDTIwMDQyMDA3NDk1M1owgY4xCzAJBgNVBAYTAktSMQ4wDAYD\r\n"
	"VQQIDAVCdXNhbjEOMAwGA1UEBwwFQnVzYW4xMTAvBgNVBAoMKEluZm9ybWF0aW9u\r\n"
	"IFNlY3VyaXR5IGFuZCBJbnRlbGxpZ2VudCBJb1QxGDAWBgNVBAsMD0NyeXB0byBI\r\n"
	"YXJkd2FyZTESMBAGA1UEAwwJaW90LWRldi0zMFkwEwYHKoZIzj0CAQYIKoZIzj0D\r\n"
	"AQcDQgAENntGVBCdl9NhysVrTAt3KsEBb7LnfnmCPCXdP7wKZJi/24RUbMHp9slZ\r\n"
	"sFBF0iI8ArEkK+Tvg8mQFsYXLuAgXTAKBggqhkjOPQQDAgNIADBFAiEA30YJT4Bw\r\n"
	"tEXNuvWj/ktWLcOC+ElpYr4EIwKvEmxNhe8CIE/kuk9YqkV2NI4DKTv3EvQd2MbU\r\n"
	"qyFf+U54MiR4Udz0\r\n"
	"-----END CERTIFICATE-----\r\n";

unsigned char key_file[] =
	"-----BEGIN PRIVATE KEY-----\r\n"
	"MIGTAgEAMBMGByqGSM49AgEGCCqGSM49AwEHBHkwdwIBAQQgGVpIj8DyWuONada6\r\n"
	"hVPChPbnVWhnz+9M4VP98tE8Ad+gCgYIKoZIzj0DAQehRANCAAQ2e0ZUEJ2X02HK\r\n"
	"xWtMC3cqwQFvsud+eYI8Jd0/vApkmL/bhFRswen2yVmwUEXSIjwCsSQr5O+DyZAW\r\n"
	"xhcu4CBd\r\n"
	"-----END PRIVATE KEY-----\r\n";

static char uri_[URI_MAX_LEN];
static char data_[DATA_MAX_LEN];

static void prv_change_obj(char *buffer, void *user_data)
{
	artik_error result;

	command cmd;

	printf("prv_change_obj()\n");

	if(client_h == NULL){
		printf("handle == NULL\n");
	}

	prv_init_command(&cmd, buffer);

	result = prv_read_uri(&cmd, uri_);
	if (result != S_OK)
		goto syntax_error;
	printf("URI: %s\n", uri_);
	result = prv_read_data(&cmd, data_);

	if (result != S_OK)
		goto syntax_error;

	result = lwm2m->client_write_resource(client_h, uri_,
			(unsigned char *)data_,
			strlen(data_));
	if (result != S_OK)
		log_err("client change object failed (%s)", error_msg(result));
	else
		fprintf(stdout, "OK");

	return;

syntax_error:
	fprintf(stdout, "Syntax error !");
}

void prv_read_obj(char *buffer, void *user_data)
{
	artik_error result;

	int len = 256;
	command cmd;

	printf("prv_read_obj()\n");

	if(client_h == NULL){
		printf("handle == NULL\n");
	}

	prv_init_command(&cmd, buffer);

	result = prv_read_uri(&cmd, uri_);
	if (result != S_OK)
		goto syntax_error;

	result = lwm2m->client_read_resource(client_h, uri_, (unsigned char *)data_,
							&len);
	if (result != S_OK) {
		log_err("read change object failed (%s)", error_msg(result));
		return;
	}

	data_[len] = '\0';
	fprintf(stdout, "URI: %s - Value: %s\r\n> ", uri_, data_);

	return;

syntax_error:
	fprintf(stdout, "Syntax error !");
}

static void test_serialization(artik_lwm2m_handle handle)
{
	int test_int[2] = {0, 1};
	char *test_str[2] = {"192.168.1.27", "192.168.1.67"};
	artik_error res = S_OK;
	unsigned char *buffer_int = NULL, *buffer_str = NULL;
	int len_int = 0, len_str = 0;

	fprintf(stdout, "TEST: %s starting\n", __func__);
	res = lwm2m->serialize_tlv_int(test_int, 2, &buffer_int, &len_int);
	if (res == S_OK) {
		fprintf(stdout, "Send to 'Error Code' (/3/0/11) multiple integer [0, 1]\n");
		res = lwm2m->client_write_resource(handle, "/3/0/11", buffer_int, len_int);
		fprintf(stdout, "result of serialization int sent : %s\n", error_msg(res));
		if (buffer_int)
			free(buffer_int);
	} else
		fprintf(stdout, "Failed to serialize array of int : %s\n",
								error_msg(res));
	res = lwm2m->serialize_tlv_string(test_str, 2, &buffer_str, &len_str);
	if (res == S_OK) {
		fprintf(stdout, "Send to 'Address' (/4/0/4) multiple string ['192.168.1.27', '192.168.1.67']\n");
		res = lwm2m->client_write_resource(handle, "/4/0/4", buffer_str,
								len_str);
		fprintf(stdout, "result of serialization string sent : %s\n",
								error_msg(res));
		if (buffer_str)
			free(buffer_str);
	} else
		fprintf(stdout, "Failed to serialize array of string : %s\n",
								error_msg(res));
}

static void prv_quit(char *buffer, void *user_data)
{
	artik_lwm2m_handle handle = (artik_lwm2m_handle) user_data;
	g_quit = 1;

	printf("prv_quit()\n");

	test_serialization(handle);
}

static struct command_desc_t commands[] = {
	{ "change", "Change the value of a resource.", NULL, prv_change_obj, NULL },
	{ "read", "Read the value of a resource", NULL, prv_read_obj, NULL },
	{ "q", "Quit the client.", NULL, prv_quit, NULL },
	{ NULL, NULL, NULL, NULL, NULL }
};

static void on_error(void *data, void *user_data)
{
	artik_error err = (artik_error)(intptr_t)data;

	fprintf(stdout, "LWM2M error: %s\r\n", error_msg(err));
}

static void on_execute_resource(void *data, void *user_data)
{
	char *uri = (char *)(((artik_lwm2m_resource_t *)data)->uri);

	fprintf(stdout, "LWM2M resource execute: %s\r\n", uri);
}

static void on_changed_resource(void *data, void *user_data)
{
	artik_lwm2m_resource_t *res = (artik_lwm2m_resource_t *)data;
	char *uri = (char *)res->uri;

	fprintf(stdout, "LWM2M resource changed: %s", uri);
	if (res->length > 0) {
		char *buffer = strndup((char *)res->buffer, res->length);

		fprintf(stdout, " with buffer : %s\r\n", buffer);
	} else {
		fprintf(stdout, "\r\n");
	}
}

artik_error test_lwm2m_default(void)
{
	static artik_error ret = S_OK;
	char *ips[2] = {"192.168.1.27", NULL};
	char *routes[2] = {"192.168.1.1", NULL};

	int i = 0;
	int watch_id;

	fprintf(stdout, "TEST: %s starting\n", __func__);

	memset(&config, 0, sizeof(config));
	memset(&ssl_config, 0, sizeof(ssl_config));
	config.server_id = 123;
	config.server_uri = akc_uri;
	config.name = akc_device_id;
	config.tls_psk_identity = akc_device_id;
	config.tls_psk_key = akc_device_token;
	config.connect_timeout = 1000;
	config.lifetime = 60;
	config.ssl_config = &ssl_config;

	if (akc_verify_peer)
		ssl_config.verify_cert = ARTIK_SSL_VERIFY_REQUIRED;

	ssl_config.ca_cert.data = ca_file;
	ssl_config.ca_cert.len = strlen(ca_file) + 1;

	ssl_config.client_cert.data = crt_file;
	ssl_config.client_cert.len = strlen(crt_file) + 1;

	ssl_config.client_key.data = key_file;
	ssl_config.client_key.len = strlen(key_file) + 1;


	fprintf(stdout, "TEST: %s akc_verify_peer=%d\n", __func__, akc_verify_peer);
	fprintf(stdout, "TEST: %s uri=%s\n", __func__, config.server_uri);
	fprintf(stdout, "TEST: %s id=%s\n", __func__, config.tls_psk_identity);
	fprintf(stdout, "TEST: %s key=%s\n", __func__, config.tls_psk_key);

	/* Fill up objects */
	config.objects[ARTIK_LWM2M_OBJECT_FIRMWARE] =
			lwm2m->create_firmware_object(true, "Artik", "1.0");
	config.objects[ARTIK_LWM2M_OBJECT_CONNECTIVITY_MONITORING] =
			lwm2m->create_connectivity_monitoring_object(0, 0, 12, 1, 2,
						(const char **)ips,
						2, (const char **)routes, 0,
						"SAMI2_5G",
						2345, 189, 33);
	config.objects[ARTIK_LWM2M_OBJECT_DEVICE] =
			lwm2m->create_device_object("ISLAB", "IoT-1", "1234567890",
										"1.0", "1.0", "1.0", "HUB", 0,
										5000, 1500, 100, 1000000, 200000,
										"Korea/Busan", "+07:00", "U");

	ret = lwm2m->client_request(&client_h, &config);
	if (ret != S_OK)
		goto exit;

//	test_serialization(client_h);

	for (i = 0; commands[i].name != NULL; i++)
		commands[i].user_data = (void *) client_h;

	lwm2m->set_callback(client_h, ARTIK_LWM2M_EVENT_ERROR, on_error,
			(void *)client_h);
	lwm2m->set_callback(client_h, ARTIK_LWM2M_EVENT_RESOURCE_EXECUTE,
			on_execute_resource,
			(void *)client_h);
	lwm2m->set_callback(client_h, ARTIK_LWM2M_EVENT_RESOURCE_CHANGED,
			on_changed_resource,
			(void *)client_h);

	ret = lwm2m->client_connect(client_h);
	if (ret != S_OK)
		goto exit;


	fprintf(stderr,"START1..\n");

exit:
	if (lwm2m)
		artik_release_api_module(lwm2m);
	return ret;
}

int lwm2m_cmd(int argc, char *argv[]){
	int i = 2;

	if(argc > 1){
		while(i < argc){
			strcat(argv[1], " ");
			strcat(argv[1], argv[i]);
			i++;
		}

		printf("\n%s\n",argv[1]);
		handle_command(commands, argv[1]);
	}

	return 0;
}

int lwm2m_main(int argc, char *argv[])
{
	int opt;
	artik_error ret = S_OK;


	setup_time();
//	StartWifiConnection();

	if (!artik_is_module_available(ARTIK_MODULE_LOOP)) {
		fprintf(stdout,
				"TEST: Loop module is not available,"\
				" skipping test...\n");
		return -1;
	}

	if (!artik_is_module_available(ARTIK_MODULE_LWM2M)) {
		fprintf(stdout,
				"TEST: LWM2M module is not available,"\
				" skipping test...\n");
		return -1;
	}

	lwm2m = (artik_lwm2m_module *) artik_request_api_module("lwm2m");

	ret = test_lwm2m_default();

	tash_cmd_install("lw", lwm2m_cmd, TASH_EXECMD_ASYNC);

	return (ret == S_OK) ? 0 : -1;
}


