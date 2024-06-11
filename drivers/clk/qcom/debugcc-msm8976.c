// SPDX-License-Identifier: GPL-2.0
// Copyright (c) 2014-2016, The Linux Foundation. All rights reserved.
// Copyright (c) 2024, Benarji Anand <benarji385@gmail.com>

#define pr_fmt(fmt) "clk: %s: " fmt, __func__

#include <linux/clk.h>
#include <linux/clk-provider.h>
#include <linux/err.h>
#include <linux/kernel.h>
#include <linux/mfd/syscon.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/regmap.h>

#include "clk-debug.h"
#include "common.h"

/* Debug Mux for measure */
static struct measure_clk_data debug_mux_priv = {
	.ctl_reg = 0x74004,
	.status_reg = 0x74008,
	.xo_div4_cbcr = 0x30034,
};

static const char *const gcc_debug_mux_parent_names[] = {
	"gcc_apss_ahb_clk",
	"gcc_apss_axi_clk",
	"gcc_blsp1_ahb_clk",
	"gcc_blsp2_ahb_clk",
	"gcc_boot_rom_ahb_clk",
	"gcc_crypto_ahb_clk",
	"gcc_crypto_axi_clk",
	"gcc_crypto_clk",
	"gcc_prng_ahb_clk",
	"gcc_qdss_dap_clk",
	"gcc_apss_tcu_async_clk",
	"gcc_cpp_tbu_clk",
	"gcc_jpeg_tbu_clk",
	"gcc_mdp_tbu_clk",
	"gcc_smmu_cfg_clk",
	"gcc_venus_tbu_clk",
	"gcc_vfe0_tbu_clk",
	"gcc_vfe1_tbu_clk",
	"gcc_apc0_droop_detector_gpll0_clk",
	"gcc_apc1_droop_detector_gpll0_clk",
	"gcc_blsp1_qup1_i2c_apps_clk",
	"gcc_blsp1_qup1_spi_apps_clk",
	"gcc_blsp1_qup2_i2c_apps_clk",
	"gcc_blsp1_qup2_spi_apps_clk",
	"gcc_blsp1_qup3_i2c_apps_clk",
	"gcc_blsp1_qup3_spi_apps_clk",
	"gcc_blsp1_qup4_i2c_apps_clk",
	"gcc_blsp1_qup4_spi_apps_clk",
	"gcc_blsp1_uart1_apps_clk",
	"gcc_blsp1_uart2_apps_clk",
	"gcc_blsp2_qup1_i2c_apps_clk",
	"gcc_blsp2_qup1_spi_apps_clk",
	"gcc_blsp2_qup2_i2c_apps_clk",
	"gcc_blsp2_qup2_spi_apps_clk",
	"gcc_blsp2_qup3_i2c_apps_clk",
	"gcc_blsp2_qup3_spi_apps_clk",
	"gcc_blsp2_qup4_i2c_apps_clk",
	"gcc_blsp2_qup4_spi_apps_clk",
	"gcc_blsp2_uart1_apps_clk",
	"gcc_blsp2_uart2_apps_clk",
	"gcc_camss_cci_ahb_clk",
	"gcc_camss_cci_clk",
	"gcc_camss_cpp_ahb_clk",
	"gcc_camss_cpp_axi_clk",
	"gcc_camss_cpp_clk",
	"gcc_camss_csi0_ahb_clk",
	"gcc_camss_csi0_clk",
	"gcc_camss_csi0_csiphy_3p_clk",
	"gcc_camss_csi0phy_clk",
	"gcc_camss_csi0pix_clk",
	"gcc_camss_csi0rdi_clk",
	"gcc_camss_csi1_ahb_clk",
	"gcc_camss_csi1_clk",
	"gcc_camss_csi1_csiphy_3p_clk",
	"gcc_camss_csi1phy_clk",
	"gcc_camss_csi1pix_clk",
	"gcc_camss_csi1rdi_clk",
	"gcc_camss_csi2_ahb_clk",
	"gcc_camss_csi2_clk",
	"gcc_camss_csi2_csiphy_3p_clk",
	"gcc_camss_csi2phy_clk",
	"gcc_camss_csi2pix_clk",
	"gcc_camss_csi2rdi_clk",
	"gcc_camss_csi_vfe0_clk",
	"gcc_camss_csi_vfe1_clk",
	"gcc_camss_gp0_clk",
	"gcc_camss_gp1_clk",
	"gcc_camss_ispif_ahb_clk",
	"gcc_camss_jpeg0_clk",
	"gcc_camss_jpeg_ahb_clk",
	"gcc_camss_jpeg_axi_clk",
	"gcc_camss_mclk0_clk",
	"gcc_camss_mclk1_clk",
	"gcc_camss_mclk2_clk",
	"gcc_camss_micro_ahb_clk",
	"gcc_camss_csi0phytimer_clk",
	"gcc_camss_csi1phytimer_clk",
	"gcc_camss_csi2phytimer_clk",
	"gcc_camss_ahb_clk",
	"gcc_camss_top_ahb_clk",
	"gcc_camss_vfe0_clk",
	"gcc_camss_vfe0_ahb_clk",
	"gcc_camss_vfe0_axi_clk",
	"gcc_camss_vfe1_ahb_clk",
	"gcc_camss_vfe1_axi_clk",
	"gcc_camss_vfe1_clk",
	"gcc_dcc_clk",
	"gcc_gp1_clk",
	"gcc_gp2_clk",
	"gcc_gp3_clk",
	"gcc_mss_cfg_ahb_clk",
	"gcc_mss_q6_bimc_axi_clk",
	"gcc_pdm2_clk",
	"gcc_pdm_ahb_clk",
	"gcc_rbcpr_gfx_clk",
	"gcc_sdcc1_ahb_clk",
	"gcc_sdcc1_apps_clk",
	"gcc_sdcc1_ice_core_clk",
	"gcc_sdcc2_ahb_clk",
	"gcc_sdcc2_apps_clk",
	"gcc_sdcc3_ahb_clk",
	"gcc_sdcc3_apps_clk",
        "gcc_pcnoc_usb3_axi_clk",
        "gcc_usb30_master_clk",
        "gcc_usb30_sleep_clk",
        "gcc_usb30_mock_utmi_clk",
        "gcc_usb_phy_cfg_ahb_clk",
        "gcc_usb3_pipe_clk",
	"gcc_venus0_ahb_clk",
	"gcc_venus0_axi_clk",
	"gcc_venus0_core0_vcodec0_clk",
	"gcc_venus0_core1_vcodec0_clk",
	"gcc_venus0_vcodec0_clk",
	"gcc_mdss_ahb_clk",
	"gcc_mdss_axi_clk",
	"gcc_mdss_pclk0_clk",
	"gcc_mdss_byte0_clk",
	"gcc_mdss_esc0_clk",
	"gcc_mdss_pclk1_clk",
	"gcc_mdss_byte1_clk",
	"gcc_mdss_esc1_clk",
	"gcc_mdss_mdp_clk",
	"gcc_mdss_vsync_clk",
	"gcc_oxili_timer_clk",
	"gcc_oxili_gfx3d_clk",
	"gcc_oxili_gmem_clk",
	"gcc_oxili_aon_clk",
	"gcc_oxili_ahb_clk",
	"gcc_bimc_gfx_clk",
	"gcc_bimc_gpu_clk",
};

