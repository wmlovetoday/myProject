
std::unique_ptr<CNvSIPLMaster> upMaster(nullptr);
// INvSIPLQuery come from libnvsipl_query.so
//  - maintains a database of supported external image devices and their properties
//  - maintains a database of platform configurations

// the database include: SensorInfo / EEPROMInfo / SerInfo / CameraModuleInfo / DeserInfo / PlatformCfg

// SIPL Query provides API calls to:
// Parse the default database
// Get information about all devices supported by the library
// Get a list of all supported platform configurations
// Retrieve a specific platform configuration by name
// Parse a user-provided platform_config.json file to override the PlatformCfg structs in the SIPL Query database
// Apply a mask to enable only specific deserializer links in a specific platform configuration
main() {
  // sep 1. Querying Platform Configuration
  // get a list of supported platform configurations
  auto pQuery = INvSIPLQuery::GetInstance();

  pQuery->ParseDatabase();
  PlatformCfg oPlatformCfg;
  status = pQuery->GetPlatformCfg(cmdline.sConfigName, oPlatformCfg);
  status = pQuery->ApplyMask(oPlatformCfg, cmdline.vMasks);

  // sep 2.
  auto upMaster = unique_ptr<CNvSIPLMaster>(new CNvSIPLMaster());  // <1>
  status = upMaster->Setup(&oPlatformCfg);

  std::unique_ptr<CPipelineNotificationHandler> upNotificationHandler(nullptr);
  upNotificationHandler.reset(new CPipelineNotificationHandler);
  upMaster->SetPipelineNotifier(uSensor, upNotificationHandler.get());

  // Create descriptor for NvMediaImage consumer
  vector<INvSIPLClient::ConsumerDesc> voConsDesc;
  INvSIPLClient::ConsumerDesc oConDesc{};
  oConDesc.sName = "Sensor" + to_string(uSensor) + "_Out" + to_string(int(eOutput));
  oConDesc.uSensorIndex = uSensor;
  oConDesc.eOutputType = eOutput;
  voConsDesc.push_back(oConDesc);
  // Set output descriptors
  status = upMaster->SetOutputs(voConsDesc);

  upMaster->Init();  // initialize SIPL,This programs the external devices to prepare them to stream, receive, and
                     // process images. Note that this step prepares SIPL to capture and process images but does not
                     // actually start the process.
  status = upMaster->AllocateAndRegisterBuffers(uSensor, !cmdline.bDisableISP0, !cmdline.bDisableISP1);

  // NITO CTL
  status = LoadNITOFile(module.name, blob, bDefaultNitoLoaded);
  status = upMaster->RegisterAutoControl(uSensor, INvSIPLAutoControl::NV_PLUGIN, nullptr, blob);
  upCustomPlugins1[uSensor].reset(new CAutoControlPlugin);
  status =
      upMaster->RegisterAutoControl(uSensor, INvSIPLAutoControl::CUSTOM_PLUGIN0, upCustomPlugins1[uSensor].get(), blob);

  upCustomPlugins2[uSensor].reset(new CAutoControlPlugin);
  upCustomPlugins2[uSensor]->m_targetLuma = 0.80F;
  status =
      upMaster->RegisterAutoControl(uSensor, INvSIPLAutoControl::CUSTOM_PLUGIN1, upCustomPlugins2[uSensor].get(), blob);

  // display
  std::unique_ptr<CComposite> upComposite(nullptr);
  upComposite.reset(new CComposite());
  upComposite->Init();

  // Creating consumers
  vector<unique_ptr<CNvSIPLConsumer>> vupConsumers;
  auto pClientDesc = upMaster->GetClientDesc(uSensor, eOutput);
  // Create NvSIPL consumer using client descriptor
  auto upCons = unique_ptr<CNvSIPLConsumer>(new CNvSIPLConsumer());
  auto outIndex = int(eOutput) - int(INvSIPLClient::ConsumerDesc::OutputType::ICP);
  status = upComposite->RegisterSource(grpIndex, module.linkIndex, outIndex, uID);
  if (cmdline.autoPlugin != INvSIPLAutoControl::MAX_NUM_PLUGINS) {
    upMaster->SetPlugin(uSensor, cmdline.autoPlugin);
  } else {
    upMaster->SetPlugin(uSensor, eDefaultPlugin);
  }
  status = upCons->Init(pClientDesc, upComposite.get(), uID, cmdline.sPrefix);
  if (cmdline.bShowMetadata) {
    upCons->EnableMetadataLogging();
  }
  vupConsumers.push_back(move(upCons));

  upComposite->Start();
  upMaster->Start();

  for (auto &cons : vupConsumers) {
    auto fps = (cons->m_uFrameCount * 1000.0) / (uTimeElapsedMs);
    cout << cons->m_pDesc->oConsDesc.sName << "\t" << fps << endl;
    cons->m_uFrameCount = 0;
  }
}

// <1>

