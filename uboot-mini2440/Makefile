#
# Copyright (C) 2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=uboot-mini2440
PKG_VERSION:=2010.03
PKG_RELEASE:=1

PKG_SOURCE:=u-boot-2010.03-tekkaman.tar.bz2
PKG_SOURCE_SUBDIR:=u-boot-2010.03-tekkaman
PKG_BUILD_DIR=$(KERNEL_BUILD_DIR)/$(PKG_SOURCE_SUBDIR)
PKG_SOURCE_URL:=git://github.com/tekkamanninja/u-boot-2010.03-tekkaman.git
PKG_SOURCE_PROTO:=git
PKG_SOURCE_VERSION:=052d395ea78d08f13554de4c920130b423db38bc
#PKG_TARGETS:=bin

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  TITLE:=for mini2440(tekkaman)
  CATEGORY:=Device
  SUBMENU:=
  SECTION:=boot
  DEPENDS:=@TARGET_s3c24xx
  URL:=http://www.denx.de/wiki/U-Boot
endef

define Package/$(PKG_NAME)/description
  U-Boot for MINI2440 Boards, by Tekkaman Ninja.
  https://github.com/tekkamanninja/u-boot-2010.03-tekkaman.git
  http://tekkamanninja.blog.chinaunix.net
endef

define Build/Prepare
	$(PKG_UNPACK)
#	$(CP) $(CP_OPTS) $(FILES_DIR)/* $(PKG_BUILD_DIR)/
	$(Build/Patch)
	$(FIND) $(PKG_BUILD_DIR) -name .svn | $(XARGS) rm -rf
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) mini2440_config
	$(MAKE) -C $(PKG_BUILD_DIR) CROSS_COMPILE=$(TARGET_CROSS)
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(BIN_DIR)
	dd if=$(PKG_BUILD_DIR)/u-boot.bin of=$(BIN_DIR)/$(PKG_NAME).bin bs=128k conv=sync
endef


$(eval $(call BuildPackage,$(PKG_NAME)))
