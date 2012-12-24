#
# Copyright (C) 2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=uboot-s3c24xx
PKG_VERSION:=2010.03
PKG_RELEASE:=1
PKG_MD5SUM:=2bf5ebf497dddc52440b1ea386cc1332

PKG_SOURCE:=u-boot-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=ftp://ftp.denx.de/pub/u-boot/
PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/u-boot-$(PKG_VERSION)
#PKG_TARGETS:=bin

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)/config
	source "$(SOURCE)/Config.in"
endef

define Package/$(PKG_NAME)
  TITLE:=for s3c24xx boards(mini2440 fs2410 ...)
  CATEGORY:=Device
  SUBMENU:=
  SECTION:=boot
  DEPENDS:=@TARGET_s3c24xx
  URL:=http://www.denx.de/wiki/U-Boot
  MAINTAINER:=Richard Nee <richard.nee.cn@gmail.com>
endef

define Package/$(PKG_NAME)/description
  based on lintel<lintel.huang@gmail.com> u-boot-2010.03-s3c24xx-openwrt.tar.bz2 !

  orig -->> tekkaman -->> lintel -->> uboot-s3c24xx

  lintel        : http://openwrt-arm.googlecode.com/files/u-boot-2010.03-s3c24xx-openwrt.tar.bz2
  tekkaman      : https://github.com/tekkamanninja/u-boot-2010.03-tekkaman.git
                  http://tekkamanninja.blog.chinaunix.net
  orig          : ftp://ftp.denx.de/pub/u-boot/u-boot-2010.03.tar.bz2
endef

define Build/Prepare
	$(PKG_UNPACK)
	$(CP) $(CP_OPTS) $(FILES_DIR)/* $(PKG_BUILD_DIR)/
	$(Build/Patch)
	$(FIND) $(PKG_BUILD_DIR) -name .svn | $(XARGS) rm -rf
endef

DEF_SOC_TYPE:=
ifeq ($(CONFIG_DEFAULT_SOC_TYPE),y)
ifeq ($(findstring 2410,$(TARGETID)),2410)
  DEF_SOC_TYPE:=SOC_IS_S3C2410
endif
ifeq ($(findstring 2440,$(TARGETID)),2440)
  DEF_SOC_TYPE:=SOC_IS_S3C2440
endif
endif

ifeq ($(CONFIG_S3C2410_SOC_TYPE),y)
  DEF_SOC_TYPE:=SOC_IS_S3C2410
endif

ifeq ($(CONFIG_S3C2440_SOC_TYPE),y)
  DEF_SOC_TYPE:=SOC_IS_S3C2440
endif

define Build/Compile
	echo "#TARGETID: $(TARGETID)"
	echo "#SOC_TYPE: $(DEF_SOC_TYPE)"
	$(SED) '/#define CONFIG_ARM920T/ i\#define $(DEF_SOC_TYPE)' $(PKG_BUILD_DIR)/include/configs/smdk2440.h

	$(MAKE) -C $(PKG_BUILD_DIR) smdk2440_config
	$(MAKE) -C $(PKG_BUILD_DIR) CROSS_COMPILE=$(TARGET_CROSS)
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(BIN_DIR)
	dd if=$(PKG_BUILD_DIR)/u-boot.bin of=$(BIN_DIR)/$(PKG_NAME).bin bs=128k conv=sync
endef


$(eval $(call BuildPackage,$(PKG_NAME)))
