// Copyright 2024 Steven Buytaert

// File is generated from hci-spec2src-5.4.h

#include <stdint.h>

#define __packed__

typedef struct BD_Addr_t {
  uint8_t                  u08[6];
} BD_Addr_t;

typedef struct OGF_OCF_t {
  uint16_t                 OCF: 10;
  uint16_t                 OGF:  6;
} OGF_OCF_t;

typedef union Opcode_OGF_OCF_t {
  uint16_t                 opcode;
  struct {
    uint16_t               OCF: 10;
    uint16_t               OGF:  6;
  };
} Opcode_OGF_OCF_t;

typedef struct HCI_Cmd_t {
  union {
    uint16_t               opcode;
    struct {
      uint16_t             OCF: 10;
      uint16_t             OGF:  6;
    };
  };
  uint8_t                  length;
  uint8_t                  data[0];
} __packed__ HCI_Cmd_t;

typedef struct Status_BD_Addr_t {
  uint8_t                  Status;
  BD_Addr_t                BD_Addr;
} Status_BD_Addr_t;

typedef struct Status_Conn_Handle_t {
  uint8_t                  Status;
  uint16_t                 Conn_Handle;
} Status_Conn_Handle_t;

// 7.1.8 Accept Connection Request command

typedef struct HCI_Accept_Conn_Req_Cmd_t {
  BD_Addr_t                BD_Addr;
  uint8_t                  Role;
} HCI_Accept_Conn_Req_Cmd_t;

// 7.1.27 Accept Synchronous Connection Request command

typedef struct HCI_Accept_Sync_Conn_Req_Cmd_t {
  BD_Addr_t                BD_Addr;
  uint32_t                 Transmit_Bandwidth;
  uint32_t                 Receive_Bandwidth;
  uint16_t                 Max_Latency;
  uint16_t                 Voice_Setting;
  uint8_t                  Retransmission_Effort;
  uint16_t                 Packet_Type;
} HCI_Accept_Sync_Conn_Req_Cmd_t;

// 7.7.75 Authenticated Payload Timeout Expired event

// HCI_Authenticated_Payload_Timeout_Expired_Evt_t only has one member: Connection_Handle.

// 7.7.6 Authentication Complete event

// HCI_Authentication_Complete_Evt_t only has one member: Status_Conn.

// 7.1.15 Authentication Requested command

// HCI_Authentication_Requested_Cmd_t only has one member: Connection_Handle.

// 7.1.17 Change Connection Link Key command

// HCI_Change_Conn_Link_Key_Cmd_t only has one member: Connection_Handle.

// 7.7.9 Change Connection Link Key Complete event

// HCI_Change_Conn_Link_Key_Complete_Evt_t only has one member: Status_Conn.

// 7.1.14 Change Connection Packet Type command

typedef struct HCI_Change_Conn_Packet_Type_Cmd_t {
  uint16_t                 Connection_Handle;
  uint16_t                 Packet_Type;
} HCI_Change_Conn_Packet_Type_Cmd_t;

// 7.7.14 Command Complete event

typedef struct HCI_Command_Complete_Evt_t {
  uint8_t                  Num_HCI_Command_Packets;
  uint16_t                 Command_Opcode;
  uint8_t                  Return_Parameters;
} HCI_Command_Complete_Evt_t;

// 7.7.15 Command Status event

typedef struct HCI_Command_Status_Evt_t {
  uint8_t                  Status;
  uint8_t                  Num_HCI_Command_Packets;
  uint16_t                 Command_Opcode;
} HCI_Command_Status_Evt_t;

// 7.3.101 Configure Data Path command

typedef struct HCI_Configure_Data_Path_Cmd_t {
  uint8_t                  Data_Path_Direction;
  uint8_t                  Data_Path_ID;
  uint8_t                  Vendor_Specific_Config_Length;
  uint8_t  *               Vendor_Specific_Config;
} HCI_Configure_Data_Path_Cmd_t;

// HCI_Configure_Data_Path_Ret_t only has one member: Status.

// 7.7.3 Connection Complete event

typedef struct HCI_Conn_Complete_Evt_t {
  Status_Conn_Handle_t     Status_Conn;
  BD_Addr_t                BD_Addr;
  uint8_t                  Link_Type;
  uint8_t                  Encryption_Enabled;
} HCI_Conn_Complete_Evt_t;

// 7.7.29 Connection Packet Type Changed event

typedef struct HCI_Conn_Packet_Type_Changed_Evt_t {
  Status_Conn_Handle_t     Status_Conn;
  uint16_t                 Packet_Type;
} HCI_Conn_Packet_Type_Changed_Evt_t;

// 7.7.4 Connection Request event

typedef struct HCI_Conn_Req_Evt_t {
  BD_Addr_t                BD_Addr;
  uint8_t                  Class_Of_Device[3];
  uint8_t                  Link_Type;
} HCI_Conn_Req_Evt_t;

// 7.7.73 Connectionless Peripheral Broadcast Channel Map Change event

typedef struct HCI_Connectionless_PBCast_Channel_Map_Change_Evt_t {
  uint8_t                  Channel_Map[10];
} HCI_Connectionless_PBCast_Channel_Map_Change_Evt_t;

// 7.7.69 Connectionless Peripheral Broadcast Receive event

typedef struct HCI_Connectionless_PBCast_Receive_Evt_t {
  BD_Addr_t                BD_Addr;
  uint8_t                  LT_ADDR;
  uint32_t                 Clock;
  uint32_t                 Offset;
  uint8_t                  RX_Status;
  uint8_t                  Fragment;
  uint8_t                  Data_Length;
  uint8_t  *               Data;
} HCI_Connectionless_PBCast_Receive_Evt_t;

// 7.7.70 Connectionless Peripheral Broadcast Timeout event

typedef struct HCI_Connectionless_PBCast_Timeout_Evt_t {
  BD_Addr_t                BD_Addr;
  uint8_t                  LT_ADDR;
} HCI_Connectionless_PBCast_Timeout_Evt_t;

// 7.1.7 Create Connection Cancel command

typedef struct HCI_Create_Conn_Cancel_Cmd_t {
  BD_Addr_t                BD_Addr;
} HCI_Create_Conn_Cancel_Cmd_t;

// HCI_Create_Conn_Cancel_Ret_t only has one member: Status_BD_Addr.

// 7.1.5 Create Connection command

typedef struct HCI_Create_Connection_Cmd_t {
  BD_Addr_t                BD_Addr;
  uint16_t                 Packet_Type;
  uint8_t                  Page_Scan_Repetition_Mode;
  uint8_t                  Reserved;
  uint16_t                 Clock_Offset;
  uint8_t                  Allow_Role_Switch;
} HCI_Create_Connection_Cmd_t;

// 7.7.26 Data Buffer Overflow event

typedef struct HCI_Data_Buffer_Overflow_Evt_t {
  uint8_t                  Link_Type;
} HCI_Data_Buffer_Overflow_Evt_t;

// 7.3.87 Delete Reserved LT_ADDR command

typedef struct HCI_Delete_Reserved_LT_ADDR_Cmd_t {
  uint8_t                  LT_ADDR;
} HCI_Delete_Reserved_LT_ADDR_Cmd_t;

typedef struct HCI_Delete_Reserved_LT_ADDR_Ret_t {
  uint8_t                  Status;
  uint8_t                  LT_ADDR;
} HCI_Delete_Reserved_LT_ADDR_Ret_t;

// 7.3.10 Delete Stored Link Key command

typedef struct HCI_Delete_Stored_Link_Key_Cmd_t {
  BD_Addr_t                BD_Addr;
  uint8_t                  Delete_All;
} HCI_Delete_Stored_Link_Key_Cmd_t;

typedef struct HCI_Delete_Stored_Link_Key_Ret_t {
  uint8_t                  Status;
  uint16_t                 Num_Keys_Deleted;
} HCI_Delete_Stored_Link_Key_Ret_t;

// 7.1.6 Disconnect command

typedef struct HCI_Disconnect_Cmd_t {
  uint16_t                 Connection_Handle;
  uint8_t                  Reason;
} HCI_Disconnect_Cmd_t;

// 7.7.5 Disconnection Complete event

typedef struct HCI_Disconnection_Complete_Evt_t {
  Status_Conn_Handle_t     Status_Conn;
  uint8_t                  Reason;
} HCI_Disconnection_Complete_Evt_t;

// 7.6.3 Enable Device Under Test Mode command

// HCI_Enable_Device_Under_Test_Mode_Ret_t only has one member: Status.

// 7.7.8 Encryption Change event

typedef struct HCI_Encryption_Change_v2_Evt_t {
  Status_Conn_Handle_t     Status_Conn;
  uint8_t                  Encryption_Enabled;
  uint8_t                  Encryption_Key_Size;
} HCI_Encryption_Change_v2_Evt_t;

typedef struct HCI_Encryption_Change_v1_Evt_t {
  Status_Conn_Handle_t     Status_Conn;
  uint8_t                  Encryption_Enabled;
} HCI_Encryption_Change_v1_Evt_t;

// 7.7.39 Encryption Key Refresh Complete event

// HCI_Encryption_Key_Refresh_Complete_Evt_t only has one member: Status_Conn.

// 7.1.46 Enhanced Accept Synchronous Connection Request command

typedef struct HCI_Enhanced_Accept_Sync_Conn_Req_Cmd_t {
  BD_Addr_t                BD_Addr;
  uint32_t                 Transmit_Bandwidth;
  uint32_t                 Receive_Bandwidth;
  uint8_t                  Transmit_Coding_Format[5];
  uint8_t                  Receive_Coding_Format[5];
  uint16_t                 Transmit_Codec_Frame_Size;
  uint16_t                 Receive_Codec_Frame_Size;
  uint32_t                 Input_Bandwidth;
  uint32_t                 Output_Bandwidth;
  uint8_t                  Input_Coding_Format[5];
  uint8_t                  Output_Coding_Format[5];
  uint16_t                 Input_Coded_Data_Size;
  uint16_t                 Output_Coded_Data_Size;
  uint8_t                  Input_PCM_Data_Format;
  uint8_t                  Output_PCM_Data_Format;
  uint8_t                  Input_PCM_Sample_Payload_MSB_Position;
  uint8_t                  Output_PCM_Sample_Payload_MSB_Position;
  uint8_t                  Input_Data_Path;
  uint8_t                  Output_Data_Path;
  uint8_t                  Input_Transport_Unit_Size;
  uint8_t                  Output_Transport_Unit_Size;
  uint16_t                 Max_Latency;
  uint16_t                 Packet_Type;
  uint8_t                  Retransmission_Effort;
} HCI_Enhanced_Accept_Sync_Conn_Req_Cmd_t;

// 7.3.66 Enhanced Flush command

typedef struct HCI_Enhanced_Flush_Cmd_t {
  uint16_t                 Handle;
  uint8_t                  Packet_Type;
} HCI_Enhanced_Flush_Cmd_t;

// 7.7.47 Enhanced Flush Complete event

typedef struct HCI_Enhanced_Flush_Complete_Evt_t {
  uint16_t                 Handle;
} HCI_Enhanced_Flush_Complete_Evt_t;

// 7.1.45 Enhanced Setup Synchronous Connection command

typedef struct HCI_Enhanced_Setup_Sync_Connection_Cmd_t {
  uint16_t                 Connection_Handle;
  uint32_t                 Transmit_Bandwidth;
  uint32_t                 Receive_Bandwidth;
  uint8_t                  Transmit_Coding_Format[5];
  uint8_t                  Receive_Coding_Format[5];
  uint16_t                 Transmit_Codec_Frame_Size;
  uint16_t                 Receive_Codec_Frame_Size;
  uint32_t                 Input_Bandwidth;
  uint32_t                 Output_Bandwidth;
  uint8_t                  Input_Coding_Format[5];
  uint8_t                  Output_Coding_Format[5];
  uint16_t                 Input_Coded_Data_Size;
  uint16_t                 Output_Coded_Data_Size;
  uint8_t                  Input_PCM_Data_Format;
  uint8_t                  Output_PCM_Data_Format;
  uint8_t                  Input_PCM_Sample_Payload_MSB_Position;
  uint8_t                  Output_PCM_Sample_Payload_MSB_Position;
  uint8_t                  Input_Data_Path;
  uint8_t                  Output_Data_Path;
  uint8_t                  Input_Transport_Unit_Size;
  uint8_t                  Output_Transport_Unit_Size;
  uint16_t                 Max_Latency;
  uint16_t                 Packet_Type;
  uint8_t                  Retransmission_Effort;
} HCI_Enhanced_Setup_Sync_Connection_Cmd_t;

// 7.1.4 Exit Periodic Inquiry Mode command

// HCI_Exit_Periodic_Inquiry_Mode_Ret_t only has one member: Status.

// 7.2.3 Exit Sniff Mode command

// HCI_Exit_Sniff_Mode_Cmd_t only has one member: Connection_Handle.

// 7.7.38 Extended Inquiry Result event

typedef struct HCI_Extended_Inquiry_Result_Evt_t {
  uint8_t                  Num_Responses;
  BD_Addr_t                BD_Addr;
  uint8_t                  Page_Scan_Repetition_Mode;
  uint8_t                  Reserved;
  uint8_t                  Class_Of_Device[3];
  uint16_t                 Clock_Offset;
  uint8_t                  RSSI;
  uint8_t                  Extended_Inquiry_Response[240];
} HCI_Extended_Inquiry_Result_Evt_t;

// 7.2.13 Flow Specification command

typedef struct HCI_Flow_Specification_Cmd_t {
  uint16_t                 Connection_Handle;
  uint8_t                  Unused;
  uint8_t                  Flow_Direction;
  uint8_t                  Service_Type;
  uint32_t                 Token_Rate;
  uint32_t                 Token_Bucket_Size;
  uint32_t                 Peak_Bandwidth;
  uint32_t                 Access_Latency;
} HCI_Flow_Specification_Cmd_t;

// 7.7.32 Flow Specification Complete event

typedef struct HCI_Flow_Specification_Complete_Evt_t {
  Status_Conn_Handle_t     Status_Conn;
  uint8_t                  Unused;
  uint8_t                  Flow_Direction;
  uint8_t                  Service_Type;
  uint32_t                 Token_Rate;
  uint32_t                 Token_Bucket_Size;
  uint32_t                 Peak_Bandwidth;
  uint32_t                 Access_Latency;
} HCI_Flow_Specification_Complete_Evt_t;

// 7.3.4 Flush command

// HCI_Flush_Cmd_t only has one member: Connection_Handle.

// HCI_Flush_Ret_t only has one member: Status_Conn.

// 7.7.17 Flush Occurred event

typedef struct HCI_Flush_Occurred_Evt_t {
  uint16_t                 Handle;
} HCI_Flush_Occurred_Evt_t;

// 7.5.11 Get MWS Transport Layer Configuration command

typedef struct HCI_Get_MWS_Transport_Layer_Configuration_Ret_t {
  uint8_t                  Status;
  uint8_t                  Num_Transports;
  struct {
    uint8_t                Transport_Layer;
    uint8_t                Num_Baud_Rates;
    struct {
      uint32_t             To_MWS_Baud_Rate;
      uint32_t             From_MWS_Baud_Rate;
    } * Baud_Rates;
  } Transports[0];
} HCI_Get_MWS_Transport_Layer_Configuration_Ret_t;

// 7.7.16 Hardware Error event

typedef struct HCI_Hardware_Error_Evt_t {
  uint8_t                  Hardware_Code;
} HCI_Hardware_Error_Evt_t;

// 7.2.1 Hold Mode command

typedef struct HCI_Hold_Mode_Cmd_t {
  uint16_t                 Connection_Handle;
  uint16_t                 Hold_Mode_Max_Interval;
  uint16_t                 Hold_Mode_Min_Interval;
} HCI_Hold_Mode_Cmd_t;

// 7.3.39 Host Buffer Size command

typedef struct HCI_Host_Buffer_Size_Cmd_t {
  uint16_t                 Host_ACL_Data_Packet_Length;
  uint8_t                  Host_Sync_Data_Packet_Length;
  uint16_t                 Host_Total_Num_ACL_Data_Packets;
  uint16_t                 Host_Total_Num_Sync_Data_Packets;
} HCI_Host_Buffer_Size_Cmd_t;

// HCI_Host_Buffer_Size_Ret_t only has one member: Status.

// 7.3.40 Host Number Of Completed Packets command

typedef struct HCI_Host_Number_Of_Completed_Packets_Cmd_t {
  uint8_t                  Num_Handles;
  struct {
    uint16_t               Connection_Handle;
    uint16_t               Host_Num_Completed_Packets;
  } Handles[0];
} HCI_Host_Number_Of_Completed_Packets_Cmd_t;

// 7.1.2 Inquiry Cancel command

// HCI_Inquiry_Cancel_Ret_t only has one member: Status.

// 7.1.1 Inquiry command

typedef struct HCI_Inquiry_Cmd_t {
  uint8_t                  LAP[3];
  uint8_t                  Inquiry_Length;
  uint8_t                  Num_Responses;
} HCI_Inquiry_Cmd_t;

// 7.7.1 Inquiry Complete event

// HCI_Inquiry_Complete_Evt_t only has one member: Status.

// 7.7.74 Inquiry Response Notification event

typedef struct HCI_Inquiry_Response_Notification_Evt_t {
  uint8_t                  LAP[3];
  uint8_t                  RSSI;
} HCI_Inquiry_Response_Notification_Evt_t;

// 7.7.2 Inquiry Result event

typedef struct HCI_Inquiry_Result_Evt_t {
  uint8_t                  Num_Responses;
  struct {
    BD_Addr_t              BD_Addr;
    uint8_t                Page_Scan_Repetition_Mode;
    uint16_t               Reserved;
    uint8_t                Class_Of_Device[3];
    uint16_t               Clock_Offset;
  } Responses[0];
} HCI_Inquiry_Result_Evt_t;

// 7.7.33 Inquiry Result with RSSI event

typedef struct HCI_Inquiry_Result_with_RSSI_Evt_t {
  uint8_t                  Num_Responses;
  struct {
    BD_Addr_t              BD_Addr;
    uint8_t                Page_Scan_Repetition_Mode;
    uint8_t                Reserved;
    uint8_t                Class_Of_Device[3];
    uint16_t               Clock_Offset;
    uint8_t                RSSI;
  } Responses[0];
} HCI_Inquiry_Result_with_RSSI_Evt_t;

// 7.7.40 IO Capability Request event

typedef struct HCI_IO_Capability_Request_Evt_t {
  BD_Addr_t                BD_Addr;
} HCI_IO_Capability_Request_Evt_t;

// 7.1.36 IO Capability Request Negative Reply command

typedef struct HCI_IO_Capability_Req_Negative_Reply_Cmd_t {
  BD_Addr_t                BD_Addr;
  uint8_t                  Reason;
} HCI_IO_Capability_Req_Negative_Reply_Cmd_t;

// HCI_IO_Capability_Req_Negative_Reply_Ret_t only has one member: Status_BD_Addr.

// 7.1.29 IO Capability Request Reply command

typedef struct HCI_IO_Capability_Req_Reply_Cmd_t {
  BD_Addr_t                BD_Addr;
  uint8_t                  IO_Capability;
  uint8_t                  OOB_Data_Present;
  uint8_t                  Authentication_Requirements;
} HCI_IO_Capability_Req_Reply_Cmd_t;

// HCI_IO_Capability_Req_Reply_Ret_t only has one member: Status_BD_Addr.

// 7.7.41 IO Capability Response event

typedef struct HCI_IO_Capability_Response_Evt_t {
  BD_Addr_t                BD_Addr;
  uint8_t                  IO_Capability;
  uint8_t                  OOB_Data_Present;
  uint8_t                  Authentication_Requirements;
} HCI_IO_Capability_Response_Evt_t;

// 7.7.49 Keypress Notification event

typedef struct HCI_Keypress_Notification_Evt_t {
  BD_Addr_t                BD_Addr;
  uint8_t                  Notification_Type;
} HCI_Keypress_Notification_Evt_t;

// 7.8.101 LE Accept CIS Request command

// HCI_LE_Accept_CIS_Request_Cmd_t only has one member: Connection_Handle.

// 7.8.16 LE Add Device To Filter Accept List command

typedef struct HCI_LE_Add_Device_To_Filter_Accept_List_Cmd_t {
  uint8_t                  Address_Type;
  BD_Addr_t                Address;
} HCI_LE_Add_Device_To_Filter_Accept_List_Cmd_t;

// HCI_LE_Add_Device_To_Filter_Accept_List_Ret_t only has one member: Status.

// 7.8.70 LE Add Device To Periodic Advertiser List command

typedef struct HCI_LE_Add_Device_To_Periodic_Advertiser_List_Cmd_t {
  uint8_t                  Advertiser_Address_Type;
  BD_Addr_t                Advertiser_Address;
  uint8_t                  Advertising_SID;
} HCI_LE_Add_Device_To_Periodic_Advertiser_List_Cmd_t;

// HCI_LE_Add_Device_To_Periodic_Advertiser_List_Ret_t only has one member: Status.

// 7.8.38 LE Add Device To Resolving List command

