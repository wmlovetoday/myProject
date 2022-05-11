
bQuit = false;
CLogger::GetInstance().SetLogLevel((CLogger::LogLevel)(4));

// INvSIPLQuery
auto pQuery = INvSIPLQuery::GetInstance();

auto status = pQuery -> ParseDatabase();

CCmdLineParser cmdline;
auto ret = cmdline.Parse(argc, argv);
cmdline.PrintArgs();

// Creating compositor
std::unique_ptr<CComposite> upComposite(nullptr);
if (cmdline.uDisplay != (uint32_t)-1) {
  if (cmdline.bNvSci) {
    upComposite.reset(new CCompositeNvSci());
  } else {
    upComposite.reset(new CComposite());
  }
}

// Set verbosity level
LOG_INFO("Setting verbosity level: %u\n", cmdline.verbosity);
INvSIPLQueryTrace::GetInstance()->SetLevel((INvSIPLQueryTrace::TraceLevel)cmdline.verbosity);
#if !NV_IS_SAFETY
INvSIPLTrace::GetInstance()->SetLevel((INvSIPLTrace::TraceLevel)cmdline.verbosity);
#endif  // !NV_IS_SAFETY
CLogger::GetInstance().SetLogLevel((CLogger::LogLevel)cmdline.verbosity);

PlatformCfg oPlatformCfg;
status = pQuery->GetPlatformCfg("V1SIM623S3RU3200NB20_CPHY_x4", oPlatformCfg);

// Apply mask
if (cmdline.vMasks.size() != 0) {
  status = pQuery->ApplyMask(oPlatformCfg, cmdline.vMasks);
}

// Get to ignore the fatal error
bIgnoreError = cmdline.bIgnoreError;

#if !NV_IS_SAFETY
// Check if file mode.
// If so we need to verify/update device block info and create fileReader
std::unique_ptr<CFileReader> fileReader(nullptr);

oPlatformCfg.deviceBlockList[0].isSimulatorModeEnabled = true;
// Create new file reader
fileReader.reset(new CFileReader());

// Initialize the feeder
const auto &vcinfo = oPlatformCfg.deviceBlockList[0].cameraModuleInfoList[0].sensorInfo.vcInfo;
auto status = fileReader -> Init(cmdline.vInputRawFiles, vcinfo, &bQuit);
}

// Warn if file dumping is requested with more than one output enabled
if (cmdline.sPrefix != "") {
  uint32_t uNumOutputsPerSensor = (uint32_t)(!cmdline.bDisableRaw) + (uint32_t)(!cmdline.bDisableISP0) +
                                  (uint32_t)(!cmdline.bDisableISP1) + (uint32_t)(!cmdline.bDisableISP2);
}

// Check if passive mode is enabled.
// If so we need to modify device block info.
for (auto d = 0u; d != oPlatformCfg.numDeviceBlocks; d++) {
  if (cmdline.bEnablePassive) {
    oPlatformCfg.deviceBlockList[d].isPassiveModeEnabled = true;
  }
}

// Check platform configuration and pipeline configuration compatibility.
for (auto d = 0u; d != oPlatformCfg.numDeviceBlocks; d++) {
  auto db = oPlatformCfg.deviceBlockList[d];
  for (auto m = 0u; m != db.numCameraModules; m++) {
    auto module = db.cameraModuleInfoList[m];
    auto sensor = module.sensorInfo;
    if (sensor.isTPGEnabled and (!cmdline.bDisableISP0 or !cmdline.bDisableISP1 or !cmdline.bDisableISP2)) {
      LOG_ERR("Cannot enable ISP output in TPG mode.\n");
      return -1;
    }
  }
}
// Creating camera master
if (cmdline.bNvSci) {
  upMaster.reset(new CNvSIPLMasterNvSci());
} else {
  upMaster.reset(new CNvSIPLMaster());
}

NvSciBufModule bufModule = nullptr;
NvSciSyncModule syncModule = nullptr;

// Setting up master
status = upMaster->Setup(&bufModule, &syncModule);  // <1>
//

status = upComposite->Init(
    cmdline.uDisplay, cmdline.uDispWinID, cmdline.uDispWinDepth, cmdline.bRectSet ? &cmdline.oDispRect : nullptr,
    bufModule, syncModule);

// Enable following outputs for all sensors
std::vector<INvSIPLClient::ConsumerDesc::OutputType> eOutputList;
if (!cmdline.bDisableRaw) {
  eOutputList.push_back(INvSIPLClient::ConsumerDesc::OutputType::ICP);
}

if (!cmdline.bDisableISP0) {
  eOutputList.push_back(INvSIPLClient::ConsumerDesc::OutputType::ISP0);
}

if (!cmdline.bDisableISP1) {
  eOutputList.push_back(INvSIPLClient::ConsumerDesc::OutputType::ISP1);
}

if (!cmdline.bDisableISP2) {
  eOutputList.push_back(INvSIPLClient::ConsumerDesc::OutputType::ISP2);
}

LOG_INFO("Creating consumers\n");
vector<CProfiler *> vpProfilerISP;
vector<unique_ptr<CProfiler>> vupProfilers;
vector<unique_ptr<CNvSIPLConsumer>> vupConsumers;
vector<unique_ptr<CPipelineNotificationHandler>> vupNotificationHandler;
vector<unique_ptr<CDeviceBlockNotificationHandler>> vupDeviceBlockNotifyHandler;
vector<unique_ptr<CPipelineFrameQueueHandler>> vupFrameCompletionQueueHandler;
vector<std::pair<INvSIPLFrameCompletionQueue *, CNvSIPLConsumer *>> vQueueConsumerPair;
NvSciStreamBlock consumer[MAX_NUM_SENSORS][MAX_OUTPUTS_PER_SENSOR];
NvSciStreamBlock *consumerUpstream[MAX_NUM_SENSORS][MAX_OUTPUTS_PER_SENSOR] = {{nullptr}};
NvSciStreamBlock queue[MAX_NUM_SENSORS][MAX_OUTPUTS_PER_SENSOR];

NvSIPLDeviceBlockQueues deviceBlockQueues;

status = upMaster->SetPlatformConfig(&oPlatformCfg, deviceBlockQueues);
CHK_STATUS_AND_RETURN(status, "Master SetPlatformConfig");

