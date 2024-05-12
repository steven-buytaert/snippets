// Copyright 2024 Steven Buytaert

// Generated codec tables from hci-spec2src-5.4.h

#include <codec-int.h>

#define INFO(SECTION, OFFSET)

static const CoI_t CoI_0[198] = {
  INFO(    7.7.14 ,   0) { .num =  6 }, 
  INFO(    7.7.15 ,   1) { .num =  5, .skip = 1 }, { .num =  2 }, 
  INFO(     7.7.3 ,   3) { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, { .num = 10 }, 
  INFO(    7.7.28 ,   6) { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, { .num =  4 }, 
  INFO(     7.7.4 ,   9) { .num = 13 }, 
  INFO(    7.7.69 ,  10) { .num = 10, .skip = 2 }, { .num = 11 }, { .action = inlinefix, .inst = 1 }, 
  INFO(     7.7.5 ,  13) { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, 
  INFO(    7.7.17 ,  16) { .num =  3, .skip = 1 }, { .num =  2 }, 
  INFO(    7.7.38 ,  18) { .num = 15, .skip = 1 }, { .action = largecopy, .arg = 1, .inst = 1 }, { .LSB = 0xf3 }, { .MSB = 0x00 }, 
  INFO(    7.7.32 ,  22) { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, { .num =  5, .skip = 1 }, { .action = largecopy, .inst = 1 }, { .LSB = 0x10 }, { .MSB = 0x00 }, 
  INFO( 7.7.65.36 ,  28) { .num =  7 }, 
  INFO(     7.7.2 ,  29) { .num =  4 }, { .action = loadloop, .inst = 1 }, { .num =  7, .skip = 1 }, { .num =  5, .skip = 1 }, { .num =  2 }, { .action = endloop, .inst = 1 }, 
  INFO(    7.7.33 ,  35) { .num =  4 }, { .action = loadloop, .inst = 1 }, { .num = 11, .skip = 1 }, { .num =  3, .skip = 1 }, { .action = endloop, .inst = 1 }, 
  INFO(    7.7.22 ,  40) { .num =  9 }, 
  INFO( 7.7.65.19 ,  41) { .num = 12 }, 
  INFO(  7.7.65.2 ,  42) { .num =  5, .skip = 3 }, { .action = loadloop, .inst = 1 }, { .num =  9, .skip = 3 }, { .action = inlinefix, .arg = 1, .inst = 1 }, { .num =  1, .skip = 3 }, { .action = endloop, .inst = 1 }, 
  INFO( 7.7.65.18 ,  48) { .num =  9, .skip = 1 }, 
  INFO( 7.7.65.29 ,  49) { .num = 13, .skip = 1 }, { .num =  5, .skip = 1 }, { .action = inlinefix, .inst = 1 }, { .num =  2 }, 
  INFO( 7.7.65.16 ,  53) { .num =  6 }, 
  INFO( 7.7.65.34 ,  54) { .num = 13, .skip = 1 }, { .num =  5, .skip = 1 }, { .num =  5, .skip = 1 }, 
  INFO( 7.7.65.25 ,  57) { .num =  5, .skip = 1 }, { .action = largecopy, .arg = 1, .inst = 1 }, { .LSB = 0x15 }, { .MSB = 0x00 }, { .num =  6 }, 
  INFO(  7.7.65.1 ,  62) { .num =  5, .skip = 1 }, { .action = largecopy, .arg = 1, .inst = 1 }, { .LSB = 0x11 }, { .MSB = 0x00 }, 
  INFO( 7.7.65.22 ,  66) { .num =  1 }, { .action = loadloop, .inst = 1 }, { .num =  2 }, { .action = endloop, .inst = 1 }, 
  INFO(  7.7.65.3 ,  70) { .num =  5, .skip = 1 }, { .num =  8 }, 
  INFO( 7.7.65.21 ,  72) { .num =  7, .skip = 1 }, { .num =  9 }, { .action = loadloop, .inst = 1 }, { .num =  2 }, { .action = endloop, .inst = 1 }, 
  INFO( 7.7.65.27 ,  77) { .action = largecopy, .arg = 1, .inst = 1 }, { .LSB = 0x11 }, { .MSB = 0x00 }, { .num =  5, .skip = 1 }, { .action = inlinefix, .inst = 1 }, { .num =  2 }, 
  INFO(  7.7.65.6 ,  83) { .num = 14 }, 
  INFO( 7.7.65.11 ,  84) { .num =  5 }, { .action = loadloop, .inst = 1 }, { .action = largecopy, .inst = 1 }, { .LSB = 0x10 }, { .MSB = 0x00 }, { .action = endloop, .inst = 1 }, 
  INFO( 7.7.65.10 ,  90) { .num =  5, .skip = 1 }, { .action = largecopy, .inst = 1 }, { .LSB = 0x20 }, { .MSB = 0x00 }, 
  INFO( 7.7.65.10 ,  94) { .num =  5, .skip = 1 }, { .action = largecopy, .arg = 1, .inst = 1 }, { .LSB = 0x1d }, { .MSB = 0x00 }, 
  INFO( 7.7.65.13 ,  98) { .num =  5, .skip = 3 }, { .action = loadloop, .inst = 1 }, { .num =  8 }, { .action = inlinefix, .arg = 1, .inst = 1 }, { .action = endloop, .inst = 1 }, 
  INFO(  7.7.65.9 , 103) { .action = largecopy, .inst = 1 }, { .LSB = 0x25 }, { .MSB = 0x00 }, 
  INFO( 7.7.65.15 , 106) { .num =  9, .skip = 1 }, { .num =  5 }, { .action = inlinefix, .inst = 1 }, 
  INFO( 7.7.65.37 , 109) { .num =  8 }, { .action = loadloop, .inst = 1 }, { .num =  6, .skip = 2 }, { .action = inlinefix, .arg = 1, .inst = 1 }, { .action = endloop, .inst = 1 }, 
  INFO( 7.7.65.14 , 114) { .num =  5, .skip = 1 }, { .num = 11, .skip = 1 }, { .num =  7, .skip = 1 }, 
  INFO( 7.7.65.14 , 117) { .num =  5, .skip = 1 }, { .num = 11, .skip = 1 }, { .num =  3, .skip = 1 }, 
  INFO( 7.7.65.24 , 120) { .num =  5, .skip = 1 }, { .num = 15, .skip = 1 }, { .num =  7, .skip = 1 }, 
  INFO( 7.7.65.24 , 123) { .num =  5, .skip = 1 }, { .num = 15, .skip = 1 }, { .num =  3, .skip = 1 }, 
  INFO( 7.7.65.12 , 126) { .num =  5, .skip = 1 }, { .num =  4 }, 
  INFO(  7.7.65.8 , 128) { .action = largecopy, .inst = 1 }, { .LSB = 0x45 }, { .MSB = 0x00 }, 
  INFO(  7.7.65.4 , 131) { .num =  5, .skip = 1 }, { .num = 10 }, 
  INFO( 7.7.65.31 , 133) { .num =  5, .skip = 1 }, { .num =  3, .skip = 1 }, 
  INFO( 7.7.65.33 , 135) { .num =  5, .skip = 1 }, { .num =  7, .skip = 1 }, 
  INFO(    7.7.24 , 137) { .action = largecopy, .inst = 1 }, { .LSB = 0x1a }, { .MSB = 0x00 }, 
  INFO(    7.7.46 , 140) { .num =  3, .skip = 1 }, { .num =  4 }, 
  INFO(    7.7.25 , 142) { .num =  3 }, 
  INFO(    7.7.20 , 143) { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  2 }, 
  INFO(    7.7.59 , 147) { .num =  3, .skip = 1 }, { .num =  3, .skip = 1 }, { .action = loadloop, .inst = 1 }, { .num =  6 }, { .action = endloop, .inst = 1 }, 
  INFO(    7.7.19 , 152) { .num =  4 }, { .action = loadloop, .inst = 1 }, { .num =  4 }, { .action = endloop, .inst = 1 }, 
  INFO(    7.7.13 , 156) { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, { .num =  4, .skip = 2 }, { .action = largecopy, .inst = 1 }, { .LSB = 0x10 }, { .MSB = 0x00 }, 
  INFO(    7.7.34 , 162) { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, { .num = 12 }, 
  INFO(    7.7.12 , 165) { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  4 }, 
  INFO(    7.7.50 , 169) { .action = largecopy, .inst = 1 }, { .LSB = 0x11 }, { .MSB = 0x00 }, 
  INFO(     7.7.7 , 172) { .action = largecopy, .inst = 1 }, { .LSB = 0x01 }, { .MSB = 0x01 }, 
  INFO(    7.7.21 , 175) { .num =  4 }, { .action = loadloop, .inst = 1 }, { .action = largecopy, .inst = 1 }, { .LSB = 0x16 }, { .MSB = 0x00 }, { .action = endloop, .inst = 1 }, 
  INFO(    7.7.76 , 181) { .num =  3, .skip = 1 }, { .num =  8 }, 
  INFO(    7.7.68 , 183) { .num =  9, .skip = 2 }, { .num = 15, .skip = 1 }, { .num =  7, .skip = 1 }, 
  INFO(    7.7.36 , 186) { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, { .num =  8 }, 
  INFO(    7.7.35 , 189) { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, { .num = 11, .skip = 1 }, { .num =  5, .skip = 1 }, 
  INFO(    7.7.66 , 193) { .num =  3, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  6, .skip = 2 }, 
  INFO(    7.7.42 , 196) { .num =  9, .skip = 3 }, { .num =  4 }, 
};

static const TE_t TE_0[95] = {
  { .code = 0x01, .issub = 1, .coistart =  62, .numcoi = 4 }, //   0   7.7.65.1   HCI_LE_Connection_Complete
  { .code = 0x02, .issub = 0, .coistart =  29, .numcoi = 6 }, //   1   7.7.2      HCI_Inquiry_Result
  { .code = 0x02, .issub = 1, .coistart =  42, .numcoi = 6 }, //   2   7.7.65.2   HCI_LE_Advertising_Report
  { .code = 0x03, .issub = 0, .coistart =   3, .numcoi = 3 }, //   3   7.7.3      HCI_Connection_Complete
  { .code = 0x03, .issub = 1, .coistart =  70, .numcoi = 2 }, //   4   7.7.65.3   HCI_LE_Connection_Update_Complete
  { .code = 0x04, .issub = 0, .coistart =   9, .numcoi = 1 }, //   5   7.7.4      HCI_Connection_Request
  { .code = 0x04, .issub = 1, .coistart = 131, .numcoi = 2 }, //   6   7.7.65.4   HCI_LE_Read_Remote_Features_Complete
  { .code = 0x05, .issub = 0, .coistart =  13, .numcoi = 3 }, //   7   7.7.5      HCI_Disconnection_Complete
  { .code = 0x05, .issub = 1, .coistart =  25, .numcoi = 3 }, //   8   7.7.65.5   HCI_LE_Long_Term_Key_Request
  { .code = 0x06, .issub = 1, .coistart =  83, .numcoi = 1 }, //   9   7.7.65.6   HCI_LE_Remote_Connection_Parameter_Request
  { .code = 0x07, .issub = 1, .coistart =  83, .numcoi = 1 }, //  10   7.7.65.7   HCI_LE_Data_Length_Change
  { .code = 0x07, .issub = 0, .coistart = 172, .numcoi = 3 }, //  11   7.7.7      HCI_Remote_Name_Request_Complete
  { .code = 0x08, .issub = 0, .coistart =  13, .numcoi = 3 }, //  12   7.7.8      HCI_Encryption_Change [v1]
  { .code = 0x08, .issub = 1, .coistart = 128, .numcoi = 3 }, //  13   7.7.65.8   HCI_LE_Read_Local_P-256_Public_Key_Complete
  { .code = 0x09, .issub = 1, .coistart = 103, .numcoi = 3 }, //  14   7.7.65.9   HCI_LE_Generate_DHKey_Complete
  { .code = 0x0a, .issub = 1, .coistart =  94, .numcoi = 4 }, //  15   7.7.65.10  HCI_LE_Enhanced_Connection_Complete [v1]
  { .code = 0x0a, .issub = 0, .coistart =  13, .numcoi = 3 }, //  16   7.7.10     HCI_Link_Key_Type_Changed
  { .code = 0x0b, .issub = 1, .coistart =  84, .numcoi = 6 }, //  17   7.7.65.11  HCI_LE_Directed_Advertising_Report
  { .code = 0x0b, .issub = 0, .coistart =   3, .numcoi = 3 }, //  18   7.7.11     HCI_Read_Remote_Supported_Features_Complete
  { .code = 0x0c, .issub = 1, .coistart = 126, .numcoi = 2 }, //  19   7.7.65.12  HCI_LE_PHY_Update_Complete
  { .code = 0x0c, .issub = 0, .coistart = 165, .numcoi = 4 }, //  20   7.7.12     HCI_Read_Remote_Version_Information_Complete
  { .code = 0x0d, .issub = 1, .coistart =  98, .numcoi = 5 }, //  21   7.7.65.13  HCI_LE_Extended_Advertising_Report
  { .code = 0x0d, .issub = 0, .coistart = 156, .numcoi = 6 }, //  22   7.7.13     HCI_QoS_Setup_Complete
  { .code = 0x0e, .issub = 0, .coistart =   0, .numcoi = 1 }, //  23   7.7.14     HCI_Command_Complete
  { .code = 0x0e, .issub = 1, .coistart = 117, .numcoi = 3 }, //  24   7.7.65.14  HCI_LE_Periodic_Advertising_Sync_Established [v1]
  { .code = 0x0f, .issub = 0, .coistart =   1, .numcoi = 2 }, //  25   7.7.15     HCI_Command_Status
  { .code = 0x0f, .issub = 1, .coistart =  11, .numcoi = 2 }, //  26   7.7.65.15  HCI_LE_Periodic_Advertising_Report [v1]
  { .code = 0x10, .issub = 0, .coistart =   8, .numcoi = 1 }, //  27   7.7.16     HCI_Hardware_Error
  { .code = 0x10, .issub = 1, .coistart =  53, .numcoi = 1 }, //  28   7.7.65.16  HCI_LE_Periodic_Advertising_Sync_Lost
  { .code = 0x11, .issub = 0, .coistart =  16, .numcoi = 2 }, //  29   7.7.17     HCI_Flush_Occurred
  { .code = 0x11, .issub = 1, .coistart =   8, .numcoi = 1 }, //  30   7.7.65.17  HCI_LE_Scan_Timeout
  { .code = 0x12, .issub = 1, .coistart =  48, .numcoi = 1 }, //  31   7.7.65.18  HCI_LE_Advertising_Set_Terminated
  { .code = 0x12, .issub = 0, .coistart =   5, .numcoi = 1 }, //  32   7.7.18     HCI_Role_Change
  { .code = 0x13, .issub = 1, .coistart =  41, .numcoi = 1 }, //  33   7.7.65.19  HCI_LE_Scan_Request_Received
  { .code = 0x13, .issub = 0, .coistart = 152, .numcoi = 4 }, //  34   7.7.19     HCI_Number_Of_Completed_Packets
  { .code = 0x14, .issub = 1, .coistart =  31, .numcoi = 1 }, //  35   7.7.65.20  HCI_LE_Channel_Selection_Algorithm
  { .code = 0x14, .issub = 0, .coistart = 143, .numcoi = 4 }, //  36   7.7.20     HCI_Mode_Change
  { .code = 0x15, .issub = 1, .coistart =  72, .numcoi = 5 }, //  37   7.7.65.21  HCI_LE_Connectionless_IQ_Report
  { .code = 0x15, .issub = 0, .coistart = 175, .numcoi = 6 }, //  38   7.7.21     HCI_Return_Link_Keys
  { .code = 0x16, .issub = 1, .coistart =  66, .numcoi = 4 }, //  39   7.7.65.22  HCI_LE_Connection_IQ_Report
  { .code = 0x16, .issub = 0, .coistart =  40, .numcoi = 1 }, //  40   7.7.22     HCI_PIN_Code_Request
  { .code = 0x17, .issub = 1, .coistart =   1, .numcoi = 2 }, //  41   7.7.65.23  HCI_LE_CTE_Request_Failed
  { .code = 0x17, .issub = 0, .coistart =  40, .numcoi = 1 }, //  42   7.7.23     HCI_Link_Key_Request
  { .code = 0x18, .issub = 1, .coistart = 123, .numcoi = 3 }, //  43   7.7.65.24  HCI_LE_Periodic_Advertising_Sync_Transfer_Received [v1]
  { .code = 0x18, .issub = 0, .coistart = 137, .numcoi = 3 }, //  44   7.7.24     HCI_Link_Key_Notification
  { .code = 0x19, .issub = 1, .coistart =  57, .numcoi = 5 }, //  45   7.7.65.25  HCI_LE_CIS_Established
  { .code = 0x19, .issub = 0, .coistart = 142, .numcoi = 1 }, //  46   7.7.25     HCI_Loopback_Command
  { .code = 0x1a, .issub = 0, .coistart =   8, .numcoi = 1 }, //  47   7.7.26     HCI_Data_Buffer_Overflow
  { .code = 0x1a, .issub = 1, .coistart =   5, .numcoi = 1 }, //  48   7.7.65.26  HCI_LE_CIS_Request
  { .code = 0x1b, .issub = 1, .coistart =  77, .numcoi = 6 }, //  49   7.7.65.27  HCI_LE_Create_BIG_Complete
  { .code = 0x1b, .issub = 0, .coistart =  15, .numcoi = 2 }, //  50   7.7.27     HCI_Max_Slots_Change
  { .code = 0x1c, .issub = 1, .coistart =  53, .numcoi = 1 }, //  51   7.7.65.28  HCI_LE_Terminate_BIG_Complete
  { .code = 0x1c, .issub = 0, .coistart =   6, .numcoi = 3 }, //  52   7.7.28     HCI_Read_Clock_Offset_Complete
  { .code = 0x1d, .issub = 0, .coistart =   6, .numcoi = 3 }, //  53   7.7.29     HCI_Connection_Packet_Type_Changed
  { .code = 0x1d, .issub = 1, .coistart =  49, .numcoi = 4 }, //  54   7.7.65.29  HCI_LE_BIG_Sync_Established
  { .code = 0x1e, .issub = 1, .coistart =  53, .numcoi = 1 }, //  55   7.7.65.30  HCI_LE_BIG_Sync_Lost
  { .code = 0x1e, .issub = 0, .coistart =  16, .numcoi = 2 }, //  56   7.7.30     HCI_QoS_Violation
  { .code = 0x1f, .issub = 1, .coistart = 133, .numcoi = 2 }, //  57   7.7.65.31  HCI_LE_Request_Peer_SCA_Complete
  { .code = 0x20, .issub = 1, .coistart =  71, .numcoi = 1 }, //  58   7.7.65.32  HCI_LE_Path_Loss_Threshold
  { .code = 0x20, .issub = 0, .coistart =   5, .numcoi = 1 }, //  59   7.7.31     HCI_Page_Scan_Repetition_Mode_Change
  { .code = 0x21, .issub = 0, .coistart =  22, .numcoi = 6 }, //  60   7.7.32     HCI_Flow_Specification_Complete
  { .code = 0x21, .issub = 1, .coistart = 135, .numcoi = 2 }, //  61   7.7.65.33  HCI_LE_Transmit_Power_Reporting
  { .code = 0x22, .issub = 0, .coistart =  35, .numcoi = 5 }, //  62   7.7.33     HCI_Inquiry_Result_with_RSSI
  { .code = 0x22, .issub = 1, .coistart =  54, .numcoi = 3 }, //  63   7.7.65.34  HCI_LE_BIGInfo_Advertising_Report
  { .code = 0x23, .issub = 1, .coistart = 131, .numcoi = 2 }, //  64   7.7.65.35  HCI_LE_Subrate_Change
  { .code = 0x23, .issub = 0, .coistart = 162, .numcoi = 3 }, //  65   7.7.34     HCI_Read_Remote_Extended_Features_Complete
  { .code = 0x24, .issub = 1, .coistart = 114, .numcoi = 3 }, //  66   7.7.65.14  HCI_LE_Periodic_Advertising_Sync_Established [v2]
  { .code = 0x25, .issub = 1, .coistart = 106, .numcoi = 3 }, //  67   7.7.65.15  HCI_LE_Periodic_Advertising_Report [v2]
  { .code = 0x26, .issub = 1, .coistart = 120, .numcoi = 3 }, //  68   7.7.65.24  HCI_LE_Periodic_Advertising_Sync_Transfer_Received [v2]
  { .code = 0x27, .issub = 1, .coistart =  28, .numcoi = 1 }, //  69   7.7.65.36  HCI_LE_Periodic_Advertising_Subevent_Data_Request
  { .code = 0x28, .issub = 1, .coistart = 109, .numcoi = 5 }, //  70   7.7.65.37  HCI_LE_Periodic_Advertising_Response_Report
  { .code = 0x29, .issub = 1, .coistart =  90, .numcoi = 4 }, //  71   7.7.65.10  HCI_LE_Enhanced_Connection_Complete [v2]
  { .code = 0x2c, .issub = 0, .coistart = 189, .numcoi = 4 }, //  72   7.7.35     HCI_Synchronous_Connection_Complete
  { .code = 0x2d, .issub = 0, .coistart = 186, .numcoi = 3 }, //  73   7.7.36     HCI_Synchronous_Connection_Changed
  { .code = 0x2e, .issub = 0, .coistart =   3, .numcoi = 3 }, //  74   7.7.37     HCI_Sniff_Subrating
  { .code = 0x2f, .issub = 0, .coistart =  18, .numcoi = 4 }, //  75   7.7.38     HCI_Extended_Inquiry_Result
  { .code = 0x31, .issub = 0, .coistart =  40, .numcoi = 1 }, //  76   7.7.40     HCI_IO_Capability_Request
  { .code = 0x32, .issub = 0, .coistart =  41, .numcoi = 1 }, //  77   7.7.41     HCI_IO_Capability_Response
  { .code = 0x33, .issub = 0, .coistart = 196, .numcoi = 2 }, //  78   7.7.42     HCI_User_Confirmation_Request
  { .code = 0x34, .issub = 0, .coistart =  40, .numcoi = 1 }, //  79   7.7.43     HCI_User_Passkey_Request
  { .code = 0x35, .issub = 0, .coistart =  40, .numcoi = 1 }, //  80   7.7.44     HCI_Remote_OOB_Data_Request
  { .code = 0x38, .issub = 0, .coistart = 140, .numcoi = 2 }, //  81   7.7.46     HCI_Link_Supervision_Timeout_Changed
  { .code = 0x39, .issub = 0, .coistart =  16, .numcoi = 2 }, //  82   7.7.47     HCI_Enhanced_Flush_Complete
  { .code = 0x3b, .issub = 0, .coistart = 196, .numcoi = 2 }, //  83   7.7.48     HCI_User_Passkey_Notification
  { .code = 0x3c, .issub = 0, .coistart =   5, .numcoi = 1 }, //  84   7.7.49     HCI_Keypress_Notification
  { .code = 0x3d, .issub = 0, .coistart = 169, .numcoi = 3 }, //  85   7.7.50     HCI_Remote_Host_Supported_Features_Notification
  { .code = 0x48, .issub = 0, .coistart = 147, .numcoi = 5 }, //  86   7.7.59     HCI_Number_Of_Completed_Data_Blocks
  { .code = 0x4e, .issub = 0, .coistart = 193, .numcoi = 3 }, //  87   7.7.66     HCI_Triggered_Clock_Capture
  { .code = 0x50, .issub = 0, .coistart = 183, .numcoi = 3 }, //  88   7.7.68     HCI_Synchronization_Train_Received
  { .code = 0x51, .issub = 0, .coistart =  10, .numcoi = 3 }, //  89   7.7.69     HCI_Connectionless_Peripheral_Broadcast_Receive
  { .code = 0x52, .issub = 0, .coistart =   5, .numcoi = 1 }, //  90   7.7.70     HCI_Connectionless_Peripheral_Broadcast_Timeout
  { .code = 0x55, .issub = 0, .coistart =   9, .numcoi = 1 }, //  91   7.7.73     HCI_Connectionless_Peripheral_Broadcast_Channel_Map_Change
  { .code = 0x56, .issub = 0, .coistart =  28, .numcoi = 1 }, //  92   7.7.74     HCI_Inquiry_Response_Notification
  { .code = 0x58, .issub = 0, .coistart = 181, .numcoi = 2 }, //  93   7.7.76     HCI_SAM_Status_Change
  { .code = 0x59, .issub = 0, .coistart =   6, .numcoi = 3 }, //  94   7.7.8      HCI_Encryption_Change [v2]
};

static const CodecTab_t CodecTab_0 = {
  .numtab = 95, .numcoi = 198, .table = TE_0, .CoI = CoI_0
};

static const CoI_t CoI_1[98] = {
  INFO(     7.1.8 ,   0) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  7, .skip = 1 }, 
  INFO(    7.1.27 ,   3) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .action = largecopy, .arg = 1, .inst = 1 }, { .LSB = 0x13 }, { .MSB = 0x00 }, { .num =  2, .skip = 2 }, 
  INFO(    7.1.14 ,   9) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  4 }, 
  INFO(     7.1.7 ,  12) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  6 }, 
  INFO(     7.1.5 ,  15) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num = 13, .skip = 1 }, 
  INFO(     7.1.6 ,  18) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  3, .skip = 1 }, 
  INFO(    7.1.46 ,  21) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .action = largecopy, .arg = 2, .inst = 1 }, { .LSB = 0x1c }, { .MSB = 0x00 }, { .action = largecopy, .arg = 1, .inst = 1 }, { .LSB = 0x23 }, { .MSB = 0x00 }, 
  INFO(    7.1.45 ,  29) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .action = largecopy, .arg = 2, .inst = 1 }, { .LSB = 0x18 }, { .MSB = 0x00 }, { .action = largecopy, .arg = 1, .inst = 1 }, { .LSB = 0x23 }, { .MSB = 0x00 }, 
  INFO(     7.1.1 ,  37) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  5, .skip = 1 }, 
  INFO(     7.1.9 ,  40) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  7, .skip = 1 }, 
  INFO(     7.1.3 ,  43) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  9, .skip = 1 }, 
  INFO(    7.1.10 ,  46) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .action = largecopy, .inst = 1 }, { .LSB = 0x16 }, { .MSB = 0x00 }, 
  INFO(    7.1.18 ,  51) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, 
  INFO(    7.1.12 ,  54) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .action = largecopy, .arg = 1, .inst = 1 }, { .LSB = 0x17 }, { .MSB = 0x00 }, 
  INFO(    7.1.25 ,  59) { .num =  7, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  4 }, 
  INFO(    7.1.52 ,  62) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num = 12 }, 
  INFO(    7.1.19 ,  65) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num = 10 }, 
  INFO(    7.1.34 ,  68) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .action = largecopy, .inst = 1 }, { .LSB = 0x26 }, { .MSB = 0x00 }, 
  INFO(    7.1.53 ,  73) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .action = largecopy, .inst = 1 }, { .LSB = 0x46 }, { .MSB = 0x00 }, 
  INFO(    7.1.49 ,  78) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  8 }, 
  INFO(    7.1.50 ,  82) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .action = largecopy, .arg = 2, .inst = 1 }, { .LSB = 0x22 }, { .MSB = 0x00 }, 
  INFO(    7.1.26 ,  87) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num = 15, .skip = 1 }, { .num =  2, .skip = 2 }, 
  INFO(    7.1.47 ,  91) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  7, .skip = 1 }, { .num =  2 }, 
  INFO(    7.1.32 ,  95) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num = 10, .skip = 2 }, 
};

