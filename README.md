# Electronics Xplore
## For those setting up the project for the first time
You need to do the following things before the project will build for **Orion rev4**:
* Within a terminal write `git submodule update --init --remote`: to pull the sub-repository BroCo (stored in a different repository)
* Within 'AV_SW_workspace\Orion_FW\System\BRoCo\include\Build\Build.h': 
	* **Comment:** #define BUILD_WITH_CAN_SOCKET_DRIVER
	* **Uncomment:** #define BUILD_WITH_FDCAN

