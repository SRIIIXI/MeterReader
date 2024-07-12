cmake_minimum_required(VERSION 3.16)

project(MeterReader VERSION 5.0 LANGUAGES CXX)

set(PROJECT_VERSION 5)
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 0)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_SHARED_LIBRARY_SUFFIX "")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if(UNIX AND NOT APPLE)
    if(${CMAKE_SYSTEM_NAME} MATCHES "Android")
        message("-- Configuring for Android ...")
        set(ANDROID_LINUX TRUE)
    endif()
endif()

if(UNIX AND APPLE)
    message("-- Configuring for Darwin (MacOS, iOS) ...")
    set(DARWIN TRUE)
endif()

include(./qzxing/zxing.cmake)

if(DARWIN)
find_package(Qt6 6.7 REQUIRED COMPONENTS Quick Core Sql Qml Multimedia Bluetooth)
qt_standard_project_setup(REQUIRES 6.7)
endif(DARWIN)

if(ANDROID)
find_package(Qt6 6.7 REQUIRED COMPONENTS Quick Core Sql Qml Multimedia Bluetooth)
qt_standard_project_setup(REQUIRES 6.7)
endif(ANDROID)

set(SOURCE_DIR ./src)
set(HEADER_DIR ./include)
set(DLMS_SOURCE_DIR ./dlms/src)
set(DLMS_HEADER_DIR ./dlms/include)
set(QZXING_SOURCE_DIR ./qzxing/src)
set(QZXING_HEADER_DIR ./qzxing/include)

include_directories(${PROJECT_HEADER_DIR} ./include ./dlms/include/ ./qzxing/include ./darwin)

