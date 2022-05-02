/******************* Sample Producer Creation **********************************/

struct SiplControlAutoInputParam {
  SiplControlEmbedData embedData;
  DevBlkISCSensorAttributes sensorAttr;
  SiplControlIspStatsInfo statsInfo;
};

struct SiplControlEmbedData {
  SiplControlEmbedInfo embedInfo;
  DevBlkISCFrameSeqNum frameSeqNum;
  DevBlkISCEmbeddedDataChunk topEmbeddedData;
  DevBlkISCEmbeddedDataChunk bottomEmbeddedData;
};

struct DevBlkISCSensorAttributes {
  char sensorName[DEVBLK_ISC_MAX_SENSOR_NAME_LENGTH];
  uint32_t sensorCFA;
  char sensorFuseId[DEVBLK_ISC_MAX_FUSE_ID_LENGTH];
  uint8_t numActiveExposures;
  DevBlkISCAttrRange sensorExpRange[DEVBLK_ISC_MAX_EXPOSURES];
  DevBlkISCAttrRange sensorGainRange[…];
  DevBlkISCAttrRange sensorWhiteBalanceRange[…];
  float_t sensorGainFactor[DEVBLK_ISC_MAX_EXPOSURES];
  uint32_t numFrameReportBytes;
};

struct SiplControlIspStatsInfo {
  const NvSiplISPLocalAvgClipStatsData* lacData[2];
  const NvSiplISPLocalAvgClipStats* lacSettings[2];
  const NvSiplISPHistogramStatsData* histData[2];
  const NvSiplISPHistogramStats* histSettings[2];
  const NvSiplISPFlickerBandStatsData* fbStatsData;
  const NvSiplISPFlickerBandStats* fbStatsSettings;
};
// output
struct SiplControlAutoOutputParam {
  SiplControlAutoSensorSetting sensorSetting;
  SiplControlAutoAwbSetting awbSetting;
  SiplControlIspStatsSetting newStatsSetting;
  float_t ispDigitalGain;
};

struct SiplControlAutoSensorSetting {
  uint8_t numSensorContexts;
  DevBlkISCExposure exposureControl[…];  // Holds the sensor exposure settings to set for each context.
  DevBlkISCWhiteBalance wbControl[…];    // Holds the sensor white balance settings to set for each context.
};

struct SiplControlAutoAwbSetting {
  SiplControlAutoAwbGain wbGainTotal[…];  // Total white balance gains, including both senor channel gains and ISP
                                          // gains.
  float_t cct;                            // Correlated Color Temperature.
  float_t ccmMatrix[…][…];                // color Correlation Matrix.
};

struct SiplControlIspStatsSetting {
  bool valid;                         // to control ISP stats blocks are valid or not.
  NvSiplISPLocalAvgClipStats lac[2];  // Settings for 2 LAC stats ISP blocks.
  NvSiplISPHistogramStats hist1;      // Settings for Histogram 1 stats blocks.
  NvSiplISPFlickerBandStats fbStats;  // Settings for Flicker Band stats block.
};

SIPLStatus Process(const SiplControlAutoInputParam& inParams, SiplControlAutoOutputParam& outParams);

// currFrameEmbedInfo: Specifies the input embedded info parameters for the current frame.
// maxSupportedNoiseProfiles: Specifies the input max number of allowed noise profiles to be used by the plugin.
// noiseProfile: Specifies the output calculated noise profile number.  0<= noiseProfile < maxSupportedNoiseProfiles.
SIPLStatus GetNoiseProfile(
    const SiplControlEmbedInfo& currFrameEmbedInfo, const uint32_t maxSupportedNoiseProfiles, uint32_t& noiseProfile);
//上个函数一旦被调用, 那接下来的函数也必须被调用
// SiplControlEmbedData
// maxSupportedNoiseProfiles

const size_t NITO_PARAMETER_SET_ID_SIZE = 16U;
const size_t NITO_SCHEMA_HASH_SIZE = 32U;
const size_t NITO_DATA_HASH_SIZE = 32U;

struct NvSIPLNitoMetadata {
  uint8_t parameterSetID[NITO_PARAMETER_SET_ID_SIZE];
  uint8_t schemaHash[NITO_SCHEMA_HASH_SIZE];
  uint8_t dataHash[NITO_DATA_HASH_SIZE];
};

SIPLStatus GetNitoMetadataFromMemory(
    uint8_t const* const nitoMem, size_t const nitoMemLength, NvSIPLNitoMetadata* const metadataArray,
    size_t const metadataArrayLength, size_t* const metadataCount);

SIPLStatus GetNitoMetadataFromMemory(
    uint8_t const* const nitoMem, size_t const nitoMemLength, NvSIPLNitoMetadata* const metadataArray,
    size_t const metadataArrayLength, size_t* const metadataCount);
/******************* Sample Producer Creation **********************************/

/******************* Sample Producer Creation **********************************/