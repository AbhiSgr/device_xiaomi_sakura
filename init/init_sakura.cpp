/*
   Copyright (c) 2016, The CyanogenMod Project
   Copyright (c) 2019, The LineageOS Project

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <cstdlib>
#include <fstream>
#include <string.h>
#include <sys/sysinfo.h>
#include <unistd.h>

#include <android-base/logging.h>
#include <android-base/properties.h>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "property_service.h"
#include "vendor_init.h"

void property_override(char const prop[], char const value[])
{
	prop_info *pi;
	pi = (prop_info *)__system_property_find(prop);
	if (pi)
		__system_property_update(pi, value, strlen(value));
	else
		__system_property_add(prop, strlen(prop), value, strlen(value));
}

static const char *snet_prop_key[] = {
	"ro.boot.vbmeta.device_state",
	"ro.boot.verifiedbootstate",
	"ro.boot.flash.locked",
	"ro.boot.selinux",
	"ro.boot.veritymode",
	"ro.boot.warranty_bit",
	"ro.warranty_bit",
	"ro.debuggable",
	"ro.secure",
	"ro.build.type",
	"ro.build.keys",
	"ro.build.tags",
	"ro.system.build.tags",
	"ro.vendor.boot.warranty_bit",
	"ro.vendor.warranty_bit",
	"vendor.boot.vbmeta.device_state",
	"vendor.boot.verifiedbootstate",
	NULL
};

static const char *snet_prop_value[] = {
	"locked", // ro.boot.vbmeta.device_state
	"green", // ro.boot.verifiedbootstate
	"1", // ro.boot.flash.locked
	"enforcing", // ro.boot.selinux
	"enforcing", // ro.boot.veritymode
	"0", // ro.boot.warranty_bit
	"0", // ro.warranty_bit
	"0", // ro.debuggable
	"1", // ro.secure
	"user", // ro.build.type
	"release-keys", // ro.build.keys
	"release-keys", // ro.build.tags
	"release-keys", // ro.system.build.tags
	"0", // ro.vendor.boot.warranty_bit
	"0", // ro.vendor.warranty_bit
	"locked", // vendor.boot.vbmeta.device_state
	"green", // vendor.boot.verifiedbootstate
	NULL
};

static void workaround_snet_properties() {

     // Hide all sensitive props
    for (int i = 0; snet_prop_key[i]; ++i) {
        property_override(snet_prop_key[i], snet_prop_value[i]);
    }

    // Workaround SafetyNet
    workaround_snet_properties();

}

void load_dalvik_properties()
{
	struct sysinfo sys;

	sysinfo(&sys);

	if (sys.totalram > 3072ull * 1024 * 1024)
	{
		// from - phone-xxhdpi-4096-dalvik-heap.mk
		property_override("dalvik.vm.heapstartsize", "8m");
		property_override("dalvik.vm.heaptargetutilization", "0.6");
		property_override("dalvik.vm.heapgrowthlimit", "192m");
		property_override("dalvik.vm.heapsize", "512m");
		property_override("dalvik.vm.heapmaxfree", "16m");
		property_override("dalvik.vm.heapminfree", "4m");
	}
	else
	{
                // from - phone-xhdpi-2048-dalvik-heap.mk
		property_override("dalvik.vm.heapstartsize", "8m");
		property_override("dalvik.vm.heaptargetutilization", "0.7");
		property_override("dalvik.vm.heapgrowthlimit", "192m");
		property_override("dalvik.vm.heapsize", "512m");
		property_override("dalvik.vm.heapmaxfree", "8m");
		property_override("dalvik.vm.heapminfree", "2m");
	}
}

void vendor_load_properties()
{
	load_dalvik_properties();
	// Eovlution-X
	property_override("org.evolution.build_donate_url", "https://t.me/BanHammerBoi");
	property_override("org.evolution.build_maintainer", "AbhiSagar");
	property_override("org.evolution.build_support_url", "https://t.me/EvolutionXSakura");
}
