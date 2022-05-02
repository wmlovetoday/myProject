

NvSciBufModule module;
NvSciBufAttrList attrlist, conflictlist;
NvSciBufObj bufObj;
NvMediaImage* image;
NvMediaSurfAllocAttr surfAllocAttrs[8];

/*NvMedia related initialization */
NvMediaDevice* device = NvMediaDeviceCreate();
NvMediaStatus status = NvMediaImageNvSciBufInit();
/*NvSciBuf related initialization*/
NvSciError err = NvSciBufModuleOpen(&module);

/*Create NvSciBuf attribute list*/
err = NvSciBufAttrListCreate(module, &attrlist);

NVM_SURF_FMT_DEFINE_ATTR(surfFormatAttrs);
/*Initialize surfFormatAttrs and surfAllocAttrs as required */

/* Get NvMediaSurfaceType */
NvMediaSurfaceType nvmsurfacetype = NvMediaSurfaceFormatGetType(surfFormatAttrs, NVM_SURF_FMT_ATTR_MAX);
/*Ask NvMedia to fill NvSciBufAttrs corresponding to nvmsurfacetype and surfAllocAttrs*/
status = NvMediaImageFillNvSciBufAttrs(device, nvmsurfacetype, surfAllocAttrs, numsurfallocattrs, 0, attrlist);

/*Reconcile the NvSciBufAttrs and then allocate a NvSciBufObj */
err = NvSciBufAttrListReconcileAndObjAlloc(&attrlist, 1, bufobj, &conflictlist);
/*Create NvMediaImage from NvSciBufObj */
status = NvMediaImageCreateFromNvSciBuf(device, bufobj, &image);

/*Free the NvSciBufAttrList which is no longer required */
err = NvSciBufAttrListFree(attrlist);
/*Use the image as input or output for any of the Nvmedia */

/*Free the resources after use*/
/*Destroy NvMediaImage */
NvMediaImageDestroy(image);
/*NvMedia related Deinit*/
NvMediaImageNvSciBufDeinit();
NvMediaDeviceDestroy(device);
/*NvSciBuf related deinit*/
NvSciBufObjFree(bufobj);
NvSciBufModuleClose(module);

/******************* NvSciBufAttrLists **********************************/
NvSciBufType bufType = NvSciBufType_RawBuffer;
uint64_t rawsize = (128 * 1024);  // Allocate 128K Raw-buffer
uint64_t align = (4 * 1024);      // Buffer Alignment of 4K
bool cpuaccess_flag = false;
NvSciBufAttrKeyValuePair rawbuffattrs[] = {
    {NvSciBufGeneralAttrKey_Types, &bufType, sizeof(bufType)},
    {NvSciBufRawBufferAttrKey_Size, &rawsize, sizeof(rawsize)},
    {NvSciBufRawBufferAttrKey_Align, &align, sizeof(align)},
    {NvSciBufGeneralAttrKey_NeedCpuAccess, &cpuaccess_flag, sizeof(cpuaccess_flag)},
};
/* Created attrlist1 will be associated with bufmodule */
NvSciBufAttrList attrlist1;
err = NvSciBufAttrListCreate(bufmodule, &attrlist1);
err = NvSciBufAttrListSetAttrs(umd1attrlist, rawbuffattrs, sizeof(rawbuffattrs) / sizeof(NvSciBufAttrKeyValuePair));
/*......*/
NvSciBufAttrListFree(attrlist1);

/******************* NvSciBuf Reconciliation **********************************/
NvSciBufAttrList unreconciledList[2] = {NULL};
NvSciBufAttrList reconciledList = NULL;
NvSciBufAttrList ConflictList = NULL;

unreconciledList[0] = AttrList1;
unreconciledList[1] = AttrList2;

/* Reconciliation will be successful if and only all the
 * unreconciledLists belong to same NvSciBufModule and the
 * outputs of this API(i.e either reconciled attribute list
 * or conflict list will also be associated with the same
 * module with which input unreconciled lists belong to.
 */
err = NvSciBufAttrListReconcile(
    unreconciledList, /* array of unreconciled lists */
    2,                /* size of this array */
    &reconciledList,  /* output reconciled list */
    &ConflictList);   /* conflict description filled
                      in case of reconciliation failure */