static const TE_t TE_1[39] = {
  { .OCF = 0x01, .isret = 0, .coistart =  37, .numcoi = 3 }, //   0   7.1.1      HCI_Inquiry
  { .OCF = 0x03, .isret = 0, .coistart =  43, .numcoi = 3 }, //   1   7.1.3      HCI_Periodic_Inquiry_Mode
  { .OCF = 0x05, .isret = 0, .coistart =  15, .numcoi = 3 }, //   2   7.1.5      HCI_Create_Connection
  { .OCF = 0x06, .isret = 0, .coistart =  18, .numcoi = 3 }, //   3   7.1.6      HCI_Disconnect
  { .OCF = 0x08, .isret = 0, .coistart =  12, .numcoi = 3 }, //   4   7.1.7      HCI_Create_Connection_Cancel
  { .OCF = 0x09, .isret = 0, .coistart =   0, .numcoi = 3 }, //   5   7.1.8      HCI_Accept_Connection_Request
  { .OCF = 0x0a, .isret = 0, .coistart =  40, .numcoi = 3 }, //   6   7.1.9      HCI_Reject_Connection_Request
  { .OCF = 0x0b, .isret = 0, .coistart =  46, .numcoi = 5 }, //   7   7.1.10     HCI_Link_Key_Request_Reply
  { .OCF = 0x0c, .isret = 0, .coistart =  12, .numcoi = 3 }, //   8   7.1.11     HCI_Link_Key_Request_Negative_Reply
  { .OCF = 0x0d, .isret = 0, .coistart =  54, .numcoi = 5 }, //   9   7.1.12     HCI_PIN_Code_Request_Reply
  { .OCF = 0x0e, .isret = 0, .coistart =  12, .numcoi = 3 }, //  10   7.1.13     HCI_PIN_Code_Request_Negative_Reply
  { .OCF = 0x0f, .isret = 0, .coistart =   9, .numcoi = 3 }, //  11   7.1.14     HCI_Change_Connection_Packet_Type
  { .OCF = 0x13, .isret = 0, .coistart =  18, .numcoi = 3 }, //  12   7.1.16     HCI_Set_Connection_Encryption
  { .OCF = 0x17, .isret = 0, .coistart =  51, .numcoi = 3 }, //  13   7.1.18     HCI_Link_Key_Selection
  { .OCF = 0x19, .isret = 0, .coistart =  65, .numcoi = 3 }, //  14   7.1.19     HCI_Remote_Name_Request
  { .OCF = 0x1a, .isret = 0, .coistart =  12, .numcoi = 3 }, //  15   7.1.20     HCI_Remote_Name_Request_Cancel
  { .OCF = 0x1c, .isret = 0, .coistart =  18, .numcoi = 3 }, //  16   7.1.22     HCI_Read_Remote_Extended_Features
  { .OCF = 0x20, .isret = 1, .coistart =  59, .numcoi = 3 }, //  17   7.1.25     HCI_Read_LMP_Handle
  { .OCF = 0x28, .isret = 0, .coistart =  87, .numcoi = 4 }, //  18   7.1.26     HCI_Setup_Synchronous_Connection
  { .OCF = 0x29, .isret = 0, .coistart =   3, .numcoi = 6 }, //  19   7.1.27     HCI_Accept_Synchronous_Connection_Request
  { .OCF = 0x2a, .isret = 0, .coistart =  40, .numcoi = 3 }, //  20   7.1.28     HCI_Reject_Synchronous_Connection_Request
  { .OCF = 0x2b, .isret = 0, .coistart =  43, .numcoi = 3 }, //  21   7.1.29     HCI_IO_Capability_Request_Reply
  { .OCF = 0x2c, .isret = 0, .coistart =  12, .numcoi = 3 }, //  22   7.1.30     HCI_User_Confirmation_Request_Reply
  { .OCF = 0x2d, .isret = 0, .coistart =  12, .numcoi = 3 }, //  23   7.1.31     HCI_User_Confirmation_Request_Negative_Reply
  { .OCF = 0x2e, .isret = 0, .coistart =  95, .numcoi = 3 }, //  24   7.1.32     HCI_User_Passkey_Request_Reply
  { .OCF = 0x2f, .isret = 0, .coistart =  12, .numcoi = 3 }, //  25   7.1.33     HCI_User_Passkey_Request_Negative_Reply
  { .OCF = 0x30, .isret = 0, .coistart =  68, .numcoi = 5 }, //  26   7.1.34     HCI_Remote_OOB_Data_Request_Reply
  { .OCF = 0x33, .isret = 0, .coistart =  12, .numcoi = 3 }, //  27   7.1.35     HCI_Remote_OOB_Data_Request_Negative_Reply
  { .OCF = 0x34, .isret = 0, .coistart =  40, .numcoi = 3 }, //  28   7.1.36     HCI_IO_Capability_Request_Negative_Reply
  { .OCF = 0x3d, .isret = 0, .coistart =  29, .numcoi = 8 }, //  29   7.1.45     HCI_Enhanced_Setup_Synchronous_Connection
  { .OCF = 0x3e, .isret = 0, .coistart =  21, .numcoi = 8 }, //  30   7.1.46     HCI_Enhanced_Accept_Synchronous_Connection_Request
  { .OCF = 0x3f, .isret = 0, .coistart =  91, .numcoi = 4 }, //  31   7.1.47     HCI_Truncated_Page
  { .OCF = 0x40, .isret = 0, .coistart =  12, .numcoi = 3 }, //  32   7.1.48     HCI_Truncated_Page_Cancel
  { .OCF = 0x41, .isret = 0, .coistart =  78, .numcoi = 4 }, //  33   7.1.49     HCI_Set_Connectionless_Peripheral_Broadcast
  { .OCF = 0x41, .isret = 1, .coistart =  67, .numcoi = 1 }, //  34   7.1.49     HCI_Set_Connectionless_Peripheral_Broadcast
  { .OCF = 0x42, .isret = 0, .coistart =  82, .numcoi = 5 }, //  35   7.1.50     HCI_Set_Connectionless_Peripheral_Broadcast_Receive
  { .OCF = 0x42, .isret = 1, .coistart =  17, .numcoi = 1 }, //  36   7.1.50     HCI_Set_Connectionless_Peripheral_Broadcast_Receive
  { .OCF = 0x44, .isret = 0, .coistart =  62, .numcoi = 3 }, //  37   7.1.52     HCI_Receive_Synchronization_Train
  { .OCF = 0x45, .isret = 0, .coistart =  73, .numcoi = 5 }, //  38   7.1.53     HCI_Remote_OOB_Extended_Data_Request_Reply
};

