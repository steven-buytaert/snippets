// Copyright 2024 Steven Buytaert

// Generated codec tables from hci-spec2src-5.4.h

#include <stddef.h>
#include <hci-types-5.4.h>
#include <codec-int.h>

#define INFO(SECTION, OFFSET)

static const CoI_t CoI_0[203] = {
  INFO(    7.7.14 ,   0) { .num =  6 }, 
  INFO(    7.7.17 ,   1) { .num =  3, .skip = 1 }, { .num =  2 }, 
  INFO(     7.7.6 ,   3) { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, { .num =  2 }, 
  INFO(    7.7.15 ,   6) { .num =  5, .skip = 1 }, { .num =  2 }, 
  INFO(     7.7.3 ,   8) { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, { .num = 10 }, 
  INFO(    7.7.28 ,  11) { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, { .num =  4 }, 
  INFO(     7.7.4 ,  14) { .num = 13 }, 
  INFO(    7.7.69 ,  15) { .num = 10, .skip = 2 }, { .num = 11 }, { .action = inlinefix, .inst = 1 }, 
  INFO(     7.7.5 ,  18) { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, 
  INFO(    7.7.38 ,  21) { .num = 15, .skip = 1 }, { .action = copyws, .arg = 1, .inst = 1 }, { .LSB = 0xf3 }, { .MSB = 0x00 }, 
  INFO(    7.7.32 ,  25) { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, { .num =  5, .skip = 1 }, { .action = copyws, .inst = 1 }, { .LSB = 0x10 }, { .MSB = 0x00 }, 
  INFO( 7.7.65.36 ,  31) { .num =  7 }, 
  INFO(     7.7.2 ,  32) { .num =  4 }, { .action = loop, .inst = 1 }, { .num =  7, .skip = 1 }, { .num =  5, .skip = 1 }, { .num =  2 }, { .action = endloop, .inst = 1 }, 
  INFO(    7.7.33 ,  38) { .num =  4 }, { .action = loop, .inst = 1 }, { .num = 11, .skip = 1 }, { .num =  3, .skip = 1 }, { .action = endloop, .inst = 1 }, 
  INFO(    7.7.22 ,  43) { .num =  9 }, 
  INFO( 7.7.65.19 ,  44) { .num = 12 }, 
  INFO(  7.7.65.2 ,  45) { .num =  5, .skip = 3 }, { .action = loop, .inst = 1 }, { .num =  9, .skip = 3 }, { .action = inlinefix, .arg = 1, .inst = 1 }, { .num =  1, .skip = 3 }, { .action = endloop, .inst = 1 }, 
  INFO( 7.7.65.18 ,  51) { .num =  9, .skip = 1 }, 
  INFO( 7.7.65.29 ,  52) { .num = 13, .skip = 1 }, { .num =  5, .skip = 1 }, { .action = inlinefix, .inst = 1 }, { .num =  2 }, 
  INFO( 7.7.65.16 ,  56) { .num =  6 }, 
  INFO( 7.7.65.34 ,  57) { .num = 13, .skip = 1 }, { .num =  5, .skip = 1 }, { .num =  5, .skip = 1 }, 
  INFO( 7.7.65.25 ,  60) { .num =  5, .skip = 1 }, { .action = copyws, .arg = 1, .inst = 1 }, { .LSB = 0x15 }, { .MSB = 0x00 }, { .num =  6 }, 
  INFO(  7.7.65.1 ,  65) { .num =  5, .skip = 1 }, { .action = copyws, .arg = 1, .inst = 1 }, { .LSB = 0x11 }, { .MSB = 0x00 }, 
  INFO( 7.7.65.22 ,  69) { .num =  1 }, { .action = loop, .inst = 1 }, { .num =  2 }, { .action = endloop, .inst = 1 }, 
  INFO(  7.7.65.3 ,  73) { .num =  5, .skip = 1 }, { .num =  8 }, 
  INFO( 7.7.65.21 ,  75) { .num =  7, .skip = 1 }, { .num =  9 }, { .action = loop, .inst = 1 }, { .num =  2 }, { .action = endloop, .inst = 1 }, 
  INFO( 7.7.65.27 ,  80) { .action = copyws, .arg = 1, .inst = 1 }, { .LSB = 0x11 }, { .MSB = 0x00 }, { .num =  5, .skip = 1 }, { .action = inlinefix, .inst = 1 }, { .num =  2 }, 
  INFO(  7.7.65.6 ,  86) { .num = 14 }, 
  INFO( 7.7.65.11 ,  87) { .num =  5 }, { .action = loop, .inst = 1 }, { .action = copyws, .inst = 1 }, { .LSB = 0x10 }, { .MSB = 0x00 }, { .action = endloop, .inst = 1 }, 
  INFO( 7.7.65.10 ,  93) { .num =  5, .skip = 1 }, { .action = copyws, .inst = 1 }, { .LSB = 0x20 }, { .MSB = 0x00 }, 
  INFO( 7.7.65.10 ,  97) { .num =  5, .skip = 1 }, { .action = copyws, .arg = 1, .inst = 1 }, { .LSB = 0x1d }, { .MSB = 0x00 }, 
  INFO( 7.7.65.13 , 101) { .num =  5, .skip = 3 }, { .action = loop, .inst = 1 }, { .num =  8 }, { .action = inlinefix, .arg = 1, .inst = 1 }, { .action = endloop, .inst = 1 }, 
  INFO(  7.7.65.9 , 106) { .action = copyws, .inst = 1 }, { .LSB = 0x25 }, { .MSB = 0x00 }, 
  INFO( 7.7.65.15 , 109) { .num =  9, .skip = 1 }, { .num =  5 }, { .action = inlinefix, .inst = 1 }, 
  INFO( 7.7.65.37 , 112) { .num =  8 }, { .action = loop, .inst = 1 }, { .num =  6, .skip = 2 }, { .action = inlinefix, .arg = 1, .inst = 1 }, { .action = endloop, .inst = 1 }, 
  INFO( 7.7.65.14 , 117) { .num =  5, .skip = 1 }, { .num = 11, .skip = 1 }, { .num =  7, .skip = 1 }, 
  INFO( 7.7.65.14 , 120) { .num =  5, .skip = 1 }, { .num = 11, .skip = 1 }, { .num =  3, .skip = 1 }, 
  INFO( 7.7.65.24 , 123) { .num =  5, .skip = 1 }, { .num = 15, .skip = 1 }, { .num =  7, .skip = 1 }, 
  INFO( 7.7.65.24 , 126) { .num =  5, .skip = 1 }, { .num = 15, .skip = 1 }, { .num =  3, .skip = 1 }, 
  INFO( 7.7.65.12 , 129) { .num =  5, .skip = 1 }, { .num =  4 }, 
  INFO(  7.7.65.8 , 131) { .action = copyws, .inst = 1 }, { .LSB = 0x45 }, { .MSB = 0x00 }, 
  INFO(  7.7.65.4 , 134) { .num =  5, .skip = 1 }, { .num = 10 }, 
  INFO( 7.7.65.31 , 136) { .num =  5, .skip = 1 }, { .num =  3, .skip = 1 }, 
  INFO( 7.7.65.33 , 138) { .num =  5, .skip = 1 }, { .num =  7, .skip = 1 }, 
  INFO(    7.7.24 , 140) { .action = copyws, .inst = 1 }, { .LSB = 0x1a }, { .MSB = 0x00 }, 
  INFO(    7.7.46 , 143) { .num =  3, .skip = 1 }, { .num =  4 }, 
  INFO(    7.7.25 , 145) { .num =  3 }, 
  INFO(    7.7.27 , 146) { .num =  3, .skip = 1 }, { .num =  3, .skip = 1 }, 
  INFO(    7.7.20 , 148) { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  2 }, 
  INFO(    7.7.59 , 152) { .num =  3, .skip = 1 }, { .num =  3, .skip = 1 }, { .action = loop, .inst = 1 }, { .num =  6 }, { .action = endloop, .inst = 1 }, 
  INFO(    7.7.19 , 157) { .num =  4 }, { .action = loop, .inst = 1 }, { .num =  4 }, { .action = endloop, .inst = 1 }, 
  INFO(    7.7.13 , 161) { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, { .num =  4, .skip = 2 }, { .action = copyws, .inst = 1 }, { .LSB = 0x10 }, { .MSB = 0x00 }, 
  INFO(    7.7.34 , 167) { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, { .num = 12 }, 
  INFO(    7.7.12 , 170) { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  4 }, 
  INFO(    7.7.50 , 174) { .action = copyws, .inst = 1 }, { .LSB = 0x11 }, { .MSB = 0x00 }, 
  INFO(     7.7.7 , 177) { .action = copyws, .inst = 1 }, { .LSB = 0x01 }, { .MSB = 0x01 }, 
  INFO(    7.7.21 , 180) { .num =  4 }, { .action = loop, .inst = 1 }, { .action = copyws, .inst = 1 }, { .LSB = 0x16 }, { .MSB = 0x00 }, { .action = endloop, .inst = 1 }, 
  INFO(    7.7.76 , 186) { .num =  3, .skip = 1 }, { .num =  8 }, 
  INFO(    7.7.68 , 188) { .num =  9, .skip = 2 }, { .num = 15, .skip = 1 }, { .num =  7, .skip = 1 }, 
  INFO(    7.7.36 , 191) { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, { .num =  8 }, 
  INFO(    7.7.35 , 194) { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, { .num = 11, .skip = 1 }, { .num =  5, .skip = 1 }, 
  INFO(    7.7.66 , 198) { .num =  3, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  6, .skip = 2 }, 
  INFO(    7.7.42 , 201) { .num =  9, .skip = 3 }, { .num =  4 }, 
};

static const TE_t TE_0[103] = {
  { .code = 0x01, .issub = 0, .coistart =  13, .numcoi = 1 }, //   0 7.7.1      p HCI_Inquiry_Complete
  { .code = 0x01, .issub = 1, .coistart =  65, .numcoi = 4 }, //   1 7.7.65.1     HCI_LE_Connection_Complete
  { .code = 0x02, .issub = 0, .coistart =  32, .numcoi = 6 }, //   2 7.7.2        HCI_Inquiry_Result
  { .code = 0x02, .issub = 1, .coistart =  45, .numcoi = 6 }, //   3 7.7.65.2     HCI_LE_Advertising_Report
  { .code = 0x03, .issub = 0, .coistart =   8, .numcoi = 3 }, //   4 7.7.3        HCI_Connection_Complete
  { .code = 0x03, .issub = 1, .coistart =  73, .numcoi = 2 }, //   5 7.7.65.3     HCI_LE_Connection_Update_Complete
  { .code = 0x04, .issub = 0, .coistart =  14, .numcoi = 1 }, //   6 7.7.4        HCI_Connection_Request
  { .code = 0x04, .issub = 1, .coistart = 134, .numcoi = 2 }, //   7 7.7.65.4     HCI_LE_Read_Remote_Features_Complete
  { .code = 0x05, .issub = 0, .coistart =  18, .numcoi = 3 }, //   8 7.7.5        HCI_Disconnection_Complete
  { .code = 0x05, .issub = 1, .coistart =  28, .numcoi = 3 }, //   9 7.7.65.5     HCI_LE_Long_Term_Key_Request
  { .code = 0x06, .issub = 0, .coistart =   3, .numcoi = 3 }, //  10 7.7.6      p HCI_Authentication_Complete
  { .code = 0x06, .issub = 1, .coistart =  86, .numcoi = 1 }, //  11 7.7.65.6     HCI_LE_Remote_Connection_Parameter_Request
  { .code = 0x07, .issub = 0, .coistart = 177, .numcoi = 3 }, //  12 7.7.7        HCI_Remote_Name_Request_Complete
  { .code = 0x07, .issub = 1, .coistart =  86, .numcoi = 1 }, //  13 7.7.65.7     HCI_LE_Data_Length_Change
  { .code = 0x08, .issub = 0, .coistart =  18, .numcoi = 3 }, //  14 7.7.8        HCI_Encryption_Change [v1]
  { .code = 0x08, .issub = 1, .coistart = 131, .numcoi = 3 }, //  15 7.7.65.8     HCI_LE_Read_Local_P-256_Public_Key_Complete
  { .code = 0x09, .issub = 0, .coistart =   3, .numcoi = 3 }, //  16 7.7.9      p HCI_Change_Connection_Link_Key_Complete
  { .code = 0x09, .issub = 1, .coistart = 106, .numcoi = 3 }, //  17 7.7.65.9     HCI_LE_Generate_DHKey_Complete
  { .code = 0x0a, .issub = 0, .coistart =  18, .numcoi = 3 }, //  18 7.7.10       HCI_Link_Key_Type_Changed
  { .code = 0x0a, .issub = 1, .coistart =  97, .numcoi = 4 }, //  19 7.7.65.10    HCI_LE_Enhanced_Connection_Complete [v1]
  { .code = 0x0b, .issub = 0, .coistart =   8, .numcoi = 3 }, //  20 7.7.11       HCI_Read_Remote_Supported_Features_Complete
  { .code = 0x0b, .issub = 1, .coistart =  87, .numcoi = 6 }, //  21 7.7.65.11    HCI_LE_Directed_Advertising_Report
  { .code = 0x0c, .issub = 0, .coistart = 170, .numcoi = 4 }, //  22 7.7.12       HCI_Read_Remote_Version_Information_Complete
  { .code = 0x0c, .issub = 1, .coistart = 129, .numcoi = 2 }, //  23 7.7.65.12    HCI_LE_PHY_Update_Complete
  { .code = 0x0d, .issub = 0, .coistart = 161, .numcoi = 6 }, //  24 7.7.13       HCI_QoS_Setup_Complete
  { .code = 0x0d, .issub = 1, .coistart = 101, .numcoi = 5 }, //  25 7.7.65.13    HCI_LE_Extended_Advertising_Report
  { .code = 0x0e, .issub = 0, .coistart =   0, .numcoi = 1 }, //  26 7.7.14       HCI_Command_Complete
  { .code = 0x0e, .issub = 1, .coistart = 120, .numcoi = 3 }, //  27 7.7.65.14    HCI_LE_Periodic_Advertising_Sync_Established [v1]
  { .code = 0x0f, .issub = 0, .coistart =   6, .numcoi = 2 }, //  28 7.7.15       HCI_Command_Status
  { .code = 0x0f, .issub = 1, .coistart =  16, .numcoi = 2 }, //  29 7.7.65.15    HCI_LE_Periodic_Advertising_Report [v1]
  { .code = 0x10, .issub = 0, .coistart =  13, .numcoi = 1 }, //  30 7.7.16       HCI_Hardware_Error
  { .code = 0x10, .issub = 1, .coistart =  56, .numcoi = 1 }, //  31 7.7.65.16    HCI_LE_Periodic_Advertising_Sync_Lost
  { .code = 0x11, .issub = 0, .coistart =   1, .numcoi = 2 }, //  32 7.7.17       HCI_Flush_Occurred
  { .code = 0x11, .issub = 1, .coistart =  13, .numcoi = 1 }, //  33 7.7.65.17    HCI_LE_Scan_Timeout
  { .code = 0x12, .issub = 0, .coistart =  10, .numcoi = 1 }, //  34 7.7.18       HCI_Role_Change
  { .code = 0x12, .issub = 1, .coistart =  51, .numcoi = 1 }, //  35 7.7.65.18    HCI_LE_Advertising_Set_Terminated
  { .code = 0x13, .issub = 0, .coistart = 157, .numcoi = 4 }, //  36 7.7.19       HCI_Number_Of_Completed_Packets
  { .code = 0x13, .issub = 1, .coistart =  44, .numcoi = 1 }, //  37 7.7.65.19    HCI_LE_Scan_Request_Received
  { .code = 0x14, .issub = 0, .coistart = 148, .numcoi = 4 }, //  38 7.7.20       HCI_Mode_Change
  { .code = 0x14, .issub = 1, .coistart =  34, .numcoi = 1 }, //  39 7.7.65.20    HCI_LE_Channel_Selection_Algorithm
  { .code = 0x15, .issub = 0, .coistart = 180, .numcoi = 6 }, //  40 7.7.21       HCI_Return_Link_Keys
  { .code = 0x15, .issub = 1, .coistart =  75, .numcoi = 5 }, //  41 7.7.65.21    HCI_LE_Connectionless_IQ_Report
  { .code = 0x16, .issub = 0, .coistart =  43, .numcoi = 1 }, //  42 7.7.22       HCI_PIN_Code_Request
  { .code = 0x16, .issub = 1, .coistart =  69, .numcoi = 4 }, //  43 7.7.65.22    HCI_LE_Connection_IQ_Report
  { .code = 0x17, .issub = 0, .coistart =  43, .numcoi = 1 }, //  44 7.7.23       HCI_Link_Key_Request
  { .code = 0x17, .issub = 1, .coistart =   6, .numcoi = 2 }, //  45 7.7.65.23    HCI_LE_CTE_Request_Failed
  { .code = 0x18, .issub = 0, .coistart = 140, .numcoi = 3 }, //  46 7.7.24       HCI_Link_Key_Notification
  { .code = 0x18, .issub = 1, .coistart = 126, .numcoi = 3 }, //  47 7.7.65.24    HCI_LE_Periodic_Advertising_Sync_Transfer_Received [v1]
  { .code = 0x19, .issub = 0, .coistart = 145, .numcoi = 1 }, //  48 7.7.25       HCI_Loopback_Command
  { .code = 0x19, .issub = 1, .coistart =  60, .numcoi = 5 }, //  49 7.7.65.25    HCI_LE_CIS_Established
  { .code = 0x1a, .issub = 0, .coistart =  13, .numcoi = 1 }, //  50 7.7.26       HCI_Data_Buffer_Overflow
  { .code = 0x1a, .issub = 1, .coistart =  10, .numcoi = 1 }, //  51 7.7.65.26    HCI_LE_CIS_Request
  { .code = 0x1b, .issub = 0, .coistart = 146, .numcoi = 2 }, //  52 7.7.27       HCI_Max_Slots_Change
  { .code = 0x1b, .issub = 1, .coistart =  80, .numcoi = 6 }, //  53 7.7.65.27    HCI_LE_Create_BIG_Complete
  { .code = 0x1c, .issub = 0, .coistart =  11, .numcoi = 3 }, //  54 7.7.28       HCI_Read_Clock_Offset_Complete
  { .code = 0x1c, .issub = 1, .coistart =  56, .numcoi = 1 }, //  55 7.7.65.28    HCI_LE_Terminate_BIG_Complete
  { .code = 0x1d, .issub = 0, .coistart =  11, .numcoi = 3 }, //  56 7.7.29       HCI_Connection_Packet_Type_Changed
  { .code = 0x1d, .issub = 1, .coistart =  52, .numcoi = 4 }, //  57 7.7.65.29    HCI_LE_BIG_Sync_Established
  { .code = 0x1e, .issub = 0, .coistart =   1, .numcoi = 2 }, //  58 7.7.30       HCI_QoS_Violation
  { .code = 0x1e, .issub = 1, .coistart =  56, .numcoi = 1 }, //  59 7.7.65.30    HCI_LE_BIG_Sync_Lost
  { .code = 0x1f, .issub = 1, .coistart = 136, .numcoi = 2 }, //  60 7.7.65.31    HCI_LE_Request_Peer_SCA_Complete
  { .code = 0x20, .issub = 0, .coistart =  10, .numcoi = 1 }, //  61 7.7.31       HCI_Page_Scan_Repetition_Mode_Change
  { .code = 0x20, .issub = 1, .coistart =  74, .numcoi = 1 }, //  62 7.7.65.32    HCI_LE_Path_Loss_Threshold
  { .code = 0x21, .issub = 0, .coistart =  25, .numcoi = 6 }, //  63 7.7.32       HCI_Flow_Specification_Complete
  { .code = 0x21, .issub = 1, .coistart = 138, .numcoi = 2 }, //  64 7.7.65.33    HCI_LE_Transmit_Power_Reporting
  { .code = 0x22, .issub = 0, .coistart =  38, .numcoi = 5 }, //  65 7.7.33       HCI_Inquiry_Result_with_RSSI
  { .code = 0x22, .issub = 1, .coistart =  57, .numcoi = 3 }, //  66 7.7.65.34    HCI_LE_BIGInfo_Advertising_Report
  { .code = 0x23, .issub = 0, .coistart = 167, .numcoi = 3 }, //  67 7.7.34       HCI_Read_Remote_Extended_Features_Complete
  { .code = 0x23, .issub = 1, .coistart = 134, .numcoi = 2 }, //  68 7.7.65.35    HCI_LE_Subrate_Change
  { .code = 0x24, .issub = 1, .coistart = 117, .numcoi = 3 }, //  69 7.7.65.14    HCI_LE_Periodic_Advertising_Sync_Established [v2]
  { .code = 0x25, .issub = 1, .coistart = 109, .numcoi = 3 }, //  70 7.7.65.15    HCI_LE_Periodic_Advertising_Report [v2]
  { .code = 0x26, .issub = 1, .coistart = 123, .numcoi = 3 }, //  71 7.7.65.24    HCI_LE_Periodic_Advertising_Sync_Transfer_Received [v2]
  { .code = 0x27, .issub = 1, .coistart =  31, .numcoi = 1 }, //  72 7.7.65.36    HCI_LE_Periodic_Advertising_Subevent_Data_Request
  { .code = 0x28, .issub = 1, .coistart = 112, .numcoi = 5 }, //  73 7.7.65.37    HCI_LE_Periodic_Advertising_Response_Report
  { .code = 0x29, .issub = 1, .coistart =  93, .numcoi = 4 }, //  74 7.7.65.10    HCI_LE_Enhanced_Connection_Complete [v2]
  { .code = 0x2c, .issub = 0, .coistart = 194, .numcoi = 4 }, //  75 7.7.35       HCI_Synchronous_Connection_Complete
  { .code = 0x2d, .issub = 0, .coistart = 191, .numcoi = 3 }, //  76 7.7.36       HCI_Synchronous_Connection_Changed
  { .code = 0x2e, .issub = 0, .coistart =   8, .numcoi = 3 }, //  77 7.7.37       HCI_Sniff_Subrating
  { .code = 0x2f, .issub = 0, .coistart =  21, .numcoi = 4 }, //  78 7.7.38       HCI_Extended_Inquiry_Result
  { .code = 0x30, .issub = 0, .coistart =   3, .numcoi = 3 }, //  79 7.7.39     p HCI_Encryption_Key_Refresh_Complete
  { .code = 0x31, .issub = 0, .coistart =  43, .numcoi = 1 }, //  80 7.7.40       HCI_IO_Capability_Request
  { .code = 0x32, .issub = 0, .coistart =  44, .numcoi = 1 }, //  81 7.7.41       HCI_IO_Capability_Response
  { .code = 0x33, .issub = 0, .coistart = 201, .numcoi = 2 }, //  82 7.7.42       HCI_User_Confirmation_Request
  { .code = 0x34, .issub = 0, .coistart =  43, .numcoi = 1 }, //  83 7.7.43       HCI_User_Passkey_Request
  { .code = 0x35, .issub = 0, .coistart =  43, .numcoi = 1 }, //  84 7.7.44       HCI_Remote_OOB_Data_Request
  { .code = 0x36, .issub = 0, .coistart =  10, .numcoi = 1 }, //  85 7.7.45     p HCI_Simple_Pairing_Complete
  { .code = 0x38, .issub = 0, .coistart = 143, .numcoi = 2 }, //  86 7.7.46       HCI_Link_Supervision_Timeout_Changed
  { .code = 0x39, .issub = 0, .coistart =   1, .numcoi = 2 }, //  87 7.7.47       HCI_Enhanced_Flush_Complete
  { .code = 0x3b, .issub = 0, .coistart = 201, .numcoi = 2 }, //  88 7.7.48       HCI_User_Passkey_Notification
  { .code = 0x3c, .issub = 0, .coistart =  10, .numcoi = 1 }, //  89 7.7.49       HCI_Keypress_Notification
  { .code = 0x3d, .issub = 0, .coistart = 174, .numcoi = 3 }, //  90 7.7.50       HCI_Remote_Host_Supported_Features_Notification
  { .code = 0x48, .issub = 0, .coistart = 152, .numcoi = 5 }, //  91 7.7.59       HCI_Number_Of_Completed_Data_Blocks
  { .code = 0x4e, .issub = 0, .coistart = 198, .numcoi = 3 }, //  92 7.7.66       HCI_Triggered_Clock_Capture
  { .code = 0x4f, .issub = 0, .coistart =  13, .numcoi = 1 }, //  93 7.7.67     p HCI_Synchronization_Train_Complete
  { .code = 0x50, .issub = 0, .coistart = 188, .numcoi = 3 }, //  94 7.7.68       HCI_Synchronization_Train_Received
  { .code = 0x51, .issub = 0, .coistart =  15, .numcoi = 3 }, //  95 7.7.69       HCI_Connectionless_Peripheral_Broadcast_Receive
  { .code = 0x52, .issub = 0, .coistart =  10, .numcoi = 1 }, //  96 7.7.70       HCI_Connectionless_Peripheral_Broadcast_Timeout
  { .code = 0x53, .issub = 0, .coistart =  10, .numcoi = 1 }, //  97 7.7.71     p HCI_Truncated_Page_Complete
  { .code = 0x55, .issub = 0, .coistart =  14, .numcoi = 1 }, //  98 7.7.73       HCI_Connectionless_Peripheral_Broadcast_Channel_Map_Change
  { .code = 0x56, .issub = 0, .coistart =  31, .numcoi = 1 }, //  99 7.7.74       HCI_Inquiry_Response_Notification
  { .code = 0x57, .issub = 0, .coistart =   1, .numcoi = 2 }, // 100 7.7.75     p HCI_Authenticated_Payload_Timeout_Expired
  { .code = 0x58, .issub = 0, .coistart = 186, .numcoi = 2 }, // 101 7.7.76       HCI_SAM_Status_Change
  { .code = 0x59, .issub = 0, .coistart =  11, .numcoi = 3 }, // 102 7.7.8        HCI_Encryption_Change [v2]
};

static const CodecTab_t CodecTab_0 = {
  .numtab = 103, .numcoi = 203, .table = TE_0, .CoI = CoI_0
};

static const CoI_t CoI_1[102] = {
  INFO(     7.1.8 ,   0) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  7, .skip = 1 }, 
  INFO(    7.1.27 ,   3) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .action = copyws, .arg = 1, .inst = 1 }, { .LSB = 0x13 }, { .MSB = 0x00 }, { .num =  2, .skip = 2 }, 
  INFO(    7.1.15 ,   9) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  2 }, 
  INFO(    7.1.14 ,  12) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  4 }, 
  INFO(     7.1.7 ,  15) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  6 }, 
  INFO(     7.1.7 ,  18) { .num = 13, .skip = 1 }, 
  INFO(     7.1.5 ,  19) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num = 13, .skip = 1 }, 
  INFO(     7.1.6 ,  22) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  3, .skip = 1 }, 
  INFO(    7.1.46 ,  25) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .action = copyws, .arg = 2, .inst = 1 }, { .LSB = 0x1c }, { .MSB = 0x00 }, { .action = copyws, .arg = 1, .inst = 1 }, { .LSB = 0x23 }, { .MSB = 0x00 }, 
  INFO(    7.1.45 ,  33) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .action = copyws, .arg = 2, .inst = 1 }, { .LSB = 0x18 }, { .MSB = 0x00 }, { .action = copyws, .arg = 1, .inst = 1 }, { .LSB = 0x23 }, { .MSB = 0x00 }, 
  INFO(     7.1.1 ,  41) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  5, .skip = 1 }, 
  INFO(     7.1.9 ,  44) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  7, .skip = 1 }, 
  INFO(     7.1.3 ,  47) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  9, .skip = 1 }, 
  INFO(    7.1.10 ,  50) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .action = copyws, .inst = 1 }, { .LSB = 0x16 }, { .MSB = 0x00 }, 
  INFO(    7.1.18 ,  55) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, 
  INFO(    7.1.12 ,  58) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .action = copyws, .arg = 1, .inst = 1 }, { .LSB = 0x17 }, { .MSB = 0x00 }, 
  INFO(    7.1.25 ,  63) { .num =  7, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  4 }, 
  INFO(    7.1.52 ,  66) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num = 12 }, 
  INFO(    7.1.19 ,  69) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num = 10 }, 
  INFO(    7.1.34 ,  72) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .action = copyws, .inst = 1 }, { .LSB = 0x26 }, { .MSB = 0x00 }, 
  INFO(    7.1.53 ,  77) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .action = copyws, .inst = 1 }, { .LSB = 0x46 }, { .MSB = 0x00 }, 
  INFO(    7.1.49 ,  82) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  8 }, 
  INFO(    7.1.50 ,  86) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .action = copyws, .arg = 2, .inst = 1 }, { .LSB = 0x22 }, { .MSB = 0x00 }, 
  INFO(    7.1.26 ,  91) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num = 15, .skip = 1 }, { .num =  2, .skip = 2 }, 
  INFO(    7.1.47 ,  95) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  7, .skip = 1 }, { .num =  2 }, 
  INFO(    7.1.32 ,  99) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num = 10, .skip = 2 }, 
};

