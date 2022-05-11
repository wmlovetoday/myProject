enum {
  CAPTURE_ELEMENT = 0,
  SAVE_ELEMENT,
  COMPOSITE_ELEMENT,
  DISPLAY_ELEMENT,
  ERR_HANDLER_ELEMENT,
  CMD_HANDLER_ELEMENT,
  MAX_NUM_ELEMENTS,
};

typedef struct {
  void *ctxs[MAX_NUM_ELEMENTS];
  TestArgs *testArgs;
  volatile NvMediaBool quit;
  volatile NvMediaBool suspend;
  volatile NvMediaBool resume;
} NvMainContext;

NvMediaStatus CaptureInit(NvMainContext *mainCtx) {
  ExtImgDevParam *extImgDevParam;
  // malloc ctx->ctxs[0]
  mainCtx->ctxs[CAPTURE_ELEMENT] = malloc(sizeof(NvCaptureContext));
  NvCaptureContext *captureCtx = mainCtx->ctxs[CAPTURE_ELEMENT];

  TestArgs *testArgs = mainCtx->testArgs;
  captureCtx->quit = &mainCtx->quit;
  captureCtx->suspend = &mainCtx->suspend;
  captureCtx->testArgs = testArgs;
  captureCtx->numSensors = testArgs->numSensors;
  captureCtx->numVirtualChannels = testArgs->numVirtualChannels;
  extImgDevParam = &captureCtx->extImgDevParam;

  for (i = 0; i < captureCtx->numVirtualChannels; i++) {
    captureCtx->captureParams[i] = &testArgs->captureConfigCollection[testArgs->config[i].uIntValue];
  }

  /* Set ExtImgDev params */
  status = _SetExtImgDevParameters(captureCtx, captureCtx->captureParams[defaultCaptureSet], extImgDevParam);

  /* Create ExtImgDev object */
  captureCtx->extImgDevice = ExtImgDevInit(extImgDevParam);

  /* Create NvMedia Device */
  captureCtx->device = NvMediaDeviceCreate();

  /* Set NvMediaICPSettingsEx */
  captureCtx->icpSettingsEx.numVirtualGroups = captureCtx->numVirtualChannels;
  captureCtx->icpSettingsEx.interfaceType = captureCtx->extImgDevice->property.interface;
  captureCtx->icpSettingsEx.interfaceLanes = captureCtx->captureParams[defaultCaptureSet]->csiLanes;
  captureCtx->icpSettingsEx.phyMode = captureCtx->extImgDevice->property.phyMode;
  // and so on

  /* Start ExtImgDevice */
  if (captureCtx->extImgDevice) ExtImgDevStart(captureCtx->extImgDevice);
};