set(DLMS_SOURCES
${DLMS_SOURCES}
${DLMS_SOURCE_DIR}/AdjacentCell.cpp
${DLMS_SOURCE_DIR}/APDU.cpp
${DLMS_SOURCE_DIR}/ApplicationContextName.cpp
${DLMS_SOURCE_DIR}/AuthenticationMechanismName.cpp
${DLMS_SOURCE_DIR}/BitString.cpp
${DLMS_SOURCE_DIR}/ByteBuffer.cpp
${DLMS_SOURCE_DIR}/ChargePerUnitScaling.cpp
${DLMS_SOURCE_DIR}/ChargeTable.cpp
${DLMS_SOURCE_DIR}/Cipher.cpp
${DLMS_SOURCE_DIR}/Commodity.cpp
${DLMS_SOURCE_DIR}/CreditChargeConfiguration.cpp
${DLMS_SOURCE_DIR}/Currency.cpp
${DLMS_SOURCE_DIR}/DateTime.cpp
${DLMS_SOURCE_DIR}/DLMS.cpp
${DLMS_SOURCE_DIR}/DLMSAccessItem.cpp
${DLMS_SOURCE_DIR}/DLMSAccount.cpp
${DLMS_SOURCE_DIR}/DLMSActionItem.cpp
${DLMS_SOURCE_DIR}/DLMSActionSchedule.cpp
${DLMS_SOURCE_DIR}/DLMSActionSet.cpp
${DLMS_SOURCE_DIR}/DLMSActivityCalendar.cpp
${DLMS_SOURCE_DIR}/DLMSArbitrator.cpp
${DLMS_SOURCE_DIR}/DLMSAssociationLogicalName.cpp
${DLMS_SOURCE_DIR}/DLMSAssociationShortName.cpp
${DLMS_SOURCE_DIR}/DLMSCaptureObject.cpp
${DLMS_SOURCE_DIR}/DLMSCertificateInfo.cpp
${DLMS_SOURCE_DIR}/DLMSCharge.cpp
${DLMS_SOURCE_DIR}/DLMSClient.cpp
${DLMS_SOURCE_DIR}/DLMSClock.cpp
${DLMS_SOURCE_DIR}/DLMSCommunicationPortProtection.cpp
${DLMS_SOURCE_DIR}/DLMSCompactData.cpp
${DLMS_SOURCE_DIR}/DLMSContextType.cpp
${DLMS_SOURCE_DIR}/DLMSConverter.cpp
${DLMS_SOURCE_DIR}/DLMSCredit.cpp
${DLMS_SOURCE_DIR}/DLMSData.cpp
${DLMS_SOURCE_DIR}/DLMSDayProfile.cpp
${DLMS_SOURCE_DIR}/DLMSDayProfileAction.cpp
${DLMS_SOURCE_DIR}/DLMSDemandRegister.cpp
${DLMS_SOURCE_DIR}/DLMSDisconnectControl.cpp
${DLMS_SOURCE_DIR}/DLMSEmergencyProfile.cpp
${DLMS_SOURCE_DIR}/DLMSExtendedRegister.cpp
${DLMS_SOURCE_DIR}/DLMSGPRSSetup.cpp
${DLMS_SOURCE_DIR}/DLMSGSMCellInfo.cpp
${DLMS_SOURCE_DIR}/DLMSGSMDiagnostic.cpp
${DLMS_SOURCE_DIR}/DLMSHdlcSetup.cpp
${DLMS_SOURCE_DIR}/DLMSIec8802LlcType1Setup.cpp
${DLMS_SOURCE_DIR}/DLMSIec8802LlcType2Setup.cpp
${DLMS_SOURCE_DIR}/DLMSIec8802LlcType3Setup.cpp
${DLMS_SOURCE_DIR}/DLMSIECOpticalPortSetup.cpp
${DLMS_SOURCE_DIR}/DLMSIecTwistedPairSetup.cpp
${DLMS_SOURCE_DIR}/DLMSImageActivateInfo.cpp
${DLMS_SOURCE_DIR}/DLMSImageTransfer.cpp
${DLMS_SOURCE_DIR}/DLMSIp4Setup.cpp
${DLMS_SOURCE_DIR}/DLMSIp4SetupIpOption.cpp
${DLMS_SOURCE_DIR}/DLMSIp6Setup.cpp
${DLMS_SOURCE_DIR}/DLMSLimiter.cpp
${DLMS_SOURCE_DIR}/DLMSLlcSscsSetup.cpp
${DLMS_SOURCE_DIR}/DLMSLNCommandHandler.cpp
${DLMS_SOURCE_DIR}/DLMSLNParameters.cpp
${DLMS_SOURCE_DIR}/DLMSMacAddressSetup.cpp
${DLMS_SOURCE_DIR}/DLMSMd5.cpp
${DLMS_SOURCE_DIR}/DLMSMessageHandler.cpp
${DLMS_SOURCE_DIR}/DLMSModemConfiguration.cpp
${DLMS_SOURCE_DIR}/DLMSModemInitialisation.cpp
${DLMS_SOURCE_DIR}/DLMSMonitoredValue.cpp
${DLMS_SOURCE_DIR}/DLMSNotify.cpp
${DLMS_SOURCE_DIR}/DLMSNtpSetup.cpp
${DLMS_SOURCE_DIR}/DLMSObject.cpp
${DLMS_SOURCE_DIR}/DLMSObjectCollection.cpp
${DLMS_SOURCE_DIR}/DLMSObjectDefinition.cpp
${DLMS_SOURCE_DIR}/DLMSObjectFactory.cpp
${DLMS_SOURCE_DIR}/DLMSParameterMonitor.cpp
${DLMS_SOURCE_DIR}/DLMSPppSetup.cpp
${DLMS_SOURCE_DIR}/DLMSPppSetupIPCPOption.cpp
${DLMS_SOURCE_DIR}/DLMSPppSetupLcpOption.cpp
${DLMS_SOURCE_DIR}/DLMSPrimeNbOfdmPlcApplicationsIdentification.cpp
${DLMS_SOURCE_DIR}/DLMSPrimeNbOfdmPlcMacCounters.cpp
${DLMS_SOURCE_DIR}/DLMSPrimeNbOfdmPlcMacFunctionalParameters.cpp
${DLMS_SOURCE_DIR}/DLMSPrimeNbOfdmPlcMacNetworkAdministrationData.cpp
${DLMS_SOURCE_DIR}/DLMSPrimeNbOfdmPlcMacSetup.cpp
${DLMS_SOURCE_DIR}/DLMSPrimeNbOfdmPlcPhysicalLayerCounters.cpp
${DLMS_SOURCE_DIR}/DLMSProfileGeneric.cpp
${DLMS_SOURCE_DIR}/DLMSQualityOfService.cpp
${DLMS_SOURCE_DIR}/DLMSRegister.cpp
${DLMS_SOURCE_DIR}/DLMSRegisterActivation.cpp
${DLMS_SOURCE_DIR}/DLMSRegisterMonitor.cpp
${DLMS_SOURCE_DIR}/DLMSSapAssignment.cpp
${DLMS_SOURCE_DIR}/DLMSSchedule.cpp
${DLMS_SOURCE_DIR}/DLMSScheduleEntry.cpp
${DLMS_SOURCE_DIR}/DLMSScript.cpp
${DLMS_SOURCE_DIR}/DLMSScriptAction.cpp
${DLMS_SOURCE_DIR}/DLMSScriptTable.cpp
${DLMS_SOURCE_DIR}/DLMSSeasonProfile.cpp
${DLMS_SOURCE_DIR}/DLMSSecureClient.cpp
${DLMS_SOURCE_DIR}/DLMSSecureServer.cpp
${DLMS_SOURCE_DIR}/DLMSSecuritySetup.cpp
${DLMS_SOURCE_DIR}/DLMSServer.cpp
${DLMS_SOURCE_DIR}/DLMSSettings.cpp
${DLMS_SOURCE_DIR}/DLMSSFSKActiveInitiator.cpp
${DLMS_SOURCE_DIR}/DLMSSFSKMacCounters.cpp
${DLMS_SOURCE_DIR}/DLMSSFSKMacSynchronizationTimeouts.cpp
${DLMS_SOURCE_DIR}/DLMSSFSKPhyMacSetUp.cpp
${DLMS_SOURCE_DIR}/DLMSSFSKReportingSystemList.cpp
${DLMS_SOURCE_DIR}/DLMSSha1.cpp
${DLMS_SOURCE_DIR}/DLMSSha256.cpp
${DLMS_SOURCE_DIR}/DLMSSNCommandHandler.cpp
${DLMS_SOURCE_DIR}/DLMSSNParameters.cpp
${DLMS_SOURCE_DIR}/DLMSSpecialDay.cpp
${DLMS_SOURCE_DIR}/DLMSSpecialDaysTable.cpp
${DLMS_SOURCE_DIR}/DLMSTarget.cpp
${DLMS_SOURCE_DIR}/DLMSTcpUdpSetup.cpp
${DLMS_SOURCE_DIR}/DLMSTokenGateway.cpp
${DLMS_SOURCE_DIR}/DLMSTranslator.cpp
${DLMS_SOURCE_DIR}/DLMSTranslatorStructure.cpp
${DLMS_SOURCE_DIR}/DLMSUtilityTables.cpp
${DLMS_SOURCE_DIR}/DLMSValueEventArg.cpp
${DLMS_SOURCE_DIR}/DLMSVariant.cpp
${DLMS_SOURCE_DIR}/DLMSWeekProfile.cpp
${DLMS_SOURCE_DIR}/HdlcSettings.cpp
${DLMS_SOURCE_DIR}/Helpers.cpp
${DLMS_SOURCE_DIR}/PlcSettings.cpp
${DLMS_SOURCE_DIR}/ReplyData.cpp
${DLMS_SOURCE_DIR}/Secure.cpp
${DLMS_SOURCE_DIR}/SerialNumberCounter.cpp
${DLMS_SOURCE_DIR}/SNInfo.cpp
${DLMS_SOURCE_DIR}/StandardObisCode.cpp
${DLMS_SOURCE_DIR}/StandardObisCodeCollection.cpp
${DLMS_SOURCE_DIR}/TokenGatewayConfiguration.cpp
${DLMS_SOURCE_DIR}/UnitCharge.cpp
${DLMS_SOURCE_DIR}/XmlReader.cpp
${DLMS_SOURCE_DIR}/XmlWriter.cpp
${DLMS_SOURCE_DIR}/XmlWriterSettings.cpp
${DLMS_SOURCE_DIR}/AES.cpp
${DLMS_SOURCE_DIR}/AES-GCM.cpp
${DLMS_SOURCE_DIR}/GCM.cpp
${DLMS_SOURCE_DIR}/SHA256.cpp
)