static const TE_t TE_1[67] = {
  { .OCF = 0x01, .isret = 0, .coistart =  41, .numcoi = 3 }, //   0 7.1.1        HCI_Inquiry
  { .OCF = 0x02, .isret = 0, .coistart =   3, .numcoi = 2 }, //   1 7.1.2      s HCI_Inquiry_Cancel
  { .OCF = 0x02, .isret = 1, .coistart =   2, .numcoi = 1 }, //   2 7.1.2      p HCI_Inquiry_Cancel
  { .OCF = 0x03, .isret = 0, .coistart =  47, .numcoi = 3 }, //   3 7.1.3        HCI_Periodic_Inquiry_Mode
  { .OCF = 0x03, .isret = 1, .coistart =   2, .numcoi = 1 }, //   4 7.1.3      p HCI_Periodic_Inquiry_Mode
  { .OCF = 0x04, .isret = 0, .coistart =   3, .numcoi = 2 }, //   5 7.1.4      s HCI_Exit_Periodic_Inquiry_Mode
  { .OCF = 0x04, .isret = 1, .coistart =   2, .numcoi = 1 }, //   6 7.1.4      p HCI_Exit_Periodic_Inquiry_Mode
  { .OCF = 0x05, .isret = 0, .coistart =  19, .numcoi = 3 }, //   7 7.1.5        HCI_Create_Connection
  { .OCF = 0x06, .isret = 0, .coistart =  22, .numcoi = 3 }, //   8 7.1.6        HCI_Disconnect
  { .OCF = 0x08, .isret = 0, .coistart =  15, .numcoi = 3 }, //   9 7.1.7        HCI_Create_Connection_Cancel
  { .OCF = 0x08, .isret = 1, .coistart =  18, .numcoi = 1 }, //  10 7.1.7      p HCI_Create_Connection_Cancel
  { .OCF = 0x09, .isret = 0, .coistart =   0, .numcoi = 3 }, //  11 7.1.8        HCI_Accept_Connection_Request
  { .OCF = 0x0a, .isret = 0, .coistart =  44, .numcoi = 3 }, //  12 7.1.9        HCI_Reject_Connection_Request
  { .OCF = 0x0b, .isret = 0, .coistart =  50, .numcoi = 5 }, //  13 7.1.10       HCI_Link_Key_Request_Reply
  { .OCF = 0x0b, .isret = 1, .coistart =  18, .numcoi = 1 }, //  14 7.1.10     p HCI_Link_Key_Request_Reply
  { .OCF = 0x0c, .isret = 0, .coistart =  15, .numcoi = 3 }, //  15 7.1.11       HCI_Link_Key_Request_Negative_Reply
  { .OCF = 0x0c, .isret = 1, .coistart =  18, .numcoi = 1 }, //  16 7.1.11     p HCI_Link_Key_Request_Negative_Reply
  { .OCF = 0x0d, .isret = 0, .coistart =  58, .numcoi = 5 }, //  17 7.1.12       HCI_PIN_Code_Request_Reply
  { .OCF = 0x0d, .isret = 1, .coistart =  18, .numcoi = 1 }, //  18 7.1.12     p HCI_PIN_Code_Request_Reply
  { .OCF = 0x0e, .isret = 0, .coistart =  15, .numcoi = 3 }, //  19 7.1.13       HCI_PIN_Code_Request_Negative_Reply
  { .OCF = 0x0e, .isret = 1, .coistart =  18, .numcoi = 1 }, //  20 7.1.13     p HCI_PIN_Code_Request_Negative_Reply
  { .OCF = 0x0f, .isret = 0, .coistart =  12, .numcoi = 3 }, //  21 7.1.14       HCI_Change_Connection_Packet_Type
  { .OCF = 0x11, .isret = 0, .coistart =   9, .numcoi = 3 }, //  22 7.1.15     p HCI_Authentication_Requested
  { .OCF = 0x13, .isret = 0, .coistart =  22, .numcoi = 3 }, //  23 7.1.16       HCI_Set_Connection_Encryption
  { .OCF = 0x15, .isret = 0, .coistart =   9, .numcoi = 3 }, //  24 7.1.17     p HCI_Change_Connection_Link_Key
  { .OCF = 0x17, .isret = 0, .coistart =  55, .numcoi = 3 }, //  25 7.1.18       HCI_Link_Key_Selection
  { .OCF = 0x19, .isret = 0, .coistart =  69, .numcoi = 3 }, //  26 7.1.19       HCI_Remote_Name_Request
  { .OCF = 0x1a, .isret = 0, .coistart =  15, .numcoi = 3 }, //  27 7.1.20       HCI_Remote_Name_Request_Cancel
  { .OCF = 0x1a, .isret = 1, .coistart =  18, .numcoi = 1 }, //  28 7.1.20     p HCI_Remote_Name_Request_Cancel
  { .OCF = 0x1b, .isret = 0, .coistart =   9, .numcoi = 3 }, //  29 7.1.21     p HCI_Read_Remote_Supported_Features
  { .OCF = 0x1c, .isret = 0, .coistart =  22, .numcoi = 3 }, //  30 7.1.22       HCI_Read_Remote_Extended_Features
  { .OCF = 0x1d, .isret = 0, .coistart =   9, .numcoi = 3 }, //  31 7.1.23     p HCI_Read_Remote_Version_Information
  { .OCF = 0x1f, .isret = 0, .coistart =   9, .numcoi = 3 }, //  32 7.1.24     p HCI_Read_Clock_Offset
  { .OCF = 0x20, .isret = 0, .coistart =   9, .numcoi = 3 }, //  33 7.1.25     p HCI_Read_LMP_Handle
  { .OCF = 0x20, .isret = 1, .coistart =  63, .numcoi = 3 }, //  34 7.1.25       HCI_Read_LMP_Handle
  { .OCF = 0x28, .isret = 0, .coistart =  91, .numcoi = 4 }, //  35 7.1.26       HCI_Setup_Synchronous_Connection
  { .OCF = 0x29, .isret = 0, .coistart =   3, .numcoi = 6 }, //  36 7.1.27       HCI_Accept_Synchronous_Connection_Request
  { .OCF = 0x2a, .isret = 0, .coistart =  44, .numcoi = 3 }, //  37 7.1.28       HCI_Reject_Synchronous_Connection_Request
  { .OCF = 0x2b, .isret = 0, .coistart =  47, .numcoi = 3 }, //  38 7.1.29       HCI_IO_Capability_Request_Reply
  { .OCF = 0x2b, .isret = 1, .coistart =  18, .numcoi = 1 }, //  39 7.1.29     p HCI_IO_Capability_Request_Reply
  { .OCF = 0x2c, .isret = 0, .coistart =  15, .numcoi = 3 }, //  40 7.1.30       HCI_User_Confirmation_Request_Reply
  { .OCF = 0x2c, .isret = 1, .coistart =  18, .numcoi = 1 }, //  41 7.1.30     p HCI_User_Confirmation_Request_Reply
  { .OCF = 0x2d, .isret = 0, .coistart =  15, .numcoi = 3 }, //  42 7.1.31       HCI_User_Confirmation_Request_Negative_Reply
  { .OCF = 0x2d, .isret = 1, .coistart =  18, .numcoi = 1 }, //  43 7.1.31     p HCI_User_Confirmation_Request_Negative_Reply
  { .OCF = 0x2e, .isret = 0, .coistart =  99, .numcoi = 3 }, //  44 7.1.32       HCI_User_Passkey_Request_Reply
  { .OCF = 0x2e, .isret = 1, .coistart =  18, .numcoi = 1 }, //  45 7.1.32     p HCI_User_Passkey_Request_Reply
  { .OCF = 0x2f, .isret = 0, .coistart =  15, .numcoi = 3 }, //  46 7.1.33       HCI_User_Passkey_Request_Negative_Reply
  { .OCF = 0x2f, .isret = 1, .coistart =  18, .numcoi = 1 }, //  47 7.1.33     p HCI_User_Passkey_Request_Negative_Reply
  { .OCF = 0x30, .isret = 0, .coistart =  72, .numcoi = 5 }, //  48 7.1.34       HCI_Remote_OOB_Data_Request_Reply
  { .OCF = 0x30, .isret = 1, .coistart =  18, .numcoi = 1 }, //  49 7.1.34     p HCI_Remote_OOB_Data_Request_Reply
  { .OCF = 0x33, .isret = 0, .coistart =  15, .numcoi = 3 }, //  50 7.1.35       HCI_Remote_OOB_Data_Request_Negative_Reply
  { .OCF = 0x33, .isret = 1, .coistart =  18, .numcoi = 1 }, //  51 7.1.35     p HCI_Remote_OOB_Data_Request_Negative_Reply
  { .OCF = 0x34, .isret = 0, .coistart =  44, .numcoi = 3 }, //  52 7.1.36       HCI_IO_Capability_Request_Negative_Reply
  { .OCF = 0x34, .isret = 1, .coistart =  18, .numcoi = 1 }, //  53 7.1.36     p HCI_IO_Capability_Request_Negative_Reply
  { .OCF = 0x3d, .isret = 0, .coistart =  33, .numcoi = 8 }, //  54 7.1.45       HCI_Enhanced_Setup_Synchronous_Connection
  { .OCF = 0x3e, .isret = 0, .coistart =  25, .numcoi = 8 }, //  55 7.1.46       HCI_Enhanced_Accept_Synchronous_Connection_Request
  { .OCF = 0x3f, .isret = 0, .coistart =  95, .numcoi = 4 }, //  56 7.1.47       HCI_Truncated_Page
  { .OCF = 0x40, .isret = 0, .coistart =  15, .numcoi = 3 }, //  57 7.1.48       HCI_Truncated_Page_Cancel
  { .OCF = 0x40, .isret = 1, .coistart =  18, .numcoi = 1 }, //  58 7.1.48     p HCI_Truncated_Page_Cancel
  { .OCF = 0x41, .isret = 0, .coistart =  82, .numcoi = 4 }, //  59 7.1.49       HCI_Set_Connectionless_Peripheral_Broadcast
  { .OCF = 0x41, .isret = 1, .coistart =  71, .numcoi = 1 }, //  60 7.1.49       HCI_Set_Connectionless_Peripheral_Broadcast
  { .OCF = 0x42, .isret = 0, .coistart =  86, .numcoi = 5 }, //  61 7.1.50       HCI_Set_Connectionless_Peripheral_Broadcast_Receive
  { .OCF = 0x42, .isret = 1, .coistart =  18, .numcoi = 1 }, //  62 7.1.50       HCI_Set_Connectionless_Peripheral_Broadcast_Receive
  { .OCF = 0x43, .isret = 0, .coistart =   3, .numcoi = 2 }, //  63 7.1.51     s HCI_Start_Synchronization_Train
  { .OCF = 0x44, .isret = 0, .coistart =  66, .numcoi = 3 }, //  64 7.1.52       HCI_Receive_Synchronization_Train
  { .OCF = 0x45, .isret = 0, .coistart =  77, .numcoi = 5 }, //  65 7.1.53       HCI_Remote_OOB_Extended_Data_Request_Reply
  { .OCF = 0x45, .isret = 1, .coistart =  18, .numcoi = 1 }, //  66 7.1.53     p HCI_Remote_OOB_Extended_Data_Request_Reply
};

