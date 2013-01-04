#
# Copyright (C) 2012 OpenWrt.org.cn
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=qemu-s3c24xx
PKG_VERSION:=20111013
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_BUILD_DIR=$(BUILD_DIR)/$(PKG_SOURCE_SUBDIR)
PKG_SOURCE_URL:=git://repo.or.cz/qemu/mini2440.git
PKG_SOURCE_PROTO:=git
PKG_SOURCE_VERSION:=18b91f222c2aebb2eb45cb7e6d5f58ca668e2aa2
PKG_TARGETS:=bin

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)/config
	source "$(SOURCE)/Config.in"
endef

define Package/$(PKG_NAME)
  TITLE:=QEMU  Emulator for S3C24XX
  CATEGORY:=Device
  SUBMENU:=
  SECTION:=emulator
  DEPENDS:=@TARGET_s3c24xx +uboot-s3c24xx
  URL:=http://wiki.qemu.org/
  MAINTAINER:=Richard Nee <openme@openwrt.org.cn>
endef

define Package/$(PKG_NAME)/description
  all release QEMU : http://download.savannah.gnu.org/releases/qemu/
                     git://repo.or.cz/qemu.git
  mini2440 QEMU    : git://repo.or.cz/qemu/mini2440.git
  openmoko QEMU    : https://svn.openmoko.org/trunk/src/host/qemu-neo1973
  goldfish QEME    : https://android.googlesource.com/platform/external/qemu.git
endef

define Build/Prepare
	$(PKG_UNPACK)
	$(CP) $(CP_OPTS) ./src/* $(PKG_BUILD_DIR)/
	$(Build/Patch)
	$(FIND) $(PKG_BUILD_DIR) -name .svn | $(XARGS) rm -rf
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

# TARGETID and SUBTARGET define in TOPDIR/include/target.mk. 
# but target.mk/kernel.mk cannot be used here, because of not cross compiling.
target_conf=$(subst .,_,$(subst -,_,$(subst /,_,$(1))))
PLATFORM_DIR:=$(TOPDIR)/target/linux/$(BOARD)
SUBTARGET:=$(strip $(foreach subdir,$(patsubst $(PLATFORM_DIR)/%/target.mk,%,$(wildcard $(PLATFORM_DIR)/*/target.mk)),$(if $(CONFIG_TARGET_$(call target_conf,$(BOARD)_$(subdir))),$(subdir))))
TARGETID:=$(BOARD)$(if $(SUBTARGET),/$(SUBTARGET))

SOC_CONF:=
ifeq ($(CONFIG_QEMU_DEFAULT_SOC_TYPE),y)
ifeq ($(findstring 2410,$(TARGETID)),2410)
  SOC_CONF:=s3c2410
endif
ifeq ($(findstring 2440,$(TARGETID)),2440)
  SOC_CONF:=s3c2440
endif
endif

ifeq ($(CONFIG_QEMU_S3C2410_SOC_TYPE),y)
  SOC_CONF:=s3c2410
endif

ifeq ($(CONFIG_QEMU_S3C2440_SOC_TYPE),y)
  SOC_CONF:=s3c2440
endif

define Package/$(PKG_NAME)/install
	echo "#TARGETID: $(TARGETID)"
	echo "#SOC_TYPE: $(SOC_CONF)"

	$(INSTALL_DIR)	$(1) $(BIN_DIR)
	$(INSTALL_BIN)	$(PKG_BUILD_DIR)/{qemu-img,qemu-io,qemu-nbd}	$(1)/
	$(INSTALL_BIN)  $(PKG_BUILD_DIR)/arm-softmmu/qemu-system-arm	$(1)/
	$(CP)           ./files/$(SOC_CONF)/                            $(1)/
	$(CP)		./files/run-$(SOC_CONF)-emulator.sh             $(BIN_DIR)/
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
