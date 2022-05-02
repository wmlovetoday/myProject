/******************* Simple Stream Loop **********************************/
/* Initialize empty fences for each direction*/
NvSciSyncFence nvmediaToCudaFence = NV_SCI_SYNC_FENCE_INITIALIZER;
NvSciSyncFence cudaToNvmediaFence = NV_SCI_SYNC_FENCE_INITIALIZER;
/* Main rendering loop */
while (!done) {
  NvMedia2DComposeresult result;
  NvMedia2DComposeParameters params;
  NvMedia2DGetComposeParameters(nvmedia2D, &params);

  /* Generate a fence when rendering finishes */
  NvMedia2DSetNvSciSyncObjforEOF(nvmedia2D, params, nvmediaToCudaSync);

  /* Instruct NvMedia pipeline to wait for the fence from CUDA */
  NvMedia2DInsertPreNvSciSyncFence(nvmedia2D, params, cudaToNvmediaFence);
  /* Generate NvMedia image */
  NvMedia2DSomeRenderingOperation(...);
  NvMedia2DCompose(nvmedia2D, params, &result);

  NvMedia2DGetEOFNvSciSyncFence(nvmedia2D, result, &nvmediaToCudaFence);  // nvmedia write complete

  /* Instruct CUDA pipeline to wait for fence from NvMedia */
  CUDA_EXTERNAL_SEMAPHORE_WAIT_PARAMS cudaWaitParams;
  cudaWaitParams.params.nvSciSync.fence = (void*)&nvmediaToCudaFence;
  cudaWaitParams.flags = 0;
  cudaWaitExternalSemaphoresAsync(&nvmediaToCudaSem, &cudaWaitParams, 1, cudaStream);

  /* Process the frame in CUDA */
  cudaSomeProcessingOperation(..., cudaBuffer, ...);

  /* Generate a fence when processing finishes */
  CUDA_EXTERNAL_SEMAPHORE_SIGNAL_PARAMS cudaSignalParams;
  cudaSignalParams.params.nvSciSync.fence = (void*)&cudaToNvmediaFence;
  cudaSignalParams.flags = 0;
  cudaSignalExternalSemaphoresAsync(&cudaToNvmediaSem, &cudaSignalParams, 1, cudaStream);
}

/******************* Simple Stream Setup **********************************/
// uses NvSciBuf and NvSciSync to take images from a camera controlled by NvMedia and sends the images to CUDA for
// processing.
/* Initialize NvMedia (not all steps are shown) */
NvMedia2D* nvmedia2D;
NvMedia2DCreate(nvmedia2D, NULL);

/* Initialize CUDA (not all steps are shown) */
CUdevice cudaDevice;
cuDeviceGet(&cudaDevice, IGPU);
CUcontext cudaContext;
cuCtxCreate(&cudaContext, CU_CTX_MAP_HOST, dev);
cuCtxPushCurrent(&cudaContext);
CUstream cudaStream;
cuStreamCreate(&cudaStream, CU_STREAM_DEFAULT);

/* Initialize NvSci buffer and sync modules */
NvSciBufModule bufModule;
NvSciBufModuleOpen(&bufModule);
NvSciSyncModule syncModule;
NvSciSyncModuleOpen(&syncModule);

/* Obtain NvMedia buffer requirements */
NvSciBufAttrList nvmediaBufAttrs;
NvSciBufAttrListCreate(bufModule, &nvmediaBufAttrs);
NvMedia2DFillNvSciBufAttrList(nvmedia2D, nvmediaBufAttrs);

// Set more buffer attributes using NvSciBufAttrListSetAttrs.

/* Obtain NvMedia sync requirements */
NvSciSyncAttrList nvmediaWriteSyncAttrs, nvmediaReadSyncAttrs;
NvSciSyncAttrListCreate(syncModule, &nvmediaWriteSyncAttrs);
NvMedia2DFillNvSciSyncAttrList(nvmediaWriteSyncAttrs, NVMEDIA_SIGNALER);
NvSciSyncAttrListCreate(syncModule, &nvmediaReadSyncAttrs);
NvMedia2DFillNvSciSyncAttrList(nvmediaReadSyncAttrs, NVMEDIA_WAITER);

/* Obtain CUDA buffer requirements */
NvSciBufAttrList cudaBufAttrs;
NvSciBufAttrListCreate(bufModule, &cudaBufAttrs);
// Fill in with CUDA raw buffer attributes