// for each sensor
for (auto d = 0u; d != oPlatformCfg.numDeviceBlocks; d++) {
  auto db = oPlatformCfg.deviceBlockList[d];
  for (auto m = 0u; m != db.numCameraModules; m++) {
    auto module = db.cameraModuleInfoList[m];
    auto sensor = module.sensorInfo;
    uint32_t uSensor = sensor.id;

    NvSIPLPipelineConfiguration pipelineCfg{};
    NvSIPLPipelineQueues pipelineQueues{};

    pipelineCfg.captureOutputRequested = !cmdline.bDisableRaw;
    pipelineCfg.isp0OutputRequested = !cmdline.bDisableISP0;
    pipelineCfg.isp1OutputRequested = !cmdline.bDisableISP1;
    pipelineCfg.isp2OutputRequested = !cmdline.bDisableISP2;

    NvSIPLDownscaleCropCfg &downscaleCropCfg = pipelineCfg.downscaleCropCfg;
    downscaleCropCfg.ispInputCropEnable = cmdline.bIspInputCropEnable;
    downscaleCropCfg.ispInputCrop.x0 = 0u;
    downscaleCropCfg.ispInputCrop.y0 = cmdline.uIspInputCropY;
    downscaleCropCfg.ispInputCrop.x1 = sensor.vcInfo.resolution.width;
    downscaleCropCfg.ispInputCrop.y1 = cmdline.uIspInputCropY + cmdline.uIspInputCropH;
    if ((oPlatformCfg.platformConfig == "IMX728_IMX623_CPHY_x2_CSI-A_CSI-C") && (uSensor == 0U)) {
      downscaleCropCfg.isp1DownscaleEnable = true;
      downscaleCropCfg.isp1DownscaleWidth = 1920U;
      downscaleCropCfg.isp1DownscaleHeight = 1080U;
    }

    if (cmdline.bEnableStatsOverrideTest) {
      NvSIPLIspStatsOverrideSetting &overrideSetting = pipelineCfg.statsOverrideSettings;

      // override ISP HIST1 statistics
      overrideSetting.enableHistStatsOverride[1] = true;
      overrideSetting.histStats[1].enable = true;
      overrideSetting.histStats[1].offset = 0.0;
      uint8_t knees[NVSIPL_ISP_HIST_KNEE_POINTS] = {4u, 24u, 48u, 96u, 140u, 192u, 248u, 255u};
      uint8_t ranges[NVSIPL_ISP_HIST_KNEE_POINTS] = {4u, 8u, 10u, 12u, 14u, 16u, 19u, 19u};
      for (uint8_t i = 0; i < NVSIPL_ISP_HIST_KNEE_POINTS; i++) {
        overrideSetting.histStats[1].knees[i] = knees[i];
        overrideSetting.histStats[1].ranges[i] = ranges[i];
      }
      overrideSetting.histStats[1].rectangularMask.x0 = 2;
      overrideSetting.histStats[1].rectangularMask.y0 = 2;
      overrideSetting.histStats[1].rectangularMask.x1 = 200;
      overrideSetting.histStats[1].rectangularMask.y1 = 200;

      // override ISP LAC1 statistcs
      overrideSetting.enableLacStatsOverride[1] = true;
      overrideSetting.lacStats[1].enable = true;
      for (uint8_t i = 0; i < NVSIPL_ISP_MAX_COLOR_COMPONENT; i++) {
        overrideSetting.lacStats[1].min[i] = 0.0;
        overrideSetting.lacStats[1].max[i] = 1.0;
      }

      for (uint8_t j = 0; j < NVSIPL_ISP_MAX_LAC_ROI; j++) {
        overrideSetting.lacStats[1].roiEnable[j] = true;
        overrideSetting.lacStats[1].ellipticalMaskEnable[j] = true;
      }

      NvSiplISPStatisticsWindows window[] = {{4, 4, 32, 32, 4, 4, {0, 0}},
                                             {4, 4, 32, 32, 4, 4, {128, 0}},
                                             {4, 4, 32, 32, 4, 4, {0, 128}},
                                             {4, 4, 32, 32, 4, 4, {128, 128}}};
      memcpy(&overrideSetting.lacStats[1].windows, window, sizeof(NvSiplISPStatisticsWindows) * 4);
      overrideSetting.lacStats[1].ellipticalMask = {{128.0F, 128.0F}, 256, 256, 180.0F};
    }

#if !NV_IS_SAFETY
    // Check if simulator mode.
    // If so, we need to set up a fileReader to feed frames to SIPL
    if (db.isSimulatorModeEnabled) {
      pipelineCfg.imageGroupWriter = fileReader.get();
    }
#endif  // !NV_IS_SAFETY

    status = upMaster->SetPipelineConfig(uSensor, pipelineCfg, pipelineQueues);
    CHK_STATUS_AND_RETURN(status, "Master SetPipelineConfig");

    INvSIPLFrameCompletionQueue *frameCompletionQueue[MAX_OUTPUTS_PER_SENSOR];

    frameCompletionQueue[(uint32_t)INvSIPLClient::ConsumerDesc::OutputType::ICP] =
        pipelineQueues.captureCompletionQueue;
    frameCompletionQueue[(uint32_t)INvSIPLClient::ConsumerDesc::OutputType::ISP0] = pipelineQueues.isp0CompletionQueue;
    frameCompletionQueue[(uint32_t)INvSIPLClient::ConsumerDesc::OutputType::ISP1] = pipelineQueues.isp1CompletionQueue;
    frameCompletionQueue[(uint32_t)INvSIPLClient::ConsumerDesc::OutputType::ISP2] = pipelineQueues.isp2CompletionQueue;

#if !NV_IS_SAFETY
    auto isRawSensor = false;
    if ((sensor.vcInfo.inputFormat == NVSIPL_CAP_INPUT_FORMAT_TYPE_RAW6) or
        (sensor.vcInfo.inputFormat == NVSIPL_CAP_INPUT_FORMAT_TYPE_RAW7) or
        (sensor.vcInfo.inputFormat == NVSIPL_CAP_INPUT_FORMAT_TYPE_RAW8) or
        (sensor.vcInfo.inputFormat == NVSIPL_CAP_INPUT_FORMAT_TYPE_RAW10) or
        (sensor.vcInfo.inputFormat == NVSIPL_CAP_INPUT_FORMAT_TYPE_RAW12) or
        (sensor.vcInfo.inputFormat == NVSIPL_CAP_INPUT_FORMAT_TYPE_RAW14) or
        (sensor.vcInfo.inputFormat == NVSIPL_CAP_INPUT_FORMAT_TYPE_RAW16) or
        (sensor.vcInfo.inputFormat == NVSIPL_CAP_INPUT_FORMAT_TYPE_RAW20)) {
      isRawSensor = true;
    }
#endif  // !NV_IS_SAFETY

    vQueueConsumerPair.clear();
    vpProfilerISP.clear();

    // for each output
    for (INvSIPLClient::ConsumerDesc::OutputType eOutput : eOutputList) {
      // Create NvSIPL consumer using client descriptor
      LOG_INFO("Creating consumer for output:%u of sensor:%u\n", eOutput, uSensor);
      auto upCons = unique_ptr<CNvSIPLConsumer>(new CNvSIPLConsumer());
      CHK_PTR_AND_RETURN(upCons, "Consumer creation");

      // Register the consumer with the compositor as source
      auto uID = -1u;
#if !NV_IS_SAFETY
      if (upComposite != nullptr) {
        auto grpIndex = 0;
        switch (db.csiPort) {
          default:
          case NVSIPL_CAP_CSI_INTERFACE_TYPE_CSI_AB:
          case NVSIPL_CAP_CSI_INTERFACE_TYPE_CSI_A:
          case NVSIPL_CAP_CSI_INTERFACE_TYPE_CSI_B:
            grpIndex = 0;
            break;
          case NVSIPL_CAP_CSI_INTERFACE_TYPE_CSI_CD:
          case NVSIPL_CAP_CSI_INTERFACE_TYPE_CSI_C:
          case NVSIPL_CAP_CSI_INTERFACE_TYPE_CSI_D:
            grpIndex = 1;
            break;
          case NVSIPL_CAP_CSI_INTERFACE_TYPE_CSI_EF:
          case NVSIPL_CAP_CSI_INTERFACE_TYPE_CSI_E:
          case NVSIPL_CAP_CSI_INTERFACE_TYPE_CSI_F:
            grpIndex = 2;
            break;
          case NVSIPL_CAP_CSI_INTERFACE_TYPE_CSI_GH:
          case NVSIPL_CAP_CSI_INTERFACE_TYPE_CSI_G:
          case NVSIPL_CAP_CSI_INTERFACE_TYPE_CSI_H:
            grpIndex = 3;
            break;
        }

        LOG_INFO("Register with compositor for output:%u of sensor:%u\n", eOutput, uSensor);
        auto outIndex = int(eOutput) - int(INvSIPLClient::ConsumerDesc::OutputType::ICP);
        auto isRawOutput = (eOutput == INvSIPLClient::ConsumerDesc::OutputType::ICP) and isRawSensor;
        status = upComposite->RegisterSource(
            grpIndex, module.linkIndex, outIndex, isRawOutput, uID, &consumer[uSensor][(uint32_t)eOutput],
            &consumerUpstream[uSensor][(uint32_t)eOutput], &queue[uSensor][(uint32_t)eOutput]);
        CHK_STATUS_AND_RETURN(status, "Composite register");
      }
#endif  // !NV_IS_SAFETY

      CProfiler *pProfiler = nullptr;
      if (cmdline.bShowFPS || cmdline.bEnableProfiling || cmdline.bEnableInitProfiling) {
        LOG_INFO("Profiler initialization for output:%u of sensor:%u\n", eOutput, uSensor);
        unique_ptr<CProfiler> upProfiler = unique_ptr<CProfiler>(new CProfiler());
        CHK_PTR_AND_RETURN(upProfiler, "Profiler creation");
        status = upProfiler->Init(
            uSensor, eOutput, uInitTimeArrUs, syncModule, cmdline.bEnableProfiling, cmdline.bEnableInitProfiling,
            !cmdline.bNvSci);
        if (status != NVSIPL_STATUS_OK) {
          LOG_ERR("Failed to initialize profiler for output:%u of sensor:%u\n", eOutput, uSensor);
          return -1;
        }
        pProfiler = upProfiler.get();
        vupProfilers.push_back(move(upProfiler));
      }

      LOG_INFO("Consumer initialization for output:%u of sensor:%u\n", eOutput, uSensor);
      if (cmdline.bNvSci) {
        status = upCons->Init(
#if !NV_IS_SAFETY
            nullptr, (upComposite != nullptr) ? (CNvSIPLMasterNvSci *)upMaster.get() : nullptr,
#endif  // !NV_IS_SAFETY
            -1, uSensor, eOutput, pProfiler
#if !NV_IS_SAFETY
            ,
            cmdline.sPrefix, cmdline.uNumSkipFrames, cmdline.uNumWriteFrames
#endif  // !NV_IS_SAFETY
        );
      } else {
        status = upCons->Init(
#if !NV_IS_SAFETY
            upComposite.get(), nullptr,
#endif  // !NV_IS_SAFETY
            uID, uSensor, eOutput, pProfiler
#if !NV_IS_SAFETY
            ,
            cmdline.sPrefix, cmdline.uNumSkipFrames, cmdline.uNumWriteFrames
#endif  // !NV_IS_SAFETY
        );
      }
      if (status != NVSIPL_STATUS_OK) {
        LOG_ERR("Failed to initialize consumer for output:%u of sensor:%u\n", eOutput, uSensor);
        return -1;
      }

      if (cmdline.bShowMetadata) {
        upCons->EnableMetadataLogging();
      }
#if !NV_IS_SAFETY
      if (cmdline.bAutoLEDControl) {
        upCons->EnableLEDControl();
      }
#endif  // !NV_IS_SAFETY

      if (eOutput != INvSIPLClient::ConsumerDesc::OutputType::ICP) {
        if (pProfiler) {
          vpProfilerISP.push_back(pProfiler);
        }
      }

      vQueueConsumerPair.push_back(std::make_pair(frameCompletionQueue[(uint32_t)eOutput], upCons.get()));
      vupConsumers.push_back(move(upCons));
    }  // output

    auto upFrameCompletionQueueHandler = std::unique_ptr<CPipelineFrameQueueHandler>(new CPipelineFrameQueueHandler());
    CHK_PTR_AND_RETURN(upFrameCompletionQueueHandler, "Frame Completion Queues handler creation");

    upFrameCompletionQueueHandler->Init(uSensor, vQueueConsumerPair);
    CHK_STATUS_AND_RETURN(status, "Frame Completion Queues Handler Init");

    vupFrameCompletionQueueHandler.push_back(move(upFrameCompletionQueueHandler));

    auto upNotificationHandler = std::unique_ptr<CPipelineNotificationHandler>(new CPipelineNotificationHandler());
    CHK_PTR_AND_RETURN(upNotificationHandler, "Notification handler creation");

    status = upNotificationHandler->Init(uSensor, pipelineQueues.notificationQueue, vpProfilerISP);
    CHK_STATUS_AND_RETURN(status, "Notification Handler Init");

    vupNotificationHandler.push_back(move(upNotificationHandler));
  }  // module
}  // device block