static const CodecTab_t CodecTab_1 = {
  .numtab = 39, .numcoi = 98, .table = TE_1, .CoI = CoI_1
};

static const CoI_t CoI_2[36] = {
  INFO(    7.2.13 ,   0) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  5, .skip = 1 }, { .action = largecopy, .inst = 1 }, { .LSB = 0x10 }, { .MSB = 0x00 }, 
  INFO(     7.2.1 ,   6) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  6 }, 
  INFO(     7.2.6 ,   9) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  4, .skip = 2 }, { .action = largecopy, .inst = 1 }, { .LSB = 0x10 }, { .MSB = 0x00 }, 
  INFO(    7.2.11 ,  15) { .num =  7, .skip = 1 }, { .num =  2 }, 
  INFO(     7.2.9 ,  17) { .num =  7, .skip = 1 }, { .num =  4 }, 
  INFO(     7.2.7 ,  19) { .num =  7, .skip = 1 }, { .num =  3, .skip = 1 }, 
  INFO(     7.2.2 ,  21) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num = 10 }, 
  INFO(    7.2.14 ,  24) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  8 }, 
  INFO(     7.2.8 ,  27) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  7, .skip = 1 }, 
  INFO(    7.2.12 ,  30) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  2 }, 
  INFO(    7.2.10 ,  33) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  4 }, 
};

static const TE_t TE_2[11] = {
  { .OCF = 0x01, .isret = 0, .coistart =   6, .numcoi = 3 }, //   0   7.2.1      HCI_Hold_Mode
  { .OCF = 0x03, .isret = 0, .coistart =  21, .numcoi = 3 }, //   1   7.2.2      HCI_Sniff_Mode
  { .OCF = 0x07, .isret = 0, .coistart =   9, .numcoi = 6 }, //   2   7.2.6      HCI_QoS_Setup
  { .OCF = 0x09, .isret = 1, .coistart =  19, .numcoi = 2 }, //   3   7.2.7      HCI_Role_Discovery
  { .OCF = 0x0b, .isret = 0, .coistart =  27, .numcoi = 3 }, //   4   7.2.8      HCI_Switch_Role
  { .OCF = 0x0c, .isret = 1, .coistart =  17, .numcoi = 2 }, //   5   7.2.9      HCI_Read_Link_Policy_Settings
  { .OCF = 0x0d, .isret = 0, .coistart =  33, .numcoi = 3 }, //   6   7.2.10     HCI_Write_Link_Policy_Settings
  { .OCF = 0x0e, .isret = 1, .coistart =  15, .numcoi = 2 }, //   7   7.2.11     HCI_Read_Default_Link_Policy_Settings
  { .OCF = 0x0f, .isret = 0, .coistart =  30, .numcoi = 3 }, //   8   7.2.12     HCI_Write_Default_Link_Policy_Settings
  { .OCF = 0x10, .isret = 0, .coistart =   0, .numcoi = 6 }, //   9   7.2.13     HCI_Flow_Specification
  { .OCF = 0x11, .isret = 0, .coistart =  24, .numcoi = 3 }, //  10   7.2.14     HCI_Sniff_Subrating
};

