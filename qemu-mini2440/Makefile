#
# Copyright (C) 2012 OpenWrt.org.cn
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=qemu-mini2440
PKG_VERSION:=stable-20111013
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_BUILD_DIR=$(BUILD_DIR)/$(PKG_SOURCE_SUBDIR)
PKG_SOURCE_URL:=git://repo.or.cz/qemu/mini2440.git
PKG_SOURCE_PROTO:=git
PKG_SOURCE_VERSION:=18b91f222c2aebb2eb45cb7e6d5f58ca668e2aa2
PKG_TARGETS:=bin

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  TITLE:=QEMU  Emulator for MINI2440
  CATEGORY:=Device
  SUBMENU:=
  SECTION:=emulator
  DEPENDS:=@TARGET_s3c24xx
  URL:=http://wiki.qemu.org/
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
	)
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR)
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR)	$(1) $(BIN_DIR)
	$(INSTALL_BIN)	$(PKG_BUILD_DIR)/{qemu-img,qemu-io,qemu-nbd}	$(1)/
	$(INSTALL_BIN)  $(PKG_BUILD_DIR)/arm-softmmu/qemu-system-arm	$(1)/
	$(CP)           ./files/mini2440                                $(1)/
	$(CP)		./files/run-mini2440-emulator.sh		$(BIN_DIR)/
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