LOG_INFO("Initializing master interface\n");
status = upMaster->Init();
CHK_STATUS_AND_RETURN(status, "Master initialization");

for (auto d = 0u; d != oPlatformCfg.numDeviceBlocks; d++) {
  auto upDeviceBlockNotifyHandler =
      std::unique_ptr<CDeviceBlockNotificationHandler>(new CDeviceBlockNotificationHandler());
  CHK_PTR_AND_RETURN(upDeviceBlockNotifyHandler, "Device Block Notification handler creation");

  status = upDeviceBlockNotifyHandler->Init(d, oPlatformCfg.deviceBlockList[d], deviceBlockQueues.notificationQueue[d]);
  CHK_STATUS_AND_RETURN(status, "Device Block Notification Handler Init");

  vupDeviceBlockNotifyHandler.push_back(move(upDeviceBlockNotifyHandler));
}

if (cmdline.bNvSci) {
  // for each sensor
  for (auto d = 0u; d != oPlatformCfg.numDeviceBlocks; d++) {
    auto db = oPlatformCfg.deviceBlockList[d];
    for (auto m = 0u; m != db.numCameraModules; m++) {
      auto module = db.cameraModuleInfoList[m];
      auto sensor = module.sensorInfo;
      uint32_t uSensor = sensor.id;

      // for each output
      for (INvSIPLClient::ConsumerDesc::OutputType eOutput : eOutputList) {
        auto status = upMaster->RegisterSource(
            uSensor, eOutput,
#if !NV_IS_SAFETY
            db.isSimulatorModeEnabled, (upComposite != nullptr),
#else   // !NV_IS_SAFETY
            false, false,
#endif  // !NV_IS_SAFETY
            &consumer[uSensor][(uint32_t)eOutput], consumerUpstream[uSensor][(uint32_t)eOutput],
            &queue[uSensor][(uint32_t)eOutput]);
        if (status != NVSIPL_STATUS_OK) {
          LOG_ERR("Failed to register source for output:%u of sensor:%u\n", eOutput, uSensor);
          return -1;
        }
      }  // output
      // ICP is needed even if not an output; register so that buffers can be
      // allocated
      if (cmdline.bDisableRaw) {
        status = upMaster->RegisterSource(
            uSensor, INvSIPLClient::ConsumerDesc::OutputType::ICP,
#if !NV_IS_SAFETY
            db.isSimulatorModeEnabled,
#else   // !NV_IS_SAFETY
            false,
#endif  // !NV_IS_SAFETY
            false, nullptr, nullptr, nullptr);
        if (status != NVSIPL_STATUS_OK) {
          LOG_ERR("Failed to register source for ICP output of sensor:%u\n", uSensor);
          return -1;
        }
      }
    }  // module
  }    // device block
}

