#
# Copyright (C) 2012 OpenWrt.org.cn
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=uboot-openmoko
PKG_VERSION:=1.3.2
PKG_RELEASE:=1

PKG_SOURCE:=u-boot-1.3.2-openmoko-stable.tar.bz2
PKG_SOURCE_SUBDIR:=u-boot-1.3.2-openmoko-stable
PKG_BUILD_DIR=$(KERNEL_BUILD_DIR)/$(PKG_SOURCE_SUBDIR)
PKG_SOURCE_URL:=git://git.openmoko.org/git/u-boot.git
PKG_SOURCE_PROTO:=git
PKG_SOURCE_VERSION:=4ed6f14590c443f20c12b2f39ec0c14f0c41c0f8
#PKG_TARGETS:=bin

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  TITLE:=for OpenMoko(GTA02)
  CATEGORY:=Device
  SUBMENU:=
  SECTION:=boot
  DEPENDS:=
  URL:=http://www.denx.de/wiki/U-Boot http://wiki.openmoko.org/wiki/U-Boot
  MAINTAINER:=richard.nee.cn <openme@openwrt.org.cn>
endef

define Package/$(PKG_NAME)/description
  U-Boot for OpenMoko
  http://wiki.openmoko.org/wiki/U-Boot
      also support QT2410.

  git://repo.or.cz/w/u-boot-openmoko/mini2440.git
     for mini2440, is based on uboot-openmoko
endef

define Build/Prepare
	$(PKG_UNPACK)
#	$(CP) $(CP_OPTS) $(FILES_DIR)/* $(PKG_BUILD_DIR)/
	$(Build/Patch)
	$(FIND) $(PKG_BUILD_DIR) -name .svn | $(XARGS) rm -rf
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) gta02v5_config
	$(MAKE) -C $(PKG_BUILD_DIR) CROSS_COMPILE=$(TARGET_CROSS) u-boot.udfu
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(BIN_DIR)
	dd if=$(PKG_BUILD_DIR)/u-boot.bin of=$(BIN_DIR)/$(PKG_NAME).bin bs=128k conv=sync
endef


$(eval $(call BuildPackage,$(PKG_NAME)))
