# Electronics Xplore
## For those setting up the project for the first time
You need to do the following things before the project will build for **Orion rev4**:
* `git submodule update --init --remote`: in a terminal in order to pull the submodule BroCo which is stored in a different repository
* Within 'AV_SW_workspace\Orion_FW\System\BRoCo\include\Build\Build.h': 
	* Comment BUILD_WITH_CAN_BUS
	* Uncomment BUILD_WITH_FDCAN