typedef struct HCI_LE_Add_Device_To_Resolving_List_Cmd_t {
  uint8_t                  Peer_Identity_Address_Type;
  BD_Addr_t                Peer_Identity_Address;
  uint8_t                  Peer_IRK[16];
  uint8_t                  Local_IRK[16];
} HCI_LE_Add_Device_To_Resolving_List_Cmd_t;

// HCI_LE_Add_Device_To_Resolving_List_Ret_t only has one member: Status.

// 7.7.65.2 LE Advertising Report event

typedef struct HCI_LE_Adv_Report_Evt_t {
  uint8_t                  Subevent_Code;
  uint8_t                  Num_Reports;
  struct {
    uint8_t                Event_Type;
    uint8_t                Address_Type;
    BD_Addr_t              Address;
    uint8_t                Data_Length;
    uint8_t  *             Data;
    uint8_t                RSSI;
  } Reports[0];
} HCI_LE_Adv_Report_Evt_t;

// 7.7.65.18 LE Advertising Set Terminated event

typedef struct HCI_LE_Adv_Set_Terminated_Evt_t {
  uint8_t                  Subevent_Code;
  uint8_t                  Status;
  uint8_t                  Advertising_Handle;
  uint16_t                 Connection_Handle;
  uint8_t                  Num_Completed_Extended_Adv_Events;
} HCI_LE_Adv_Set_Terminated_Evt_t;

// 7.8.106 LE BIG Create Sync command

typedef struct HCI_LE_BIG_Create_Sync_Cmd_t {
  uint8_t                  BIG_Handle;
  uint16_t                 Sync_Handle;
  uint8_t                  Encryption;
  uint8_t                  Broadcast_Code[16];
  uint8_t                  MSE;
  uint16_t                 BIG_Sync_Timeout;
  uint8_t                  Num_BIS;
  uint8_t  *               BIS;
} HCI_LE_BIG_Create_Sync_Cmd_t;

// 7.7.65.29 LE BIG Sync Established event

typedef struct HCI_LE_BIG_Sync_Established_Evt_t {
  uint8_t                  Subevent_Code;
  uint8_t                  Status;
  uint8_t                  BIG_Handle;
  uint8_t                  Transport_Latency_BIG[3];
  uint8_t                  NSE;
  uint8_t                  BN;
  uint8_t                  PTO;
  uint8_t                  IRC;
  uint16_t                 Max_PDU;
  uint16_t                 ISO_Interval;
  uint8_t                  Num_BIS;
  uint8_t  *               Connection_Handle;
} HCI_LE_BIG_Sync_Established_Evt_t;

// 7.7.65.30 LE BIG Sync Lost event

typedef struct HCI_LE_BIG_Sync_Lost_Evt_t {
  uint8_t                  Subevent_Code;
  uint8_t                  BIG_Handle;
  uint8_t                  Reason;
} HCI_LE_BIG_Sync_Lost_Evt_t;

// 7.8.107 LE BIG Terminate Sync command

typedef struct HCI_LE_BIG_Terminate_Sync_Cmd_t {
  uint8_t                  BIG_Handle;
} HCI_LE_BIG_Terminate_Sync_Cmd_t;

typedef struct HCI_LE_BIG_Terminate_Sync_Ret_t {
  uint8_t                  Status;
  uint8_t                  BIG_Handle;
} HCI_LE_BIG_Terminate_Sync_Ret_t;

// 7.7.65.34 LE BIGInfo Advertising Report event

typedef struct HCI_LE_BIGInfo_Adv_Report_Evt_t {
  uint8_t                  Subevent_Code;
  uint16_t                 Sync_Handle;
  uint8_t                  Num_BIS;
  uint8_t                  NSE;
  uint16_t                 ISO_Interval;
  uint8_t                  BN;
  uint8_t                  PTO;
  uint8_t                  IRC;
  uint16_t                 Max_PDU;
  uint8_t                  SDU_Interval[3];
  uint16_t                 Max_SDU;
  uint8_t                  PHY;
  uint8_t                  Framing;
  uint8_t                  Encryption;
} HCI_LE_BIGInfo_Adv_Report_Evt_t;

// 7.7.65.20 LE Channel Selection Algorithm event

typedef struct HCI_LE_Channel_Selection_Algorithm_Evt_t {
  uint8_t                  Subevent_Code;
  uint16_t                 Connection_Handle;
  uint8_t                  Channel_Selection_Algorithm;
} HCI_LE_Channel_Selection_Algorithm_Evt_t;

// 7.7.65.25 LE CIS Established event

typedef struct HCI_LE_CIS_Established_Evt_t {
  uint8_t                  Subevent_Code;
  uint8_t                  Status;
  uint16_t                 Connection_Handle;
  uint8_t                  CIG_Sync_Delay[3];
  uint8_t                  CIS_Sync_Delay[3];
  uint8_t                  Transport_Latency_C_To_P[3];
  uint8_t                  Transport_Latency_P_To_C[3];
  uint8_t                  PHY_C_To_P;
  uint8_t                  PHY_P_To_C;
  uint8_t                  NSE;
  uint8_t                  BN_C_To_P;
  uint8_t                  BN_P_To_C;
  uint8_t                  FT_C_To_P;
  uint8_t                  FT_P_To_C;
  uint16_t                 Max_PDU_C_To_P;
  uint16_t                 Max_PDU_P_To_C;
  uint16_t                 ISO_Interval;
} HCI_LE_CIS_Established_Evt_t;

// 7.7.65.26 LE CIS Request event

typedef struct HCI_LE_CIS_Request_Evt_t {
  uint8_t                  Subevent_Code;
  uint16_t                 ACL_Conn_Handle;
  uint16_t                 CIS_Conn_Handle;
  uint8_t                  CIG_ID;
  uint8_t                  CIS_ID;
} HCI_LE_CIS_Request_Evt_t;

// 7.8.60 LE Clear Advertising Sets command

// HCI_LE_Clear_Adv_Sets_Ret_t only has one member: Status.

// 7.8.15 LE Clear Filter Accept List command

// HCI_LE_Clear_Filter_Accept_List_Ret_t only has one member: Status.

// 7.8.72 LE Clear Periodic Advertiser List command

// HCI_LE_Clear_Periodic_Advertiser_List_Ret_t only has one member: Status.

// 7.8.40 LE Clear Resolving List command

// HCI_LE_Clear_Resolving_List_Ret_t only has one member: Status.

// 7.7.65.1 LE Connection Complete event

typedef struct HCI_LE_Conn_Complete_Evt_t {
  uint8_t                  Subevent_Code;
  uint8_t                  Status;
  uint16_t                 Connection_Handle;
  uint8_t                  Role;
  uint8_t                  Peer_Address_Type;
  BD_Addr_t                Peer_Address;
  uint16_t                 Connection_Interval;
  uint16_t                 Peripheral_Latency;
  uint16_t                 Supervision_Timeout;
  uint8_t                  Central_Clock_Accuracy;
} HCI_LE_Conn_Complete_Evt_t;

// 7.8.85 LE Connection CTE Request Enable command

typedef struct HCI_LE_Conn_CTE_Req_Enable_Cmd_t {
  uint16_t                 Connection_Handle;
  uint8_t                  Enable;
  uint16_t                 CTE_Req_Interval;
  uint8_t                  Requested_CTE_Length;
  uint8_t                  Requested_CTE_Type;
} HCI_LE_Conn_CTE_Req_Enable_Cmd_t;

// HCI_LE_Conn_CTE_Req_Enable_Ret_t only has one member: Status_Conn.

// 7.8.86 LE Connection CTE Response Enable command

typedef struct HCI_LE_Conn_CTE_Response_Enable_Cmd_t {
  uint16_t                 Connection_Handle;
  uint8_t                  Enable;
} HCI_LE_Conn_CTE_Response_Enable_Cmd_t;

// HCI_LE_Conn_CTE_Response_Enable_Ret_t only has one member: Status_Conn.

// 7.7.65.22 LE Connection IQ Report event

typedef struct HCI_LE_Conn_IQ_Report_Evt_t {
  uint8_t                  Subevent_Code;
  uint16_t                 Connection_Handle;
  uint8_t                  RX_PHY;
  uint8_t                  Data_Channel_Index;
  uint16_t                 RSSI;
  uint8_t                  RSSI_Antenna_ID;
  uint8_t                  CTE_Type;
  uint8_t                  Slot_Durations;
  uint8_t                  Packet_Status;
  uint16_t                 Connection_Event_Counter;
  uint8_t                  Sample_Count;
  struct {
    uint8_t                I_Sample;
    uint8_t                Q_Sample;
  } Sample[0];
} HCI_LE_Conn_IQ_Report_Evt_t;

// 7.8.18 LE Connection Update command

typedef struct HCI_LE_Conn_Update_Cmd_t {
  uint16_t                 Connection_Handle;
  uint16_t                 Connection_Interval_Min;
  uint16_t                 Connection_Interval_Max;
  uint16_t                 Max_Latency;
  uint16_t                 Supervision_Timeout;
  uint16_t                 Min_CE_Length;
  uint16_t                 Max_CE_Length;
} HCI_LE_Conn_Update_Cmd_t;

// 7.7.65.3 LE Connection Update Complete event

typedef struct HCI_LE_Conn_Update_Complete_Evt_t {
  uint8_t                  Subevent_Code;
  uint8_t                  Status;
  uint16_t                 Connection_Handle;
  uint16_t                 Connection_Interval;
  uint16_t                 Peripheral_Latency;
  uint16_t                 Supervision_Timeout;
} HCI_LE_Conn_Update_Complete_Evt_t;

// 7.7.65.21 LE Connectionless IQ Report event

typedef struct HCI_LE_Connectionless_IQ_Report_Evt_t {
  uint8_t                  Subevent_Code;
  uint16_t                 Sync_Handle;
  uint8_t                  Channel_Index;
  uint16_t                 RSSI;
  uint8_t                  RSSI_Antenna_ID;
  uint8_t                  CTE_Type;
  uint8_t                  Slot_Durations;
  uint8_t                  Packet_Status;
  uint16_t                 Periodic_Event_Counter;
  uint8_t                  Sample_Count;
  struct {
    uint8_t                I_Sample;
    uint8_t                Q_Sample;
  } Sample[0];
} HCI_LE_Connectionless_IQ_Report_Evt_t;

// 7.8.103 LE Create BIG command

typedef struct HCI_LE_Create_BIG_Cmd_t {
  uint8_t                  BIG_Handle;
  uint8_t                  Advertising_Handle;
  uint8_t                  Num_BIS;
  uint8_t                  SDU_Interval[3];
  uint16_t                 Max_SDU;
  uint16_t                 Max_Transport_Latency;
  uint8_t                  RTN;
  uint8_t                  PHY;
  uint8_t                  Packing;
  uint8_t                  Framing;
  uint8_t                  Encryption;
  uint8_t                  Broadcast_Code[16];
} HCI_LE_Create_BIG_Cmd_t;

// 7.7.65.27 LE Create BIG Complete event

typedef struct HCI_LE_Create_BIG_Complete_Evt_t {
  uint8_t                  Subevent_Code;
  uint8_t                  Status;
  uint8_t                  BIG_Handle;
  uint8_t                  BIG_Sync_Delay[3];
  uint8_t                  Transport_Latency_BIG[3];
  uint8_t                  PHY;
  uint8_t                  NSE;
  uint8_t                  BN;
  uint8_t                  PTO;
  uint8_t                  IRC;
  uint16_t                 Max_PDU;
  uint16_t                 ISO_Interval;
  uint8_t                  Num_BIS;
  uint8_t  *               Connection_Handle;
} HCI_LE_Create_BIG_Complete_Evt_t;

// 7.8.104 LE Create BIG Test command

typedef struct HCI_LE_Create_BIG_Test_Cmd_t {
  uint8_t                  BIG_Handle;
  uint8_t                  Advertising_Handle;
  uint8_t                  Num_BIS;
  uint8_t                  SDU_Interval[3];
  uint16_t                 ISO_Interval;
  uint8_t                  NSE;
  uint16_t                 Max_SDU;
  uint16_t                 Max_PDU;
  uint8_t                  PHY;
  uint8_t                  Packing;
  uint8_t                  Framing;
  uint8_t                  BN;
  uint8_t                  IRC;
  uint8_t                  PTO;
  uint8_t                  Encryption;
  uint8_t                  Broadcast_Code[16];
} HCI_LE_Create_BIG_Test_Cmd_t;

// 7.8.99 LE Create CIS command

typedef struct HCI_LE_Create_CIS_Cmd_t {
  uint8_t                  CIS_Count;
  struct {
    uint16_t               CIS_Conn_Handle;
    uint16_t               ACL_Conn_Handle;
  } CIS[0];
} HCI_LE_Create_CIS_Cmd_t;

// 7.8.13 LE Create Connection Cancel command

// HCI_LE_Create_Conn_Cancel_Ret_t only has one member: Status.

// 7.8.12 LE Create Connection command

typedef struct HCI_LE_Create_Connection_Cmd_t {
  uint16_t                 LE_Scan_Interval;
  uint16_t                 LE_Scan_Window;
  uint8_t                  Initiator_Filter_Policy;
  uint8_t                  Peer_Address_Type;
  BD_Addr_t                Peer_Address;
  uint8_t                  Own_Address_Type;
  uint16_t                 Connection_Interval_Min;
  uint16_t                 Connection_Interval_Max;
  uint16_t                 Max_Latency;
  uint16_t                 Supervision_Timeout;
  uint16_t                 Min_CE_Length;
  uint16_t                 Max_CE_Length;
} HCI_LE_Create_Connection_Cmd_t;

// 7.7.65.23 LE CTE Request Failed event

typedef struct HCI_LE_CTE_Req_Failed_Evt_t {
  uint8_t                  Subevent_Code;
  uint8_t                  Status;
  uint16_t                 Connection_Handle;
} HCI_LE_CTE_Req_Failed_Evt_t;

// 7.7.65.7 LE Data Length Change event

typedef struct HCI_LE_Data_Length_Change_Evt_t {
  uint8_t                  Subevent_Code;
  uint16_t                 Connection_Handle;
  uint16_t                 Max_TX_Octets;
  uint16_t                 Max_TX_Time;
  uint16_t                 Max_RX_Octets;
  uint16_t                 Max_RX_Time;
} HCI_LE_Data_Length_Change_Evt_t;

// 7.7.65.11 LE Directed Advertising Report event

typedef struct HCI_LE_Directed_Adv_Report_Evt_t {
  uint8_t                  Subevent_Code;
  uint8_t                  Num_Reports;
  struct {
    uint8_t                Event_Type;
    uint8_t                Address_Type;
    BD_Addr_t              Address;
    uint8_t                Direct_Address_Type;
    BD_Addr_t              Direct_Address;
    uint8_t                RSSI;
  } Reports[0];
} HCI_LE_Directed_Adv_Report_Evt_t;

// 7.8.24 LE Enable Encryption command

typedef struct HCI_LE_Enable_Encryption_Cmd_t {
  uint16_t                 Connection_Handle;
  uint8_t                  Random_Number[8];
  uint16_t                 Encrypted_Diversifier;
  uint8_t                  Long_Term_Key[16];
} HCI_LE_Enable_Encryption_Cmd_t;

// 7.8.22 LE Encrypt command

typedef struct HCI_LE_Encrypt_Cmd_t {
  uint8_t                  Key[16];
  uint8_t                  Plaintext_Data[16];
} HCI_LE_Encrypt_Cmd_t;

typedef struct HCI_LE_Encrypt_Ret_t {
  uint8_t                  Status;
  uint8_t                  Encrypted_Data[16];
} HCI_LE_Encrypt_Ret_t;

// 7.7.65.10 LE Enhanced Connection Complete event

typedef struct HCI_LE_Enhanced_Conn_Complete_v2_Evt_t {
  uint8_t                  Subevent_Code;
  uint8_t                  Status;
  uint16_t                 Connection_Handle;
  uint8_t                  Role;
  uint8_t                  Peer_Address_Type;
  BD_Addr_t                Peer_Address;
  BD_Addr_t                Local_Resolvable_Private_Address;
  BD_Addr_t                Peer_Resolvable_Private_Address;
  uint16_t                 Connection_Interval;
  uint16_t                 Peripheral_Latency;
  uint16_t                 Supervision_Timeout;
  uint8_t                  Central_Clock_Accuracy;
  uint8_t                  Advertising_Handle;
  uint16_t                 Sync_Handle;
} HCI_LE_Enhanced_Conn_Complete_v2_Evt_t;

typedef struct HCI_LE_Enhanced_Conn_Complete_v1_Evt_t {
  uint8_t                  Subevent_Code;
  uint8_t                  Status;
  uint16_t                 Connection_Handle;
  uint8_t                  Role;
  uint8_t                  Peer_Address_Type;
  BD_Addr_t                Peer_Address;
  BD_Addr_t                Local_Resolvable_Private_Address;
  BD_Addr_t                Peer_Resolvable_Private_Address;
  uint16_t                 Connection_Interval;
  uint16_t                 Peripheral_Latency;
  uint16_t                 Supervision_Timeout;
  uint8_t                  Central_Clock_Accuracy;
} HCI_LE_Enhanced_Conn_Complete_v1_Evt_t;

// 7.8.117 LE Enhanced Read Transmit Power Level command

typedef struct HCI_LE_Enhanced_Read_Transmit_Power_Level_Cmd_t {
  uint16_t                 Connection_Handle;
  uint8_t                  PHY;
} HCI_LE_Enhanced_Read_Transmit_Power_Level_Cmd_t;

typedef struct HCI_LE_Enhanced_Read_Transmit_Power_Level_Ret_t {
  Status_Conn_Handle_t     Status_Conn;
  uint8_t                  PHY;
  uint8_t                  Current_TX_Power_Level;
  uint8_t                  Max_TX_Power_Level;
} HCI_LE_Enhanced_Read_Transmit_Power_Level_Ret_t;

// 7.7.65.13 LE Extended Advertising Report event

typedef struct HCI_LE_Extended_Adv_Report_Evt_t {
  uint8_t                  Subevent_Code;
  uint8_t                  Num_Reports;
  struct {
    uint16_t               Event_Type;
    uint8_t                Address_Type;
    BD_Addr_t              Address;
    uint8_t                Primary_PHY;
    uint8_t                Secondary_PHY;
    uint8_t                Advertising_SID;
    uint8_t                TX_Power;
    uint8_t                RSSI;
    uint16_t               Periodic_Adv_Interval;
    uint8_t                Direct_Address_Type;
    BD_Addr_t              Direct_Address;
    uint8_t                Data_Length;
    uint8_t  *             Data;
  } Reports[0];
} HCI_LE_Extended_Adv_Report_Evt_t;

// 7.8.66 LE Extended Create Connection command

typedef struct HCI_LE_Extended_Create_Connection_v2_Cmd_t {
  uint8_t                  Advertising_Handle;
  uint8_t                  Subevent;
  uint8_t                  Initiator_Filter_Policy;
  uint8_t                  Own_Address_Type;
  uint8_t                  Peer_Address_Type;
  BD_Addr_t                Peer_Address;
  uint8_t                  Initiating_PHYs;
  struct {
    uint8_t                Scan_Interval;
    uint8_t                Scan_Window;
    uint8_t                Connection_Interval_Min;
    uint8_t                Connection_Interval_Max;
    uint8_t                Max_Latency;
    uint8_t                Supervision_Timeout;
    uint8_t                Min_CE_Length;
    uint8_t                Max_CE_Length;
  } Initiating_PHYs_data[0];
} HCI_LE_Extended_Create_Connection_v2_Cmd_t;

typedef struct HCI_LE_Extended_Create_Connection_v1_Cmd_t {
  uint8_t                  Initiator_Filter_Policy;
  uint8_t                  Own_Address_Type;
  uint8_t                  Peer_Address_Type;
  BD_Addr_t                Peer_Address;
  uint8_t                  Initiating_PHYs;
  struct {
    uint8_t                Scan_Interval;
    uint8_t                Scan_Window;
    uint8_t                Connection_Interval_Min;
    uint8_t                Connection_Interval_Max;
    uint8_t                Max_Latency;
    uint8_t                Supervision_Timeout;
    uint8_t                Min_CE_Length;
    uint8_t                Max_CE_Length;
  } Initiating_PHYs_data[0];
} HCI_LE_Extended_Create_Connection_v1_Cmd_t;

// 7.8.37 LE Generate DHKey command

typedef struct HCI_LE_Generate_DHKey_v2_Cmd_t {
  uint8_t                  Key_X_Coordinate[32];
  uint8_t                  Key_Y_Coordinate[32];
  uint8_t                  Key_Type;
} HCI_LE_Generate_DHKey_v2_Cmd_t;

typedef struct HCI_LE_Generate_DHKey_v1_Cmd_t {
  uint8_t                  Key_X_Coordinate[32];
  uint8_t                  Key_Y_Coordinate[32];
} HCI_LE_Generate_DHKey_v1_Cmd_t;

// 7.7.65.9 LE Generate DHKey Complete event

typedef struct HCI_LE_Generate_DHKey_Complete_Evt_t {
  uint8_t                  Subevent_Code;
  uint8_t                  Status;
  uint8_t                  DH_Key[32];
} HCI_LE_Generate_DHKey_Complete_Evt_t;

