#include <contiki.h>
#include <button-sensor.h>
#include <debug-uart.h>
#include <stdio.h>

#include "sensors-test.h"

PROCESS(sensors_test_process, "Sensors test process");

PROCESS_THREAD(sensors_test_process, ev, data)
{
	struct sensors_sensor *sensor;

	PROCESS_BEGIN();

	while (1)
	{
		PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event);

		sensor = (struct sensors_sensor *)data;

		if (sensor == &button_sensor)
		{
			if (sensor->value(0) == BUTTON_SHORT_PRESS)
			{	
				printf("Short button press\n");
			}
			else if (sensor->value(0) == BUTTON_LONG_PRESS)
			{
				printf("Long button press\n");
			}
			else
			{
				printf("Button released, shouldn't happen\n");
			}
		}
	}

	PROCESS_END();
}

