#include <iostream>
#include <libusb.h>
#include <set>
#include <vector>
#include <unordered_map>
#include <curses.h>

#include "UltrafineDisplay.h"

using std::set;
using std::unordered_map;
using std::vector;
const uint16_t LG_ID_Vendor = 0x43e;

unordered_map<uint16_t, char *> support_device = {
	{0x9a63, "24MD4KL"},
	{0x9a70, "27MD5KL"},
	{0x9a40, "27MD5KA"}};

static int get_lg_ultrafine_usb_devices(libusb_device **devs, int usb_cnt, libusb_device ***lg_devs)
{
	int lg_cnt = 0;
	struct libusb_device_descriptor desc;

	for (int i = 0; i < usb_cnt; i++)
	{
		int r = libusb_get_device_descriptor(devs[i], &desc);
		if (desc.idVendor != LG_ID_Vendor)
			continue;
		auto lg_search = support_device.find(desc.idProduct);
		if (lg_search != support_device.end())
		{
			lg_cnt++;
		}
	}
	if (lg_cnt == 0)
		return 0;
	*lg_devs = (libusb_device **)malloc(sizeof(libusb_device *) * lg_cnt);

	int k = 0;
	for (int i = 0; i < usb_cnt; i++)
	{
		int r = libusb_get_device_descriptor(devs[i], &desc);
		auto lg_search = support_device.find(desc.idProduct);
		if (lg_search != support_device.end())
		{
			(*lg_devs)[k++] = devs[i];
		}
	}

	return lg_cnt;
}

void manually_set_brightness(int lg_cnt, libusb_device **lgdevs)
{
	int end = false;
	while (!end)
	{
		printw("Get %d UltraFine display\n", lg_cnt);
		vector<UltrafineDisplay> Display_pool;
		for (int i = 0; i < lg_cnt; i++)
		{
			Display_pool.emplace_back(lgdevs[i]);
			printw("Find Ultrafine Display %s brightness %d. [press %d to choose]\n", Display_pool[i].getDisplayName(), Display_pool[i].get_brightness_level(), i);
		}
		char c = getch();
		if (c == 'q')
		{
			printw("Press q to exit\n");
			break;
		}
		int chooseDisplay = atoi(&c);
		clear();
		Display_pool[chooseDisplay].interactive();
		for (auto &a : Display_pool)
		{
			a.LG_Close();
		}
	}
}

void set_brightness_with_argv(int lg_cnt, libusb_device **lgdevs, char *argv_1)
{
	int input_volume = atoi(argv_1);
	uint8_t brightnessVolume;
	brightnessVolume = input_volume > 100 ? 100 : input_volume;
	vector<UltrafineDisplay> Display_pool;
	// we set all display's brightness to certian value
	for (int i = 0; i < lg_cnt; i++)
	{
		Display_pool.emplace_back(lgdevs[i]);
		Display_pool[i].set_brightness_level(input_volume);
	}
	printf("successfully set brightness to %d\n", input_volume);
}

int main(int argc, char *argv[])
{
	initscr();
	noecho();
	cbreak();

	libusb_device **devs;
	libusb_context *ctx = NULL;
	int r;
	int cnt;

	r = libusb_init(&ctx);
	if (r < 0)
	{
		printw("Unable to initialize libusb.Exitting\n");
		getch();
		endwin();
		return r;
	}

	cnt = libusb_get_device_list(ctx, &devs);
	if (cnt < 0)
	{
		printw("Unable to get USB device list.Exitting\n");
		getch();
		endwin();
		return -1;
	}

	int lg_cnt = 0;
	libusb_device **lgdevs;
	lg_cnt = get_lg_ultrafine_usb_devices(devs, cnt, &lgdevs);
	// lg_cnt = 2; // test
	if (lg_cnt == 0)
	{
		printw("Unable to get LG screen device.\n");
		printw("Please comfirm there is a UltraFine display here.\n");
		printw("Maybe your need restart your display to active thunderbolt USB\n");
		getch();
		endwin();
		return -2;
	}
	if (argc == 1)
	{
		manually_set_brightness(lg_cnt, lgdevs);
	}
	else if (argc == 2)
	{
		set_brightness_with_argv(lg_cnt, lgdevs, argv[1]);
	}
	else
	{
		printw("invalid params");
	}
	endwin();
	return 0;
}