// 7.8.113 LE ISO Read Test Counters command

// HCI_LE_ISO_Read_Test_Counters_Cmd_t only has one member: Connection_Handle.

typedef struct HCI_LE_ISO_Read_Test_Counters_Ret_t {
  Status_Conn_Handle_t     Status_Conn;
  uint32_t                 Received_SDU_Count;
  uint32_t                 Missed_SDU_Count;
  uint32_t                 Failed_SDU_Count;
} HCI_LE_ISO_Read_Test_Counters_Ret_t;

// 7.8.112 LE ISO Receive Test command

typedef struct HCI_LE_ISO_Receive_Test_Cmd_t {
  uint16_t                 Connection_Handle;
  uint8_t                  Payload_Type;
} HCI_LE_ISO_Receive_Test_Cmd_t;

// HCI_LE_ISO_Receive_Test_Ret_t only has one member: Status_Conn.

// 7.8.114 LE ISO Test End command

// HCI_LE_ISO_Test_End_Cmd_t only has one member: Connection_Handle.

typedef struct HCI_LE_ISO_Test_End_Ret_t {
  Status_Conn_Handle_t     Status_Conn;
  uint32_t                 Received_SDU_Count;
  uint32_t                 Missed_SDU_Count;
  uint32_t                 Failed_SDU_Count;
} HCI_LE_ISO_Test_End_Ret_t;

// 7.8.111 LE ISO Transmit Test command

typedef struct HCI_LE_ISO_Transmit_Test_Cmd_t {
  uint16_t                 Connection_Handle;
  uint8_t                  Payload_Type;
} HCI_LE_ISO_Transmit_Test_Cmd_t;

// HCI_LE_ISO_Transmit_Test_Ret_t only has one member: Status_Conn.

// 7.7.65.5 LE Long Term Key Request event

typedef struct HCI_LE_Long_Term_Key_Request_Evt_t {
  uint8_t                  Subevent_Code;
  uint16_t                 Connection_Handle;
  uint8_t                  Random_Number[8];
  uint16_t                 Encrypted_Diversifier;
} HCI_LE_Long_Term_Key_Request_Evt_t;

// 7.8.26 LE Long Term Key Request Negative Reply command

// HCI_LE_Long_Term_Key_Req_Negative_Reply_Cmd_t only has one member: Connection_Handle.

// HCI_LE_Long_Term_Key_Req_Negative_Reply_Ret_t only has one member: Status_Conn.

// 7.8.25 LE Long Term Key Request Reply command

typedef struct HCI_LE_Long_Term_Key_Req_Reply_Cmd_t {
  uint16_t                 Connection_Handle;
  uint8_t                  Long_Term_Key[16];
} HCI_LE_Long_Term_Key_Req_Reply_Cmd_t;

// HCI_LE_Long_Term_Key_Req_Reply_Ret_t only has one member: Status_Conn.

// 7.8.94 LE Modify Sleep Clock Accuracy command

typedef struct HCI_LE_Modify_Sleep_Clock_Accuracy_Cmd_t {
  uint8_t                  Action;
} HCI_LE_Modify_Sleep_Clock_Accuracy_Cmd_t;

// HCI_LE_Modify_Sleep_Clock_Accuracy_Ret_t only has one member: Status.

// 7.7.65.32 LE Path Loss Threshold event

typedef struct HCI_LE_Path_Loss_Threshold_Evt_t {
  uint8_t                  Subevent_Code;
  uint16_t                 Connection_Handle;
  uint8_t                  Current_Path_Loss;
  uint8_t                  Zone_Entered;
} HCI_LE_Path_Loss_Threshold_Evt_t;

// 7.8.68 LE Periodic Advertising Create Sync Cancel command

// HCI_LE_Periodic_Adv_Create_Sync_Cancel_Ret_t only has one member: Status.

// 7.8.67 LE Periodic Advertising Create Sync command

typedef struct HCI_LE_Periodic_Adv_Create_Sync_Cmd_t {
  uint8_t                  Options;
  uint8_t                  Advertising_SID;
  uint8_t                  Advertiser_Address_Type;
  BD_Addr_t                Advertiser_Address;
  uint16_t                 Skip;
  uint16_t                 Sync_Timeout;
  uint8_t                  Sync_CTE_Type;
} HCI_LE_Periodic_Adv_Create_Sync_Cmd_t;

// 7.7.65.15 LE Periodic Advertising Report event

typedef struct HCI_LE_Periodic_Adv_Report_v2_Evt_t {
  uint8_t                  Subevent_Code;
  uint16_t                 Sync_Handle;
  uint8_t                  TX_Power;
  uint8_t                  RSSI;
  uint8_t                  CTE_Type;
  uint16_t                 Periodic_Event_Counter;
  uint8_t                  Subevent;
  uint8_t                  Data_Status;
  uint8_t                  Data_Length;
  uint8_t  *               Data;
} HCI_LE_Periodic_Adv_Report_v2_Evt_t;

typedef struct HCI_LE_Periodic_Adv_Report_v1_Evt_t {
  uint8_t                  Subevent_Code;
  uint16_t                 Sync_Handle;
  uint8_t                  TX_Power;
  uint8_t                  RSSI;
  uint8_t                  CTE_Type;
  uint8_t                  Data_Status;
  uint8_t                  Data_Length;
  uint8_t  *               Data;
} HCI_LE_Periodic_Adv_Report_v1_Evt_t;

// 7.8.90 LE Periodic Advertising Set Info Transfer command

typedef struct HCI_LE_Periodic_Adv_Set_Info_Transfer_Cmd_t {
  uint16_t                 Connection_Handle;
  uint16_t                 Service_Data;
  uint8_t                  Advertising_Handle;
} HCI_LE_Periodic_Adv_Set_Info_Transfer_Cmd_t;

// HCI_LE_Periodic_Adv_Set_Info_Transfer_Ret_t only has one member: Status_Conn.

// 7.7.65.37 LE Periodic Advertising Response Report event

typedef struct HCI_LE_Periodic_Adv_Response_Report_Evt_t {
  uint8_t                  Subevent_Code;
  uint8_t                  Advertising_Handle;
  uint8_t                  Subevent;
  uint8_t                  Tx_Status;
  uint8_t                  Num_Responses;
  struct {
    uint8_t                Tx_Power;
    uint8_t                RSSI;
    uint8_t                CTE_Type;
    uint8_t                Response_Slot;
    uint8_t                Data_Status;
    uint8_t                Data_Length;
    uint8_t  *             Data;
  } Responses[0];
} HCI_LE_Periodic_Adv_Response_Report_Evt_t;

// 7.7.65.36 LE Periodic Advertising Subevent Data Request event

typedef struct HCI_LE_Periodic_Adv_Subevent_Data_Request_Evt_t {
  uint8_t                  Subevent_Code;
  uint8_t                  Advertising_Handle;
  uint8_t                  Subevent_Start;
  uint8_t                  Subevent_Data_Count;
} HCI_LE_Periodic_Adv_Subevent_Data_Request_Evt_t;

// 7.7.65.14 LE Periodic Advertising Sync Established event

typedef struct HCI_LE_Periodic_Adv_Sync_Established_v2_Evt_t {
  uint8_t                  Subevent_Code;
  uint8_t                  Status;
  uint16_t                 Sync_Handle;
  uint8_t                  Advertising_SID;
  uint8_t                  Advertiser_Address_Type;
  BD_Addr_t                Advertiser_Address;
  uint8_t                  Advertiser_PHY;
  uint16_t                 Periodic_Adv_Interval;
  uint8_t                  Advertiser_Clock_Accuracy;
  uint8_t                  Num_Subevents;
  uint8_t                  Subevent_Interval;
  uint8_t                  Response_Slot_Delay;
  uint8_t                  Response_Slot_Spacing;
} HCI_LE_Periodic_Adv_Sync_Established_v2_Evt_t;

typedef struct HCI_LE_Periodic_Adv_Sync_Established_v1_Evt_t {
  uint8_t                  Subevent_Code;
  uint8_t                  Status;
  uint16_t                 Sync_Handle;
  uint8_t                  Advertising_SID;
  uint8_t                  Advertiser_Address_Type;
  BD_Addr_t                Advertiser_Address;
  uint8_t                  Advertiser_PHY;
  uint16_t                 Periodic_Adv_Interval;
  uint8_t                  Advertiser_Clock_Accuracy;
} HCI_LE_Periodic_Adv_Sync_Established_v1_Evt_t;

// 7.7.65.16 LE Periodic Advertising Sync Lost event

typedef struct HCI_LE_Periodic_Adv_Sync_Lost_Evt_t {
  uint8_t                  Subevent_Code;
  uint16_t                 Sync_Handle;
} HCI_LE_Periodic_Adv_Sync_Lost_Evt_t;

// 7.8.89 LE Periodic Advertising Sync Transfer command

typedef struct HCI_LE_PAST_Cmd_t {
  uint16_t                 Connection_Handle;
  uint16_t                 Service_Data;
  uint16_t                 Sync_Handle;
} HCI_LE_PAST_Cmd_t;

// HCI_LE_PAST_Ret_t only has one member: Status_Conn.

// 7.7.65.24 LE Periodic Advertising Sync Transfer Received event

typedef struct HCI_LE_PAST_Received_v2_Evt_t {
  uint8_t                  Subevent_Code;
  uint8_t                  Status;
  uint16_t                 Connection_Handle;
  uint16_t                 Service_Data;
  uint16_t                 Sync_Handle;
  uint8_t                  Advertising_SID;
  uint8_t                  Advertiser_Address_Type;
  BD_Addr_t                Advertiser_Address;
  uint8_t                  Advertiser_PHY;
  uint16_t                 Periodic_Adv_Interval;
  uint8_t                  Advertiser_Clock_Accuracy;
  uint8_t                  Num_Subevents;
  uint8_t                  Subevent_Interval;
  uint8_t                  Response_Slot_Delay;
  uint8_t                  Response_Slot_Spacing;
} HCI_LE_PAST_Received_v2_Evt_t;

typedef struct HCI_LE_PAST_Received_v1_Evt_t {
  uint8_t                  Subevent_Code;
  uint8_t                  Status;
  uint16_t                 Connection_Handle;
  uint16_t                 Service_Data;
  uint16_t                 Sync_Handle;
  uint8_t                  Advertising_SID;
  uint8_t                  Advertiser_Address_Type;
  BD_Addr_t                Advertiser_Address;
  uint8_t                  Advertiser_PHY;
  uint16_t                 Periodic_Adv_Interval;
  uint8_t                  Advertiser_Clock_Accuracy;
} HCI_LE_PAST_Received_v1_Evt_t;

// 7.8.69 LE Periodic Advertising Terminate Sync command

typedef struct HCI_LE_Periodic_Adv_Terminate_Sync_Cmd_t {
  uint16_t                 Sync_Handle;
} HCI_LE_Periodic_Adv_Terminate_Sync_Cmd_t;

// HCI_LE_Periodic_Adv_Terminate_Sync_Ret_t only has one member: Status.

// 7.7.65.12 LE PHY Update Complete event

typedef struct HCI_LE_PHY_Update_Complete_Evt_t {
  uint8_t                  Subevent_Code;
  uint8_t                  Status;
  uint16_t                 Connection_Handle;
  uint8_t                  TX_PHY;
  uint8_t                  RX_PHY;
} HCI_LE_PHY_Update_Complete_Evt_t;

// 7.8.23 LE Rand command

typedef struct HCI_LE_Rand_Ret_t {
  uint8_t                  Status;
  uint8_t                  Random_Number[8];
} HCI_LE_Rand_Ret_t;

// 7.8.6 LE Read Advertising Physical Channel Tx Power command

typedef struct HCI_LE_Read_Adv_Channel_Tx_Power_Ret_t {
  uint8_t                  Status;
  uint8_t                  TX_Power_Level;
} HCI_LE_Read_Adv_Channel_Tx_Power_Ret_t;

// 7.8.87 LE Read Antenna Information command

typedef struct HCI_LE_Read_Antenna_Information_Ret_t {
  uint8_t                  Status;
  uint8_t                  Supported_Switching_Sampling_Rates;
  uint8_t                  Num_Antennae;
  uint8_t                  Max_Switching_Pattern_Length;
  uint8_t                  Max_CTE_Length;
} HCI_LE_Read_Antenna_Information_Ret_t;

// 7.8.2 LE Read Buffer Size command

typedef struct HCI_LE_Read_Buffer_Size_v2_Ret_t {
  uint8_t                  Status;
  uint16_t                 LE_ACL_Data_Packet_Length;
  uint8_t                  Total_Num_LE_ACL_Data_Packets;
  uint16_t                 ISO_Data_Packet_Length;
  uint8_t                  Total_Num_ISO_Data_Packets;
} HCI_LE_Read_Buffer_Size_v2_Ret_t;

typedef struct HCI_LE_Read_Buffer_Size_v1_Ret_t {
  uint8_t                  Status;
  uint16_t                 LE_ACL_Data_Packet_Length;
  uint8_t                  Total_Num_LE_ACL_Data_Packets;
} HCI_LE_Read_Buffer_Size_v1_Ret_t;

// 7.8.20 LE Read Channel Map command

// HCI_LE_Read_Channel_Map_Cmd_t only has one member: Connection_Handle.

typedef struct HCI_LE_Read_Channel_Map_Ret_t {
  Status_Conn_Handle_t     Status_Conn;
  uint8_t                  Channel_Map[5];
} HCI_LE_Read_Channel_Map_Ret_t;

// 7.8.14 LE Read Filter Accept List Size command

typedef struct HCI_LE_Read_Filter_Accept_List_Size_Ret_t {
  uint8_t                  Status;
  uint8_t                  Filter_Accept_List_Size;
} HCI_LE_Read_Filter_Accept_List_Size_Ret_t;

// 7.8.116 LE Read ISO Link Quality command

// HCI_LE_Read_ISO_Link_Quality_Cmd_t only has one member: Connection_Handle.

typedef struct HCI_LE_Read_ISO_Link_Quality_Ret_t {
  Status_Conn_Handle_t     Status_Conn;
  uint32_t                 TX_UnACKed_Packets;
  uint32_t                 TX_Flushed_Packets;
  uint32_t                 TX_Last_Subevent_Packets;
  uint32_t                 Retransmitted_Packets;
  uint32_t                 CRC_Error_Packets;
  uint32_t                 RX_Unreceived_Packets;
  uint32_t                 Duplicate_Packets;
} HCI_LE_Read_ISO_Link_Quality_Ret_t;

// 7.8.96 LE Read ISO TX Sync command

// HCI_LE_Read_ISO_TX_Sync_Cmd_t only has one member: Connection_Handle.

typedef struct HCI_LE_Read_ISO_TX_Sync_Ret_t {
  Status_Conn_Handle_t     Status_Conn;
  uint16_t                 Packet_Sequence_Number;
  uint32_t                 TX_Time_Stamp;
  uint8_t                  Time_Offset[3];
} HCI_LE_Read_ISO_TX_Sync_Ret_t;

// 7.7.65.8 LE Read Local P-256 Public Key Complete event

typedef struct HCI_LE_Read_Local_P_256_Public_Key_Complete_Evt_t {
  uint8_t                  Subevent_Code;
  uint8_t                  Status;
  uint8_t                  Key_X_Coordinate[32];
  uint8_t                  Key_Y_Coordinate[32];
} HCI_LE_Read_Local_P_256_Public_Key_Complete_Evt_t;

// 7.8.43 LE Read Local Resolvable Address command

typedef struct HCI_LE_Read_Local_Resolvable_Address_Cmd_t {
  uint8_t                  Peer_Identity_Address_Type;
  BD_Addr_t                Peer_Identity_Address;
} HCI_LE_Read_Local_Resolvable_Address_Cmd_t;

typedef struct HCI_LE_Read_Local_Resolvable_Address_Ret_t {
  uint8_t                  Status;
  BD_Addr_t                Local_Resolvable_Address;
} HCI_LE_Read_Local_Resolvable_Address_Ret_t;

// 7.8.3 LE Read Local Supported Features command

typedef struct HCI_LE_Read_Local_Supported_Features_Ret_t {
  uint8_t                  Status;
  uint8_t                  LE_Features[8];
} HCI_LE_Read_Local_Supported_Features_Ret_t;

// 7.8.57 LE Read Maximum Advertising Data Length command

typedef struct HCI_LE_Read_Maximum_Adv_Data_Length_Ret_t {
  uint8_t                  Status;
  uint16_t                 Max_Adv_Data_Length;
} HCI_LE_Read_Maximum_Adv_Data_Length_Ret_t;

// 7.8.46 LE Read Maximum Data Length command

typedef struct HCI_LE_Read_Maximum_Data_Length_Ret_t {
  uint8_t                  Status;
  uint16_t                 Supported_Max_TX_Octets;
  uint16_t                 Supported_Max_TX_Time;
  uint16_t                 Supported_Max_RX_Octets;
  uint16_t                 Supported_Max_RX_Time;
} HCI_LE_Read_Maximum_Data_Length_Ret_t;

// 7.8.58 LE Read Number of Supported Advertising Sets command

typedef struct HCI_LE_Read_Number_of_Supported_Adv_Sets_Ret_t {
  uint8_t                  Status;
  uint8_t                  Num_Supported_Adv_Sets;
} HCI_LE_Read_Number_of_Supported_Adv_Sets_Ret_t;

// 7.8.42 LE Read Peer Resolvable Address command

typedef struct HCI_LE_Read_Peer_Resolvable_Address_Cmd_t {
  uint8_t                  Peer_Identity_Address_Type;
  BD_Addr_t                Peer_Identity_Address;
} HCI_LE_Read_Peer_Resolvable_Address_Cmd_t;

typedef struct HCI_LE_Read_Peer_Resolvable_Address_Ret_t {
  uint8_t                  Status;
  BD_Addr_t                Peer_Resolvable_Address;
} HCI_LE_Read_Peer_Resolvable_Address_Ret_t;

// 7.8.73 LE Read Periodic Advertiser List Size command

typedef struct HCI_LE_Read_Periodic_Advertiser_List_Size_Ret_t {
  uint8_t                  Status;
  uint8_t                  Periodic_Advertiser_List_Size;
} HCI_LE_Read_Periodic_Advertiser_List_Size_Ret_t;

// 7.8.47 LE Read PHY command

// HCI_LE_Read_PHY_Cmd_t only has one member: Connection_Handle.

typedef struct HCI_LE_Read_PHY_Ret_t {
  Status_Conn_Handle_t     Status_Conn;
  uint8_t                  TX_PHY;
  uint8_t                  RX_PHY;
} HCI_LE_Read_PHY_Ret_t;

// 7.8.21 LE Read Remote Features command

// HCI_LE_Read_Remote_Features_Cmd_t only has one member: Connection_Handle.

// 7.7.65.4 LE Read Remote Features Complete event

typedef struct HCI_LE_Read_Remote_Features_Complete_Evt_t {
  uint8_t                  Subevent_Code;
  uint8_t                  Status;
  uint16_t                 Connection_Handle;
  uint8_t                  LE_Features[8];
} HCI_LE_Read_Remote_Features_Complete_Evt_t;

// 7.8.118 LE Read Remote Transmit Power Level command

typedef struct HCI_LE_Read_Remote_Transmit_Power_Level_Cmd_t {
  uint16_t                 Connection_Handle;
  uint8_t                  PHY;
} HCI_LE_Read_Remote_Transmit_Power_Level_Cmd_t;

// 7.8.41 LE Read Resolving List Size command

typedef struct HCI_LE_Read_Resolving_List_Size_Ret_t {
  uint8_t                  Status;
  uint8_t                  Resolving_List_Size;
} HCI_LE_Read_Resolving_List_Size_Ret_t;

// 7.8.75 LE Read RF Path Compensation command

typedef struct HCI_LE_Read_RF_Path_Compensation_Ret_t {
  uint8_t                  Status;
  uint16_t                 RF_TX_Path_Compensation_Value;
  uint16_t                 RF_RX_Path_Compensation_Value;
} HCI_LE_Read_RF_Path_Compensation_Ret_t;

// 7.8.34 LE Read Suggested Default Data Length command

typedef struct HCI_LE_Read_Suggested_Default_Data_Length_Ret_t {
  uint8_t                  Status;
  uint16_t                 Suggested_Max_TX_Octets;
  uint16_t                 Suggested_Max_TX_Time;
} HCI_LE_Read_Suggested_Default_Data_Length_Ret_t;

// 7.8.27 LE Read Supported States command

typedef struct HCI_LE_Read_Supported_States_Ret_t {
  uint8_t                  Status;
  uint8_t                  LE_States;
} HCI_LE_Read_Supported_States_Ret_t;

// 7.8.74 LE Read Transmit Power command

typedef struct HCI_LE_Read_Transmit_Power_Ret_t {
  uint8_t                  Status;
  uint8_t                  Min_TX_Power;
  uint8_t                  Max_TX_Power;
} HCI_LE_Read_Transmit_Power_Ret_t;

// 7.8.28 LE Receiver Test command