set(DLMS_HEADERS
${DLMS_HEADERS}
${DLMS_HEADER_DIR}/AdjacentCell.h
${DLMS_HEADER_DIR}/APDU.h
${DLMS_HEADER_DIR}/ApplicationContextName.h
${DLMS_HEADER_DIR}/AttributeCollection.h
${DLMS_HEADER_DIR}/Authentication.h
${DLMS_HEADER_DIR}/AuthenticationMechanismName.h
${DLMS_HEADER_DIR}/BitString.h
${DLMS_HEADER_DIR}/ByteBuffer.h
${DLMS_HEADER_DIR}/ChargePerUnitScaling.h
${DLMS_HEADER_DIR}/ChargeTable.h
${DLMS_HEADER_DIR}/ChipperingEnums.h
${DLMS_HEADER_DIR}/Cipher.h
${DLMS_HEADER_DIR}/Commodity.h
${DLMS_HEADER_DIR}/CreditChargeConfiguration.h
${DLMS_HEADER_DIR}/Currency.h
${DLMS_HEADER_DIR}/DataInfo.h
${DLMS_HEADER_DIR}/Date.h
${DLMS_HEADER_DIR}/DateTime.h
${DLMS_HEADER_DIR}/DLMS.h
${DLMS_HEADER_DIR}/DLMSAccessItem.h
${DLMS_HEADER_DIR}/DLMSAccount.h
${DLMS_HEADER_DIR}/DLMSActionItem.h
${DLMS_HEADER_DIR}/DLMSActionSchedule.h
${DLMS_HEADER_DIR}/DLMSActionSet.h
${DLMS_HEADER_DIR}/DLMSActivityCalendar.h
${DLMS_HEADER_DIR}/DLMSArbitrator.h
${DLMS_HEADER_DIR}/DLMSAssociationLogicalName.h
${DLMS_HEADER_DIR}/DLMSAssociationShortName.h
${DLMS_HEADER_DIR}/DLMSAttribute.h
${DLMS_HEADER_DIR}/DLMSCaptureObject.h
${DLMS_HEADER_DIR}/DLMSCertificateInfo.h
${DLMS_HEADER_DIR}/DLMSCharge.h
${DLMS_HEADER_DIR}/DLMSClient.h
${DLMS_HEADER_DIR}/DLMSClock.h
${DLMS_HEADER_DIR}/DLMSCommunicationPortProtection.h
${DLMS_HEADER_DIR}/DLMSCompactData.h
${DLMS_HEADER_DIR}/DLMSConnectionEventArgs.h
${DLMS_HEADER_DIR}/DLMSContextType.h
${DLMS_HEADER_DIR}/DLMSConverter.h
${DLMS_HEADER_DIR}/DLMSCredit.h
${DLMS_HEADER_DIR}/DLMSData.h
${DLMS_HEADER_DIR}/DLMSDayProfile.h
${DLMS_HEADER_DIR}/DLMSDayProfileAction.h
${DLMS_HEADER_DIR}/DLMSDemandRegister.h
${DLMS_HEADER_DIR}/DLMSDisconnectControl.h
${DLMS_HEADER_DIR}/DLMSEmergencyProfile.h
${DLMS_HEADER_DIR}/DLMSExtendedRegister.h
${DLMS_HEADER_DIR}/DLMSGPRSSetup.h
${DLMS_HEADER_DIR}/DLMSGSMCellInfo.h
${DLMS_HEADER_DIR}/DLMSGSMDiagnostic.h
${DLMS_HEADER_DIR}/DLMSHdlcSetup.h
${DLMS_HEADER_DIR}/DLMSIec8802LlcType1Setup.h
${DLMS_HEADER_DIR}/DLMSIec8802LlcType2Setup.h
${DLMS_HEADER_DIR}/DLMSIec8802LlcType3Setup.h
${DLMS_HEADER_DIR}/DLMSIECOpticalPortSetup.h
${DLMS_HEADER_DIR}/DLMSIecTwistedPairSetup.h
${DLMS_HEADER_DIR}/DLMSImageActivateInfo.h
${DLMS_HEADER_DIR}/DLMSImageTransfer.h
${DLMS_HEADER_DIR}/DLMSIp4Setup.h
${DLMS_HEADER_DIR}/DLMSIp4SetupIpOption.h
${DLMS_HEADER_DIR}/DLMSIp6Setup.h
${DLMS_HEADER_DIR}/DLMSLimiter.h
${DLMS_HEADER_DIR}/DLMSLimits.h
${DLMS_HEADER_DIR}/DLMSLlcSscsSetup.h
${DLMS_HEADER_DIR}/DLMSLNCommandHandler.h
${DLMS_HEADER_DIR}/DLMSLNParameters.h
${DLMS_HEADER_DIR}/DLMSLongTransaction.h
${DLMS_HEADER_DIR}/DLMSMacAddressSetup.h
${DLMS_HEADER_DIR}/DLMSMd5.h
${DLMS_HEADER_DIR}/DLMSMessageHandler.h
${DLMS_HEADER_DIR}/DLMSModemConfiguration.h
${DLMS_HEADER_DIR}/DLMSModemInitialisation.h
${DLMS_HEADER_DIR}/DLMSMonitoredValue.h
${DLMS_HEADER_DIR}/DLMSNotify.h
${DLMS_HEADER_DIR}/DLMSNtpSetup.h
${DLMS_HEADER_DIR}/DLMSObject.h
${DLMS_HEADER_DIR}/DLMSObjectCollection.h
${DLMS_HEADER_DIR}/DLMSObjectDefinition.h
${DLMS_HEADER_DIR}/DLMSObjectFactory.h
${DLMS_HEADER_DIR}/DLMSParameterMonitor.h
${DLMS_HEADER_DIR}/DLMSPlcMeterInfo.h
${DLMS_HEADER_DIR}/DLMSPlcRegister.h
${DLMS_HEADER_DIR}/DLMSPppSetup.h
${DLMS_HEADER_DIR}/DLMSPppSetupIPCPOption.h
${DLMS_HEADER_DIR}/DLMSPppSetupLcpOption.h
${DLMS_HEADER_DIR}/DLMSPrimeNbOfdmPlcApplicationsIdentification.h
${DLMS_HEADER_DIR}/DLMSPrimeNbOfdmPlcMacCounters.h
${DLMS_HEADER_DIR}/DLMSPrimeNbOfdmPlcMacFunctionalParameters.h
${DLMS_HEADER_DIR}/DLMSPrimeNbOfdmPlcMacNetworkAdministrationData.h
${DLMS_HEADER_DIR}/DLMSPrimeNbOfdmPlcMacSetup.h
${DLMS_HEADER_DIR}/DLMSPrimeNbOfdmPlcPhysicalLayerCounters.h
${DLMS_HEADER_DIR}/DLMSProfileGeneric.h
${DLMS_HEADER_DIR}/DLMSQualityOfService.h
${DLMS_HEADER_DIR}/DLMSRegister.h
${DLMS_HEADER_DIR}/DLMSRegisterActivation.h
${DLMS_HEADER_DIR}/DLMSRegisterMonitor.h
${DLMS_HEADER_DIR}/DLMSSapAssignment.h
${DLMS_HEADER_DIR}/DLMSSchedule.h
${DLMS_HEADER_DIR}/DLMSScheduleEntry.h
${DLMS_HEADER_DIR}/DLMSScript.h
${DLMS_HEADER_DIR}/DLMSScriptAction.h
${DLMS_HEADER_DIR}/DLMSScriptTable.h
${DLMS_HEADER_DIR}/DLMSSeasonProfile.h
${DLMS_HEADER_DIR}/DLMSSecureClient.h
${DLMS_HEADER_DIR}/DLMSSecureServer.h
${DLMS_HEADER_DIR}/DLMSSecuritySetup.h
${DLMS_HEADER_DIR}/DLMSServer.h
${DLMS_HEADER_DIR}/DLMSSettings.h
${DLMS_HEADER_DIR}/DLMSSFSKActiveInitiator.h
${DLMS_HEADER_DIR}/DLMSSFSKMacCounters.h
${DLMS_HEADER_DIR}/DLMSSFSKMacSynchronizationTimeouts.h
${DLMS_HEADER_DIR}/DLMSSFSKPhyMacSetUp.h
${DLMS_HEADER_DIR}/DLMSSFSKReportingSystemList.h
${DLMS_HEADER_DIR}/DLMSSha1.h
${DLMS_HEADER_DIR}/DLMSSha256.h
${DLMS_HEADER_DIR}/DLMSSNCommandHandler.h
${DLMS_HEADER_DIR}/DLMSSNParameters.h
${DLMS_HEADER_DIR}/DLMSSpecialDay.h
${DLMS_HEADER_DIR}/DLMSSpecialDaysTable.h
${DLMS_HEADER_DIR}/DLMSTarget.h
${DLMS_HEADER_DIR}/DLMSTcpUdpSetup.h
${DLMS_HEADER_DIR}/DLMSTokenGateway.h
${DLMS_HEADER_DIR}/DLMSTranslator.h
${DLMS_HEADER_DIR}/DLMSTranslatorStructure.h
${DLMS_HEADER_DIR}/DLMSUtilityTables.h
${DLMS_HEADER_DIR}/DLMSValueEventArg.h
${DLMS_HEADER_DIR}/DLMSValueEventCollection.h
${DLMS_HEADER_DIR}/DLMSVariant.h
${DLMS_HEADER_DIR}/DLMSWeekProfile.h
${DLMS_HEADER_DIR}/Enums.h
${DLMS_HEADER_DIR}/ErrorCodes.h
${DLMS_HEADER_DIR}/HdlcSettings.h
${DLMS_HEADER_DIR}/Helpers.h
${DLMS_HEADER_DIR}/Ignore.h
${DLMS_HEADER_DIR}/MacAvailableSwitch.h
${DLMS_HEADER_DIR}/MacDirectTable.h
${DLMS_HEADER_DIR}/MacMulticastEntry.h
${DLMS_HEADER_DIR}/MacPhyCommunication.h
${DLMS_HEADER_DIR}/MBusClientData.h
${DLMS_HEADER_DIR}/NeighborDiscoverySetup.h
${DLMS_HEADER_DIR}/PlcSettings.h
${DLMS_HEADER_DIR}/ReplyData.h
${DLMS_HEADER_DIR}/Secure.h
${DLMS_HEADER_DIR}/SerialNumberCounter.h
${DLMS_HEADER_DIR}/ServerReply.h
${DLMS_HEADER_DIR}/SNInfo.h
${DLMS_HEADER_DIR}/StandardObisCode.h
${DLMS_HEADER_DIR}/StandardObisCodeCollection.h
${DLMS_HEADER_DIR}/DLMSTime.h
${DLMS_HEADER_DIR}/TokenGatewayConfiguration.h
${DLMS_HEADER_DIR}/UnitCharge.h
${DLMS_HEADER_DIR}/XmlReader.h
${DLMS_HEADER_DIR}/XmlWriter.h
${DLMS_HEADER_DIR}/XmlWriterSettings.h
${DLMS_HEADER_DIR}/IDLMSBase.h
${DLMS_HEADER_DIR}/OBiscodes.h
${DLMS_HEADER_DIR}/TranslatorGeneralTags.h
${DLMS_HEADER_DIR}/TranslatorSimpleTags.h
${DLMS_HEADER_DIR}/TranslatorStandardTags.h
${DLMS_HEADER_DIR}/TranslatorTags.h
${DLMS_HEADER_DIR}/PlatformDefinitions.h
${DLMS_HEADER_DIR}/AES.h
${DLMS_HEADER_DIR}/AES-GCM.h
${DLMS_HEADER_DIR}/GCM.h
${DLMS_HEADER_DIR}/SHA256.h
${DLMS_HEADER_DIR}/DetectPlatform.h
)