static const CodecTab_t CodecTab_1 = {
  .numtab = 67, .numcoi = 102, .table = TE_1, .CoI = CoI_1
};

static const CoI_t CoI_2[39] = {
  INFO(     7.2.3 ,   0) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  2 }, 
  INFO(    7.2.13 ,   3) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  5, .skip = 1 }, { .action = copyws, .inst = 1 }, { .LSB = 0x10 }, { .MSB = 0x00 }, 
  INFO(     7.2.1 ,   9) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  6 }, 
  INFO(     7.2.6 ,  12) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  4, .skip = 2 }, { .action = copyws, .inst = 1 }, { .LSB = 0x10 }, { .MSB = 0x00 }, 
  INFO(    7.2.10 ,  18) { .num =  7, .skip = 1 }, { .num =  2 }, 
  INFO(     7.2.7 ,  20) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  2 }, 
  INFO(     7.2.9 ,  23) { .num =  7, .skip = 1 }, { .num =  4 }, 
  INFO(     7.2.7 ,  25) { .num =  7, .skip = 1 }, { .num =  3, .skip = 1 }, 
  INFO(     7.2.2 ,  27) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num = 10 }, 
  INFO(    7.2.14 ,  30) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  8 }, 
  INFO(     7.2.8 ,  33) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  7, .skip = 1 }, 
  INFO(    7.2.10 ,  36) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  4 }, 
};

static const TE_t TE_2[18] = {
  { .OCF = 0x01, .isret = 0, .coistart =   9, .numcoi = 3 }, //   0 7.2.1        HCI_Hold_Mode
  { .OCF = 0x03, .isret = 0, .coistart =  27, .numcoi = 3 }, //   1 7.2.2        HCI_Sniff_Mode
  { .OCF = 0x04, .isret = 0, .coistart =   0, .numcoi = 3 }, //   2 7.2.3      p HCI_Exit_Sniff_Mode
  { .OCF = 0x07, .isret = 0, .coistart =  12, .numcoi = 6 }, //   3 7.2.6        HCI_QoS_Setup
  { .OCF = 0x09, .isret = 0, .coistart =  20, .numcoi = 3 }, //   4 7.2.7      p HCI_Role_Discovery
  { .OCF = 0x09, .isret = 1, .coistart =  25, .numcoi = 2 }, //   5 7.2.7        HCI_Role_Discovery
  { .OCF = 0x0b, .isret = 0, .coistart =  33, .numcoi = 3 }, //   6 7.2.8        HCI_Switch_Role
  { .OCF = 0x0c, .isret = 0, .coistart =  20, .numcoi = 3 }, //   7 7.2.9      p HCI_Read_Link_Policy_Settings
  { .OCF = 0x0c, .isret = 1, .coistart =  23, .numcoi = 2 }, //   8 7.2.9        HCI_Read_Link_Policy_Settings
  { .OCF = 0x0d, .isret = 0, .coistart =  36, .numcoi = 3 }, //   9 7.2.10       HCI_Write_Link_Policy_Settings
  { .OCF = 0x0d, .isret = 1, .coistart =  18, .numcoi = 2 }, //  10 7.2.10     p HCI_Write_Link_Policy_Settings
  { .OCF = 0x0e, .isret = 0, .coistart =   3, .numcoi = 2 }, //  11 7.2.11     s HCI_Read_Default_Link_Policy_Settings
  { .OCF = 0x0e, .isret = 1, .coistart =  18, .numcoi = 2 }, //  12 7.2.11       HCI_Read_Default_Link_Policy_Settings
  { .OCF = 0x0f, .isret = 0, .coistart =  20, .numcoi = 3 }, //  13 7.2.12       HCI_Write_Default_Link_Policy_Settings
  { .OCF = 0x0f, .isret = 1, .coistart =  18, .numcoi = 1 }, //  14 7.2.12     p HCI_Write_Default_Link_Policy_Settings
  { .OCF = 0x10, .isret = 0, .coistart =   3, .numcoi = 6 }, //  15 7.2.13       HCI_Flow_Specification
  { .OCF = 0x11, .isret = 0, .coistart =  30, .numcoi = 3 }, //  16 7.2.14       HCI_Sniff_Subrating
  { .OCF = 0x11, .isret = 1, .coistart =  18, .numcoi = 2 }, //  17 7.2.14     p HCI_Sniff_Subrating
};

static const CodecTab_t CodecTab_2 = {
  .numtab = 18, .numcoi = 39, .table = TE_2, .CoI = CoI_2
};

static const CoI_t CoI_3[122] = {
  INFO(   7.3.101 ,   0) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  3 }, { .action = inlinefix, .inst = 1 }, 
  INFO(     7.3.1 ,   4) { .num =  7, .skip = 1 }, 
  INFO(    7.3.83 ,   5) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, { .num =  8 }, 
  INFO(     7.3.8 ,   9) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  7, .skip = 1 }, 
  INFO(     7.3.4 ,  12) { .num =  7, .skip = 1 }, { .num =  2 }, 
  INFO(    7.3.26 ,  14) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  3, .skip = 1 }, 
  INFO(     7.3.4 ,  17) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  2 }, 
  INFO(    7.3.39 ,  20) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  4 }, 
  INFO(    7.3.40 ,  24) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, { .action = loop, .inst = 1 }, { .num =  4 }, { .action = endloop, .inst = 1 }, 
  INFO(     7.3.8 ,  30) { .num =  7, .skip = 1 }, { .num =  4 }, 
  INFO(    7.3.25 ,  32) { .num = 10 }, 
  INFO(    7.3.44 ,  33) { .num =  8 }, { .action = inlinefix, .inst = 1 }, { .num =  3, .skip = 1 }, 
  INFO(    7.3.74 ,  36) { .num =  7, .skip = 1 }, { .num =  5, .skip = 1 }, 
  INFO(    7.3.55 ,  38) { .action = copyws, .inst = 1 }, { .LSB = 0xf8 }, { .MSB = 0x00 }, 
  INFO(    7.3.78 ,  41) { .num =  9, .skip = 1 }, 
  INFO(    7.3.12 ,  42) { .action = copyws, .arg = 1, .inst = 1 }, { .LSB = 0xff }, { .MSB = 0x00 }, 
  INFO(    7.3.60 ,  45) { .action = copyws, .arg = 1, .inst = 1 }, { .LSB = 0x27 }, { .MSB = 0x00 }, 
  INFO(    7.3.95 ,  48) { .action = copyws, .arg = 1, .inst = 1 }, { .LSB = 0x47 }, { .MSB = 0x00 }, 
  INFO(    7.3.89 ,  51) { .num =  7, .skip = 1 }, { .num =  2, .skip = 2 }, { .num =  5, .skip = 3 }, 
  INFO(    7.3.35 ,  54) { .num =  7, .skip = 1 }, { .num =  3, .skip = 1 }, 
  INFO(    7.3.63 ,  56) { .num = 13, .skip = 1 }, 
  INFO(    7.3.46 ,  57) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num = 10 }, 
  INFO(    7.3.88 ,  60) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  3 }, { .action = inlinefix, .inst = 1 }, 
  INFO(     7.3.1 ,  64) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  8 }, 
  INFO(    7.3.81 ,  67) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  7, .skip = 1 }, { .action = loop, .inst = 1 }, { .num =  3, .skip = 1 }, { .action = endloop, .inst = 1 }, 
  INFO(    7.3.80 ,  73) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, { .num =  9, .skip = 1 }, 
  INFO(    7.3.82 ,  77) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .action = copyws, .inst = 1 }, { .LSB = 0x1e }, { .MSB = 0x00 }, 
  INFO(    7.3.82 ,  82) { .num =  7, .skip = 1 }, { .action = copyws, .inst = 1 }, { .LSB = 0x20 }, { .MSB = 0x00 }, 
  INFO(    7.3.85 ,  86) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  2 }, { .action = loop, .inst = 1 }, { .num =  1, .skip = 1 }, { .action = endloop, .inst = 1 }, 
  INFO(    7.3.20 ,  92) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  4 }, 
  INFO(    7.3.45 ,  95) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  1 }, { .action = inlinefix, .inst = 1 }, { .num =  3, .skip = 1 }, 
  INFO(    7.3.56 , 100) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .action = copyws, .arg = 1, .inst = 1 }, { .LSB = 0xf1 }, { .MSB = 0x00 }, 
  INFO(    7.3.11 , 105) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .action = copyws, .inst = 1 }, { .LSB = 0xf8 }, { .MSB = 0x00 }, 
  INFO(     7.3.9 , 110) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  1 }, { .action = loop, .inst = 1 }, { .action = copyws, .inst = 1 }, { .LSB = 0x16 }, { .MSB = 0x00 }, { .action = endloop, .inst = 1 }, 
  INFO(    7.3.90 , 118) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  4, .skip = 2 }, { .num =  5, .skip = 3 }, 
};