static const CodecTab_t CodecTab_2 = {
  .numtab = 11, .numcoi = 36, .table = TE_2, .CoI = CoI_2
};

static const CoI_t CoI_3[120] = {
  INFO(   7.3.101 ,   0) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  3 }, { .action = inlinefix, .inst = 1 }, 
  INFO(    7.3.83 ,   4) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, { .num =  8 }, 
  INFO(     7.3.8 ,   8) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  7, .skip = 1 }, 
  INFO(    7.3.10 ,  11) { .num =  7, .skip = 1 }, { .num =  2 }, 
  INFO(    7.3.26 ,  13) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  3, .skip = 1 }, 
  INFO(    7.3.39 ,  16) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  4 }, 
  INFO(    7.3.40 ,  20) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, { .action = loadloop, .inst = 1 }, { .num =  4 }, { .action = endloop, .inst = 1 }, 
  INFO(     7.3.8 ,  26) { .num =  7, .skip = 1 }, { .num =  4 }, 
  INFO(    7.3.25 ,  28) { .num = 10 }, 
  INFO(    7.3.44 ,  29) { .num =  8 }, { .action = inlinefix, .inst = 1 }, { .num =  3, .skip = 1 }, 
  INFO(    7.3.74 ,  32) { .num =  7, .skip = 1 }, { .num =  5, .skip = 1 }, 
  INFO(    7.3.55 ,  34) { .action = largecopy, .inst = 1 }, { .LSB = 0xf8 }, { .MSB = 0x00 }, 
  INFO(    7.3.78 ,  37) { .num =  9, .skip = 1 }, 
  INFO(    7.3.14 ,  38) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  2 }, 
  INFO(    7.3.12 ,  41) { .action = largecopy, .arg = 1, .inst = 1 }, { .LSB = 0xff }, { .MSB = 0x00 }, 
  INFO(    7.3.60 ,  44) { .action = largecopy, .arg = 1, .inst = 1 }, { .LSB = 0x27 }, { .MSB = 0x00 }, 
  INFO(    7.3.95 ,  47) { .action = largecopy, .arg = 1, .inst = 1 }, { .LSB = 0x47 }, { .MSB = 0x00 }, 
  INFO(    7.3.89 ,  50) { .num =  7, .skip = 1 }, { .num =  2, .skip = 2 }, { .num =  5, .skip = 3 }, 
  INFO(    7.3.35 ,  53) { .num =  7, .skip = 1 }, { .num =  3, .skip = 1 }, 
  INFO(    7.3.46 ,  55) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num = 10 }, 
  INFO(    7.3.88 ,  58) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  3 }, { .action = inlinefix, .inst = 1 }, 
  INFO(     7.3.1 ,  62) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  8 }, 
  INFO(    7.3.81 ,  65) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  7, .skip = 1 }, { .action = loadloop, .inst = 1 }, { .num =  3, .skip = 1 }, { .action = endloop, .inst = 1 }, 
  INFO(    7.3.80 ,  71) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, { .num =  9, .skip = 1 }, 
  INFO(    7.3.82 ,  75) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .action = largecopy, .inst = 1 }, { .LSB = 0x1e }, { .MSB = 0x00 }, 
  INFO(    7.3.82 ,  80) { .num =  7, .skip = 1 }, { .action = largecopy, .inst = 1 }, { .LSB = 0x20 }, { .MSB = 0x00 }, 
  INFO(    7.3.85 ,  84) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  2 }, { .action = loadloop, .inst = 1 }, { .num =  1, .skip = 1 }, { .action = endloop, .inst = 1 }, 
  INFO(    7.3.20 ,  90) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  4 }, 
  INFO(    7.3.45 ,  93) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  1 }, { .action = inlinefix, .inst = 1 }, { .num =  3, .skip = 1 }, 
  INFO(    7.3.56 ,  98) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .action = largecopy, .arg = 1, .inst = 1 }, { .LSB = 0xf1 }, { .MSB = 0x00 }, 
  INFO(    7.3.11 , 103) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .action = largecopy, .inst = 1 }, { .LSB = 0xf8 }, { .MSB = 0x00 }, 
  INFO(     7.3.9 , 108) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  1 }, { .action = loadloop, .inst = 1 }, { .action = largecopy, .inst = 1 }, { .LSB = 0x16 }, { .MSB = 0x00 }, { .action = endloop, .inst = 1 }, 
  INFO(    7.3.90 , 116) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  4, .skip = 2 }, { .num =  5, .skip = 3 }, 
};