if (cmdline.bAutoRecovery) {
  m_upAutoRecovery.reset(new CAutoRecovery(upMaster.get()));
  CHK_PTR_AND_RETURN(m_upAutoRecovery, "Auto recovery creation");
}

#if !NV_IS_SAFETY
if (upComposite != nullptr) {
  LOG_INFO("Starting compositor\n");
  status = upComposite->Start();
  CHK_STATUS_AND_RETURN(status, "Compositor start");

  if (cmdline.bNvSci) {
    status = upMaster->SetupElements();
    if (status != NVSIPL_STATUS_OK) {
      LOG_ERR("Failed to set up producer streaming\n");
      return -1;
    }
  }
}
#endif  // !NV_IS_SAFETY

for (auto d = 0u; d != oPlatformCfg.numDeviceBlocks; d++) {
  auto db = oPlatformCfg.deviceBlockList[d];
  for (auto m = 0u; m != db.numCameraModules; m++) {
    auto module = db.cameraModuleInfoList[m];
    auto sensor = module.sensorInfo;
    uint32_t uSensor = sensor.id;

    if (cmdline.bShowEEPROM) {
      LOG_MSG("EEPROM of Sensor %d\n", uSensor);
      upMaster->ShowEEPROM(uSensor);
    }

    status = upMaster->AllocateAndRegisterBuffers(
        uSensor, oPlatformCfg.platformConfig, !cmdline.bDisableISP0, !cmdline.bDisableISP1, !cmdline.bDisableISP2,
        cmdline.bNvSciBufPath);

    status = upMaster->SetupNvSciSync(uSensor, !cmdline.bDisableISP0, !cmdline.bDisableISP1, !cmdline.bDisableISP2);
    if (status != NVSIPL_STATUS_OK) {
      LOG_ERR("SetupNvSciSync failed for sensor pipeline:%u\n", uSensor);
      return -1;
    }
  }
}

#if !NV_IS_SAFETY
if ((upComposite != nullptr) && cmdline.bNvSci) {
  status = upMaster->SetupBuffers();
  if (status != NVSIPL_STATUS_OK) {
    LOG_ERR("Failed to set up producer buffers\n");
    return -1;
  }

  status = upMaster->SetupSync();
  if (status != NVSIPL_STATUS_OK) {
    LOG_ERR("Failed to set up producer synchronization\n");
    return -1;
  }

  status = upMaster->SetupComplete();
  if (status != NVSIPL_STATUS_OK) {
    LOG_ERR("Failed to finalize producer setup\n");
    return -1;
  }
}
#endif  // !NV_IS_SAFETY

if (cmdline.bNvSci) {
  for (auto d = 0u; d != oPlatformCfg.numDeviceBlocks; d++) {
    auto db = oPlatformCfg.deviceBlockList[d];
    for (auto m = 0u; m != db.numCameraModules; m++) {
      auto module = db.cameraModuleInfoList[m];
      auto sensor = module.sensorInfo;
      uint32_t uSensor = sensor.id;
      status = upMaster->RegisterBuffers(uSensor, !cmdline.bDisableISP0, !cmdline.bDisableISP1, !cmdline.bDisableISP2);
      if (status != NVSIPL_STATUS_OK) {
        LOG_ERR("RegisterBuffers failed for sensor pipeline:%u\n", uSensor);
        return -1;
      }
    }
  }
}

nvsipl::ISiplControlAuto *upCustomPlugins[MAX_SENSORS_PER_PLATFORM]{nullptr};

for (auto d = 0u; d != oPlatformCfg.numDeviceBlocks; d++) {
  auto db = oPlatformCfg.deviceBlockList[d];
  for (auto m = 0u; m != db.numCameraModules; m++) {
    auto module = db.cameraModuleInfoList[m];
    auto sensor = module.sensorInfo;
    uint32_t uSensor = sensor.id;

    if (!cmdline.bDisableISP0 || !cmdline.bDisableISP1 || !cmdline.bDisableISP2) {
      std::vector<uint8_t> blob;
      status = LoadNITOFile(cmdline.sNitoFolderPath, module.name, blob);
      if (status != NVSIPL_STATUS_OK) {
        LOG_ERR("Failed to load NITO file\n");
        //   return -1;
      }
#if !NV_IS_SAFETY
      if (cmdline.bShowNitoMetadata) {
        /**
         * SIPL Fetch NITO Metadata Interface Sample Usage
         */
        LOG_MSG("Retrieving metadata from NITO file for sensorID %u \n", uSensor);
        status = callFetchNITOMetadataAPI(blob, cmdline.uShowNitoMetadataNumParamSets);
        if (status != NVSIPL_STATUS_OK) {
          LOG_ERR("Failed to run Fetch NITO Metadata API \n");
          return -1;
        }
      }
#endif  // !NV_IS_SAFETY
      if (cmdline.autoPlugin == NV_PLUGIN) {
        status = upMaster->RegisterAutoControl(uSensor, NV_PLUGIN, nullptr, blob);
        if (status != NVSIPL_STATUS_OK) {
          LOG_ERR("SetSiplControl(NV) failed for ISP output of sensor:%u\n", uSensor);
          // return -1;
        }
      } else {
        pluginlib_handle = dlopen("libnvsipl_sampleplugin.so", RTLD_LAZY);
        if (!pluginlib_handle) {
          LOG_ERR("Failed to open lib libnvsipl_sampleplugin.so\n");
          return -1;
        }
        nvsipl::ISiplControlAuto *(*libCreatePlugin)() =
            (nvsipl::ISiplControlAuto * (*)()) dlsym(pluginlib_handle, "CreatePlugin");
        if (!libCreatePlugin) {
          LOG_ERR("Failed to create function pointer for CreatePlugin\n");
          return -1;
        }
        upCustomPlugins[uSensor] = libCreatePlugin();
        CHK_PTR_AND_RETURN(upCustomPlugins[uSensor], "AutoControl plugin creation");

        status = upMaster->RegisterAutoControl(uSensor, CUSTOM_PLUGIN0, upCustomPlugins[uSensor], blob);
        if (status != NVSIPL_STATUS_OK) {
          LOG_ERR("SetAutoControl(CUST0) failed for ISP output of sensor:%u\n", uSensor);
          return -1;
        }
      }
    }
  }
}

LOG_INFO("Starting master\n");
status = upMaster->Start();
CHK_STATUS_AND_RETURN(status, "Master start");

if (m_upAutoRecovery != nullptr) {
  LOG_INFO("Starting auto recovery\n");
  status = m_upAutoRecovery->Start();
  CHK_STATUS_AND_RETURN(status, "Auto recovery start");
}

