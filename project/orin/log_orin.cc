nvsipl_camera: =========== not safety ============

nvsipl_camera: Checking SIPL version

nvsipl_camera: NvSIPL library version: 1.0.0

nvsipl_camera: NVSIPL header version: 1 0 0

nvsipl_camera: Parsing command line arguments

Platform configuration name: V1SIM623S3RU3200NB20_CPHY_x4
Verbosity level: 4
Raw output: disabled
ISP0 output: enabled
ISP1 output: enabled
ISP2 output: enabled
NvSciBuf path: disabled. Using NvMediaImage path.
Enabled FPS logging
Disabled Metadata logging
Disabled automatic recovery
Disabled NvSci
Disabled profiling
Disabled initialization profiling
Disabled ISP Statistics settings override
Input raw file(s):           save_vc0_100.raw 
Disabled Passive mode
Disabled retrieving/displaying NITO Metadata
nvsipl_camera: Setting verbosity level: 4

nvsipl_camera: Setting up signal handler

nvsipl_camera: Getting platform configuration for V1SIM623S3RU3200NB20_CPHY_x4

nvsipl_camera: Setting link masks

libnvsipl_query.so: CNvMPlatformCfgInfo.cpp : 91: ApplyMask: block:0 enabled
libnvsipl_query.so: CNvMPlatformCfgInfo.cpp : 100: ApplyMask: module:0 enabled
nvsipl_camera: ERROR: ming Init:69 w 1920 h 1559 s 5986560

nvsipl_camera: Creating camera master

nvsipl_camera: Setting up master

nvsipl_camera: Creating consumers