static const TE_t TE_3[103] = {
  { .OCF = 0x01, .isret = 0, .coistart =  62, .numcoi = 3 }, //   0   7.3.1      HCI_Set_Event_Mask
  { .OCF = 0x05, .isret = 0, .coistart =  62, .numcoi = 3 }, //   1   7.3.3      HCI_Set_Event_Filter
  { .OCF = 0x09, .isret = 1, .coistart =   7, .numcoi = 1 }, //   2   7.3.5      HCI_Read_PIN_Type
  { .OCF = 0x0a, .isret = 0, .coistart =   4, .numcoi = 3 }, //   3   7.3.6      HCI_Write_PIN_Type
  { .OCF = 0x0d, .isret = 0, .coistart =   8, .numcoi = 3 }, //   4   7.3.8      HCI_Read_Stored_Link_Key
  { .OCF = 0x0d, .isret = 1, .coistart =  26, .numcoi = 2 }, //   5   7.3.8      HCI_Read_Stored_Link_Key
  { .OCF = 0x11, .isret = 0, .coistart = 108, .numcoi = 8 }, //   6   7.3.9      HCI_Write_Stored_Link_Key
  { .OCF = 0x11, .isret = 1, .coistart =   7, .numcoi = 1 }, //   7   7.3.9      HCI_Write_Stored_Link_Key
  { .OCF = 0x12, .isret = 0, .coistart =   8, .numcoi = 3 }, //   8   7.3.10     HCI_Delete_Stored_Link_Key
  { .OCF = 0x12, .isret = 1, .coistart =  11, .numcoi = 2 }, //   9   7.3.10     HCI_Delete_Stored_Link_Key
  { .OCF = 0x13, .isret = 0, .coistart = 103, .numcoi = 5 }, //  10   7.3.11     HCI_Write_Local_Name
  { .OCF = 0x14, .isret = 1, .coistart =  41, .numcoi = 3 }, //  11   7.3.12     HCI_Read_Local_Name
  { .OCF = 0x15, .isret = 1, .coistart =  11, .numcoi = 2 }, //  12   7.3.13     HCI_Read_Connection_Accept_Timeout
  { .OCF = 0x16, .isret = 0, .coistart =  38, .numcoi = 3 }, //  13   7.3.14     HCI_Write_Connection_Accept_Timeout
  { .OCF = 0x17, .isret = 1, .coistart =  11, .numcoi = 2 }, //  14   7.3.15     HCI_Read_Page_Timeout
  { .OCF = 0x18, .isret = 0, .coistart =  38, .numcoi = 3 }, //  15   7.3.16     HCI_Write_Page_Timeout
  { .OCF = 0x19, .isret = 1, .coistart =   7, .numcoi = 1 }, //  16   7.3.17     HCI_Read_Scan_Enable
  { .OCF = 0x1a, .isret = 0, .coistart =   4, .numcoi = 3 }, //  17   7.3.18     HCI_Write_Scan_Enable
  { .OCF = 0x1b, .isret = 1, .coistart =  26, .numcoi = 2 }, //  18   7.3.19     HCI_Read_Page_Scan_Activity
  { .OCF = 0x1c, .isret = 0, .coistart =  90, .numcoi = 3 }, //  19   7.3.20     HCI_Write_Page_Scan_Activity
  { .OCF = 0x1d, .isret = 1, .coistart =  26, .numcoi = 2 }, //  20   7.3.21     HCI_Read_Inquiry_Scan_Activity
  { .OCF = 0x1e, .isret = 0, .coistart =  90, .numcoi = 3 }, //  21   7.3.22     HCI_Write_Inquiry_Scan_Activity
  { .OCF = 0x1f, .isret = 1, .coistart =   7, .numcoi = 1 }, //  22   7.3.23     HCI_Read_Authentication_Enable
  { .OCF = 0x20, .isret = 0, .coistart =   4, .numcoi = 3 }, //  23   7.3.24     HCI_Write_Authentication_Enable
  { .OCF = 0x23, .isret = 1, .coistart =  28, .numcoi = 1 }, //  24   7.3.25     HCI_Read_Class_Of_Device
  { .OCF = 0x24, .isret = 0, .coistart =  13, .numcoi = 3 }, //  25   7.3.26     HCI_Write_Class_Of_Device
  { .OCF = 0x25, .isret = 1, .coistart =  11, .numcoi = 2 }, //  26   7.3.27     HCI_Read_Voice_Setting
  { .OCF = 0x26, .isret = 0, .coistart =  38, .numcoi = 3 }, //  27   7.3.28     HCI_Write_Voice_Setting
  { .OCF = 0x27, .isret = 1, .coistart =  26, .numcoi = 2 }, //  28   7.3.29     HCI_Read_Automatic_Flush_Timeout
  { .OCF = 0x28, .isret = 0, .coistart =  90, .numcoi = 3 }, //  29   7.3.30     HCI_Write_Automatic_Flush_Timeout
  { .OCF = 0x29, .isret = 1, .coistart =   7, .numcoi = 1 }, //  30   7.3.31     HCI_Read_Num_Broadcast_Retransmissions
  { .OCF = 0x2a, .isret = 0, .coistart =   4, .numcoi = 3 }, //  31   7.3.32     HCI_Write_Num_Broadcast_Retransmissions
  { .OCF = 0x2b, .isret = 1, .coistart =   7, .numcoi = 1 }, //  32   7.3.33     HCI_Read_Hold_Mode_Activity
  { .OCF = 0x2c, .isret = 0, .coistart =   4, .numcoi = 3 }, //  33   7.3.34     HCI_Write_Hold_Mode_Activity
  { .OCF = 0x2d, .isret = 0, .coistart =  13, .numcoi = 3 }, //  34   7.3.35     HCI_Read_Transmit_Power_Level
  { .OCF = 0x2d, .isret = 1, .coistart =  53, .numcoi = 2 }, //  35   7.3.35     HCI_Read_Transmit_Power_Level
  { .OCF = 0x2e, .isret = 1, .coistart =   7, .numcoi = 1 }, //  36   7.3.36     HCI_Read_Synchronous_Flow_Control_Enable
  { .OCF = 0x2f, .isret = 0, .coistart =   4, .numcoi = 3 }, //  37   7.3.37     HCI_Write_Synchronous_Flow_Control_Enable
  { .OCF = 0x31, .isret = 0, .coistart =   4, .numcoi = 3 }, //  38   7.3.38     HCI_Set_Controller_To_Host_Flow_Control
  { .OCF = 0x33, .isret = 0, .coistart =  16, .numcoi = 4 }, //  39   7.3.39     HCI_Host_Buffer_Size
  { .OCF = 0x35, .isret = 0, .coistart =  20, .numcoi = 6 }, //  40   7.3.40     HCI_Host_Number_Of_Completed_Packets
  { .OCF = 0x36, .isret = 0, .coistart =  38, .numcoi = 3 }, //  41   7.3.41     HCI_Read_Link_Supervision_Timeout
  { .OCF = 0x36, .isret = 1, .coistart =  26, .numcoi = 2 }, //  42   7.3.41     HCI_Read_Link_Supervision_Timeout
  { .OCF = 0x37, .isret = 0, .coistart =  90, .numcoi = 3 }, //  43   7.3.42     HCI_Write_Link_Supervision_Timeout
  { .OCF = 0x37, .isret = 1, .coistart =  11, .numcoi = 2 }, //  44   7.3.42     HCI_Write_Link_Supervision_Timeout
  { .OCF = 0x38, .isret = 1, .coistart =   7, .numcoi = 1 }, //  45   7.3.43     HCI_Read_Number_Of_Supported_IAC
  { .OCF = 0x39, .isret = 1, .coistart =  29, .numcoi = 3 }, //  46   7.3.44     HCI_Read_Current_IAC_LAP
  { .OCF = 0x3a, .isret = 0, .coistart =  93, .numcoi = 5 }, //  47   7.3.45     HCI_Write_Current_IAC_LAP
  { .OCF = 0x3f, .isret = 0, .coistart =  55, .numcoi = 3 }, //  48   7.3.46     HCI_Set_AFH_Host_Channel_Classification
  { .OCF = 0x42, .isret = 1, .coistart =   7, .numcoi = 1 }, //  49   7.3.47     HCI_Read_Inquiry_Scan_Type
  { .OCF = 0x43, .isret = 0, .coistart =   4, .numcoi = 3 }, //  50   7.3.48     HCI_Write_Inquiry_Scan_Type
  { .OCF = 0x44, .isret = 1, .coistart =   7, .numcoi = 1 }, //  51   7.3.49     HCI_Read_Inquiry_Mode
  { .OCF = 0x45, .isret = 0, .coistart =   4, .numcoi = 3 }, //  52   7.3.50     HCI_Write_Inquiry_Mode
  { .OCF = 0x46, .isret = 1, .coistart =   7, .numcoi = 1 }, //  53   7.3.51     HCI_Read_Page_Scan_Type
  { .OCF = 0x47, .isret = 0, .coistart =   4, .numcoi = 3 }, //  54   7.3.52     HCI_Write_Page_Scan_Type
  { .OCF = 0x48, .isret = 1, .coistart =   7, .numcoi = 1 }, //  55   7.3.53     HCI_Read_AFH_Channel_Assessment_Mode
  { .OCF = 0x49, .isret = 0, .coistart =   4, .numcoi = 3 }, //  56   7.3.54     HCI_Write_AFH_Channel_Assessment_Mode
  { .OCF = 0x51, .isret = 1, .coistart =  34, .numcoi = 3 }, //  57   7.3.55     HCI_Read_Extended_Inquiry_Response
  { .OCF = 0x52, .isret = 0, .coistart =  98, .numcoi = 5 }, //  58   7.3.56     HCI_Write_Extended_Inquiry_Response
  { .OCF = 0x55, .isret = 1, .coistart =   7, .numcoi = 1 }, //  59   7.3.58     HCI_Read_Simple_Pairing_Mode
  { .OCF = 0x56, .isret = 0, .coistart =   4, .numcoi = 3 }, //  60   7.3.59     HCI_Write_Simple_Pairing_Mode
  { .OCF = 0x57, .isret = 1, .coistart =  44, .numcoi = 3 }, //  61   7.3.60     HCI_Read_Local_OOB_Data
  { .OCF = 0x58, .isret = 1, .coistart =   7, .numcoi = 1 }, //  62   7.3.61     HCI_Read_Inquiry_Response_Transmit_Power_Level
  { .OCF = 0x59, .isret = 0, .coistart =   4, .numcoi = 3 }, //  63   7.3.62     HCI_Write_Inquiry_Transmit_Power_Level
  { .OCF = 0x5a, .isret = 1, .coistart =   7, .numcoi = 1 }, //  64   7.3.64     HCI_Read_Default_Erroneous_Data_Reporting
  { .OCF = 0x5b, .isret = 0, .coistart =   4, .numcoi = 3 }, //  65   7.3.65     HCI_Write_Default_Erroneous_Data_Reporting
  { .OCF = 0x5f, .isret = 0, .coistart =  13, .numcoi = 3 }, //  66   7.3.66     HCI_Enhanced_Flush
  { .OCF = 0x60, .isret = 0, .coistart =   8, .numcoi = 3 }, //  67   7.3.63     HCI_Send_Keypress_Notification
  { .OCF = 0x63, .isret = 0, .coistart =  62, .numcoi = 3 }, //  68   7.3.69     HCI_Set_Event_Mask_Page_2
  { .OCF = 0x66, .isret = 1, .coistart =   7, .numcoi = 1 }, //  69   7.3.72     HCI_Read_Flow_Control_Mode
  { .OCF = 0x67, .isret = 0, .coistart =   4, .numcoi = 3 }, //  70   7.3.73     HCI_Write_Flow_Control_Mode
  { .OCF = 0x68, .isret = 0, .coistart =  13, .numcoi = 3 }, //  71   7.3.74     HCI_Read_Enhanced_Transmit_Power_Level
  { .OCF = 0x68, .isret = 1, .coistart =  32, .numcoi = 2 }, //  72   7.3.74     HCI_Read_Enhanced_Transmit_Power_Level
  { .OCF = 0x6c, .isret = 1, .coistart =  37, .numcoi = 1 }, //  73   7.3.78     HCI_Read_LE_Host_Support
  { .OCF = 0x6d, .isret = 0, .coistart =  38, .numcoi = 3 }, //  74   7.3.79     HCI_Write_LE_Host_Support
  { .OCF = 0x6e, .isret = 0, .coistart =  71, .numcoi = 4 }, //  75   7.3.80     HCI_Set_MWS_Channel_Parameters
  { .OCF = 0x6f, .isret = 0, .coistart =  65, .numcoi = 6 }, //  76   7.3.81     HCI_Set_External_Frame_Configuration
  { .OCF = 0x70, .isret = 0, .coistart =  75, .numcoi = 5 }, //  77   7.3.82     HCI_Set_MWS_Signaling
  { .OCF = 0x70, .isret = 1, .coistart =  80, .numcoi = 4 }, //  78   7.3.82     HCI_Set_MWS_Signaling
  { .OCF = 0x71, .isret = 0, .coistart =   4, .numcoi = 4 }, //  79   7.3.83     HCI_Set_MWS_Transport_Layer
  { .OCF = 0x72, .isret = 0, .coistart =  20, .numcoi = 6 }, //  80   7.3.84     HCI_Set_MWS_Scan_Frequency_Table
  { .OCF = 0x73, .isret = 0, .coistart =  84, .numcoi = 6 }, //  81   7.3.85     HCI_Set_MWS_PATTERN_Configuration
  { .OCF = 0x74, .isret = 0, .coistart =   4, .numcoi = 3 }, //  82   7.3.86     HCI_Set_Reserved_LT_ADDR
  { .OCF = 0x74, .isret = 1, .coistart =   7, .numcoi = 1 }, //  83   7.3.86     HCI_Set_Reserved_LT_ADDR
  { .OCF = 0x75, .isret = 0, .coistart =   4, .numcoi = 3 }, //  84   7.3.87     HCI_Delete_Reserved_LT_ADDR
  { .OCF = 0x75, .isret = 1, .coistart =   7, .numcoi = 1 }, //  85   7.3.87     HCI_Delete_Reserved_LT_ADDR
  { .OCF = 0x76, .isret = 0, .coistart =  58, .numcoi = 4 }, //  86   7.3.88     HCI_Set_Connectionless_Peripheral_Broadcast_Data
  { .OCF = 0x76, .isret = 1, .coistart =   7, .numcoi = 1 }, //  87   7.3.88     HCI_Set_Connectionless_Peripheral_Broadcast_Data
  { .OCF = 0x77, .isret = 1, .coistart =  50, .numcoi = 3 }, //  88   7.3.89     HCI_Read_Synchronization_Train_Parameters
  { .OCF = 0x78, .isret = 0, .coistart = 116, .numcoi = 4 }, //  89   7.3.90     HCI_Write_Synchronization_Train_Parameters
  { .OCF = 0x78, .isret = 1, .coistart =  11, .numcoi = 2 }, //  90   7.3.90     HCI_Write_Synchronization_Train_Parameters
  { .OCF = 0x79, .isret = 1, .coistart =   7, .numcoi = 1 }, //  91   7.3.91     HCI_Read_Secure_Connections_Host_Support
  { .OCF = 0x7a, .isret = 0, .coistart =   4, .numcoi = 3 }, //  92   7.3.92     HCI_Write_Secure_Connections_Host_Support
  { .OCF = 0x7b, .isret = 1, .coistart =  26, .numcoi = 2 }, //  93   7.3.93     HCI_Read_Authenticated_Payload_Timeout
  { .OCF = 0x7c, .isret = 0, .coistart =  90, .numcoi = 3 }, //  94   7.3.94     HCI_Write_Authenticated_Payload_Timeout
  { .OCF = 0x7d, .isret = 1, .coistart =  47, .numcoi = 3 }, //  95   7.3.95     HCI_Read_Local_OOB_Extended_Data
  { .OCF = 0x7e, .isret = 1, .coistart =  11, .numcoi = 2 }, //  96   7.3.96     HCI_Read_Extended_Page_Timeout
  { .OCF = 0x7f, .isret = 0, .coistart =  38, .numcoi = 3 }, //  97   7.3.97     HCI_Write_Extended_Page_Timeout
  { .OCF = 0x80, .isret = 1, .coistart =  11, .numcoi = 2 }, //  98   7.3.98     HCI_Read_Extended_Inquiry_Length
  { .OCF = 0x81, .isret = 0, .coistart =  38, .numcoi = 3 }, //  99   7.3.99     HCI_Write_Extended_Inquiry_Length
  { .OCF = 0x82, .isret = 0, .coistart =  38, .numcoi = 3 }, // 100   7.3.100    HCI_Set_Ecosystem_Base_Interval
  { .OCF = 0x83, .isret = 0, .coistart =   0, .numcoi = 4 }, // 101   7.3.101    HCI_Configure_Data_Path
  { .OCF = 0x84, .isret = 0, .coistart =   4, .numcoi = 3 }, // 102   7.3.102    HCI_Set_Min_Encryption_Key_Size
};

static const CodecTab_t CodecTab_3 = {
  .numtab = 103, .numcoi = 120, .table = TE_3, .CoI = CoI_3
};

static const CoI_t CoI_4[37] = {
  INFO(     7.4.5 ,   0) { .num =  7, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  4 }, 
  INFO(     7.4.7 ,   3) { .num =  7, .skip = 1 }, { .num =  6 }, 
  INFO(     7.4.4 ,   5) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, 
  INFO(     7.4.4 ,   8) { .action = largecopy, .arg = 1, .inst = 1 }, { .LSB = 0x11 }, { .MSB = 0x00 }, 
  INFO(     7.4.9 ,  11) { .num =  9, .skip = 1 }, 
  INFO(    7.4.10 ,  12) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  7, .skip = 1 }, 
  INFO(    7.4.10 ,  15) { .num =  8 }, { .action = loadloop, .inst = 1 }, { .num =  1, .skip = 3 }, { .action = inlinefix, .arg = 1, .inst = 1 }, { .action = endloop, .inst = 1 }, 
  INFO(     7.4.8 ,  20) { .num =  8 }, { .action = laterfix, .arg = 2, .inst = 1 }, { .num =  1, .skip = 3 }, 
  INFO(     7.4.8 ,  23) { .num =  8 }, { .action = inlinefix, .arg = 1, .inst = 1 }, { .num =  2, .skip = 2 }, 
  INFO(     7.4.2 ,  26) { .action = largecopy, .arg = 1, .inst = 1 }, { .LSB = 0x47 }, { .MSB = 0x00 }, 
  INFO(    7.4.11 ,  29) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  8 }, { .action = inlinefix, .inst = 1 }, 
  INFO(    7.4.11 ,  33) { .num = 13, .skip = 1 }, 
  INFO(     7.4.3 ,  34) { .num = 15, .skip = 1 }, 
  INFO(     7.4.1 ,  35) { .num = 11, .skip = 1 }, { .num =  4 }, 
};