// Spawn a background thread to accept user's runtime command
std::thread([&] {
  pthread_setname_np(pthread_self(), "RuntimeMenu");

  while (!bQuit) {
#if !NV_IS_SAFETY
    if (upComposite != nullptr) {
      cout << "Enter 'ld' to list display-able outputs.\n";
      cout << "Enter 'e' followed by 'ID' to enable display for specific "
              "camera group\n";
    }
    cout << "Enter 'les' followed by sensor ID to enable LED\n";
    cout << "Enter 'lds' followed by sensor ID to disable LED\n";
#endif  // !NV_IS_SAFETY
    cout << "Enter 'dl' followed by sensor ID to disable the link\n";
    cout << "Enter 'el' followed by sensor ID to enable the link without "
            "module reset\n";
    cout << "Enter 'elr' followed by sensor ID to enable the link with "
            "module reset\n";

    if (cmdline.sConfigName.find("OV2311") != std::string::npos) {
      // Show custom interface command
      cout << "Enter 'cust <sensor ID>,<value>' to set an example value for "
              "OV2311 custom API\n";
    }

    cout << "Enter 'q' to quit the application\n";
    char line[256];
    cout << "-\n";
    cin.getline(line, 256);
    if (line[0] == 'q') {
      bQuit = true;
    }
#if !NV_IS_SAFETY
    else if (upComposite != nullptr) {
      if (line[0] == 'l' and line[1] == 'd') {
        upComposite->PrintDisplayableGroups();
      } else if ((line[0] == 'e') && (line[1] != 'l')) {
        auto uActiveDisplayable = atoi(&line[1]);
        cout << "Enabled output" << uActiveDisplayable << endl;
        upComposite->SetActiveGroup(uActiveDisplayable);
      }
    }

    if ((line[0] == 'l') && (line[2] == 's')) {
      auto id = atoi(&line[3]);
      if ((id >= 0) && (id < 16)) {
        if (line[1] == 'e') {
          upMaster->ToggleLED(id, true);
          cout << "Enable LED: " << id << endl;
        } else if (line[1] == 'd') {
          upMaster->ToggleLED(id, false);
          cout << "Disable LED: " << id << endl;
        }
      } else {
        cout << "The sensor id " << id << " is out of the range. The valid sensor id is from 0 to 15" << endl;
      }
    }
#endif  // !NV_IS_SAFETY

    if ((line[0] == 'd') && (line[1] == 'l')) {
      auto id = atoi(&line[2]);
      if ((id >= 0) && (id < 16)) {
        upMaster->DisableLink(id);
        cout << "Disable Link: " << id << endl;
      } else {
        cout << "The sensor id " << id << " is out of the range. The valid sensor id is from 0 to 15" << endl;
      }
    }

    if ((line[0] == 'e') && (line[1] == 'l')) {
      uint32_t id = -1U;
      bool resetModule = false;
      if (line[2] == 'r') {
        id = atoi(&line[3]);
        resetModule = true;
      } else {
        id = atoi(&line[2]);
      }
      if ((id >= 0) && (id < 16)) {
        upMaster->EnableLink(id, resetModule);
        cout << "Enable Link: " << id << endl;
      } else {
        cout << "The sensor id " << id << " is out of the range. The valid sensor id is from 0 to 15" << endl;
      }
    }

    if ((line[0] == 'c') && (line[1] == 'u') && (line[2] == 's') && (line[3] == 't')) {
      processOV2311CustomCommand(line);
    }
  }
}).detach();

bool bFirstIteration = true;
bool bValidFrameCount = false;
uint64_t uFrameCountDelta = 0u;
uint32_t uFrameDropPrintTimer = 0u;
double firstCaptureDelayMs = 0.0;
double firstReceivedDelayMs = 0.0;
double firstReadyDelayMs = 0.0;
double firstLatencyMs = 0.0;
double avgLatencyMs = 0.0;
double rmsJitterMs = 0.0;
double avgISPProcessingTimeMs = 0.0;
// Wait for quit
while (!bQuit) {
  // Wait for SECONDS_PER_ITERATION
  auto oStartTime = chrono::steady_clock::now();
  std::this_thread::sleep_for(std::chrono::seconds(SECONDS_PER_ITERATION));

  if (cmdline.uRunDurationSec != -1) {
    cmdline.uRunDurationSec -= SECONDS_PER_ITERATION;
    if (cmdline.uRunDurationSec < SECONDS_PER_ITERATION) {
      bQuit = true;
    }
  }

#if !NV_IS_SAFETY
  // check if frames dumping is done
  if (cmdline.sPrefix != "") {
    bool bFrameWritePending = false;
    for (auto &cons : vupConsumers) {
      if (!cons->IsFrameWriteComplete()) {
        bFrameWritePending = true;
        break;
      }
    }
    if (!bFrameWritePending) {
      bQuit = true;
    }
  }
#endif  // !NV_IS_SAFETY

  // Check for any asynchronous fatal errors reported by pipeline threads in
  // the library
  for (auto &notificationHandler : vupNotificationHandler) {
    if (notificationHandler->IsPipelineInError()) {
      bQuit = true;
    }
  }

  // Check for any asynchronous errors reported by the device blocks
  for (auto &notificationHandler : vupDeviceBlockNotifyHandler) {
    if (notificationHandler->IsDeviceBlockInError()) {
      bQuit = true;
    }
  }

  if (cmdline.bEnableProfiling) {
    uFrameDropPrintTimer += SECONDS_PER_ITERATION;
  }

  // Log FPS
  if (cmdline.bShowFPS || cmdline.bEnableProfiling || cmdline.bEnableInitProfiling) {
    auto uTimeElapsedMs = chrono::duration<double, std::milli>(chrono::steady_clock::now() - oStartTime).count();
    cout << "Output" << endl;
    for (auto &prof : vupProfilers) {
      prof->m_profData.profDataMut.lock();
      uFrameCountDelta = prof->m_profData.uFrameCount - prof->m_profData.uPrevFrameCount;
      prof->m_profData.uPrevFrameCount = prof->m_profData.uFrameCount;
      bValidFrameCount = uFrameCountDelta != 0u;
      if (bValidFrameCount) {
        if (cmdline.bEnableInitProfiling && bFirstIteration) {
          firstCaptureDelayMs = prof->m_profData.uFirstCaptureDelayUs / 1000.0;
          firstReceivedDelayMs = prof->m_profData.uFirstReceivedDelayUs / 1000.0;
          firstReadyDelayMs = prof->m_profData.uFirstReadyDelayUs / 1000.0;
        }
        if (cmdline.bEnableProfiling) {
          if (bFirstIteration) {
            firstLatencyMs = prof->m_profData.uFirstLatencyUs / 1000.0;
          }
          avgLatencyMs = (prof->m_profData.uTotLatencyUs / 1000.0) / uFrameCountDelta;
          rmsJitterMs = sqrt(prof->m_profData.uTotSqJitterUs / uFrameCountDelta) / 1000.0;
          avgISPProcessingTimeMs = (prof->m_profData.uTotISPProcessingTime / 1000.0) / uFrameCountDelta;
          prof->m_profData.uTotLatencyUs = 0u;
          prof->m_profData.uTotSqJitterUs = 0u;
          prof->m_profData.uTotISPProcessingTime = 0u;
        }
      }
      prof->m_profData.profDataMut.unlock();
      auto fps = uFrameCountDelta / (uTimeElapsedMs / 1000.0);
      string profName = "Sensor" + to_string(prof->m_uSensor) + "_Out" + to_string(int(prof->m_outputType)) + "\t";
      cout << profName << "Frame rate (fps):\t\t" << fps << endl;
      if (cmdline.bEnableInitProfiling && bFirstIteration) {
        cout << profName << "First capture delay (ms):\t";
        if (bValidFrameCount) {
          cout << firstCaptureDelayMs << endl;
        } else {
          cout << "N/A" << endl;
        }
        cout << profName << "First received delay (ms):\t";
        if (bValidFrameCount) {
          cout << firstReceivedDelayMs << endl;
        } else {
          cout << "N/A" << endl;
        }
        cout << profName << "First ready delay (ms):\t\t";
        if (bValidFrameCount) {
          cout << firstReadyDelayMs << endl;
        } else {
          cout << "N/A" << endl;
        }
      }
      if (cmdline.bEnableProfiling) {
        if (bFirstIteration) {
          cout << profName << "First latency (ms):\t\t";
          if (bValidFrameCount) {
            cout << firstLatencyMs << endl;
          } else {
            cout << "N/A" << endl;
          }
        }
        cout << profName << "Average latency (ms):\t\t";
        if (bValidFrameCount) {
          cout << avgLatencyMs << endl;
        } else {
          cout << "N/A" << endl;
        }
        cout << profName << "RMS jitter (ms):\t\t";
        if (bValidFrameCount) {
          cout << rmsJitterMs << endl;
        } else {
          cout << "N/A" << endl;
        }
        if (prof->m_outputType != INvSIPLClient::ConsumerDesc::OutputType::ICP) {
          cout << profName << "Average ISP Time (ms):\t\t";
          if (bValidFrameCount) {
            cout << avgISPProcessingTimeMs << endl;
          } else {
            cout << "N/A" << endl;
          }
        }
        // Only print number of frame drops each hour
        if (uFrameDropPrintTimer >= SECONDS_PER_HOUR) {
          uint32_t numFrameDrops = 0U;
          for (auto &notificationHandler : vupNotificationHandler) {
            if (prof->m_uSensor == notificationHandler->m_uSensor) {
              numFrameDrops = notificationHandler->GetNumFrameDrops(prof->m_uSensor);
              break;
            }
          }
          cout << profName << "Number of frame drops:\t\t" << numFrameDrops << endl;
        }
      }
      if (cmdline.bEnableProfiling || (cmdline.bEnableInitProfiling && bFirstIteration)) {
        cout << endl;
      }
    }
    if (cmdline.bEnableProfiling || cmdline.bEnableInitProfiling) {
      if (bFirstIteration) {
        bFirstIteration = false;
      }

      if (uFrameDropPrintTimer >= SECONDS_PER_HOUR) {
        uFrameDropPrintTimer = 0u;
        for (auto &notificationHandler : vupNotificationHandler) {
          notificationHandler->ResetFrameDropCounter();
        }
      }
    }
    cout << endl;
  }
}