NvSciSyncAttrListFree(AttrList1);
NvSciSyncAttrListFree(AttrList2);
NvSciSyncAttrListFree(reconciledList);  // In case of successful reconciliation.
NvSciSyncAttrListFree(ConflictList);    // In case of failed reconciliation.

/******************* Multi Datatype Attribute Lists Reconciliation **********************************/
NvSciBufType bufType = NvSciBufType_Image;
NvSciBufAttrValImageLayoutType layout = NvSciBufImage_PitchLinearType;
uint64_t lrpad = 0, tbpad = 0, imageCount = 1;
bool cpuaccess_flag = true;
bool vpr = false;
int32_t planecount = 1;
NvSciBufAttrValColorFmt planecolorfmts[] = {NvSciColor_A8B8G8R8};
NvSciBufAttrValColorStd planecolorstds[] = {NvSciColorStd_SRGB};
NvSciBufAttrValImageScanType planescantype[] = {NvSciBufScan_ProgressiveType};
int32_t plane_widths[] = {1920};
int32_t plane_heights[] = {1080};

NvSciBufAttrKeyValuePair imagebuffattrs[] = {
    {NvSciBufGeneralAttrKey_Types, &bufType, sizeof(bufType)},
    {NvSciBufGeneralAttrKey_NeedCpuAccess, &cpuaccess_flag, sizeof(cpuaccess_flag)},
    {NvSciBufImageAttrKey_Layout, &layout, sizeof(layout)},
    {NvSciBufImageAttrKey_TopPadding, &tbpad, sizeof(tbpad)},
    {NvSciBufImageAttrKey_BottomPadding, &tbpad, sizeof(tbpad)},
    {NvSciBufImageAttrKey_LeftPadding, &lrpad, sizeof(lrpad)},
    {NvSciBufImageAttrKey_RightPadding, &lrpad, sizeof(lrpad)},
    {NvSciBufImageAttrKey_VprFlag, &vpr, sizeof(vpr)},
    {NvSciBufImageAttrKey_PlaneCount, &planecount, sizeof(planecount)},
    {NvSciBufImageAttrKey_PlaneColorFormat, planecolorfmts, sizeof(planecolorfmts)},
    {NvSciBufImageAttrKey_PlaneColorStd, planecolorstds, sizeof(planecolorstds)},
    {NvSciBufImageAttrKey_PlaneWidth, plane_widths, sizeof(plane_widths)},
    {NvSciBufImageAttrKey_PlaneHeight, plane_heights, sizeof(plane_heights)},
    {NvSciBufImageAttrKey_ScanType, planescantype, sizeof(planescantype)},
    {NvSciBufImageAttrKey_ImageCount, &imageCount, sizeof(imageCount)},
};

err = NvSciBufModuleOpen(&bufModule);
err = NvSciBufAttrListCreate(bufModule, &app1AttrList);

err = NvSciBufAttrListSetAttrs(app1AttrList, imagebuffattrs, sizeof(imagebuffattrs) / sizeof(NvSciBufAttrKeyValuePair));
/****TBD***/
/* Need to fill NvMediaTensor attributes using Nvmedia APIs */

unreconciledLists[0] = app1AttrList;
unreconciledLists[0] = app2AttrList;
err = NvSciBufAttrListReconcileAndObjAlloc(unreconciledLists, &nvscibufObj, &conflictList);
// NvSciBufObjAlloc

/******************* NvSciBuf Object **********************************/
NvSciBufType bufType = NvSciBufType_Image;
/* Allocate a Buffer using reconciled attribute list and the
 * created NvSciBufObj will be associated with the module to
 * which reconciledAttrlist belongs to.
 */
err = NvSciBufAttrListObjAlloc(reconciledAttrlist, &nvscibufobj);

/* Get the associated reconciled attrlist of the object. */
err = NvSciBufObjGetAttrList(nvscibufobj, &objReconciledAttrList);

err = NvSciBufObjGetCpuPtr(nvscibufobj, &va_ptr);