typedef struct HCI_LE_Receiver_Test_v3_Cmd_t {
  uint8_t                  RX_Channel;
  uint8_t                  PHY;
  uint8_t                  Modulation_Index;
  uint8_t                  Expected_CTE_Length;
  uint8_t                  Expected_CTE_Type;
  uint8_t                  Slot_Durations;
  uint8_t                  Switching_Pattern_Length;
  uint8_t  *               Antenna_IDs;
} HCI_LE_Receiver_Test_v3_Cmd_t;

// HCI_LE_Receiver_Test_v3_Ret_t only has one member: Status.

typedef struct HCI_LE_Receiver_Test_v2_Cmd_t {
  uint8_t                  RX_Channel;
  uint8_t                  PHY;
  uint8_t                  Modulation_Index;
} HCI_LE_Receiver_Test_v2_Cmd_t;

// HCI_LE_Receiver_Test_v2_Ret_t only has one member: Status.

typedef struct HCI_LE_Receiver_Test_v1_Cmd_t {
  uint8_t                  RX_Channel;
} HCI_LE_Receiver_Test_v1_Cmd_t;

// HCI_LE_Receiver_Test_v1_Ret_t only has one member: Status.

// 7.8.102 LE Reject CIS Request command

typedef struct HCI_LE_Reject_CIS_Request_Cmd_t {
  uint16_t                 Connection_Handle;
  uint8_t                  Reason;
} HCI_LE_Reject_CIS_Request_Cmd_t;

// HCI_LE_Reject_CIS_Request_Ret_t only has one member: Status_Conn.

// 7.7.65.6 LE Remote Connection Parameter Request event

typedef struct HCI_LE_Remote_Conn_Param_Request_Evt_t {
  uint8_t                  Subevent_Code;
  uint16_t                 Connection_Handle;
  uint16_t                 Interval_Min;
  uint16_t                 Interval_Max;
  uint16_t                 Max_Latency;
  uint16_t                 Timeout;
} HCI_LE_Remote_Conn_Param_Request_Evt_t;

// 7.8.32 LE Remote Connection Parameter Request Negative Reply command

typedef struct HCI_LE_Remote_Conn_Param_Req_Negative_Reply_Cmd_t {
  uint16_t                 Connection_Handle;
  uint8_t                  Reason;
} HCI_LE_Remote_Conn_Param_Req_Negative_Reply_Cmd_t;

// HCI_LE_Remote_Conn_Param_Req_Negative_Reply_Ret_t only has one member: Status_Conn.

// 7.8.31 LE Remote Connection Parameter Request Reply command

typedef struct HCI_LE_Remote_Conn_Param_Req_Reply_Cmd_t {
  uint16_t                 Connection_Handle;
  uint16_t                 Interval_Min;
  uint16_t                 Interval_Max;
  uint16_t                 Max_Latency;
  uint16_t                 Timeout;
  uint16_t                 Min_CE_Length;
  uint16_t                 Max_CE_Length;
} HCI_LE_Remote_Conn_Param_Req_Reply_Cmd_t;

// HCI_LE_Remote_Conn_Param_Req_Reply_Ret_t only has one member: Status_Conn.

// 7.8.59 LE Remove Advertising Set command

typedef struct HCI_LE_Remove_Adv_Set_Cmd_t {
  uint8_t                  Advertising_Handle;
} HCI_LE_Remove_Adv_Set_Cmd_t;

// HCI_LE_Remove_Adv_Set_Ret_t only has one member: Status.

// 7.8.100 LE Remove CIG command

typedef struct HCI_LE_Remove_CIG_Cmd_t {
  uint8_t                  CIG_ID;
} HCI_LE_Remove_CIG_Cmd_t;

typedef struct HCI_LE_Remove_CIG_Ret_t {
  uint8_t                  Status;
  uint8_t                  CIG_ID;
} HCI_LE_Remove_CIG_Ret_t;

// 7.8.17 LE Remove Device From Filter Accept List command

typedef struct HCI_LE_Remove_Device_From_Filter_Accept_List_Cmd_t {
  uint8_t                  Address_Type;
  BD_Addr_t                Address;
} HCI_LE_Remove_Device_From_Filter_Accept_List_Cmd_t;

// HCI_LE_Remove_Device_From_Filter_Accept_List_Ret_t only has one member: Status.

// 7.8.71 LE Remove Device From Periodic Advertiser List command

typedef struct HCI_LE_Remove_Device_From_Periodic_Advertiser_List_Cmd_t {
  uint8_t                  Advertiser_Address_Type;
  BD_Addr_t                Advertiser_Address;
  uint8_t                  Advertising_SID;
} HCI_LE_Remove_Device_From_Periodic_Advertiser_List_Cmd_t;

// HCI_LE_Remove_Device_From_Periodic_Advertiser_List_Ret_t only has one member: Status.

// 7.8.39 LE Remove Device From Resolving List command

typedef struct HCI_LE_Remove_Device_From_Resolving_List_Cmd_t {
  uint8_t                  Peer_Identity_Address_Type;
  BD_Addr_t                Peer_Identity_Address;
} HCI_LE_Remove_Device_From_Resolving_List_Cmd_t;

// HCI_LE_Remove_Device_From_Resolving_List_Ret_t only has one member: Status.

// 7.8.110 LE Remove ISO Data Path command

typedef struct HCI_LE_Remove_ISO_Data_Path_Cmd_t {
  uint16_t                 Connection_Handle;
  uint8_t                  Data_Path_Direction;
} HCI_LE_Remove_ISO_Data_Path_Cmd_t;

// HCI_LE_Remove_ISO_Data_Path_Ret_t only has one member: Status_Conn.

// 7.8.108 LE Request Peer SCA command

// HCI_LE_Req_Peer_SCA_Cmd_t only has one member: Connection_Handle.

// 7.7.65.31 LE Request Peer SCA Complete event

typedef struct HCI_LE_Req_Peer_SCA_Complete_Evt_t {
  uint8_t                  Subevent_Code;
  uint8_t                  Status;
  uint16_t                 Connection_Handle;
  uint8_t                  Peer_Clock_Accuracy;
} HCI_LE_Req_Peer_SCA_Complete_Evt_t;

// 7.7.65.19 LE Scan Request Received event

typedef struct HCI_LE_Scan_Req_Received_Evt_t {
  uint8_t                  Subevent_Code;
  uint8_t                  Advertising_Handle;
  uint8_t                  Scanner_Address_Type;
  BD_Addr_t                Scanner_Address;
} HCI_LE_Scan_Req_Received_Evt_t;

// 7.7.65.17 LE Scan Timeout event

typedef struct HCI_LE_Scan_Timeout_Evt_t {
  uint8_t                  Subevent_Code;
} HCI_LE_Scan_Timeout_Evt_t;

// 7.8.44 LE Set Address Resolution Enable command

typedef struct HCI_LE_Set_Address_Resolution_Enable_Cmd_t {
  uint8_t                  Address_Resolution_Enable;
} HCI_LE_Set_Address_Resolution_Enable_Cmd_t;

// HCI_LE_Set_Address_Resolution_Enable_Ret_t only has one member: Status.

// 7.8.7 LE Set Advertising Data command

typedef struct HCI_LE_Set_Adv_Data_Cmd_t {
  uint8_t                  Advertising_Data_Length;
  uint8_t                  Advertising_Data[31];
} HCI_LE_Set_Adv_Data_Cmd_t;

// HCI_LE_Set_Adv_Data_Ret_t only has one member: Status.

// 7.8.9 LE Set Advertising Enable command

typedef struct HCI_LE_Set_Adv_Enable_Cmd_t {
  uint8_t                  Advertising_Enable;
} HCI_LE_Set_Adv_Enable_Cmd_t;

// HCI_LE_Set_Adv_Enable_Ret_t only has one member: Status.

// 7.8.5 LE Set Advertising Parameters command

typedef struct HCI_LE_Set_Adv_Parameters_Cmd_t {
  uint16_t                 Advertising_Interval_Min;
  uint16_t                 Advertising_Interval_Max;
  uint8_t                  Advertising_Type;
  uint8_t                  Own_Address_Type;
  uint8_t                  Peer_Address_Type;
  BD_Addr_t                Peer_Address;
  uint8_t                  Advertising_Channel_Map;
  uint8_t                  Advertising_Filter_Policy;
} HCI_LE_Set_Adv_Parameters_Cmd_t;

// HCI_LE_Set_Adv_Parameters_Ret_t only has one member: Status.

// 7.8.52 LE Set Advertising Set Random Address command

typedef struct HCI_LE_Set_Adv_Set_Random_Address_Cmd_t {
  uint8_t                  Advertising_Handle;
  BD_Addr_t                Random_Address;
} HCI_LE_Set_Adv_Set_Random_Address_Cmd_t;

// HCI_LE_Set_Adv_Set_Random_Address_Ret_t only has one member: Status.

// 7.8.97 LE Set CIG Parameters command

typedef struct HCI_LE_Set_CIG_Parameters_Cmd_t {
  uint8_t                  CIG_ID;
  uint8_t                  SDU_Interval_C_To_P[3];
  uint8_t                  SDU_Interval_P_To_C[3];
  uint8_t                  Worst_Case_SCA;
  uint8_t                  Packing;
  uint8_t                  Framing;
  uint16_t                 Max_Transport_Latency_C_To_P;
  uint16_t                 Max_Transport_Latency_P_To_C;
  uint8_t                  CIS_Count;
  struct {
    uint8_t                CIS_ID;
    uint16_t               Max_SDU_C_To_P;
    uint16_t               Max_SDU_P_To_C;
    uint8_t                PHY_C_To_P;
    uint8_t                PHY_P_To_C;
    uint8_t                RTN_C_To_P;
    uint8_t                RTN_P_To_C;
  } CIS[0];
} HCI_LE_Set_CIG_Parameters_Cmd_t;

typedef struct HCI_LE_Set_CIG_Parameters_Ret_t {
  uint8_t                  Status;
  uint8_t                  CIG_ID;
  uint8_t                  CIS_Count;
  uint8_t  *               Connection_Handle;
} HCI_LE_Set_CIG_Parameters_Ret_t;

// 7.8.98 LE Set CIG Parameters Test command

typedef struct HCI_LE_Set_CIG_Parameters_Test_Cmd_t {
  uint8_t                  CIG_ID;
  uint8_t                  SDU_Interval_C_To_P[3];
  uint8_t                  SDU_Interval_P_To_C[3];
  uint8_t                  FT_C_To_P;
  uint8_t                  FT_P_To_C;
  uint16_t                 ISO_Interval;
  uint8_t                  Worst_Case_SCA;
  uint8_t                  Packing;
  uint8_t                  Framing;
  uint8_t                  CIS_Count;
  struct {
    uint8_t                CIS_ID;
    uint8_t                NSE;
    uint16_t               Max_SDU_C_To_P;
    uint16_t               Max_SDU_P_To_C;
    uint16_t               Max_PDU_C_To_P;
    uint16_t               Max_PDU_P_To_C;
    uint8_t                PHY_C_To_P;
    uint8_t                PHY_P_To_C;
    uint8_t                BN_C_To_P;
    uint8_t                BN_P_To_C;
  } CIS[0];
} HCI_LE_Set_CIG_Parameters_Test_Cmd_t;

typedef struct HCI_LE_Set_CIG_Parameters_Test_Ret_t {
  uint8_t                  Status;
  uint8_t                  CIG_ID;
  uint8_t                  CIS_Count;
  uint8_t  *               Connection_Handle;
} HCI_LE_Set_CIG_Parameters_Test_Ret_t;

// 7.8.83 LE Set Connection CTE Receive Parameters command

typedef struct HCI_LE_Set_Conn_CTE_Receive_Parameters_Cmd_t {
  uint16_t                 Connection_Handle;
  uint8_t                  Sampling_Enable;
  uint8_t                  Slot_Durations;
  uint8_t                  Switching_Pattern_Length;
  uint8_t  *               Antenna_IDs;
} HCI_LE_Set_Conn_CTE_Receive_Parameters_Cmd_t;

// HCI_LE_Set_Conn_CTE_Receive_Parameters_Ret_t only has one member: Status_Conn.

// 7.8.84 LE Set Connection CTE Transmit Parameters command

typedef struct HCI_LE_Set_Conn_CTE_Transmit_Parameters_Cmd_t {
  uint16_t                 Connection_Handle;
  uint8_t                  CTE_Types;
  uint8_t                  Switching_Pattern_Length;
  uint8_t  *               Antenna_IDs;
} HCI_LE_Set_Conn_CTE_Transmit_Parameters_Cmd_t;

// HCI_LE_Set_Conn_CTE_Transmit_Parameters_Ret_t only has one member: Status_Conn.

// 7.8.81 LE Set Connectionless CTE Transmit Enable command

typedef struct HCI_LE_Set_Connectionless_CTE_Transmit_Enable_Cmd_t {
  uint8_t                  Advertising_Handle;
  uint8_t                  CTE_Enable;
} HCI_LE_Set_Connectionless_CTE_Transmit_Enable_Cmd_t;

// HCI_LE_Set_Connectionless_CTE_Transmit_Enable_Ret_t only has one member: Status.

// 7.8.80 LE Set Connectionless CTE Transmit Parameters command

typedef struct HCI_LE_Set_Connectionless_CTE_Transmit_Parameters_Cmd_t {
  uint8_t                  Advertising_Handle;
  uint8_t                  CTE_Length;
  uint8_t                  CTE_Type;
  uint8_t                  CTE_Count;
  uint8_t                  Switching_Pattern_Length;
  uint8_t  *               Antenna_IDs;
} HCI_LE_Set_Connectionless_CTE_Transmit_Parameters_Cmd_t;

// HCI_LE_Set_Connectionless_CTE_Transmit_Parameters_Ret_t only has one member: Status.

// 7.8.82 LE Set Connectionless IQ Sampling Enable command

typedef struct HCI_LE_Set_Connectionless_IQ_Sampling_Enable_Cmd_t {
  uint16_t                 Sync_Handle;
  uint8_t                  Sampling_Enable;
  uint8_t                  Slot_Durations;
  uint8_t                  Max_Sampled_CTEs;
  uint8_t                  Switching_Pattern_Length;
  uint8_t  *               Antenna_IDs;
} HCI_LE_Set_Connectionless_IQ_Sampling_Enable_Cmd_t;

typedef struct HCI_LE_Set_Connectionless_IQ_Sampling_Enable_Ret_t {
  uint8_t                  Status;
  uint16_t                 Sync_Handle;
} HCI_LE_Set_Connectionless_IQ_Sampling_Enable_Ret_t;

// 7.8.33 LE Set Data Length command

typedef struct HCI_LE_Set_Data_Length_Cmd_t {
  uint16_t                 Connection_Handle;
  uint16_t                 TX_Octets;
  uint16_t                 TX_Time;
} HCI_LE_Set_Data_Length_Cmd_t;

// HCI_LE_Set_Data_Length_Ret_t only has one member: Status_Conn.

// 7.8.122 LE Set Data Related Address Changes command

typedef struct HCI_LE_Set_Data_Related_Address_Changes_Cmd_t {
  uint8_t                  Advertising_Handle;
  uint8_t                  Change_Reasons;
} HCI_LE_Set_Data_Related_Address_Changes_Cmd_t;

// HCI_LE_Set_Data_Related_Address_Changes_Ret_t only has one member: Status.

// 7.8.92 LE Set Default Periodic Advertising Sync Transfer Parameters command

typedef struct HCI_LE_Set_Default_PAST_Parameters_Cmd_t {
  uint8_t                  Mode;
  uint16_t                 Skip;
  uint16_t                 Sync_Timeout;
  uint8_t                  CTE_Type;
} HCI_LE_Set_Default_PAST_Parameters_Cmd_t;

// HCI_LE_Set_Default_PAST_Parameters_Ret_t only has one member: Status.

// 7.8.48 LE Set Default PHY command

typedef struct HCI_LE_Set_Default_PHY_Cmd_t {
  uint8_t                  All_PHYs;
  uint8_t                  TX_PHYs;
  uint8_t                  RX_PHYs;
} HCI_LE_Set_Default_PHY_Cmd_t;

// HCI_LE_Set_Default_PHY_Ret_t only has one member: Status.

// 7.8.123 LE Set Default Subrate command

typedef struct HCI_LE_Set_Default_Subrate_Cmd_t {
  uint16_t                 Subrate_Min;
  uint16_t                 Subrate_Max;
  uint16_t                 Max_Latency;
  uint16_t                 Continuation_Number;
  uint16_t                 Supervision_Timeout;
} HCI_LE_Set_Default_Subrate_Cmd_t;

// HCI_LE_Set_Default_Subrate_Ret_t only has one member: Status.

// 7.8.1 LE Set Event Mask command

typedef struct HCI_LE_Set_Event_Mask_Cmd_t {
  uint8_t                  LE_Event_Mask[8];
} HCI_LE_Set_Event_Mask_Cmd_t;

// HCI_LE_Set_Event_Mask_Ret_t only has one member: Status.

// 7.8.54 LE Set Extended Advertising Data command

typedef struct HCI_LE_Set_Extended_Adv_Data_Cmd_t {
  uint8_t                  Advertising_Handle;
  uint8_t                  Operation;
  uint8_t                  Fragment_Preference;
  uint8_t                  Advertising_Data_Length;
  uint8_t  *               Advertising_Data;
} HCI_LE_Set_Extended_Adv_Data_Cmd_t;

// HCI_LE_Set_Extended_Adv_Data_Ret_t only has one member: Status.

// 7.8.56 LE Set Extended Advertising Enable command

typedef struct HCI_LE_Set_Extended_Adv_Enable_Cmd_t {
  uint8_t                  Enable;
  uint8_t                  Num_Sets;
  struct {
    uint8_t                Advertising_Handle;
    uint16_t               Duration;
    uint8_t                Max_Extended_Adv_Events;
  } Sets[0];
} HCI_LE_Set_Extended_Adv_Enable_Cmd_t;

// HCI_LE_Set_Extended_Adv_Enable_Ret_t only has one member: Status.

// 7.8.53 LE Set Extended Advertising Parameters command

typedef struct HCI_LE_Set_Extended_Adv_Parameters_v2_Cmd_t {
  uint8_t                  Advertising_Handle;
  uint16_t                 Advertising_Event_Properties;
  uint8_t                  Primary_Adv_Interval_Min[3];
  uint8_t                  Primary_Adv_Interval_Max[3];
  uint8_t                  Primary_Adv_Channel_Map;
  uint8_t                  Own_Address_Type;
  uint8_t                  Peer_Address_Type;
  BD_Addr_t                Peer_Address;
  uint8_t                  Advertising_Filter_Policy;
  uint8_t                  Advertising_TX_Power;
  uint8_t                  Primary_Adv_PHY;
  uint8_t                  Secondary_Adv_Max_Skip;
  uint8_t                  Secondary_Adv_PHY;
  uint8_t                  Advertising_SID;
  uint8_t                  Scan_Req_Notification_Enable;
  uint8_t                  Primary_Adv_PHY_Options;
  uint8_t                  Secondary_Adv_PHY_Options;
} HCI_LE_Set_Extended_Adv_Parameters_v2_Cmd_t;

typedef struct HCI_LE_Set_Extended_Adv_Parameters_v2_Ret_t {
  uint8_t                  Status;
  uint8_t                  Selected_TX_Power;
} HCI_LE_Set_Extended_Adv_Parameters_v2_Ret_t;

typedef struct HCI_LE_Set_Extended_Adv_Parameters_v1_Cmd_t {
  uint8_t                  Advertising_Handle;
  uint16_t                 Advertising_Event_Properties;
  uint8_t                  Primary_Adv_Interval_Min[3];
  uint8_t                  Primary_Adv_Interval_Max[3];
  uint8_t                  Primary_Adv_Channel_Map;
  uint8_t                  Own_Address_Type;
  uint8_t                  Peer_Address_Type;
  BD_Addr_t                Peer_Address;
  uint8_t                  Advertising_Filter_Policy;
  uint8_t                  Advertising_TX_Power;
  uint8_t                  Primary_Adv_PHY;
  uint8_t                  Secondary_Adv_Max_Skip;
  uint8_t                  Secondary_Adv_PHY;
  uint8_t                  Advertising_SID;
  uint8_t                  Scan_Req_Notification_Enable;
} HCI_LE_Set_Extended_Adv_Parameters_v1_Cmd_t;

typedef struct HCI_LE_Set_Extended_Adv_Parameters_v1_Ret_t {
  uint8_t                  Status;
  uint8_t                  Selected_TX_Power;
} HCI_LE_Set_Extended_Adv_Parameters_v1_Ret_t;

// 7.8.65 LE Set Extended Scan Enable command

typedef struct HCI_LE_Set_Extended_Scan_Enable_Cmd_t {
  uint8_t                  Enable;
  uint8_t                  Filter_Duplicates;
  uint16_t                 Duration;
  uint16_t                 Period;
} HCI_LE_Set_Extended_Scan_Enable_Cmd_t;

// HCI_LE_Set_Extended_Scan_Enable_Ret_t only has one member: Status.

// 7.8.64 LE Set Extended Scan Parameters command