if (m_upAutoRecovery != nullptr) {
  LOG_INFO("Stopping auto recovery\n");
  m_upAutoRecovery->Stop();
}

if (upMaster != nullptr) {
  LOG_INFO("Stopping master\n");
  status = upMaster->Stop();
  if (status != NVSIPL_STATUS_OK) {
    LOG_ERR("Failed to Stop master %u\n", status);
  }
}

#if !NV_IS_SAFETY
if (upComposite != nullptr) {
  LOG_INFO("Stopping compositor\n");
  upComposite->Stop();
}

if (fileReader != nullptr) {
  fileReader->Deinit();
}

if (upComposite != nullptr) {
  LOG_INFO("Deinitializing compositor\n");
  upComposite->Deinit();
}
#endif  // !NV_IS_SAFETY

bool bDeviceBlockError = false;
for (auto &notificationHandler : vupDeviceBlockNotifyHandler) {
  LOG_INFO("Deinitializing devblk notificationHandler: %u\n", notificationHandler->m_uDevBlkIndex);
  bDeviceBlockError |= notificationHandler->IsDeviceBlockInError();
  notificationHandler->Deinit();
}

bool bPipelineError = false;
for (auto &notificationHandler : vupNotificationHandler) {
  LOG_INFO("Deinitializing pipeline notificationHandler: %u\n", notificationHandler->m_uSensor);
  bPipelineError |= notificationHandler->IsPipelineInError();
  notificationHandler->Deinit();
}

for (auto &frameCompletionQueueHandler : vupFrameCompletionQueueHandler) {
  LOG_INFO("Deinitializing frameCompletionQueueHandler: %u\n", frameCompletionQueueHandler->m_uSensor);
  frameCompletionQueueHandler->Deinit();
}

for (auto &cons : vupConsumers) {
  string consumerName = "Sensor" + to_string(cons->m_uSensor) + "_Out" + to_string(int(cons->m_outputType));
  LOG_INFO("Deinitializing consumer %s\n", consumerName.c_str());
  cons->Deinit();
}

for (auto &prof : vupProfilers) {
  string profName = "Sensor" + to_string(prof->m_uSensor) + "_Out" + to_string(int(prof->m_outputType));
  LOG_INFO("Deinitializing profiler %s\n", profName.c_str());
  prof->Deinit();
}

if (upMaster != nullptr) {
  LOG_INFO("Deinitializing master\n");
  upMaster->Deinit();
}

if (pluginlib_handle) {
  dlclose(pluginlib_handle);
}

if (bPipelineError) {
  LOG_ERR("Pipeline failure\n");
  return -1;
}

if (bDeviceBlockError) {
  LOG_ERR("Device Block failure\n");
  return -1;
}

LOG_MSG("SUCCESS\n");
return 0;
}

class CNvSIPLMaster {
  virtual SIPLStatus Setup(NvSciBufModule *bufModule, NvSciSyncModule *syncModule) {
    // Camera Master setup
    m_pCamera = INvSIPLCamera::GetInstance();
    // sep 1, open buf mudule.
    NvSciError err = NvSciBufModuleOpen(&m_sciBufModule);  // NvSciBufModuleClose(module);
    auto nvmStatus = NvMediaImageNvSciBufInit();
    err = NvSciSyncModuleOpen(&m_sciSyncModule);
    *syncModule = m_sciSyncModule;
  }

  //! Allocates imagegroups to be used with ICP
  SIPLStatus AllocateImageGroups(
      NvMediaDevice *pDevice, NvMediaSurfaceType surfType, std::vector<NvMediaSurfAllocAttr> allocAttr,
      uint32_t numPlanes, std::vector<NvMediaImageGroup *> &imageGroups) {
    auto bufferPoolSize = 6U;

    NvSciBufAttrList reconciledAttrlist;
    NvSciBufAttrList unreconciledAttrlist = NULL;
    NvSciBufAttrList conflictlist = NULL;

    NvSciError err = NvSciBufAttrListCreate(m_sciBufModule, &unreconciledAttrlist);
    CHK_NVSCISTATUS_AND_RETURN(err, "NvSciBufAttrListCreate");

    // Need CPU Read permission for RAW->RGB conversion on compositor
    // Need CPU Write permission for FileReader
    // TODO: Determine the permission based on exact config instead of hardcoded value.
    NvSciBufAttrValAccessPerm accessPerm = NvSciBufAccessPerm_ReadWrite;
    NvSciBufAttrKeyValuePair attrKvp = {NvSciBufGeneralAttrKey_RequiredPerm, &accessPerm, sizeof(accessPerm)};

    err = NvSciBufAttrListSetAttrs(unreconciledAttrlist, &attrKvp, 1);
    CHK_NVSCISTATUS_AND_RETURN(err, "NvSciBufAttrListSetAttrs");

    auto nvmStatus =
        NvMediaImageFillNvSciBufAttrs(pDevice, surfType, allocAttr.data(), allocAttr.size(), 0, unreconciledAttrlist);
    CHK_NVMSTATUS_AND_RETURN(nvmStatus, "NvMediaImageFillNvSciBufAttrs");

    err = NvSciBufAttrListReconcile(&unreconciledAttrlist, 1, &reconciledAttrlist, &conflictlist);
    CHK_NVSCISTATUS_AND_RETURN(err, "NvSciBufAttrListReconcile");

    for (auto i = 0u; i < bufferPoolSize; i++) {
      // Allocate image group
      auto imgGrp = new (std::nothrow) NvMediaImageGroup;
      if (imgGrp == nullptr) {
        LOG_ERR("Failed to allocate image Group\n");
        return NVSIPL_STATUS_OUT_OF_MEMORY;
      }

      imgGrp->numImages = numPlanes;
      for (auto p = 0U; p < numPlanes; p++) {
        NvSciBufObj imageGrpSciBufObj;
        err = NvSciBufObjAlloc(reconciledAttrlist, &imageGrpSciBufObj);
        CHK_NVSCISTATUS_AND_RETURN(err, "NvSciBufObjAlloc");

        NvMediaImageCreateFromNvSciBuf(pDevice, imageGrpSciBufObj, &imgGrp->imageList[p]);
        if (imgGrp->imageList[p] == NULL) {
          LOG_ERR("NvMediaImageCreateFromNvSciBuf failed\n");
          return NVSIPL_STATUS_OUT_OF_MEMORY;
        }
        m_sciBufObjs.push_back(imageGrpSciBufObj);
      }
      imageGroups.push_back(imgGrp);
    }

    if (conflictlist != NULL) {
      NvSciBufAttrListFree(conflictlist);
    }
    if (unreconciledAttrlist != NULL) {
      NvSciBufAttrListFree(unreconciledAttrlist);
    }
    if (reconciledAttrlist != NULL) {
      NvSciBufAttrListFree(reconciledAttrlist);
    }
    return NVSIPL_STATUS_OK;
  }