/******************* Vidmem Allocation **********************************/
NvSciBufType bufType = NvSciBufType_RawBuffer;
uint64_t rawBufSize = (8U * 1024U);
uint64_t alignment = (4U * 1024U);
bool cpuAccessFlag = false;
NvSciBufAttrKeyValuePair rawBufAttrs[] = {{NvSciBufGeneralAttrKey_Types, &bufType, sizeof(bufType)},
                                          {NvSciBufRawBufferAttrKey_Size, &rawBufSize, sizeof(rawBufSize)},
                                          {NvSciBufRawBufferAttrKey_Align, &alignment, sizeof(alignment)},
                                          {NvSciBufGeneralAttrKey_NeedCpuAccess, &cpuAccessFlag, sizeof(cpuAccessFlag)},
                                          {NvSciBufGeneralAttrKey_VidMem_GpuId, &uuId, sizeof(uuId)}};
err = NvSciBufModuleOpen(&bufModule);
err = NvSciBufAttrListCreate(bufModule, appAttrList);
err = NvSciBufAttrListSetAttrs(appAttrList, rawBufAttrs, sizeof(rawBufAttrs) / sizeof(NvSciBufAttrKeyValuePair));
err = NvSciBufAttrListReconcileAndObjAlloc(appAttrlist, &nvscibufObj, &conflictList);

/******************* NvSciIpc Init **********************************/
NvSciIpcEndpoint ipcEndpoint = 0;
err = NvSciIpcInit();
err = NvSciIpcOpenEndpoint("ipc_endpoint", &ipcEndpoint);
NvSciIpcCloseEndpoint(ipcEndpoint);
NvSciIpcDeinit();
/******************* Export/Import NvSciBuf AttrLists **********************************/

/* --------------------------App Process1 ----------------------------------*/
NvSciBufAttrList AttrList1 = NULL;
void* ListDesc = NULL;
size_t ListDescSize = 0U;
/* creation of the attribute list, receiving other lists from other listeners */
err = NvSciBufAttrListIpcExportUnreconciled(
    &AttrList1,     /* array of unreconciled lists to be exported */
    1,              /* size of the array */
    ipcEndpoint,    /* valid and opened NvSciIpcEndpoint intended to send the descriptor through */
    &ListDesc,      /* The descriptor buffer to be allocated and filled in */
    &ListDescSize); /* size of the newly created buffer */