static const TE_t TE_3[185] = {
  { .OCF = 0x01, .isret = 0, .coistart =  64, .numcoi = 3 }, //   0 7.3.1        HCI_Set_Event_Mask
  { .OCF = 0x01, .isret = 1, .coistart =   4, .numcoi = 1 }, //   1 7.3.1      p HCI_Set_Event_Mask
  { .OCF = 0x03, .isret = 0, .coistart =   5, .numcoi = 2 }, //   2 7.3.2      s HCI_Reset
  { .OCF = 0x03, .isret = 1, .coistart =   4, .numcoi = 1 }, //   3 7.3.2      p HCI_Reset
  { .OCF = 0x05, .isret = 0, .coistart =  64, .numcoi = 3 }, //   4 7.3.3        HCI_Set_Event_Filter
  { .OCF = 0x05, .isret = 1, .coistart =   4, .numcoi = 1 }, //   5 7.3.3      p HCI_Set_Event_Filter
  { .OCF = 0x08, .isret = 0, .coistart =  17, .numcoi = 3 }, //   6 7.3.4      p HCI_Flush
  { .OCF = 0x08, .isret = 1, .coistart =  12, .numcoi = 2 }, //   7 7.3.4      p HCI_Flush
  { .OCF = 0x09, .isret = 0, .coistart =   5, .numcoi = 2 }, //   8 7.3.5      s HCI_Read_PIN_Type
  { .OCF = 0x09, .isret = 1, .coistart =   8, .numcoi = 1 }, //   9 7.3.5        HCI_Read_PIN_Type
  { .OCF = 0x0a, .isret = 0, .coistart =   5, .numcoi = 3 }, //  10 7.3.6        HCI_Write_PIN_Type
  { .OCF = 0x0a, .isret = 1, .coistart =   4, .numcoi = 1 }, //  11 7.3.6      p HCI_Write_PIN_Type
  { .OCF = 0x0d, .isret = 0, .coistart =   9, .numcoi = 3 }, //  12 7.3.8        HCI_Read_Stored_Link_Key
  { .OCF = 0x0d, .isret = 1, .coistart =  30, .numcoi = 2 }, //  13 7.3.8        HCI_Read_Stored_Link_Key
  { .OCF = 0x11, .isret = 0, .coistart = 110, .numcoi = 8 }, //  14 7.3.9        HCI_Write_Stored_Link_Key
  { .OCF = 0x11, .isret = 1, .coistart =   8, .numcoi = 1 }, //  15 7.3.9        HCI_Write_Stored_Link_Key
  { .OCF = 0x12, .isret = 0, .coistart =   9, .numcoi = 3 }, //  16 7.3.10       HCI_Delete_Stored_Link_Key
  { .OCF = 0x12, .isret = 1, .coistart =  12, .numcoi = 2 }, //  17 7.3.10       HCI_Delete_Stored_Link_Key
  { .OCF = 0x13, .isret = 0, .coistart = 105, .numcoi = 5 }, //  18 7.3.11       HCI_Write_Local_Name
  { .OCF = 0x13, .isret = 1, .coistart =   4, .numcoi = 1 }, //  19 7.3.11     p HCI_Write_Local_Name
  { .OCF = 0x14, .isret = 0, .coistart =   5, .numcoi = 2 }, //  20 7.3.12     s HCI_Read_Local_Name
  { .OCF = 0x14, .isret = 1, .coistart =  42, .numcoi = 3 }, //  21 7.3.12       HCI_Read_Local_Name
  { .OCF = 0x15, .isret = 0, .coistart =   5, .numcoi = 2 }, //  22 7.3.13     s HCI_Read_Connection_Accept_Timeout
  { .OCF = 0x15, .isret = 1, .coistart =  12, .numcoi = 2 }, //  23 7.3.13       HCI_Read_Connection_Accept_Timeout
  { .OCF = 0x16, .isret = 0, .coistart =  17, .numcoi = 3 }, //  24 7.3.14       HCI_Write_Connection_Accept_Timeout
  { .OCF = 0x16, .isret = 1, .coistart =   4, .numcoi = 1 }, //  25 7.3.14     p HCI_Write_Connection_Accept_Timeout
  { .OCF = 0x17, .isret = 0, .coistart =   5, .numcoi = 2 }, //  26 7.3.15     s HCI_Read_Page_Timeout
  { .OCF = 0x17, .isret = 1, .coistart =  12, .numcoi = 2 }, //  27 7.3.15       HCI_Read_Page_Timeout
  { .OCF = 0x18, .isret = 0, .coistart =  17, .numcoi = 3 }, //  28 7.3.16       HCI_Write_Page_Timeout
  { .OCF = 0x18, .isret = 1, .coistart =   4, .numcoi = 1 }, //  29 7.3.16     p HCI_Write_Page_Timeout
  { .OCF = 0x19, .isret = 0, .coistart =   5, .numcoi = 2 }, //  30 7.3.17     s HCI_Read_Scan_Enable
  { .OCF = 0x19, .isret = 1, .coistart =   8, .numcoi = 1 }, //  31 7.3.17       HCI_Read_Scan_Enable
  { .OCF = 0x1a, .isret = 0, .coistart =   5, .numcoi = 3 }, //  32 7.3.18       HCI_Write_Scan_Enable
  { .OCF = 0x1a, .isret = 1, .coistart =   4, .numcoi = 1 }, //  33 7.3.18     p HCI_Write_Scan_Enable
  { .OCF = 0x1b, .isret = 0, .coistart =   5, .numcoi = 2 }, //  34 7.3.19     s HCI_Read_Page_Scan_Activity
  { .OCF = 0x1b, .isret = 1, .coistart =  30, .numcoi = 2 }, //  35 7.3.19       HCI_Read_Page_Scan_Activity
  { .OCF = 0x1c, .isret = 0, .coistart =  92, .numcoi = 3 }, //  36 7.3.20       HCI_Write_Page_Scan_Activity
  { .OCF = 0x1c, .isret = 1, .coistart =   4, .numcoi = 1 }, //  37 7.3.20     p HCI_Write_Page_Scan_Activity
  { .OCF = 0x1d, .isret = 0, .coistart =   5, .numcoi = 2 }, //  38 7.3.21     s HCI_Read_Inquiry_Scan_Activity
  { .OCF = 0x1d, .isret = 1, .coistart =  30, .numcoi = 2 }, //  39 7.3.21       HCI_Read_Inquiry_Scan_Activity
  { .OCF = 0x1e, .isret = 0, .coistart =  92, .numcoi = 3 }, //  40 7.3.22       HCI_Write_Inquiry_Scan_Activity
  { .OCF = 0x1e, .isret = 1, .coistart =   4, .numcoi = 1 }, //  41 7.3.22     p HCI_Write_Inquiry_Scan_Activity
  { .OCF = 0x1f, .isret = 0, .coistart =   5, .numcoi = 2 }, //  42 7.3.23     s HCI_Read_Authentication_Enable
  { .OCF = 0x1f, .isret = 1, .coistart =   8, .numcoi = 1 }, //  43 7.3.23       HCI_Read_Authentication_Enable
  { .OCF = 0x20, .isret = 0, .coistart =   5, .numcoi = 3 }, //  44 7.3.24       HCI_Write_Authentication_Enable
  { .OCF = 0x20, .isret = 1, .coistart =   4, .numcoi = 1 }, //  45 7.3.24     p HCI_Write_Authentication_Enable
  { .OCF = 0x23, .isret = 0, .coistart =   5, .numcoi = 2 }, //  46 7.3.25     s HCI_Read_Class_Of_Device
  { .OCF = 0x23, .isret = 1, .coistart =  32, .numcoi = 1 }, //  47 7.3.25       HCI_Read_Class_Of_Device
  { .OCF = 0x24, .isret = 0, .coistart =  14, .numcoi = 3 }, //  48 7.3.26       HCI_Write_Class_Of_Device
  { .OCF = 0x24, .isret = 1, .coistart =   4, .numcoi = 1 }, //  49 7.3.26     p HCI_Write_Class_Of_Device
  { .OCF = 0x25, .isret = 0, .coistart =   5, .numcoi = 2 }, //  50 7.3.27     s HCI_Read_Voice_Setting
  { .OCF = 0x25, .isret = 1, .coistart =  12, .numcoi = 2 }, //  51 7.3.27       HCI_Read_Voice_Setting
  { .OCF = 0x26, .isret = 0, .coistart =  17, .numcoi = 3 }, //  52 7.3.28       HCI_Write_Voice_Setting
  { .OCF = 0x26, .isret = 1, .coistart =   4, .numcoi = 1 }, //  53 7.3.28     p HCI_Write_Voice_Setting
  { .OCF = 0x27, .isret = 0, .coistart =  17, .numcoi = 3 }, //  54 7.3.29     p HCI_Read_Automatic_Flush_Timeout
  { .OCF = 0x27, .isret = 1, .coistart =  30, .numcoi = 2 }, //  55 7.3.29       HCI_Read_Automatic_Flush_Timeout
  { .OCF = 0x28, .isret = 0, .coistart =  92, .numcoi = 3 }, //  56 7.3.30       HCI_Write_Automatic_Flush_Timeout
  { .OCF = 0x28, .isret = 1, .coistart =  12, .numcoi = 2 }, //  57 7.3.30     p HCI_Write_Automatic_Flush_Timeout
  { .OCF = 0x29, .isret = 0, .coistart =   5, .numcoi = 2 }, //  58 7.3.31     s HCI_Read_Num_Broadcast_Retransmissions
  { .OCF = 0x29, .isret = 1, .coistart =   8, .numcoi = 1 }, //  59 7.3.31       HCI_Read_Num_Broadcast_Retransmissions
  { .OCF = 0x2a, .isret = 0, .coistart =   5, .numcoi = 3 }, //  60 7.3.32       HCI_Write_Num_Broadcast_Retransmissions
  { .OCF = 0x2a, .isret = 1, .coistart =   4, .numcoi = 1 }, //  61 7.3.32     p HCI_Write_Num_Broadcast_Retransmissions
  { .OCF = 0x2b, .isret = 0, .coistart =   5, .numcoi = 2 }, //  62 7.3.33     s HCI_Read_Hold_Mode_Activity
  { .OCF = 0x2b, .isret = 1, .coistart =   8, .numcoi = 1 }, //  63 7.3.33       HCI_Read_Hold_Mode_Activity
  { .OCF = 0x2c, .isret = 0, .coistart =   5, .numcoi = 3 }, //  64 7.3.34       HCI_Write_Hold_Mode_Activity
  { .OCF = 0x2c, .isret = 1, .coistart =   4, .numcoi = 1 }, //  65 7.3.34     p HCI_Write_Hold_Mode_Activity
  { .OCF = 0x2d, .isret = 0, .coistart =  14, .numcoi = 3 }, //  66 7.3.35       HCI_Read_Transmit_Power_Level
  { .OCF = 0x2d, .isret = 1, .coistart =  54, .numcoi = 2 }, //  67 7.3.35       HCI_Read_Transmit_Power_Level
  { .OCF = 0x2e, .isret = 0, .coistart =   5, .numcoi = 2 }, //  68 7.3.36     s HCI_Read_Synchronous_Flow_Control_Enable
  { .OCF = 0x2e, .isret = 1, .coistart =   8, .numcoi = 1 }, //  69 7.3.36       HCI_Read_Synchronous_Flow_Control_Enable
  { .OCF = 0x2f, .isret = 0, .coistart =   5, .numcoi = 3 }, //  70 7.3.37       HCI_Write_Synchronous_Flow_Control_Enable
  { .OCF = 0x2f, .isret = 1, .coistart =   4, .numcoi = 1 }, //  71 7.3.37     p HCI_Write_Synchronous_Flow_Control_Enable
  { .OCF = 0x31, .isret = 0, .coistart =   5, .numcoi = 3 }, //  72 7.3.38       HCI_Set_Controller_To_Host_Flow_Control
  { .OCF = 0x31, .isret = 1, .coistart =   4, .numcoi = 1 }, //  73 7.3.38     p HCI_Set_Controller_To_Host_Flow_Control
  { .OCF = 0x33, .isret = 0, .coistart =  20, .numcoi = 4 }, //  74 7.3.39       HCI_Host_Buffer_Size
  { .OCF = 0x33, .isret = 1, .coistart =   4, .numcoi = 1 }, //  75 7.3.39     p HCI_Host_Buffer_Size
  { .OCF = 0x35, .isret = 0, .coistart =  24, .numcoi = 6 }, //  76 7.3.40       HCI_Host_Number_Of_Completed_Packets
  { .OCF = 0x36, .isret = 0, .coistart =  17, .numcoi = 3 }, //  77 7.3.41       HCI_Read_Link_Supervision_Timeout
  { .OCF = 0x36, .isret = 1, .coistart =  30, .numcoi = 2 }, //  78 7.3.41       HCI_Read_Link_Supervision_Timeout
  { .OCF = 0x37, .isret = 0, .coistart =  92, .numcoi = 3 }, //  79 7.3.42       HCI_Write_Link_Supervision_Timeout
  { .OCF = 0x37, .isret = 1, .coistart =  12, .numcoi = 2 }, //  80 7.3.42       HCI_Write_Link_Supervision_Timeout
  { .OCF = 0x38, .isret = 0, .coistart =   5, .numcoi = 2 }, //  81 7.3.43     s HCI_Read_Number_Of_Supported_IAC
  { .OCF = 0x38, .isret = 1, .coistart =   8, .numcoi = 1 }, //  82 7.3.43       HCI_Read_Number_Of_Supported_IAC
  { .OCF = 0x39, .isret = 0, .coistart =   5, .numcoi = 2 }, //  83 7.3.44     s HCI_Read_Current_IAC_LAP
  { .OCF = 0x39, .isret = 1, .coistart =  33, .numcoi = 3 }, //  84 7.3.44       HCI_Read_Current_IAC_LAP
  { .OCF = 0x3a, .isret = 0, .coistart =  95, .numcoi = 5 }, //  85 7.3.45       HCI_Write_Current_IAC_LAP
  { .OCF = 0x3a, .isret = 1, .coistart =   4, .numcoi = 1 }, //  86 7.3.45     p HCI_Write_Current_IAC_LAP
  { .OCF = 0x3f, .isret = 0, .coistart =  57, .numcoi = 3 }, //  87 7.3.46       HCI_Set_AFH_Host_Channel_Classification
  { .OCF = 0x3f, .isret = 1, .coistart =   4, .numcoi = 1 }, //  88 7.3.46     p HCI_Set_AFH_Host_Channel_Classification
  { .OCF = 0x42, .isret = 0, .coistart =   5, .numcoi = 2 }, //  89 7.3.47     s HCI_Read_Inquiry_Scan_Type
  { .OCF = 0x42, .isret = 1, .coistart =   8, .numcoi = 1 }, //  90 7.3.47       HCI_Read_Inquiry_Scan_Type
  { .OCF = 0x43, .isret = 0, .coistart =   5, .numcoi = 3 }, //  91 7.3.48       HCI_Write_Inquiry_Scan_Type
  { .OCF = 0x43, .isret = 1, .coistart =   4, .numcoi = 1 }, //  92 7.3.48     p HCI_Write_Inquiry_Scan_Type
  { .OCF = 0x44, .isret = 0, .coistart =   5, .numcoi = 2 }, //  93 7.3.49     s HCI_Read_Inquiry_Mode
  { .OCF = 0x44, .isret = 1, .coistart =   8, .numcoi = 1 }, //  94 7.3.49       HCI_Read_Inquiry_Mode
  { .OCF = 0x45, .isret = 0, .coistart =   5, .numcoi = 3 }, //  95 7.3.50       HCI_Write_Inquiry_Mode
  { .OCF = 0x45, .isret = 1, .coistart =   4, .numcoi = 1 }, //  96 7.3.50     p HCI_Write_Inquiry_Mode
  { .OCF = 0x46, .isret = 0, .coistart =   5, .numcoi = 2 }, //  97 7.3.51     s HCI_Read_Page_Scan_Type
  { .OCF = 0x46, .isret = 1, .coistart =   8, .numcoi = 1 }, //  98 7.3.51       HCI_Read_Page_Scan_Type
  { .OCF = 0x47, .isret = 0, .coistart =   5, .numcoi = 3 }, //  99 7.3.52       HCI_Write_Page_Scan_Type
  { .OCF = 0x47, .isret = 1, .coistart =   4, .numcoi = 1 }, // 100 7.3.52     p HCI_Write_Page_Scan_Type
  { .OCF = 0x48, .isret = 0, .coistart =   5, .numcoi = 2 }, // 101 7.3.53     s HCI_Read_AFH_Channel_Assessment_Mode
  { .OCF = 0x48, .isret = 1, .coistart =   8, .numcoi = 1 }, // 102 7.3.53       HCI_Read_AFH_Channel_Assessment_Mode
  { .OCF = 0x49, .isret = 0, .coistart =   5, .numcoi = 3 }, // 103 7.3.54       HCI_Write_AFH_Channel_Assessment_Mode
  { .OCF = 0x49, .isret = 1, .coistart =   4, .numcoi = 1 }, // 104 7.3.54     p HCI_Write_AFH_Channel_Assessment_Mode
  { .OCF = 0x51, .isret = 0, .coistart =   5, .numcoi = 2 }, // 105 7.3.55     s HCI_Read_Extended_Inquiry_Response
  { .OCF = 0x51, .isret = 1, .coistart =  38, .numcoi = 3 }, // 106 7.3.55       HCI_Read_Extended_Inquiry_Response
  { .OCF = 0x52, .isret = 0, .coistart = 100, .numcoi = 5 }, // 107 7.3.56       HCI_Write_Extended_Inquiry_Response
  { .OCF = 0x52, .isret = 1, .coistart =   4, .numcoi = 1 }, // 108 7.3.56     p HCI_Write_Extended_Inquiry_Response
  { .OCF = 0x53, .isret = 0, .coistart =  17, .numcoi = 3 }, // 109 7.3.57     p HCI_Refresh_Encryption_Key
  { .OCF = 0x55, .isret = 0, .coistart =   5, .numcoi = 2 }, // 110 7.3.58     s HCI_Read_Simple_Pairing_Mode
  { .OCF = 0x55, .isret = 1, .coistart =   8, .numcoi = 1 }, // 111 7.3.58       HCI_Read_Simple_Pairing_Mode
  { .OCF = 0x56, .isret = 0, .coistart =   5, .numcoi = 3 }, // 112 7.3.59       HCI_Write_Simple_Pairing_Mode
  { .OCF = 0x56, .isret = 1, .coistart =   4, .numcoi = 1 }, // 113 7.3.59     p HCI_Write_Simple_Pairing_Mode
  { .OCF = 0x57, .isret = 0, .coistart =   5, .numcoi = 2 }, // 114 7.3.60     s HCI_Read_Local_OOB_Data
  { .OCF = 0x57, .isret = 1, .coistart =  45, .numcoi = 3 }, // 115 7.3.60       HCI_Read_Local_OOB_Data
  { .OCF = 0x58, .isret = 0, .coistart =   5, .numcoi = 2 }, // 116 7.3.61     s HCI_Read_Inquiry_Response_Transmit_Power_Level
  { .OCF = 0x58, .isret = 1, .coistart =   8, .numcoi = 1 }, // 117 7.3.61       HCI_Read_Inquiry_Response_Transmit_Power_Level
  { .OCF = 0x59, .isret = 0, .coistart =   5, .numcoi = 3 }, // 118 7.3.62       HCI_Write_Inquiry_Transmit_Power_Level
  { .OCF = 0x59, .isret = 1, .coistart =   4, .numcoi = 1 }, // 119 7.3.62     p HCI_Write_Inquiry_Transmit_Power_Level
  { .OCF = 0x5a, .isret = 0, .coistart =   5, .numcoi = 2 }, // 120 7.3.64     s HCI_Read_Default_Erroneous_Data_Reporting
  { .OCF = 0x5a, .isret = 1, .coistart =   8, .numcoi = 1 }, // 121 7.3.64       HCI_Read_Default_Erroneous_Data_Reporting
  { .OCF = 0x5b, .isret = 0, .coistart =   5, .numcoi = 3 }, // 122 7.3.65       HCI_Write_Default_Erroneous_Data_Reporting
  { .OCF = 0x5b, .isret = 1, .coistart =   4, .numcoi = 1 }, // 123 7.3.65     p HCI_Write_Default_Erroneous_Data_Reporting
  { .OCF = 0x5f, .isret = 0, .coistart =  14, .numcoi = 3 }, // 124 7.3.66       HCI_Enhanced_Flush
  { .OCF = 0x60, .isret = 0, .coistart =   9, .numcoi = 3 }, // 125 7.3.63       HCI_Send_Keypress_Notification
  { .OCF = 0x60, .isret = 1, .coistart =  56, .numcoi = 1 }, // 126 7.3.63     p HCI_Send_Keypress_Notification
  { .OCF = 0x63, .isret = 0, .coistart =  64, .numcoi = 3 }, // 127 7.3.69       HCI_Set_Event_Mask_Page_2
  { .OCF = 0x63, .isret = 1, .coistart =   4, .numcoi = 1 }, // 128 7.3.69     p HCI_Set_Event_Mask_Page_2
  { .OCF = 0x66, .isret = 0, .coistart =   5, .numcoi = 2 }, // 129 7.3.72     s HCI_Read_Flow_Control_Mode
  { .OCF = 0x66, .isret = 1, .coistart =   8, .numcoi = 1 }, // 130 7.3.72       HCI_Read_Flow_Control_Mode
  { .OCF = 0x67, .isret = 0, .coistart =   5, .numcoi = 3 }, // 131 7.3.73       HCI_Write_Flow_Control_Mode
  { .OCF = 0x67, .isret = 1, .coistart =   4, .numcoi = 1 }, // 132 7.3.73     p HCI_Write_Flow_Control_Mode
  { .OCF = 0x68, .isret = 0, .coistart =  14, .numcoi = 3 }, // 133 7.3.74       HCI_Read_Enhanced_Transmit_Power_Level
  { .OCF = 0x68, .isret = 1, .coistart =  36, .numcoi = 2 }, // 134 7.3.74       HCI_Read_Enhanced_Transmit_Power_Level
  { .OCF = 0x6c, .isret = 0, .coistart =   5, .numcoi = 2 }, // 135 7.3.78     s HCI_Read_LE_Host_Support
  { .OCF = 0x6c, .isret = 1, .coistart =  41, .numcoi = 1 }, // 136 7.3.78       HCI_Read_LE_Host_Support
  { .OCF = 0x6d, .isret = 0, .coistart =  17, .numcoi = 3 }, // 137 7.3.79       HCI_Write_LE_Host_Support
  { .OCF = 0x6d, .isret = 1, .coistart =   4, .numcoi = 1 }, // 138 7.3.79     p HCI_Write_LE_Host_Support
  { .OCF = 0x6e, .isret = 0, .coistart =  73, .numcoi = 4 }, // 139 7.3.80       HCI_Set_MWS_Channel_Parameters
  { .OCF = 0x6e, .isret = 1, .coistart =   4, .numcoi = 1 }, // 140 7.3.80     p HCI_Set_MWS_Channel_Parameters
  { .OCF = 0x6f, .isret = 0, .coistart =  67, .numcoi = 6 }, // 141 7.3.81       HCI_Set_External_Frame_Configuration
  { .OCF = 0x6f, .isret = 1, .coistart =   4, .numcoi = 1 }, // 142 7.3.81     p HCI_Set_External_Frame_Configuration
  { .OCF = 0x70, .isret = 0, .coistart =  77, .numcoi = 5 }, // 143 7.3.82       HCI_Set_MWS_Signaling
  { .OCF = 0x70, .isret = 1, .coistart =  82, .numcoi = 4 }, // 144 7.3.82       HCI_Set_MWS_Signaling
  { .OCF = 0x71, .isret = 0, .coistart =   5, .numcoi = 4 }, // 145 7.3.83       HCI_Set_MWS_Transport_Layer
  { .OCF = 0x71, .isret = 1, .coistart =   4, .numcoi = 1 }, // 146 7.3.83     p HCI_Set_MWS_Transport_Layer
  { .OCF = 0x72, .isret = 0, .coistart =  24, .numcoi = 6 }, // 147 7.3.84       HCI_Set_MWS_Scan_Frequency_Table
  { .OCF = 0x72, .isret = 1, .coistart =   4, .numcoi = 1 }, // 148 7.3.84     p HCI_Set_MWS_Scan_Frequency_Table
  { .OCF = 0x73, .isret = 0, .coistart =  86, .numcoi = 6 }, // 149 7.3.85       HCI_Set_MWS_PATTERN_Configuration
  { .OCF = 0x73, .isret = 1, .coistart =   4, .numcoi = 1 }, // 150 7.3.85     p HCI_Set_MWS_PATTERN_Configuration
  { .OCF = 0x74, .isret = 0, .coistart =   5, .numcoi = 3 }, // 151 7.3.86       HCI_Set_Reserved_LT_ADDR
  { .OCF = 0x74, .isret = 1, .coistart =   8, .numcoi = 1 }, // 152 7.3.86       HCI_Set_Reserved_LT_ADDR
  { .OCF = 0x75, .isret = 0, .coistart =   5, .numcoi = 3 }, // 153 7.3.87       HCI_Delete_Reserved_LT_ADDR
  { .OCF = 0x75, .isret = 1, .coistart =   8, .numcoi = 1 }, // 154 7.3.87       HCI_Delete_Reserved_LT_ADDR
  { .OCF = 0x76, .isret = 0, .coistart =  60, .numcoi = 4 }, // 155 7.3.88       HCI_Set_Connectionless_Peripheral_Broadcast_Data
  { .OCF = 0x76, .isret = 1, .coistart =   8, .numcoi = 1 }, // 156 7.3.88       HCI_Set_Connectionless_Peripheral_Broadcast_Data
  { .OCF = 0x77, .isret = 0, .coistart =   5, .numcoi = 2 }, // 157 7.3.89     s HCI_Read_Synchronization_Train_Parameters
  { .OCF = 0x77, .isret = 1, .coistart =  51, .numcoi = 3 }, // 158 7.3.89       HCI_Read_Synchronization_Train_Parameters
  { .OCF = 0x78, .isret = 0, .coistart = 118, .numcoi = 4 }, // 159 7.3.90       HCI_Write_Synchronization_Train_Parameters
  { .OCF = 0x78, .isret = 1, .coistart =  12, .numcoi = 2 }, // 160 7.3.90       HCI_Write_Synchronization_Train_Parameters
  { .OCF = 0x79, .isret = 0, .coistart =   5, .numcoi = 2 }, // 161 7.3.91     s HCI_Read_Secure_Connections_Host_Support
  { .OCF = 0x79, .isret = 1, .coistart =   8, .numcoi = 1 }, // 162 7.3.91       HCI_Read_Secure_Connections_Host_Support
  { .OCF = 0x7a, .isret = 0, .coistart =   5, .numcoi = 3 }, // 163 7.3.92       HCI_Write_Secure_Connections_Host_Support
  { .OCF = 0x7a, .isret = 1, .coistart =   4, .numcoi = 1 }, // 164 7.3.92     p HCI_Write_Secure_Connections_Host_Support
  { .OCF = 0x7b, .isret = 0, .coistart =  17, .numcoi = 3 }, // 165 7.3.93     p HCI_Read_Authenticated_Payload_Timeout
  { .OCF = 0x7b, .isret = 1, .coistart =  30, .numcoi = 2 }, // 166 7.3.93       HCI_Read_Authenticated_Payload_Timeout
  { .OCF = 0x7c, .isret = 0, .coistart =  92, .numcoi = 3 }, // 167 7.3.94       HCI_Write_Authenticated_Payload_Timeout
  { .OCF = 0x7c, .isret = 1, .coistart =  12, .numcoi = 2 }, // 168 7.3.94     p HCI_Write_Authenticated_Payload_Timeout
  { .OCF = 0x7d, .isret = 0, .coistart =   5, .numcoi = 2 }, // 169 7.3.95     s HCI_Read_Local_OOB_Extended_Data
  { .OCF = 0x7d, .isret = 1, .coistart =  48, .numcoi = 3 }, // 170 7.3.95       HCI_Read_Local_OOB_Extended_Data
  { .OCF = 0x7e, .isret = 0, .coistart =   5, .numcoi = 2 }, // 171 7.3.96     s HCI_Read_Extended_Page_Timeout
  { .OCF = 0x7e, .isret = 1, .coistart =  12, .numcoi = 2 }, // 172 7.3.96       HCI_Read_Extended_Page_Timeout
  { .OCF = 0x7f, .isret = 0, .coistart =  17, .numcoi = 3 }, // 173 7.3.97       HCI_Write_Extended_Page_Timeout
  { .OCF = 0x7f, .isret = 1, .coistart =   4, .numcoi = 1 }, // 174 7.3.97     p HCI_Write_Extended_Page_Timeout
  { .OCF = 0x80, .isret = 0, .coistart =   5, .numcoi = 2 }, // 175 7.3.98     s HCI_Read_Extended_Inquiry_Length
  { .OCF = 0x80, .isret = 1, .coistart =  12, .numcoi = 2 }, // 176 7.3.98       HCI_Read_Extended_Inquiry_Length
  { .OCF = 0x81, .isret = 0, .coistart =  17, .numcoi = 3 }, // 177 7.3.99       HCI_Write_Extended_Inquiry_Length
  { .OCF = 0x81, .isret = 1, .coistart =   4, .numcoi = 1 }, // 178 7.3.99     p HCI_Write_Extended_Inquiry_Length
  { .OCF = 0x82, .isret = 0, .coistart =  17, .numcoi = 3 }, // 179 7.3.100      HCI_Set_Ecosystem_Base_Interval
  { .OCF = 0x82, .isret = 1, .coistart =   4, .numcoi = 1 }, // 180 7.3.100    p HCI_Set_Ecosystem_Base_Interval
  { .OCF = 0x83, .isret = 0, .coistart =   0, .numcoi = 4 }, // 181 7.3.101      HCI_Configure_Data_Path
  { .OCF = 0x83, .isret = 1, .coistart =   4, .numcoi = 1 }, // 182 7.3.101    p HCI_Configure_Data_Path
  { .OCF = 0x84, .isret = 0, .coistart =   5, .numcoi = 3 }, // 183 7.3.102      HCI_Set_Min_Encryption_Key_Size
  { .OCF = 0x84, .isret = 1, .coistart =   4, .numcoi = 1 }, // 184 7.3.102    p HCI_Set_Min_Encryption_Key_Size
};