class CNvSIPLConsumer : public INvSIPLClient::INvMCallback {};
class INvSIPLClient {
  class INvSIPLBuffer {};
  class INvSIPLNvMBuffer : public INvSIPLBuffer {};
  class INvMCallback {};
  static std::unique_ptr<INvSIPLClient> Create();
  virtual SIPLStatus Init(ClientDesc *pDesc, INvMCallback *pCallback) = 0;
  virtual SIPLStatus Start(void) = 0;
  virtual SIPLStatus Stop(void) = 0;
  struct ImageMetaData {
    /** Holds the TSC timestamp of the frame capture. */
    std::uint64_t frameCaptureTSC;
    /** Holds the parsed embedded data frame number of exposures info for the captured frame.*/
    uint32_t numExposures;
    /** Holds the parsed embedded data sensor exposure info for the captured frame. */
    NvMediaISCExposure sensorExpInfo;
    /** Holds the parsed embedded data sensor white balance info for the captured frame. */
    NvMediaISCWhiteBalance sensorWBInfo;
    /** Holds the parsed embedded data sensor PWL info for the captured frame. */
    NvMediaISCPWL sensorPWLInfo;
    /** Holds the parsed embedded data sensor crc info for the captured frame. */
    NvMediaISCCRC sensorCRCInfo;
    /** Holds the parsed embedded data frame report info for the captured frame. */
    NvMediaISCFrameReport sensorReportInfo;
    /** Holds the parsed embedded data sensor temperature info for the captured frame. */
    NvMediaISCTemperature sensorTempInfo;
    /** Holds parsed embedded data frame sequence number info for the captured frame. */
    NvMediaISCFrameSeqNum frameSeqNumInfo;
  };
};
// samples/nvmedia/nvsipl/test/camera/CNvSIPLMaster.hpp
class CNvSIPLMaster {
 public:
  // sep2.1 Initialize SIPL, configure the settings that SIPL Core is to use during capture:
  SIPLStatus Setup(PlatformCfg *pPlatformCfg) {
    if (CLogger::GetInstance().GetLogLevel() >= CLogger::LEVEL_INFORMATION) {
      DisplayPlatformCfg(pPlatformCfg);  // Display platform configuration
    }
    INvSIPLCamera::SetPipelineCfg() m_pCamera = INvSIPLCamera::GetInstance();  // Camera Master setup
    auto status = m_pCamera->SetPlatformCfg(pPlatformCfg);
  }
  SIPLStatus SetOutputs(vector<INvSIPLClient::ConsumerDesc> &voConsDesc) {
    return m_pCamera->SetOutputDesc(voConsDesc);
  }
  SIPLStatus Init(void) {
    auto status = m_pCamera->Init();
    m_upDevice.reset(NvMediaDeviceCreate());
    return NVSIPL_STATUS_OK;
  }
  INvSIPLClient::ClientDesc *GetClientDesc(uint32_t sensor, INvSIPLClient::ConsumerDesc::OutputType outType) {
    return m_pCamera->GetClientDesc(sensor, outType);
  }

 private:
  unique_ptr<INvSIPLCamera> m_pCamera;
  std::unique_ptr<NvMediaDevice, CloseNvMediaDevice> m_upDevice;
  std::vector<std::vector<NvMediaImageGroup *>> m_imageGroupList;
  std::vector<std::vector<NvMediaImage *>> m_imagesList;
};

class CNvSIPLMasterNvSci final : public CNvSIPLMaster {
  SIPLStatus SetPlatformConfig(PlatformCfg *pPlatformCfg, NvSIPLDeviceBlockQueues &queues) {
    if (CLogger::GetInstance().GetLogLevel() >= CLogger::LEVEL_INFORMATION) {
      DisplayPlatformCfg(pPlatformCfg);
    }
    return m_pCamera->SetPlatformCfg(pPlatformCfg, queues);
  }

  SIPLStatus SetPipelineConfig(
      uint32_t uIndex, NvSIPLPipelineConfiguration &pipelineCfg, NvSIPLPipelineQueues &pipelineQueues) {
    return m_pCamera->SetPipelineCfg(uIndex, pipelineCfg, pipelineQueues);
  }
};
class INvSIPLCamera {
  // This method must be called before  SetPipelineCfg().
  virtual SIPLStatus SetPlatformCfg(const PlatformCfg *const platformConfig) = 0;
  virtual SIPLStatus SetPlatformCfg(const PlatformCfg *const platformConfig, NvSIPLDeviceBlockQueues &queues) = 0;
  virtual SIPLStatus SetPipelineCfg(
      uint32_t index, const NvSIPLPipelineConfiguration &pipelineCfg, NvSIPLPipelineQueues &queues) = 0;

  /**@param[in] vDescs The vector of
   *@ref INvSIPLClient::ConsumerDesc objects.*@returns::SIPLStatus.The completion status of the operation.
   */
  virtual SIPLStatus SetOutputDesc(std::vector<INvSIPLClient::ConsumerDesc> vDescs) = 0;
};