typedef struct HCI_LE_Set_Extended_Scan_Parameters_Cmd_t {
  uint8_t                  Own_Address_Type;
  uint8_t                  Scanning_Filter_Policy;
  uint8_t                  Scanning_PHYs;
  struct {
    uint8_t                Scan_Type;
    uint8_t                Scan_Interval;
    uint8_t                Scan_Window;
  } Scanning_PHYs_data[0];
} HCI_LE_Set_Extended_Scan_Parameters_Cmd_t;

// HCI_LE_Set_Extended_Scan_Parameters_Ret_t only has one member: Status.

// 7.8.55 LE Set Extended Scan Response Data command

typedef struct HCI_LE_Set_Extended_Scan_Response_Data_Cmd_t {
  uint8_t                  Advertising_Handle;
  uint8_t                  Operation;
  uint8_t                  Fragment_Preference;
  uint8_t                  Scan_Response_Data_Length;
  uint8_t  *               Scan_Response_Data;
} HCI_LE_Set_Extended_Scan_Response_Data_Cmd_t;

// HCI_LE_Set_Extended_Scan_Response_Data_Ret_t only has one member: Status.

// 7.8.19 LE Set Host Channel Classification command

typedef struct HCI_LE_Set_Host_Channel_Classification_Cmd_t {
  uint8_t                  Channel_Map[5];
} HCI_LE_Set_Host_Channel_Classification_Cmd_t;

// HCI_LE_Set_Host_Channel_Classification_Ret_t only has one member: Status.

// 7.8.115 LE Set Host Feature command

typedef struct HCI_LE_Set_Host_Feature_Cmd_t {
  uint8_t                  Bit_Number;
  uint8_t                  Bit_Value;
} HCI_LE_Set_Host_Feature_Cmd_t;

// HCI_LE_Set_Host_Feature_Ret_t only has one member: Status.

// 7.8.120 LE Set Path Loss Reporting Enable command

typedef struct HCI_LE_Set_Path_Loss_Reporting_Enable_Cmd_t {
  uint16_t                 Connection_Handle;
  uint8_t                  Enable;
} HCI_LE_Set_Path_Loss_Reporting_Enable_Cmd_t;

// HCI_LE_Set_Path_Loss_Reporting_Enable_Ret_t only has one member: Status_Conn.

// 7.8.119 LE Set Path Loss Reporting Parameters command

typedef struct HCI_LE_Set_Path_Loss_Reporting_Parameters_Cmd_t {
  uint16_t                 Connection_Handle;
  uint8_t                  High_Threshold;
  uint8_t                  High_Hysteresis;
  uint8_t                  Low_Threshold;
  uint8_t                  Low_Hysteresis;
  uint16_t                 Min_Time_Spent;
} HCI_LE_Set_Path_Loss_Reporting_Parameters_Cmd_t;

// HCI_LE_Set_Path_Loss_Reporting_Parameters_Ret_t only has one member: Status_Conn.

// 7.8.62 LE Set Periodic Advertising Data command

typedef struct HCI_LE_Set_Periodic_Adv_Data_Cmd_t {
  uint8_t                  Advertising_Handle;
  uint8_t                  Operation;
  uint8_t                  Advertising_Data_Length;
  uint8_t  *               Advertising_Data;
} HCI_LE_Set_Periodic_Adv_Data_Cmd_t;

// HCI_LE_Set_Periodic_Adv_Data_Ret_t only has one member: Status.

// 7.8.63 LE Set Periodic Advertising Enable command

typedef struct HCI_LE_Set_Periodic_Adv_Enable_Cmd_t {
  uint8_t                  Enable;
  uint8_t                  Advertising_Handle;
} HCI_LE_Set_Periodic_Adv_Enable_Cmd_t;

// HCI_LE_Set_Periodic_Adv_Enable_Ret_t only has one member: Status.

// 7.8.61 LE Set Periodic Advertising Parameters command

typedef struct HCI_LE_Set_Periodic_Adv_Parameters_v2_Cmd_t {
  uint8_t                  Advertising_Handle;
  uint16_t                 Periodic_Adv_Interval_Min;
  uint16_t                 Periodic_Adv_Interval_Max;
  uint16_t                 Periodic_Adv_Properties;
  uint8_t                  Num_Subevents;
  uint8_t                  Subevent_Interval;
  uint8_t                  Response_Slot_Delay;
  uint8_t                  Response_Slot_Spacing;
  uint8_t                  Num_Response_Slots;
} HCI_LE_Set_Periodic_Adv_Parameters_v2_Cmd_t;

typedef struct HCI_LE_Set_Periodic_Adv_Parameters_v2_Ret_t {
  uint8_t                  Status;
  uint8_t                  Advertising_Handle;
} HCI_LE_Set_Periodic_Adv_Parameters_v2_Ret_t;

typedef struct HCI_LE_Set_Periodic_Adv_Parameters_v1_Cmd_t {
  uint8_t                  Advertising_Handle;
  uint16_t                 Periodic_Adv_Interval_Min;
  uint16_t                 Periodic_Adv_Interval_Max;
  uint16_t                 Periodic_Adv_Properties;
} HCI_LE_Set_Periodic_Adv_Parameters_v1_Cmd_t;

// HCI_LE_Set_Periodic_Adv_Parameters_v1_Ret_t only has one member: Status.

// 7.8.88 LE Set Periodic Advertising Receive Enable command

typedef struct HCI_LE_Set_Periodic_Adv_Receive_Enable_Cmd_t {
  uint16_t                 Sync_Handle;
  uint8_t                  Enable;
} HCI_LE_Set_Periodic_Adv_Receive_Enable_Cmd_t;

// HCI_LE_Set_Periodic_Adv_Receive_Enable_Ret_t only has one member: Status.

// 7.8.126 LE Set Periodic Advertising Response Data command

typedef struct HCI_LE_Set_Periodic_Adv_Response_Data_Cmd_t {
  uint16_t                 Sync_Handle;
  uint16_t                 Request_Event;
  uint8_t                  Request_Subevent;
  uint8_t                  Response_Subevent;
  uint8_t                  Response_Slot;
  uint8_t                  Response_Data_Length;
  uint8_t  *               Response_Data;
} HCI_LE_Set_Periodic_Adv_Response_Data_Cmd_t;

typedef struct HCI_LE_Set_Periodic_Adv_Response_Data_Ret_t {
  uint8_t                  Status;
  uint16_t                 Sync_Handle;
} HCI_LE_Set_Periodic_Adv_Response_Data_Ret_t;

// 7.8.125 LE Set Periodic Advertising Subevent Data command

typedef struct HCI_LE_Set_Periodic_Adv_Subevent_Data_Cmd_t {
  uint8_t                  Advertising_Handle;
  uint8_t                  Num_Subevents;
  struct {
    uint8_t                Subevent;
    uint8_t                Response_Slot_Start;
    uint8_t                Response_Slot_Count;
    uint8_t                Subevent_Data_Length;
    uint8_t  *             Subevent_Data;
  } Subevents[0];
} HCI_LE_Set_Periodic_Adv_Subevent_Data_Cmd_t;

typedef struct HCI_LE_Set_Periodic_Adv_Subevent_Data_Ret_t {
  uint8_t                  Status;
  uint8_t                  Advertising_Handle;
} HCI_LE_Set_Periodic_Adv_Subevent_Data_Ret_t;

// 7.8.127 LE Set Periodic Sync Subevent command

typedef struct HCI_LE_Set_Periodic_Sync_Subevent_Cmd_t {
  uint16_t                 Sync_Handle;
  uint16_t                 Periodic_Adv_Properties;
  uint8_t                  Num_Subevents;
  uint8_t  *               Subevent;
} HCI_LE_Set_Periodic_Sync_Subevent_Cmd_t;

typedef struct HCI_LE_Set_Periodic_Sync_Subevent_Ret_t {
  uint8_t                  Status;
  uint16_t                 Sync_Handle;
} HCI_LE_Set_Periodic_Sync_Subevent_Ret_t;

// 7.8.91 LE Set Periodic Advertising Sync Transfer Parameters command

typedef struct HCI_LE_Set_PAST_Parameters_Cmd_t {
  uint16_t                 Connection_Handle;
  uint8_t                  Mode;
  uint16_t                 Skip;
  uint16_t                 Sync_Timeout;
  uint8_t                  CTE_Type;
} HCI_LE_Set_PAST_Parameters_Cmd_t;

// HCI_LE_Set_PAST_Parameters_Ret_t only has one member: Status_Conn.

// 7.8.49 LE Set PHY command

typedef struct HCI_LE_Set_PHY_Cmd_t {
  uint16_t                 Connection_Handle;
  uint8_t                  All_PHYs;
  uint8_t                  TX_PHYs;
  uint8_t                  RX_PHYs;
  uint16_t                 PHY_Options;
} HCI_LE_Set_PHY_Cmd_t;

// 7.8.77 LE Set Privacy Mode command

typedef struct HCI_LE_Set_Privacy_Mode_Cmd_t {
  uint8_t                  Peer_Identity_Address_Type;
  BD_Addr_t                Peer_Identity_Address;
  uint8_t                  Privacy_Mode;
} HCI_LE_Set_Privacy_Mode_Cmd_t;

// HCI_LE_Set_Privacy_Mode_Ret_t only has one member: Status.

// 7.8.4 LE Set Random Address command

typedef struct HCI_LE_Set_Random_Address_Cmd_t {
  BD_Addr_t                Random_Address;
} HCI_LE_Set_Random_Address_Cmd_t;

// HCI_LE_Set_Random_Address_Ret_t only has one member: Status.

// 7.8.45 LE Set Resolvable Private Address Timeout command

typedef struct HCI_LE_Set_Resolvable_Private_Address_Timeout_Cmd_t {
  uint16_t                 RPA_Timeout;
} HCI_LE_Set_Resolvable_Private_Address_Timeout_Cmd_t;

// HCI_LE_Set_Resolvable_Private_Address_Timeout_Ret_t only has one member: Status.

// 7.8.11 LE Set Scan Enable command

typedef struct HCI_LE_Set_Scan_Enable_Cmd_t {
  uint8_t                  LE_Scan_Enable;
  uint8_t                  Filter_Duplicates;
} HCI_LE_Set_Scan_Enable_Cmd_t;

// HCI_LE_Set_Scan_Enable_Ret_t only has one member: Status.

// 7.8.10 LE Set Scan Parameters command

typedef struct HCI_LE_Set_Scan_Parameters_Cmd_t {
  uint8_t                  LE_Scan_Type;
  uint16_t                 LE_Scan_Interval;
  uint16_t                 LE_Scan_Window;
  uint8_t                  Own_Address_Type;
  uint8_t                  Scanning_Filter_Policy;
} HCI_LE_Set_Scan_Parameters_Cmd_t;

// HCI_LE_Set_Scan_Parameters_Ret_t only has one member: Status.

// 7.8.8 LE Set Scan Response Data command

typedef struct HCI_LE_Set_Scan_Response_Data_Cmd_t {
  uint8_t                  Scan_Response_Data_Length;
  uint8_t                  Scan_Response_Data[31];
} HCI_LE_Set_Scan_Response_Data_Cmd_t;

// HCI_LE_Set_Scan_Response_Data_Ret_t only has one member: Status.

// 7.8.121 LE Set Transmit Power Reporting Enable command

typedef struct HCI_LE_Set_Transmit_Power_Reporting_Enable_Cmd_t {
  uint16_t                 Connection_Handle;
  uint8_t                  Local_Enable;
  uint8_t                  Remote_Enable;
} HCI_LE_Set_Transmit_Power_Reporting_Enable_Cmd_t;

// HCI_LE_Set_Transmit_Power_Reporting_Enable_Ret_t only has one member: Status_Conn.

// 7.8.109 LE Setup ISO Data Path command

typedef struct HCI_LE_Setup_ISO_Data_Path_Cmd_t {
  uint16_t                 Connection_Handle;
  uint8_t                  Data_Path_Direction;
  uint8_t                  Data_Path_ID;
  uint8_t                  Codec_ID[5];
  uint8_t                  Controller_Delay[3];
  uint8_t                  Codec_Configuration_Length;
  uint8_t  *               Codec_Configuration;
} HCI_LE_Setup_ISO_Data_Path_Cmd_t;

// HCI_LE_Setup_ISO_Data_Path_Ret_t only has one member: Status_Conn.

// 7.7.65.35 LE Subrate Change event

typedef struct HCI_LE_Subrate_Change_Evt_t {
  uint8_t                  Subevent_Code;
  uint8_t                  Status;
  uint16_t                 Connection_Handle;
  uint16_t                 Subrate_Factor;
  uint16_t                 Peripheral_Latency;
  uint16_t                 Continuation_Number;
  uint16_t                 Supervision_Timeout;
} HCI_LE_Subrate_Change_Evt_t;

// 7.8.124 LE Subrate Request command

typedef struct HCI_LE_Subrate_Request_Cmd_t {
  uint16_t                 Connection_Handle;
  uint16_t                 Subrate_Min;
  uint16_t                 Subrate_Max;
  uint16_t                 Max_Latency;
  uint16_t                 Continuation_Number;
  uint16_t                 Supervision_Timeout;
} HCI_LE_Subrate_Request_Cmd_t;

// 7.8.105 LE Terminate BIG command

typedef struct HCI_LE_Terminate_BIG_Cmd_t {
  uint8_t                  BIG_Handle;
  uint8_t                  Reason;
} HCI_LE_Terminate_BIG_Cmd_t;

// 7.7.65.28 LE Terminate BIG Complete event

typedef struct HCI_LE_Terminate_BIG_Complete_Evt_t {
  uint8_t                  Subevent_Code;
  uint8_t                  BIG_Handle;
  uint8_t                  Reason;
} HCI_LE_Terminate_BIG_Complete_Evt_t;

// 7.8.30 LE Test End command

typedef struct HCI_LE_Test_End_Ret_t {
  uint8_t                  Status;
  uint16_t                 Num_Packets;
} HCI_LE_Test_End_Ret_t;

// 7.7.65.33 LE Transmit Power Reporting event

typedef struct HCI_LE_Transmit_Power_Reporting_Evt_t {
  uint8_t                  Subevent_Code;
  uint8_t                  Status;
  uint16_t                 Connection_Handle;
  uint8_t                  Reason;
  uint8_t                  PHY;
  uint8_t                  TX_Power_Level;
  uint8_t                  TX_Power_Level_Flag;
  uint8_t                  Delta;
} HCI_LE_Transmit_Power_Reporting_Evt_t;

// 7.8.29 LE Transmitter Test command

typedef struct HCI_LE_Transmitter_Test_v4_Cmd_t {
  uint8_t                  TX_Channel;
  uint8_t                  Test_Data_Length;
  uint8_t                  Packet_Payload;
  uint8_t                  PHY;
  uint8_t                  CTE_Length;
  uint8_t                  CTE_Type;
  uint8_t                  Switching_Pattern_Length;
  uint8_t  *               Antenna_IDs;
  uint8_t                  TX_Power_Level;
} HCI_LE_Transmitter_Test_v4_Cmd_t;

// HCI_LE_Transmitter_Test_v4_Ret_t only has one member: Status.

typedef struct HCI_LE_Transmitter_Test_v3_Cmd_t {
  uint8_t                  TX_Channel;
  uint8_t                  Test_Data_Length;
  uint8_t                  Packet_Payload;
  uint8_t                  PHY;
  uint8_t                  CTE_Length;
  uint8_t                  CTE_Type;
  uint8_t                  Switching_Pattern_Length;
  uint8_t  *               Antenna_IDs;
} HCI_LE_Transmitter_Test_v3_Cmd_t;

// HCI_LE_Transmitter_Test_v3_Ret_t only has one member: Status.

typedef struct HCI_LE_Transmitter_Test_v2_Cmd_t {
  uint8_t                  TX_Channel;
  uint8_t                  Test_Data_Length;
  uint8_t                  Packet_Payload;
  uint8_t                  PHY;
} HCI_LE_Transmitter_Test_v2_Cmd_t;

// HCI_LE_Transmitter_Test_v2_Ret_t only has one member: Status.

typedef struct HCI_LE_Transmitter_Test_v1_Cmd_t {
  uint8_t                  TX_Channel;
  uint8_t                  Test_Data_Length;
  uint8_t                  Packet_Payload;
} HCI_LE_Transmitter_Test_v1_Cmd_t;

// HCI_LE_Transmitter_Test_v1_Ret_t only has one member: Status.

// 7.8.76 LE Write RF Path Compensation command

typedef struct HCI_LE_Write_RF_Path_Compensation_Cmd_t {
  uint16_t                 RF_TX_Path_Compensation_Value;
  uint16_t                 RF_RX_Path_Compensation_Value;
} HCI_LE_Write_RF_Path_Compensation_Cmd_t;

// HCI_LE_Write_RF_Path_Compensation_Ret_t only has one member: Status.

// 7.8.35 LE Write Suggested Default Data Length command

typedef struct HCI_LE_Write_Suggested_Default_Data_Length_Cmd_t {
  uint16_t                 Suggested_Max_TX_Octets;
  uint16_t                 Suggested_Max_TX_Time;
} HCI_LE_Write_Suggested_Default_Data_Length_Cmd_t;

// HCI_LE_Write_Suggested_Default_Data_Length_Ret_t only has one member: Status.

// 7.7.24 Link Key Notification event

typedef struct HCI_Link_Key_Notification_Evt_t {
  BD_Addr_t                BD_Addr;
  uint8_t                  Link_Key[16];
  uint8_t                  Key_Type;
} HCI_Link_Key_Notification_Evt_t;

// 7.7.23 Link Key Request event

typedef struct HCI_Link_Key_Request_Evt_t {
  BD_Addr_t                BD_Addr;
} HCI_Link_Key_Request_Evt_t;

// 7.1.11 Link Key Request Negative Reply command

typedef struct HCI_Link_Key_Req_Negative_Reply_Cmd_t {
  BD_Addr_t                BD_Addr;
} HCI_Link_Key_Req_Negative_Reply_Cmd_t;

// HCI_Link_Key_Req_Negative_Reply_Ret_t only has one member: Status_BD_Addr.

// 7.1.10 Link Key Request Reply command

typedef struct HCI_Link_Key_Req_Reply_Cmd_t {
  BD_Addr_t                BD_Addr;
  uint8_t                  Link_Key[16];
} HCI_Link_Key_Req_Reply_Cmd_t;

// HCI_Link_Key_Req_Reply_Ret_t only has one member: Status_BD_Addr.

// 7.1.18 Link Key Selection command

typedef struct HCI_Link_Key_Selection_Cmd_t {
  uint8_t                  Key_Flag;
} HCI_Link_Key_Selection_Cmd_t;

// 7.7.10 Link Key Type Changed event

typedef struct HCI_Link_Key_Type_Changed_Evt_t {
  Status_Conn_Handle_t     Status_Conn;
  uint8_t                  Key_Flag;
} HCI_Link_Key_Type_Changed_Evt_t;

// 7.7.46 Link Supervision Timeout Changed event

typedef struct HCI_Link_Supervision_Timeout_Changed_Evt_t {
  uint16_t                 Connection_Handle;
  uint16_t                 Link_Supervision_Timeout;
} HCI_Link_Supervision_Timeout_Changed_Evt_t;

// 7.7.25 Loopback Command event

typedef struct HCI_Loopback_Command_Evt_t {
  uint8_t                  HCI_Command_Packet;
} HCI_Loopback_Command_Evt_t;

// 7.7.27 Max Slots Change event

typedef struct HCI_Max_Slots_Change_Evt_t {
  uint16_t                 Connection_Handle;
  uint8_t                  LMP_Max_Slots;
} HCI_Max_Slots_Change_Evt_t;

// 7.7.20 Mode Change event

typedef struct HCI_Mode_Change_Evt_t {
  Status_Conn_Handle_t     Status_Conn;
  uint8_t                  Current_Mode;
  uint16_t                 Interval;
} HCI_Mode_Change_Evt_t;

// 7.7.59 Number Of Completed Data Blocks event

typedef struct HCI_Number_Of_Completed_Data_Blocks_Evt_t {
  uint16_t                 Total_Num_Data_Blocks;
  uint8_t                  Num_Handles;
  struct {
    uint16_t               Connection_Handle;
    uint16_t               Num_Completed_Packets;
    uint16_t               Num_Completed_Blocks;
  } Handles[0];
} HCI_Number_Of_Completed_Data_Blocks_Evt_t;

// 7.7.19 Number Of Completed Packets event

typedef struct HCI_Number_Of_Completed_Packets_Evt_t {
  uint8_t                  Num_Handles;
  struct {
    uint16_t               Connection_Handle;
    uint16_t               Num_Completed_Packets;
  } Handles[0];
} HCI_Number_Of_Completed_Packets_Evt_t;

// 7.7.31 Page Scan Repetition Mode Change event

typedef struct HCI_Page_Scan_Repetition_Mode_Change_Evt_t {
  BD_Addr_t                BD_Addr;
  uint8_t                  Page_Scan_Repetition_Mode;
} HCI_Page_Scan_Repetition_Mode_Change_Evt_t;

// 7.1.3 Periodic Inquiry Mode command

