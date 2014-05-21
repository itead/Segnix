###############################################################################
# Select libraries needed to be installed
# 
# if INSTALL_LIBRARIES_* is y, the library will be compiled and installed to
# your system.
# if INSTALL_LIBRARIES_* is not "y", the library will be ignored.
#
###############################################################################

#
# User can replace "y" with "n" to ignore the corresponding library.
# Actually, commenting the line which begins with INSTALL_LIBRARIES_* by
# inserting "#" at the head will also get the same result.
# NOTE:
#     User MUST select one INSTALL_LIBRARIES_* as "y" at least.
#
INSTALL_LIBRARIES_itead_GSM-GPRS-GPS    := y
INSTALL_LIBRARIES_itead_LiquidCrystal   := y
INSTALL_LIBRARIES_itead_SSD1306         := y
INSTALL_LIBRARIES_itead_GFX             := y
INSTALL_LIBRARIES_itead_TinyGPS         := y



###############################################################################
# DO NOT TOUCH the fellowing content from here UNLESS you know what you are 
# doing !
###############################################################################

#
# itead_SSD1306 depends on itead_GFX
#
ifeq ($(INSTALL_LIBRARIES_itead_SSD1306),y)
INSTALL_LIBRARIES_itead_GFX 			:= y
endif



#
# export for other programs.
#
export INSTALL_LIBRARIES_itead_GSM-GPRS-GPS
export INSTALL_LIBRARIES_itead_LiquidCrystal
export INSTALL_LIBRARIES_itead_SSD1306
export INSTALL_LIBRARIES_itead_GFX
export INSTALL_LIBRARIES_itead_TinyGPS