set(QZXING_SOURCES
${QZXING_SOURCES}
${QZXING_SOURCE_DIR}/BarcodeFormat.cpp
${QZXING_SOURCE_DIR}/BinaryBitmap.cpp
${QZXING_SOURCE_DIR}/BitArray.cpp
${QZXING_SOURCE_DIR}/BitMatrix.cpp
${QZXING_SOURCE_DIR}/BitMatrixIO.cpp
${QZXING_SOURCE_DIR}/BitSource.cpp
${QZXING_SOURCE_DIR}/CharacterSet.cpp
${QZXING_SOURCE_DIR}/ConcentricFinder.cpp
${QZXING_SOURCE_DIR}/Content.cpp
${QZXING_SOURCE_DIR}/DecodeHints.cpp
${QZXING_SOURCE_DIR}/ECI.cpp
${QZXING_SOURCE_DIR}/GenericGF.cpp
${QZXING_SOURCE_DIR}/GenericGFPoly.cpp
${QZXING_SOURCE_DIR}/GlobalHistogramBinarizer.cpp
${QZXING_SOURCE_DIR}/GridSampler.cpp
${QZXING_SOURCE_DIR}/GTIN.cpp
${QZXING_SOURCE_DIR}/HRI.cpp
${QZXING_SOURCE_DIR}/HybridBinarizer.cpp
${QZXING_SOURCE_DIR}/MultiFormatReader.cpp
${QZXING_SOURCE_DIR}/MultiFormatWriter.cpp
${QZXING_SOURCE_DIR}/PerspectiveTransform.cpp
${QZXING_SOURCE_DIR}/QRBitMatrixParser.cpp
${QZXING_SOURCE_DIR}/QRCodecMode.cpp
${QZXING_SOURCE_DIR}/QRDataBlock.cpp
${QZXING_SOURCE_DIR}/QRDecoder.cpp
${QZXING_SOURCE_DIR}/QRDetector.cpp
${QZXING_SOURCE_DIR}/QREncoder.cpp
${QZXING_SOURCE_DIR}/QRErrorCorrectionLevel.cpp
${QZXING_SOURCE_DIR}/QRFormatInformation.cpp
${QZXING_SOURCE_DIR}/QRMaskUtil.cpp
${QZXING_SOURCE_DIR}/QRMatrixUtil.cpp
${QZXING_SOURCE_DIR}/QRReader.cpp
${QZXING_SOURCE_DIR}/QRVersion.cpp
${QZXING_SOURCE_DIR}/QRWriter.cpp
${QZXING_SOURCE_DIR}/ReadBarcode.cpp
${QZXING_SOURCE_DIR}/ReedSolomonDecoder.cpp
${QZXING_SOURCE_DIR}/ReedSolomonEncoder.cpp
${QZXING_SOURCE_DIR}/Result.cpp
${QZXING_SOURCE_DIR}/ResultPoint.cpp
${QZXING_SOURCE_DIR}/TextDecoder.cpp
${QZXING_SOURCE_DIR}/TextEncoder.cpp
${QZXING_SOURCE_DIR}/TextUtfEncoding.cpp
${QZXING_SOURCE_DIR}/Utf.cpp
${QZXING_SOURCE_DIR}/WhiteRectDetector.cpp
${QZXING_SOURCE_DIR}/zueci.cpp
${QZXING_SOURCE_DIR}/ZXBigInteger.cpp
)