typedef struct HCI_Periodic_Inquiry_Mode_Cmd_t {
  uint16_t                 Max_Period_Length;
  uint16_t                 Min_Period_Length;
  uint8_t                  LAP[3];
  uint8_t                  Inquiry_Length;
  uint8_t                  Num_Responses;
} HCI_Periodic_Inquiry_Mode_Cmd_t;

// HCI_Periodic_Inquiry_Mode_Ret_t only has one member: Status.

// 7.7.22 PIN Code Request event

typedef struct HCI_PIN_Code_Request_Evt_t {
  BD_Addr_t                BD_Addr;
} HCI_PIN_Code_Request_Evt_t;

// 7.1.13 PIN Code Request Negative Reply command

typedef struct HCI_PIN_Code_Req_Negative_Reply_Cmd_t {
  BD_Addr_t                BD_Addr;
} HCI_PIN_Code_Req_Negative_Reply_Cmd_t;

// HCI_PIN_Code_Req_Negative_Reply_Ret_t only has one member: Status_BD_Addr.

// 7.1.12 PIN Code Request Reply command

typedef struct HCI_PIN_Code_Req_Reply_Cmd_t {
  BD_Addr_t                BD_Addr;
  uint8_t                  PIN_Code_Length;
  uint8_t                  PIN_Code[16];
} HCI_PIN_Code_Req_Reply_Cmd_t;

// HCI_PIN_Code_Req_Reply_Ret_t only has one member: Status_BD_Addr.

// 7.2.6 QoS Setup command

typedef struct HCI_QoS_Setup_Cmd_t {
  uint16_t                 Connection_Handle;
  uint8_t                  Unused;
  uint8_t                  Service_Type;
  uint32_t                 Token_Rate;
  uint32_t                 Peak_Bandwidth;
  uint32_t                 Latency;
  uint32_t                 Delay_Variation;
} HCI_QoS_Setup_Cmd_t;

// 7.7.13 QoS Setup Complete event

typedef struct HCI_QoS_Setup_Complete_Evt_t {
  Status_Conn_Handle_t     Status_Conn;
  uint8_t                  Unused;
  uint8_t                  Service_Type;
  uint32_t                 Token_Rate;
  uint32_t                 Peak_Bandwidth;
  uint32_t                 Latency;
  uint32_t                 Delay_Variation;
} HCI_QoS_Setup_Complete_Evt_t;

// 7.7.30 QoS Violation event

typedef struct HCI_QoS_Violation_Evt_t {
  uint16_t                 Handle;
} HCI_QoS_Violation_Evt_t;

// 7.3.53 Read AFH Channel Assessment Mode command

typedef struct HCI_Read_AFH_Channel_Assessment_Mode_Ret_t {
  uint8_t                  Status;
  uint8_t                  AFH_Channel_Assessment_Mode;
} HCI_Read_AFH_Channel_Assessment_Mode_Ret_t;

// 7.5.5 Read AFH Channel Map command

// HCI_Read_AFH_Channel_Map_Cmd_t only has one member: Connection_Handle.

typedef struct HCI_Read_AFH_Channel_Map_Ret_t {
  Status_Conn_Handle_t     Status_Conn;
  uint8_t                  AFH_Mode;
  uint8_t                  AFH_Channel_Map[10];
} HCI_Read_AFH_Channel_Map_Ret_t;

// 7.3.93 Read Authenticated Payload Timeout command

// HCI_Read_Authenticated_Payload_Timeout_Cmd_t only has one member: Connection_Handle.

typedef struct HCI_Read_Authenticated_Payload_Timeout_Ret_t {
  Status_Conn_Handle_t     Status_Conn;
  uint16_t                 Authenticated_Payload_Timeout;
} HCI_Read_Authenticated_Payload_Timeout_Ret_t;

// 7.3.23 Read Authentication Enable command

typedef struct HCI_Read_Authentication_Enable_Ret_t {
  uint8_t                  Status;
  uint8_t                  Authentication_Enable;
} HCI_Read_Authentication_Enable_Ret_t;

// 7.3.29 Read Automatic Flush Timeout command

// HCI_Read_Automatic_Flush_Timeout_Cmd_t only has one member: Connection_Handle.

typedef struct HCI_Read_Automatic_Flush_Timeout_Ret_t {
  Status_Conn_Handle_t     Status_Conn;
  uint16_t                 Flush_Timeout;
} HCI_Read_Automatic_Flush_Timeout_Ret_t;

// 7.4.6 Read BD_ADDR command

// HCI_Read_BD_ADDR_Ret_t only has one member: Status_BD_Addr.

// 7.4.5 Read Buffer Size command

typedef struct HCI_Read_Buffer_Size_Ret_t {
  uint8_t                  Status;
  uint16_t                 ACL_Data_Packet_Length;
  uint8_t                  Synchronous_Data_Packet_Length;
  uint16_t                 Total_Num_ACL_Data_Packets;
  uint16_t                 Total_Num_Sync_Data_Packets;
} HCI_Read_Buffer_Size_Ret_t;

// 7.3.25 Read Class of Device command

typedef struct HCI_Read_Class_Of_Device_Ret_t {
  uint8_t                  Status;
  uint8_t                  Class_Of_Device[3];
} HCI_Read_Class_Of_Device_Ret_t;

// 7.5.6 Read Clock command

typedef struct HCI_Read_Clock_Cmd_t {
  uint16_t                 Connection_Handle;
  uint8_t                  Which_Clock;
} HCI_Read_Clock_Cmd_t;

typedef struct HCI_Read_Clock_Ret_t {
  Status_Conn_Handle_t     Status_Conn;
  uint32_t                 Clock;
  uint16_t                 Accuracy;
} HCI_Read_Clock_Ret_t;

// 7.1.24 Read Clock Offset command

// HCI_Read_Clock_Offset_Cmd_t only has one member: Connection_Handle.

// 7.7.28 Read Clock Offset Complete event

typedef struct HCI_Read_Clock_Offset_Complete_Evt_t {
  Status_Conn_Handle_t     Status_Conn;
  uint16_t                 Clock_Offset;
} HCI_Read_Clock_Offset_Complete_Evt_t;

// 7.3.13 Read Connection Accept Timeout command

typedef struct HCI_Read_Conn_Accept_Timeout_Ret_t {
  uint8_t                  Status;
  uint16_t                 Connection_Accept_Timeout;
} HCI_Read_Conn_Accept_Timeout_Ret_t;

// 7.3.44 Read Current IAC LAP command

typedef struct HCI_Read_Current_IAC_LAP_Ret_t {
  uint8_t                  Status;
  uint8_t                  Num_Current_IAC;
  uint8_t  *               IAC_LAP;
} HCI_Read_Current_IAC_LAP_Ret_t;

// 7.4.7 Read Data Block Size command

typedef struct HCI_Read_Data_Block_Size_Ret_t {
  uint8_t                  Status;
  uint16_t                 Max_ACL_Data_Packet_Length;
  uint16_t                 Data_Block_Length;
  uint16_t                 Total_Num_Data_Blocks;
} HCI_Read_Data_Block_Size_Ret_t;

// 7.3.64 Read Default Erroneous Data Reporting command

typedef struct HCI_Read_Default_Erroneous_Data_Reporting_Ret_t {
  uint8_t                  Status;
  uint8_t                  Erroneous_Data_Reporting;
} HCI_Read_Default_Erroneous_Data_Reporting_Ret_t;

// 7.2.11 Read Default Link Policy Settings command

typedef struct HCI_Read_Default_Link_Policy_Settings_Ret_t {
  uint8_t                  Status;
  uint16_t                 Default_Link_Policy_Settings;
} HCI_Read_Default_Link_Policy_Settings_Ret_t;

// 7.5.7 Read Encryption Key Size command

// HCI_Read_Encryption_Key_Size_Cmd_t only has one member: Connection_Handle.

typedef struct HCI_Read_Encryption_Key_Size_Ret_t {
  Status_Conn_Handle_t     Status_Conn;
  uint8_t                  Key_Size;
} HCI_Read_Encryption_Key_Size_Ret_t;

// 7.3.74 Read Enhanced Transmit Power Level command

typedef struct HCI_Read_Enhanced_Transmit_Power_Level_Cmd_t {
  uint16_t                 Connection_Handle;
  uint8_t                  Type;
} HCI_Read_Enhanced_Transmit_Power_Level_Cmd_t;

typedef struct HCI_Read_Enhanced_Transmit_Power_Level_Ret_t {
  Status_Conn_Handle_t     Status_Conn;
  uint8_t                  TX_Power_Level_GFSK;
  uint8_t                  TX_Power_Level_DQPSK;
  uint8_t                  TX_Power_Level_8DPSK;
} HCI_Read_Enhanced_Transmit_Power_Level_Ret_t;

// 7.3.98 Read Extended Inquiry Length command

typedef struct HCI_Read_Extended_Inquiry_Length_Ret_t {
  uint8_t                  Status;
  uint16_t                 Extended_Inquiry_Length;
} HCI_Read_Extended_Inquiry_Length_Ret_t;

// 7.3.55 Read Extended Inquiry Response command

typedef struct HCI_Read_Extended_Inquiry_Response_Ret_t {
  uint8_t                  Status;
  uint8_t                  FEC_Required;
  uint8_t                  Extended_Inquiry_Response[240];
} HCI_Read_Extended_Inquiry_Response_Ret_t;

// 7.3.96 Read Extended Page Timeout command

typedef struct HCI_Read_Extended_Page_Timeout_Ret_t {
  uint8_t                  Status;
  uint16_t                 Extended_Page_Timeout;
} HCI_Read_Extended_Page_Timeout_Ret_t;

// 7.5.1 Read Failed Contact Counter command

typedef struct HCI_Read_Failed_Contact_Counter_Cmd_t {
  uint16_t                 Handle;
} HCI_Read_Failed_Contact_Counter_Cmd_t;

typedef struct HCI_Read_Failed_Contact_Counter_Ret_t {
  uint8_t                  Status;
  uint16_t                 Handle;
  uint16_t                 Failed_Contact_Counter;
} HCI_Read_Failed_Contact_Counter_Ret_t;

// 7.3.72 Read Flow Control Mode command

typedef struct HCI_Read_Flow_Control_Mode_Ret_t {
  uint8_t                  Status;
  uint8_t                  Flow_Control_Mode;
} HCI_Read_Flow_Control_Mode_Ret_t;

// 7.3.33 Read Hold Mode Activity command

typedef struct HCI_Read_Hold_Mode_Activity_Ret_t {
  uint8_t                  Status;
  uint8_t                  Hold_Mode_Activity;
} HCI_Read_Hold_Mode_Activity_Ret_t;

// 7.3.49 Read Inquiry Mode command

typedef struct HCI_Read_Inquiry_Mode_Ret_t {
  uint8_t                  Status;
  uint8_t                  Inquiry_Mode;
} HCI_Read_Inquiry_Mode_Ret_t;

// 7.3.61 Read Inquiry Response Transmit Power Level command

typedef struct HCI_Read_Inquiry_Response_Transmit_Power_Level_Ret_t {
  uint8_t                  Status;
  uint8_t                  TX_Power;
} HCI_Read_Inquiry_Response_Transmit_Power_Level_Ret_t;

// 7.3.21 Read Inquiry Scan Activity command

typedef struct HCI_Read_Inquiry_Scan_Activity_Ret_t {
  uint8_t                  Status;
  uint16_t                 Inquiry_Scan_Interval;
  uint16_t                 Inquiry_Scan_Window;
} HCI_Read_Inquiry_Scan_Activity_Ret_t;

// 7.3.47 Read Inquiry Scan Type command

typedef struct HCI_Read_Inquiry_Scan_Type_Ret_t {
  uint8_t                  Status;
  uint8_t                  Inquiry_Scan_Type;
} HCI_Read_Inquiry_Scan_Type_Ret_t;

// 7.3.78 Read LE Host Support command

typedef struct HCI_Read_LE_Host_Support_Ret_t {
  uint8_t                  Status;
  uint8_t                  LE_Supported_Host;
  uint8_t                  Unused;
} HCI_Read_LE_Host_Support_Ret_t;

// 7.2.9 Read Link Policy Settings command

// HCI_Read_Link_Policy_Settings_Cmd_t only has one member: Connection_Handle.

typedef struct HCI_Read_Link_Policy_Settings_Ret_t {
  Status_Conn_Handle_t     Status_Conn;
  uint16_t                 Link_Policy_Settings;
} HCI_Read_Link_Policy_Settings_Ret_t;

// 7.5.3 Read Link Quality command

typedef struct HCI_Read_Link_Quality_Cmd_t {
  uint16_t                 Handle;
} HCI_Read_Link_Quality_Cmd_t;

typedef struct HCI_Read_Link_Quality_Ret_t {
  uint8_t                  Status;
  uint16_t                 Handle;
  uint8_t                  Link_Quality;
} HCI_Read_Link_Quality_Ret_t;

// 7.3.41 Read Link Supervision Timeout command

typedef struct HCI_Read_Link_Supervision_Timeout_Cmd_t {
  uint16_t                 Handle;
} HCI_Read_Link_Supervision_Timeout_Cmd_t;

typedef struct HCI_Read_Link_Supervision_Timeout_Ret_t {
  uint8_t                  Status;
  uint16_t                 Handle;
  uint16_t                 Link_Supervision_Timeout;
} HCI_Read_Link_Supervision_Timeout_Ret_t;

// 7.1.25 Read LMP Handle command

// HCI_Read_LMP_Handle_Cmd_t only has one member: Connection_Handle.

typedef struct HCI_Read_LMP_Handle_Ret_t {
  Status_Conn_Handle_t     Status_Conn;
  uint8_t                  LMP_Handle;
  uint32_t                 Reserved;
} HCI_Read_LMP_Handle_Ret_t;

// 7.4.4 Read Local Extended Features command

typedef struct HCI_Read_Local_Extended_Features_Cmd_t {
  uint8_t                  Page_Number;
} HCI_Read_Local_Extended_Features_Cmd_t;

typedef struct HCI_Read_Local_Extended_Features_Ret_t {
  uint8_t                  Status;
  uint8_t                  Page_Number;
  uint8_t                  Max_Page_Number;
  uint8_t                  Extended_LMP_Features[8];
} HCI_Read_Local_Extended_Features_Ret_t;

// 7.3.12 Read Local Name command

typedef struct HCI_Read_Local_Name_Ret_t {
  uint8_t                  Status;
  uint8_t                  Local_Name[248];
} HCI_Read_Local_Name_Ret_t;

// 7.3.60 Read Local OOB Data command

typedef struct HCI_Read_Local_OOB_Data_Ret_t {
  uint8_t                  Status;
  uint8_t                  C[16];
  uint8_t                  R[16];
} HCI_Read_Local_OOB_Data_Ret_t;

// 7.3.95 Read Local OOB Extended Data command

typedef struct HCI_Read_Local_OOB_Extended_Data_Ret_t {
  uint8_t                  Status;
  uint8_t                  C_192[16];
  uint8_t                  R_192[16];
  uint8_t                  C_256[16];
  uint8_t                  R_256[16];
} HCI_Read_Local_OOB_Extended_Data_Ret_t;

// 7.4.9 Read Local Simple Pairing Options command

typedef struct HCI_Read_Local_Simple_Pairing_Options_Ret_t {
  uint8_t                  Status;
  uint8_t                  Simple_Pairing_Options;
  uint8_t                  Max_Encryption_Key_Size;
} HCI_Read_Local_Simple_Pairing_Options_Ret_t;

// 7.4.10 Read Local Supported Codec Capabilities command

typedef struct HCI_Read_Local_Supported_Codec_Capabilities_Cmd_t {
  uint8_t                  Codec_ID[5];
  uint8_t                  Logical_Transport_Type;
  uint8_t                  Direction;
} HCI_Read_Local_Supported_Codec_Capabilities_Cmd_t;

typedef struct HCI_Read_Local_Supported_Codec_Capabilities_Ret_t {
  uint8_t                  Status;
  uint8_t                  Num_Codec_Capabilities;
  struct {
    uint8_t                Codec_Capability_Length;
    uint8_t  *             Codec_Capability;
  } Codec_Capabilities[0];
} HCI_Read_Local_Supported_Codec_Capabilities_Ret_t;

// 7.4.8 Read Local Supported Codecs command

typedef struct HCI_Read_Local_Supported_Codecs_v2_Ret_t {
  uint8_t                  Status;
  uint8_t                  Num_Supported_Standard_Codecs;
  struct {
    uint8_t                Standard_Codec_ID;
    uint8_t                Standard_Codec_Transport;
  } * Supported_Standard_Codecs;
  uint8_t                  Num_Supported_Vendor_Specific_Codecs;
  uint8_t                  Vendor_Specific_Codec_ID;
  uint8_t                  Vendor_Specific_Codec_Transport;
} HCI_Read_Local_Supported_Codecs_v2_Ret_t;

typedef struct HCI_Read_Local_Supported_Codecs_v1_Ret_t {
  uint8_t                  Status;
  uint8_t                  Num_Supported_Standard_Codecs;
  uint8_t  *               Standard_Codec_ID;
  uint8_t                  Num_Supported_Vendor_Specific_Codecs;
  uint8_t                  Vendor_Specific_Codec_ID;
} HCI_Read_Local_Supported_Codecs_v1_Ret_t;

// 7.4.2 Read Local Supported Commands command

typedef struct HCI_Read_Local_Supported_Commands_Ret_t {
  uint8_t                  Status;
  uint8_t                  Supported_Commands[64];
} HCI_Read_Local_Supported_Commands_Ret_t;

// 7.4.11 Read Local Supported Controller Delay command

typedef struct HCI_Read_Local_Supported_Controller_Delay_Cmd_t {
  uint8_t                  Codec_ID[5];
  uint8_t                  Logical_Transport_Type;
  uint8_t                  Direction;
  uint8_t                  Codec_Configuration_Length;
  uint8_t  *               Codec_Configuration;
} HCI_Read_Local_Supported_Controller_Delay_Cmd_t;

typedef struct HCI_Read_Local_Supported_Controller_Delay_Ret_t {
  uint8_t                  Status;
  uint8_t                  Min_Controller_Delay[3];
  uint8_t                  Max_Controller_Delay[3];
} HCI_Read_Local_Supported_Controller_Delay_Ret_t;

// 7.4.3 Read Local Supported Features command

typedef struct HCI_Read_Local_Supported_Features_Ret_t {
  uint8_t                  Status;
  uint8_t                  LMP_Features[8];
} HCI_Read_Local_Supported_Features_Ret_t;

// 7.4.1 Read Local Version Information command

typedef struct HCI_Read_Local_Version_Information_Ret_t {
  uint8_t                  Status;
  uint8_t                  HCI_Version;
  uint16_t                 HCI_Subversion;
  uint8_t                  LMP_Version;
  uint16_t                 Company_Identifier;
  uint16_t                 LMP_Subversion;
} HCI_Read_Local_Version_Information_Ret_t;

// 7.6.1 Read Loopback Mode command

typedef struct HCI_Read_Loopback_Mode_Ret_t {
  uint8_t                  Status;
  uint8_t                  Loopback_Mode;
} HCI_Read_Loopback_Mode_Ret_t;

// 7.3.31 Read Num Broadcast Retransmissions command

typedef struct HCI_Read_Num_Broadcast_Retransmissions_Ret_t {
  uint8_t                  Status;
  uint8_t                  Num_Broadcast_Retransmissions;
} HCI_Read_Num_Broadcast_Retransmissions_Ret_t;

// 7.3.43 Read Number Of Supported IAC command

typedef struct HCI_Read_Number_Of_Supported_IAC_Ret_t {
  uint8_t                  Status;
  uint8_t                  Num_Supported_IAC;
} HCI_Read_Number_Of_Supported_IAC_Ret_t;

// 7.3.19 Read Page Scan Activity command

typedef struct HCI_Read_Page_Scan_Activity_Ret_t {
  uint8_t                  Status;
  uint16_t                 Page_Scan_Interval;
  uint16_t                 Page_Scan_Window;
} HCI_Read_Page_Scan_Activity_Ret_t;

// 7.3.51 Read Page Scan Type command

typedef struct HCI_Read_Page_Scan_Type_Ret_t {
  uint8_t                  Status;
  uint8_t                  Page_Scan_Type;
} HCI_Read_Page_Scan_Type_Ret_t;

// 7.3.15 Read Page Timeout command

typedef struct HCI_Read_Page_Timeout_Ret_t {
  uint8_t                  Status;
  uint16_t                 Page_Timeout;
} HCI_Read_Page_Timeout_Ret_t;

// 7.3.5 Read PIN Type command

typedef struct HCI_Read_PIN_Type_Ret_t {
  uint8_t                  Status;
  uint8_t                  PIN_Type;
} HCI_Read_PIN_Type_Ret_t;

// 7.1.22 Read Remote Extended Features command

typedef struct HCI_Read_Remote_Extended_Features_Cmd_t {
  uint16_t                 Connection_Handle;
  uint8_t                  Page_Number;
} HCI_Read_Remote_Extended_Features_Cmd_t;

// 7.7.34 Read Remote Extended Features Complete event