Platform: 
Platform configuration: V1SIM623S3RU3200NB20_CPHY_x4
Platform configuration description: IMX623 RGGB module in 4 lane CPHY mode

	Simulator Mode Enabled
	CSI Port: 2
	I2C Device 0
	Deserializer Name:MAX96712_Fusa
	             Description: Maxim 96712 Aggregator Fusa
	             I2C Address: 0x29
	CameraModule on link 0
	             Name: V1SIM623S3RU3200NB20
	             Description: Sony IMX623 RGGB module - 120-deg FOV, MIPI-IMX623, MAX96717F
		Serializer Name: MAX96717F
		           Address: 0x40
		Sensor Name: IMX623
		       Description: Sony IMX623 Sensor
		       ID: 0
		       Address: 0x1a
		EEPROM Name: M24C04
		       Address: 0x54



Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/CNvMCamera.cpp: 218: SetPlatformCfg: Create device block control
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/CNvMDeviceBlockMgr.cpp: 154: SetConfig: Create device block
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/CNvMDeviceBlockMgr.cpp: 162: SetConfig: Set device block config
Main: CDevBlkDriverLibs.cpp: 121: FindFirstLibPath: Checking "/proc/boot/safety" for drivers
Main: CDevBlkDriverLibs.cpp: 121: FindFirstLibPath: Checking "/proc/boot" for drivers
Main: CDevBlkDriverLibs.cpp: 121: FindFirstLibPath: Checking "/lib" for drivers
Main: CDevBlkDriverLibs.cpp: 121: FindFirstLibPath: Checking "/lib/dll" for drivers
Main: CDevBlkDriverLibs.cpp: 121: FindFirstLibPath: Checking "/lib/dll/pci" for drivers
Main: CDevBlkDriverLibs.cpp: 121: FindFirstLibPath: Checking "/usr/libnvidia" for drivers
Main: CDevBlkDriverLibs.cpp: 121: FindFirstLibPath: Checking "/usr/libnvidia/nvsipl_drv" for drivers
Main: CDevBlkDriverLibs.cpp: 133: FindFirstLibPath: Loading library "@E∫†"
Main: CDevBlkDriverLibs.cpp: 133: FindFirstLibPath: Loading library "PN∫†"
Main: CDevBlkDriverLibs.cpp: 133: FindFirstLibPath: Loading library "ê@∫†"
Main: CDevBlkDriverLibs.cpp: 133: FindFirstLibPath: Loading library "PN∫†"
Main: CDevBlkDriverLibs.cpp: 133: FindFirstLibPath: Loading library "†D∫†"
Main: CDevBlkDriverLibs.cpp: 133: FindFirstLibPath: Loading library "D∫†"
Main: CDevBlkDriverLibs.cpp: 133: FindFirstLibPath: Loading library "ù√†"
Main: CDevBlkDriverLibs.cpp: 133: FindFirstLibPath: Loading library "`ù√†"
Main: CDevBlkDriverLibs.cpp: 121: FindFirstLibPath: Checking "/usr/libnvidia/screen" for drivers
Main: CDevBlkDriverLibs.cpp: 121: FindFirstLibPath: Checking "/usr/lib" for drivers
Main: CDevBlkDriverLibs.cpp: 121: FindFirstLibPath: Checking "/usr/local/cuda/lib" for drivers
Main: CDevBlkDriverLibs.cpp: 121: FindFirstLibPath: Checking "/usr/local/driveworks/lib" for drivers
Main: CDevBlkDriverLibs.cpp: 121: FindFirstLibPath: Checking "/usr/libnvidia" for drivers
Main: CDevBlkDriverLibs.cpp: 121: FindFirstLibPath: Checking "/usr/libnvidia/nvsipl_drv" for drivers
Main: CDevBlkDriverLibs.cpp: 214: LoadDriversLibraries: Opening driver library "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_imx623.so"
Main: CDevBlkDriverLibs.cpp: 214: LoadDriversLibraries: Opening driver library "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_imx728.so"
Main: CDevBlkDriverLibs.cpp: 214: LoadDriversLibraries: Opening driver library "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_imx623vb2.so"
Main: CDevBlkDriverLibs.cpp: 214: LoadDriversLibraries: Opening driver library "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_imx728vb2.so"
Main: CDevBlkDriverLibs.cpp: 214: LoadDriversLibraries: Opening driver library "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_ov2311.so"
Main: CDevBlkDriverLibs.cpp: 214: LoadDriversLibraries: Opening driver library "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_ar0820_b_3461.so"
Main: CDevBlkDriverLibs.cpp: 214: LoadDriversLibraries: Opening driver library "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_ar0820.so"
Main: CDevBlkDriverLibs.cpp: 214: LoadDriversLibraries: Opening driver library "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_max96712tpg.so"
Main: CNvMDeserializerFactory.cpp: 61: LoadDeserializerDriver: Loading Deserializer driver from library "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_imx623.so"
Main: CNvMDeserializerFactory.cpp: 61: LoadDeserializerDriver: Loading Deserializer driver from library "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_imx728.so"
Main: CNvMDeserializerFactory.cpp: 61: LoadDeserializerDriver: Loading Deserializer driver from library "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_imx623vb2.so"
Main: CNvMDeserializerFactory.cpp: 61: LoadDeserializerDriver: Loading Deserializer driver from library "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_imx728vb2.so"
Main: CNvMDeserializerFactory.cpp: 61: LoadDeserializerDriver: Loading Deserializer driver from library "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_ov2311.so"
Main: CNvMDeserializerFactory.cpp: 61: LoadDeserializerDriver: Loading Deserializer driver from library "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_ar0820_b_3461.so"
Main: CNvMDeserializerFactory.cpp: 61: LoadDeserializerDriver: Loading Deserializer driver from library "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_ar0820.so"
Main: CNvMDeserializerFactory.cpp: 61: LoadDeserializerDriver: Loading Deserializer driver from library "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_max96712tpg.so"
Main: CDevBlkDriverLibs.cpp: 35: CDevBlkDriverLibs: Driver libraries are already loaded
Main: CNvMCameraModuleFactory.cpp: 131: LoadDriver: Loading CameraModule driver from library "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_imx623.so"
Main: CNvMCameraModuleFactory.cpp: 177: LoadDriver: Mapping module name "V623S2-195V1-SVS" to module driver "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_imx623.so"
Main: CNvMCameraModuleFactory.cpp: 131: LoadDriver: Loading CameraModule driver from library "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_imx728.so"
Main: CNvMCameraModuleFactory.cpp: 177: LoadDriver: Mapping module name "V728S1-120V0" to module driver "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_imx728.so"
Main: CNvMCameraModuleFactory.cpp: 177: LoadDriver: Mapping module name "V728S1-120V1-FWC" to module driver "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_imx728.so"
Main: CNvMCameraModuleFactory.cpp: 177: LoadDriver: Mapping module name "V728S1-120V1-SCF" to module driver "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_imx728.so"
Main: CNvMCameraModuleFactory.cpp: 177: LoadDriver: Mapping module name "V728S1-070V1-SCR" to module driver "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_imx728.so"
Main: CNvMCameraModuleFactory.cpp: 177: LoadDriver: Mapping module name "V728S1-030V1-FTC" to module driver "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_imx728.so"
Main: CNvMCameraModuleFactory.cpp: 177: LoadDriver: Mapping module name "V728S1-030V1-RC" to module driver "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_imx728.so"
Main: CNvMCameraModuleFactory.cpp: 131: LoadDriver: Loading CameraModule driver from library "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_imx623vb2.so"
Main: CNvMCameraModuleFactory.cpp: 177: LoadDriver: Mapping module name "V1SIM623S3RU3200NB20" to module driver "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_imx623vb2.so"
Main: CNvMCameraModuleFactory.cpp: 131: LoadDriver: Loading CameraModule driver from library "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_imx728vb2.so"
Main: CNvMCameraModuleFactory.cpp: 177: LoadDriver: Mapping module name "V1SIM728S1RU3120NB20" to module driver "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_imx728vb2.so"
Main: CNvMCameraModuleFactory.cpp: 177: LoadDriver: Mapping module name "V1SIM728S1RU3070HB20" to module driver "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_imx728vb2.so"
Main: CNvMCameraModuleFactory.cpp: 177: LoadDriver: Mapping module name "V1SIM728S1RU3030NB20" to module driver "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_imx728vb2.so"
Main: CNvMCameraModuleFactory.cpp: 131: LoadDriver: Loading CameraModule driver from library "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_ov2311.so"
Main: CNvMCameraModuleFactory.cpp: 177: LoadDriver: Mapping module name "LI-OV2311-VCSEL-GMSL2-60H" to module driver "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_ov2311.so"
Main: CNvMCameraModuleFactory.cpp: 177: LoadDriver: Mapping module name "LI-OV2311-VCSEL-GMSL2-60H_L" to module driver "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_ov2311.so"
Main: CNvMCameraModuleFactory.cpp: 131: LoadDriver: Loading CameraModule driver from library "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_ar0820_b_3461.so"
Main: CNvMCameraModuleFactory.cpp: 177: LoadDriver: Mapping module name "AR0820C120FOV_24BIT_RGGB" to module driver "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_ar0820_b_3461.so"
Main: CNvMCameraModuleFactory.cpp: 177: LoadDriver: Mapping module name "AR0820C70FOV_24BIT_RGGB" to module driver "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_ar0820_b_3461.so"
Main: CNvMCameraModuleFactory.cpp: 177: LoadDriver: Mapping module name "VC0820C120R24" to module driver "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_ar0820_b_3461.so"
Main: CNvMCameraModuleFactory.cpp: 177: LoadDriver: Mapping module name "VC0820C070R24" to module driver "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_ar0820_b_3461.so"
Main: CNvMCameraModuleFactory.cpp: 131: LoadDriver: Loading CameraModule driver from library "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_ar0820.so"
Main: CNvMCameraModuleFactory.cpp: 177: LoadDriver: Mapping module name "F008A030BM0A_RCCB" to module driver "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_ar0820.so"
Main: CNvMCameraModuleFactory.cpp: 177: LoadDriver: Mapping module name "F008A030BM0A_24BIT_RCCB" to module driver "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_ar0820.so"
Main: CNvMCameraModuleFactory.cpp: 177: LoadDriver: Mapping module name "F008A030BM0A_RCCB_BINNING" to module driver "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_ar0820.so"
Main: CNvMCameraModuleFactory.cpp: 177: LoadDriver: Mapping module name "F008A120BM0A_RCCB" to module driver "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_ar0820.so"
Main: CNvMCameraModuleFactory.cpp: 177: LoadDriver: Mapping module name "F008A120BM0A_24BIT_RCCB" to module driver "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_ar0820.so"
Main: CNvMCameraModuleFactory.cpp: 177: LoadDriver: Mapping module name "F008A120BM0A_RCCB_BINNING" to module driver "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_ar0820.so"
Main: CNvMCameraModuleFactory.cpp: 177: LoadDriver: Mapping module name "F008A030BM0A_RGGB" to module driver "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_ar0820.so"
Main: CNvMCameraModuleFactory.cpp: 177: LoadDriver: Mapping module name "F008A030BM0A_24BIT_RGGB" to module driver "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_ar0820.so"
Main: CNvMCameraModuleFactory.cpp: 177: LoadDriver: Mapping module name "F008A030BM0A_RGGB_BINNING" to module driver "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_ar0820.so"
Main: CNvMCameraModuleFactory.cpp: 177: LoadDriver: Mapping module name "F008A120BM0A_RGGB" to module driver "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_ar0820.so"
Main: CNvMCameraModuleFactory.cpp: 177: LoadDriver: Mapping module name "F008A120BM0A_24BIT_RGGB" to module driver "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_ar0820.so"
Main: CNvMCameraModuleFactory.cpp: 177: LoadDriver: Mapping module name "F008A120BM0A_RGGB_BINNING" to module driver "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_ar0820.so"
Main: CNvMCameraModuleFactory.cpp: 177: LoadDriver: Mapping module name "F008A070BM0A_24BIT_RGGB" to module driver "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_ar0820.so"
Main: CNvMCameraModuleFactory.cpp: 177: LoadDriver: Mapping module name "F008A030RM0A" to module driver "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_ar0820.so"
Main: CNvMCameraModuleFactory.cpp: 177: LoadDriver: Mapping module name "F008A070RM0A" to module driver "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_ar0820.so"
Main: CNvMCameraModuleFactory.cpp: 177: LoadDriver: Mapping module name "F008A120RM0A" to module driver "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_ar0820.so"
Main: CNvMCameraModuleFactory.cpp: 177: LoadDriver: Mapping module name "F008A120RM0A_SIDE" to module driver "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_ar0820.so"
Main: CNvMCameraModuleFactory.cpp: 131: LoadDriver: Loading CameraModule driver from library "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_max96712tpg.so"
Main: CNvMCameraModuleFactory.cpp: 177: LoadDriver: Mapping module name "MAX96712TPG" to module driver "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_max96712tpg.so"
Main: CNvMCameraModuleFactory.cpp: 177: LoadDriver: Mapping module name "MAX96712TPG_YUV_8" to module driver "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_max96712tpg.so"
nvsipl_camera: Creating consumer for output:1 of sensor:0