static const CodecTab_t CodecTab_3 = {
  .numtab = 185, .numcoi = 122, .table = TE_3, .CoI = CoI_3
};

static const CoI_t CoI_4[38] = {
  INFO(     7.4.6 ,   0) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, 
  INFO(     7.4.6 ,   2) { .num = 13, .skip = 1 }, 
  INFO(    7.4.10 ,   3) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  7, .skip = 1 }, 
  INFO(     7.4.5 ,   5)  /* 2 */ { .num =  3, .skip = 1 }, { .num =  4 }, 
  INFO(     7.4.7 ,   8) { .num =  7, .skip = 1 }, { .num =  6 }, 
  INFO(     7.4.4 ,  10) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, 
  INFO(     7.4.4 ,  13) { .action = copyws, .arg = 1, .inst = 1 }, { .LSB = 0x11 }, { .MSB = 0x00 }, 
  INFO(     7.4.9 ,  16) { .num =  9, .skip = 1 }, 
  INFO(    7.4.10 ,  17) { .num =  8 }, { .action = loop, .inst = 1 }, { .num =  1, .skip = 3 }, { .action = inlinefix, .arg = 1, .inst = 1 }, { .action = endloop, .inst = 1 }, 
  INFO(     7.4.8 ,  22) { .num =  8 }, { .action = laterfix, .arg = 2, .inst = 1 }, { .num =  1, .skip = 3 }, 
  INFO(     7.4.8 ,  25) { .num =  8 }, { .action = inlinefix, .arg = 1, .inst = 1 }, { .num =  2, .skip = 2 }, 
  INFO(     7.4.2 ,  28) { .action = copyws, .arg = 1, .inst = 1 }, { .LSB = 0x47 }, { .MSB = 0x00 }, 
  INFO(    7.4.11 ,  31) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  8 }, { .action = inlinefix, .inst = 1 }, 
  INFO(     7.4.3 ,  35) { .num = 15, .skip = 1 }, 
  INFO(     7.4.1 ,  36) { .num = 11, .skip = 1 }, { .num =  4 }, 
};

static const TE_t TE_4[24] = {
  { .OCF = 0x01, .isret = 0, .coistart =   3, .numcoi = 2 }, //   0 7.4.1      s HCI_Read_Local_Version_Information
  { .OCF = 0x01, .isret = 1, .coistart =  36, .numcoi = 2 }, //   1 7.4.1        HCI_Read_Local_Version_Information
  { .OCF = 0x02, .isret = 0, .coistart =   3, .numcoi = 2 }, //   2 7.4.2      s HCI_Read_Local_Supported_Commands
  { .OCF = 0x02, .isret = 1, .coistart =  28, .numcoi = 3 }, //   3 7.4.2        HCI_Read_Local_Supported_Commands
  { .OCF = 0x03, .isret = 0, .coistart =   3, .numcoi = 2 }, //   4 7.4.3      s HCI_Read_Local_Supported_Features
  { .OCF = 0x03, .isret = 1, .coistart =  35, .numcoi = 1 }, //   5 7.4.3        HCI_Read_Local_Supported_Features
  { .OCF = 0x04, .isret = 0, .coistart =  10, .numcoi = 3 }, //   6 7.4.4        HCI_Read_Local_Extended_Features
  { .OCF = 0x04, .isret = 1, .coistart =  13, .numcoi = 3 }, //   7 7.4.4        HCI_Read_Local_Extended_Features
  { .OCF = 0x05, .isret = 0, .coistart =   3, .numcoi = 2 }, //   8 7.4.5      s HCI_Read_Buffer_Size
  { .OCF = 0x05, .isret = 1, .coistart =   5, .numcoi = 3 }, //   9 7.4.5        HCI_Read_Buffer_Size
  { .OCF = 0x09, .isret = 0, .coistart =   0, .numcoi = 2 }, //  10 7.4.6      s HCI_Read_BD_ADDR
  { .OCF = 0x09, .isret = 1, .coistart =   2, .numcoi = 1 }, //  11 7.4.6      p HCI_Read_BD_ADDR
  { .OCF = 0x0a, .isret = 0, .coistart =   3, .numcoi = 2 }, //  12 7.4.7      s HCI_Read_Data_Block_Size
  { .OCF = 0x0a, .isret = 1, .coistart =   8, .numcoi = 2 }, //  13 7.4.7        HCI_Read_Data_Block_Size
  { .OCF = 0x0b, .isret = 0, .coistart =   3, .numcoi = 2 }, //  14 7.4.8      s HCI_Read_Local_Supported_Codecs [v1]
  { .OCF = 0x0b, .isret = 1, .coistart =  25, .numcoi = 3 }, //  15 7.4.8        HCI_Read_Local_Supported_Codecs [v1]
  { .OCF = 0x0c, .isret = 0, .coistart =   3, .numcoi = 2 }, //  16 7.4.9      s HCI_Read_Local_Simple_Pairing_Options
  { .OCF = 0x0c, .isret = 1, .coistart =  16, .numcoi = 1 }, //  17 7.4.9        HCI_Read_Local_Simple_Pairing_Options
  { .OCF = 0x0d, .isret = 0, .coistart =   3, .numcoi = 2 }, //  18 7.4.8      s HCI_Read_Local_Supported_Codecs [v2]
  { .OCF = 0x0d, .isret = 1, .coistart =  22, .numcoi = 3 }, //  19 7.4.8        HCI_Read_Local_Supported_Codecs [v2]
  { .OCF = 0x0e, .isret = 0, .coistart =   3, .numcoi = 3 }, //  20 7.4.10       HCI_Read_Local_Supported_Codec_Capabilities
  { .OCF = 0x0e, .isret = 1, .coistart =  17, .numcoi = 5 }, //  21 7.4.10       HCI_Read_Local_Supported_Codec_Capabilities
  { .OCF = 0x0f, .isret = 0, .coistart =  31, .numcoi = 4 }, //  22 7.4.11       HCI_Read_Local_Supported_Controller_Delay
  { .OCF = 0x0f, .isret = 1, .coistart =   2, .numcoi = 1 }, //  23 7.4.11       HCI_Read_Local_Supported_Controller_Delay
};

static const CodecTab_t CodecTab_4 = {
  .numtab = 24, .numcoi = 38, .table = TE_4, .CoI = CoI_4
};

static const CoI_t CoI_5[24] = {
  INFO(    7.5.11 ,   0) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, 
  INFO(    7.5.11 ,   2) { .num =  8 }, { .action = loop, .inst = 1 }, { .num =  2, .skip = 2 }, { .action = laterfix, .arg = 8, .inst = 1 }, { .action = endloop, .inst = 1 }, 
  INFO(     7.5.1 ,   7) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  2 }, 
  INFO(     7.5.5 ,  10) { .num =  7, .skip = 1 }, { .num = 13, .skip = 1 }, 
  INFO(     7.5.6 ,  12) { .num =  7, .skip = 1 }, { .num =  2, .skip = 2 }, { .num =  6, .skip = 2 }, 
  INFO(     7.5.3 ,  15) { .num =  7, .skip = 1 }, { .num =  3, .skip = 1 }, 
  INFO(     7.5.1 ,  17) { .num =  7, .skip = 1 }, { .num =  4 }, 
  INFO(     7.5.2 ,  19) { .num =  7, .skip = 1 }, { .num =  2 }, 
  INFO(    7.5.12 ,  21) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  6 }, 
};

static const TE_t TE_5[18] = {
  { .OCF = 0x01, .isret = 0, .coistart =   7, .numcoi = 3 }, //   0 7.5.1        HCI_Read_Failed_Contact_Counter
  { .OCF = 0x01, .isret = 1, .coistart =  17, .numcoi = 2 }, //   1 7.5.1        HCI_Read_Failed_Contact_Counter
  { .OCF = 0x02, .isret = 0, .coistart =   7, .numcoi = 3 }, //   2 7.5.2        HCI_Reset_Failed_Contact_Counter
  { .OCF = 0x02, .isret = 1, .coistart =  19, .numcoi = 2 }, //   3 7.5.2        HCI_Reset_Failed_Contact_Counter
  { .OCF = 0x03, .isret = 0, .coistart =   7, .numcoi = 3 }, //   4 7.5.3        HCI_Read_Link_Quality
  { .OCF = 0x03, .isret = 1, .coistart =  15, .numcoi = 2 }, //   5 7.5.3        HCI_Read_Link_Quality
  { .OCF = 0x05, .isret = 0, .coistart =   7, .numcoi = 3 }, //   6 7.5.4        HCI_Read_RSSI
  { .OCF = 0x05, .isret = 1, .coistart =  15, .numcoi = 2 }, //   7 7.5.4        HCI_Read_RSSI
  { .OCF = 0x06, .isret = 0, .coistart =   7, .numcoi = 3 }, //   8 7.5.5      p HCI_Read_AFH_Channel_Map
  { .OCF = 0x06, .isret = 1, .coistart =  10, .numcoi = 2 }, //   9 7.5.5        HCI_Read_AFH_Channel_Map
  { .OCF = 0x07, .isret = 0, .coistart =   7, .numcoi = 3 }, //  10 7.5.6        HCI_Read_Clock
  { .OCF = 0x07, .isret = 1, .coistart =  12, .numcoi = 3 }, //  11 7.5.6        HCI_Read_Clock
  { .OCF = 0x08, .isret = 0, .coistart =   7, .numcoi = 3 }, //  12 7.5.7      p HCI_Read_Encryption_Key_Size
  { .OCF = 0x08, .isret = 1, .coistart =  15, .numcoi = 2 }, //  13 7.5.7        HCI_Read_Encryption_Key_Size
  { .OCF = 0x0c, .isret = 0, .coistart =   0, .numcoi = 2 }, //  14 7.5.11     s HCI_Get_MWS_Transport_Layer_Configuration
  { .OCF = 0x0c, .isret = 1, .coistart =   2, .numcoi = 5 }, //  15 7.5.11       HCI_Get_MWS_Transport_Layer_Configuration
  { .OCF = 0x0d, .isret = 0, .coistart =  21, .numcoi = 3 }, //  16 7.5.12       HCI_Set_Triggered_Clock_Capture
  { .OCF = 0x0d, .isret = 1, .coistart =  10, .numcoi = 1 }, //  17 7.5.12     p HCI_Set_Triggered_Clock_Capture
};