typedef struct HCI_Read_Remote_Extended_Features_Complete_Evt_t {
  Status_Conn_Handle_t     Status_Conn;
  uint8_t                  Page_Number;
  uint8_t                  Max_Page_Number;
  uint8_t                  Extended_LMP_Features[8];
} HCI_Read_Remote_Extended_Features_Complete_Evt_t;

// 7.1.21 Read Remote Supported Features command

// HCI_Read_Remote_Supported_Features_Cmd_t only has one member: Connection_Handle.

// 7.7.11 Read Remote Supported Features Complete event

typedef struct HCI_Read_Remote_Supported_Features_Complete_Evt_t {
  Status_Conn_Handle_t     Status_Conn;
  uint8_t                  LMP_Features[8];
} HCI_Read_Remote_Supported_Features_Complete_Evt_t;

// 7.1.23 Read Remote Version Information command

// HCI_Read_Remote_Version_Information_Cmd_t only has one member: Connection_Handle.

// 7.7.12 Read Remote Version Information Complete event

typedef struct HCI_Read_Remote_Version_Information_Complete_Evt_t {
  Status_Conn_Handle_t     Status_Conn;
  uint8_t                  Version;
  uint16_t                 Company_Identifier;
  uint16_t                 Subversion;
} HCI_Read_Remote_Version_Information_Complete_Evt_t;

// 7.5.4 Read RSSI command

typedef struct HCI_Read_RSSI_Cmd_t {
  uint16_t                 Handle;
} HCI_Read_RSSI_Cmd_t;

typedef struct HCI_Read_RSSI_Ret_t {
  uint8_t                  Status;
  uint16_t                 Handle;
  uint8_t                  RSSI;
} HCI_Read_RSSI_Ret_t;

// 7.3.17 Read Scan Enable command

typedef struct HCI_Read_Scan_Enable_Ret_t {
  uint8_t                  Status;
  uint8_t                  Scan_Enable;
} HCI_Read_Scan_Enable_Ret_t;

// 7.3.91 Read Secure Connections Host Support command

typedef struct HCI_Read_Secure_Connections_Host_Support_Ret_t {
  uint8_t                  Status;
  uint8_t                  Secure_Connections_Host_Support;
} HCI_Read_Secure_Connections_Host_Support_Ret_t;

// 7.3.58 Read Simple Pairing Mode command

typedef struct HCI_Read_Simple_Pairing_Mode_Ret_t {
  uint8_t                  Status;
  uint8_t                  Simple_Pairing_Mode;
} HCI_Read_Simple_Pairing_Mode_Ret_t;

// 7.3.8 Read Stored Link Key command

typedef struct HCI_Read_Stored_Link_Key_Cmd_t {
  BD_Addr_t                BD_Addr;
  uint8_t                  Read_All;
} HCI_Read_Stored_Link_Key_Cmd_t;

typedef struct HCI_Read_Stored_Link_Key_Ret_t {
  uint8_t                  Status;
  uint16_t                 Max_Num_Keys;
  uint16_t                 Num_Keys_Read;
} HCI_Read_Stored_Link_Key_Ret_t;

// 7.3.89 Read Synchronization Train Parameters command

typedef struct HCI_Read_Synchronization_Train_Parameters_Ret_t {
  uint8_t                  Status;
  uint16_t                 Sync_Train_Interval;
  uint32_t                 Sync_Train_Timeout;
  uint8_t                  Service_Data;
} HCI_Read_Synchronization_Train_Parameters_Ret_t;

// 7.3.36 Read Synchronous Flow Control Enable command

typedef struct HCI_Read_Sync_Flow_Control_Enable_Ret_t {
  uint8_t                  Status;
  uint8_t                  Synchronous_Flow_Control_Enable;
} HCI_Read_Sync_Flow_Control_Enable_Ret_t;

// 7.3.35 Read Transmit Power Level command

typedef struct HCI_Read_Transmit_Power_Level_Cmd_t {
  uint16_t                 Connection_Handle;
  uint8_t                  Type;
} HCI_Read_Transmit_Power_Level_Cmd_t;

typedef struct HCI_Read_Transmit_Power_Level_Ret_t {
  Status_Conn_Handle_t     Status_Conn;
  uint8_t                  TX_Power_Level;
} HCI_Read_Transmit_Power_Level_Ret_t;

// 7.3.27 Read Voice Setting command

typedef struct HCI_Read_Voice_Setting_Ret_t {
  uint8_t                  Status;
  uint16_t                 Voice_Setting;
} HCI_Read_Voice_Setting_Ret_t;

// 7.1.52 Receive Synchronization Train command

typedef struct HCI_Receive_Synchronization_Train_Cmd_t {
  BD_Addr_t                BD_Addr;
  uint16_t                 Sync_Scan_Timeout;
  uint16_t                 Sync_Scan_Window;
  uint16_t                 Sync_Scan_Interval;
} HCI_Receive_Synchronization_Train_Cmd_t;

// 7.3.57 Refresh Encryption Key command

// HCI_Refresh_Encryption_Key_Cmd_t only has one member: Connection_Handle.

// 7.1.9 Reject Connection Request command

typedef struct HCI_Reject_Conn_Req_Cmd_t {
  BD_Addr_t                BD_Addr;
  uint8_t                  Reason;
} HCI_Reject_Conn_Req_Cmd_t;

// 7.1.28 Reject Synchronous Connection Request command

typedef struct HCI_Reject_Sync_Conn_Req_Cmd_t {
  BD_Addr_t                BD_Addr;
  uint8_t                  Reason;
} HCI_Reject_Sync_Conn_Req_Cmd_t;

// 7.7.50 Remote Host Supported Features Notification event

typedef struct HCI_Remote_Host_Supported_Features_Notification_Evt_t {
  BD_Addr_t                BD_Addr;
  uint8_t                  Host_Supported_Features[8];
} HCI_Remote_Host_Supported_Features_Notification_Evt_t;

// 7.1.20 Remote Name Request Cancel command

typedef struct HCI_Remote_Name_Req_Cancel_Cmd_t {
  BD_Addr_t                BD_Addr;
} HCI_Remote_Name_Req_Cancel_Cmd_t;

// HCI_Remote_Name_Req_Cancel_Ret_t only has one member: Status_BD_Addr.

// 7.1.19 Remote Name Request command

typedef struct HCI_Remote_Name_Request_Cmd_t {
  BD_Addr_t                BD_Addr;
  uint8_t                  Page_Scan_Repetition_Mode;
  uint8_t                  Reserved;
  uint16_t                 Clock_Offset;
} HCI_Remote_Name_Request_Cmd_t;

// 7.7.7 Remote Name Request Complete event

typedef struct HCI_Remote_Name_Req_Complete_Evt_t {
  Status_BD_Addr_t         Status_BD_Addr;
  uint8_t                  Remote_Name[248];
} HCI_Remote_Name_Req_Complete_Evt_t;

// 7.7.44 Remote OOB Data Request event

typedef struct HCI_Remote_OOB_Data_Request_Evt_t {
  BD_Addr_t                BD_Addr;
} HCI_Remote_OOB_Data_Request_Evt_t;

// 7.1.35 Remote OOB Data Request Negative Reply command

typedef struct HCI_Remote_OOB_Data_Req_Negative_Reply_Cmd_t {
  BD_Addr_t                BD_Addr;
} HCI_Remote_OOB_Data_Req_Negative_Reply_Cmd_t;

// HCI_Remote_OOB_Data_Req_Negative_Reply_Ret_t only has one member: Status_BD_Addr.

// 7.1.34 Remote OOB Data Request Reply command

typedef struct HCI_Remote_OOB_Data_Req_Reply_Cmd_t {
  BD_Addr_t                BD_Addr;
  uint8_t                  C[16];
  uint8_t                  R[16];
} HCI_Remote_OOB_Data_Req_Reply_Cmd_t;

// HCI_Remote_OOB_Data_Req_Reply_Ret_t only has one member: Status_BD_Addr.

// 7.1.53 Remote OOB Extended Data Request Reply command

typedef struct HCI_Remote_OOB_Extended_Data_Req_Reply_Cmd_t {
  BD_Addr_t                BD_Addr;
  uint8_t                  C_192[16];
  uint8_t                  R_192[16];
  uint8_t                  C_256[16];
  uint8_t                  R_256[16];
} HCI_Remote_OOB_Extended_Data_Req_Reply_Cmd_t;

// HCI_Remote_OOB_Extended_Data_Req_Reply_Ret_t only has one member: Status_BD_Addr.

// 7.3.2 Reset command

// HCI_Reset_Ret_t only has one member: Status.

// 7.5.2 Reset Failed Contact Counter command

typedef struct HCI_Reset_Failed_Contact_Counter_Cmd_t {
  uint16_t                 Handle;
} HCI_Reset_Failed_Contact_Counter_Cmd_t;

typedef struct HCI_Reset_Failed_Contact_Counter_Ret_t {
  uint8_t                  Status;
  uint16_t                 Handle;
} HCI_Reset_Failed_Contact_Counter_Ret_t;

// 7.7.21 Return Link Keys event

typedef struct HCI_Return_Link_Keys_Evt_t {
  uint8_t                  Num_Keys;
  struct {
    BD_Addr_t              BD_Addr;
    uint8_t                Link_Key[16];
  } Keys[0];
} HCI_Return_Link_Keys_Evt_t;

// 7.7.18 Role Change event

typedef struct HCI_Role_Change_Evt_t {
  Status_BD_Addr_t         Status_BD_Addr;
  uint8_t                  New_Role;
} HCI_Role_Change_Evt_t;

// 7.2.7 Role Discovery command

// HCI_Role_Discovery_Cmd_t only has one member: Connection_Handle.

typedef struct HCI_Role_Discovery_Ret_t {
  Status_Conn_Handle_t     Status_Conn;
  uint8_t                  Current_Role;
} HCI_Role_Discovery_Ret_t;

// 7.7.76 SAM Status Change event

typedef struct HCI_SAM_Status_Change_Evt_t {
  uint16_t                 Connection_Handle;
  uint8_t                  Local_SAM_Index;
  uint8_t                  Local_SAM_TX_Availability;
  uint8_t                  Local_SAM_RX_Availability;
  uint8_t                  Remote_SAM_Index;
  uint8_t                  Remote_SAM_TX_Availability;
  uint8_t                  Remote_SAM_RX_Availability;
} HCI_SAM_Status_Change_Evt_t;

// 7.3.63 Send Keypress Notification command

typedef struct HCI_Send_Keypress_Notification_Cmd_t {
  BD_Addr_t                BD_Addr;
  uint8_t                  Notification_Type;
} HCI_Send_Keypress_Notification_Cmd_t;

// HCI_Send_Keypress_Notification_Ret_t only has one member: Status_BD_Addr.

// 7.3.46 Set AFH Host Channel Classification command

typedef struct HCI_Set_AFH_Host_Channel_Classification_Cmd_t {
  uint8_t                  AFH_Host_Channel_Classification[10];
} HCI_Set_AFH_Host_Channel_Classification_Cmd_t;

// HCI_Set_AFH_Host_Channel_Classification_Ret_t only has one member: Status.

// 7.1.16 Set Connection Encryption command

typedef struct HCI_Set_Conn_Encryption_Cmd_t {
  uint16_t                 Connection_Handle;
  uint8_t                  Encryption_Enable;
} HCI_Set_Conn_Encryption_Cmd_t;

// 7.1.49 Set Connectionless Peripheral Broadcast command

typedef struct HCI_Set_Connectionless_PBCast_Cmd_t {
  uint8_t                  Enable;
  uint8_t                  LT_ADDR;
  uint8_t                  LPO_Allowed;
  uint16_t                 Packet_Type;
  uint16_t                 Interval_Min;
  uint16_t                 Interval_Max;
  uint16_t                 Supervision_Timeout;
} HCI_Set_Connectionless_PBCast_Cmd_t;

typedef struct HCI_Set_Connectionless_PBCast_Ret_t {
  uint8_t                  Status;
  uint8_t                  LT_ADDR;
  uint16_t                 Interval;
} HCI_Set_Connectionless_PBCast_Ret_t;

// 7.3.88 Set Connectionless Peripheral Broadcast Data command

typedef struct HCI_Set_Connectionless_PBCast_Data_Cmd_t {
  uint8_t                  LT_ADDR;
  uint8_t                  Fragment;
  uint8_t                  Data_Length;
  uint8_t  *               Data;
} HCI_Set_Connectionless_PBCast_Data_Cmd_t;

typedef struct HCI_Set_Connectionless_PBCast_Data_Ret_t {
  uint8_t                  Status;
  uint8_t                  LT_ADDR;
} HCI_Set_Connectionless_PBCast_Data_Ret_t;

// 7.1.50 Set Connectionless Peripheral Broadcast Receive command

typedef struct HCI_Set_Connectionless_PBCast_Receive_Cmd_t {
  uint8_t                  Enable;
  BD_Addr_t                BD_Addr;
  uint8_t                  LT_ADDR;
  uint16_t                 Interval;
  uint32_t                 Clock_Offset;
  uint32_t                 Next_Connectionless_PBCast_Clock;
  uint16_t                 Supervision_Timeout;
  uint8_t                  Remote_Timing_Accuracy;
  uint8_t                  Skip;
  uint16_t                 Packet_Type;
  uint8_t                  AFH_Channel_Map[10];
} HCI_Set_Connectionless_PBCast_Receive_Cmd_t;

typedef struct HCI_Set_Connectionless_PBCast_Receive_Ret_t {
  Status_BD_Addr_t         Status_BD_Addr;
  uint8_t                  LT_ADDR;
} HCI_Set_Connectionless_PBCast_Receive_Ret_t;

// 7.3.38 Set Controller To Host Flow Control command

typedef struct HCI_Set_Controller_To_Host_Flow_Control_Cmd_t {
  uint8_t                  Flow_Control_Enable;
} HCI_Set_Controller_To_Host_Flow_Control_Cmd_t;

// HCI_Set_Controller_To_Host_Flow_Control_Ret_t only has one member: Status.

// 7.3.100 Set Ecosystem Base Interval command

typedef struct HCI_Set_Ecosystem_Base_Interval_Cmd_t {
  uint16_t                 Interval;
} HCI_Set_Ecosystem_Base_Interval_Cmd_t;

// HCI_Set_Ecosystem_Base_Interval_Ret_t only has one member: Status.

// 7.3.3 Set Event Filter command

typedef struct HCI_Set_Event_Filter_Cmd_t {
  uint8_t                  Filter_Type;
  uint8_t                  Filter_Condition_Type;
  uint8_t                  Condition[7];
} HCI_Set_Event_Filter_Cmd_t;

// HCI_Set_Event_Filter_Ret_t only has one member: Status.

// 7.3.1 Set Event Mask command

typedef struct HCI_Set_Event_Mask_Cmd_t {
  uint8_t                  Event_Mask[8];
} HCI_Set_Event_Mask_Cmd_t;

// HCI_Set_Event_Mask_Ret_t only has one member: Status.

// 7.3.69 Set Event Mask Page 2 command

typedef struct HCI_Set_Event_Mask_Page_2_Cmd_t {
  uint8_t                  Event_Mask_Page_2[8];
} HCI_Set_Event_Mask_Page_2_Cmd_t;

// HCI_Set_Event_Mask_Page_2_Ret_t only has one member: Status.

// 7.3.81 Set External Frame Configuration command

typedef struct HCI_Set_External_Frame_Configuration_Cmd_t {
  uint16_t                 MWS_Frame_Duration;
  uint16_t                 MWS_Frame_Sync_Assert_Offset;
  uint16_t                 MWS_Frame_Sync_Assert_Jitter;
  uint8_t                  MWS_Num_Periods;
  struct {
    uint16_t               Period_Duration;
    uint8_t                Period_Type;
  } Periods[0];
} HCI_Set_External_Frame_Configuration_Cmd_t;

// HCI_Set_External_Frame_Configuration_Ret_t only has one member: Status.

// 7.3.102 Set Min Encryption Key Size command

typedef struct HCI_Set_Min_Encryption_Key_Size_Cmd_t {
  uint8_t                  Min_Encryption_Key_Size;
} HCI_Set_Min_Encryption_Key_Size_Cmd_t;

// HCI_Set_Min_Encryption_Key_Size_Ret_t only has one member: Status.

// 7.3.80 Set MWS Channel Parameters command

typedef struct HCI_Set_MWS_Channel_Parameters_Cmd_t {
  uint8_t                  MWS_Channel_Enable;
  uint16_t                 MWS_RX_Center_Frequency;
  uint16_t                 MWS_TX_Center_Frequency;
  uint16_t                 MWS_RX_Channel_Bandwidth;
  uint16_t                 MWS_TX_Channel_Bandwidth;
  uint8_t                  MWS_Channel_Type;
} HCI_Set_MWS_Channel_Parameters_Cmd_t;

// HCI_Set_MWS_Channel_Parameters_Ret_t only has one member: Status.

// 7.3.84 Set MWS Scan Frequency Table command

typedef struct HCI_Set_MWS_Scan_Frequency_Table_Cmd_t {
  uint8_t                  Num_Scan_Frequencies;
  struct {
    uint16_t               Scan_Frequency_Low;
    uint16_t               Scan_Frequency_High;
  } Scan_Frequencies[0];
} HCI_Set_MWS_Scan_Frequency_Table_Cmd_t;

// HCI_Set_MWS_Scan_Frequency_Table_Ret_t only has one member: Status.

// 7.3.82 Set MWS Signaling command

typedef struct HCI_Set_MWS_Signaling_Cmd_t {
  uint16_t                 MWS_RX_Assert_Offset;
  uint16_t                 MWS_RX_Assert_Jitter;
  uint16_t                 MWS_RX_Deassert_Offset;
  uint16_t                 MWS_RX_Deassert_Jitter;
  uint16_t                 MWS_TX_Assert_Offset;
  uint16_t                 MWS_TX_Assert_Jitter;
  uint16_t                 MWS_TX_Deassert_Offset;
  uint16_t                 MWS_TX_Deassert_Jitter;
  uint16_t                 MWS_Pattern_Assert_Offset;
  uint16_t                 MWS_Pattern_Assert_Jitter;
  uint16_t                 MWS_Inactivity_Duration_Assert_Offset;
  uint16_t                 MWS_Inactivity_Duration_Assert_Jitter;
  uint16_t                 MWS_Scan_Frequency_Assert_Offset;
  uint16_t                 MWS_Scan_Frequency_Assert_Jitter;
  uint16_t                 MWS_Priority_Assert_Offset_Request;
} HCI_Set_MWS_Signaling_Cmd_t;

typedef struct HCI_Set_MWS_Signaling_Ret_t {
  uint8_t                  Status;
  uint16_t                 Bluetooth_RX_Priority_Assert_Offset;
  uint16_t                 Bluetooth_RX_Priority_Assert_Jitter;
  uint16_t                 Bluetooth_RX_Priority_Deassert_Offset;
  uint16_t                 Bluetooth_RX_Priority_Deassert_Jitter;
  uint16_t                 IEEE_802_RX_Priority_Assert_Offset;
  uint16_t                 IEEE_802_RX_Priority_Assert_Jitter;
  uint16_t                 IEEE_802_RX_Priority_Deassert_Offset;
  uint16_t                 IEEE_802_RX_Priority_Deassert_Jitter;
  uint16_t                 Bluetooth_TX_On_Assert_Offset;
  uint16_t                 Bluetooth_TX_On_Assert_Jitter;
  uint16_t                 Bluetooth_TX_On_Deassert_Offset;
  uint16_t                 Bluetooth_TX_On_Deassert_Jitter;
  uint16_t                 IEEE_802_TX_On_Assert_Offset;
  uint16_t                 IEEE_802_TX_On_Assert_Jitter;
  uint16_t                 IEEE_802_TX_On_Deassert_Offset;
  uint16_t                 IEEE_802_TX_On_Deassert_Jitter;
} HCI_Set_MWS_Signaling_Ret_t;

// 7.3.83 Set MWS Transport Layer command

typedef struct HCI_Set_MWS_Transport_Layer_Cmd_t {
  uint8_t                  Transport_Layer;
  uint32_t                 To_MWS_Baud_Rate;
  uint32_t                 From_MWS_Baud_Rate;
} HCI_Set_MWS_Transport_Layer_Cmd_t;

// HCI_Set_MWS_Transport_Layer_Ret_t only has one member: Status.

// 7.3.85 Set MWS_PATTERN Configuration command

typedef struct HCI_Set_MWS_PATTERN_Configuration_Cmd_t {
  uint8_t                  MWS_Pattern_Index;
  uint8_t                  MWS_Pattern_Num_Intervals;
  struct {
    uint8_t                MWS_Pattern_Interval_Duration;
    uint8_t                MWS_Pattern_Interval_Type;
  } Intervals[0];
} HCI_Set_MWS_PATTERN_Configuration_Cmd_t;

// HCI_Set_MWS_PATTERN_Configuration_Ret_t only has one member: Status.

// 7.3.86 Set Reserved LT_ADDR command

typedef struct HCI_Set_Reserved_LT_ADDR_Cmd_t {
  uint8_t                  LT_ADDR;
} HCI_Set_Reserved_LT_ADDR_Cmd_t;