nvsipl_camera: Profiler initialization for output:1 of sensor:0

nvsipl_camera: Consumer initialization for output:1 of sensor:0

nvsipl_camera: Creating consumer for output:2 of sensor:0

nvsipl_camera: Profiler initialization for output:2 of sensor:0

nvsipl_camera: Consumer initialization for output:2 of sensor:0

nvsipl_camera: Creating consumer for output:3 of sensor:0

nvsipl_camera: Profiler initialization for output:3 of sensor:0

nvsipl_camera: Consumer initialization for output:3 of sensor:0

nvsipl_camera: Initializing master interface

Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/CNvMCamera.cpp: 709: Init: Initializing device blocks
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/CNvMDeviceBlockMgr.cpp: 242: Init: Initializing worker thread for block:0
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 34: ThreadCreate: Creating thread:DEVBLK_WORKER_0
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/CNvMDeviceBlockMgr.cpp: 248: Init: Initialized worke: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 117: ThreadSetName: Setting thread  thread for block:0
: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 117: ThreadSetName: Setting thread name:DEVBLK_WORKER_0
DEVBLK_WORKER_0: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 145: m_Func: Running thread:DEVBLK_WORKER_0
DEVBLK_WORKER_0: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 148: m_Func: Calling ThreadFunc for thread:DEVBLK_WORKER_0
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/CNvMDeviceBlockMgr.cpp: 205: WorkersDoAction: All worker threads starting action:1
DEVBLK_WORKER_0: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 148: m_Func: Calling ThreadFunc for thread:DEVBLK_WORKER_0
DEVBLK_WORKER_0: CNvMTransportLink_Max96712_96717F.cpp: 715: Init: Initializing link 0
DEVBLK_WORKER_0: CNvMTransportLink_Max96712_96717F.cpp: 66: DumpLinkParams: Link parameters
DEVBLK_WORKER_0: CNvMTransportLink_Max96712_96717F.cpp: 67: DumpLinkParams: Link Index: 0 
DEVBLK_WORKER_0: CNvMTransportLink_Max96712_96717F.cpp: 68: DumpLinkParams: Broadcast serializer addr: 0x0 
DEVBLK_WORKER_0: CNvMTransportLink_Max96712_96717F.cpp: 69: DumpLinkParams: Serializer addr: 0xf1 
DEVBLK_WORKER_0: CNvMTransportLink_Max96712_96717F.cpp: 72: DumpLinkParams: 
Broadcast sensor addr: 0x0 
DEVBLK_WORKER_0: CNvMTransportLink_Max96712_96717F.cpp: 74: DumpLinkParams: Sensor addr: 0xf1 
DEVBLK_WORKER_0: CNvMTransportLink_Max96712_96717F.cpp: 76: DumpLinkParams: VCID: 0 
DEVBLK_WORKER_0: CNvMTransportLink_Max96712_96717F.cpp: 78: DumpLinkParams: Embedded data type: false 
DEVBLK_WORKER_0: CNvMTransportLink_Max96712_96717F.cpp: 80: DumpLinkParams: Trigger mode sync: true 
DEVBLK_WORKER_0: CNvMTransportLink_Max96712_96717F.cpp: 82: DumpLinkParams: Frame rate: 30.00 fps 
DEVBLK_WORKER_0: CNvMTransportLink_Max96712_96717F.cpp: 84: DumpLinkParams: 
Broadcast EEPROM addr: 0x0 
DEVBLK_WORKER_0: CNvMTransportLink_Max96712_96717F.cpp: 85: DumpLinkParams: EEPROM addr: 0xf1 
DEVBLK_WORKER_0: CNvMTransportLink_Max96712_96717F.cpp: 86: DumpLinkParams: Simulator mode: 1 
DEVBLK_WORKER_0: CNvMTransportLink_Max96712_96717F.cpp: 87: DumpLinkParams: Passive mode: 0 
DEVBLK_WORKER_0: CNvMTransportLink_Max96712_96717F.cpp: 765: PostSensorInit: Read revision
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/CNvMDeviceBlockMgr.cpp: 231: WorkersDoAction: All worker threads finished action:1
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/CNvMCamera.cpp: 747: Init: Number of device blocks: 1
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/pipelineMgr/spmgr/pipeline/CNvMSensorPipeline.cpp: 122: CDIInit: Creating CDI block
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 34: ThreadCreate: Creating thread:SIPL_CDI_0
: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 130: m_FuncStatic: Thread created
Main: /git/dirty/git-mas-master_qnx/camera/fusa/sipl/src/core/utils/CNvMBufferPool.hpphreadSetName: Setting thread name:SIPL_CDI_0
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMBufferPool.hpp: 101: Init: Creating buffer pool with 2 buffers
SIPL_CDI_0: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 145: m_Func: Running thread:SIPL_CDI_0
SIPL_CDI_0: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 148: m_Func: Calling ThreadFunc for thread:SIPL_CDI_0
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/pipelineMgr/spmgr/pipeline/CNvMSensorPipeline.cpp: 85: ISPInit: Creating ISP block
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/pipelineMgr/spmgr/pipeline/blocks/CNvMISPBlock.cpp: 154: Init: Fuse ID: 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 34: ThreadCreate: Creating thread:SIPL_ICP_ISP_0
: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 130: m_FuncStatic: Thread created
: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 117: ThreadSetName: Setting thread name:SIPL_ICP_ISP_0
SIPL_ICP_ISP_0: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 145: m_Func: Running thread:SIPL_ICP_ISP_0
SIPL_ICP_ISP_0: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 148: m_Func: Calling ThreadFunc for thread:SIPL_ICP_ISP_0
Main: CNvMMAX96712CameraModule.cpp: 1076: GetErrorSize: GetErrorSize not implemented for sensor
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/pipelineMgr/spmgr/pipeline/CNvMSensorPipeline.cpp: 255: RegisterImageGroupBuffers: Creating ICP buffer pool of size: 6
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMBufferPool.hpp: 101: Init: Creating buffer pool with 6 buffers
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/pipelineMgr/spmgr/pipeline/CNvMSensorPipeline.cpp: 31: OnBufferRelease: Wake up thread
SIPL_ICP_ISP_0: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 148: m_Func: Calling ThreadFunc for thread:SIPL_ICP_ISP_0
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/pipelineMgr/spmgr/pipeline/CNvMSensorPipeline.cpp: 31: OnBufferRelease: Wake up thread
SIPL_ICP_ISP_0: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 148: m_Func: Calling ThreadFunc for thread:SIPL_ICP_ISP_0
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/pipelineMgr/spmgr/pipeline/CNvMSensorPipeline.cpp: 31: OnBufferRelease: Wake up thread
SIPL_ICP_ISP_0: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 148: m_Func: Calling ThreadFunc for thread:SIPL_ICP_ISP_0
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/pipelineMgr/spmgr/pipeline/CNvMSensorPipeline.cpp: 31: OnBufferRelease: Wake up thread
SIPL_ICP_ISP_0: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 148: m_Func: Calling ThreadFunc for thread:SIPL_ICP_ISP_0
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/pipelineMgr/spmgr/pipeline/CNvMSensorPipeline.cpp: 31: OnBufferRelease: Wake up thread
SIPL_ICP_ISP_0: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 148: m_Func: Calling ThreadFunc for thread:SIPL_ICP_ISP_0
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/pipelineMgr/spmgr/pipeline/CNvMSensorPipeline.cpp: 31: OnBufferRelease: Wake up thread
SIPL_ICP_ISP_0: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 148: m_Func: Calling ThreadFunc for thread:SIPL_ICP_ISP_0
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/pipelineMgr/spmgr/pipeline/CNvMSensorPipeline.cpp: 362: RegisterImageBuffer: Creating ISP[0] buffer pool of size: 4
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMBufferPool.hpp: 101: Init: Creating buffer pool with 4 buffers
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/pipelineMgr/spmgr/pipeline/CNvMSensorPipeline.cpp: 31: OnBufferRelease: Wake up thread
SIPL_ICP_ISP_0: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 148: m_Func: Calling ThreadFunc for thread:SIPL_ICP_ISP_0
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/pipelineMgr/spmgr/pipeline/CNvMSensorPipeline.cpp: 31: OnBufferRelease: Wake up thread
SIPL_ICP_ISP_0: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 148: m_Func: Calling ThreadFunc for thread:SIPL_ICP_ISP_0
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/pipelineMgr/spmgr/pipeline/CNvMSensorPipeline.cpp: 31: OnBufferRelease: Wake up thread
SIPL_ICP_ISP_0: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 148: m_Func: Calling ThreadFunc for thread:SIPL_ICP_ISP_0
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/pipelineMgr/spmgr/pipeline/CNvMSensorPipeline.cpp: 31: OnBufferRelease: Wake up thread
SIPL_ICP_ISP_0: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 148: m_Func: Calling ThreadFunc for thread:SIPL_ICP_ISP_0
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/pipelineMgr/spmgr/pipeline/CNvMSensorPipeline.cpp: 362: RegisterImageBuffer: Creating ISP[1] buffer pool of size: 4
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMBufferPool.hpp: 101: Init: Creating buffer pool with 4 buffers
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/pipelineMgr/spmgr/pipeline/CNvMSensorPipeline.cpp: 31: OnBufferRelease: Wake up thread
SIPL_ICP_ISP_0: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 148: m_Func: Calling ThreadFunc for thread:SIPL_ICP_ISP_0
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/pipelineMgr/spmgr/pipeline/CNvMSensorPipeline.cpp: 31: OnBufferRelease: Wake up thread
SIPL_ICP_ISP_0: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 148: m_Func: Calling ThreadFunc for thread:SIPL_ICP_ISP_0
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/pipelineMgr/spmgr/pipeline/CNvMSensorPipeline.cpp: 31: OnBufferRelease: Wake up thread
SIPL_ICP_ISP_0: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 148: m_Func: Calling ThreadFunc for thread:SIPL_ICP_ISP_0
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/pipelineMgr/spmgr/pipeline/CNvMSensorPipeline.cpp: 31: OnBufferRelease: Wake up thread
SIPL_ICP_ISP_0: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 148: m_Func: Calling ThreadFunc for thread:SIPL_ICP_ISP_0
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/pipelineMgr/spmgr/pipeline/CNvMSensorPipeline.cpp: 362: RegisterImageBuffer: Creating ISP[2] buffer pool of size: 4
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMBufferPool.hpp: 101: Init: Creating buffer pool with 4 buffers
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/pipelineMgr/spmgr/pipeline/CNvMSensorPipeline.cpp: 31: OnBufferRelease: Wake up thread
SIPL_ICP_ISP_0: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 148: m_Func: Calling ThreadFunc for thread:SIPL_ICP_ISP_0
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/pipelineMgr/spmgr/pipeline/CNvMSensorPipeline.cpp: 31: OnBufferRelease: Wake up thread
SIPL_ICP_ISP_0: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 148: m_Func: Calling ThreadFunc for thread:SIPL_ICP_ISP_0
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/pipelineMgr/spmgr/pipeline/CNvMSensorPipeline.cpp: 31: OnBufferRelease: Wake up thread
SIPL_ICP_ISP_0: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 148: m_Func: Calling ThreadFunc for thread:SIPL_ICP_ISP_0
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/pipelineMgr/spmgr/pipeline/CNvMSensorPipeline.cpp: 31: OnBufferRelease: Wake up thread
SIPL_ICP_ISP_0: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 148: m_Func: Calling ThreadFunc for thread:SIPL_ICP_ISP_0
nvsipl_camera: Opened NITO file for module "V1SIM623S3RU3200NB20", file name: "/proc/boot/V1SIM623S3RU3200NB20.nito" 
nvsipl_camera: data from NITO file loaded for module "V1SIM623S3RU3200NB20"

Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/pipelineMgr/spmgr/pipeline/blocks/CNvMCDIBlock.cpp: 103: SetSensorControl: Wake up threadSIPL_CDI_0: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 148: m_Func: Calling ThreadFunc for thread:SIPL_CDI_0
SIPL_CDI_0: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/pipelineMgr/spmgr/pipeline/blocks/CNvMCDIBlock.cpp: 191: PrintSensorControl: i: 0 exposureTime: 0.009981 sensorGain: 7.943282
SIPL_CDI_0: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/pipelineMgr/spmgr/pipeline/blocks/CNvMCDIBlock.cpp: 191: PrintSensorControl: i: 1 exposureTime: 0.009981 sensorGain: 16.000000
SIPL_CDI_0: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/pipelineMgr/spmgr/pipeline/blocks/CNvMCDIBlock.cpp: 191: PrintSensorControl: i: 2 exposureTime: 0.009981 sensorGain: 1.000000
SIPL_CDI_0: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 148: m_Func: Calling ThreadFunc for thread:SIPL_CDI_0
nvsipl_camera: Pipeline: 0, NOTIF_INFO_CDI_PROCESSING_DONE

Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/CNvMCamera.cpp: 500: RegisterAutoControlPlugin: RegisterAutoControlPlugin type:0 successful for pipeline:0
nvsipl_camera: Starting master

Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/pipelineMgr/spmgr/pipeline/blocks/CNvMCDIBlock.cpp: 125: Start: Start the thread
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/pipelineMgr/spmgr/pipeline/CNvMSensorPipeline.cpp: 684: StartProcessing: Start the thread
SIPL_ICP_ISP_0: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 148: m_Func:  All worker threads starting action:2
SIPL_ICP_ISP_0: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 148: m_Func: Calling ThreadFunc for thread:SIPL_ICP_ISP_0
DEVBLK_WORKER_0: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 148: m_Func: Calling ThreadFunc for thread:DEVBLK_WORKER_0
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/CNvMDeviceBlockMgr.cpp: 231: WorkersDoAction: All worker threads finished action:2
Enter 'les' followed by sensor ID to enable LED
Enter 'lds' followed by sensor ID to disable LED
Enter 'dl' followed by sensor ID to disable the link
Enter 'el' followed by sensor ID to enable the link without module reset
Enter 'elr' followed by sensor ID to enable the link with module reset
Enter 'q' to quit the application
-
nvsipl_camera: ERROR: ming FillRawBuffer:101 w 3840 h 1536 s 5986560