set(QZXING_HEADERS
${QZXING_HEADERS}
${QZXING_HEADER_DIR}/BarcodeFormat.h
${QZXING_HEADER_DIR}/BinaryBitmap.h
${QZXING_HEADER_DIR}/BitArray.h
${QZXING_HEADER_DIR}/BitHacks.h
${QZXING_HEADER_DIR}/BitMatrix.h
${QZXING_HEADER_DIR}/BitMatrixCursor.h
${QZXING_HEADER_DIR}/BitMatrixIO.h
${QZXING_HEADER_DIR}/BitSource.h
${QZXING_HEADER_DIR}/ByteArray.h
${QZXING_HEADER_DIR}/ByteMatrix.h
${QZXING_HEADER_DIR}/CharacterSet.h
${QZXING_HEADER_DIR}/ConcentricFinder.h
${QZXING_HEADER_DIR}/Content.h
${QZXING_HEADER_DIR}/CustomData.h
${QZXING_HEADER_DIR}/DecodeHints.h
${QZXING_HEADER_DIR}/DecoderResult.h
${QZXING_HEADER_DIR}/DetectorResult.h
${QZXING_HEADER_DIR}/ECI.h
${QZXING_HEADER_DIR}/Error.h
${QZXING_HEADER_DIR}/Flags.h
${QZXING_HEADER_DIR}/Generator.h
${QZXING_HEADER_DIR}/GenericGF.h
${QZXING_HEADER_DIR}/GenericGFPoly.h
${QZXING_HEADER_DIR}/GlobalHistogramBinarizer.h
${QZXING_HEADER_DIR}/GridSampler.h
${QZXING_HEADER_DIR}/GTIN.h
${QZXING_HEADER_DIR}/HRI.h
${QZXING_HEADER_DIR}/HybridBinarizer.h
${QZXING_HEADER_DIR}/ImageView.h
${QZXING_HEADER_DIR}/LogMatrix.h
${QZXING_HEADER_DIR}/Matrix.h
${QZXING_HEADER_DIR}/MultiFormatReader.h
${QZXING_HEADER_DIR}/MultiFormatWriter.h
${QZXING_HEADER_DIR}/Pattern.h
${QZXING_HEADER_DIR}/PerspectiveTransform.h
${QZXING_HEADER_DIR}/Point.h
${QZXING_HEADER_DIR}/QRBitMatrixParser.h
${QZXING_HEADER_DIR}/QRCodecMode.h
${QZXING_HEADER_DIR}/QRDataBlock.h
${QZXING_HEADER_DIR}/QRDataMask.h
${QZXING_HEADER_DIR}/QRDecoder.h
${QZXING_HEADER_DIR}/QRDetector.h
${QZXING_HEADER_DIR}/QRECB.h
${QZXING_HEADER_DIR}/QREncoder.h
${QZXING_HEADER_DIR}/QREncodeResult.h
${QZXING_HEADER_DIR}/QRErrorCorrectionLevel.h
${QZXING_HEADER_DIR}/QRFormatInformation.h
${QZXING_HEADER_DIR}/QRMaskUtil.h
${QZXING_HEADER_DIR}/QRMatrixUtil.h
${QZXING_HEADER_DIR}/QRReader.h
${QZXING_HEADER_DIR}/QRVersion.h
${QZXING_HEADER_DIR}/QRWriter.h
${QZXING_HEADER_DIR}/Quadrilateral.h
${QZXING_HEADER_DIR}/Range.h
${QZXING_HEADER_DIR}/ReadBarcode.h
${QZXING_HEADER_DIR}/Reader.h
${QZXING_HEADER_DIR}/ReedSolomonDecoder.h
${QZXING_HEADER_DIR}/ReedSolomonEncoder.h
${QZXING_HEADER_DIR}/RegressionLine.h
${QZXING_HEADER_DIR}/Result.h
${QZXING_HEADER_DIR}/ResultPoint.h
${QZXING_HEADER_DIR}/Scope.h
${QZXING_HEADER_DIR}/StructuredAppend.h
${QZXING_HEADER_DIR}/TextDecoder.h
${QZXING_HEADER_DIR}/TextEncoder.h
${QZXING_HEADER_DIR}/TextUtfEncoding.h
${QZXING_HEADER_DIR}/ThresholdBinarizer.h
${QZXING_HEADER_DIR}/TritMatrix.h
${QZXING_HEADER_DIR}/Utf.h
${QZXING_HEADER_DIR}/WhiteRectDetector.h
${QZXING_HEADER_DIR}/zueci.h
${QZXING_HEADER_DIR}/zueci_big5.h
${QZXING_HEADER_DIR}/zueci_common.h
${QZXING_HEADER_DIR}/zueci_gb2312.h
${QZXING_HEADER_DIR}/zueci_gb18030.h
${QZXING_HEADER_DIR}/zueci_gbk.h
${QZXING_HEADER_DIR}/zueci_ksx1001.h
${QZXING_HEADER_DIR}/zueci_sb.h
${QZXING_HEADER_DIR}/zueci_sjis.h
${QZXING_HEADER_DIR}/ZXAlgorithms.h
${QZXING_HEADER_DIR}/ZXBigInteger.h
${QZXING_HEADER_DIR}/ZXConfig.h
${QZXING_HEADER_DIR}/ZXingQtReader.h
${QZXING_HEADER_DIR}/ZXNullable.h
${QZXING_HEADER_DIR}/ZXTestSupport.h
)