/* Obtain CUDA sync requirements */
NvSciSyncAttrList cudaWriteSyncAttrs, cudaReadSyncAttrs;
NvSciSyncAttrListCreate(syncModule, &cudaWriteSyncAttrs);
cuDeviceGetNvSciSyncAttributes(cudaWriteSyncAttrs, cudaDevice, CUDA_NVSCISYNC_ATTR_SIGNAL);
NvSciSyncAttrListCreate(syncModule, &cudaReadSyncAttrs);
cuDeviceGetNvSciSyncAttributes(cudaReadSyncAttrs, cudaDevice, CUDA_NVSCISYNC_ATTR_WAIT);

/* Combine buffer requirements and allocate buffer */
NvSciBufAttrList allBufAttrs[2], conflictBufAttrs;
NvSciBufAttrList combinedBufAttrs;
allBufAttrs[0] = nvmediaBufAttrs;
allBufAttrs[1] = cudaBufAttrs;
NvSciBufAttrListReconcile(allBufAttrs, 2, &combinedBufAttrs, &conflictBufAttrs);
NvSciBufObj buffer;
NvSciBufObjAlloc(combinedBufAttrs, &buffer);

/* Combine sync requirements and allocate
nvmedia to cuda sync object */
NvSciSyncAttrList allSyncAttrs[2], conflictSyncAttrs;
allSyncAttrs[0] = nvmediaWriteSyncAttrs;
allSyncAttrs[1] = cudaReadSyncAttrs;
NvSciSyncAttrList nvmediaToCudaSyncAttrs;
NvSciSyncAttrListReconcile(allSyncAttrs, 2, &nvmediaToCudaSyncAttrs, &confictSyncAttrs);
NvSciSyncObj nvmediaToCudaSync;
NvSciSyncObjAlloc(nvmediaToCudaSyncAttrs, &nvmediaToCudaSync);

/* Combine sync requirements and allocate cuda
to nvmedia sync object */
allSyncAttrs[0] = cudaWriteSyncAttrs;
allSyncAttrs[1] = nvmediaReadSyncAttrs;
NvSciSyncAttrList cudaToNvmediaSyncAttrs;
NvSciSyncAttrListReconcile(allSyncAttrs, 2, &cudaToNvmediaSyncAttrs, &confictSyncAttrs);
NvSciSyncObj cudaToNvmediaSync;
NvSciSyncObjAlloc(cudaToNvmediaSyncAttrs, &cudaToNvmediaSync);

/* Map objects into NvMedia */
NvMedia2DRegisterNvSciBufObj(nvmedia2D, buffer);
NvMedia2DRegisterNvSciSyncObj(nvmedia2D, NVMEDIA_EOFSYNCOBJ, nvmediaToCudaSync);
NvMedia2DRegisterNvSciSyncObj(nvmedia2D, NVMEDIA_PRESYNCOBJ, cudaToNvmediaSync);

/* Map objects into CUDA */
cudaExternalMemoryHandleDesc cudaMemHandleDesc;
memset(&cudaMemHandleDesc, 0, sizeof(cudaMemHandleDesc));
cudaMemHandleDesc.type = cudaExternalMemoryHandleTypeNvSciBuf;
cudaMemHandleDesc.handle.nvSciBufObject = buffer;
cudaMemHandleDesc.size = <allocated size>;
cudaImportExternalMemory(&cudaBuffer, &cudaMemHandleDesc);
CUDA_EXTERNAL_SEMAPHORE_HANDLE_DESC cudaSemDec;
CUexternalSemaphore nvmediaToCudaSem, cudaToNvmediaSem;
cudaSemDesc.type = CU_EXTERNAL_SEMAPHORE_HANDLE_TYPE_NVSCISYNC;
cudaSemDesc.handle.nvSciSyncObj = (void*)nvmediaToCudaSync;
cuImportExternalSemaphore(&nvmediaToCudaSem, &cudaSemDesc);
cudaSemDesc.type = CU_EXTERNAL_SEMAPHORE_HANDLE_TYPE_NVSCISYNC;
cudaSemDesc.handle.nvSciSyncObj = (void*)cudaToNvmediaSync;
cuImportExternalSemaphore(&cudaToNvmediaSem, &cudaSemDesc);

/******************* NvSciBufAttrLists **********************************/
/******************* NvSciBufAttrLists **********************************/
/******************* NvSciBufAttrLists **********************************/
/******************* NvSciBufAttrLists **********************************/
/******************* NvSciBufAttrLists **********************************/
/******************* NvSciBufAttrLists **********************************/
/******************* NvSciBufAttrLists **********************************/