End of file reached for: save_vc0_100.raw
SIPL_ICP_ISP_0: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/pipelineMgr/spmgr/pipeline/CNvMSensorPipeline.cpp: 1146: GetFeederBuffer: End of file reached
SIPL_ICP_ISP_0: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 171: m_Func: Exiting thread:SIPL_ICP_ISP_0
nvsipl_camera: Queue timeout

nvsipl_camera: Queue timeout

nvsipl_camera: Queue timeout

nvsipl_camera: Queue timeout

nvsipl_camera: Queue timeout

nvsipl_camera: Queue timeout

Output
Sensor0_Out1	Frame rate (fps):		0
Sensor0_Out2	Frame rate (fps):		0
Sensor0_Out3	Frame rate (fps):		0

nvsipl_camera: Stopping master

Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/CNvMDeviceBlockMgr.cpp: 205: WorkersDoAction: All worker threads starting action:3
DEVBLK_WORKER_0: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 148: m_Func: Calling ThreadFunc for thread:DEVBLK_WORKER_0
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/CNvMDeviceBlockMgr.cpp: 231: WorkersDoAction: All worker threads finished action:3
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/pipelineMgr/spmgr/pipeline/blocks/CNvMCDIBlock.cpp: 132: Stop: Stop the thread
nvsipl_camera: Deinitializing devblk notificationHandler: 0