static int gcc_debug_mux_sels[] = {
	0x168,		 /* gcc_apss_ahb_clk */
	0x169,		 /* gcc_apss_axi_clk */
	0x88,		 /* gcc_blsp1_ahb_clk */
	0x98,		 /* gcc_blsp2_ahb_clk */
	0xf8,		 /* gcc_boot_rom_ahb_clk */
	0x13a,		 /* gcc_crypto_ahb_clk */
	0x139,		 /* gcc_crypto_axi_clk */
	0x138,		 /* gcc_crypto_clk */
	0xd8,		 /* gcc_prng_ahb_clk */
	0x49,		 /* gcc_qdss_dap_clk */
	0x50,		 /* gcc_apss_tcu_async_clk */
	0xe9,		 /* gcc_cpp_tbu_clk */
	0x5c,		 /* gcc_jpeg_tbu_clk */
	0x51,		 /* gcc_mdp_tbu_clk */
	0x5b,		 /* gcc_smmu_cfg_clk */
	0x54,		 /* gcc_venus_tbu_clk */
	0x5a,		 /* gcc_vfe0_tbu_clk */
	0x199,		 /* gcc_vfe1_tbu_clk */
	0x1c,		 /* gcc_apc0_droop_detector_gpll0_clk */
	0x1f,		 /* gcc_apc1_droop_detector_gpll0_clk */
	0x8b,		 /* gcc_blsp1_qup1_i2c_apps_clk */
	0x8a,		 /* gcc_blsp1_qup1_spi_apps_clk */
	0x90,		 /* gcc_blsp1_qup2_i2c_apps_clk */
	0x8e,		 /* gcc_blsp1_qup2_spi_apps_clk */
	0x94,		 /* gcc_blsp1_qup3_i2c_apps_clk */
	0x93,		 /* gcc_blsp1_qup3_spi_apps_clk */
	0x96,		 /* gcc_blsp1_qup4_i2c_apps_clk */
	0x95,		 /* gcc_blsp1_qup4_spi_apps_clk */
	0x8c,		 /* gcc_blsp1_uart1_apps_clk */
	0x91,		 /* gcc_blsp1_uart2_apps_clk */
	0x9b,		 /* gcc_blsp2_qup1_i2c_apps_clk */
	0x9a,		 /* gcc_blsp2_qup1_spi_apps_clk */
	0xa0,		 /* gcc_blsp2_qup2_i2c_apps_clk */
	0x9e,		 /* gcc_blsp2_qup2_spi_apps_clk */
	0xa4,		 /* gcc_blsp2_qup3_i2c_apps_clk */
	0xa3,		 /* gcc_blsp2_qup3_spi_apps_clk */
	0xa6,		 /* gcc_blsp2_qup4_i2c_apps_clk */
	0xa5,		 /* gcc_blsp2_qup4_spi_apps_clk */
	0x9c,		 /* gcc_blsp2_uart1_apps_clk */
	0xa1,		 /* gcc_blsp2_uart2_apps_clk */
	0xb0,		 /* gcc_camss_cci_ahb_clk */
	0xaf,		 /* gcc_camss_cci_clk */
	0xba,		 /* gcc_camss_cpp_ahb_clk */
	0x1a4,		 /* gcc_camss_cpp_axi_clk */
	0xb9,		 /* gcc_camss_cpp_clk */
	0xc1,		 /* gcc_camss_csi0_ahb_clk */
	0xc0,		 /* gcc_camss_csi0_clk */
	0xdc,		 /* gcc_camss_csi0_csiphy_3p_clk */
	0xc2,		 /* gcc_camss_csi0phy_clk */
	0xc4,		 /* gcc_camss_csi0pix_clk */
	0xc3,		 /* gcc_camss_csi0rdi_clk */
	0xc6,		 /* gcc_camss_csi1_ahb_clk */
	0xc5,		 /* gcc_camss_csi1_clk */
	0xdd,		 /* gcc_camss_csi1_csiphy_3p_clk */
	0xc7,		 /* gcc_camss_csi1phy_clk */
	0xe1,		 /* gcc_camss_csi1pix_clk */
	0xe0,		 /* gcc_camss_csi1rdi_clk */
	0xe4,		 /* gcc_camss_csi2_ahb_clk */
	0xe3,		 /* gcc_camss_csi2_clk */
	0xde,		 /* gcc_camss_csi2_csiphy_3p_clk */
	0xe5,		 /* gcc_camss_csi2phy_clk */
	0xe7,		 /* gcc_camss_csi2pix_clk */
	0xe6,		 /* gcc_camss_csi2rdi_clk */
	0xbf,		 /* gcc_camss_csi_vfe0_clk */
	0x1a0,		 /* gcc_camss_csi_vfe1_clk */
	0xab,		 /* gcc_camss_gp0_clk */
	0xac,		 /* gcc_camss_gp1_clk */
	0xe2,		 /* gcc_camss_ispif_ahb_clk */
	0xb3,		 /* gcc_camss_jpeg0_clk */
	0xb4,		 /* gcc_camss_jpeg_ahb_clk */
	0xb5,		 /* gcc_camss_jpeg_axi_clk */
	0xad,		 /* gcc_camss_mclk0_clk */
	0xae,		 /* gcc_camss_mclk1_clk */
	0x1bd,		 /* gcc_camss_mclk2_clk */
	0xaa,		 /* gcc_camss_micro_ahb_clk */
	0xb1,		 /* gcc_camss_csi0phytimer_clk */
	0xb2,		 /* gcc_camss_csi1phytimer_clk */
	0x1d,		 /* gcc_camss_csi2phytimer_clk */
	0xa8,		 /* gcc_camss_ahb_clk */
	0xa9,		 /* gcc_camss_top_ahb_clk */
	0xb8,		 /* gcc_camss_vfe0_clk */
	0xbb,		 /* gcc_camss_vfe0_ahb_clk */
	0xbc,		 /* gcc_camss_vfe0_axi_clk */
	0x1a2,		 /* gcc_camss_vfe1_ahb_clk */
	0x1a3,		 /* gcc_camss_vfe1_axi_clk */
	0x1a1,		 /* gcc_camss_vfe1_clk */
	0xd,		 /* gcc_dcc_clk */
	0x10,		 /* gcc_gp1_clk */
	0x11,		 /* gcc_gp2_clk */
	0x12,		 /* gcc_gp3_clk */
	0x30,		 /* gcc_mss_cfg_ahb_clk */
	0x31,		 /* gcc_mss_q6_bimc_axi_clk */
	0xd2,		 /* gcc_pdm2_clk */
	0xd0,		 /* gcc_pdm_ahb_clk */
	0xf0,		 /* gcc_rbcpr_gfx_clk */
	0x69,		 /* gcc_sdcc1_ahb_clk */
	0x68,		 /* gcc_sdcc1_apps_clk */
	0x6a,		 /* gcc_sdcc1_ice_core_clk */
	0x71,		 /* gcc_sdcc2_ahb_clk */
	0x70,		 /* gcc_sdcc2_apps_clk */
	0x221,		 /* gcc_sdcc3_ahb_clk */
	0x220,		 /* gcc_sdcc3_apps_clk */
	0x0e,		 /* gcc_pcnoc_usb3_axi_clk */
	0x60,		 /* gcc_usb30_master_clk */
	0x61,		 /* gcc_usb30_sleep_clk */
	0x62,		 /* gcc_usb30_mock_utmi_clk */
	0x63,		 /* gcc_usb_phy_cfg_ahb_clk */
	0x66,		 /* gcc_usb3_pipe_clk */
	0x1f3,		 /* gcc_venus0_ahb_clk */
	0x1f2,		 /* gcc_venus0_axi_clk */
	0x1b8,		 /* gcc_venus0_core0_vcodec0_clk */
	0x1b9,		 /* gcc_venus0_core1_vcodec0_clk */
	0x1f1,		 /* gcc_venus0_vcodec0_clk */
	0x1f6,		 /* gcc_mdss_ahb_clk */
	0x1f7,		 /* gcc_mdss_axi_clk */
	0x1f8,		 /* gcc_mdss_pclk0_clk */
	0x1fc,		 /* gcc_mdss_byte0_clk */
	0x1fd,		 /* gcc_mdss_esc0_clk */
	0x1fa,		 /* gcc_mdss_pclk1_clk */
	0xda,		 /* gcc_mdss_byte1_clk */
	0xdb,		 /* gcc_mdss_esc1_clk */
	0x1f9,		 /* gcc_mdss_mdp_clk */
	0x1fb,		 /* gcc_mdss_vsync_clk */
	0x1e9,		 /* gcc_oxili_timer_clk */
	0x1ea,		 /* gcc_oxili_gfx3d_clk */
	0x1f0,		 /* gcc_oxili_gmem_clk */
	0x1ee,		 /* gcc_oxili_aon_clk */
	0x1eb,		 /* gcc_oxili_ahb_clk */
	0x2d,		 /* gcc_bimc_gfx_clk */
	0x157,		 /* gcc_bimc_gpu_clk */
};