  SIPLStatus AllocateAndRegisterBuffers(
      uint32_t pip, std::string platformConfigName, bool isp0Enabled, bool isp1Enabled, bool isp2Enabled,
      bool useNvSciBufPath) {
    if (!useNvSciBufPath) {
      SIPLStatus status = NVSIPL_STATUS_OK;
      NvMediaStatus nvmStatus = NVMEDIA_STATUS_OK;
      NvSIPLImageAttr attr = {};
      static bool platCfgChecked = false;
      if ((!platCfgChecked) && (platformConfigName == "IMX728_IMX623_CPHY_x2_CSI-A_CSI-C")) {
        // Check the platform configuration for compatibility
        attr = {};
        status = m_pCamera->GetImageAttributes(pip, INvSIPLClient::ConsumerDesc::OutputType::ISP1, attr);
        CHK_STATUS_AND_RETURN(status, "GetImageAttributes");
        NVM_SURF_FMT_DEFINE_ATTR(surfFormatAttrs);
        nvmStatus = NvMediaSurfaceFormatGetAttrs(attr.surfaceType, surfFormatAttrs, NVM_SURF_FMT_ATTR_MAX);
        CHK_NVMSTATUS_AND_RETURN(nvmStatus, "NvMediaSurfaceFormatGetAttrs");
        if (surfFormatAttrs[NVM_SURF_ATTR_SURF_TYPE].value == NVM_SURF_ATTR_SURF_TYPE_RGBA) {
          LOG_ERR("Configuration IMX728_IMX623_CPHY_x2_CSI-A_CSI-C is not supported on this platform\n");
          return NVSIPL_STATUS_NOT_SUPPORTED;
        } else {
          platCfgChecked = true;
        }
      }

      // Get attributes
      auto pDevice = m_upDevice.get();
      status = m_pCamera->GetImageAttributes(pip, INvSIPLClient::ConsumerDesc::OutputType::ICP, attr);

      std::vector<NvMediaImageGroup *> imageGroups;
      status = AllocateImageGroups(pDevice, attr.surfaceType, attr.surfaceAllocAttr, 1U, imageGroups);

      status = m_pCamera->RegisterImageGroups(pip, imageGroups);

      m_imageGroupList.push_back(imageGroups);

      if (isp0Enabled) {
        // Get attributes
        attr = {};
        status = m_pCamera->GetImageAttributes(pip, INvSIPLClient::ConsumerDesc::OutputType::ISP0, attr);

        if (platformConfigName == "IMX728_IMX623_CPHY_x2_CSI-A_CSI-C") {
          status = IMX728IMX623SurfTypeOverride(pip, INvSIPLClient::ConsumerDesc::OutputType::ISP0, attr);
        }
#if !NV_IS_SAFETY
        // Add CPU_ACCESS_CACHED attribute if not already set, for FileWriter to be backward compatible
        // TODO: Remove this, instead get the attributes from consumers and do the attribute reconciliation
        bool found = false;
        for (const auto &it : attr.surfaceAllocAttr) {
          if (it.type == NVM_SURF_ATTR_CPU_ACCESS) {
            found = true;
            break;
          }
        }
        if (!found) {
          attr.surfaceAllocAttr.push_back({NVM_SURF_ATTR_CPU_ACCESS, NVM_SURF_ATTR_CPU_ACCESS_CACHED});
        }
#endif  // !NV_IS_SAFETY

        std::vector<NvMediaImage *> images;
        status = AllocateImages(pDevice, attr.surfaceType, attr.surfaceAllocAttr, images);

        status = m_pCamera->RegisterImages(pip, INvSIPLClient::ConsumerDesc::OutputType::ISP0, images);

        m_imagesList.push_back(images);
      }

      if (isp1Enabled) {
        // Get attributes
        attr = {};
        status = m_pCamera->GetImageAttributes(pip, INvSIPLClient::ConsumerDesc::OutputType::ISP1, attr);
        if (status != NVSIPL_STATUS_OK) {
          LOG_ERR("GetImageAttributes failed\n");
          return status;
        }
        if (platformConfigName == "IMX728_IMX623_CPHY_x2_CSI-A_CSI-C") {
          status = IMX728IMX623SurfTypeOverride(pip, INvSIPLClient::ConsumerDesc::OutputType::ISP1, attr);
          if (status != NVSIPL_STATUS_OK) {
            LOG_ERR("IMX728IMX623SurfTypeOverride failed\n");
            return status;
          }
        }
#if !NV_IS_SAFETY
        else {
          NVM_SURF_FMT_DEFINE_ATTR(tempAttr);
          NvMediaStatus nvmStatus = NvMediaSurfaceFormatGetAttrs(attr.surfaceType, tempAttr, NVM_SURF_FMT_ATTR_MAX);
          CHK_NVMSTATUS_AND_RETURN(nvmStatus, "NvMediaSurfaceFormatGetAttrs");
          uint32_t surfType = tempAttr[NVM_SURF_ATTR_SURF_TYPE].value;
          if (surfType == NVM_SURF_ATTR_SURF_TYPE_RGBA) {
            // Override surface type to YUV 420 SEMI-PLANAR, UINT8 BL for display
            NVM_SURF_FMT_DEFINE_ATTR(surfFormatAttrs);
            NVM_SURF_FMT_SET_ATTR_YUV(surfFormatAttrs, YUV, 420, SEMI_PLANAR, UINT, 8, BL);
            attr.surfaceType = NvMediaSurfaceFormatGetType(surfFormatAttrs, NVM_SURF_FMT_ATTR_MAX);
            // Override NVM_SURF_ATTR_COLOR_STD_TYPE to NVM_SURF_ATTR_COLOR_STD_REC709_ER
            for (std::vector<int>::size_type i = 0U; i < attr.surfaceAllocAttr.size(); i++) {
              if (attr.surfaceAllocAttr[i].type == NVM_SURF_ATTR_COLOR_STD_TYPE) {
                attr.surfaceAllocAttr[i].value = NVM_SURF_ATTR_COLOR_STD_REC709_ER;
                break;
              }
            }
          }
        }
        // Add CPU_ACCESS_CACHED attribute if not already set, for FileWto be backward compatible
        // TODO: Remove this, instead get the attributes from consumers and do the attribute reconciliation
        bool found = false;
        for (const auto &it : attr.surfaceAllocAttr) {
          if (it.type == NVM_SURF_ATTR_CPU_ACCESS) {
            found = true;
            break;
          }
        }
        if (!found) {
          attr.surfaceAllocAttr.push_back({NVM_SURF_ATTR_CPU_ACCESS, NVM_SURF_ATTR_CPU_ACCESS_CACHED});
        }
#endif  // !NV_IS_SAFETY
        std::vector<NvMediaImage *> images;
        status = AllocateImages(pDevice, attr.surfaceType, attr.surfaceAllocAttr, images);
        if (status != NVSIPL_STATUS_OK) {
          LOG_ERR("AllocateImages failed\n");
          return status;
        }
        status = m_pCamera->RegisterImages(pip, INvSIPLClient::ConsumerDesc::OutputType::ISP1, images);
        if (status != NVSIPL_STATUS_OK) {
          LOG_WARN("RegisterImageGroups failed\n");
          DeleteImages(images);
          return status;
        }
        m_imagesList.push_back(images);
      }
      if (isp2Enabled) {
        // Get attributes
        attr = {};
        status = m_pCamera->GetImageAttributes(pip, INvSIPLClient::ConsumerDesc::OutputType::ISP2, attr);
        if (status != NVSIPL_STATUS_OK) {
          LOG_ERR("GetImageAttributes failed\n");
          return status;
        }
        if (platformConfigName == "IMX728_IMX623_CPHY_x2_CSI-A_CSI-C") {
          status = IMX728IMX623SurfTypeOverride(pip, INvSIPLClient::ConsumerDesc::OutputType::ISP2, attr);
          if (status != NVSIPL_STATUS_OK) {
            LOG_ERR("IMX728IMX623SurfTypeOverride failed\n");
            return status;
          }
        }
#if !NV_IS_SAFETY
        else {
          // Override surface type to YUV 420 SEMI-PLANAR, UINT8 BL for display
          NVM_SURF_FMT_DEFINE_ATTR(surfFormatAttrs);
          NVM_SURF_FMT_SET_ATTR_YUV(surfFormatAttrs, YUV, 420, SEMI_PLANAR, UINT, 8, BL);
          attr.surfaceType = NvMediaSurfaceFormatGetType(surfFormatAttrs, NVM_SURF_FMT_ATTR_MAX);
          // Override NVM_SURF_ATTR_COLOR_STD_TYPE to NVM_SURF_ATTR_COLOR_STD_REC709_ER
          for (std::vector<int>::size_type i = 0U; i < attr.surfaceAllocAttr.size(); i++) {
            if (attr.surfaceAllocAttr[i].type == NVM_SURF_ATTR_COLOR_STD_TYPE) {
              attr.surfaceAllocAttr[i].value = NVM_SURF_ATTR_COLOR_STD_REC709_ER;
              break;
            }
          }
        }

        // Add CPU_ACCESS_CACHED attribute if not already set, for FileWto be backward compatible
        // TODO: Remove this, instead get the attributes from consumers and do the attribute reconciliation
        bool found = false;
        for (const auto &it : attr.surfaceAllocAttr) {
          if (it.type == NVM_SURF_ATTR_CPU_ACCESS) {
            found = true;
            break;
          }
        }
        if (!found) {
          attr.surfaceAllocAttr.push_back({NVM_SURF_ATTR_CPU_ACCESS, NVM_SURF_ATTR_CPU_ACCESS_CACHED});
        }
#endif  // !NV_IS_SAFETY

        std::vector<NvMediaImage *> images;
        status = AllocateImages(pDevice, attr.surfaceType, attr.surfaceAllocAttr, images);
        if (status != NVSIPL_STATUS_OK) {
          LOG_ERR("AllocateImageGroups failed\n");
          return status;
        }
        status = m_pCamera->RegisterImages(pip, INvSIPLClient::ConsumerDesc::OutputType::ISP2, images);

        if (status != NVSIPL_STATUS_OK) {
          LOG_WARN("RegisterImageGroups failed\n");
          DeleteImages(images);
          return status;
        }
        m_imagesList.push_back(images);
      }
    } else {
      LOG_DBG("Running in NvSciBuf API Path");
      // Get attributes
      std::vector<NvSciBufObj> sciBufObjs;
      auto status = CaptureFillAttributesAllocateSciBuf(pip, sciBufObjs);
      if (status != NVSIPL_STATUS_OK) {
        LOG_WARN("CaptureFillAttributesAllocateSciBuf failed\n");
        return status;
      }

      status = m_pCamera->RegisterImages(pip, INvSIPLClient::ConsumerDesc::OutputType::ICP, sciBufObjs);
      if (status != NVSIPL_STATUS_OK) {
        LOG_WARN("RegisterImages for Capture failed\n");
        return status;
      }

      if (isp0Enabled) {
        std::vector<NvSciBufObj> sciBufObjs;
        status = ProcessFillAttributesAllocateSciBuf(pip, INvSIPLClient::ConsumerDesc::OutputType::ISP0, sciBufObjs);
        if (status != NVSIPL_STATUS_OK) {
          LOG_WARN("ProcessFillAttributesAllocateSciBuf failed for ISP0\n");
          return status;
        }

        status = m_pCamera->RegisterImages(pip, INvSIPLClient::ConsumerDesc::OutputType::ISP0, sciBufObjs);
        if (status != NVSIPL_STATUS_OK) {
          LOG_WARN("RegisterImages  failed for ISP0\n");
          return status;
        }
      }

      if (isp1Enabled) {
        std::vector<NvSciBufObj> sciBufObjs;
        status = ProcessFillAttributesAllocateSciBuf(pip, INvSIPLClient::ConsumerDesc::OutputType::ISP1, sciBufObjs);
        if (status != NVSIPL_STATUS_OK) {
          LOG_WARN("ProcessFillAttributesAllocateSciBuf  failed for ISP1\n");
          return status;
        }

        status = m_pCamera->RegisterImages(pip, INvSIPLClient::ConsumerDesc::OutputType::ISP1, sciBufObjs);
        if (status != NVSIPL_STATUS_OK) {
          LOG_WARN("RegisterImages  failed for ISP1\n");
          return status;
        }
      }

      if (isp2Enabled) {
        std::vector<NvSciBufObj> sciBufObjs;
        status = ProcessFillAttributesAllocateSciBuf(pip, INvSIPLClient::ConsumerDesc::OutputType::ISP2, sciBufObjs);
        if (status != NVSIPL_STATUS_OK) {
          LOG_WARN("ProcessFillAttributesAllocateSciBuf  failed for ISP2\n");
          return status;
        }

        status = m_pCamera->RegisterImages(pip, INvSIPLClient::ConsumerDesc::OutputType::ISP2, sciBufObjs);
        if (status != NVSIPL_STATUS_OK) {
          LOG_WARN("RegisterImages  failed for ISP2\n");
          return status;
        }
      }
    }
    return NVSIPL_STATUS_OK;
  }

 protected:
  unique_ptr<INvSIPLCamera> m_pCamera;
  std::unique_ptr<NvMediaDevice, CloseNvMediaDevice> m_upDevice;
  NvSciBufModule m_sciBufModule{NULL};
  NvSciSyncModule m_sciSyncModule{NULL};

 private:
  std::mutex m_mutex;
  std::vector<std::vector<NvMediaImageGroup *>> m_imageGroupList;
  std::vector<std::vector<NvMediaImage *>> m_imagesList;
  std::vector<NvSciBufObj> m_sciBufObjs;
};
