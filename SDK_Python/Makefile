so_dir := build/lib.linux*
so_install_dir := /usr/lib/python2.7/dist-packages
so_file := iteadcfunction.so
so_file += iteadserial.so
so_file += iteadspi.so
so_file += iteadwire.so

py_dir := .
py_install_dir := $(so_install_dir)
py_file := itead_config.py
py_file += iteadsdk.py
py_file += BOARD_ITEADUINO_PLUS_GPIO.py  
py_file += BOARD_ITEAD_CORE_EVB_GPIO.py  
py_file += BOARD_RASPBERRY_PI_RV2_GPIO.py          
py_file += BOARD_RASPBERRY_PI_MODEL_BPLUS_GPIO.py
py_file += BOARD_BEAGLE_BONE_BLACK_GPIO.py  


.PHONY: install 
install:
	cd core && \
	./setup.py build && \
	cp $(addprefix $(so_dir)/,$(so_file)) $(so_install_dir) && \
	chmod 755 $(addprefix $(so_install_dir)/,$(so_file)) && \
	cd -
	cp $(addprefix $(py_dir)/,$(py_file)) $(py_install_dir)
	chmod 755 $(addprefix $(py_install_dir)/,$(py_file))
	@echo "install done!"



.PHONY: uninstall
uninstall:
	rm -f $(addprefix $(so_install_dir)/,$(so_file))
	rm -f $(addprefix $(py_install_dir)/,$(py_file))
	rm -f $(addprefix $(py_install_dir)/,$(subst .py,.pyc,$(py_file)))
	@echo "uninstall done!"



.PHONY: clean
clean:
	rm -rf core/build
	@echo "clean done!"