static const CodecTab_t CodecTab_5 = {
  .numtab = 18, .numcoi = 24, .table = TE_5, .CoI = CoI_5
};

static const CoI_t CoI_6[14] = {
  INFO(     7.6.3 ,   0) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, 
  INFO(     7.6.2 ,   2) { .num =  7, .skip = 1 }, 
  INFO(     7.6.1 ,   3) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, 
  INFO(     7.6.1 ,   5) { .num =  8 }, 
  INFO(     7.6.2 ,   6) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, 
  INFO(     7.6.8 ,   9) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  4 }, 
  INFO(     7.6.8 ,  12) { .num =  7, .skip = 1 }, { .num =  2 }, 
};

static const TE_t TE_6[10] = {
  { .OCF = 0x01, .isret = 0, .coistart =   3, .numcoi = 2 }, //   0 7.6.1      s HCI_Read_Loopback_Mode
  { .OCF = 0x01, .isret = 1, .coistart =   5, .numcoi = 1 }, //   1 7.6.1        HCI_Read_Loopback_Mode
  { .OCF = 0x02, .isret = 0, .coistart =   6, .numcoi = 3 }, //   2 7.6.2        HCI_Write_Loopback_Mode
  { .OCF = 0x02, .isret = 1, .coistart =   2, .numcoi = 1 }, //   3 7.6.2      p HCI_Write_Loopback_Mode
  { .OCF = 0x03, .isret = 0, .coistart =   0, .numcoi = 2 }, //   4 7.6.3      s HCI_Enable_Device_Under_Test_Mode
  { .OCF = 0x03, .isret = 1, .coistart =   2, .numcoi = 1 }, //   5 7.6.3      p HCI_Enable_Device_Under_Test_Mode
  { .OCF = 0x04, .isret = 0, .coistart =   6, .numcoi = 3 }, //   6 7.6.4        HCI_Write_Simple_Pairing_Debug_Mode
  { .OCF = 0x04, .isret = 1, .coistart =   2, .numcoi = 1 }, //   7 7.6.4      p HCI_Write_Simple_Pairing_Debug_Mode
  { .OCF = 0x0a, .isret = 0, .coistart =   9, .numcoi = 3 }, //   8 7.6.8        HCI_Write_Secure_Connections_Test_Mode
  { .OCF = 0x0a, .isret = 1, .coistart =  12, .numcoi = 2 }, //   9 7.6.8      p HCI_Write_Secure_Connections_Test_Mode
};

static const CodecTab_t CodecTab_6 = {
  .numtab = 10, .numcoi = 14, .table = TE_6, .CoI = CoI_6
};

static const CodecTab_t CodecTab_7 = {
  .numtab = 0, .numcoi = 0, .table = NULL, .CoI = NULL
};

static const CoI_t CoI_8[233] = {
  INFO(   7.8.101 ,   0) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  2 }, 
  INFO(    7.8.16 ,   3) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  7, .skip = 1 }, 
  INFO(     7.8.1 ,   6) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  8 }, 
  INFO(    7.8.38 ,   9) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .action = copyws, .arg = 1, .inst = 1 }, { .LSB = 0x27 }, { .MSB = 0x00 }, 
  INFO(   7.8.106 ,  14) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, { .num =  7 }, { .action = inlinefix, .inst = 1 }, { .num =  1, .skip = 1 }, 
  INFO(    7.8.85 ,  20) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  4 }, 
  INFO(    7.8.25 ,  24) { .num =  7, .skip = 1 }, { .num =  2 }, 
  INFO(    7.8.18 ,  26) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num = 14 }, 
  INFO(   7.8.103 ,  29) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .action = copyws, .arg = 1, .inst = 1 }, { .LSB = 0x1f }, { .MSB = 0x00 }, 
  INFO(   7.8.104 ,  34) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  9, .skip = 1 }, { .action = copyws, .arg = 1, .inst = 1 }, { .LSB = 0x1b }, { .MSB = 0x00 }, 
  INFO(    7.8.99 ,  40) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, { .action = loop, .inst = 1 }, { .num =  4 }, { .action = endloop, .inst = 1 }, 
  INFO(    7.8.12 ,  46) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num = 13, .skip = 1 }, { .num = 12 }, 
  INFO(    7.8.24 ,  50) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .action = copyws, .inst = 1 }, { .LSB = 0x1c }, { .MSB = 0x00 }, 
  INFO(     7.8.7 ,  55) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .action = copyws, .inst = 1 }, { .LSB = 0x20 }, { .MSB = 0x00 }, 
  INFO(    7.8.22 ,  60) { .action = copyws, .arg = 1, .inst = 1 }, { .LSB = 0x17 }, { .MSB = 0x00 }, 
  INFO(   7.8.117 ,  63) { .num =  7, .skip = 1 }, { .num =  5, .skip = 1 }, 
  INFO(    7.8.66 ,  65) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num = 12 }, { .action = modif, .inst = 1 }, { .action = loop, .inst = 1 }, { .action = endloop, .inst = 1 }, 
  INFO(    7.8.66 ,  71) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num = 10 }, { .action = modif, .inst = 1 }, { .action = loop, .inst = 1 }, { .action = endloop, .inst = 1 }, 
  INFO(    7.8.37 ,  77) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .action = copyws, .arg = 1, .inst = 1 }, { .LSB = 0x41 }, { .MSB = 0x00 }, 
  INFO(    7.8.37 ,  82) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .action = copyws, .inst = 1 }, { .LSB = 0x40 }, { .MSB = 0x00 }, 
  INFO(    7.8.11 ,  87) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  2 }, 
  INFO(   7.8.113 ,  90) { .num =  7, .skip = 1 }, { .num =  2, .skip = 2 }, { .num = 12 }, 
  INFO(    7.8.25 ,  93) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .action = copyws, .inst = 1 }, { .LSB = 0x12 }, { .MSB = 0x00 }, 
  INFO(    7.8.67 ,  98) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  9, .skip = 1 }, { .num =  5, .skip = 1 }, 
  INFO(    7.8.19 , 102) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  5, .skip = 1 }, 
  INFO(     7.8.4 , 105) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  6 }, 
  INFO(     7.8.2 , 108) { .num =  7, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  3, .skip = 1 }, 
  INFO(    7.8.20 , 111) { .num =  7, .skip = 1 }, { .num =  7, .skip = 1 }, 
  INFO(   7.8.116 , 113) { .num =  7, .skip = 1 }, { .num =  2, .skip = 2 }, { .action = copyws, .inst = 1 }, { .LSB = 0x1c }, { .MSB = 0x00 }, 
  INFO(    7.8.96 , 118) { .num =  7, .skip = 1 }, { .num = 11, .skip = 1 }, 
  INFO(    7.8.46 , 120) { .num =  7, .skip = 1 }, { .num =  8 }, 
  INFO(    7.8.34 , 122) { .num =  7, .skip = 1 }, { .num =  4 }, 
  INFO(    7.8.28 , 124) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  7 }, { .action = inlinefix, .inst = 1 }, { .num =  1, .skip = 1 }, 
  INFO(     7.8.5 , 129) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num = 15, .skip = 1 }, 
  INFO(    7.8.97 , 132) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num = 15, .skip = 1 }, { .action = loop, .inst = 1 }, { .num =  1, .skip = 1 }, { .num =  8 }, { .action = endloop, .inst = 1 }, 
  INFO(    7.8.97 , 139) { .num =  9, .skip = 1 }, { .action = inlinefix, .inst = 1 }, { .num =  2 }, 
  INFO(    7.8.98 , 142) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  9, .skip = 1 }, { .num =  6 }, { .action = loop, .inst = 1 }, { .num = 14 }, { .action = endloop, .inst = 1 }, 
  INFO(    7.8.80 , 149) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  5 }, { .action = inlinefix, .inst = 1 }, { .num =  1, .skip = 1 }, 
  INFO(    7.8.84 , 154) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  4 }, { .action = inlinefix, .inst = 1 }, { .num =  1, .skip = 1 }, 
  INFO(    7.8.82 , 159) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  6 }, { .action = inlinefix, .inst = 1 }, { .num =  1, .skip = 1 }, 
  INFO(    7.8.92 , 164) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, { .num =  5, .skip = 1 }, 
  INFO(    7.8.56 , 168) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  2 }, { .action = loop, .inst = 1 }, { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .action = endloop, .inst = 1 }, 
  INFO(    7.8.53 , 175) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, { .action = copyws, .inst = 1 }, { .LSB = 0x1a }, { .MSB = 0x00 }, 
  INFO(    7.8.53 , 181) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, { .action = copyws, .inst = 1 }, { .LSB = 0x18 }, { .MSB = 0x00 }, 
  INFO(    7.8.64 , 187) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  3 }, { .action = modif, .inst = 1 }, { .action = loop, .inst = 1 }, { .action = endloop, .inst = 1 }, 
  INFO(    7.8.62 , 193) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  3 }, { .action = inlinefix, .inst = 1 }, 
  INFO(    7.8.61 , 197) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, { .num = 11, .skip = 1 }, 
  INFO(    7.8.10 , 201) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  1, .skip = 1 }, { .num =  6 }, 
  INFO(   7.8.126 , 205) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  8 }, { .action = inlinefix, .inst = 1 }, 
  INFO(   7.8.125 , 209) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  2 }, { .action = loop, .inst = 1 }, { .num =  4 }, { .action = inlinefix, .arg = 1, .inst = 1 }, { .action = endloop, .inst = 1 }, 
  INFO(    7.8.91 , 216) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  5, .skip = 1 }, 
  INFO(    7.8.49 , 220) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  5, .skip = 1 }, { .num =  2 }, 
  INFO(   7.8.109 , 224) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num = 13 }, { .action = inlinefix, .inst = 1 }, 
  INFO(    7.8.29 , 228) { .num =  1, .skip = 1 }, { .num =  3, .skip = 1 }, { .num =  7, .skip = 3 }, { .action = inlinefix, .arg = 1, .inst = 1 }, { .num =  2, .skip = 2 }, 
};