static const TE_t TE_4[14] = {
  { .OCF = 0x01, .isret = 1, .coistart =  35, .numcoi = 2 }, //   0   7.4.1      HCI_Read_Local_Version_Information
  { .OCF = 0x02, .isret = 1, .coistart =  26, .numcoi = 3 }, //   1   7.4.2      HCI_Read_Local_Supported_Commands
  { .OCF = 0x03, .isret = 1, .coistart =  34, .numcoi = 1 }, //   2   7.4.3      HCI_Read_Local_Supported_Features
  { .OCF = 0x04, .isret = 0, .coistart =   5, .numcoi = 3 }, //   3   7.4.4      HCI_Read_Local_Extended_Features
  { .OCF = 0x04, .isret = 1, .coistart =   8, .numcoi = 3 }, //   4   7.4.4      HCI_Read_Local_Extended_Features
  { .OCF = 0x05, .isret = 1, .coistart =   0, .numcoi = 3 }, //   5   7.4.5      HCI_Read_Buffer_Size
  { .OCF = 0x0a, .isret = 1, .coistart =   3, .numcoi = 2 }, //   6   7.4.7      HCI_Read_Data_Block_Size
  { .OCF = 0x0b, .isret = 1, .coistart =  23, .numcoi = 3 }, //   7   7.4.8      HCI_Read_Local_Supported_Codecs [v1]
  { .OCF = 0x0c, .isret = 1, .coistart =  11, .numcoi = 1 }, //   8   7.4.9      HCI_Read_Local_Simple_Pairing_Options
  { .OCF = 0x0d, .isret = 1, .coistart =  20, .numcoi = 3 }, //   9   7.4.8      HCI_Read_Local_Supported_Codecs [v2]
  { .OCF = 0x0e, .isret = 0, .coistart =  12, .numcoi = 3 }, //  10   7.4.10     HCI_Read_Local_Supported_Codec_Capabilities
  { .OCF = 0x0e, .isret = 1, .coistart =  15, .numcoi = 5 }, //  11   7.4.10     HCI_Read_Local_Supported_Codec_Capabilities
  { .OCF = 0x0f, .isret = 0, .coistart =  29, .numcoi = 4 }, //  12   7.4.11     HCI_Read_Local_Supported_Controller_Delay
  { .OCF = 0x0f, .isret = 1, .coistart =  33, .numcoi = 1 }, //  13   7.4.11     HCI_Read_Local_Supported_Controller_Delay
};

static const CodecTab_t CodecTab_4 = {
  .numtab = 14, .numcoi = 37, .table = TE_4, .CoI = CoI_4
};

static const CoI_t CoI_5[22] = {
  INFO(    7.5.11 ,   0) { .num =  8 }, { .action = loadloop, .inst = 1 }, { .num =  2, .skip = 2 }, { .action = laterfix, .arg = 8, .inst = 1 }, { .action = endloop, .inst = 1 }, 
  INFO(     7.5.5 ,   5) { .num =  7, .skip = 1 }, { .num = 13, .skip = 1 }, 
  INFO(     7.5.1 ,   7) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  2 }, 
  INFO(     7.5.6 ,  10) { .num =  7, .skip = 1 }, { .num =  2, .skip = 2 }, { .num =  6, .skip = 2 }, 
  INFO(     7.5.3 ,  13) { .num =  7, .skip = 1 }, { .num =  3, .skip = 1 }, 
  INFO(     7.5.1 ,  15) { .num =  7, .skip = 1 }, { .num =  4 }, 
  INFO(     7.5.2 ,  17) { .num =  7, .skip = 1 }, { .num =  2 }, 
  INFO(    7.5.12 ,  19) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  6 }, 
};

static const TE_t TE_5[14] = {
  { .OCF = 0x01, .isret = 0, .coistart =   7, .numcoi = 3 }, //   0   7.5.1      HCI_Read_Failed_Contact_Counter
  { .OCF = 0x01, .isret = 1, .coistart =  15, .numcoi = 2 }, //   1   7.5.1      HCI_Read_Failed_Contact_Counter
  { .OCF = 0x02, .isret = 0, .coistart =   7, .numcoi = 3 }, //   2   7.5.2      HCI_Reset_Failed_Contact_Counter
  { .OCF = 0x02, .isret = 1, .coistart =  17, .numcoi = 2 }, //   3   7.5.2      HCI_Reset_Failed_Contact_Counter
  { .OCF = 0x03, .isret = 0, .coistart =   7, .numcoi = 3 }, //   4   7.5.3      HCI_Read_Link_Quality
  { .OCF = 0x03, .isret = 1, .coistart =  13, .numcoi = 2 }, //   5   7.5.3      HCI_Read_Link_Quality
  { .OCF = 0x05, .isret = 0, .coistart =   7, .numcoi = 3 }, //   6   7.5.4      HCI_Read_RSSI
  { .OCF = 0x05, .isret = 1, .coistart =  13, .numcoi = 2 }, //   7   7.5.4      HCI_Read_RSSI
  { .OCF = 0x06, .isret = 1, .coistart =   5, .numcoi = 2 }, //   8   7.5.5      HCI_Read_AFH_Channel_Map
  { .OCF = 0x07, .isret = 0, .coistart =   7, .numcoi = 3 }, //   9   7.5.6      HCI_Read_Clock
  { .OCF = 0x07, .isret = 1, .coistart =  10, .numcoi = 3 }, //  10   7.5.6      HCI_Read_Clock
  { .OCF = 0x08, .isret = 1, .coistart =  13, .numcoi = 2 }, //  11   7.5.7      HCI_Read_Encryption_Key_Size
  { .OCF = 0x0c, .isret = 1, .coistart =   0, .numcoi = 5 }, //  12   7.5.11     HCI_Get_MWS_Transport_Layer_Configuration
  { .OCF = 0x0d, .isret = 0, .coistart =  19, .numcoi = 3 }, //  13   7.5.12     HCI_Set_Triggered_Clock_Capture
};

static const CodecTab_t CodecTab_5 = {
  .numtab = 14, .numcoi = 22, .table = TE_5, .CoI = CoI_5
};

static const CoI_t CoI_6[7] = {
  INFO(     7.6.1 ,   0) { .num =  8 }, 
  INFO(     7.6.2 ,   1) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, 
  INFO(     7.6.8 ,   4) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  4 }, 
};

static const TE_t TE_6[4] = {
  { .OCF = 0x01, .isret = 1, .coistart =   0, .numcoi = 1 }, //   0   7.6.1      HCI_Read_Loopback_Mode
  { .OCF = 0x02, .isret = 0, .coistart =   1, .numcoi = 3 }, //   1   7.6.2      HCI_Write_Loopback_Mode
  { .OCF = 0x04, .isret = 0, .coistart =   1, .numcoi = 3 }, //   2   7.6.4      HCI_Write_Simple_Pairing_Debug_Mode
  { .OCF = 0x0a, .isret = 0, .coistart =   4, .numcoi = 3 }, //   3   7.6.8      HCI_Write_Secure_Connections_Test_Mode
};

static const CodecTab_t CodecTab_6 = {
  .numtab = 4, .numcoi = 7, .table = TE_6, .CoI = CoI_6
};

static const CoI_t CoI_7[0] = {
};

static const TE_t TE_7[0] = {
};

static const CodecTab_t CodecTab_7 = {
  .numtab = 0, .numcoi = 0, .table = TE_7, .CoI = CoI_7
};

static const CoI_t CoI_8[233] = {
  INFO(    7.8.16 ,   0) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  7, .skip = 1 }, 
  INFO(     7.8.1 ,   3) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  8 }, 
  INFO(    7.8.38 ,   6) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .action = largecopy, .arg = 1, .inst = 1 }, { .LSB = 0x27 }, { .MSB = 0x00 }, 
  INFO(   7.8.106 ,  11) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, { .num =  7 }, { .action = inlinefix, .inst = 1 }, { .num =  1, .skip = 1 }, 
  INFO(    7.8.85 ,  17) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  4 }, 
  INFO(    7.8.18 ,  21) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num = 14 }, 
  INFO(   7.8.103 ,  24) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .action = largecopy, .arg = 1, .inst = 1 }, { .LSB = 0x1f }, { .MSB = 0x00 }, 
  INFO(   7.8.104 ,  29) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  9, .skip = 1 }, { .action = largecopy, .arg = 1, .inst = 1 }, { .LSB = 0x1b }, { .MSB = 0x00 }, 
  INFO(    7.8.99 ,  35) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, { .action = loadloop, .inst = 1 }, { .num =  4 }, { .action = endloop, .inst = 1 }, 
  INFO(    7.8.12 ,  41) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num = 13, .skip = 1 }, { .num = 12 }, 
  INFO(    7.8.24 ,  45) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .action = largecopy, .inst = 1 }, { .LSB = 0x1c }, { .MSB = 0x00 }, 
  INFO(     7.8.7 ,  50) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .action = largecopy, .inst = 1 }, { .LSB = 0x20 }, { .MSB = 0x00 }, 
  INFO(    7.8.22 ,  55) { .action = largecopy, .arg = 1, .inst = 1 }, { .LSB = 0x17 }, { .MSB = 0x00 }, 
  INFO(   7.8.117 ,  58) { .num =  7, .skip = 1 }, { .num =  5, .skip = 1 }, 
  INFO(    7.8.66 ,  60) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num = 12 }, { .action = countmodif, .arg = 1, .inst = 1 }, { .action = loadloop, .inst = 1 }, { .action = endloop, .inst = 1 }, 
  INFO(    7.8.66 ,  66) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num = 10 }, { .action = countmodif, .arg = 1, .inst = 1 }, { .action = loadloop, .inst = 1 }, { .action = endloop, .inst = 1 }, 
  INFO(    7.8.37 ,  72) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .action = largecopy, .arg = 1, .inst = 1 }, { .LSB = 0x41 }, { .MSB = 0x00 }, 
  INFO(    7.8.37 ,  77) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .action = largecopy, .inst = 1 }, { .LSB = 0x40 }, { .MSB = 0x00 }, 
  INFO(   7.8.113 ,  82) { .num =  7, .skip = 1 }, { .num =  2, .skip = 2 }, { .num = 12 }, 
  INFO(    7.8.25 ,  85) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .action = largecopy, .inst = 1 }, { .LSB = 0x12 }, { .MSB = 0x00 }, 
  INFO(    7.8.67 ,  90) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  9, .skip = 1 }, { .num =  5, .skip = 1 }, 
  INFO(    7.8.19 ,  94) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  5, .skip = 1 }, 
  INFO(     7.8.4 ,  97) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  6 }, 
  INFO(    7.8.11 , 100) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  2 }, 
  INFO(     7.8.2 , 103) { .num =  7, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  3, .skip = 1 }, 
  INFO(    7.8.20 , 106) { .num =  7, .skip = 1 }, { .num =  7, .skip = 1 }, 
  INFO(   7.8.116 , 108) { .num =  7, .skip = 1 }, { .num =  2, .skip = 2 }, { .action = largecopy, .inst = 1 }, { .LSB = 0x1c }, { .MSB = 0x00 }, 
  INFO(    7.8.96 , 113) { .num =  7, .skip = 1 }, { .num = 11, .skip = 1 }, 
  INFO(    7.8.17 , 115) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  7, .skip = 1 }, 
  INFO(    7.8.30 , 118) { .num =  7, .skip = 1 }, { .num =  2 }, 
  INFO(    7.8.46 , 120) { .num =  7, .skip = 1 }, { .num =  8 }, 
  INFO(    7.8.34 , 122) { .num =  7, .skip = 1 }, { .num =  4 }, 
  INFO(    7.8.28 , 124) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  7 }, { .action = inlinefix, .inst = 1 }, { .num =  1, .skip = 1 }, 
  INFO(     7.8.5 , 129) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num = 15, .skip = 1 }, 
  INFO(    7.8.97 , 132) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num = 15, .skip = 1 }, { .action = loadloop, .inst = 1 }, { .num =  1, .skip = 1 }, { .num =  8 }, { .action = endloop, .inst = 1 }, 
  INFO(    7.8.97 , 139) { .num =  9, .skip = 1 }, { .action = inlinefix, .inst = 1 }, { .num =  2 }, 
  INFO(    7.8.98 , 142) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  9, .skip = 1 }, { .num =  6 }, { .action = loadloop, .inst = 1 }, { .num = 14 }, { .action = endloop, .inst = 1 }, 
  INFO(    7.8.80 , 149) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  5 }, { .action = inlinefix, .inst = 1 }, { .num =  1, .skip = 1 }, 
  INFO(    7.8.84 , 154) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  4 }, { .action = inlinefix, .inst = 1 }, { .num =  1, .skip = 1 }, 
  INFO(    7.8.82 , 159) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  6 }, { .action = inlinefix, .inst = 1 }, { .num =  1, .skip = 1 }, 
  INFO(    7.8.92 , 164) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, { .num =  5, .skip = 1 }, 
  INFO(    7.8.56 , 168) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  2 }, { .action = loadloop, .inst = 1 }, { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .action = endloop, .inst = 1 }, 
  INFO(    7.8.53 , 175) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, { .action = largecopy, .inst = 1 }, { .LSB = 0x1a }, { .MSB = 0x00 }, 
  INFO(    7.8.53 , 181) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, { .action = largecopy, .inst = 1 }, { .LSB = 0x18 }, { .MSB = 0x00 }, 
  INFO(    7.8.64 , 187) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  3 }, { .action = countmodif, .arg = 1, .inst = 1 }, { .action = loadloop, .inst = 1 }, { .action = endloop, .inst = 1 }, 
  INFO(    7.8.62 , 193) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  3 }, { .action = inlinefix, .inst = 1 }, 
  INFO(    7.8.61 , 197) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, { .num = 11, .skip = 1 }, 
  INFO(    7.8.10 , 201) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, { .num =  6 }, 
  INFO(   7.8.126 , 205) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  8 }, { .action = inlinefix, .inst = 1 }, 
  INFO(   7.8.125 , 209) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  2 }, { .action = loadloop, .inst = 1 }, { .num =  4 }, { .action = inlinefix, .arg = 1, .inst = 1 }, { .action = endloop, .inst = 1 }, 
  INFO(    7.8.91 , 216) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  5, .skip = 1 }, 
  INFO(    7.8.49 , 220) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  5, .skip = 1 }, { .num =  2 }, 
  INFO(   7.8.109 , 224) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num = 13 }, { .action = inlinefix, .inst = 1 }, 
  INFO(    7.8.29 , 228) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  7, .skip = 3 }, { .action = inlinefix, .arg = 1, .inst = 1 }, { .num =  2, .skip = 2 }, 
};

