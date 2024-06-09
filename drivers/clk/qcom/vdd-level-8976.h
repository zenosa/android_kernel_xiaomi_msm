// SPDX-License-Identifier: GPL-2.0
// Copyright (c) 2014-2016, The Linux Foundation. All rights reserved.
// Copyright (c) 2024, Benarji Anand <benarji385@gmail.com>

#ifndef __DRIVERS_CLK_QCOM_VDD_LEVEL_8976_H
#define __DRIVERS_CLK_QCOM_VDD_LEVEL_8976_H

#include <linux/regulator/rpm-smd-regulator.h>
#include <linux/regulator/consumer.h>

enum vdd_cx_levels {
	VDD_CX_MIN,		/* LOW_SVS */
	VDD_CX_LOW,		/* SVS */
	VDD_CX_LOW_L1,		/* SVS_PLUS */
	VDD_CX_NOMINAL,         /* NOM */
	VDD_CX_NOMINAL_L1,      /* NOM_PLUS */
	VDD_CX_HIGH,		/* TURBO */
	VDD_CX_NUM
};

enum vdd_gx_levels {
	VDD_GX_MIN,		/* LOW_SVS */
	VDD_GX_LOW,		/* SVS */
	VDD_GX_LOW_L1,          /* SVS_PLUS */
	VDD_GX_NOMINAL,         /* NOM */
	VDD_GX_NOMINAL_L1,      /* NOM_PLUS */
	VDD_GX_HIGH,            /* TURBO */
	VDD_GX_NUM
};

static int vdd_corner[] = {
	RPM_REGULATOR_LEVEL_MIN_SVS,		/* VDD_CX_MIN */
	RPM_REGULATOR_LEVEL_SVS,		/* VDD_CX_LOW */
	RPM_REGULATOR_LEVEL_SVS_PLUS,		/* VDD_CX_LOW_L1 */
	RPM_REGULATOR_LEVEL_NOM,		/* VDD_CX_NOMINAL */
	RPM_REGULATOR_LEVEL_NOM_PLUS,		/* VDD_CX_NOMINAL_L1 */
	RPM_REGULATOR_LEVEL_TURBO,		/* VDD_CX_HIGH */
};

#endif /* __DRIVERS_CLK_QCOM_VDD_LEVEL_8976_H */