static const TE_t TE_8[242] = {
  { .OCF = 0x01, .isret = 0, .coistart =   6, .numcoi = 3 }, //   0 7.8.1        HCI_LE_Set_Event_Mask
  { .OCF = 0x01, .isret = 1, .coistart =   5, .numcoi = 1 }, //   1 7.8.1      p HCI_LE_Set_Event_Mask
  { .OCF = 0x02, .isret = 0, .coistart =   3, .numcoi = 2 }, //   2 7.8.2      s HCI_LE_Read_Buffer_Size [v1]
  { .OCF = 0x02, .isret = 1, .coistart = 108, .numcoi = 2 }, //   3 7.8.2        HCI_LE_Read_Buffer_Size [v1]
  { .OCF = 0x03, .isret = 0, .coistart =   3, .numcoi = 2 }, //   4 7.8.3      s HCI_LE_Read_Local_Supported_Features
  { .OCF = 0x03, .isret = 1, .coistart =  32, .numcoi = 1 }, //   5 7.8.3        HCI_LE_Read_Local_Supported_Features
  { .OCF = 0x05, .isret = 0, .coistart = 105, .numcoi = 3 }, //   6 7.8.4        HCI_LE_Set_Random_Address
  { .OCF = 0x05, .isret = 1, .coistart =   5, .numcoi = 1 }, //   7 7.8.4      p HCI_LE_Set_Random_Address
  { .OCF = 0x06, .isret = 0, .coistart = 129, .numcoi = 3 }, //   8 7.8.5        HCI_LE_Set_Advertising_Parameters
  { .OCF = 0x06, .isret = 1, .coistart =   5, .numcoi = 1 }, //   9 7.8.5      p HCI_LE_Set_Advertising_Parameters
  { .OCF = 0x07, .isret = 0, .coistart =   3, .numcoi = 2 }, //  10 7.8.6      s HCI_LE_Read_Advertising_Channel_Tx_Power
  { .OCF = 0x07, .isret = 1, .coistart =   8, .numcoi = 1 }, //  11 7.8.6        HCI_LE_Read_Advertising_Channel_Tx_Power
  { .OCF = 0x08, .isret = 0, .coistart =  55, .numcoi = 5 }, //  12 7.8.7        HCI_LE_Set_Advertising_Data
  { .OCF = 0x08, .isret = 1, .coistart =   5, .numcoi = 1 }, //  13 7.8.7      p HCI_LE_Set_Advertising_Data
  { .OCF = 0x09, .isret = 0, .coistart =  55, .numcoi = 5 }, //  14 7.8.8        HCI_LE_Set_Scan_Response_Data
  { .OCF = 0x09, .isret = 1, .coistart =   5, .numcoi = 1 }, //  15 7.8.8      p HCI_LE_Set_Scan_Response_Data
  { .OCF = 0x0a, .isret = 0, .coistart =  14, .numcoi = 3 }, //  16 7.8.9        HCI_LE_Set_Advertising_Enable
  { .OCF = 0x0a, .isret = 1, .coistart =   5, .numcoi = 1 }, //  17 7.8.9      p HCI_LE_Set_Advertising_Enable
  { .OCF = 0x0b, .isret = 0, .coistart = 201, .numcoi = 4 }, //  18 7.8.10       HCI_LE_Set_Scan_Parameters
  { .OCF = 0x0b, .isret = 1, .coistart =   5, .numcoi = 1 }, //  19 7.8.10     p HCI_LE_Set_Scan_Parameters
  { .OCF = 0x0c, .isret = 0, .coistart =  87, .numcoi = 3 }, //  20 7.8.11       HCI_LE_Set_Scan_Enable
  { .OCF = 0x0c, .isret = 1, .coistart =   5, .numcoi = 1 }, //  21 7.8.11     p HCI_LE_Set_Scan_Enable
  { .OCF = 0x0d, .isret = 0, .coistart =  46, .numcoi = 4 }, //  22 7.8.12       HCI_LE_Create_Connection
  { .OCF = 0x0e, .isret = 0, .coistart =   3, .numcoi = 2 }, //  23 7.8.13     s HCI_LE_Create_Connection_Cancel
  { .OCF = 0x0e, .isret = 1, .coistart =   5, .numcoi = 1 }, //  24 7.8.13     p HCI_LE_Create_Connection_Cancel
  { .OCF = 0x0f, .isret = 0, .coistart =   3, .numcoi = 2 }, //  25 7.8.14     s HCI_LE_Read_Filter_Accept_List_Size
  { .OCF = 0x0f, .isret = 1, .coistart =   8, .numcoi = 1 }, //  26 7.8.14       HCI_LE_Read_Filter_Accept_List_Size
  { .OCF = 0x10, .isret = 0, .coistart =   3, .numcoi = 2 }, //  27 7.8.15     s HCI_LE_Clear_Filter_Accept_List
  { .OCF = 0x10, .isret = 1, .coistart =   5, .numcoi = 1 }, //  28 7.8.15     p HCI_LE_Clear_Filter_Accept_List
  { .OCF = 0x11, .isret = 0, .coistart =   3, .numcoi = 3 }, //  29 7.8.16       HCI_LE_Add_Device_To_Filter_Accept_List
  { .OCF = 0x11, .isret = 1, .coistart =   5, .numcoi = 1 }, //  30 7.8.16     p HCI_LE_Add_Device_To_Filter_Accept_List
  { .OCF = 0x12, .isret = 0, .coistart =   3, .numcoi = 3 }, //  31 7.8.17       HCI_LE_Remove_Device_From_Filter_Accept_List
  { .OCF = 0x12, .isret = 1, .coistart =   5, .numcoi = 1 }, //  32 7.8.17     p HCI_LE_Remove_Device_From_Filter_Accept_List
  { .OCF = 0x13, .isret = 0, .coistart =  26, .numcoi = 3 }, //  33 7.8.18       HCI_LE_Connection_Update
  { .OCF = 0x14, .isret = 0, .coistart = 102, .numcoi = 3 }, //  34 7.8.19       HCI_LE_Set_Host_Channel_Classification
  { .OCF = 0x14, .isret = 1, .coistart =   5, .numcoi = 1 }, //  35 7.8.19     p HCI_LE_Set_Host_Channel_Classification
  { .OCF = 0x15, .isret = 0, .coistart =  87, .numcoi = 3 }, //  36 7.8.20     p HCI_LE_Read_Channel_Map
  { .OCF = 0x15, .isret = 1, .coistart = 111, .numcoi = 2 }, //  37 7.8.20       HCI_LE_Read_Channel_Map
  { .OCF = 0x16, .isret = 0, .coistart =  87, .numcoi = 3 }, //  38 7.8.21     p HCI_LE_Read_Remote_Features
  { .OCF = 0x17, .isret = 0, .coistart =  55, .numcoi = 5 }, //  39 7.8.22       HCI_LE_Encrypt
  { .OCF = 0x17, .isret = 1, .coistart =  60, .numcoi = 3 }, //  40 7.8.22       HCI_LE_Encrypt
  { .OCF = 0x18, .isret = 0, .coistart =   3, .numcoi = 2 }, //  41 7.8.23     s HCI_LE_Rand
  { .OCF = 0x18, .isret = 1, .coistart =  32, .numcoi = 1 }, //  42 7.8.23       HCI_LE_Rand
  { .OCF = 0x19, .isret = 0, .coistart =  50, .numcoi = 5 }, //  43 7.8.24       HCI_LE_Enable_Encryption
  { .OCF = 0x1a, .isret = 0, .coistart =  93, .numcoi = 5 }, //  44 7.8.25       HCI_LE_Long_Term_Key_Request_Reply
  { .OCF = 0x1a, .isret = 1, .coistart =  24, .numcoi = 2 }, //  45 7.8.25     p HCI_LE_Long_Term_Key_Request_Reply
  { .OCF = 0x1b, .isret = 0, .coistart =  87, .numcoi = 3 }, //  46 7.8.26     p HCI_LE_Long_Term_Key_Request_Negative_Reply
  { .OCF = 0x1b, .isret = 1, .coistart =  24, .numcoi = 2 }, //  47 7.8.26     p HCI_LE_Long_Term_Key_Request_Negative_Reply
  { .OCF = 0x1c, .isret = 0, .coistart =   3, .numcoi = 2 }, //  48 7.8.27     s HCI_LE_Read_Supported_States
  { .OCF = 0x1c, .isret = 1, .coistart =   5, .numcoi = 1 }, //  49 7.8.27       HCI_LE_Read_Supported_States
  { .OCF = 0x1d, .isret = 0, .coistart =  14, .numcoi = 3 }, //  50 7.8.28       HCI_LE_Receiver_Test [v1]
  { .OCF = 0x1d, .isret = 1, .coistart =   5, .numcoi = 1 }, //  51 7.8.28     p HCI_LE_Receiver_Test [v1]
  { .OCF = 0x1e, .isret = 0, .coistart =  20, .numcoi = 3 }, //  52 7.8.29       HCI_LE_Transmitter_Test [v1]
  { .OCF = 0x1e, .isret = 1, .coistart =   5, .numcoi = 1 }, //  53 7.8.29     p HCI_LE_Transmitter_Test [v1]
  { .OCF = 0x1f, .isret = 0, .coistart =   3, .numcoi = 2 }, //  54 7.8.30     s HCI_LE_Test_End
  { .OCF = 0x1f, .isret = 1, .coistart =  24, .numcoi = 2 }, //  55 7.8.30       HCI_LE_Test_End
  { .OCF = 0x20, .isret = 0, .coistart =  26, .numcoi = 3 }, //  56 7.8.31       HCI_LE_Remote_Connection_Parameter_Request_Reply
  { .OCF = 0x20, .isret = 1, .coistart =  24, .numcoi = 2 }, //  57 7.8.31     p HCI_LE_Remote_Connection_Parameter_Request_Reply
  { .OCF = 0x21, .isret = 0, .coistart =  20, .numcoi = 3 }, //  58 7.8.32       HCI_LE_Remote_Connection_Parameter_Request_Negative_Reply
  { .OCF = 0x21, .isret = 1, .coistart =  24, .numcoi = 2 }, //  59 7.8.32     p HCI_LE_Remote_Connection_Parameter_Request_Negative_Reply
  { .OCF = 0x22, .isret = 0, .coistart = 105, .numcoi = 3 }, //  60 7.8.33       HCI_LE_Set_Data_Length
  { .OCF = 0x22, .isret = 1, .coistart =  24, .numcoi = 2 }, //  61 7.8.33     p HCI_LE_Set_Data_Length
  { .OCF = 0x23, .isret = 0, .coistart =   3, .numcoi = 2 }, //  62 7.8.34     s HCI_LE_Read_Suggested_Default_Data_Length
  { .OCF = 0x23, .isret = 1, .coistart = 122, .numcoi = 2 }, //  63 7.8.34       HCI_LE_Read_Suggested_Default_Data_Length
  { .OCF = 0x24, .isret = 0, .coistart = 154, .numcoi = 3 }, //  64 7.8.35       HCI_LE_Write_Suggested_Default_Data_Length
  { .OCF = 0x24, .isret = 1, .coistart =   5, .numcoi = 1 }, //  65 7.8.35     p HCI_LE_Write_Suggested_Default_Data_Length
  { .OCF = 0x25, .isret = 0, .coistart =   3, .numcoi = 2 }, //  66 7.8.36     s HCI_LE_Read_Local_P-256_Public_Key
  { .OCF = 0x26, .isret = 0, .coistart =  82, .numcoi = 5 }, //  67 7.8.37       HCI_LE_Generate_DHKey [v1]
  { .OCF = 0x27, .isret = 0, .coistart =   9, .numcoi = 5 }, //  68 7.8.38       HCI_LE_Add_Device_To_Resolving_List
  { .OCF = 0x27, .isret = 1, .coistart =   5, .numcoi = 1 }, //  69 7.8.38     p HCI_LE_Add_Device_To_Resolving_List
  { .OCF = 0x28, .isret = 0, .coistart =   3, .numcoi = 3 }, //  70 7.8.39       HCI_LE_Remove_Device_From_Resolving_List
  { .OCF = 0x28, .isret = 1, .coistart =   5, .numcoi = 1 }, //  71 7.8.39     p HCI_LE_Remove_Device_From_Resolving_List
  { .OCF = 0x29, .isret = 0, .coistart =   3, .numcoi = 2 }, //  72 7.8.40     s HCI_LE_Clear_Resolving_List
  { .OCF = 0x29, .isret = 1, .coistart =   5, .numcoi = 1 }, //  73 7.8.40     p HCI_LE_Clear_Resolving_List
  { .OCF = 0x2a, .isret = 0, .coistart =   3, .numcoi = 2 }, //  74 7.8.41     s HCI_LE_Read_Resolving_List_Size
  { .OCF = 0x2a, .isret = 1, .coistart =   8, .numcoi = 1 }, //  75 7.8.41       HCI_LE_Read_Resolving_List_Size
  { .OCF = 0x2b, .isret = 0, .coistart =   3, .numcoi = 3 }, //  76 7.8.42       HCI_LE_Read_Peer_Resolvable_Address
  { .OCF = 0x2b, .isret = 1, .coistart =  48, .numcoi = 1 }, //  77 7.8.42       HCI_LE_Read_Peer_Resolvable_Address
  { .OCF = 0x2c, .isret = 0, .coistart =   3, .numcoi = 3 }, //  78 7.8.43       HCI_LE_Read_Local_Resolvable_Address
  { .OCF = 0x2c, .isret = 1, .coistart =  48, .numcoi = 1 }, //  79 7.8.43       HCI_LE_Read_Local_Resolvable_Address
  { .OCF = 0x2d, .isret = 0, .coistart =  14, .numcoi = 3 }, //  80 7.8.44       HCI_LE_Set_Address_Resolution_Enable
  { .OCF = 0x2d, .isret = 1, .coistart =   5, .numcoi = 1 }, //  81 7.8.44     p HCI_LE_Set_Address_Resolution_Enable
  { .OCF = 0x2e, .isret = 0, .coistart =  87, .numcoi = 3 }, //  82 7.8.45       HCI_LE_Set_Resolvable_Private_Address_Timeout
  { .OCF = 0x2e, .isret = 1, .coistart =   5, .numcoi = 1 }, //  83 7.8.45     p HCI_LE_Set_Resolvable_Private_Address_Timeout
  { .OCF = 0x2f, .isret = 0, .coistart =   3, .numcoi = 2 }, //  84 7.8.46     s HCI_LE_Read_Maximum_Data_Length
  { .OCF = 0x2f, .isret = 1, .coistart = 120, .numcoi = 2 }, //  85 7.8.46       HCI_LE_Read_Maximum_Data_Length
  { .OCF = 0x30, .isret = 0, .coistart =  87, .numcoi = 3 }, //  86 7.8.47     p HCI_LE_Read_PHY
  { .OCF = 0x30, .isret = 1, .coistart = 122, .numcoi = 2 }, //  87 7.8.47       HCI_LE_Read_PHY
  { .OCF = 0x31, .isret = 0, .coistart =  20, .numcoi = 3 }, //  88 7.8.48       HCI_LE_Set_Default_PHY
  { .OCF = 0x31, .isret = 1, .coistart =   5, .numcoi = 1 }, //  89 7.8.48     p HCI_LE_Set_Default_PHY
  { .OCF = 0x32, .isret = 0, .coistart = 220, .numcoi = 4 }, //  90 7.8.49       HCI_LE_Set_PHY
  { .OCF = 0x33, .isret = 0, .coistart =  20, .numcoi = 3 }, //  91 7.8.28       HCI_LE_Receiver_Test [v2]
  { .OCF = 0x33, .isret = 1, .coistart =   5, .numcoi = 1 }, //  92 7.8.28     p HCI_LE_Receiver_Test [v2]
  { .OCF = 0x34, .isret = 0, .coistart = 154, .numcoi = 3 }, //  93 7.8.29       HCI_LE_Transmitter_Test [v2]
  { .OCF = 0x34, .isret = 1, .coistart =   5, .numcoi = 1 }, //  94 7.8.29     p HCI_LE_Transmitter_Test [v2]
  { .OCF = 0x35, .isret = 0, .coistart =   3, .numcoi = 3 }, //  95 7.8.52       HCI_LE_Set_Advertising_Set_Random_Address
  { .OCF = 0x35, .isret = 1, .coistart =   5, .numcoi = 1 }, //  96 7.8.52     p HCI_LE_Set_Advertising_Set_Random_Address
  { .OCF = 0x36, .isret = 0, .coistart = 181, .numcoi = 6 }, //  97 7.8.53       HCI_LE_Set_Extended_Advertising_Parameters [v1]
  { .OCF = 0x36, .isret = 1, .coistart =   8, .numcoi = 1 }, //  98 7.8.53       HCI_LE_Set_Extended_Advertising_Parameters [v1]
  { .OCF = 0x37, .isret = 0, .coistart = 154, .numcoi = 4 }, //  99 7.8.54       HCI_LE_Set_Extended_Advertising_Data
  { .OCF = 0x37, .isret = 1, .coistart =   5, .numcoi = 1 }, // 100 7.8.54     p HCI_LE_Set_Extended_Advertising_Data
  { .OCF = 0x38, .isret = 0, .coistart = 154, .numcoi = 4 }, // 101 7.8.55       HCI_LE_Set_Extended_Scan_Response_Data
  { .OCF = 0x38, .isret = 1, .coistart =   5, .numcoi = 1 }, // 102 7.8.55     p HCI_LE_Set_Extended_Scan_Response_Data
  { .OCF = 0x39, .isret = 0, .coistart = 168, .numcoi = 7 }, // 103 7.8.56       HCI_LE_Set_Extended_Advertising_Enable
  { .OCF = 0x39, .isret = 1, .coistart =   5, .numcoi = 1 }, // 104 7.8.56     p HCI_LE_Set_Extended_Advertising_Enable
  { .OCF = 0x3a, .isret = 0, .coistart =   3, .numcoi = 2 }, // 105 7.8.57     s HCI_LE_Read_Maximum_Advertising_Data_Length
  { .OCF = 0x3a, .isret = 1, .coistart =  24, .numcoi = 2 }, // 106 7.8.57       HCI_LE_Read_Maximum_Advertising_Data_Length
  { .OCF = 0x3b, .isret = 0, .coistart =   3, .numcoi = 2 }, // 107 7.8.58     s HCI_LE_Read_Number_of_Supported_Advertising_Sets
  { .OCF = 0x3b, .isret = 1, .coistart =   8, .numcoi = 1 }, // 108 7.8.58       HCI_LE_Read_Number_of_Supported_Advertising_Sets
  { .OCF = 0x3c, .isret = 0, .coistart =  14, .numcoi = 3 }, // 109 7.8.59       HCI_LE_Remove_Advertising_Set
  { .OCF = 0x3c, .isret = 1, .coistart =   5, .numcoi = 1 }, // 110 7.8.59     p HCI_LE_Remove_Advertising_Set
  { .OCF = 0x3d, .isret = 0, .coistart =   3, .numcoi = 2 }, // 111 7.8.60     s HCI_LE_Clear_Advertising_Sets
  { .OCF = 0x3d, .isret = 1, .coistart =   5, .numcoi = 1 }, // 112 7.8.60     p HCI_LE_Clear_Advertising_Sets
  { .OCF = 0x3e, .isret = 0, .coistart = 201, .numcoi = 4 }, // 113 7.8.61       HCI_LE_Set_Periodic_Advertising_Parameters [v1]
  { .OCF = 0x3e, .isret = 1, .coistart =   5, .numcoi = 1 }, // 114 7.8.61     p HCI_LE_Set_Periodic_Advertising_Parameters [v1]
  { .OCF = 0x3f, .isret = 0, .coistart = 193, .numcoi = 4 }, // 115 7.8.62       HCI_LE_Set_Periodic_Advertising_Data
  { .OCF = 0x3f, .isret = 1, .coistart =   5, .numcoi = 1 }, // 116 7.8.62     p HCI_LE_Set_Periodic_Advertising_Data
  { .OCF = 0x40, .isret = 0, .coistart =  87, .numcoi = 3 }, // 117 7.8.63       HCI_LE_Set_Periodic_Advertising_Enable
  { .OCF = 0x40, .isret = 1, .coistart =   5, .numcoi = 1 }, // 118 7.8.63     p HCI_LE_Set_Periodic_Advertising_Enable
  { .OCF = 0x41, .isret = 0, .coistart = 187, .numcoi = 6 }, // 119 7.8.64       HCI_LE_Set_Extended_Scan_Parameters
  { .OCF = 0x41, .isret = 1, .coistart =   5, .numcoi = 1 }, // 120 7.8.64     p HCI_LE_Set_Extended_Scan_Parameters
  { .OCF = 0x42, .isret = 0, .coistart = 105, .numcoi = 3 }, // 121 7.8.65       HCI_LE_Set_Extended_Scan_Enable
  { .OCF = 0x42, .isret = 1, .coistart =   5, .numcoi = 1 }, // 122 7.8.65     p HCI_LE_Set_Extended_Scan_Enable
  { .OCF = 0x43, .isret = 0, .coistart =  71, .numcoi = 6 }, // 123 7.8.66       HCI_LE_Extended_Create_Connection [v1]
  { .OCF = 0x44, .isret = 0, .coistart =  98, .numcoi = 4 }, // 124 7.8.67       HCI_LE_Periodic_Advertising_Create_Sync
  { .OCF = 0x45, .isret = 0, .coistart =   3, .numcoi = 2 }, // 125 7.8.68     s HCI_LE_Periodic_Advertising_Create_Sync_Cancel
  { .OCF = 0x45, .isret = 1, .coistart =   5, .numcoi = 1 }, // 126 7.8.68     p HCI_LE_Periodic_Advertising_Create_Sync_Cancel
  { .OCF = 0x46, .isret = 0, .coistart =  87, .numcoi = 3 }, // 127 7.8.69       HCI_LE_Periodic_Advertising_Terminate_Sync
  { .OCF = 0x46, .isret = 1, .coistart =   5, .numcoi = 1 }, // 128 7.8.69     p HCI_LE_Periodic_Advertising_Terminate_Sync
  { .OCF = 0x47, .isret = 0, .coistart =   6, .numcoi = 3 }, // 129 7.8.70       HCI_LE_Add_Device_To_Periodic_Advertiser_List
  { .OCF = 0x47, .isret = 1, .coistart =   5, .numcoi = 1 }, // 130 7.8.70     p HCI_LE_Add_Device_To_Periodic_Advertiser_List
  { .OCF = 0x48, .isret = 0, .coistart =   6, .numcoi = 3 }, // 131 7.8.71       HCI_LE_Remove_Device_From_Periodic_Advertiser_List
  { .OCF = 0x48, .isret = 1, .coistart =   5, .numcoi = 1 }, // 132 7.8.71     p HCI_LE_Remove_Device_From_Periodic_Advertiser_List
  { .OCF = 0x49, .isret = 0, .coistart =   3, .numcoi = 2 }, // 133 7.8.72     s HCI_LE_Clear_Periodic_Advertiser_List
  { .OCF = 0x49, .isret = 1, .coistart =   5, .numcoi = 1 }, // 134 7.8.72     p HCI_LE_Clear_Periodic_Advertiser_List
  { .OCF = 0x4a, .isret = 0, .coistart =   3, .numcoi = 2 }, // 135 7.8.73     s HCI_LE_Read_Periodic_Advertiser_List_Size
  { .OCF = 0x4a, .isret = 1, .coistart =   8, .numcoi = 1 }, // 136 7.8.73       HCI_LE_Read_Periodic_Advertiser_List_Size
  { .OCF = 0x4b, .isret = 0, .coistart =   3, .numcoi = 2 }, // 137 7.8.74     s HCI_LE_Read_Transmit_Power
  { .OCF = 0x4b, .isret = 1, .coistart =  36, .numcoi = 1 }, // 138 7.8.74       HCI_LE_Read_Transmit_Power
  { .OCF = 0x4c, .isret = 0, .coistart =   3, .numcoi = 2 }, // 139 7.8.75     s HCI_LE_Read_RF_Path_Compensation
  { .OCF = 0x4c, .isret = 1, .coistart = 122, .numcoi = 2 }, // 140 7.8.75       HCI_LE_Read_RF_Path_Compensation
  { .OCF = 0x4d, .isret = 0, .coistart = 154, .numcoi = 3 }, // 141 7.8.76       HCI_LE_Write_RF_Path_Compensation
  { .OCF = 0x4d, .isret = 1, .coistart =   5, .numcoi = 1 }, // 142 7.8.76     p HCI_LE_Write_RF_Path_Compensation
  { .OCF = 0x4e, .isret = 0, .coistart =   6, .numcoi = 3 }, // 143 7.8.77       HCI_LE_Set_Privacy_Mode
  { .OCF = 0x4e, .isret = 1, .coistart =   5, .numcoi = 1 }, // 144 7.8.77     p HCI_LE_Set_Privacy_Mode
  { .OCF = 0x4f, .isret = 0, .coistart = 124, .numcoi = 5 }, // 145 7.8.28       HCI_LE_Receiver_Test [v3]
  { .OCF = 0x4f, .isret = 1, .coistart =   5, .numcoi = 1 }, // 146 7.8.28     p HCI_LE_Receiver_Test [v3]
  { .OCF = 0x50, .isret = 0, .coistart = 124, .numcoi = 5 }, // 147 7.8.29       HCI_LE_Transmitter_Test [v3]
  { .OCF = 0x50, .isret = 1, .coistart =   5, .numcoi = 1 }, // 148 7.8.29     p HCI_LE_Transmitter_Test [v3]
  { .OCF = 0x51, .isret = 0, .coistart = 149, .numcoi = 5 }, // 149 7.8.80       HCI_LE_Set_Connectionless_CTE_Transmit_Parameters
  { .OCF = 0x51, .isret = 1, .coistart =   5, .numcoi = 1 }, // 150 7.8.80     p HCI_LE_Set_Connectionless_CTE_Transmit_Parameters
  { .OCF = 0x52, .isret = 0, .coistart =  87, .numcoi = 3 }, // 151 7.8.81       HCI_LE_Set_Connectionless_CTE_Transmit_Enable
  { .OCF = 0x52, .isret = 1, .coistart =   5, .numcoi = 1 }, // 152 7.8.81     p HCI_LE_Set_Connectionless_CTE_Transmit_Enable
  { .OCF = 0x53, .isret = 0, .coistart = 159, .numcoi = 5 }, // 153 7.8.82       HCI_LE_Set_Connectionless_IQ_Sampling_Enable
  { .OCF = 0x53, .isret = 1, .coistart =  24, .numcoi = 2 }, // 154 7.8.82       HCI_LE_Set_Connectionless_IQ_Sampling_Enable
  { .OCF = 0x54, .isret = 0, .coistart = 149, .numcoi = 5 }, // 155 7.8.83       HCI_LE_Set_Connection_CTE_Receive_Parameters
  { .OCF = 0x54, .isret = 1, .coistart =  24, .numcoi = 2 }, // 156 7.8.83     p HCI_LE_Set_Connection_CTE_Receive_Parameters
  { .OCF = 0x55, .isret = 0, .coistart = 154, .numcoi = 5 }, // 157 7.8.84       HCI_LE_Set_Connection_CTE_Transmit_Parameters
  { .OCF = 0x55, .isret = 1, .coistart =  24, .numcoi = 2 }, // 158 7.8.84     p HCI_LE_Set_Connection_CTE_Transmit_Parameters
  { .OCF = 0x56, .isret = 0, .coistart =  20, .numcoi = 4 }, // 159 7.8.85       HCI_LE_Connection_CTE_Request_Enable
  { .OCF = 0x56, .isret = 1, .coistart =  24, .numcoi = 2 }, // 160 7.8.85     p HCI_LE_Connection_CTE_Request_Enable
  { .OCF = 0x57, .isret = 0, .coistart =  20, .numcoi = 3 }, // 161 7.8.86       HCI_LE_Connection_CTE_Response_Enable
  { .OCF = 0x57, .isret = 1, .coistart =  24, .numcoi = 2 }, // 162 7.8.86     p HCI_LE_Connection_CTE_Response_Enable
  { .OCF = 0x58, .isret = 0, .coistart =   3, .numcoi = 2 }, // 163 7.8.87     s HCI_LE_Read_Antenna_Information
  { .OCF = 0x58, .isret = 1, .coistart =  38, .numcoi = 1 }, // 164 7.8.87       HCI_LE_Read_Antenna_Information
  { .OCF = 0x59, .isret = 0, .coistart =  20, .numcoi = 3 }, // 165 7.8.88       HCI_LE_Set_Periodic_Advertising_Receive_Enable
  { .OCF = 0x59, .isret = 1, .coistart =   5, .numcoi = 1 }, // 166 7.8.88     p HCI_LE_Set_Periodic_Advertising_Receive_Enable
  { .OCF = 0x5a, .isret = 0, .coistart = 105, .numcoi = 3 }, // 167 7.8.89       HCI_LE_Periodic_Advertising_Sync_Transfer
  { .OCF = 0x5a, .isret = 1, .coistart =  24, .numcoi = 2 }, // 168 7.8.89     p HCI_LE_Periodic_Advertising_Sync_Transfer
  { .OCF = 0x5b, .isret = 0, .coistart = 102, .numcoi = 3 }, // 169 7.8.90       HCI_LE_Periodic_Advertising_Set_Info_Transfer
  { .OCF = 0x5b, .isret = 1, .coistart =  24, .numcoi = 2 }, // 170 7.8.90     p HCI_LE_Periodic_Advertising_Set_Info_Transfer
  { .OCF = 0x5c, .isret = 0, .coistart = 216, .numcoi = 4 }, // 171 7.8.91       HCI_LE_Set_Periodic_Advertising_Sync_Transfer_Parameters
  { .OCF = 0x5c, .isret = 1, .coistart =  24, .numcoi = 2 }, // 172 7.8.91     p HCI_LE_Set_Periodic_Advertising_Sync_Transfer_Parameters
  { .OCF = 0x5d, .isret = 0, .coistart = 164, .numcoi = 4 }, // 173 7.8.92       HCI_LE_Set_Default_Periodic_Advertising_Sync_Transfer_Parameters
  { .OCF = 0x5d, .isret = 1, .coistart =   5, .numcoi = 1 }, // 174 7.8.92     p HCI_LE_Set_Default_Periodic_Advertising_Sync_Transfer_Parameters
  { .OCF = 0x5e, .isret = 0, .coistart =  77, .numcoi = 5 }, // 175 7.8.37       HCI_LE_Generate_DHKey [v2]
  { .OCF = 0x5f, .isret = 0, .coistart =  14, .numcoi = 3 }, // 176 7.8.94       HCI_LE_Modify_Sleep_Clock_Accuracy
  { .OCF = 0x5f, .isret = 1, .coistart =   5, .numcoi = 1 }, // 177 7.8.94     p HCI_LE_Modify_Sleep_Clock_Accuracy
  { .OCF = 0x60, .isret = 0, .coistart =   3, .numcoi = 2 }, // 178 7.8.2      s HCI_LE_Read_Buffer_Size [v2]
  { .OCF = 0x60, .isret = 1, .coistart = 108, .numcoi = 3 }, // 179 7.8.2        HCI_LE_Read_Buffer_Size [v2]
  { .OCF = 0x61, .isret = 0, .coistart =  87, .numcoi = 3 }, // 180 7.8.96     p HCI_LE_Read_ISO_TX_Sync
  { .OCF = 0x61, .isret = 1, .coistart = 118, .numcoi = 2 }, // 181 7.8.96       HCI_LE_Read_ISO_TX_Sync
  { .OCF = 0x62, .isret = 0, .coistart = 132, .numcoi = 7 }, // 182 7.8.97       HCI_LE_Set_CIG_Parameters
  { .OCF = 0x62, .isret = 1, .coistart = 139, .numcoi = 3 }, // 183 7.8.97       HCI_LE_Set_CIG_Parameters
  { .OCF = 0x63, .isret = 0, .coistart = 142, .numcoi = 7 }, // 184 7.8.98       HCI_LE_Set_CIG_Parameters_Test
  { .OCF = 0x63, .isret = 1, .coistart = 139, .numcoi = 3 }, // 185 7.8.98       HCI_LE_Set_CIG_Parameters_Test
  { .OCF = 0x64, .isret = 0, .coistart =  40, .numcoi = 6 }, // 186 7.8.99       HCI_LE_Create_CIS
  { .OCF = 0x65, .isret = 0, .coistart =  14, .numcoi = 3 }, // 187 7.8.100      HCI_LE_Remove_CIG
  { .OCF = 0x65, .isret = 1, .coistart =   8, .numcoi = 1 }, // 188 7.8.100      HCI_LE_Remove_CIG
  { .OCF = 0x66, .isret = 0, .coistart =   0, .numcoi = 3 }, // 189 7.8.101    p HCI_LE_Accept_CIS_Request
  { .OCF = 0x67, .isret = 0, .coistart =  20, .numcoi = 3 }, // 190 7.8.102      HCI_LE_Reject_CIS_Request
  { .OCF = 0x67, .isret = 1, .coistart =  24, .numcoi = 2 }, // 191 7.8.102    p HCI_LE_Reject_CIS_Request
  { .OCF = 0x68, .isret = 0, .coistart =  29, .numcoi = 5 }, // 192 7.8.103      HCI_LE_Create_BIG
  { .OCF = 0x69, .isret = 0, .coistart =  34, .numcoi = 6 }, // 193 7.8.104      HCI_LE_Create_BIG_Test
  { .OCF = 0x6a, .isret = 0, .coistart =  87, .numcoi = 3 }, // 194 7.8.105      HCI_LE_Terminate_BIG
  { .OCF = 0x6b, .isret = 0, .coistart =  14, .numcoi = 6 }, // 195 7.8.106      HCI_LE_BIG_Create_Sync
  { .OCF = 0x6c, .isret = 0, .coistart =  14, .numcoi = 3 }, // 196 7.8.107      HCI_LE_BIG_Terminate_Sync
  { .OCF = 0x6c, .isret = 1, .coistart =   8, .numcoi = 1 }, // 197 7.8.107      HCI_LE_BIG_Terminate_Sync
  { .OCF = 0x6d, .isret = 0, .coistart =  87, .numcoi = 3 }, // 198 7.8.108    p HCI_LE_Request_Peer_SCA
  { .OCF = 0x6e, .isret = 0, .coistart = 224, .numcoi = 4 }, // 199 7.8.109      HCI_LE_Setup_ISO_Data_Path
  { .OCF = 0x6e, .isret = 1, .coistart =  24, .numcoi = 2 }, // 200 7.8.109    p HCI_LE_Setup_ISO_Data_Path
  { .OCF = 0x6f, .isret = 0, .coistart =  20, .numcoi = 3 }, // 201 7.8.110      HCI_LE_Remove_ISO_Data_Path
  { .OCF = 0x6f, .isret = 1, .coistart =  24, .numcoi = 2 }, // 202 7.8.110    p HCI_LE_Remove_ISO_Data_Path
  { .OCF = 0x70, .isret = 0, .coistart =  20, .numcoi = 3 }, // 203 7.8.111      HCI_LE_ISO_Transmit_Test
  { .OCF = 0x70, .isret = 1, .coistart =  24, .numcoi = 2 }, // 204 7.8.111    p HCI_LE_ISO_Transmit_Test
  { .OCF = 0x71, .isret = 0, .coistart =  20, .numcoi = 3 }, // 205 7.8.112      HCI_LE_ISO_Receive_Test
  { .OCF = 0x71, .isret = 1, .coistart =  24, .numcoi = 2 }, // 206 7.8.112    p HCI_LE_ISO_Receive_Test
  { .OCF = 0x72, .isret = 0, .coistart =  87, .numcoi = 3 }, // 207 7.8.113    p HCI_LE_ISO_Read_Test_Counters
  { .OCF = 0x72, .isret = 1, .coistart =  90, .numcoi = 3 }, // 208 7.8.113      HCI_LE_ISO_Read_Test_Counters
  { .OCF = 0x73, .isret = 0, .coistart =  87, .numcoi = 3 }, // 209 7.8.114    p HCI_LE_ISO_Test_End
  { .OCF = 0x73, .isret = 1, .coistart =  90, .numcoi = 3 }, // 210 7.8.114      HCI_LE_ISO_Test_End
  { .OCF = 0x74, .isret = 0, .coistart =  87, .numcoi = 3 }, // 211 7.8.115      HCI_LE_Set_Host_Feature
  { .OCF = 0x74, .isret = 1, .coistart =   5, .numcoi = 1 }, // 212 7.8.115    p HCI_LE_Set_Host_Feature
  { .OCF = 0x75, .isret = 0, .coistart =  87, .numcoi = 3 }, // 213 7.8.116    p HCI_LE_Read_ISO_Link_Quality
  { .OCF = 0x75, .isret = 1, .coistart = 113, .numcoi = 5 }, // 214 7.8.116      HCI_LE_Read_ISO_Link_Quality
  { .OCF = 0x76, .isret = 0, .coistart =  20, .numcoi = 3 }, // 215 7.8.117      HCI_LE_Enhanced_Read_Transmit_Power_Level
  { .OCF = 0x76, .isret = 1, .coistart =  63, .numcoi = 2 }, // 216 7.8.117      HCI_LE_Enhanced_Read_Transmit_Power_Level
  { .OCF = 0x77, .isret = 0, .coistart =  20, .numcoi = 3 }, // 217 7.8.118      HCI_LE_Read_Remote_Transmit_Power_Level
  { .OCF = 0x78, .isret = 0, .coistart =   6, .numcoi = 3 }, // 218 7.8.119      HCI_LE_Set_Path_Loss_Reporting_Parameters
  { .OCF = 0x78, .isret = 1, .coistart =  24, .numcoi = 2 }, // 219 7.8.119    p HCI_LE_Set_Path_Loss_Reporting_Parameters
  { .OCF = 0x79, .isret = 0, .coistart =  20, .numcoi = 3 }, // 220 7.8.120      HCI_LE_Set_Path_Loss_Reporting_Enable
  { .OCF = 0x79, .isret = 1, .coistart =  24, .numcoi = 2 }, // 221 7.8.120    p HCI_LE_Set_Path_Loss_Reporting_Enable
  { .OCF = 0x7a, .isret = 0, .coistart = 154, .numcoi = 3 }, // 222 7.8.121      HCI_LE_Set_Transmit_Power_Reporting_Enable
  { .OCF = 0x7a, .isret = 1, .coistart =  24, .numcoi = 2 }, // 223 7.8.121    p HCI_LE_Set_Transmit_Power_Reporting_Enable
  { .OCF = 0x7b, .isret = 0, .coistart = 228, .numcoi = 5 }, // 224 7.8.29       HCI_LE_Transmitter_Test [v4]
  { .OCF = 0x7b, .isret = 1, .coistart =   5, .numcoi = 1 }, // 225 7.8.29     p HCI_LE_Transmitter_Test [v4]
  { .OCF = 0x7c, .isret = 0, .coistart =  87, .numcoi = 3 }, // 226 7.8.122      HCI_LE_Set_Data_Related_Address_Changes
  { .OCF = 0x7c, .isret = 1, .coistart =   5, .numcoi = 1 }, // 227 7.8.122    p HCI_LE_Set_Data_Related_Address_Changes
  { .OCF = 0x7d, .isret = 0, .coistart =  71, .numcoi = 3 }, // 228 7.8.123      HCI_LE_Set_Default_Subrate
  { .OCF = 0x7d, .isret = 1, .coistart =   5, .numcoi = 1 }, // 229 7.8.123    p HCI_LE_Set_Default_Subrate
  { .OCF = 0x7e, .isret = 0, .coistart =  65, .numcoi = 3 }, // 230 7.8.124      HCI_LE_Subrate_Request
  { .OCF = 0x7f, .isret = 0, .coistart = 175, .numcoi = 6 }, // 231 7.8.53       HCI_LE_Set_Extended_Advertising_Parameters [v2]
  { .OCF = 0x7f, .isret = 1, .coistart =   8, .numcoi = 1 }, // 232 7.8.53       HCI_LE_Set_Extended_Advertising_Parameters [v2]
  { .OCF = 0x82, .isret = 0, .coistart = 209, .numcoi = 7 }, // 233 7.8.125      HCI_LE_Set_Periodic_Advertising_Subevent_Data
  { .OCF = 0x82, .isret = 1, .coistart =   8, .numcoi = 1 }, // 234 7.8.125      HCI_LE_Set_Periodic_Advertising_Subevent_Data
  { .OCF = 0x83, .isret = 0, .coistart = 205, .numcoi = 4 }, // 235 7.8.126      HCI_LE_Set_Periodic_Advertising_Response_Data
  { .OCF = 0x83, .isret = 1, .coistart =  24, .numcoi = 2 }, // 236 7.8.126      HCI_LE_Set_Periodic_Advertising_Response_Data
  { .OCF = 0x84, .isret = 0, .coistart = 149, .numcoi = 5 }, // 237 7.8.127      HCI_LE_Set_Periodic_Sync_Subevent
  { .OCF = 0x84, .isret = 1, .coistart =  24, .numcoi = 2 }, // 238 7.8.127      HCI_LE_Set_Periodic_Sync_Subevent
  { .OCF = 0x85, .isret = 0, .coistart =  65, .numcoi = 6 }, // 239 7.8.66       HCI_LE_Extended_Create_Connection [v2]
  { .OCF = 0x86, .isret = 0, .coistart = 197, .numcoi = 4 }, // 240 7.8.61       HCI_LE_Set_Periodic_Advertising_Parameters [v2]
  { .OCF = 0x86, .isret = 1, .coistart =   8, .numcoi = 1 }, // 241 7.8.61       HCI_LE_Set_Periodic_Advertising_Parameters [v2]
};

static const CodecTab_t CodecTab_8 = {
  .numtab = 242, .numcoi = 233, .table = TE_8, .CoI = CoI_8
};

const ctab_t ctabs[9] = {
  & CodecTab_0, & CodecTab_1, & CodecTab_2,  & CodecTab_3,
  & CodecTab_4, & CodecTab_5, & CodecTab_6,  & CodecTab_7,
  & CodecTab_8
};