static struct clk_debug_mux gcc_debug_mux = {
	.priv = &debug_mux_priv,
	.en_mask = BIT(16),
	.debug_offset = 0x74000,
	.post_div_offset = 0x74000,
	.cbcr_offset = 0x74000,
	.src_sel_mask = 0x1FF,
	.src_sel_shift = 0,
	.post_div_mask = 0xF000,
	.post_div_shift = 12,
	.post_div_val = 1,
	.mux_sels = gcc_debug_mux_sels,
	.hw.init = &(struct clk_init_data){
		.name = "gcc_debug_mux",
		.ops = &clk_debug_mux_ops,
		.parent_names = gcc_debug_mux_parent_names,
		.num_parents = ARRAY_SIZE(gcc_debug_mux_parent_names),
		.flags = CLK_IS_MEASURE,
	},
};

static const char *const cpu_cc_debug_mux_parent_names[] = {
	"perfcl_clk",
	"pwrcl_clk",
};

static int cpu_cc_debug_mux_sels[] = {
	0x0,		/* pwrcl_clk */
        0x1,		/* perfcl_clk */
};

static int cpu_cc_debug_mux_pre_divs[] = {
	0x1,		/* perfcl_clk */
	0x1,		/* pwrcl_clk */
};

static struct clk_debug_mux cpu_cc_debug_mux = {
	.priv = &debug_mux_priv,
	.debug_offset = 0x0,
	.post_div_offset = 0x0,
	.cbcr_offset = U32_MAX,
	.src_sel_mask = 0x3FF00,
	.src_sel_shift = 8,
	.post_div_mask = 0xF0000000,
	.post_div_shift = 28,
	.post_div_val = 1,
	.mux_sels = cpu_cc_debug_mux_sels,
	.pre_div_vals = cpu_cc_debug_mux_pre_divs,
	.hw.init = &(struct clk_init_data){
		.name = "cpu_cc_debug_mux",
		.ops = &clk_debug_mux_ops,
		.parent_names = cpu_cc_debug_mux_parent_names,
		.num_parents = ARRAY_SIZE(cpu_cc_debug_mux_parent_names),
		.flags = CLK_IS_MEASURE,
	},
};