/* send the descriptor to the process2 */
/* wait for process 1 to reconcile and export reconciled list */
err = NvSciBufAttrListIpcImportReconciled(
    module,                    /* NvSciBuf module using which this attrlist to be imported */
    ipcEndpoint,               /* valid and opened NvSciIpcEndpoint on which the descriptor is received */
    ListDesc,                  /* The descriptor buffer to be imported */
    ListDescSize,              /* size of the descriptor buffer */
    &AttrList1,                /* array of unreconciled lists to be used for validating the reconciled list */
    1,                         /* Number or unreconciled lists */
    &reconciledAttrList,       /* Imported reconciled list */
    
/* --------------------------App Process2 ----------------------------------*/
void* ListDesc = NULL;
size_t ListDescSize = 0U;
NvSciBufAttrList unreconciledList[2] = {NULL};
NvSciBufAttrList reconciledList = NULL;
NvSciBufAttrList newConflictList = NULL;
NvSciBufAttrList AttrList2 = NULL;
NvSciSyncAttrList importedUnreconciledAttrList = NULL;
/* create the local AttrList */
/* receive the descriptor from the other process */
err = NvSciBufAttrListIpcImportUnreconciled(module, ipcEndpoint,
    ListDesc, ListDescSize,
    &importedUnreconciledAttrList);

/* gather all the lists into an array and reconcile */
unreconciledList[0] = AttrList2;
unreconciledList[1] = importedUnreconciledAttrList;
err = NvSciBufAttrListReconcile(unreconciledList, 2, &reconciledList,&newConflictList);

err = NvSciBufAttrListIpcExportReconciled(
    &AttrList1,                /* array of unreconciled lists to be exported */
    ipcEndpoint,               /* valid and opened NvSciIpcEndpoint intended to send the descriptor through */
    &ListDesc,                 /* The descriptor buffer to be allocated and filled in */
    &ListDescSize );           /* size of the newly created buffer */

/******************* Export/Import NvSciBufObj **********************************/
 /* process1 */
void* objAndList;
size_t objAndListSize;
err = NvSciBufIpcExportAttrListAndObj(
    bufObj,                        /* bufObj to be exported (the reconciled list is inside it) */
    NvSciBufAccessPerm_ReadOnly,   /* permissions we want the receiver to have */
    ipcEndpoint,                   /* IpcEndpoint via which the object is to be exported */
    &objAndList,                   /* descriptor of the object and list to be communicated */
    &objAndListSize);              /* size of the descriptor */
    /* send via Ipc */
/* process2 */
void* objAndList;
size_t objAndListSize;
err = NvSciBufIpcImportAttrListAndObj(
    module,                        /* NvSciBufModule use to create original unreconciled lists in the waiter */
    ipcEndpoint,                   /* ipcEndpoint from which the descriptor was received */
    objAndList,                    /* the desciptor of the buf obj and associated reconciled attribute list received from the signaler */
    objAndListSize,                /* size of the descriptor */
    &AttrList1,                    /* the array of original unreconciled lists prepared in this process */
    1,                             /* size of the array */
    NvSciBufAccessPerm_ReadOnly,   /* permissions expected by this process */
    10000U,                        /* timeout in microseconds. Some primitives might require time to transport all needed resources */
    &bufObj);                      /* buf object generated from the descriptor */
/* use the buf object */
NvSciBufObjFree(bufObj);

/******************* NvSciBuf and NvMediaImage API Usage **********************************/
NvSciBufModule module;
NvSciBufAttrList attrlist, conflictlist;
NvSciBufObj bufObj;
NvMediaImage *image;
NvMediaSurfAllocAttr surfAllocAttrs[8];
NVM_SURF_FMT_DEFINE_ATTR(surfFormatAttrs);
    /*NvMedia related initialization */
NvMediaDevice * device = NvMediaDeviceCreate();
NvMediaStatus status = NvMediaImageNvSciBufInit();
NvSciError err = NvSciBufModuleOpen(&module);
    /*Create NvSciBuf attribute list*/
err = NvSciBufAttrListCreate(module, &attrlist);
    /*Initialize surfFormatAttrs and surfAllocAttrs as required */

    /* Get NvMediaSurfaceType */
NvMediaSurfaceType nvmsurfacetype = NvMediaSurfaceFormatGetType(surfFormatAttrs, NVM_SURF_FMT_ATTR_MAX);
    /*Ask NvMedia to fill NvSciBufAttrs corresponding to nvmsurfacetype and surfAllocAttrs*/
status = NvMediaImageFillNvSciBufAttrs(device, nvmsurfacetype, surfAllocAttrs, numsurfallocattrs, 0, attrlist);
    /*Reconcile the NvSciBufAttrs and then allocate a NvSciBufObj */
err = NvSciBufAttrListReconcileAndObjAlloc(&attrlist, 1, bufobj,  &conflictlist);
    /*Create NvMediaImage from NvSciBufObj */
status = NvMediaImageCreateFromNvSciBuf(device, bufobj, &image);
    /*Free the NvSciBufAttrList which is no longer required */
err = NvSciBufAttrListFree(attrlist);
    /*Use the image as input or output for any of the Nvmedia */

    /*Free the resources after use*/
    /*Destroy NvMediaImage */
    NvMediaImageDestroy(image);
    /*NvMedia related Deinit*/
    NvMediaImageNvSciBufDeinit();
    NvMediaDeviceDestroy(device);
    /*NvSciBuf related deinit*/
    NvSciBufObjFree(bufobj);
    NvSciBufModuleClose(module);
/******************* Multi Datatype Attribute Lists Reconciliation **********************************/

/******************* Multi Datatype Attribute Lists Reconciliation **********************************/
/******************* Multi Datatype Attribute Lists Reconciliation **********************************/
/******************* Multi Datatype Attribute Lists Reconciliation **********************************/
/******************* Multi Datatype Attribute Lists Reconciliation **********************************/
/******************* Multi Datatype Attribute Lists Reconciliation **********************************/
