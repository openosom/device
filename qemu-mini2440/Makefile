#
# Copyright (C) 2006-2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=qemu-mini2440
PKG_VERSION:=master-2012-09-06
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_BUILD_DIR=$(BUILD_DIR)/$(PKG_SOURCE_SUBDIR)
PKG_SOURCE_URL:=git://repo.or.cz/qemu/mini2440.git
PKG_SOURCE_PROTO:=git
PKG_SOURCE_VERSION:=18b91f222c2aebb2eb45cb7e6d5f58ca668e2aa2
#PKG_TARGETS:=bin

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  TITLE:=QEMU  Emulator for MINI2440 Boards
  CATEGORY:=Hardware
  SUBMENU:=
  SECTION:=emulator
  DEPENDS:=@TARGET_s3c24xx
  URL:=http://wiki.qemu.org/
endef

define Package/$(PKG_NAME)/description
  http://download.savannah.gnu.org/releases/qemu/
  git://repo.or.cz/qemu.git
  git://repo.or.cz/qemu/mini2440.git
  https://svn.openmoko.org/trunk/src/host/qemu-neo1973
  git://android.git.kernel.org/platform/external/qemu
endef

define Build/Configure
	( cd $(PKG_BUILD_DIR)/ ; \
		$(CONFIGURE_CMD) \
		--target-list=arm-softmmu \
	)
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR)
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR)	$(TOPDIR)/bin/host/bin
	$(INSTALL_BIN)	$(PKG_BUILD_DIR)/qemu-img	$(TOPDIR)/bin/host/bin/
	$(CP)		./files/skins			$(TOPDIR)/bin/host/
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
