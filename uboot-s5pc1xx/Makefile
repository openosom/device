#
# Copyright (C) 2013 OpenWrt.org.cn
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=uboot-s5pc1xx
PKG_VERSION:=2011.10.2
PKG_RELEASE:=1
PKG_MD5SUM:=

PKG_SOURCE:=u-boot-linaro-2011.10.2.tar.gz
PKG_SOURCE_URL:=https://launchpad.net/u-boot-linaro/11.11/11.10/+download/
PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/u-boot-linaro-stable-ca0ef63
#PKG_TARGETS:=bin

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)/config
	source "$(SOURCE)/Config.in"
endef

define Package/$(PKG_NAME)
  TITLE:=s5pc1xx/s5pv2xx soc(mini210 tiny210...)
  CATEGORY:=Device
  SUBMENU:=
  SECTION:=boot
  DEPENDS:=@TARGET_s5pc1xx
  URL:=http://www.linaro.org/downloads/1110/
  MAINTAINER:=Richard Nee <openme@openwrt.org.cn>
endef

define Package/$(PKG_NAME)/description
  U-Boot for S5PC1xx/S5PV2xx
	http://www.linaro.org/downloads/1110/
	https://gitorious.org/opencsbc/u-boot/archive-tarball/mini210_linaro-2011.10-stable
	uboot for tiny210 ver3.1
endef

define Build/Prepare
	$(PKG_UNPACK)
	$(CP) $(CP_OPTS) $(FILES_DIR)/* $(PKG_BUILD_DIR)/
	$(Build/Patch)
	$(FIND) $(PKG_BUILD_DIR) -name .svn | $(XARGS) rm -rf
endef

MACH_CONF:=tiny210
ifeq ($(findstring tiny210,$(TARGETID)),tiny210)
  MACH_CONF:=tiny210
endif

ifeq ($(findstring mini210,$(TARGETID)),mini210)
  MACH_CONF:=mini210
endif

define Build/Compile
	echo "#TARGETID: $(TARGETID)"
	echo "#MACH_CONF: $(MACH_CONF)"

	$(MAKE) -C $(PKG_BUILD_DIR) $(MACH_CONF)_config
	$(MAKE) -C $(PKG_BUILD_DIR) CROSS_COMPILE=$(TARGET_CROSS)
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(BIN_DIR)
	$(CP) $(PKG_BUILD_DIR)/u-boot-$(MACH_CONF).bin $(BIN_DIR)/$(PKG_NAME).bin
endef


$(eval $(call BuildPackage,$(PKG_NAME)))
