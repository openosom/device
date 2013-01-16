#
# Copyright (C) 2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=uboot-s3c24xx-lintel
PKG_VERSION:=2010.03
PKG_RELEASE:=1
PKG_MD5SUM:=46db32c4cbb6222096f68295a85fada1

PKG_SOURCE:=u-boot-2010.03-s3c24xx-openwrt.tar.bz2
PKG_SOURCE_URL:=http://openwrt-arm.googlecode.com/files/
PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/u-boot-$(PKG_VERSION)
#PKG_TARGETS:=bin

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  TITLE:=for s3c24xx boards(mini2440 gec2440...)
  CATEGORY:=Device
  SUBMENU:=
  SECTION:=boot
  DEPENDS:=@TARGET_s3c24xx @BROKEN
  URL:=http://www.denx.de/wiki/U-Boot
endef

define Package/$(PKG_NAME)/description
  U-Boot for S3C24XX Boards, by lintel.huang@gmail.com
endef

define Build/Prepare
	$(PKG_UNPACK)
#	$(CP) $(CP_OPTS) $(FILES_DIR)/* $(PKG_BUILD_DIR)/
	$(Build/Patch)
	$(FIND) $(PKG_BUILD_DIR) -name .svn | $(XARGS) rm -rf
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) smdk2440_config
	$(MAKE) -C $(PKG_BUILD_DIR) CROSS_COMPILE=$(TARGET_CROSS)
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(BIN_DIR)
	dd if=$(PKG_BUILD_DIR)/u-boot.bin of=$(BIN_DIR)/$(PKG_NAME).bin bs=128k conv=sync
endef


$(eval $(call BuildPackage,$(PKG_NAME)))
