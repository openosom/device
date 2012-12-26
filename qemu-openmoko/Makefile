#
# Copyright (C) 2012 OpenWrt.org.cn
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=qemu-openmoko
PKG_REV:=6008
PKG_VERSION:=$(PKG_REV)
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_BUILD_DIR=$(BUILD_DIR)/$(PKG_SOURCE_SUBDIR)
PKG_SOURCE_URL:=https://svn.openmoko.org/trunk/src/host/qemu-neo1973
PKG_SOURCE_PROTO:=svn
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_TARGETS:=bin

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  TITLE:=QEMU  Emulator for OpenMoko
  CATEGORY:=Device
  SUBMENU:=
  SECTION:=emulator
  DEPENDS:=
  URL:=http://wiki.qemu.org/ http://wiki.openmoko.org/wiki/Openmoko_under_QEMU
endef

define Package/$(PKG_NAME)/description
  all release QEMU : http://download.savannah.gnu.org/releases/qemu/
                     git://repo.or.cz/qemu.git
  mini2440 QEMU    : git://repo.or.cz/qemu/mini2440.git
  openmoko QEMU    : https://svn.openmoko.org/trunk/src/host/qemu-neo1973
  goldfish QEME    : https://android.googlesource.com/platform/external/qemu.git
endef

define Build/Configure
	( cd $(PKG_BUILD_DIR)/ ; \
		$(CONFIGURE_CMD) \
		--target-list=arm-softmmu \
		--disable-gcc-check \
	)
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR)
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR)	$(BIN_DIR) \
		$(1) \
		$(1)/arm-softmmu

	$(INSTALL_BIN)  $(PKG_BUILD_DIR)/arm-softmmu/qemu-system-arm    $(1)/arm-softmmu/
	$(INSTALL_BIN)	$(PKG_BUILD_DIR)/{qemu-img,dyngen}              $(1)/
	$(CP)           $(PKG_BUILD_DIR)/flash2raw*                     $(1)/
	$(CP)           $(PKG_BUILD_DIR)/raw2flash*                     $(1)/
	$(CP)           $(PKG_BUILD_DIR)/openmoko                       $(1)/
	$(CP)           ./files/env-gta01                               $(1)/openmoko/env
	$(CP)		./files/run-openmoko-emulator.sh                $(1)/
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