typedef struct HCI_Set_Reserved_LT_ADDR_Ret_t {
  uint8_t                  Status;
  uint8_t                  LT_ADDR;
} HCI_Set_Reserved_LT_ADDR_Ret_t;

// 7.5.12 Set Triggered Clock Capture command

typedef struct HCI_Set_Triggered_Clock_Capture_Cmd_t {
  uint16_t                 Connection_Handle;
  uint8_t                  Enable;
  uint8_t                  Which_Clock;
  uint8_t                  LPO_Allowed;
  uint8_t                  Num_Clock_Captures_To_Filter;
} HCI_Set_Triggered_Clock_Capture_Cmd_t;

// HCI_Set_Triggered_Clock_Capture_Ret_t only has one member: Status.

// 7.1.26 Setup Synchronous Connection command

typedef struct HCI_Setup_Sync_Connection_Cmd_t {
  uint16_t                 Connection_Handle;
  uint32_t                 Transmit_Bandwidth;
  uint32_t                 Receive_Bandwidth;
  uint16_t                 Max_Latency;
  uint16_t                 Voice_Setting;
  uint8_t                  Retransmission_Effort;
  uint16_t                 Packet_Type;
} HCI_Setup_Sync_Connection_Cmd_t;

// 7.7.45 Simple Pairing Complete event

// HCI_Simple_Pairing_Complete_Evt_t only has one member: Status_BD_Addr.

// 7.2.2 Sniff Mode command

typedef struct HCI_Sniff_Mode_Cmd_t {
  uint16_t                 Connection_Handle;
  uint16_t                 Sniff_Max_Interval;
  uint16_t                 Sniff_Min_Interval;
  uint16_t                 Sniff_Attempt;
  uint16_t                 Sniff_Timeout;
} HCI_Sniff_Mode_Cmd_t;

// 7.2.14 Sniff Subrating command

typedef struct HCI_Sniff_Subrating_Cmd_t {
  uint16_t                 Connection_Handle;
  uint16_t                 Max_Latency;
  uint16_t                 Min_Remote_Timeout;
  uint16_t                 Min_Local_Timeout;
} HCI_Sniff_Subrating_Cmd_t;

// HCI_Sniff_Subrating_Ret_t only has one member: Status_Conn.

// 7.7.37 Sniff Subrating event

typedef struct HCI_Sniff_Subrating_Evt_t {
  Status_Conn_Handle_t     Status_Conn;
  uint16_t                 Max_TX_Latency;
  uint16_t                 Max_RX_Latency;
  uint16_t                 Min_Remote_Timeout;
  uint16_t                 Min_Local_Timeout;
} HCI_Sniff_Subrating_Evt_t;

// 7.2.8 Switch Role command

typedef struct HCI_Switch_Role_Cmd_t {
  BD_Addr_t                BD_Addr;
  uint8_t                  Role;
} HCI_Switch_Role_Cmd_t;

// 7.7.67 Synchronization Train Complete event

// HCI_Synchronization_Train_Complete_Evt_t only has one member: Status.

// 7.7.68 Synchronization Train Received event

typedef struct HCI_Synchronization_Train_Received_Evt_t {
  Status_BD_Addr_t         Status_BD_Addr;
  uint32_t                 Clock_Offset;
  uint8_t                  AFH_Channel_Map[10];
  uint8_t                  LT_ADDR;
  uint32_t                 Next_Broadcast_Instant;
  uint16_t                 Connectionless_PBCast_Interval;
  uint8_t                  Service_Data;
} HCI_Synchronization_Train_Received_Evt_t;

// 7.7.36 Synchronous Connection Changed event

typedef struct HCI_Sync_Conn_Changed_Evt_t {
  Status_Conn_Handle_t     Status_Conn;
  uint8_t                  Transmission_Interval;
  uint8_t                  Retransmission_Window;
  uint16_t                 RX_Packet_Length;
  uint16_t                 TX_Packet_Length;
} HCI_Sync_Conn_Changed_Evt_t;

// 7.7.35 Synchronous Connection Complete event

typedef struct HCI_Sync_Conn_Complete_Evt_t {
  Status_Conn_Handle_t     Status_Conn;
  BD_Addr_t                BD_Addr;
  uint8_t                  Link_Type;
  uint8_t                  Transmission_Interval;
  uint8_t                  Retransmission_Window;
  uint16_t                 RX_Packet_Length;
  uint16_t                 TX_Packet_Length;
  uint8_t                  Air_Mode;
} HCI_Sync_Conn_Complete_Evt_t;

// 7.7.66 Triggered Clock Capture event

typedef struct HCI_Triggered_Clock_Capture_Evt_t {
  uint16_t                 Connection_Handle;
  uint8_t                  Which_Clock;
  uint32_t                 Clock;
  uint16_t                 Slot_Offset;
} HCI_Triggered_Clock_Capture_Evt_t;

// 7.1.48 Truncated Page Cancel command

typedef struct HCI_Truncated_Page_Cancel_Cmd_t {
  BD_Addr_t                BD_Addr;
} HCI_Truncated_Page_Cancel_Cmd_t;

// HCI_Truncated_Page_Cancel_Ret_t only has one member: Status_BD_Addr.

// 7.1.47 Truncated Page command

typedef struct HCI_Truncated_Page_Cmd_t {
  BD_Addr_t                BD_Addr;
  uint8_t                  Page_Scan_Repetition_Mode;
  uint16_t                 Clock_Offset;
} HCI_Truncated_Page_Cmd_t;

// 7.7.71 Truncated Page Complete event

// HCI_Truncated_Page_Complete_Evt_t only has one member: Status_BD_Addr.

// 7.7.42 User Confirmation Request event

typedef struct HCI_User_Confirmation_Request_Evt_t {
  BD_Addr_t                BD_Addr;
  uint32_t                 Numeric_Value;
} HCI_User_Confirmation_Request_Evt_t;

// 7.1.31 User Confirmation Request Negative Reply command

typedef struct HCI_User_Confirmation_Req_Negative_Reply_Cmd_t {
  BD_Addr_t                BD_Addr;
} HCI_User_Confirmation_Req_Negative_Reply_Cmd_t;

// HCI_User_Confirmation_Req_Negative_Reply_Ret_t only has one member: Status_BD_Addr.

// 7.1.30 User Confirmation Request Reply command

typedef struct HCI_User_Confirmation_Req_Reply_Cmd_t {
  BD_Addr_t                BD_Addr;
} HCI_User_Confirmation_Req_Reply_Cmd_t;

// HCI_User_Confirmation_Req_Reply_Ret_t only has one member: Status_BD_Addr.

// 7.7.48 User Passkey Notification event

typedef struct HCI_User_Passkey_Notification_Evt_t {
  BD_Addr_t                BD_Addr;
  uint32_t                 Passkey;
} HCI_User_Passkey_Notification_Evt_t;

// 7.7.43 User Passkey Request event

typedef struct HCI_User_Passkey_Request_Evt_t {
  BD_Addr_t                BD_Addr;
} HCI_User_Passkey_Request_Evt_t;

// 7.1.33 User Passkey Request Negative Reply command

typedef struct HCI_User_Passkey_Req_Negative_Reply_Cmd_t {
  BD_Addr_t                BD_Addr;
} HCI_User_Passkey_Req_Negative_Reply_Cmd_t;

// HCI_User_Passkey_Req_Negative_Reply_Ret_t only has one member: Status_BD_Addr.

// 7.1.32 User Passkey Request Reply command

typedef struct HCI_User_Passkey_Req_Reply_Cmd_t {
  BD_Addr_t                BD_Addr;
  uint32_t                 Numeric_Value;
} HCI_User_Passkey_Req_Reply_Cmd_t;

// HCI_User_Passkey_Req_Reply_Ret_t only has one member: Status_BD_Addr.

// 7.3.54 Write AFH Channel Assessment Mode command

typedef struct HCI_Write_AFH_Channel_Assessment_Mode_Cmd_t {
  uint8_t                  AFH_Channel_Assessment_Mode;
} HCI_Write_AFH_Channel_Assessment_Mode_Cmd_t;

// HCI_Write_AFH_Channel_Assessment_Mode_Ret_t only has one member: Status.

// 7.3.94 Write Authenticated Payload Timeout command

typedef struct HCI_Write_Authenticated_Payload_Timeout_Cmd_t {
  uint16_t                 Connection_Handle;
  uint16_t                 Authenticated_Payload_Timeout;
} HCI_Write_Authenticated_Payload_Timeout_Cmd_t;

// HCI_Write_Authenticated_Payload_Timeout_Ret_t only has one member: Status_Conn.

// 7.3.24 Write Authentication Enable command

typedef struct HCI_Write_Authentication_Enable_Cmd_t {
  uint8_t                  Authentication_Enable;
} HCI_Write_Authentication_Enable_Cmd_t;

// HCI_Write_Authentication_Enable_Ret_t only has one member: Status.

// 7.3.30 Write Automatic Flush Timeout command

typedef struct HCI_Write_Automatic_Flush_Timeout_Cmd_t {
  uint16_t                 Connection_Handle;
  uint16_t                 Flush_Timeout;
} HCI_Write_Automatic_Flush_Timeout_Cmd_t;

// HCI_Write_Automatic_Flush_Timeout_Ret_t only has one member: Status_Conn.

// 7.3.26 Write Class of Device command

typedef struct HCI_Write_Class_Of_Device_Cmd_t {
  uint8_t                  Class_Of_Device[3];
} HCI_Write_Class_Of_Device_Cmd_t;

// HCI_Write_Class_Of_Device_Ret_t only has one member: Status.

// 7.3.14 Write Connection Accept Timeout command

typedef struct HCI_Write_Conn_Accept_Timeout_Cmd_t {
  uint16_t                 Connection_Accept_Timeout;
} HCI_Write_Conn_Accept_Timeout_Cmd_t;

// HCI_Write_Conn_Accept_Timeout_Ret_t only has one member: Status.

// 7.3.45 Write Current IAC LAP command

typedef struct HCI_Write_Current_IAC_LAP_Cmd_t {
  uint8_t                  Num_Current_IAC;
  uint8_t  *               IAC_LAP;
} HCI_Write_Current_IAC_LAP_Cmd_t;

// HCI_Write_Current_IAC_LAP_Ret_t only has one member: Status.

// 7.3.65 Write Default Erroneous Data Reporting command

typedef struct HCI_Write_Default_Erroneous_Data_Reporting_Cmd_t {
  uint8_t                  Erroneous_Data_Reporting;
} HCI_Write_Default_Erroneous_Data_Reporting_Cmd_t;

// HCI_Write_Default_Erroneous_Data_Reporting_Ret_t only has one member: Status.

// 7.2.12 Write Default Link Policy Settings command

typedef struct HCI_Write_Default_Link_Policy_Settings_Cmd_t {
  uint16_t                 Default_Link_Policy_Settings;
} HCI_Write_Default_Link_Policy_Settings_Cmd_t;

// HCI_Write_Default_Link_Policy_Settings_Ret_t only has one member: Status.

// 7.3.99 Write Extended Inquiry Length command

typedef struct HCI_Write_Extended_Inquiry_Length_Cmd_t {
  uint16_t                 Extended_Inquiry_Length;
} HCI_Write_Extended_Inquiry_Length_Cmd_t;

// HCI_Write_Extended_Inquiry_Length_Ret_t only has one member: Status.

// 7.3.56 Write Extended Inquiry Response command

typedef struct HCI_Write_Extended_Inquiry_Response_Cmd_t {
  uint8_t                  FEC_Required;
  uint8_t                  Extended_Inquiry_Response[240];
} HCI_Write_Extended_Inquiry_Response_Cmd_t;

// HCI_Write_Extended_Inquiry_Response_Ret_t only has one member: Status.

// 7.3.97 Write Extended Page Timeout command

typedef struct HCI_Write_Extended_Page_Timeout_Cmd_t {
  uint16_t                 Extended_Page_Timeout;
} HCI_Write_Extended_Page_Timeout_Cmd_t;

// HCI_Write_Extended_Page_Timeout_Ret_t only has one member: Status.

// 7.3.73 Write Flow Control Mode command

typedef struct HCI_Write_Flow_Control_Mode_Cmd_t {
  uint8_t                  Flow_Control_Mode;
} HCI_Write_Flow_Control_Mode_Cmd_t;

// HCI_Write_Flow_Control_Mode_Ret_t only has one member: Status.

// 7.3.34 Write Hold Mode Activity command

typedef struct HCI_Write_Hold_Mode_Activity_Cmd_t {
  uint8_t                  Hold_Mode_Activity;
} HCI_Write_Hold_Mode_Activity_Cmd_t;

// HCI_Write_Hold_Mode_Activity_Ret_t only has one member: Status.

// 7.3.50 Write Inquiry Mode command

typedef struct HCI_Write_Inquiry_Mode_Cmd_t {
  uint8_t                  Inquiry_Mode;
} HCI_Write_Inquiry_Mode_Cmd_t;

// HCI_Write_Inquiry_Mode_Ret_t only has one member: Status.

// 7.3.22 Write Inquiry Scan Activity command

typedef struct HCI_Write_Inquiry_Scan_Activity_Cmd_t {
  uint16_t                 Inquiry_Scan_Interval;
  uint16_t                 Inquiry_Scan_Window;
} HCI_Write_Inquiry_Scan_Activity_Cmd_t;

// HCI_Write_Inquiry_Scan_Activity_Ret_t only has one member: Status.

// 7.3.48 Write Inquiry Scan Type command

typedef struct HCI_Write_Inquiry_Scan_Type_Cmd_t {
  uint8_t                  Scan_Type;
} HCI_Write_Inquiry_Scan_Type_Cmd_t;

// HCI_Write_Inquiry_Scan_Type_Ret_t only has one member: Status.

// 7.3.62 Write Inquiry Transmit Power Level command

typedef struct HCI_Write_Inquiry_Transmit_Power_Level_Cmd_t {
  uint8_t                  TX_Power;
} HCI_Write_Inquiry_Transmit_Power_Level_Cmd_t;

// HCI_Write_Inquiry_Transmit_Power_Level_Ret_t only has one member: Status.

// 7.3.79 Write LE Host Support command

typedef struct HCI_Write_LE_Host_Support_Cmd_t {
  uint8_t                  LE_Supported_Host;
  uint8_t                  Unused;
} HCI_Write_LE_Host_Support_Cmd_t;

// HCI_Write_LE_Host_Support_Ret_t only has one member: Status.

// 7.2.10 Write Link Policy Settings command

typedef struct HCI_Write_Link_Policy_Settings_Cmd_t {
  uint16_t                 Connection_Handle;
  uint16_t                 Link_Policy_Settings;
} HCI_Write_Link_Policy_Settings_Cmd_t;

// HCI_Write_Link_Policy_Settings_Ret_t only has one member: Status_Conn.

// 7.3.42 Write Link Supervision Timeout command

typedef struct HCI_Write_Link_Supervision_Timeout_Cmd_t {
  uint16_t                 Connection_Handle;
  uint16_t                 Link_Supervision_Timeout;
} HCI_Write_Link_Supervision_Timeout_Cmd_t;

typedef struct HCI_Write_Link_Supervision_Timeout_Ret_t {
  uint8_t                  Status;
  uint16_t                 Handle;
} HCI_Write_Link_Supervision_Timeout_Ret_t;

// 7.3.11 Write Local Name command

typedef struct HCI_Write_Local_Name_Cmd_t {
  uint8_t                  Local_Name[248];
} HCI_Write_Local_Name_Cmd_t;

// HCI_Write_Local_Name_Ret_t only has one member: Status.

// 7.6.2 Write Loopback Mode command

typedef struct HCI_Write_Loopback_Mode_Cmd_t {
  uint8_t                  Loopback_Mode;
} HCI_Write_Loopback_Mode_Cmd_t;

// HCI_Write_Loopback_Mode_Ret_t only has one member: Status.

// 7.3.32 Write Num Broadcast Retransmissions command

typedef struct HCI_Write_Num_Broadcast_Retransmissions_Cmd_t {
  uint8_t                  Num_Broadcast_Retransmissions;
} HCI_Write_Num_Broadcast_Retransmissions_Cmd_t;

// HCI_Write_Num_Broadcast_Retransmissions_Ret_t only has one member: Status.

// 7.3.20 Write Page Scan Activity command

typedef struct HCI_Write_Page_Scan_Activity_Cmd_t {
  uint16_t                 Page_Scan_Interval;
  uint16_t                 Page_Scan_Window;
} HCI_Write_Page_Scan_Activity_Cmd_t;

// HCI_Write_Page_Scan_Activity_Ret_t only has one member: Status.

// 7.3.52 Write Page Scan Type command

typedef struct HCI_Write_Page_Scan_Type_Cmd_t {
  uint8_t                  Page_Scan_Type;
} HCI_Write_Page_Scan_Type_Cmd_t;

// HCI_Write_Page_Scan_Type_Ret_t only has one member: Status.

// 7.3.16 Write Page Timeout command

typedef struct HCI_Write_Page_Timeout_Cmd_t {
  uint16_t                 Page_Timeout;
} HCI_Write_Page_Timeout_Cmd_t;

// HCI_Write_Page_Timeout_Ret_t only has one member: Status.

// 7.3.6 Write PIN Type command

typedef struct HCI_Write_PIN_Type_Cmd_t {
  uint8_t                  PIN_Type;
} HCI_Write_PIN_Type_Cmd_t;

// HCI_Write_PIN_Type_Ret_t only has one member: Status.

// 7.3.18 Write Scan Enable command

typedef struct HCI_Write_Scan_Enable_Cmd_t {
  uint8_t                  Scan_Enable;
} HCI_Write_Scan_Enable_Cmd_t;

// HCI_Write_Scan_Enable_Ret_t only has one member: Status.

// 7.3.92 Write Secure Connections Host Support command

typedef struct HCI_Write_Secure_Connections_Host_Support_Cmd_t {
  uint8_t                  Secure_Connections_Host_Support;
} HCI_Write_Secure_Connections_Host_Support_Cmd_t;

// HCI_Write_Secure_Connections_Host_Support_Ret_t only has one member: Status.

// 7.6.8 Write Secure Connections Test Mode command

typedef struct HCI_Write_Secure_Connections_Test_Mode_Cmd_t {
  uint16_t                 Connection_Handle;
  uint8_t                  DM1_ACL_U_Mode;
  uint8_t                  eSCO_Loopback_Mode;
} HCI_Write_Secure_Connections_Test_Mode_Cmd_t;

// HCI_Write_Secure_Connections_Test_Mode_Ret_t only has one member: Status_Conn.

// 7.6.4 Write Simple Pairing Debug Mode command

typedef struct HCI_Write_Simple_Pairing_Debug_Mode_Cmd_t {
  uint8_t                  Simple_Pairing_Debug_Mode;
} HCI_Write_Simple_Pairing_Debug_Mode_Cmd_t;

// HCI_Write_Simple_Pairing_Debug_Mode_Ret_t only has one member: Status.

// 7.3.59 Write Simple Pairing Mode command

typedef struct HCI_Write_Simple_Pairing_Mode_Cmd_t {
  uint8_t                  Simple_Pairing_Mode;
} HCI_Write_Simple_Pairing_Mode_Cmd_t;

// HCI_Write_Simple_Pairing_Mode_Ret_t only has one member: Status.

// 7.3.9 Write Stored Link Key command

typedef struct HCI_Write_Stored_Link_Key_Cmd_t {
  uint8_t                  Num_Keys_To_Write;
  struct {
    BD_Addr_t              BD_Addr;
    uint8_t                Link_Key[16];
  } Keys_To_Write[0];
} HCI_Write_Stored_Link_Key_Cmd_t;

typedef struct HCI_Write_Stored_Link_Key_Ret_t {
  uint8_t                  Status;
  uint8_t                  Num_Keys_Written;
} HCI_Write_Stored_Link_Key_Ret_t;

// 7.3.90 Write Synchronization Train Parameters command

typedef struct HCI_Write_Synchronization_Train_Parameters_Cmd_t {
  uint16_t                 Interval_Min;
  uint16_t                 Interval_Max;
  uint32_t                 Sync_Train_Timeout;
  uint8_t                  Service_Data;
} HCI_Write_Synchronization_Train_Parameters_Cmd_t;

typedef struct HCI_Write_Synchronization_Train_Parameters_Ret_t {
  uint8_t                  Status;
  uint16_t                 Sync_Train_Interval;
} HCI_Write_Synchronization_Train_Parameters_Ret_t;

// 7.3.37 Write Synchronous Flow Control Enable command

typedef struct HCI_Write_Sync_Flow_Control_Enable_Cmd_t {
  uint8_t                  Synchronous_Flow_Control_Enable;
} HCI_Write_Sync_Flow_Control_Enable_Cmd_t;

// HCI_Write_Sync_Flow_Control_Enable_Ret_t only has one member: Status.

// 7.3.28 Write Voice Setting command

typedef struct HCI_Write_Voice_Setting_Cmd_t {
  uint16_t                 Voice_Setting;
} HCI_Write_Voice_Setting_Cmd_t;

// HCI_Write_Voice_Setting_Ret_t only has one member: Status.