struct PlatformCfg {
  /** Holds the platform name. For example, "ddpx-a". */
  std::string platform = "";
  /** Holds the platform configuration name. */
  std::string platformConfig = "";
  /** Holds the platform configuration description. */
  std::string description = "";
  /** Holds the number of device blocks.
   * This value must be less than or equal to @ref MAX_DEVICEBLOCKS_PER_PLATFORM. */
  uint32_t numDeviceBlocks = 0U;
  /** Holds an array of @ref DeviceBlockInfo. */
  DeviceBlockInfo deviceBlockList[MAX_DEVICEBLOCKS_PER_PLATFORM];
};
struct DeviceBlockInfo {
  /** Holds a flag which indicates whether simulator mode has been enabled. Used for the ISP reprocessing use case to
   * simulate the presence of a device block. */
  bool isSimulatorModeEnabled = false;
  /** Holds a flag which indicates whether slave mode must be enabled.
   Used when a Jetson AGX Xavier&trade; SoC connected to the deserializer
   does not have an I2C connection to control it. */
  bool isSlaveModeEnabled = false;
  /** Holds the @ref NvMediaICPInterfaceType that specifies the CSI port of
   the SoC to which the deserializer is connected. */
  NvMediaICPInterfaceType csiPort = NVMEDIA_IMAGE_CAPTURE_CSI_INTERFACE_TYPE_CSI_A;
  /** Holds the @ref NvMediaICPCsiPhyMode Phy mode. */
  NvMediaICPCsiPhyMode phyMode = NVMEDIA_ICP_CSI_DPHY_MODE;
  /** Holds a flag which indicates whether group initialization is enabled. */
  bool isGroupInitProg = false;
  /** Holds the I2C device bus number used to connect the deserializer with
   the SoC. */
  std::uint32_t i2cDevice = -1U;
  /** Holds the @ref DeserInfo deserializer information. */
  DeserInfo deserInfo;
  /** Holds a flag which indicates whether power control is disabled on the
   platform. */
  bool isPwrCtrlDisabled = false;

  /** Holds the number of camera modules connected to the deserializer.
   * This value must be less than or equal to
   * @ref MAX_CAMERAMODULES_PER_BLOCK. */
  uint32_t numCameraModules = 0;
  /** Holds an array of information about each camera module
   in the device block. */
  CameraModuleInfo cameraModuleInfoList[MAX_CAMERAMODULES_PER_BLOCK];
};
struct CameraModuleInfo {
  /** Holds the name of the camera module, for example, "SF3324". */
  std::string name = "";
  /** Holds the description of the camera module. */
  std::string description = "";
  /** Holds  the index of the deserializer link to which this module is
   connected. */
  std::uint32_t linkIndex = -1U;
  /** Holds the @ref SerInfo of the serializer. */
  SerInfo serInfo;
  /** Holds the number of EEPROMs in the module. This value must be less than or equal to @ref
   * MAX_EEPROMS_PER_CAMERA_MODULE. */
  uint32_t numEEPROMs = 0;
  /** Holds an array of information about each EEPROM device in a camera module. */
  EEPROMInfo eepromInfoList[MAX_EEPROMS_PER_CAMERA_MODULE];
  /** Holds the number of image sensors in the module. This value must be less than or equal to @ref
   * MAX_SENSORS_PER_CAMERA_MODULE. */
  uint32_t numSensors = 0;
  /** Holds an array of information about each sensor in a camera module. */
  SensorInfo sensorInfoList[MAX_SENSORS_PER_CAMERA_MODULE];
};

#./ nvsipl_camera - c SF3325_DPHY_x4 -- link - enable - masks '0x0000 0x0000 0x0000 0x0001' - d 0 - w 1 - v 4
Platform configuration name: SF3325_DPHY_x4
Display ID:                  0
Display window ID:           1
Display window Depth:        0
Display window position:     0 0 0 0
Verbosity level: 4
Raw output: disabled
ISP0 output: enabled
ISP1 output: enabled
Disabled FPS logging
Disabled Metadata logging
Disabled Slave mode
nvsipl_camera: Setting up signal handler

nvsipl_camera: Getting platform configuration for SF3325_DPHY_x4

nvsipl_camera: Setting link masks

nvsipl_camera: Creating camera master

nvsipl_camera: Setting up master

static void DisplayPlatformCfg(const PlatformCfg *pPlatformCfg)
    
Platform: ddpx-a
Platform configuration: SF3325_DPHY_x4
Platform configuration description: SF3325 module in 4 lane DPHY mode

	CSI Port: 11
	I2C Device 7
	Deserializer Name:MAX96712
	             Description: Maxim 96712 Aggregator
	             I2C Address: 0x29
	CameraModule on link 0
	             Name: SF3325
	             Description: Sekonix SF3325 module - 60-deg FOV, DVP AR0231-RCCB, MAX96705
		Serializer Name: MAX96705
		           Address: 0x40
		Sensor Name: AR0231
		       ID: 12
		       Address: 0x10