nvsipl_camera: Queue timeout

nvsipl_camera: Queue timeout

nvsipl_camera: Deinitializing pipeline notificationHandler: 0

nvsipl_camera: Queue timeout

nvsipl_camera: Deinitializing frameCompletionQueueHandler: 0

nvsipl_camera: Queue timeout

nvsipl_camera: Deinitializing consumer Sensor0_Out1

nvsipl_camera: Deinitializing consumer Sensor0_Out2

nvsipl_camera: Deinitializing consumer Sensor0_Out3

nvsipl_camera: Deinitializing profiler Sensor0_Out1

nvsipl_camera: Deinitializing profiler Sensor0_Out2

nvsipl_camera: Deinitializing profiler Sensor0_Out3

nvsipl_camera: Deinitializing master

Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/pipelineMgr/spmgr/pipeline/CNvMSensorPipeline.cpp: 839: Deinit: Quit the thread
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 55: ThreadQuit: Quitting thread:SIPL_ICP_ISP_0
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 65: ThreadQuit: Waiting for thread:SIPL_ICP_ISP_0 to end
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/pipelineMgr/spmgr/pipeline/blocks/CNvMCDIBlock.cpp: 146: Deinit: Quit the thread
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 55: ThreadQuit: Quitting thread:SIPL_CDI_0
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 65: ThreadQuit: Waiting for thread:SIPL_CDI_0 to end
SIPL_CDI_0: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 171: m_Func: Exiting thread:SIPL_CDI_0
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/CNvMCamera.cpp: 2772: Deinit: Deinit device block in CNvMCamera::Deinit()
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/CNvMDeviceBlockMgr.cpp: 205: WorkersDoAction: All worker threads starting action:4
DEVBLK_WORKER_0: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 148: m_Func: Calling ThreadFunc for thread:DEVBLK_WORKER_0
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/CNvMDeviceBlockMgr.cpp: ead.cpp: 171: m_Func: Exiting thread:DEVBLK_WORKER_0
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/CNvMDeviceBlockMgr.cpp: 231: WorkersDoAction: All worker threads finished action:4
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 55: ThreadQuit: Quitting thread:DEVBLK_WORKER_0
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 65: ThreadQuit: Waiting for thread:DEVBLK_WORKER_0 to end
Main: CDevBlkDriverLibs.cpp: 66: ~CDevBlkDriverLibs: Closing library: "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_imx623.so"
Main: CDevBlkDriverLibs.cpp: 68: ~CDevBlkDriverLibs: dlclose
Main: CDevBlkDriverLibs.cpp: 66: ~CDevBlkDriverLibs: Closing library: "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_imx728.so"
Main: CDevBlkDriverLibs.cpp: 68: ~CDevBlkDriverLibs: dlclose
Main: CDevBlkDriverLibs.cpp: 66: ~CDevBlkDriverLibs: Closing library: "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_imx623vb2.so"
Main: CDevBlkDriverLibs.cpp: 68: ~CDevBlkDriverLibs: dlclose
Main: CDevBlkDriverLibs.cpp: 66: ~CDevBlkDriverLibs: Closing library: "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_imx728vb2.so"
Main: CDevBlkDriverLibs.cpp: 68: ~CDevBlkDriverLibs: dlclose
Main: CDevBlkDriverLibs.cpp: 66: ~CDevBlkDriverLibs: Closing library: "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_ov2311.so"
Main: CDevBlkDriverLibs.cpp: 68: ~CDevBlkDriverLibs: dlclose
Main: CDevBlkDriverLibs.cpp: 66: ~CDevBlkDriverLibs: Closing library: "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_ar0820_b_3461.so"
Main: CDevBlkDriverLibs.cpp: 68: ~CDevBlkDriverLibs: dlclose
Main: CDevBlkDriverLibs.cpp: 66: ~CDevBlkDriverLibs: Closing library: "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_ar0820.so"
Main: CDevBlkDriverLibs.cpp: 68: ~CDevBlkDriverLibs: dlclose
Main: CDevBlkDriverLibs.cpp: 66: ~CDevBlkDriverLibs: Closing library: "/usr/libnvidia/nvsipl_drv/libnvsipl_devblk_drv_max96712tpg.so"
Main: CDevBlkDriverLibs.cpp: 68: ~CDevBlkDriverLibs: dlclose
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/CNvMDeviceBlockMgr.cpp: 205: WorkersDoAction: All worker threads starting action:4
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/CNvMDeviceBlockMgr.cpp: 231: WorkersDoAction: All worker threads finished action:4
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 55: ThreadQuit: Quitting thread:
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 55: ThreadQuit: Quitting thread:
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 55: ThreadQuit: Quitting thread:
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 55: ThreadQuit: Quitting thread:
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 55: ThreadQuit: Quitting thread:
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 55: ThreadQuit: Quitting thread:DEVBLK_WORKER_0
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 55: ThreadQuit: Quitting thread:
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 55: ThreadQuit: Quitting thread:
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 55: ThreadQuit: Quitting thread:
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 55: ThreadQuit: Quitting thread:
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 55: ThreadQuit: Quitting thread:
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 55: ThreadQuit: Quitting thread:
SUCCESS
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMBuffer.hpp: 63: ~CNvMBuffer: Destroying buffer with reference count: 1
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/pipelineMgr/spmgr/pipeline/blocks/CNvMCDIBlock.cpp: 146: Deinit: Quit the thread
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 55: ThreadQuit: Quitting thread:SIPL_CDI_0
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 55: ThreadQuit: Quitting thread:SIPL_CDI_0
Main: /dvs/git/dirty/git-master_qnx/camera/fusa/sipl/src/core/utils/CNvMThread.cpp: 55: ThreadQuit: Quitting thread:SIPL_ICP_ISP_0