set(SOURCES
${SOURCES}
${SOURCE_DIR}/main.cpp
${SOURCE_DIR}/Logger.cpp
${SOURCE_DIR}/MeterReader.cpp
${SOURCE_DIR}/Help.cpp
${SOURCE_DIR}/Mk7Mi.cpp
${SOURCE_DIR}/Controller.cpp
${SOURCE_DIR}/ControllerComm.cpp
${SOURCE_DIR}/ControllerUserMgmt.cpp
${SOURCE_DIR}/ControllerAppMgmt.cpp
${SOURCE_DIR}/DlmsClient.cpp
${SOURCE_DIR}/DataInterface.cpp
${SOURCE_DIR}/DataEntity.cpp
${SOURCE_DIR}/EntityMeter.cpp
${SOURCE_DIR}/EntityMeterSettings.cpp
${SOURCE_DIR}/EntityAlarmFilter.cpp
${SOURCE_DIR}/EntityAlarm.cpp
${SOURCE_DIR}/EntityToken.cpp
${SOURCE_DIR}/EntityTokenHistory.cpp
${SOURCE_DIR}/EntityUserInfo.cpp
${SOURCE_DIR}/EntityStatisticsEnergy.cpp
${SOURCE_DIR}/IWorkFlow.cpp
${SOURCE_DIR}/WorkFlowAddMeter.cpp
${SOURCE_DIR}/WorkFlowConnectMeter.cpp
${SOURCE_DIR}/WorkFlowDisconnectMeter.cpp
${SOURCE_DIR}/WorkFlowTokenTransfer.cpp
${SOURCE_DIR}/WorkFlowKeepAlive.cpp
${SOURCE_DIR}/BluetoothInterface.cpp
${SOURCE_DIR}/CameraInterface.cpp
)

