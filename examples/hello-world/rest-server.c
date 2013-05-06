#include <contiki.h>
#include <contiki-net.h>
#include <rest.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dev/leds.h>
#include <debug-uart.h>

static char temp[100];

RESOURCE(helloworld, METHOD_GET, "helloworld");
void
helloworld_handler(REQUEST *request, RESPONSE *response)
{
	sprintf(temp, "Hello, world!\n");

	rest_set_header_content_type(response, TEXT_PLAIN);
	rest_set_response_payload(response, (uint8_t*)temp, strlen(temp));
}

RESOURCE(turnredled, METHOD_POST, "turnledred");
void
turnredled_handler(REQUEST *request, RESPONSE *response)
{
	char color[10];
	uint8_t success = 0;

	printf("I'm in POST!\n");

	if(rest_get_post_variable(request, "color", color, 10))
	{
		printf(color);
		if (!strcmp(color, "red"))
		{
			leds_toggle(LEDS_RED);
			success = 1;
		}
	}

	if (!success)
	{
		rest_set_response_status(response, BAD_REQUEST_400);
	}
}

PROCESS(rest_server, "Rest server");

PROCESS_THREAD(rest_server, ev, data)
{
	PROCESS_BEGIN();

	rest_init();

	rest_activate_resource(&resource_helloworld);
	rest_activate_resource(&resource_turnredled);

	PROCESS_END();
}