static const TE_t TE_8[135] = {
  { .OCF = 0x01, .isret = 0, .coistart =   3, .numcoi = 3 }, //   0   7.8.1      HCI_LE_Set_Event_Mask
  { .OCF = 0x02, .isret = 1, .coistart = 103, .numcoi = 2 }, //   1   7.8.2      HCI_LE_Read_Buffer_Size [v1]
  { .OCF = 0x03, .isret = 1, .coistart =  27, .numcoi = 1 }, //   2   7.8.3      HCI_LE_Read_Local_Supported_Features
  { .OCF = 0x05, .isret = 0, .coistart =  97, .numcoi = 3 }, //   3   7.8.4      HCI_LE_Set_Random_Address
  { .OCF = 0x06, .isret = 0, .coistart = 129, .numcoi = 3 }, //   4   7.8.5      HCI_LE_Set_Advertising_Parameters
  { .OCF = 0x07, .isret = 1, .coistart =   5, .numcoi = 1 }, //   5   7.8.6      HCI_LE_Read_Advertising_Channel_Tx_Power
  { .OCF = 0x08, .isret = 0, .coistart =  50, .numcoi = 5 }, //   6   7.8.7      HCI_LE_Set_Advertising_Data
  { .OCF = 0x09, .isret = 0, .coistart =  50, .numcoi = 5 }, //   7   7.8.8      HCI_LE_Set_Scan_Response_Data
  { .OCF = 0x0a, .isret = 0, .coistart =  11, .numcoi = 3 }, //   8   7.8.9      HCI_LE_Set_Advertising_Enable
  { .OCF = 0x0b, .isret = 0, .coistart = 201, .numcoi = 4 }, //   9   7.8.10     HCI_LE_Set_Scan_Parameters
  { .OCF = 0x0c, .isret = 0, .coistart = 100, .numcoi = 3 }, //  10   7.8.11     HCI_LE_Set_Scan_Enable
  { .OCF = 0x0d, .isret = 0, .coistart =  41, .numcoi = 4 }, //  11   7.8.12     HCI_LE_Create_Connection
  { .OCF = 0x0f, .isret = 1, .coistart =   5, .numcoi = 1 }, //  12   7.8.14     HCI_LE_Read_Filter_Accept_List_Size
  { .OCF = 0x11, .isret = 0, .coistart =   0, .numcoi = 3 }, //  13   7.8.16     HCI_LE_Add_Device_To_Filter_Accept_List
  { .OCF = 0x12, .isret = 0, .coistart = 115, .numcoi = 3 }, //  14   7.8.17     HCI_LE_Remove_Device_From_Filter_Accept_List
  { .OCF = 0x13, .isret = 0, .coistart =  21, .numcoi = 3 }, //  15   7.8.18     HCI_LE_Connection_Update
  { .OCF = 0x14, .isret = 0, .coistart =  94, .numcoi = 3 }, //  16   7.8.19     HCI_LE_Set_Host_Channel_Classification
  { .OCF = 0x15, .isret = 1, .coistart = 106, .numcoi = 2 }, //  17   7.8.20     HCI_LE_Read_Channel_Map
  { .OCF = 0x17, .isret = 0, .coistart =  50, .numcoi = 5 }, //  18   7.8.22     HCI_LE_Encrypt
  { .OCF = 0x17, .isret = 1, .coistart =  55, .numcoi = 3 }, //  19   7.8.22     HCI_LE_Encrypt
  { .OCF = 0x18, .isret = 1, .coistart =  27, .numcoi = 1 }, //  20   7.8.23     HCI_LE_Rand
  { .OCF = 0x19, .isret = 0, .coistart =  45, .numcoi = 5 }, //  21   7.8.24     HCI_LE_Enable_Encryption
  { .OCF = 0x1a, .isret = 0, .coistart =  85, .numcoi = 5 }, //  22   7.8.25     HCI_LE_Long_Term_Key_Request_Reply
  { .OCF = 0x1c, .isret = 1, .coistart =   2, .numcoi = 1 }, //  23   7.8.27     HCI_LE_Read_Supported_States
  { .OCF = 0x1d, .isret = 0, .coistart =  11, .numcoi = 3 }, //  24   7.8.28     HCI_LE_Receiver_Test [v1]
  { .OCF = 0x1e, .isret = 0, .coistart =  17, .numcoi = 3 }, //  25   7.8.29     HCI_LE_Transmitter_Test [v1]
  { .OCF = 0x1f, .isret = 1, .coistart = 118, .numcoi = 2 }, //  26   7.8.30     HCI_LE_Test_End
  { .OCF = 0x20, .isret = 0, .coistart =  21, .numcoi = 3 }, //  27   7.8.31     HCI_LE_Remote_Connection_Parameter_Request_Reply
  { .OCF = 0x21, .isret = 0, .coistart =  17, .numcoi = 3 }, //  28   7.8.32     HCI_LE_Remote_Connection_Parameter_Request_Negative_Reply
  { .OCF = 0x22, .isret = 0, .coistart =  97, .numcoi = 3 }, //  29   7.8.33     HCI_LE_Set_Data_Length
  { .OCF = 0x23, .isret = 1, .coistart = 122, .numcoi = 2 }, //  30   7.8.34     HCI_LE_Read_Suggested_Default_Data_Length
  { .OCF = 0x24, .isret = 0, .coistart = 154, .numcoi = 3 }, //  31   7.8.35     HCI_LE_Write_Suggested_Default_Data_Length
  { .OCF = 0x26, .isret = 0, .coistart =  77, .numcoi = 5 }, //  32   7.8.37     HCI_LE_Generate_DHKey [v1]
  { .OCF = 0x27, .isret = 0, .coistart =   6, .numcoi = 5 }, //  33   7.8.38     HCI_LE_Add_Device_To_Resolving_List
  { .OCF = 0x28, .isret = 0, .coistart = 115, .numcoi = 3 }, //  34   7.8.39     HCI_LE_Remove_Device_From_Resolving_List
  { .OCF = 0x2a, .isret = 1, .coistart =   5, .numcoi = 1 }, //  35   7.8.41     HCI_LE_Read_Resolving_List_Size
  { .OCF = 0x2b, .isret = 0, .coistart = 115, .numcoi = 3 }, //  36   7.8.42     HCI_LE_Read_Peer_Resolvable_Address
  { .OCF = 0x2b, .isret = 1, .coistart =  43, .numcoi = 1 }, //  37   7.8.42     HCI_LE_Read_Peer_Resolvable_Address
  { .OCF = 0x2c, .isret = 0, .coistart = 115, .numcoi = 3 }, //  38   7.8.43     HCI_LE_Read_Local_Resolvable_Address
  { .OCF = 0x2c, .isret = 1, .coistart =  43, .numcoi = 1 }, //  39   7.8.43     HCI_LE_Read_Local_Resolvable_Address
  { .OCF = 0x2d, .isret = 0, .coistart =  11, .numcoi = 3 }, //  40   7.8.44     HCI_LE_Set_Address_Resolution_Enable
  { .OCF = 0x2e, .isret = 0, .coistart = 100, .numcoi = 3 }, //  41   7.8.45     HCI_LE_Set_Resolvable_Private_Address_Timeout
  { .OCF = 0x2f, .isret = 1, .coistart = 120, .numcoi = 2 }, //  42   7.8.46     HCI_LE_Read_Maximum_Data_Length
  { .OCF = 0x30, .isret = 1, .coistart = 122, .numcoi = 2 }, //  43   7.8.47     HCI_LE_Read_PHY
  { .OCF = 0x31, .isret = 0, .coistart =  17, .numcoi = 3 }, //  44   7.8.48     HCI_LE_Set_Default_PHY
  { .OCF = 0x32, .isret = 0, .coistart = 220, .numcoi = 4 }, //  45   7.8.49     HCI_LE_Set_PHY
  { .OCF = 0x33, .isret = 0, .coistart =  17, .numcoi = 3 }, //  46   7.8.28     HCI_LE_Receiver_Test [v2]
  { .OCF = 0x34, .isret = 0, .coistart = 154, .numcoi = 3 }, //  47   7.8.29     HCI_LE_Transmitter_Test [v2]
  { .OCF = 0x35, .isret = 0, .coistart = 115, .numcoi = 3 }, //  48   7.8.52     HCI_LE_Set_Advertising_Set_Random_Address
  { .OCF = 0x36, .isret = 0, .coistart = 181, .numcoi = 6 }, //  49   7.8.53     HCI_LE_Set_Extended_Advertising_Parameters [v1]
  { .OCF = 0x36, .isret = 1, .coistart =   5, .numcoi = 1 }, //  50   7.8.53     HCI_LE_Set_Extended_Advertising_Parameters [v1]
  { .OCF = 0x37, .isret = 0, .coistart = 154, .numcoi = 4 }, //  51   7.8.54     HCI_LE_Set_Extended_Advertising_Data
  { .OCF = 0x38, .isret = 0, .coistart = 154, .numcoi = 4 }, //  52   7.8.55     HCI_LE_Set_Extended_Scan_Response_Data
  { .OCF = 0x39, .isret = 0, .coistart = 168, .numcoi = 7 }, //  53   7.8.56     HCI_LE_Set_Extended_Advertising_Enable
  { .OCF = 0x3a, .isret = 1, .coistart = 118, .numcoi = 2 }, //  54   7.8.57     HCI_LE_Read_Maximum_Advertising_Data_Length
  { .OCF = 0x3b, .isret = 1, .coistart =   5, .numcoi = 1 }, //  55   7.8.58     HCI_LE_Read_Number_of_Supported_Advertising_Sets
  { .OCF = 0x3c, .isret = 0, .coistart =  11, .numcoi = 3 }, //  56   7.8.59     HCI_LE_Remove_Advertising_Set
  { .OCF = 0x3e, .isret = 0, .coistart = 201, .numcoi = 4 }, //  57   7.8.61     HCI_LE_Set_Periodic_Advertising_Parameters [v1]
  { .OCF = 0x3f, .isret = 0, .coistart = 193, .numcoi = 4 }, //  58   7.8.62     HCI_LE_Set_Periodic_Advertising_Data
  { .OCF = 0x40, .isret = 0, .coistart = 100, .numcoi = 3 }, //  59   7.8.63     HCI_LE_Set_Periodic_Advertising_Enable
  { .OCF = 0x41, .isret = 0, .coistart = 187, .numcoi = 6 }, //  60   7.8.64     HCI_LE_Set_Extended_Scan_Parameters
  { .OCF = 0x42, .isret = 0, .coistart =  97, .numcoi = 3 }, //  61   7.8.65     HCI_LE_Set_Extended_Scan_Enable
  { .OCF = 0x43, .isret = 0, .coistart =  66, .numcoi = 6 }, //  62   7.8.66     HCI_LE_Extended_Create_Connection [v1]
  { .OCF = 0x44, .isret = 0, .coistart =  90, .numcoi = 4 }, //  63   7.8.67     HCI_LE_Periodic_Advertising_Create_Sync
  { .OCF = 0x46, .isret = 0, .coistart = 100, .numcoi = 3 }, //  64   7.8.69     HCI_LE_Periodic_Advertising_Terminate_Sync
  { .OCF = 0x47, .isret = 0, .coistart =   3, .numcoi = 3 }, //  65   7.8.70     HCI_LE_Add_Device_To_Periodic_Advertiser_List
  { .OCF = 0x48, .isret = 0, .coistart =   3, .numcoi = 3 }, //  66   7.8.71     HCI_LE_Remove_Device_From_Periodic_Advertiser_List
  { .OCF = 0x4a, .isret = 1, .coistart =   5, .numcoi = 1 }, //  67   7.8.73     HCI_LE_Read_Periodic_Advertiser_List_Size
  { .OCF = 0x4b, .isret = 1, .coistart =  31, .numcoi = 1 }, //  68   7.8.74     HCI_LE_Read_Transmit_Power
  { .OCF = 0x4c, .isret = 1, .coistart = 122, .numcoi = 2 }, //  69   7.8.75     HCI_LE_Read_RF_Path_Compensation
  { .OCF = 0x4d, .isret = 0, .coistart = 154, .numcoi = 3 }, //  70   7.8.76     HCI_LE_Write_RF_Path_Compensation
  { .OCF = 0x4e, .isret = 0, .coistart =   3, .numcoi = 3 }, //  71   7.8.77     HCI_LE_Set_Privacy_Mode
  { .OCF = 0x4f, .isret = 0, .coistart = 124, .numcoi = 5 }, //  72   7.8.28     HCI_LE_Receiver_Test [v3]
  { .OCF = 0x50, .isret = 0, .coistart = 124, .numcoi = 5 }, //  73   7.8.29     HCI_LE_Transmitter_Test [v3]
  { .OCF = 0x51, .isret = 0, .coistart = 149, .numcoi = 5 }, //  74   7.8.80     HCI_LE_Set_Connectionless_CTE_Transmit_Parameters
  { .OCF = 0x52, .isret = 0, .coistart = 100, .numcoi = 3 }, //  75   7.8.81     HCI_LE_Set_Connectionless_CTE_Transmit_Enable
  { .OCF = 0x53, .isret = 0, .coistart = 159, .numcoi = 5 }, //  76   7.8.82     HCI_LE_Set_Connectionless_IQ_Sampling_Enable
  { .OCF = 0x53, .isret = 1, .coistart = 118, .numcoi = 2 }, //  77   7.8.82     HCI_LE_Set_Connectionless_IQ_Sampling_Enable
  { .OCF = 0x54, .isret = 0, .coistart = 149, .numcoi = 5 }, //  78   7.8.83     HCI_LE_Set_Connection_CTE_Receive_Parameters
  { .OCF = 0x55, .isret = 0, .coistart = 154, .numcoi = 5 }, //  79   7.8.84     HCI_LE_Set_Connection_CTE_Transmit_Parameters
  { .OCF = 0x56, .isret = 0, .coistart =  17, .numcoi = 4 }, //  80   7.8.85     HCI_LE_Connection_CTE_Request_Enable
  { .OCF = 0x57, .isret = 0, .coistart =  17, .numcoi = 3 }, //  81   7.8.86     HCI_LE_Connection_CTE_Response_Enable
  { .OCF = 0x58, .isret = 1, .coistart =  33, .numcoi = 1 }, //  82   7.8.87     HCI_LE_Read_Antenna_Information
  { .OCF = 0x59, .isret = 0, .coistart =  17, .numcoi = 3 }, //  83   7.8.88     HCI_LE_Set_Periodic_Advertising_Receive_Enable
  { .OCF = 0x5a, .isret = 0, .coistart =  97, .numcoi = 3 }, //  84   7.8.89     HCI_LE_Periodic_Advertising_Sync_Transfer
  { .OCF = 0x5b, .isret = 0, .coistart =  94, .numcoi = 3 }, //  85   7.8.90     HCI_LE_Periodic_Advertising_Set_Info_Transfer
  { .OCF = 0x5c, .isret = 0, .coistart = 216, .numcoi = 4 }, //  86   7.8.91     HCI_LE_Set_Periodic_Advertising_Sync_Transfer_Parameters
  { .OCF = 0x5d, .isret = 0, .coistart = 164, .numcoi = 4 }, //  87   7.8.92     HCI_LE_Set_Default_Periodic_Advertising_Sync_Transfer_Parameters
  { .OCF = 0x5e, .isret = 0, .coistart =  72, .numcoi = 5 }, //  88   7.8.37     HCI_LE_Generate_DHKey [v2]
  { .OCF = 0x5f, .isret = 0, .coistart =  11, .numcoi = 3 }, //  89   7.8.94     HCI_LE_Modify_Sleep_Clock_Accuracy
  { .OCF = 0x60, .isret = 1, .coistart = 103, .numcoi = 3 }, //  90   7.8.2      HCI_LE_Read_Buffer_Size [v2]
  { .OCF = 0x61, .isret = 1, .coistart = 113, .numcoi = 2 }, //  91   7.8.96     HCI_LE_Read_ISO_TX_Sync
  { .OCF = 0x62, .isret = 0, .coistart = 132, .numcoi = 7 }, //  92   7.8.97     HCI_LE_Set_CIG_Parameters
  { .OCF = 0x62, .isret = 1, .coistart = 139, .numcoi = 3 }, //  93   7.8.97     HCI_LE_Set_CIG_Parameters
  { .OCF = 0x63, .isret = 0, .coistart = 142, .numcoi = 7 }, //  94   7.8.98     HCI_LE_Set_CIG_Parameters_Test
  { .OCF = 0x63, .isret = 1, .coistart = 139, .numcoi = 3 }, //  95   7.8.98     HCI_LE_Set_CIG_Parameters_Test
  { .OCF = 0x64, .isret = 0, .coistart =  35, .numcoi = 6 }, //  96   7.8.99     HCI_LE_Create_CIS
  { .OCF = 0x65, .isret = 0, .coistart =  11, .numcoi = 3 }, //  97   7.8.100    HCI_LE_Remove_CIG
  { .OCF = 0x65, .isret = 1, .coistart =   5, .numcoi = 1 }, //  98   7.8.100    HCI_LE_Remove_CIG
  { .OCF = 0x67, .isret = 0, .coistart =  17, .numcoi = 3 }, //  99   7.8.102    HCI_LE_Reject_CIS_Request
  { .OCF = 0x68, .isret = 0, .coistart =  24, .numcoi = 5 }, // 100   7.8.103    HCI_LE_Create_BIG
  { .OCF = 0x69, .isret = 0, .coistart =  29, .numcoi = 6 }, // 101   7.8.104    HCI_LE_Create_BIG_Test
  { .OCF = 0x6a, .isret = 0, .coistart = 100, .numcoi = 3 }, // 102   7.8.105    HCI_LE_Terminate_BIG
  { .OCF = 0x6b, .isret = 0, .coistart =  11, .numcoi = 6 }, // 103   7.8.106    HCI_LE_BIG_Create_Sync
  { .OCF = 0x6c, .isret = 0, .coistart =  11, .numcoi = 3 }, // 104   7.8.107    HCI_LE_BIG_Terminate_Sync
  { .OCF = 0x6c, .isret = 1, .coistart =   5, .numcoi = 1 }, // 105   7.8.107    HCI_LE_BIG_Terminate_Sync
  { .OCF = 0x6e, .isret = 0, .coistart = 224, .numcoi = 4 }, // 106   7.8.109    HCI_LE_Setup_ISO_Data_Path
  { .OCF = 0x6f, .isret = 0, .coistart =  17, .numcoi = 3 }, // 107   7.8.110    HCI_LE_Remove_ISO_Data_Path
  { .OCF = 0x70, .isret = 0, .coistart =  17, .numcoi = 3 }, // 108   7.8.111    HCI_LE_ISO_Transmit_Test
  { .OCF = 0x71, .isret = 0, .coistart =  17, .numcoi = 3 }, // 109   7.8.112    HCI_LE_ISO_Receive_Test
  { .OCF = 0x72, .isret = 1, .coistart =  82, .numcoi = 3 }, // 110   7.8.113    HCI_LE_ISO_Read_Test_Counters
  { .OCF = 0x73, .isret = 1, .coistart =  82, .numcoi = 3 }, // 111   7.8.114    HCI_LE_ISO_Test_End
  { .OCF = 0x74, .isret = 0, .coistart = 100, .numcoi = 3 }, // 112   7.8.115    HCI_LE_Set_Host_Feature
  { .OCF = 0x75, .isret = 1, .coistart = 108, .numcoi = 5 }, // 113   7.8.116    HCI_LE_Read_ISO_Link_Quality
  { .OCF = 0x76, .isret = 0, .coistart =  17, .numcoi = 3 }, // 114   7.8.117    HCI_LE_Enhanced_Read_Transmit_Power_Level
  { .OCF = 0x76, .isret = 1, .coistart =  58, .numcoi = 2 }, // 115   7.8.117    HCI_LE_Enhanced_Read_Transmit_Power_Level
  { .OCF = 0x77, .isret = 0, .coistart =  17, .numcoi = 3 }, // 116   7.8.118    HCI_LE_Read_Remote_Transmit_Power_Level
  { .OCF = 0x78, .isret = 0, .coistart =   3, .numcoi = 3 }, // 117   7.8.119    HCI_LE_Set_Path_Loss_Reporting_Parameters
  { .OCF = 0x79, .isret = 0, .coistart =  17, .numcoi = 3 }, // 118   7.8.120    HCI_LE_Set_Path_Loss_Reporting_Enable
  { .OCF = 0x7a, .isret = 0, .coistart = 154, .numcoi = 3 }, // 119   7.8.121    HCI_LE_Set_Transmit_Power_Reporting_Enable
  { .OCF = 0x7b, .isret = 0, .coistart = 228, .numcoi = 5 }, // 120   7.8.29     HCI_LE_Transmitter_Test [v4]
  { .OCF = 0x7c, .isret = 0, .coistart = 100, .numcoi = 3 }, // 121   7.8.122    HCI_LE_Set_Data_Related_Address_Changes
  { .OCF = 0x7d, .isret = 0, .coistart =  66, .numcoi = 3 }, // 122   7.8.123    HCI_LE_Set_Default_Subrate
  { .OCF = 0x7e, .isret = 0, .coistart =  60, .numcoi = 3 }, // 123   7.8.124    HCI_LE_Subrate_Request
  { .OCF = 0x7f, .isret = 0, .coistart = 175, .numcoi = 6 }, // 124   7.8.53     HCI_LE_Set_Extended_Advertising_Parameters [v2]
  { .OCF = 0x7f, .isret = 1, .coistart =   5, .numcoi = 1 }, // 125   7.8.53     HCI_LE_Set_Extended_Advertising_Parameters [v2]
  { .OCF = 0x82, .isret = 0, .coistart = 209, .numcoi = 7 }, // 126   7.8.125    HCI_LE_Set_Periodic_Advertising_Subevent_Data
  { .OCF = 0x82, .isret = 1, .coistart =   5, .numcoi = 1 }, // 127   7.8.125    HCI_LE_Set_Periodic_Advertising_Subevent_Data
  { .OCF = 0x83, .isret = 0, .coistart = 205, .numcoi = 4 }, // 128   7.8.126    HCI_LE_Set_Periodic_Advertising_Response_Data
  { .OCF = 0x83, .isret = 1, .coistart = 118, .numcoi = 2 }, // 129   7.8.126    HCI_LE_Set_Periodic_Advertising_Response_Data
  { .OCF = 0x84, .isret = 0, .coistart = 149, .numcoi = 5 }, // 130   7.8.127    HCI_LE_Set_Periodic_Sync_Subevent
  { .OCF = 0x84, .isret = 1, .coistart = 118, .numcoi = 2 }, // 131   7.8.127    HCI_LE_Set_Periodic_Sync_Subevent
  { .OCF = 0x85, .isret = 0, .coistart =  60, .numcoi = 6 }, // 132   7.8.66     HCI_LE_Extended_Create_Connection [v2]
  { .OCF = 0x86, .isret = 0, .coistart = 197, .numcoi = 4 }, // 133   7.8.61     HCI_LE_Set_Periodic_Advertising_Parameters [v2]
  { .OCF = 0x86, .isret = 1, .coistart =   5, .numcoi = 1 }, // 134   7.8.61     HCI_LE_Set_Periodic_Advertising_Parameters [v2]
};

static const CodecTab_t CodecTab_8 = {
  .numtab = 135, .numcoi = 233, .table = TE_8, .CoI = CoI_8
};

const ctab_t ctabs[9] = {
  & CodecTab_0, & CodecTab_1, & CodecTab_2,  & CodecTab_3,
  & CodecTab_4, & CodecTab_5, & CodecTab_6,  & CodecTab_7,
  & CodecTab_8
};

