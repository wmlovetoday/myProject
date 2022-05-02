/******************* Sample Producer Creation **********************************/
// We'll use triple buffering for this stream
const uint32_t numPackets = 3;
// Stream variables
NvSciIpcEndpoint srcIpc;
NvSciBufModule bufModule = 0;
NvSciSyncModule syncModule = 0;
NvSciStreamBlock producerBlock = 0;
NvSciStreamBlock poolBlock = 0;
NvSciStreamBlock srcIpcBlock = 0;
NvSciError err;
// Setting up communication is outside the scope of this guide
srcIpc = <something>;
// Set up buffer and sync modules
// (If using multiple streams, or doing other non-stream NvSci operations, these might be passed in from some global
// setup.)
err = NvSciBufModuleOpen(&bufModule);

err = NvSciSyncModuleOpen(&syncModule);

// Create all the stream blocks
err = NvSciStreamStaticPoolCreate(numPackets, &poolBlock);

err = NvSciStreamProducerCreate(poolBlock, &producerBlock);

err = NvSciStreamIpcSrcCreate(srcIpc, syncModule, bufModule, &srcIpcBlock);

// Connect the blocks
err = NvSciStreamBlockConnect(producerBlock, srcIpcBlock);

/******************* Sample Consumer Creation **********************************/
// Stream variables
NvSciIpcEndpoint dstIpc;
NvSciBufModule bufModule = 0;
NvSciSyncModule syncModule = 0;
NvSciStreamBlock consumerBlock = 0;
NvSciStreamBlock fifoBlock = 0;
NvSciStreamBlock dstIpcBlock = 0;
NvSciError err;
// Setting up communication is outside the scope of this guide
dstIpc = <something>;
// Set up buffer and sync modules
// (If using multiple streams, or doing other non-stream NvSci operations, these might be passed in from some global
// setup.)
err = NvSciBufModuleOpen(&bufModule);

err = NvSciSyncModuleOpen(&syncModule);

// Create all the stream blocks
err = NvSciStreamFifoQueueCreate(&fifoBlock);

err = NvSciStreamConsumerCreate(fifoBlock, &consumerBlock);

err = NvSciStreamIpcDstCreate(dstIpc, syncModule, bufModule, &dstIpcBlock);

// Connect the blocks
err = NvSciStreamBlockConnect(dstIpcBlock, consumerBlock);