set(HEADERS
${HEADERS}
${HEADER_DIR}/Logger.hpp
${HEADER_DIR}/MeterReader.hpp
${HEADER_DIR}/Help.hpp
${HEADER_DIR}/GraphData.hpp
${HEADER_DIR}/Mk7Mi.hpp
${HEADER_DIR}/ApplicationTheme.hpp
${HEADER_DIR}/Controller.hpp
${HEADER_DIR}/DlmsClient.hpp
${HEADER_DIR}/DataInterface.hpp
${HEADER_DIR}/DataEntity.hpp
${HEADER_DIR}/EntityMeter.hpp
${HEADER_DIR}/EntityMeterSettings.hpp
${HEADER_DIR}/EntityAlarmFilter.hpp
${HEADER_DIR}/EntityAlarm.hpp
${HEADER_DIR}/EntityToken.hpp
${HEADER_DIR}/EntityTokenHistory.hpp
${HEADER_DIR}/EntityUserInfo.hpp
${HEADER_DIR}/EntityStatisticsEnergy.hpp
${HEADER_DIR}/IWorkFlow.hpp
${HEADER_DIR}/WorkFlowAddMeter.hpp
${HEADER_DIR}/WorkFlowConnectMeter.hpp
${HEADER_DIR}/WorkFlowDisconnectMeter.hpp
${HEADER_DIR}/WorkFlowTokenTransfer.hpp
${HEADER_DIR}/WorkFlowKeepAlive.hpp
${HEADER_DIR}/BluetoothInterface.hpp
${HEADER_DIR}/CameraInterface.hpp
${HEADER_DIR}/Version.hpp
)

set(RESOURCE_FILE ./resources/MeterReader.qrc)

qt_add_executable(MeterReader ${DLMS_SOURCES} ${DLMS_HEADERS} ${SOURCES} ${HEADERS} ${QZXING_SOURCES} ${QZXING_HEADERS} ${RESOURCE_FILE})

set_target_properties(MeterReader PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER MeterReader
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_FRAMEWORK_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/darwin/Info.plist
    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/darwin/Info.plist
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android
)

target_link_libraries(MeterReader
    PRIVATE Qt6::Quick
    PRIVATE Qt6::Core
    PRIVATE Qt6::CorePrivate
    PRIVATE Qt6::Sql
    PRIVATE Qt6::Qml
    PRIVATE Qt6::Multimedia
    PRIVATE Qt6::Bluetooth
)

install(TARGETS MeterReader
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
