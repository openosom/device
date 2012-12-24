#
# Copyright (C) 2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=uboot-s3c64xx
PKG_VERSION:=2011.06
PKG_RELEASE:=1
PKG_MD5SUM:=

PKG_SOURCE:=u-boot-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=ftp://ftp.denx.de/pub/u-boot/
PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/u-boot-$(PKG_VERSION)
#PKG_TARGETS:=bin

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)/config
	source "$(SOURCE)/Config.in"
endef

define Package/$(PKG_NAME)
  TITLE:=for s3c64xx boards(mini6410 gec6410 ...)
  CATEGORY:=Device
  SUBMENU:=
  SECTION:=boot
  DEPENDS:=
  URL:=http://www.denx.de/wiki/U-Boot
  MAINTAINER:=Richard Nee <richard.nee.cn@gmail.com>
endef

define Package/$(PKG_NAME)/description
  U-Boot for Friendly-ARM MINI6410 Boards, by Tekkaman Ninja.
  tekkaman      : https://github.com/tekkamanninja/u-boot-2011.06-for-MINI6410.git
                  http://tekkamanninja.blog.chinaunix.net
  orig          : ftp://ftp.denx.de/pub/u-boot/u-boot-2011.06.tar.bz2
endef

define Build/Prepare
	$(PKG_UNPACK)
	$(CP) $(CP_OPTS) $(FILES_DIR)/* $(PKG_BUILD_DIR)/
	$(Build/Patch)
	$(FIND) $(PKG_BUILD_DIR) -name .svn | $(XARGS) rm -rf
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) mini6410_config
	$(MAKE) -C $(PKG_BUILD_DIR) CROSS_COMPILE=$(TARGET_CROSS)
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(BIN_DIR)
	dd if=$(PKG_BUILD_DIR)/u-boot.bin of=$(BIN_DIR)/$(PKG_NAME).bin bs=128k conv=sync
endef


$(eval $(call BuildPackage,$(PKG_NAME)))