static struct clk_dummy pwrcl_clk = {
	.rrate = 1000,
	.hw.init = &(struct clk_init_data){
		.name = "pwrcl_clk",
		.ops = &clk_dummy_ops,
	},
};

static struct clk_dummy perfcl_clk = {
	.rrate = 1000,
	.hw.init = &(struct clk_init_data){
		.name = "perfcl_clk",
		.ops = &clk_dummy_ops,
	},
};

struct clk_hw *debugcc_msm8976_hws[] = {
	&perfcl_clk.hw,
	&pwrcl_clk.hw,
};

static struct mux_regmap_names mux_list[] = {
	{ .mux = &gcc_debug_mux, .regmap_name = "qcom,gcc" },
	{ .mux = &cpu_cc_debug_mux, .regmap_name = "qcom,cpu" },
};

static const struct of_device_id clk_debug_match_table[] = {
	{ .compatible = "qcom,msm8976-debugcc" },
	{}
};

static int clk_debug_msm8976_probe(struct platform_device *pdev)
{
	struct clk *clk;
	int ret, i;

	clk = devm_clk_get(&pdev->dev, "xo_board");
	if (IS_ERR(clk)) {
		if (PTR_ERR(clk) != -EPROBE_DEFER)
			dev_err(&pdev->dev, "Unable to get XO clock\n");
		return PTR_ERR(clk);
	}

	debug_mux_priv.cxo = clk;

	for (i = 0; i < ARRAY_SIZE(mux_list); i++) {
		ret = map_debug_bases(pdev, mux_list[i].regmap_name,
				      mux_list[i].mux);
		if (ret == -EBADR)
			continue;
		else if (ret)
			return ret;

		clk = devm_clk_register(&pdev->dev, &mux_list[i].mux->hw);
		if (IS_ERR(clk)) {
			dev_err(&pdev->dev, "Unable to register %s, err:(%d)\n",
				clk_hw_get_name(&mux_list[i].mux->hw),
				PTR_ERR(clk));
			return PTR_ERR(clk);
		}
	}

	ret = clk_debug_measure_register(&gcc_debug_mux.hw);
	if (ret) {
		dev_err(&pdev->dev, "Could not register Measure clocks\n");
		return ret;
	}

	dev_info(&pdev->dev, "Registered Debug Measure clocks\n");
	return ret;
};

static struct platform_driver clk_debug_driver = {
	.probe = clk_debug_msm8976_probe,
	.driver = {
		.name = "debugcc-msm8976",
		.of_match_table = clk_debug_match_table,
	},
};

int __init clk_debug_msm8976_init(void)
{
	return platform_driver_register(&clk_debug_driver);
}
fs_initcall(clk_debug_msm8976_init);

MODULE_DESCRIPTION("QCOM DEBUG CC MSM8976 Driver");
MODULE_LICENSE("GPL v2");
