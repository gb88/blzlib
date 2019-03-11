#include <stdlib.h>

#include "blzlib.h"
#include "blzlib_log.h"

int main(int argc, char** argv)
{
	blz* blz = NULL;
	blz_dev* dev = NULL;
	blz_char* rch = NULL;
	char buf[20];

	if (argv[1] == NULL) {
		LOG_ERR("Pass MAC address of device to connect to");
		return EXIT_FAILURE;
	}

	/* Initialize bluetooth adapter and context */
	blz = blz_init("hci0");
	if (!blz)
		goto exit;

	/* Connect to device with MAC address */
	LOG_INF("Connecting to %s...", argv[1]);
	dev = blz_connect(blz, argv[1], NULL);
	if (!dev)
		goto exit;

	/* List available characteristics, this is not necessary */
	char** uuids = blz_list_char_uuids(dev);
	for (int i = 0; uuids != NULL && uuids[i] != NULL; i++) {
		LOG_INF("Characteristic UUID %s", uuids[i]);
	}

	/* Find characteristic for manufacturer name */
	rch = blz_get_char_from_uuid(dev, "00002a29-0000-1000-8000-00805f9b34fb");
	if (!rch)
		goto exit;

	/* Read manufacturer name string */
	int len = blz_char_read(rch, buf, 20);
	LOG_INF("Manufacturer Name: '%.*s'", len, buf);

exit:
	/* Disconnect and free resources associated to device */
	blz_disconnect(dev);

	/* Free resources associated with context */
	blz_fini(blz);

	return EXIT_SUCCESS;
}
