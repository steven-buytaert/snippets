// Copyright 2024 Steven Buytaert

// File is generated from hci-spec2src-5.4.h

#ifndef HCI_TYPES_H
#define HCI_TYPES_H

#include <stdint.h>

#define __packed__

#define COMMON_CLUSTERED // Common structures are clustered into separate types.

typedef enum HCI_Pkt_Type_t {
   type_CMD                   = 0x01,
   type_ACL                   = 0x02,
   type_SYN                   = 0x03,
   type_EVT                   = 0x04,
   type_ISO                   = 0x05,
} HCI_Pkt_Type_t;

typedef struct BD_Addr_t {
  uint8_t                     u08[6];
} BD_Addr_t;

typedef struct HCI_Ident_t {
  uint8_t                     type;            // Address type
  BD_Addr_t                   Addr;
} HCI_Ident_t;

typedef union HCI_Opcode_t {
  uint16_t                    opcode;
  struct {
    uint16_t                  OCF    : 10;
    uint16_t                  OGF    :  6;
  };
} HCI_Opcode_t;

typedef struct HCI_Cmd_t {
  uint8_t                     type;            // type_CMD.
  union {
    uint16_t                  opcode;
    struct {
      uint16_t                OCF    : 10;
      uint16_t                OGF    :  6;
    };
  };
  uint8_t                     length;
} HCI_Cmd_t;

typedef struct HCI_ACL_t {
  uint8_t                     type;            // type_ACL.
  struct {
    uint16_t                  handle : 12;
    uint16_t                  PB     :  2;
    uint16_t                  BC     :  2;
  };
  uint16_t                    length;
} __packed__ HCI_ACL_t;

typedef struct HCI_SYN_t {
  uint8_t                     type;            // type_SYN.
  struct {
    uint16_t                  handle : 12;
    uint16_t                  status :  2;
    uint16_t                         :  2;
  };
  uint16_t                    length;
} __packed__ HCI_SYN_t;

typedef struct HCI_Evt_t {
  uint8_t                     type;            // type_EVT.
  uint8_t                     code;
  uint8_t                     length;
  uint8_t                     subevent[0];     // Only relevant when 0x3e == code.
} HCI_Evt_t;

typedef struct HCI_ISO_t {
  uint8_t                     type;            // type_ISO.
  struct {
    uint16_t                  handle : 12;
    uint16_t                  PB     :  2;
    uint16_t                         :  1;
  };
  struct {
    uint16_t                  length : 14;
    uint16_t                         :  2;
  };
} __packed__ HCI_ISO_t;

typedef struct Status_BD_Addr_t {
  uint8_t                     Status;
  BD_Addr_t                   BD_Addr;
} Status_BD_Addr_t;

typedef struct Status_Conn_Handle_t {
  uint8_t                     Status;
  uint16_t                    Conn_Handle;
} Status_Conn_Handle_t;

// <no spec> { Evt / Status / Connection Handle } passe partout.

typedef struct Evt_Status_Conn_t {
  HCI_Evt_t                   Evt;
  Status_Conn_Handle_t        SC;
} Evt_Status_Conn_t;

// <no spec> { Cmd / Connection Handle } passe partout.

typedef struct Cmd_Conn_Handle_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Conn_Handle;
} Cmd_Conn_Handle_t;

// <no spec> { Evt / Connection Handle } passe partout.

typedef struct Evt_Conn_Handle_t {
  HCI_Evt_t                   Evt;
  uint16_t                    Conn_Handle;
} Evt_Conn_Handle_t;

// <no spec> // { Evt / Status } passe partout.

typedef struct Evt_Status_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Status;
} Evt_Status_t;

// <no spec> // { Evt / Status / BD_Addr } passe partout.

typedef struct Evt_Status_Addr_t {
  HCI_Evt_t                   Evt;
  Status_BD_Addr_t            SA;
} Evt_Status_Addr_t;

// 7.7.14 Command Complete event

static const uint8_t Command_Complete_Evt = 0x0e;

typedef struct HCI_Command_Complete_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Num_HCI_Command_Packets;
  HCI_Opcode_t                Opcode;
} HCI_Command_Complete_Evt_t;

// <no spec> { CCEvt / Status / Address } passe partout.

typedef struct CC_Status_Addr_t {
  HCI_Command_Complete_Evt_t  CCEvt;
  Status_BD_Addr_t            SA;
} CC_Status_Addr_t;

// { CCEvt / Status } passe partout.

typedef struct CC_Status_t {
  HCI_Command_Complete_Evt_t  CCEvt;
  uint8_t                     Status;
} CC_Status_t;

// { CCEvt / Status / Conn_Handle } passe partout.

typedef struct CC_Status_Conn_t {
  HCI_Command_Complete_Evt_t  CCEvt;
  Status_Conn_Handle_t        SC;
} CC_Status_Conn_t;

// 7.1.8 Accept Connection Request command

static const uint16_t Accept_Connection_Request_Cmd = 0x0409; // OGF 0x1 OCF 0x009

typedef struct HCI_Accept_Connection_Request_Cmd_t {
  HCI_Cmd_t                   Cmd;
  BD_Addr_t                   Addr;
  uint8_t                     Role;
} HCI_Accept_Connection_Request_Cmd_t;

// 7.1.27 Accept Synchronous Connection Request command

static const uint16_t Accept_Sync_Conn_Request_Cmd = 0x0429; // OGF 0x1 OCF 0x029

typedef struct HCI_Accept_Sync_Conn_Request_Cmd_t {
  HCI_Cmd_t                   Cmd;
  BD_Addr_t                   Addr;
  uint32_t                    Transmit_Bandwidth;
  uint32_t                    Receive_Bandwidth;
  uint16_t                    Max_Latency;
  uint16_t                    Voice_Setting;
  uint8_t                     Retransmission_Effort;
  uint16_t                    Packet_Type;
} HCI_Accept_Sync_Conn_Request_Cmd_t;

// 7.7.75 Authenticated Payload Timeout Expired event

// HCI_Authenticated_Payload_Timeout_Expired_Evt_t only has one member: Connection_Handle.

static const uint8_t Authenticated_Payload_Timeout_Expired_Evt = 0x57;

// 7.7.6 Authentication Complete event

// HCI_Authentication_Complete_Evt_t only has one member: Status_Conn.

static const uint8_t Authentication_Complete_Evt = 0x06;

// 7.1.15 Authentication Requested command

// HCI_Authentication_Requested_Cmd_t only has one member: Connection_Handle.

static const uint16_t Authentication_Requested_Cmd = 0x0411; // OGF 0x1 OCF 0x011

// 7.1.17 Change Connection Link Key command

// HCI_Change_Connection_Link_Key_Cmd_t only has one member: Connection_Handle.

static const uint16_t Change_Connection_Link_Key_Cmd = 0x0415; // OGF 0x1 OCF 0x015

// 7.7.9 Change Connection Link Key Complete event

// HCI_Change_Connection_Link_Key_Complete_Evt_t only has one member: Status_Conn.

static const uint8_t Change_Connection_Link_Key_Complete_Evt = 0x09;

// 7.1.14 Change Connection Packet Type command

static const uint16_t Change_Connection_Packet_Type_Cmd = 0x040f; // OGF 0x1 OCF 0x00f

typedef struct HCI_Change_Connection_Packet_Type_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Conn_Handle;
  uint16_t                    Packet_Type;
} HCI_Change_Connection_Packet_Type_Cmd_t;

// 7.7.15 Command Status event

static const uint8_t Command_Status_Evt = 0x0f;

typedef struct HCI_Command_Status_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Status;
  uint8_t                     Num_HCI_Command_Packets;
  HCI_Opcode_t                Opcode;
} HCI_Command_Status_Evt_t;

// 7.3.101 Configure Data Path command

static const uint16_t Configure_Data_Path_Cmd = 0x0c83; // OGF 0x3 OCF 0x083

typedef struct HCI_Configure_Data_Path_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Data_Path_Direction;
  uint8_t                     Data_Path_ID;
  uint8_t                     Vendor_Specific_Config_Length;
  uint8_t                     Vendor_Specific_Config[0];
} HCI_Configure_Data_Path_Cmd_t;

// HCI_Configure_Data_Path_Ret_t only has one member: Status.

// 7.7.3 Connection Complete event

static const uint8_t Connection_Complete_Evt = 0x03;

typedef struct HCI_Connection_Complete_Evt_t {
  HCI_Evt_t                   Evt;
  Status_Conn_Handle_t        Status_Conn;
  BD_Addr_t                   Addr;
  uint8_t                     Link_Type;
  uint8_t                     Encryption_Enabled;
} HCI_Connection_Complete_Evt_t;

// 7.7.29 Connection Packet Type Changed event

static const uint8_t Connection_Packet_Type_Changed_Evt = 0x1d;

typedef struct HCI_Connection_Packet_Type_Changed_Evt_t {
  HCI_Evt_t                   Evt;
  Status_Conn_Handle_t        Status_Conn;
  uint16_t                    Packet_Type;
} HCI_Connection_Packet_Type_Changed_Evt_t;

// 7.7.4 Connection Request event

static const uint8_t Connection_Request_Evt = 0x04;

typedef struct HCI_Connection_Request_Evt_t {
  HCI_Evt_t                   Evt;
  BD_Addr_t                   Addr;
  uint8_t                     Class_Of_Device[3];
  uint8_t                     Link_Type;
} HCI_Connection_Request_Evt_t;

// 7.7.73 Connectionless Peripheral Broadcast Channel Map Change event

static const uint8_t Connectionless_Peripheral_Broadcast_Channel_Map_Change_Evt = 0x55;

typedef struct HCI_Connectionless_Peripheral_Broadcast_Channel_Map_Change_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Channel_Map[10];
} HCI_Connectionless_Peripheral_Broadcast_Channel_Map_Change_Evt_t;

// 7.7.69 Connectionless Peripheral Broadcast Receive event

static const uint8_t Connectionless_Peripheral_Broadcast_Receive_Evt = 0x51;

typedef struct HCI_Connectionless_Peripheral_Broadcast_Receive_Evt_t {
  HCI_Evt_t                   Evt;
  BD_Addr_t                   Addr;
  uint8_t                     LT_ADDR;
  uint32_t                    Clock;
  uint32_t                    Offset;
  uint8_t                     RX_Status;
  uint8_t                     Fragment;
  uint8_t                     Data_Length;
  uint8_t                     Data[0];
} HCI_Connectionless_Peripheral_Broadcast_Receive_Evt_t;

// 7.7.70 Connectionless Peripheral Broadcast Timeout event

static const uint8_t Connectionless_Peripheral_Broadcast_Timeout_Evt = 0x52;

typedef struct HCI_Connectionless_Peripheral_Broadcast_Timeout_Evt_t {
  HCI_Evt_t                   Evt;
  BD_Addr_t                   Addr;
  uint8_t                     LT_ADDR;
} HCI_Connectionless_Peripheral_Broadcast_Timeout_Evt_t;

// 7.1.7 Create Connection Cancel command

static const uint16_t Create_Connection_Cancel_Cmd = 0x0408; // OGF 0x1 OCF 0x008

typedef struct HCI_Create_Connection_Cancel_Cmd_t {
  HCI_Cmd_t                   Cmd;
  BD_Addr_t                   Addr;
} HCI_Create_Connection_Cancel_Cmd_t;

// HCI_Create_Connection_Cancel_Ret_t only has one member: Status_BD_Addr.

// 7.1.5 Create Connection command

static const uint16_t Create_Connection_Cmd = 0x0405; // OGF 0x1 OCF 0x005

typedef struct HCI_Create_Connection_Cmd_t {
  HCI_Cmd_t                   Cmd;
  BD_Addr_t                   Addr;
  uint16_t                    Packet_Type;
  uint8_t                     Page_Scan_Repetition_Mode;
  uint8_t                     Reserved;
  uint16_t                    Clock_Offset;
  uint8_t                     Allow_Role_Switch;
} HCI_Create_Connection_Cmd_t;

// 7.7.26 Data Buffer Overflow event

static const uint8_t Data_Buffer_Overflow_Evt = 0x1a;

typedef struct HCI_Data_Buffer_Overflow_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Link_Type;
} HCI_Data_Buffer_Overflow_Evt_t;

// 7.3.87 Delete Reserved LT_ADDR command

static const uint16_t Delete_Reserved_LT_ADDR_Cmd = 0x0c75; // OGF 0x3 OCF 0x075

typedef struct HCI_Delete_Reserved_LT_ADDR_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     LT_ADDR;
} HCI_Delete_Reserved_LT_ADDR_Cmd_t;

typedef struct HCI_Delete_Reserved_LT_ADDR_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     LT_ADDR;
} HCI_Delete_Reserved_LT_ADDR_Ret_t;

// 7.3.10 Delete Stored Link Key command

static const uint16_t Delete_Stored_Link_Key_Cmd = 0x0c12; // OGF 0x3 OCF 0x012

typedef struct HCI_Delete_Stored_Link_Key_Cmd_t {
  HCI_Cmd_t                   Cmd;
  BD_Addr_t                   Addr;
  uint8_t                     Delete_All;
} HCI_Delete_Stored_Link_Key_Cmd_t;

typedef struct HCI_Delete_Stored_Link_Key_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint16_t                    Num_Keys_Deleted;
} HCI_Delete_Stored_Link_Key_Ret_t;

// 7.1.6 Disconnect command

static const uint16_t Disconnect_Cmd = 0x0406; // OGF 0x1 OCF 0x006

typedef struct HCI_Disconnect_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Conn_Handle;
  uint8_t                     Reason;
} HCI_Disconnect_Cmd_t;

// 7.7.5 Disconnection Complete event

static const uint8_t Disconnection_Complete_Evt = 0x05;

typedef struct HCI_Disconnection_Complete_Evt_t {
  HCI_Evt_t                   Evt;
  Status_Conn_Handle_t        Status_Conn;
  uint8_t                     Reason;
} HCI_Disconnection_Complete_Evt_t;

// 7.6.3 Enable Device Under Test Mode command

// HCI_Enable_Device_Under_Test_Mode_Ret_t only has one member: Status.

static const uint16_t Enable_Device_Under_Test_Mode_Cmd = 0x1803; // OGF 0x6 OCF 0x003

// 7.7.8 Encryption Change event

static const uint8_t Encryption_Change_v2_Evt = 0x59;

typedef struct HCI_Encryption_Change_v2_Evt_t {
  HCI_Evt_t                   Evt;
  Status_Conn_Handle_t        Status_Conn;
  uint8_t                     Encryption_Enabled;
  uint8_t                     Encryption_Key_Size;
} HCI_Encryption_Change_v2_Evt_t;

static const uint8_t Encryption_Change_v1_Evt = 0x08;

typedef struct HCI_Encryption_Change_v1_Evt_t {
  HCI_Evt_t                   Evt;
  Status_Conn_Handle_t        Status_Conn;
  uint8_t                     Encryption_Enabled;
} HCI_Encryption_Change_v1_Evt_t;

// 7.7.39 Encryption Key Refresh Complete event

// HCI_Encryption_Key_Refresh_Complete_Evt_t only has one member: Status_Conn.

static const uint8_t Encryption_Key_Refresh_Complete_Evt = 0x30;

// 7.1.46 Enhanced Accept Synchronous Connection Request command

typedef struct HCI_Sync_Config_t {             // Common
  uint32_t                    Transmit_Bandwidth;
  uint32_t                    Receive_Bandwidth;
  uint8_t                     Transmit_Coding_Format[5];
  uint8_t                     Receive_Coding_Format[5];
  uint16_t                    Transmit_Codec_Frame_Size;
  uint16_t                    Receive_Codec_Frame_Size;
  uint32_t                    Input_Bandwidth;
  uint32_t                    Output_Bandwidth;
  uint8_t                     Input_Coding_Format[5];
  uint8_t                     Output_Coding_Format[5];
  uint16_t                    Input_Coded_Data_Size;
  uint16_t                    Output_Coded_Data_Size;
  uint8_t                     Input_PCM_Data_Format;
  uint8_t                     Output_PCM_Data_Format;
  uint8_t                     Input_PCM_Sample_Payload_MSB_Position;
  uint8_t                     Output_PCM_Sample_Payload_MSB_Position;
  uint8_t                     Input_Data_Path;
  uint8_t                     Output_Data_Path;
  uint8_t                     Input_Transport_Unit_Size;
  uint8_t                     Output_Transport_Unit_Size;
  uint16_t                    Max_Latency;
  uint16_t                    Packet_Type;
  uint8_t                     Retransmission_Effort;
} HCI_Sync_Config_t;

static const uint16_t Enhanced_Accept_Sync_Conn_Request_Cmd = 0x043e; // OGF 0x1 OCF 0x03e

typedef struct HCI_Enhanced_Accept_Sync_Conn_Request_Cmd_t {
  HCI_Cmd_t                   Cmd;
  BD_Addr_t                   Addr;
  HCI_Sync_Config_t           Sync_Config;
} HCI_Enhanced_Accept_Sync_Conn_Request_Cmd_t;

// 7.3.66 Enhanced Flush command

static const uint16_t Enhanced_Flush_Cmd = 0x0c5f; // OGF 0x3 OCF 0x05f

typedef struct HCI_Enhanced_Flush_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Handle;
  uint8_t                     Packet_Type;
} HCI_Enhanced_Flush_Cmd_t;

// 7.7.47 Enhanced Flush Complete event

static const uint8_t Enhanced_Flush_Complete_Evt = 0x39;

typedef struct HCI_Enhanced_Flush_Complete_Evt_t {
  HCI_Evt_t                   Evt;
  uint16_t                    Handle;
} HCI_Enhanced_Flush_Complete_Evt_t;

// 7.1.45 Enhanced Setup Synchronous Connection command

static const uint16_t Enhanced_Setup_Sync_Conn_Cmd = 0x043d; // OGF 0x1 OCF 0x03d

typedef struct HCI_Enhanced_Setup_Sync_Conn_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Conn_Handle;
  HCI_Sync_Config_t           Sync_Config;
} HCI_Enhanced_Setup_Sync_Conn_Cmd_t;

// 7.1.4 Exit Periodic Inquiry Mode command

// HCI_Exit_Periodic_Inquiry_Mode_Ret_t only has one member: Status.

static const uint16_t Exit_Periodic_Inquiry_Mode_Cmd = 0x0404; // OGF 0x1 OCF 0x004

// 7.2.3 Exit Sniff Mode command

// HCI_Exit_Sniff_Mode_Cmd_t only has one member: Connection_Handle.

static const uint16_t Exit_Sniff_Mode_Cmd = 0x0804; // OGF 0x2 OCF 0x004

// 7.7.38 Extended Inquiry Result event

static const uint8_t Extended_Inquiry_Result_Evt = 0x2f;

typedef struct HCI_Extended_Inquiry_Result_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Num_Responses;
  BD_Addr_t                   Addr;
  uint8_t                     Page_Scan_Repetition_Mode;
  uint8_t                     Reserved;
  uint8_t                     Class_Of_Device[3];
  uint16_t                    Clock_Offset;
  uint8_t                     RSSI;
  uint8_t                     Extended_Inquiry_Response[240];
} HCI_Extended_Inquiry_Result_Evt_t;

// 7.2.13 Flow Specification command

typedef struct HCI_Flow_Spec_t {               // Common
  uint8_t                     Flow_Direction;
  uint8_t                     Service_Type;
  uint32_t                    Token_Rate;
  uint32_t                    Token_Bucket_Size;
  uint32_t                    Peak_Bandwidth;
  uint32_t                    Access_Latency;
} HCI_Flow_Spec_t;

static const uint16_t Flow_Specification_Cmd = 0x0810; // OGF 0x2 OCF 0x010

typedef struct HCI_Flow_Specification_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Conn_Handle;
  uint8_t                     Unused;
  HCI_Flow_Spec_t             Flow_Spec;
} HCI_Flow_Specification_Cmd_t;

// 7.7.32 Flow Specification Complete event

static const uint8_t Flow_Specification_Complete_Evt = 0x21;

typedef struct HCI_Flow_Specification_Complete_Evt_t {
  HCI_Evt_t                   Evt;
  Status_Conn_Handle_t        Status_Conn;
  uint8_t                     Unused;
  HCI_Flow_Spec_t             Flow_Spec;
} HCI_Flow_Specification_Complete_Evt_t;

// 7.3.4 Flush command

// HCI_Flush_Cmd_t only has one member: Connection_Handle.

static const uint16_t Flush_Cmd = 0x0c08; // OGF 0x3 OCF 0x008

// HCI_Flush_Ret_t only has one member: Status_Conn.

// 7.7.17 Flush Occurred event

static const uint8_t Flush_Occurred_Evt = 0x11;

typedef struct HCI_Flush_Occurred_Evt_t {
  HCI_Evt_t                   Evt;
  uint16_t                    Handle;
} HCI_Flush_Occurred_Evt_t;

// 7.5.11 Get MWS Transport Layer Configuration command

static const uint16_t Get_MWS_Transport_Layer_Configuration_Cmd = 0x140c; // OGF 0x5 OCF 0x00c

typedef struct HCI_Get_MWS_Transport_Layer_Configuration_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     Num_Transports;
  struct {
    uint8_t                   Transport_Layer;
    uint8_t                   Num_Baud_Rates;
    struct {
      uint32_t                To_MWS_Baud_Rate;
      uint32_t                From_MWS_Baud_Rate;
    } * Baud_Rates;
  } Transports[0];
} HCI_Get_MWS_Transport_Layer_Configuration_Ret_t;

// 7.7.16 Hardware Error event

static const uint8_t Hardware_Error_Evt = 0x10;

typedef struct HCI_Hardware_Error_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Hardware_Code;
} HCI_Hardware_Error_Evt_t;

// 7.2.1 Hold Mode command

static const uint16_t Hold_Mode_Cmd = 0x0801; // OGF 0x2 OCF 0x001

typedef struct HCI_Hold_Mode_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Conn_Handle;
  uint16_t                    Hold_Mode_Max_Interval;
  uint16_t                    Hold_Mode_Min_Interval;
} HCI_Hold_Mode_Cmd_t;

// 7.3.39 Host Buffer Size command

static const uint16_t Host_Buffer_Size_Cmd = 0x0c33; // OGF 0x3 OCF 0x033

typedef struct HCI_Host_Buffer_Size_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Host_ACL_Data_Packet_Length;
  uint8_t                     Host_Synchronous_Data_Packet_Length;
  uint16_t                    Host_Total_Num_ACL_Data_Packets;
  uint16_t                    Host_Total_Num_Synchronous_Data_Packets;
} HCI_Host_Buffer_Size_Cmd_t;

// HCI_Host_Buffer_Size_Ret_t only has one member: Status.

// 7.3.40 Host Number Of Completed Packets command

static const uint16_t Host_Number_Of_Completed_Packets_Cmd = 0x0c35; // OGF 0x3 OCF 0x035

typedef struct HCI_Host_Number_Of_Completed_Packets_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Num_Handles;
  struct {
    uint16_t                  Conn_Handle;
    uint16_t                  Host_Num_Completed_Packets;
  } Handles[0];
} HCI_Host_Number_Of_Completed_Packets_Cmd_t;

// 7.1.2 Inquiry Cancel command

// HCI_Inquiry_Cancel_Ret_t only has one member: Status.

static const uint16_t Inquiry_Cancel_Cmd = 0x0402; // OGF 0x1 OCF 0x002

// 7.1.1 Inquiry command

static const uint16_t Inquiry_Cmd = 0x0401; // OGF 0x1 OCF 0x001

typedef struct HCI_Inquiry_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     LAP[3];
  uint8_t                     Inquiry_Length;
  uint8_t                     Num_Responses;
} HCI_Inquiry_Cmd_t;

// 7.7.1 Inquiry Complete event

// HCI_Inquiry_Complete_Evt_t only has one member: Status.

static const uint8_t Inquiry_Complete_Evt = 0x01;

// 7.7.74 Inquiry Response Notification event

static const uint8_t Inquiry_Response_Notification_Evt = 0x56;

typedef struct HCI_Inquiry_Response_Notification_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     LAP[3];
  uint8_t                     RSSI;
} HCI_Inquiry_Response_Notification_Evt_t;

// 7.7.2 Inquiry Result event

static const uint8_t Inquiry_Result_Evt = 0x02;

typedef struct HCI_Inquiry_Result_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Num_Responses;
  struct {
    BD_Addr_t                 Addr;
    uint8_t                   Page_Scan_Repetition_Mode;
    uint16_t                  Reserved;
    uint8_t                   Class_Of_Device[3];
    uint16_t                  Clock_Offset;
  } Responses[0];
} HCI_Inquiry_Result_Evt_t;

// 7.7.33 Inquiry Result with RSSI event

static const uint8_t Inquiry_Result_with_RSSI_Evt = 0x22;

typedef struct HCI_Inquiry_Result_with_RSSI_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Num_Responses;
  struct {
    BD_Addr_t                 Addr;
    uint8_t                   Page_Scan_Repetition_Mode;
    uint8_t                   Reserved;
    uint8_t                   Class_Of_Device[3];
    uint16_t                  Clock_Offset;
    uint8_t                   RSSI;
  } Responses[0];
} HCI_Inquiry_Result_with_RSSI_Evt_t;

// 7.7.40 IO Capability Request event

static const uint8_t IO_Capability_Request_Evt = 0x31;

typedef struct HCI_IO_Capability_Request_Evt_t {
  HCI_Evt_t                   Evt;
  BD_Addr_t                   Addr;
} HCI_IO_Capability_Request_Evt_t;

// 7.1.36 IO Capability Request Negative Reply command

static const uint16_t IO_Capability_Request_Negative_Reply_Cmd = 0x0434; // OGF 0x1 OCF 0x034

typedef struct HCI_IO_Capability_Request_Negative_Reply_Cmd_t {
  HCI_Cmd_t                   Cmd;
  BD_Addr_t                   Addr;
  uint8_t                     Reason;
} HCI_IO_Capability_Request_Negative_Reply_Cmd_t;

// HCI_IO_Capability_Request_Negative_Reply_Ret_t only has one member: Status_BD_Addr.

// 7.1.29 IO Capability Request Reply command

static const uint16_t IO_Capability_Request_Reply_Cmd = 0x042b; // OGF 0x1 OCF 0x02b

typedef struct HCI_IO_Capability_Request_Reply_Cmd_t {
  HCI_Cmd_t                   Cmd;
  BD_Addr_t                   Addr;
  uint8_t                     IO_Capability;
  uint8_t                     OOB_Data_Present;
  uint8_t                     Authentication_Requirements;
} HCI_IO_Capability_Request_Reply_Cmd_t;

// HCI_IO_Capability_Request_Reply_Ret_t only has one member: Status_BD_Addr.

// 7.7.41 IO Capability Response event

static const uint8_t IO_Capability_Response_Evt = 0x32;

typedef struct HCI_IO_Capability_Response_Evt_t {
  HCI_Evt_t                   Evt;
  BD_Addr_t                   Addr;
  uint8_t                     IO_Capability;
  uint8_t                     OOB_Data_Present;
  uint8_t                     Authentication_Requirements;
} HCI_IO_Capability_Response_Evt_t;

// 7.7.49 Keypress Notification event

static const uint8_t Keypress_Notification_Evt = 0x3c;

typedef struct HCI_Keypress_Notification_Evt_t {
  HCI_Evt_t                   Evt;
  BD_Addr_t                   Addr;
  uint8_t                     Notification_Type;
} HCI_Keypress_Notification_Evt_t;

// 7.8.101 LE Accept CIS Request command

// HCI_LE_Accept_CIS_Request_Cmd_t only has one member: Connection_Handle.

static const uint16_t LE_Accept_CIS_Request_Cmd = 0x2066; // OGF 0x8 OCF 0x066

// 7.8.16 LE Add Device To Filter Accept List command

static const uint16_t LE_Add_Device_To_Filter_Accept_List_Cmd = 0x2011; // OGF 0x8 OCF 0x011

typedef struct HCI_LE_Add_Device_To_Filter_Accept_List_Cmd_t {
  HCI_Cmd_t                   Cmd;
  HCI_Ident_t                 Ident;
} HCI_LE_Add_Device_To_Filter_Accept_List_Cmd_t;

// HCI_LE_Add_Device_To_Filter_Accept_List_Ret_t only has one member: Status.

// 7.8.70 LE Add Device To Periodic Advertiser List command

static const uint16_t LE_Add_Device_To_PAdvertiser_List_Cmd = 0x2047; // OGF 0x8 OCF 0x047

typedef struct HCI_LE_Add_Device_To_PAdvertiser_List_Cmd_t {
  HCI_Cmd_t                   Cmd;
  HCI_Ident_t                 Advertiser;
  uint8_t                     Advertising_SID;
} HCI_LE_Add_Device_To_PAdvertiser_List_Cmd_t;

// HCI_LE_Add_Device_To_PAdvertiser_List_Ret_t only has one member: Status.

// 7.8.38 LE Add Device To Resolving List command

static const uint16_t LE_Add_Device_To_Resolving_List_Cmd = 0x2027; // OGF 0x8 OCF 0x027

typedef struct HCI_LE_Add_Device_To_Resolving_List_Cmd_t {
  HCI_Cmd_t                   Cmd;
  HCI_Ident_t                 Peer_Identity;
  uint8_t                     Peer_IRK[16];
  uint8_t                     Local_IRK[16];
} HCI_LE_Add_Device_To_Resolving_List_Cmd_t;

// HCI_LE_Add_Device_To_Resolving_List_Ret_t only has one member: Status.

// 7.7.65.2 LE Advertising Report event

static const uint8_t LE_Advertising_Report_Evt_sub = 0x02;

typedef struct HCI_LE_Advertising_Report_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Subevent_Code;
  uint8_t                     Num_Reports;
  struct {
    uint8_t                   Event_Type;
    HCI_Ident_t               Ident;
    uint8_t                   Data_Length;
    uint8_t  *                Data;
    uint8_t                   RSSI;
  } Reports[0];
} HCI_LE_Advertising_Report_Evt_t;

// 7.7.65.18 LE Advertising Set Terminated event

static const uint8_t LE_Advertising_Set_Terminated_Evt_sub = 0x12;

typedef struct HCI_LE_Advertising_Set_Terminated_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Subevent_Code;
  uint8_t                     Status;
  uint8_t                     Advertising_Handle;
  uint16_t                    Conn_Handle;
  uint8_t                     Num_Completed_Extended_Advertising_Events;
} HCI_LE_Advertising_Set_Terminated_Evt_t;

// 7.8.106 LE BIG Create Sync command

static const uint16_t LE_BIG_Create_Sync_Cmd = 0x206b; // OGF 0x8 OCF 0x06b

typedef struct HCI_LE_BIG_Create_Sync_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     BIG_Handle;
  uint16_t                    Sync_Handle;
  uint8_t                     Encryption;
  uint8_t                     Broadcast_Code[16];
  uint8_t                     MSE;
  uint16_t                    BIG_Sync_Timeout;
  uint8_t                     Num_BIS;
  uint8_t                     BIS[0];
} HCI_LE_BIG_Create_Sync_Cmd_t;

// 7.7.65.29 LE BIG Sync Established event

static const uint8_t LE_BIG_Sync_Established_Evt_sub = 0x1d;

typedef struct HCI_LE_BIG_Sync_Established_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Subevent_Code;
  uint8_t                     Status;
  uint8_t                     BIG_Handle;
  uint8_t                     Transport_Latency_BIG[3];
  uint8_t                     NSE;
  uint8_t                     BN;
  uint8_t                     PTO;
  uint8_t                     IRC;
  uint16_t                    Max_PDU;
  uint16_t                    ISO_Interval;
  uint8_t                     Num_BIS;
  uint16_t                    Conn_Handle[0];
} HCI_LE_BIG_Sync_Established_Evt_t;

// 7.7.65.30 LE BIG Sync Lost event

static const uint8_t LE_BIG_Sync_Lost_Evt_sub = 0x1e;

typedef struct HCI_LE_BIG_Sync_Lost_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Subevent_Code;
  uint8_t                     BIG_Handle;
  uint8_t                     Reason;
} HCI_LE_BIG_Sync_Lost_Evt_t;

// 7.8.107 LE BIG Terminate Sync command

static const uint16_t LE_BIG_Terminate_Sync_Cmd = 0x206c; // OGF 0x8 OCF 0x06c

typedef struct HCI_LE_BIG_Terminate_Sync_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     BIG_Handle;
} HCI_LE_BIG_Terminate_Sync_Cmd_t;

typedef struct HCI_LE_BIG_Terminate_Sync_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     BIG_Handle;
} HCI_LE_BIG_Terminate_Sync_Ret_t;

// 7.7.65.34 LE BIGInfo Advertising Report event

static const uint8_t LE_BIGInfo_Advertising_Report_Evt_sub = 0x22;

typedef struct HCI_LE_BIGInfo_Advertising_Report_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Subevent_Code;
  uint16_t                    Sync_Handle;
  uint8_t                     Num_BIS;
  uint8_t                     NSE;
  uint16_t                    ISO_Interval;
  uint8_t                     BN;
  uint8_t                     PTO;
  uint8_t                     IRC;
  uint16_t                    Max_PDU;
  uint8_t                     SDU_Interval[3];
  uint16_t                    Max_SDU;
  uint8_t                     PHY;
  uint8_t                     Framing;
  uint8_t                     Encryption;
} HCI_LE_BIGInfo_Advertising_Report_Evt_t;

// 7.7.65.20 LE Channel Selection Algorithm event

static const uint8_t LE_Channel_Selection_Algorithm_Evt_sub = 0x14;

typedef struct HCI_LE_Channel_Selection_Algorithm_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Subevent_Code;
  uint16_t                    Conn_Handle;
  uint8_t                     Channel_Selection_Algorithm;
} HCI_LE_Channel_Selection_Algorithm_Evt_t;

// 7.7.65.25 LE CIS Established event

static const uint8_t LE_CIS_Established_Evt_sub = 0x19;

typedef struct HCI_LE_CIS_Established_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Subevent_Code;
  uint8_t                     Status;
  uint16_t                    Conn_Handle;
  uint8_t                     CIG_Sync_Delay[3];
  uint8_t                     CIS_Sync_Delay[3];
  uint8_t                     Transport_Latency_C_To_P[3];
  uint8_t                     Transport_Latency_P_To_C[3];
  uint8_t                     PHY_C_To_P;
  uint8_t                     PHY_P_To_C;
  uint8_t                     NSE;
  uint8_t                     BN_C_To_P;
  uint8_t                     BN_P_To_C;
  uint8_t                     FT_C_To_P;
  uint8_t                     FT_P_To_C;
  uint16_t                    Max_PDU_C_To_P;
  uint16_t                    Max_PDU_P_To_C;
  uint16_t                    ISO_Interval;
} HCI_LE_CIS_Established_Evt_t;

// 7.7.65.26 LE CIS Request event

static const uint8_t LE_CIS_Request_Evt_sub = 0x1a;

typedef struct HCI_LE_CIS_Request_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Subevent_Code;
  uint16_t                    ACL_Conn_Handle;
  uint16_t                    CIS_Conn_Handle;
  uint8_t                     CIG_ID;
  uint8_t                     CIS_ID;
} HCI_LE_CIS_Request_Evt_t;

// 7.8.60 LE Clear Advertising Sets command

// HCI_LE_Clear_Advertising_Sets_Ret_t only has one member: Status.

static const uint16_t LE_Clear_Advertising_Sets_Cmd = 0x203d; // OGF 0x8 OCF 0x03d

// 7.8.15 LE Clear Filter Accept List command

// HCI_LE_Clear_Filter_Accept_List_Ret_t only has one member: Status.

static const uint16_t LE_Clear_Filter_Accept_List_Cmd = 0x2010; // OGF 0x8 OCF 0x010

// 7.8.72 LE Clear Periodic Advertiser List command

// HCI_LE_Clear_PAdvertiser_List_Ret_t only has one member: Status.

static const uint16_t LE_Clear_PAdvertiser_List_Cmd = 0x2049; // OGF 0x8 OCF 0x049

// 7.8.40 LE Clear Resolving List command

// HCI_LE_Clear_Resolving_List_Ret_t only has one member: Status.

static const uint16_t LE_Clear_Resolving_List_Cmd = 0x2029; // OGF 0x8 OCF 0x029

// 7.7.65.1 LE Connection Complete event

static const uint8_t LE_Connection_Complete_Evt_sub = 0x01;

typedef struct HCI_LE_Connection_Complete_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Subevent_Code;
  uint8_t                     Status;
  uint16_t                    Conn_Handle;
  uint8_t                     Role;
  HCI_Ident_t                 Peer;
  uint16_t                    Connection_Interval;
  uint16_t                    Peripheral_Latency;
  uint16_t                    Supervision_Timeout;
  uint8_t                     Central_Clock_Accuracy;
} HCI_LE_Connection_Complete_Evt_t;

// 7.8.85 LE Connection CTE Request Enable command

static const uint16_t LE_Connection_CTE_Request_Enable_Cmd = 0x2056; // OGF 0x8 OCF 0x056

typedef struct HCI_LE_Connection_CTE_Request_Enable_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Conn_Handle;
  uint8_t                     Enable;
  uint16_t                    CTE_Request_Interval;
  uint8_t                     Requested_CTE_Length;
  uint8_t                     Requested_CTE_Type;
} HCI_LE_Connection_CTE_Request_Enable_Cmd_t;

// HCI_LE_Connection_CTE_Request_Enable_Ret_t only has one member: Status_Conn.

// 7.8.86 LE Connection CTE Response Enable command

static const uint16_t LE_Connection_CTE_Response_Enable_Cmd = 0x2057; // OGF 0x8 OCF 0x057

typedef struct HCI_LE_Connection_CTE_Response_Enable_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Conn_Handle;
  uint8_t                     Enable;
} HCI_LE_Connection_CTE_Response_Enable_Cmd_t;

// HCI_LE_Connection_CTE_Response_Enable_Ret_t only has one member: Status_Conn.

// 7.7.65.22 LE Connection IQ Report event

static const uint8_t LE_Connection_IQ_Report_Evt_sub = 0x16;

typedef struct HCI_LE_Connection_IQ_Report_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Subevent_Code;
  uint16_t                    Conn_Handle;
  uint8_t                     RX_PHY;
  uint8_t                     Data_Channel_Index;
  uint16_t                    RSSI;
  uint8_t                     RSSI_Antenna_ID;
  uint8_t                     CTE_Type;
  uint8_t                     Slot_Durations;
  uint8_t                     Packet_Status;
  uint16_t                    Connection_Event_Counter;
  uint8_t                     Sample_Count;
  struct {
    uint8_t                   I_Sample;
    uint8_t                   Q_Sample;
  } Sample[0];
} HCI_LE_Connection_IQ_Report_Evt_t;

// 7.8.18 LE Connection Update command

static const uint16_t LE_Connection_Update_Cmd = 0x2013; // OGF 0x8 OCF 0x013

typedef struct HCI_LE_Connection_Update_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Conn_Handle;
  uint16_t                    Connection_Interval_Min;
  uint16_t                    Connection_Interval_Max;
  uint16_t                    Max_Latency;
  uint16_t                    Supervision_Timeout;
  uint16_t                    Min_CE_Length;
  uint16_t                    Max_CE_Length;
} HCI_LE_Connection_Update_Cmd_t;

// 7.7.65.3 LE Connection Update Complete event

static const uint8_t LE_Connection_Update_Complete_Evt_sub = 0x03;

typedef struct HCI_LE_Connection_Update_Complete_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Subevent_Code;
  uint8_t                     Status;
  uint16_t                    Conn_Handle;
  uint16_t                    Connection_Interval;
  uint16_t                    Peripheral_Latency;
  uint16_t                    Supervision_Timeout;
} HCI_LE_Connection_Update_Complete_Evt_t;

// 7.7.65.21 LE Connectionless IQ Report event

static const uint8_t LE_Connectionless_IQ_Report_Evt_sub = 0x15;

typedef struct HCI_LE_Connectionless_IQ_Report_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Subevent_Code;
  uint16_t                    Sync_Handle;
  uint8_t                     Channel_Index;
  uint16_t                    RSSI;
  uint8_t                     RSSI_Antenna_ID;
  uint8_t                     CTE_Type;
  uint8_t                     Slot_Durations;
  uint8_t                     Packet_Status;
  uint16_t                    Periodic_Event_Counter;
  uint8_t                     Sample_Count;
  struct {
    uint8_t                   I_Sample;
    uint8_t                   Q_Sample;
  } Sample[0];
} HCI_LE_Connectionless_IQ_Report_Evt_t;

// 7.8.103 LE Create BIG command

static const uint16_t LE_Create_BIG_Cmd = 0x2068; // OGF 0x8 OCF 0x068

typedef struct HCI_LE_Create_BIG_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     BIG_Handle;
  uint8_t                     Advertising_Handle;
  uint8_t                     Num_BIS;
  uint8_t                     SDU_Interval[3];
  uint16_t                    Max_SDU;
  uint16_t                    Max_Transport_Latency;
  uint8_t                     RTN;
  uint8_t                     PHY;
  uint8_t                     Packing;
  uint8_t                     Framing;
  uint8_t                     Encryption;
  uint8_t                     Broadcast_Code[16];
} HCI_LE_Create_BIG_Cmd_t;

// 7.7.65.27 LE Create BIG Complete event

static const uint8_t LE_Create_BIG_Complete_Evt_sub = 0x1b;

typedef struct HCI_LE_Create_BIG_Complete_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Subevent_Code;
  uint8_t                     Status;
  uint8_t                     BIG_Handle;
  uint8_t                     BIG_Sync_Delay[3];
  uint8_t                     Transport_Latency_BIG[3];
  uint8_t                     PHY;
  uint8_t                     NSE;
  uint8_t                     BN;
  uint8_t                     PTO;
  uint8_t                     IRC;
  uint16_t                    Max_PDU;
  uint16_t                    ISO_Interval;
  uint8_t                     Num_BIS;
  uint16_t                    Conn_Handle[0];
} HCI_LE_Create_BIG_Complete_Evt_t;

// 7.8.104 LE Create BIG Test command

static const uint16_t LE_Create_BIG_Test_Cmd = 0x2069; // OGF 0x8 OCF 0x069

typedef struct HCI_LE_Create_BIG_Test_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     BIG_Handle;
  uint8_t                     Advertising_Handle;
  uint8_t                     Num_BIS;
  uint8_t                     SDU_Interval[3];
  uint16_t                    ISO_Interval;
  uint8_t                     NSE;
  uint16_t                    Max_SDU;
  uint16_t                    Max_PDU;
  uint8_t                     PHY;
  uint8_t                     Packing;
  uint8_t                     Framing;
  uint8_t                     BN;
  uint8_t                     IRC;
  uint8_t                     PTO;
  uint8_t                     Encryption;
  uint8_t                     Broadcast_Code[16];
} HCI_LE_Create_BIG_Test_Cmd_t;

// 7.8.99 LE Create CIS command

static const uint16_t LE_Create_CIS_Cmd = 0x2064; // OGF 0x8 OCF 0x064

typedef struct HCI_LE_Create_CIS_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     CIS_Count;
  struct {
    uint16_t                  CIS_Conn_Handle;
    uint16_t                  ACL_Conn_Handle;
  } CIS[0];
} HCI_LE_Create_CIS_Cmd_t;

// 7.8.13 LE Create Connection Cancel command

// HCI_LE_Create_Connection_Cancel_Ret_t only has one member: Status.

static const uint16_t LE_Create_Connection_Cancel_Cmd = 0x200e; // OGF 0x8 OCF 0x00e

// 7.8.12 LE Create Connection command

static const uint16_t LE_Create_Connection_Cmd = 0x200d; // OGF 0x8 OCF 0x00d

typedef struct HCI_LE_Create_Connection_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    LE_Scan_Interval;
  uint16_t                    LE_Scan_Window;
  uint8_t                     Initiator_Filter_Policy;
  HCI_Ident_t                 Peer;
  uint8_t                     Own_Addr_Type;
  uint16_t                    Connection_Interval_Min;
  uint16_t                    Connection_Interval_Max;
  uint16_t                    Max_Latency;
  uint16_t                    Supervision_Timeout;
  uint16_t                    Min_CE_Length;
  uint16_t                    Max_CE_Length;
} HCI_LE_Create_Connection_Cmd_t;

// 7.7.65.23 LE CTE Request Failed event

static const uint8_t LE_CTE_Request_Failed_Evt_sub = 0x17;

typedef struct HCI_LE_CTE_Request_Failed_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Subevent_Code;
  uint8_t                     Status;
  uint16_t                    Conn_Handle;
} HCI_LE_CTE_Request_Failed_Evt_t;

// 7.7.65.7 LE Data Length Change event

static const uint8_t LE_Data_Length_Change_Evt_sub = 0x07;

typedef struct HCI_LE_Data_Length_Change_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Subevent_Code;
  uint16_t                    Conn_Handle;
  uint16_t                    Max_TX_Octets;
  uint16_t                    Max_TX_Time;
  uint16_t                    Max_RX_Octets;
  uint16_t                    Max_RX_Time;
} HCI_LE_Data_Length_Change_Evt_t;

// 7.7.65.11 LE Directed Advertising Report event

static const uint8_t LE_Directed_Advertising_Report_Evt_sub = 0x0b;

typedef struct HCI_LE_Directed_Advertising_Report_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Subevent_Code;
  uint8_t                     Num_Reports;
  struct {
    uint8_t                   Event_Type;
    HCI_Ident_t               Ident;
    HCI_Ident_t               Direct;
    uint8_t                   RSSI;
  } Reports[0];
} HCI_LE_Directed_Advertising_Report_Evt_t;

// 7.8.24 LE Enable Encryption command

static const uint16_t LE_Enable_Encryption_Cmd = 0x2019; // OGF 0x8 OCF 0x019

typedef struct HCI_LE_Enable_Encryption_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Conn_Handle;
  uint8_t                     Random_Number[8];
  uint16_t                    Encrypted_Diversifier;
  uint8_t                     Long_Term_Key[16];
} HCI_LE_Enable_Encryption_Cmd_t;

// 7.8.22 LE Encrypt command

static const uint16_t LE_Encrypt_Cmd = 0x2017; // OGF 0x8 OCF 0x017

typedef struct HCI_LE_Encrypt_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Key[16];
  uint8_t                     Plaintext_Data[16];
} HCI_LE_Encrypt_Cmd_t;

typedef struct HCI_LE_Encrypt_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     Encrypted_Data[16];
} HCI_LE_Encrypt_Ret_t;

// 7.7.65.10 LE Enhanced Connection Complete event

static const uint8_t LE_Enhanced_Connection_Complete_v2_Evt_sub = 0x29;

typedef struct HCI_LE_Enhanced_Connection_Complete_v2_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Subevent_Code;
  uint8_t                     Status;
  uint16_t                    Conn_Handle;
  uint8_t                     Role;
  HCI_Ident_t                 Peer;
  BD_Addr_t                   Local_Resolvable_Private_Addr;
  BD_Addr_t                   Peer_Resolvable_Private_Addr;
  uint16_t                    Connection_Interval;
  uint16_t                    Peripheral_Latency;
  uint16_t                    Supervision_Timeout;
  uint8_t                     Central_Clock_Accuracy;
  uint8_t                     Advertising_Handle;
  uint16_t                    Sync_Handle;
} HCI_LE_Enhanced_Connection_Complete_v2_Evt_t;

static const uint8_t LE_Enhanced_Connection_Complete_v1_Evt_sub = 0x0a;

typedef struct HCI_LE_Enhanced_Connection_Complete_v1_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Subevent_Code;
  uint8_t                     Status;
  uint16_t                    Conn_Handle;
  uint8_t                     Role;
  HCI_Ident_t                 Peer;
  BD_Addr_t                   Local_Resolvable_Private_Addr;
  BD_Addr_t                   Peer_Resolvable_Private_Addr;
  uint16_t                    Connection_Interval;
  uint16_t                    Peripheral_Latency;
  uint16_t                    Supervision_Timeout;
  uint8_t                     Central_Clock_Accuracy;
} HCI_LE_Enhanced_Connection_Complete_v1_Evt_t;

// 7.8.117 LE Enhanced Read Transmit Power Level command

static const uint16_t LE_Enhanced_Read_Transmit_Power_Level_Cmd = 0x2076; // OGF 0x8 OCF 0x076

typedef struct HCI_LE_Enhanced_Read_Transmit_Power_Level_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Conn_Handle;
  uint8_t                     PHY;
} HCI_LE_Enhanced_Read_Transmit_Power_Level_Cmd_t;

typedef struct HCI_LE_Enhanced_Read_Transmit_Power_Level_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  Status_Conn_Handle_t        Status_Conn;
  uint8_t                     PHY;
  uint8_t                     Current_TX_Power_Level;
  uint8_t                     Max_TX_Power_Level;
} HCI_LE_Enhanced_Read_Transmit_Power_Level_Ret_t;

// 7.7.65.13 LE Extended Advertising Report event

static const uint8_t LE_Extended_Advertising_Report_Evt_sub = 0x0d;

typedef struct HCI_LE_Extended_Advertising_Report_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Subevent_Code;
  uint8_t                     Num_Reports;
  struct {
    uint16_t                  Event_Type;
    HCI_Ident_t               Ident;
    uint8_t                   Primary_PHY;
    uint8_t                   Secondary_PHY;
    uint8_t                   Advertising_SID;
    uint8_t                   TX_Power;
    uint8_t                   RSSI;
    uint16_t                  PA_Interval;
    HCI_Ident_t               Direct;
    uint8_t                   Data_Length;
    uint8_t  *                Data;
  } Reports[0];
} HCI_LE_Extended_Advertising_Report_Evt_t;

// 7.8.66 LE Extended Create Connection command

typedef struct HCI_Conn_Cfg_t {                // Common
  uint8_t                     Scan_Interval;
  uint8_t                     Scan_Window;
  uint8_t                     Connection_Interval_Min;
  uint8_t                     Connection_Interval_Max;
  uint8_t                     Max_Latency;
  uint8_t                     Supervision_Timeout;
  uint8_t                     Min_CE_Length;
  uint8_t                     Max_CE_Length;
} HCI_Conn_Cfg_t;

static const uint16_t LE_Extended_Create_Connection_v2_Cmd = 0x2085; // OGF 0x8 OCF 0x085

typedef struct HCI_LE_Extended_Create_Connection_v2_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Advertising_Handle;
  uint8_t                     Subevent;
  uint8_t                     Initiator_Filter_Policy;
  uint8_t                     Own_Addr_Type;
  HCI_Ident_t                 Peer;
  uint8_t                     Initiating_PHYs;
  struct {
    HCI_Conn_Cfg_t            Conn_Cfg;
  } Initiating_PHYs_data[0];
} HCI_LE_Extended_Create_Connection_v2_Cmd_t;

static const uint16_t LE_Extended_Create_Connection_v1_Cmd = 0x2043; // OGF 0x8 OCF 0x043

typedef struct HCI_LE_Extended_Create_Connection_v1_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Initiator_Filter_Policy;
  uint8_t                     Own_Addr_Type;
  HCI_Ident_t                 Peer;
  uint8_t                     Initiating_PHYs;
  struct {
    HCI_Conn_Cfg_t            Conn_Cfg;
  } Initiating_PHYs_data[0];
} HCI_LE_Extended_Create_Connection_v1_Cmd_t;

// 7.8.37 LE Generate DHKey command

static const uint16_t LE_Generate_DHKey_v2_Cmd = 0x205e; // OGF 0x8 OCF 0x05e

typedef struct HCI_LE_Generate_DHKey_v2_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Key_X_Coordinate[32];
  uint8_t                     Key_Y_Coordinate[32];
  uint8_t                     Key_Type;
} HCI_LE_Generate_DHKey_v2_Cmd_t;

static const uint16_t LE_Generate_DHKey_v1_Cmd = 0x2026; // OGF 0x8 OCF 0x026

typedef struct HCI_LE_Generate_DHKey_v1_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Key_X_Coordinate[32];
  uint8_t                     Key_Y_Coordinate[32];
} HCI_LE_Generate_DHKey_v1_Cmd_t;

// 7.7.65.9 LE Generate DHKey Complete event

static const uint8_t LE_Generate_DHKey_Complete_Evt_sub = 0x09;

typedef struct HCI_LE_Generate_DHKey_Complete_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Subevent_Code;
  uint8_t                     Status;
  uint8_t                     DH_Key[32];
} HCI_LE_Generate_DHKey_Complete_Evt_t;

// 7.8.113 LE ISO Read Test Counters command

// HCI_LE_ISO_Read_Test_Counters_Cmd_t only has one member: Connection_Handle.

static const uint16_t LE_ISO_Read_Test_Counters_Cmd = 0x2072; // OGF 0x8 OCF 0x072

typedef struct HCI_LE_ISO_Read_Test_Counters_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  Status_Conn_Handle_t        Status_Conn;
  uint32_t                    Received_SDU_Count;
  uint32_t                    Missed_SDU_Count;
  uint32_t                    Failed_SDU_Count;
} HCI_LE_ISO_Read_Test_Counters_Ret_t;

// 7.8.112 LE ISO Receive Test command

static const uint16_t LE_ISO_Receive_Test_Cmd = 0x2071; // OGF 0x8 OCF 0x071

typedef struct HCI_LE_ISO_Receive_Test_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Conn_Handle;
  uint8_t                     Payload_Type;
} HCI_LE_ISO_Receive_Test_Cmd_t;

// HCI_LE_ISO_Receive_Test_Ret_t only has one member: Status_Conn.

// 7.8.114 LE ISO Test End command

// HCI_LE_ISO_Test_End_Cmd_t only has one member: Connection_Handle.

static const uint16_t LE_ISO_Test_End_Cmd = 0x2073; // OGF 0x8 OCF 0x073

typedef struct HCI_LE_ISO_Test_End_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  Status_Conn_Handle_t        Status_Conn;
  uint32_t                    Received_SDU_Count;
  uint32_t                    Missed_SDU_Count;
  uint32_t                    Failed_SDU_Count;
} HCI_LE_ISO_Test_End_Ret_t;

// 7.8.111 LE ISO Transmit Test command

static const uint16_t LE_ISO_Transmit_Test_Cmd = 0x2070; // OGF 0x8 OCF 0x070

typedef struct HCI_LE_ISO_Transmit_Test_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Conn_Handle;
  uint8_t                     Payload_Type;
} HCI_LE_ISO_Transmit_Test_Cmd_t;

// HCI_LE_ISO_Transmit_Test_Ret_t only has one member: Status_Conn.

// 7.7.65.5 LE Long Term Key Request event

static const uint8_t LE_Long_Term_Key_Request_Evt_sub = 0x05;

typedef struct HCI_LE_Long_Term_Key_Request_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Subevent_Code;
  uint16_t                    Conn_Handle;
  uint8_t                     Random_Number[8];
  uint16_t                    Encrypted_Diversifier;
} HCI_LE_Long_Term_Key_Request_Evt_t;

// 7.8.26 LE Long Term Key Request Negative Reply command

// HCI_LE_Long_Term_Key_Request_Negative_Reply_Cmd_t only has one member: Connection_Handle.

static const uint16_t LE_Long_Term_Key_Request_Negative_Reply_Cmd = 0x201b; // OGF 0x8 OCF 0x01b

// HCI_LE_Long_Term_Key_Request_Negative_Reply_Ret_t only has one member: Status_Conn.

// 7.8.25 LE Long Term Key Request Reply command

static const uint16_t LE_Long_Term_Key_Request_Reply_Cmd = 0x201a; // OGF 0x8 OCF 0x01a

typedef struct HCI_LE_Long_Term_Key_Request_Reply_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Conn_Handle;
  uint8_t                     Long_Term_Key[16];
} HCI_LE_Long_Term_Key_Request_Reply_Cmd_t;

// HCI_LE_Long_Term_Key_Request_Reply_Ret_t only has one member: Status_Conn.

// 7.8.94 LE Modify Sleep Clock Accuracy command

static const uint16_t LE_Modify_Sleep_Clock_Accuracy_Cmd = 0x205f; // OGF 0x8 OCF 0x05f

typedef struct HCI_LE_Modify_Sleep_Clock_Accuracy_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Action;
} HCI_LE_Modify_Sleep_Clock_Accuracy_Cmd_t;

// HCI_LE_Modify_Sleep_Clock_Accuracy_Ret_t only has one member: Status.

// 7.7.65.32 LE Path Loss Threshold event

static const uint8_t LE_Path_Loss_Threshold_Evt_sub = 0x20;

typedef struct HCI_LE_Path_Loss_Threshold_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Subevent_Code;
  uint16_t                    Conn_Handle;
  uint8_t                     Current_Path_Loss;
  uint8_t                     Zone_Entered;
} HCI_LE_Path_Loss_Threshold_Evt_t;

// 7.8.68 LE Periodic Advertising Create Sync Cancel command

// HCI_LE_PA_Create_Sync_Cancel_Ret_t only has one member: Status.

static const uint16_t LE_PA_Create_Sync_Cancel_Cmd = 0x2045; // OGF 0x8 OCF 0x045

// 7.8.67 LE Periodic Advertising Create Sync command

static const uint16_t LE_PA_Create_Sync_Cmd = 0x2044; // OGF 0x8 OCF 0x044

typedef struct HCI_LE_PA_Create_Sync_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Options;
  uint8_t                     Advertising_SID;
  HCI_Ident_t                 Advertiser;
  uint16_t                    Skip;
  uint16_t                    Sync_Timeout;
  uint8_t                     Sync_CTE_Type;
} HCI_LE_PA_Create_Sync_Cmd_t;

// 7.7.65.15 LE Periodic Advertising Report event

static const uint8_t LE_PA_Report_v2_Evt_sub = 0x25;

typedef struct HCI_LE_PA_Report_v2_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Subevent_Code;
  uint16_t                    Sync_Handle;
  uint8_t                     TX_Power;
  uint8_t                     RSSI;
  uint8_t                     CTE_Type;
  uint16_t                    Periodic_Event_Counter;
  uint8_t                     Subevent;
  uint8_t                     Data_Status;
  uint8_t                     Data_Length;
  uint8_t                     Data[0];
} HCI_LE_PA_Report_v2_Evt_t;

static const uint8_t LE_PA_Report_v1_Evt_sub = 0x0f;

typedef struct HCI_LE_PA_Report_v1_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Subevent_Code;
  uint16_t                    Sync_Handle;
  uint8_t                     TX_Power;
  uint8_t                     RSSI;
  uint8_t                     CTE_Type;
  uint8_t                     Data_Status;
  uint8_t                     Data_Length;
  uint8_t                     Data[0];
} HCI_LE_PA_Report_v1_Evt_t;

// 7.8.90 LE Periodic Advertising Set Info Transfer command

static const uint16_t LE_PA_Set_Info_Transfer_Cmd = 0x205b; // OGF 0x8 OCF 0x05b

typedef struct HCI_LE_PA_Set_Info_Transfer_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Conn_Handle;
  uint16_t                    Service_Data;
  uint8_t                     Advertising_Handle;
} HCI_LE_PA_Set_Info_Transfer_Cmd_t;

// HCI_LE_PA_Set_Info_Transfer_Ret_t only has one member: Status_Conn.

// 7.7.65.37 LE Periodic Advertising Response Report event

static const uint8_t LE_PA_Response_Report_Evt_sub = 0x28;

typedef struct HCI_LE_PA_Response_Report_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Subevent_Code;
  uint8_t                     Advertising_Handle;
  uint8_t                     Subevent;
  uint8_t                     Tx_Status;
  uint8_t                     Num_Responses;
  struct {
    uint8_t                   Tx_Power;
    uint8_t                   RSSI;
    uint8_t                   CTE_Type;
    uint8_t                   Response_Slot;
    uint8_t                   Data_Status;
    uint8_t                   Data_Length;
    uint8_t  *                Data;
  } Responses[0];
} HCI_LE_PA_Response_Report_Evt_t;

// 7.7.65.36 LE Periodic Advertising Subevent Data Request event

static const uint8_t LE_PA_Subevent_Data_Request_Evt_sub = 0x27;

typedef struct HCI_LE_PA_Subevent_Data_Request_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Subevent_Code;
  uint8_t                     Advertising_Handle;
  uint8_t                     Subevent_Start;
  uint8_t                     Subevent_Data_Count;
} HCI_LE_PA_Subevent_Data_Request_Evt_t;

// 7.7.65.14 LE Periodic Advertising Sync Established event

typedef struct HCI_Adv_Ctx_t {                 // Common
  uint8_t                     Advertising_SID;
  HCI_Ident_t                 Advertiser;
  uint8_t                     Advertiser_PHY;
} HCI_Adv_Ctx_t;

static const uint8_t LE_PAS_Established_v2_Evt_sub = 0x24;

typedef struct HCI_LE_PAS_Established_v2_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Subevent_Code;
  uint8_t                     Status;
  uint16_t                    Sync_Handle;
  HCI_Adv_Ctx_t               Adv_Ctx;
  uint16_t                    PA_Interval;
  uint8_t                     Advertiser_Clock_Accuracy;
  uint8_t                     Num_Subevents;
  uint8_t                     Subevent_Interval;
  uint8_t                     Response_Slot_Delay;
  uint8_t                     Response_Slot_Spacing;
} HCI_LE_PAS_Established_v2_Evt_t;

static const uint8_t LE_PAS_Established_v1_Evt_sub = 0x0e;

typedef struct HCI_LE_PAS_Established_v1_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Subevent_Code;
  uint8_t                     Status;
  uint16_t                    Sync_Handle;
  HCI_Adv_Ctx_t               Adv_Ctx;
  uint16_t                    PA_Interval;
  uint8_t                     Advertiser_Clock_Accuracy;
} HCI_LE_PAS_Established_v1_Evt_t;

// 7.7.65.16 LE Periodic Advertising Sync Lost event

static const uint8_t LE_PAS_Lost_Evt_sub = 0x10;

typedef struct HCI_LE_PAS_Lost_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Subevent_Code;
  uint16_t                    Sync_Handle;
} HCI_LE_PAS_Lost_Evt_t;

// 7.8.89 LE Periodic Advertising Sync Transfer command

static const uint16_t LE_PAST_Cmd = 0x205a; // OGF 0x8 OCF 0x05a

typedef struct HCI_LE_PAST_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Conn_Handle;
  uint16_t                    Service_Data;
  uint16_t                    Sync_Handle;
} HCI_LE_PAST_Cmd_t;

// HCI_LE_PAST_Ret_t only has one member: Status_Conn.

// 7.7.65.24 LE Periodic Advertising Sync Transfer Received event

static const uint8_t LE_PAST_Received_v2_Evt_sub = 0x26;

typedef struct HCI_LE_PAST_Received_v2_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Subevent_Code;
  uint8_t                     Status;
  uint16_t                    Conn_Handle;
  uint16_t                    Service_Data;
  uint16_t                    Sync_Handle;
  HCI_Adv_Ctx_t               Adv_Ctx;
  uint16_t                    PA_Interval;
  uint8_t                     Advertiser_Clock_Accuracy;
  uint8_t                     Num_Subevents;
  uint8_t                     Subevent_Interval;
  uint8_t                     Response_Slot_Delay;
  uint8_t                     Response_Slot_Spacing;
} HCI_LE_PAST_Received_v2_Evt_t;

static const uint8_t LE_PAST_Received_v1_Evt_sub = 0x18;

typedef struct HCI_LE_PAST_Received_v1_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Subevent_Code;
  uint8_t                     Status;
  uint16_t                    Conn_Handle;
  uint16_t                    Service_Data;
  uint16_t                    Sync_Handle;
  HCI_Adv_Ctx_t               Adv_Ctx;
  uint16_t                    PA_Interval;
  uint8_t                     Advertiser_Clock_Accuracy;
} HCI_LE_PAST_Received_v1_Evt_t;

// 7.8.69 LE Periodic Advertising Terminate Sync command

static const uint16_t LE_PA_Terminate_Sync_Cmd = 0x2046; // OGF 0x8 OCF 0x046

typedef struct HCI_LE_PA_Terminate_Sync_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Sync_Handle;
} HCI_LE_PA_Terminate_Sync_Cmd_t;

// HCI_LE_PA_Terminate_Sync_Ret_t only has one member: Status.

// 7.7.65.12 LE PHY Update Complete event

static const uint8_t LE_PHY_Update_Complete_Evt_sub = 0x0c;

typedef struct HCI_LE_PHY_Update_Complete_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Subevent_Code;
  uint8_t                     Status;
  uint16_t                    Conn_Handle;
  uint8_t                     TX_PHY;
  uint8_t                     RX_PHY;
} HCI_LE_PHY_Update_Complete_Evt_t;

// 7.8.23 LE Rand command

static const uint16_t LE_Rand_Cmd = 0x2018; // OGF 0x8 OCF 0x018

typedef struct HCI_LE_Rand_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     Random_Number[8];
} HCI_LE_Rand_Ret_t;

// 7.8.6 LE Read Advertising Physical Channel Tx Power command

static const uint16_t LE_Read_Advertising_Channel_Tx_Power_Cmd = 0x2007; // OGF 0x8 OCF 0x007

typedef struct HCI_LE_Read_Advertising_Channel_Tx_Power_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     TX_Power_Level;
} HCI_LE_Read_Advertising_Channel_Tx_Power_Ret_t;

// 7.8.87 LE Read Antenna Information command

static const uint16_t LE_Read_Antenna_Information_Cmd = 0x2058; // OGF 0x8 OCF 0x058

typedef struct HCI_LE_Read_Antenna_Information_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     Supported_Switching_Sampling_Rates;
  uint8_t                     Num_Antennae;
  uint8_t                     Max_Switching_Pattern_Length;
  uint8_t                     Max_CTE_Length;
} HCI_LE_Read_Antenna_Information_Ret_t;

// 7.8.2 LE Read Buffer Size command

static const uint16_t LE_Read_Buffer_Size_v2_Cmd = 0x2060; // OGF 0x8 OCF 0x060

typedef struct HCI_LE_Read_Buffer_Size_v2_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint16_t                    LE_ACL_Data_Packet_Length;
  uint8_t                     Total_Num_LE_ACL_Data_Packets;
  uint16_t                    ISO_Data_Packet_Length;
  uint8_t                     Total_Num_ISO_Data_Packets;
} HCI_LE_Read_Buffer_Size_v2_Ret_t;

static const uint16_t LE_Read_Buffer_Size_v1_Cmd = 0x2002; // OGF 0x8 OCF 0x002

typedef struct HCI_LE_Read_Buffer_Size_v1_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint16_t                    LE_ACL_Data_Packet_Length;
  uint8_t                     Total_Num_LE_ACL_Data_Packets;
} HCI_LE_Read_Buffer_Size_v1_Ret_t;

// 7.8.20 LE Read Channel Map command

// HCI_LE_Read_Channel_Map_Cmd_t only has one member: Connection_Handle.

static const uint16_t LE_Read_Channel_Map_Cmd = 0x2015; // OGF 0x8 OCF 0x015

typedef struct HCI_LE_Read_Channel_Map_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  Status_Conn_Handle_t        Status_Conn;
  uint8_t                     Channel_Map[5];
} HCI_LE_Read_Channel_Map_Ret_t;

// 7.8.14 LE Read Filter Accept List Size command

static const uint16_t LE_Read_Filter_Accept_List_Size_Cmd = 0x200f; // OGF 0x8 OCF 0x00f

typedef struct HCI_LE_Read_Filter_Accept_List_Size_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     Filter_Accept_List_Size;
} HCI_LE_Read_Filter_Accept_List_Size_Ret_t;

// 7.8.116 LE Read ISO Link Quality command

// HCI_LE_Read_ISO_Link_Quality_Cmd_t only has one member: Connection_Handle.

static const uint16_t LE_Read_ISO_Link_Quality_Cmd = 0x2075; // OGF 0x8 OCF 0x075

typedef struct HCI_LE_Read_ISO_Link_Quality_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  Status_Conn_Handle_t        Status_Conn;
  uint32_t                    TX_UnACKed_Packets;
  uint32_t                    TX_Flushed_Packets;
  uint32_t                    TX_Last_Subevent_Packets;
  uint32_t                    Retransmitted_Packets;
  uint32_t                    CRC_Error_Packets;
  uint32_t                    RX_Unreceived_Packets;
  uint32_t                    Duplicate_Packets;
} HCI_LE_Read_ISO_Link_Quality_Ret_t;

// 7.8.96 LE Read ISO TX Sync command

// HCI_LE_Read_ISO_TX_Sync_Cmd_t only has one member: Connection_Handle.

static const uint16_t LE_Read_ISO_TX_Sync_Cmd = 0x2061; // OGF 0x8 OCF 0x061

typedef struct HCI_LE_Read_ISO_TX_Sync_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  Status_Conn_Handle_t        Status_Conn;
  uint16_t                    Packet_Sequence_Number;
  uint32_t                    TX_Time_Stamp;
  uint8_t                     Time_Offset[3];
} HCI_LE_Read_ISO_TX_Sync_Ret_t;

// 7.7.65.8 LE Read Local P-256 Public Key Complete event

static const uint8_t LE_Read_Local_P_256_Public_Key_Complete_Evt_sub = 0x08;

typedef struct HCI_LE_Read_Local_P_256_Public_Key_Complete_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Subevent_Code;
  uint8_t                     Status;
  uint8_t                     Key_X_Coordinate[32];
  uint8_t                     Key_Y_Coordinate[32];
} HCI_LE_Read_Local_P_256_Public_Key_Complete_Evt_t;

// 7.8.43 LE Read Local Resolvable Address command

static const uint16_t LE_Read_Local_Resolvable_Addr_Cmd = 0x202c; // OGF 0x8 OCF 0x02c

typedef struct HCI_LE_Read_Local_Resolvable_Addr_Cmd_t {
  HCI_Cmd_t                   Cmd;
  HCI_Ident_t                 Peer_Identity;
} HCI_LE_Read_Local_Resolvable_Addr_Cmd_t;

typedef struct HCI_LE_Read_Local_Resolvable_Addr_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  BD_Addr_t                   Local_Resolvable_Addr;
} HCI_LE_Read_Local_Resolvable_Addr_Ret_t;

// 7.8.3 LE Read Local Supported Features command

static const uint16_t LE_Read_Local_Supported_Features_Cmd = 0x2003; // OGF 0x8 OCF 0x003

typedef struct HCI_LE_Read_Local_Supported_Features_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     LE_Features[8];
} HCI_LE_Read_Local_Supported_Features_Ret_t;

// 7.8.57 LE Read Maximum Advertising Data Length command

static const uint16_t LE_Read_Maximum_Advertising_Data_Length_Cmd = 0x203a; // OGF 0x8 OCF 0x03a

typedef struct HCI_LE_Read_Maximum_Advertising_Data_Length_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint16_t                    Max_Advertising_Data_Length;
} HCI_LE_Read_Maximum_Advertising_Data_Length_Ret_t;

// 7.8.46 LE Read Maximum Data Length command

static const uint16_t LE_Read_Maximum_Data_Length_Cmd = 0x202f; // OGF 0x8 OCF 0x02f

typedef struct HCI_LE_Read_Maximum_Data_Length_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint16_t                    Supported_Max_TX_Octets;
  uint16_t                    Supported_Max_TX_Time;
  uint16_t                    Supported_Max_RX_Octets;
  uint16_t                    Supported_Max_RX_Time;
} HCI_LE_Read_Maximum_Data_Length_Ret_t;

// 7.8.58 LE Read Number of Supported Advertising Sets command

static const uint16_t LE_Read_Number_of_Supported_Advertising_Sets_Cmd = 0x203b; // OGF 0x8 OCF 0x03b

typedef struct HCI_LE_Read_Number_of_Supported_Advertising_Sets_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     Num_Supported_Advertising_Sets;
} HCI_LE_Read_Number_of_Supported_Advertising_Sets_Ret_t;

// 7.8.42 LE Read Peer Resolvable Address command

static const uint16_t LE_Read_Peer_Resolvable_Addr_Cmd = 0x202b; // OGF 0x8 OCF 0x02b

typedef struct HCI_LE_Read_Peer_Resolvable_Addr_Cmd_t {
  HCI_Cmd_t                   Cmd;
  HCI_Ident_t                 Peer_Identity;
} HCI_LE_Read_Peer_Resolvable_Addr_Cmd_t;

typedef struct HCI_LE_Read_Peer_Resolvable_Addr_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  BD_Addr_t                   Peer_Resolvable_Addr;
} HCI_LE_Read_Peer_Resolvable_Addr_Ret_t;

// 7.8.73 LE Read Periodic Advertiser List Size command

static const uint16_t LE_Read_PAdvertiser_List_Size_Cmd = 0x204a; // OGF 0x8 OCF 0x04a

typedef struct HCI_LE_Read_PAdvertiser_List_Size_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     PAdvertiser_List_Size;
} HCI_LE_Read_PAdvertiser_List_Size_Ret_t;

// 7.8.47 LE Read PHY command

// HCI_LE_Read_PHY_Cmd_t only has one member: Connection_Handle.

static const uint16_t LE_Read_PHY_Cmd = 0x2030; // OGF 0x8 OCF 0x030

typedef struct HCI_LE_Read_PHY_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  Status_Conn_Handle_t        Status_Conn;
  uint8_t                     TX_PHY;
  uint8_t                     RX_PHY;
} HCI_LE_Read_PHY_Ret_t;

// 7.8.21 LE Read Remote Features command

// HCI_LE_Read_Remote_Features_Cmd_t only has one member: Connection_Handle.

static const uint16_t LE_Read_Remote_Features_Cmd = 0x2016; // OGF 0x8 OCF 0x016

// 7.7.65.4 LE Read Remote Features Complete event

static const uint8_t LE_Read_Remote_Features_Complete_Evt_sub = 0x04;

typedef struct HCI_LE_Read_Remote_Features_Complete_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Subevent_Code;
  uint8_t                     Status;
  uint16_t                    Conn_Handle;
  uint8_t                     LE_Features[8];
} HCI_LE_Read_Remote_Features_Complete_Evt_t;

// 7.8.118 LE Read Remote Transmit Power Level command

static const uint16_t LE_Read_Remote_Transmit_Power_Level_Cmd = 0x2077; // OGF 0x8 OCF 0x077

typedef struct HCI_LE_Read_Remote_Transmit_Power_Level_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Conn_Handle;
  uint8_t                     PHY;
} HCI_LE_Read_Remote_Transmit_Power_Level_Cmd_t;

// 7.8.41 LE Read Resolving List Size command

static const uint16_t LE_Read_Resolving_List_Size_Cmd = 0x202a; // OGF 0x8 OCF 0x02a

typedef struct HCI_LE_Read_Resolving_List_Size_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     Resolving_List_Size;
} HCI_LE_Read_Resolving_List_Size_Ret_t;

// 7.8.75 LE Read RF Path Compensation command

static const uint16_t LE_Read_RF_Path_Compensation_Cmd = 0x204c; // OGF 0x8 OCF 0x04c

typedef struct HCI_LE_Read_RF_Path_Compensation_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint16_t                    RF_TX_Path_Compensation_Value;
  uint16_t                    RF_RX_Path_Compensation_Value;
} HCI_LE_Read_RF_Path_Compensation_Ret_t;

// 7.8.34 LE Read Suggested Default Data Length command

static const uint16_t LE_Read_Suggested_Default_Data_Length_Cmd = 0x2023; // OGF 0x8 OCF 0x023

typedef struct HCI_LE_Read_Suggested_Default_Data_Length_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint16_t                    Suggested_Max_TX_Octets;
  uint16_t                    Suggested_Max_TX_Time;
} HCI_LE_Read_Suggested_Default_Data_Length_Ret_t;

// 7.8.27 LE Read Supported States command

static const uint16_t LE_Read_Supported_States_Cmd = 0x201c; // OGF 0x8 OCF 0x01c

typedef struct HCI_LE_Read_Supported_States_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     LE_States;
} HCI_LE_Read_Supported_States_Ret_t;

// 7.8.74 LE Read Transmit Power command

static const uint16_t LE_Read_Transmit_Power_Cmd = 0x204b; // OGF 0x8 OCF 0x04b

typedef struct HCI_LE_Read_Transmit_Power_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     Min_TX_Power;
  uint8_t                     Max_TX_Power;
} HCI_LE_Read_Transmit_Power_Ret_t;

// 7.8.28 LE Receiver Test command

static const uint16_t LE_Receiver_Test_v3_Cmd = 0x204f; // OGF 0x8 OCF 0x04f

typedef struct HCI_LE_Receiver_Test_v3_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     RX_Channel;
  uint8_t                     PHY;
  uint8_t                     Modulation_Index;
  uint8_t                     Expected_CTE_Length;
  uint8_t                     Expected_CTE_Type;
  uint8_t                     Slot_Durations;
  uint8_t                     Switching_Pattern_Length;
  uint8_t                     Antenna_IDs[0];
} HCI_LE_Receiver_Test_v3_Cmd_t;

// HCI_LE_Receiver_Test_v3_Ret_t only has one member: Status.

static const uint16_t LE_Receiver_Test_v2_Cmd = 0x2033; // OGF 0x8 OCF 0x033

typedef struct HCI_LE_Receiver_Test_v2_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     RX_Channel;
  uint8_t                     PHY;
  uint8_t                     Modulation_Index;
} HCI_LE_Receiver_Test_v2_Cmd_t;

// HCI_LE_Receiver_Test_v2_Ret_t only has one member: Status.

static const uint16_t LE_Receiver_Test_v1_Cmd = 0x201d; // OGF 0x8 OCF 0x01d

typedef struct HCI_LE_Receiver_Test_v1_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     RX_Channel;
} HCI_LE_Receiver_Test_v1_Cmd_t;

// HCI_LE_Receiver_Test_v1_Ret_t only has one member: Status.

// 7.8.102 LE Reject CIS Request command

static const uint16_t LE_Reject_CIS_Request_Cmd = 0x2067; // OGF 0x8 OCF 0x067

typedef struct HCI_LE_Reject_CIS_Request_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Conn_Handle;
  uint8_t                     Reason;
} HCI_LE_Reject_CIS_Request_Cmd_t;

// HCI_LE_Reject_CIS_Request_Ret_t only has one member: Status_Conn.

// 7.7.65.6 LE Remote Connection Parameter Request event

static const uint8_t LE_Remote_Connection_Parameter_Request_Evt_sub = 0x06;

typedef struct HCI_LE_Remote_Connection_Parameter_Request_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Subevent_Code;
  uint16_t                    Conn_Handle;
  uint16_t                    Interval_Min;
  uint16_t                    Interval_Max;
  uint16_t                    Max_Latency;
  uint16_t                    Timeout;
} HCI_LE_Remote_Connection_Parameter_Request_Evt_t;

// 7.8.32 LE Remote Connection Parameter Request Negative Reply command

static const uint16_t LE_Remote_Connection_Parameter_Request_Negative_Reply_Cmd = 0x2021; // OGF 0x8 OCF 0x021

typedef struct HCI_LE_Remote_Connection_Parameter_Request_Negative_Reply_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Conn_Handle;
  uint8_t                     Reason;
} HCI_LE_Remote_Connection_Parameter_Request_Negative_Reply_Cmd_t;

// HCI_LE_Remote_Connection_Parameter_Request_Negative_Reply_Ret_t only has one member: Status_Conn.

// 7.8.31 LE Remote Connection Parameter Request Reply command

static const uint16_t LE_Remote_Connection_Parameter_Request_Reply_Cmd = 0x2020; // OGF 0x8 OCF 0x020

typedef struct HCI_LE_Remote_Connection_Parameter_Request_Reply_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Conn_Handle;
  uint16_t                    Interval_Min;
  uint16_t                    Interval_Max;
  uint16_t                    Max_Latency;
  uint16_t                    Timeout;
  uint16_t                    Min_CE_Length;
  uint16_t                    Max_CE_Length;
} HCI_LE_Remote_Connection_Parameter_Request_Reply_Cmd_t;

// HCI_LE_Remote_Connection_Parameter_Request_Reply_Ret_t only has one member: Status_Conn.

// 7.8.59 LE Remove Advertising Set command

static const uint16_t LE_Remove_Advertising_Set_Cmd = 0x203c; // OGF 0x8 OCF 0x03c

typedef struct HCI_LE_Remove_Advertising_Set_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Advertising_Handle;
} HCI_LE_Remove_Advertising_Set_Cmd_t;

// HCI_LE_Remove_Advertising_Set_Ret_t only has one member: Status.

// 7.8.100 LE Remove CIG command

static const uint16_t LE_Remove_CIG_Cmd = 0x2065; // OGF 0x8 OCF 0x065

typedef struct HCI_LE_Remove_CIG_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     CIG_ID;
} HCI_LE_Remove_CIG_Cmd_t;

typedef struct HCI_LE_Remove_CIG_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     CIG_ID;
} HCI_LE_Remove_CIG_Ret_t;

// 7.8.17 LE Remove Device From Filter Accept List command

static const uint16_t LE_Remove_Device_From_Filter_Accept_List_Cmd = 0x2012; // OGF 0x8 OCF 0x012

typedef struct HCI_LE_Remove_Device_From_Filter_Accept_List_Cmd_t {
  HCI_Cmd_t                   Cmd;
  HCI_Ident_t                 Ident;
} HCI_LE_Remove_Device_From_Filter_Accept_List_Cmd_t;

// HCI_LE_Remove_Device_From_Filter_Accept_List_Ret_t only has one member: Status.

// 7.8.71 LE Remove Device From Periodic Advertiser List command

static const uint16_t LE_Remove_Device_From_PAdvertiser_List_Cmd = 0x2048; // OGF 0x8 OCF 0x048

typedef struct HCI_LE_Remove_Device_From_PAdvertiser_List_Cmd_t {
  HCI_Cmd_t                   Cmd;
  HCI_Ident_t                 Advertiser;
  uint8_t                     Advertising_SID;
} HCI_LE_Remove_Device_From_PAdvertiser_List_Cmd_t;

// HCI_LE_Remove_Device_From_PAdvertiser_List_Ret_t only has one member: Status.

// 7.8.39 LE Remove Device From Resolving List command

static const uint16_t LE_Remove_Device_From_Resolving_List_Cmd = 0x2028; // OGF 0x8 OCF 0x028

typedef struct HCI_LE_Remove_Device_From_Resolving_List_Cmd_t {
  HCI_Cmd_t                   Cmd;
  HCI_Ident_t                 Peer_Identity;
} HCI_LE_Remove_Device_From_Resolving_List_Cmd_t;

// HCI_LE_Remove_Device_From_Resolving_List_Ret_t only has one member: Status.

// 7.8.110 LE Remove ISO Data Path command

static const uint16_t LE_Remove_ISO_Data_Path_Cmd = 0x206f; // OGF 0x8 OCF 0x06f

typedef struct HCI_LE_Remove_ISO_Data_Path_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Conn_Handle;
  uint8_t                     Data_Path_Direction;
} HCI_LE_Remove_ISO_Data_Path_Cmd_t;

// HCI_LE_Remove_ISO_Data_Path_Ret_t only has one member: Status_Conn.

// 7.8.108 LE Request Peer SCA command

// HCI_LE_Request_Peer_SCA_Cmd_t only has one member: Connection_Handle.

static const uint16_t LE_Request_Peer_SCA_Cmd = 0x206d; // OGF 0x8 OCF 0x06d

// 7.7.65.31 LE Request Peer SCA Complete event

static const uint8_t LE_Request_Peer_SCA_Complete_Evt_sub = 0x1f;

typedef struct HCI_LE_Request_Peer_SCA_Complete_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Subevent_Code;
  uint8_t                     Status;
  uint16_t                    Conn_Handle;
  uint8_t                     Peer_Clock_Accuracy;
} HCI_LE_Request_Peer_SCA_Complete_Evt_t;

// 7.7.65.19 LE Scan Request Received event

static const uint8_t LE_Scan_Request_Received_Evt_sub = 0x13;

typedef struct HCI_LE_Scan_Request_Received_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Subevent_Code;
  uint8_t                     Advertising_Handle;
  HCI_Ident_t                 Scanner;
} HCI_LE_Scan_Request_Received_Evt_t;

// 7.7.65.17 LE Scan Timeout event

static const uint8_t LE_Scan_Timeout_Evt_sub = 0x11;

typedef struct HCI_LE_Scan_Timeout_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Subevent_Code;
} HCI_LE_Scan_Timeout_Evt_t;

// 7.8.44 LE Set Address Resolution Enable command

static const uint16_t LE_Set_Addr_Resolution_Enable_Cmd = 0x202d; // OGF 0x8 OCF 0x02d

typedef struct HCI_LE_Set_Addr_Resolution_Enable_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Addr_Resolution_Enable;
} HCI_LE_Set_Addr_Resolution_Enable_Cmd_t;

// HCI_LE_Set_Addr_Resolution_Enable_Ret_t only has one member: Status.

// 7.8.7 LE Set Advertising Data command

static const uint16_t LE_Set_Advertising_Data_Cmd = 0x2008; // OGF 0x8 OCF 0x008

typedef struct HCI_LE_Set_Advertising_Data_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Advertising_Data_Length;
  uint8_t                     Advertising_Data[31];
} HCI_LE_Set_Advertising_Data_Cmd_t;

// HCI_LE_Set_Advertising_Data_Ret_t only has one member: Status.

// 7.8.9 LE Set Advertising Enable command

static const uint16_t LE_Set_Advertising_Enable_Cmd = 0x200a; // OGF 0x8 OCF 0x00a

typedef struct HCI_LE_Set_Advertising_Enable_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Advertising_Enable;
} HCI_LE_Set_Advertising_Enable_Cmd_t;

// HCI_LE_Set_Advertising_Enable_Ret_t only has one member: Status.

// 7.8.5 LE Set Advertising Parameters command

static const uint16_t LE_Set_Advertising_Parameters_Cmd = 0x2006; // OGF 0x8 OCF 0x006

typedef struct HCI_LE_Set_Advertising_Parameters_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Advertising_Interval_Min;
  uint16_t                    Advertising_Interval_Max;
  uint8_t                     Advertising_Type;
  uint8_t                     Own_Addr_Type;
  HCI_Ident_t                 Peer;
  uint8_t                     Advertising_Channel_Map;
  uint8_t                     Advertising_Filter_Policy;
} HCI_LE_Set_Advertising_Parameters_Cmd_t;

// HCI_LE_Set_Advertising_Parameters_Ret_t only has one member: Status.

// 7.8.52 LE Set Advertising Set Random Address command

static const uint16_t LE_Set_Advertising_Set_Random_Addr_Cmd = 0x2035; // OGF 0x8 OCF 0x035

typedef struct HCI_LE_Set_Advertising_Set_Random_Addr_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Advertising_Handle;
  BD_Addr_t                   Random_Addr;
} HCI_LE_Set_Advertising_Set_Random_Addr_Cmd_t;

// HCI_LE_Set_Advertising_Set_Random_Addr_Ret_t only has one member: Status.

// 7.8.97 LE Set CIG Parameters command

static const uint16_t LE_Set_CIG_Parameters_Cmd = 0x2062; // OGF 0x8 OCF 0x062

typedef struct HCI_LE_Set_CIG_Parameters_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     CIG_ID;
  uint8_t                     SDU_Interval_C_To_P[3];
  uint8_t                     SDU_Interval_P_To_C[3];
  uint8_t                     Worst_Case_SCA;
  uint8_t                     Packing;
  uint8_t                     Framing;
  uint16_t                    Max_Transport_Latency_C_To_P;
  uint16_t                    Max_Transport_Latency_P_To_C;
  uint8_t                     CIS_Count;
  struct {
    uint8_t                   CIS_ID;
    uint16_t                  Max_SDU_C_To_P;
    uint16_t                  Max_SDU_P_To_C;
    uint8_t                   PHY_C_To_P;
    uint8_t                   PHY_P_To_C;
    uint8_t                   RTN_C_To_P;
    uint8_t                   RTN_P_To_C;
  } CIS[0];
} HCI_LE_Set_CIG_Parameters_Cmd_t;

typedef struct HCI_LE_Set_CIG_Parameters_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     CIG_ID;
  uint8_t                     CIS_Count;
  uint16_t                    Conn_Handle[0];
} HCI_LE_Set_CIG_Parameters_Ret_t;

// 7.8.98 LE Set CIG Parameters Test command

static const uint16_t LE_Set_CIG_Parameters_Test_Cmd = 0x2063; // OGF 0x8 OCF 0x063

typedef struct HCI_LE_Set_CIG_Parameters_Test_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     CIG_ID;
  uint8_t                     SDU_Interval_C_To_P[3];
  uint8_t                     SDU_Interval_P_To_C[3];
  uint8_t                     FT_C_To_P;
  uint8_t                     FT_P_To_C;
  uint16_t                    ISO_Interval;
  uint8_t                     Worst_Case_SCA;
  uint8_t                     Packing;
  uint8_t                     Framing;
  uint8_t                     CIS_Count;
  struct {
    uint8_t                   CIS_ID;
    uint8_t                   NSE;
    uint16_t                  Max_SDU_C_To_P;
    uint16_t                  Max_SDU_P_To_C;
    uint16_t                  Max_PDU_C_To_P;
    uint16_t                  Max_PDU_P_To_C;
    uint8_t                   PHY_C_To_P;
    uint8_t                   PHY_P_To_C;
    uint8_t                   BN_C_To_P;
    uint8_t                   BN_P_To_C;
  } CIS[0];
} HCI_LE_Set_CIG_Parameters_Test_Cmd_t;

typedef struct HCI_LE_Set_CIG_Parameters_Test_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     CIG_ID;
  uint8_t                     CIS_Count;
  uint16_t                    Conn_Handle[0];
} HCI_LE_Set_CIG_Parameters_Test_Ret_t;

// 7.8.83 LE Set Connection CTE Receive Parameters command

static const uint16_t LE_Set_Connection_CTE_Receive_Parameters_Cmd = 0x2054; // OGF 0x8 OCF 0x054

typedef struct HCI_LE_Set_Connection_CTE_Receive_Parameters_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Conn_Handle;
  uint8_t                     Sampling_Enable;
  uint8_t                     Slot_Durations;
  uint8_t                     Switching_Pattern_Length;
  uint8_t                     Antenna_IDs[0];
} HCI_LE_Set_Connection_CTE_Receive_Parameters_Cmd_t;

// HCI_LE_Set_Connection_CTE_Receive_Parameters_Ret_t only has one member: Status_Conn.

// 7.8.84 LE Set Connection CTE Transmit Parameters command

static const uint16_t LE_Set_Connection_CTE_Transmit_Parameters_Cmd = 0x2055; // OGF 0x8 OCF 0x055

typedef struct HCI_LE_Set_Connection_CTE_Transmit_Parameters_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Conn_Handle;
  uint8_t                     CTE_Types;
  uint8_t                     Switching_Pattern_Length;
  uint8_t                     Antenna_IDs[0];
} HCI_LE_Set_Connection_CTE_Transmit_Parameters_Cmd_t;

// HCI_LE_Set_Connection_CTE_Transmit_Parameters_Ret_t only has one member: Status_Conn.

// 7.8.81 LE Set Connectionless CTE Transmit Enable command

static const uint16_t LE_Set_Connectionless_CTE_Transmit_Enable_Cmd = 0x2052; // OGF 0x8 OCF 0x052

typedef struct HCI_LE_Set_Connectionless_CTE_Transmit_Enable_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Advertising_Handle;
  uint8_t                     CTE_Enable;
} HCI_LE_Set_Connectionless_CTE_Transmit_Enable_Cmd_t;

// HCI_LE_Set_Connectionless_CTE_Transmit_Enable_Ret_t only has one member: Status.

// 7.8.80 LE Set Connectionless CTE Transmit Parameters command

static const uint16_t LE_Set_Connectionless_CTE_Transmit_Parameters_Cmd = 0x2051; // OGF 0x8 OCF 0x051

typedef struct HCI_LE_Set_Connectionless_CTE_Transmit_Parameters_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Advertising_Handle;
  uint8_t                     CTE_Length;
  uint8_t                     CTE_Type;
  uint8_t                     CTE_Count;
  uint8_t                     Switching_Pattern_Length;
  uint8_t                     Antenna_IDs[0];
} HCI_LE_Set_Connectionless_CTE_Transmit_Parameters_Cmd_t;

// HCI_LE_Set_Connectionless_CTE_Transmit_Parameters_Ret_t only has one member: Status.

// 7.8.82 LE Set Connectionless IQ Sampling Enable command

static const uint16_t LE_Set_Connectionless_IQ_Sampling_Enable_Cmd = 0x2053; // OGF 0x8 OCF 0x053

typedef struct HCI_LE_Set_Connectionless_IQ_Sampling_Enable_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Sync_Handle;
  uint8_t                     Sampling_Enable;
  uint8_t                     Slot_Durations;
  uint8_t                     Max_Sampled_CTEs;
  uint8_t                     Switching_Pattern_Length;
  uint8_t                     Antenna_IDs[0];
} HCI_LE_Set_Connectionless_IQ_Sampling_Enable_Cmd_t;

typedef struct HCI_LE_Set_Connectionless_IQ_Sampling_Enable_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint16_t                    Sync_Handle;
} HCI_LE_Set_Connectionless_IQ_Sampling_Enable_Ret_t;

// 7.8.33 LE Set Data Length command

static const uint16_t LE_Set_Data_Length_Cmd = 0x2022; // OGF 0x8 OCF 0x022

typedef struct HCI_LE_Set_Data_Length_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Conn_Handle;
  uint16_t                    TX_Octets;
  uint16_t                    TX_Time;
} HCI_LE_Set_Data_Length_Cmd_t;

// HCI_LE_Set_Data_Length_Ret_t only has one member: Status_Conn.

// 7.8.122 LE Set Data Related Address Changes command

static const uint16_t LE_Set_Data_Related_Addr_Changes_Cmd = 0x207c; // OGF 0x8 OCF 0x07c

typedef struct HCI_LE_Set_Data_Related_Addr_Changes_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Advertising_Handle;
  uint8_t                     Change_Reasons;
} HCI_LE_Set_Data_Related_Addr_Changes_Cmd_t;

// HCI_LE_Set_Data_Related_Addr_Changes_Ret_t only has one member: Status.

// 7.8.92 LE Set Default Periodic Advertising Sync Transfer Parameters command

static const uint16_t LE_Set_Default_PAST_Parameters_Cmd = 0x205d; // OGF 0x8 OCF 0x05d

typedef struct HCI_LE_Set_Default_PAST_Parameters_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Mode;
  uint16_t                    Skip;
  uint16_t                    Sync_Timeout;
  uint8_t                     CTE_Type;
} HCI_LE_Set_Default_PAST_Parameters_Cmd_t;

// HCI_LE_Set_Default_PAST_Parameters_Ret_t only has one member: Status.

// 7.8.48 LE Set Default PHY command

static const uint16_t LE_Set_Default_PHY_Cmd = 0x2031; // OGF 0x8 OCF 0x031

typedef struct HCI_LE_Set_Default_PHY_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     All_PHYs;
  uint8_t                     TX_PHYs;
  uint8_t                     RX_PHYs;
} HCI_LE_Set_Default_PHY_Cmd_t;

// HCI_LE_Set_Default_PHY_Ret_t only has one member: Status.

// 7.8.123 LE Set Default Subrate command

static const uint16_t LE_Set_Default_Subrate_Cmd = 0x207d; // OGF 0x8 OCF 0x07d

typedef struct HCI_LE_Set_Default_Subrate_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Subrate_Min;
  uint16_t                    Subrate_Max;
  uint16_t                    Max_Latency;
  uint16_t                    Continuation_Number;
  uint16_t                    Supervision_Timeout;
} HCI_LE_Set_Default_Subrate_Cmd_t;

// HCI_LE_Set_Default_Subrate_Ret_t only has one member: Status.

// 7.8.1 LE Set Event Mask command

static const uint16_t LE_Set_Event_Mask_Cmd = 0x2001; // OGF 0x8 OCF 0x001

typedef struct HCI_LE_Set_Event_Mask_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     LE_Event_Mask[8];
} HCI_LE_Set_Event_Mask_Cmd_t;

// HCI_LE_Set_Event_Mask_Ret_t only has one member: Status.

// 7.8.54 LE Set Extended Advertising Data command

static const uint16_t LE_Set_Extended_Advertising_Data_Cmd = 0x2037; // OGF 0x8 OCF 0x037

typedef struct HCI_LE_Set_Extended_Advertising_Data_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Advertising_Handle;
  uint8_t                     Operation;
  uint8_t                     Fragment_Preference;
  uint8_t                     Advertising_Data_Length;
  uint8_t                     Advertising_Data[0];
} HCI_LE_Set_Extended_Advertising_Data_Cmd_t;

// HCI_LE_Set_Extended_Advertising_Data_Ret_t only has one member: Status.

// 7.8.56 LE Set Extended Advertising Enable command

static const uint16_t LE_Set_Extended_Advertising_Enable_Cmd = 0x2039; // OGF 0x8 OCF 0x039

typedef struct HCI_LE_Set_Extended_Advertising_Enable_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Enable;
  uint8_t                     Num_Sets;
  struct {
    uint8_t                   Advertising_Handle;
    uint16_t                  Duration;
    uint8_t                   Max_Extended_Advertising_Events;
  } Sets[0];
} HCI_LE_Set_Extended_Advertising_Enable_Cmd_t;

// HCI_LE_Set_Extended_Advertising_Enable_Ret_t only has one member: Status.

// 7.8.53 LE Set Extended Advertising Parameters command

static const uint16_t LE_Set_Extended_Advertising_Parameters_v2_Cmd = 0x207f; // OGF 0x8 OCF 0x07f

typedef struct HCI_LE_Set_Extended_Advertising_Parameters_v2_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Advertising_Handle;
  uint16_t                    Advertising_Event_Properties;
  uint8_t                     Primary_Advertising_Interval_Min[3];
  uint8_t                     Primary_Advertising_Interval_Max[3];
  uint8_t                     Primary_Advertising_Channel_Map;
  uint8_t                     Own_Addr_Type;
  HCI_Ident_t                 Peer;
  uint8_t                     Advertising_Filter_Policy;
  uint8_t                     Advertising_TX_Power;
  uint8_t                     Primary_Advertising_PHY;
  uint8_t                     Secondary_Advertising_Max_Skip;
  uint8_t                     Secondary_Advertising_PHY;
  uint8_t                     Advertising_SID;
  uint8_t                     Scan_Request_Notification_Enable;
  uint8_t                     Primary_Advertising_PHY_Options;
  uint8_t                     Secondary_Advertising_PHY_Options;
} HCI_LE_Set_Extended_Advertising_Parameters_v2_Cmd_t;

typedef struct HCI_LE_Set_Extended_Advertising_Parameters_v2_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     Selected_TX_Power;
} HCI_LE_Set_Extended_Advertising_Parameters_v2_Ret_t;

static const uint16_t LE_Set_Extended_Advertising_Parameters_v1_Cmd = 0x2036; // OGF 0x8 OCF 0x036

typedef struct HCI_LE_Set_Extended_Advertising_Parameters_v1_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Advertising_Handle;
  uint16_t                    Advertising_Event_Properties;
  uint8_t                     Primary_Advertising_Interval_Min[3];
  uint8_t                     Primary_Advertising_Interval_Max[3];
  uint8_t                     Primary_Advertising_Channel_Map;
  uint8_t                     Own_Addr_Type;
  HCI_Ident_t                 Peer;
  uint8_t                     Advertising_Filter_Policy;
  uint8_t                     Advertising_TX_Power;
  uint8_t                     Primary_Advertising_PHY;
  uint8_t                     Secondary_Advertising_Max_Skip;
  uint8_t                     Secondary_Advertising_PHY;
  uint8_t                     Advertising_SID;
  uint8_t                     Scan_Request_Notification_Enable;
} HCI_LE_Set_Extended_Advertising_Parameters_v1_Cmd_t;

typedef struct HCI_LE_Set_Extended_Advertising_Parameters_v1_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     Selected_TX_Power;
} HCI_LE_Set_Extended_Advertising_Parameters_v1_Ret_t;

// 7.8.65 LE Set Extended Scan Enable command

static const uint16_t LE_Set_Extended_Scan_Enable_Cmd = 0x2042; // OGF 0x8 OCF 0x042

typedef struct HCI_LE_Set_Extended_Scan_Enable_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Enable;
  uint8_t                     Filter_Duplicates;
  uint16_t                    Duration;
  uint16_t                    Period;
} HCI_LE_Set_Extended_Scan_Enable_Cmd_t;

// HCI_LE_Set_Extended_Scan_Enable_Ret_t only has one member: Status.

// 7.8.64 LE Set Extended Scan Parameters command

static const uint16_t LE_Set_Extended_Scan_Parameters_Cmd = 0x2041; // OGF 0x8 OCF 0x041

typedef struct HCI_LE_Set_Extended_Scan_Parameters_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Own_Addr_Type;
  uint8_t                     Scanning_Filter_Policy;
  uint8_t                     Scanning_PHYs;
  struct {
    uint8_t                   Scan_Type;
    uint8_t                   Scan_Interval;
    uint8_t                   Scan_Window;
  } Scanning_PHYs_data[0];
} HCI_LE_Set_Extended_Scan_Parameters_Cmd_t;

// HCI_LE_Set_Extended_Scan_Parameters_Ret_t only has one member: Status.

// 7.8.55 LE Set Extended Scan Response Data command

static const uint16_t LE_Set_Extended_Scan_Response_Data_Cmd = 0x2038; // OGF 0x8 OCF 0x038

typedef struct HCI_LE_Set_Extended_Scan_Response_Data_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Advertising_Handle;
  uint8_t                     Operation;
  uint8_t                     Fragment_Preference;
  uint8_t                     Scan_Response_Data_Length;
  uint8_t                     Scan_Response_Data[0];
} HCI_LE_Set_Extended_Scan_Response_Data_Cmd_t;

// HCI_LE_Set_Extended_Scan_Response_Data_Ret_t only has one member: Status.

// 7.8.19 LE Set Host Channel Classification command

static const uint16_t LE_Set_Host_Channel_Classification_Cmd = 0x2014; // OGF 0x8 OCF 0x014

typedef struct HCI_LE_Set_Host_Channel_Classification_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Channel_Map[5];
} HCI_LE_Set_Host_Channel_Classification_Cmd_t;

// HCI_LE_Set_Host_Channel_Classification_Ret_t only has one member: Status.

// 7.8.115 LE Set Host Feature command

static const uint16_t LE_Set_Host_Feature_Cmd = 0x2074; // OGF 0x8 OCF 0x074

typedef struct HCI_LE_Set_Host_Feature_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Bit_Number;
  uint8_t                     Bit_Value;
} HCI_LE_Set_Host_Feature_Cmd_t;

// HCI_LE_Set_Host_Feature_Ret_t only has one member: Status.

// 7.8.120 LE Set Path Loss Reporting Enable command

static const uint16_t LE_Set_Path_Loss_Reporting_Enable_Cmd = 0x2079; // OGF 0x8 OCF 0x079

typedef struct HCI_LE_Set_Path_Loss_Reporting_Enable_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Conn_Handle;
  uint8_t                     Enable;
} HCI_LE_Set_Path_Loss_Reporting_Enable_Cmd_t;

// HCI_LE_Set_Path_Loss_Reporting_Enable_Ret_t only has one member: Status_Conn.

// 7.8.119 LE Set Path Loss Reporting Parameters command

static const uint16_t LE_Set_Path_Loss_Reporting_Parameters_Cmd = 0x2078; // OGF 0x8 OCF 0x078

typedef struct HCI_LE_Set_Path_Loss_Reporting_Parameters_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Conn_Handle;
  uint8_t                     High_Threshold;
  uint8_t                     High_Hysteresis;
  uint8_t                     Low_Threshold;
  uint8_t                     Low_Hysteresis;
  uint16_t                    Min_Time_Spent;
} HCI_LE_Set_Path_Loss_Reporting_Parameters_Cmd_t;

// HCI_LE_Set_Path_Loss_Reporting_Parameters_Ret_t only has one member: Status_Conn.

// 7.8.62 LE Set Periodic Advertising Data command

static const uint16_t LE_Set_PA_Data_Cmd = 0x203f; // OGF 0x8 OCF 0x03f

typedef struct HCI_LE_Set_PA_Data_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Advertising_Handle;
  uint8_t                     Operation;
  uint8_t                     Advertising_Data_Length;
  uint8_t                     Advertising_Data[0];
} HCI_LE_Set_PA_Data_Cmd_t;

// HCI_LE_Set_PA_Data_Ret_t only has one member: Status.

// 7.8.63 LE Set Periodic Advertising Enable command

static const uint16_t LE_Set_PA_Enable_Cmd = 0x2040; // OGF 0x8 OCF 0x040

typedef struct HCI_LE_Set_PA_Enable_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Enable;
  uint8_t                     Advertising_Handle;
} HCI_LE_Set_PA_Enable_Cmd_t;

// HCI_LE_Set_PA_Enable_Ret_t only has one member: Status.

// 7.8.61 LE Set Periodic Advertising Parameters command

static const uint16_t LE_Set_PA_Parameters_v2_Cmd = 0x2086; // OGF 0x8 OCF 0x086

typedef struct HCI_LE_Set_PA_Parameters_v2_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Advertising_Handle;
  uint16_t                    PA_Interval_Min;
  uint16_t                    PA_Interval_Max;
  uint16_t                    PA_Properties;
  uint8_t                     Num_Subevents;
  uint8_t                     Subevent_Interval;
  uint8_t                     Response_Slot_Delay;
  uint8_t                     Response_Slot_Spacing;
  uint8_t                     Num_Response_Slots;
} HCI_LE_Set_PA_Parameters_v2_Cmd_t;

typedef struct HCI_LE_Set_PA_Parameters_v2_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     Advertising_Handle;
} HCI_LE_Set_PA_Parameters_v2_Ret_t;

static const uint16_t LE_Set_PA_Parameters_v1_Cmd = 0x203e; // OGF 0x8 OCF 0x03e

typedef struct HCI_LE_Set_PA_Parameters_v1_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Advertising_Handle;
  uint16_t                    PA_Interval_Min;
  uint16_t                    PA_Interval_Max;
  uint16_t                    PA_Properties;
} HCI_LE_Set_PA_Parameters_v1_Cmd_t;

// HCI_LE_Set_PA_Parameters_v1_Ret_t only has one member: Status.

// 7.8.88 LE Set Periodic Advertising Receive Enable command

static const uint16_t LE_Set_PA_Receive_Enable_Cmd = 0x2059; // OGF 0x8 OCF 0x059

typedef struct HCI_LE_Set_PA_Receive_Enable_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Sync_Handle;
  uint8_t                     Enable;
} HCI_LE_Set_PA_Receive_Enable_Cmd_t;

// HCI_LE_Set_PA_Receive_Enable_Ret_t only has one member: Status.

// 7.8.126 LE Set Periodic Advertising Response Data command

static const uint16_t LE_Set_PA_Response_Data_Cmd = 0x2083; // OGF 0x8 OCF 0x083

typedef struct HCI_LE_Set_PA_Response_Data_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Sync_Handle;
  uint16_t                    Request_Event;
  uint8_t                     Request_Subevent;
  uint8_t                     Response_Subevent;
  uint8_t                     Response_Slot;
  uint8_t                     Response_Data_Length;
  uint8_t                     Response_Data[0];
} HCI_LE_Set_PA_Response_Data_Cmd_t;

typedef struct HCI_LE_Set_PA_Response_Data_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint16_t                    Sync_Handle;
} HCI_LE_Set_PA_Response_Data_Ret_t;

// 7.8.125 LE Set Periodic Advertising Subevent Data command

static const uint16_t LE_Set_PA_Subevent_Data_Cmd = 0x2082; // OGF 0x8 OCF 0x082

typedef struct HCI_LE_Set_PA_Subevent_Data_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Advertising_Handle;
  uint8_t                     Num_Subevents;
  struct {
    uint8_t                   Subevent;
    uint8_t                   Response_Slot_Start;
    uint8_t                   Response_Slot_Count;
    uint8_t                   Subevent_Data_Length;
    uint8_t  *                Subevent_Data;
  } Subevents[0];
} HCI_LE_Set_PA_Subevent_Data_Cmd_t;

typedef struct HCI_LE_Set_PA_Subevent_Data_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     Advertising_Handle;
} HCI_LE_Set_PA_Subevent_Data_Ret_t;

// 7.8.127 LE Set Periodic Sync Subevent command

static const uint16_t LE_Set_Periodic_Sync_Subevent_Cmd = 0x2084; // OGF 0x8 OCF 0x084

typedef struct HCI_LE_Set_Periodic_Sync_Subevent_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Sync_Handle;
  uint16_t                    PA_Properties;
  uint8_t                     Num_Subevents;
  uint8_t                     Subevent[0];
} HCI_LE_Set_Periodic_Sync_Subevent_Cmd_t;

typedef struct HCI_LE_Set_Periodic_Sync_Subevent_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint16_t                    Sync_Handle;
} HCI_LE_Set_Periodic_Sync_Subevent_Ret_t;

// 7.8.91 LE Set Periodic Advertising Sync Transfer Parameters command

static const uint16_t LE_Set_PAST_Parameters_Cmd = 0x205c; // OGF 0x8 OCF 0x05c

typedef struct HCI_LE_Set_PAST_Parameters_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Conn_Handle;
  uint8_t                     Mode;
  uint16_t                    Skip;
  uint16_t                    Sync_Timeout;
  uint8_t                     CTE_Type;
} HCI_LE_Set_PAST_Parameters_Cmd_t;

// HCI_LE_Set_PAST_Parameters_Ret_t only has one member: Status_Conn.

// 7.8.49 LE Set PHY command

static const uint16_t LE_Set_PHY_Cmd = 0x2032; // OGF 0x8 OCF 0x032

typedef struct HCI_LE_Set_PHY_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Conn_Handle;
  uint8_t                     All_PHYs;
  uint8_t                     TX_PHYs;
  uint8_t                     RX_PHYs;
  uint16_t                    PHY_Options;
} HCI_LE_Set_PHY_Cmd_t;

// 7.8.77 LE Set Privacy Mode command

static const uint16_t LE_Set_Privacy_Mode_Cmd = 0x204e; // OGF 0x8 OCF 0x04e

typedef struct HCI_LE_Set_Privacy_Mode_Cmd_t {
  HCI_Cmd_t                   Cmd;
  HCI_Ident_t                 Peer_Identity;
  uint8_t                     Privacy_Mode;
} HCI_LE_Set_Privacy_Mode_Cmd_t;

// HCI_LE_Set_Privacy_Mode_Ret_t only has one member: Status.

// 7.8.4 LE Set Random Address command

static const uint16_t LE_Set_Random_Addr_Cmd = 0x2005; // OGF 0x8 OCF 0x005

typedef struct HCI_LE_Set_Random_Addr_Cmd_t {
  HCI_Cmd_t                   Cmd;
  BD_Addr_t                   Random_Addr;
} HCI_LE_Set_Random_Addr_Cmd_t;

// HCI_LE_Set_Random_Addr_Ret_t only has one member: Status.

// 7.8.45 LE Set Resolvable Private Address Timeout command

static const uint16_t LE_Set_Resolvable_Private_Addr_Timeout_Cmd = 0x202e; // OGF 0x8 OCF 0x02e

typedef struct HCI_LE_Set_Resolvable_Private_Addr_Timeout_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    RPA_Timeout;
} HCI_LE_Set_Resolvable_Private_Addr_Timeout_Cmd_t;

// HCI_LE_Set_Resolvable_Private_Addr_Timeout_Ret_t only has one member: Status.

// 7.8.11 LE Set Scan Enable command

static const uint16_t LE_Set_Scan_Enable_Cmd = 0x200c; // OGF 0x8 OCF 0x00c

typedef struct HCI_LE_Set_Scan_Enable_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     LE_Scan_Enable;
  uint8_t                     Filter_Duplicates;
} HCI_LE_Set_Scan_Enable_Cmd_t;

// HCI_LE_Set_Scan_Enable_Ret_t only has one member: Status.

// 7.8.10 LE Set Scan Parameters command

static const uint16_t LE_Set_Scan_Parameters_Cmd = 0x200b; // OGF 0x8 OCF 0x00b

typedef struct HCI_LE_Set_Scan_Parameters_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     LE_Scan_Type;
  uint16_t                    LE_Scan_Interval;
  uint16_t                    LE_Scan_Window;
  uint8_t                     Own_Addr_Type;
  uint8_t                     Scanning_Filter_Policy;
} HCI_LE_Set_Scan_Parameters_Cmd_t;

// HCI_LE_Set_Scan_Parameters_Ret_t only has one member: Status.

// 7.8.8 LE Set Scan Response Data command

static const uint16_t LE_Set_Scan_Response_Data_Cmd = 0x2009; // OGF 0x8 OCF 0x009

typedef struct HCI_LE_Set_Scan_Response_Data_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Scan_Response_Data_Length;
  uint8_t                     Scan_Response_Data[31];
} HCI_LE_Set_Scan_Response_Data_Cmd_t;

// HCI_LE_Set_Scan_Response_Data_Ret_t only has one member: Status.

// 7.8.121 LE Set Transmit Power Reporting Enable command

static const uint16_t LE_Set_Transmit_Power_Reporting_Enable_Cmd = 0x207a; // OGF 0x8 OCF 0x07a

typedef struct HCI_LE_Set_Transmit_Power_Reporting_Enable_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Conn_Handle;
  uint8_t                     Local_Enable;
  uint8_t                     Remote_Enable;
} HCI_LE_Set_Transmit_Power_Reporting_Enable_Cmd_t;

// HCI_LE_Set_Transmit_Power_Reporting_Enable_Ret_t only has one member: Status_Conn.

// 7.8.109 LE Setup ISO Data Path command

static const uint16_t LE_Setup_ISO_Data_Path_Cmd = 0x206e; // OGF 0x8 OCF 0x06e

typedef struct HCI_LE_Setup_ISO_Data_Path_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Conn_Handle;
  uint8_t                     Data_Path_Direction;
  uint8_t                     Data_Path_ID;
  uint8_t                     Codec_ID[5];
  uint8_t                     Controller_Delay[3];
  uint8_t                     Codec_Configuration_Length;
  uint8_t                     Codec_Configuration[0];
} HCI_LE_Setup_ISO_Data_Path_Cmd_t;

// HCI_LE_Setup_ISO_Data_Path_Ret_t only has one member: Status_Conn.

// 7.7.65.35 LE Subrate Change event

static const uint8_t LE_Subrate_Change_Evt_sub = 0x23;

typedef struct HCI_LE_Subrate_Change_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Subevent_Code;
  uint8_t                     Status;
  uint16_t                    Conn_Handle;
  uint16_t                    Subrate_Factor;
  uint16_t                    Peripheral_Latency;
  uint16_t                    Continuation_Number;
  uint16_t                    Supervision_Timeout;
} HCI_LE_Subrate_Change_Evt_t;

// 7.8.124 LE Subrate Request command

static const uint16_t LE_Subrate_Request_Cmd = 0x207e; // OGF 0x8 OCF 0x07e

typedef struct HCI_LE_Subrate_Request_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Conn_Handle;
  uint16_t                    Subrate_Min;
  uint16_t                    Subrate_Max;
  uint16_t                    Max_Latency;
  uint16_t                    Continuation_Number;
  uint16_t                    Supervision_Timeout;
} HCI_LE_Subrate_Request_Cmd_t;

// 7.8.105 LE Terminate BIG command

static const uint16_t LE_Terminate_BIG_Cmd = 0x206a; // OGF 0x8 OCF 0x06a

typedef struct HCI_LE_Terminate_BIG_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     BIG_Handle;
  uint8_t                     Reason;
} HCI_LE_Terminate_BIG_Cmd_t;

// 7.7.65.28 LE Terminate BIG Complete event

static const uint8_t LE_Terminate_BIG_Complete_Evt_sub = 0x1c;

typedef struct HCI_LE_Terminate_BIG_Complete_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Subevent_Code;
  uint8_t                     BIG_Handle;
  uint8_t                     Reason;
} HCI_LE_Terminate_BIG_Complete_Evt_t;

// 7.8.30 LE Test End command

static const uint16_t LE_Test_End_Cmd = 0x201f; // OGF 0x8 OCF 0x01f

typedef struct HCI_LE_Test_End_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint16_t                    Num_Packets;
} HCI_LE_Test_End_Ret_t;

// 7.7.65.33 LE Transmit Power Reporting event

static const uint8_t LE_Transmit_Power_Reporting_Evt_sub = 0x21;

typedef struct HCI_LE_Transmit_Power_Reporting_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Subevent_Code;
  uint8_t                     Status;
  uint16_t                    Conn_Handle;
  uint8_t                     Reason;
  uint8_t                     PHY;
  uint8_t                     TX_Power_Level;
  uint8_t                     TX_Power_Level_Flag;
  uint8_t                     Delta;
} HCI_LE_Transmit_Power_Reporting_Evt_t;

// 7.8.29 LE Transmitter Test command

static const uint16_t LE_Transmitter_Test_v4_Cmd = 0x207b; // OGF 0x8 OCF 0x07b

typedef struct HCI_LE_Transmitter_Test_v4_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     TX_Channel;
  uint8_t                     Test_Data_Length;
  uint8_t                     Packet_Payload;
  uint8_t                     PHY;
  uint8_t                     CTE_Length;
  uint8_t                     CTE_Type;
  uint8_t                     Switching_Pattern_Length;
  uint8_t  *                  Antenna_IDs;
  uint8_t                     TX_Power_Level;
} HCI_LE_Transmitter_Test_v4_Cmd_t;

// HCI_LE_Transmitter_Test_v4_Ret_t only has one member: Status.

static const uint16_t LE_Transmitter_Test_v3_Cmd = 0x2050; // OGF 0x8 OCF 0x050

typedef struct HCI_LE_Transmitter_Test_v3_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     TX_Channel;
  uint8_t                     Test_Data_Length;
  uint8_t                     Packet_Payload;
  uint8_t                     PHY;
  uint8_t                     CTE_Length;
  uint8_t                     CTE_Type;
  uint8_t                     Switching_Pattern_Length;
  uint8_t                     Antenna_IDs[0];
} HCI_LE_Transmitter_Test_v3_Cmd_t;

// HCI_LE_Transmitter_Test_v3_Ret_t only has one member: Status.

static const uint16_t LE_Transmitter_Test_v2_Cmd = 0x2034; // OGF 0x8 OCF 0x034

typedef struct HCI_LE_Transmitter_Test_v2_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     TX_Channel;
  uint8_t                     Test_Data_Length;
  uint8_t                     Packet_Payload;
  uint8_t                     PHY;
} HCI_LE_Transmitter_Test_v2_Cmd_t;

// HCI_LE_Transmitter_Test_v2_Ret_t only has one member: Status.

static const uint16_t LE_Transmitter_Test_v1_Cmd = 0x201e; // OGF 0x8 OCF 0x01e

typedef struct HCI_LE_Transmitter_Test_v1_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     TX_Channel;
  uint8_t                     Test_Data_Length;
  uint8_t                     Packet_Payload;
} HCI_LE_Transmitter_Test_v1_Cmd_t;

// HCI_LE_Transmitter_Test_v1_Ret_t only has one member: Status.

// 7.8.76 LE Write RF Path Compensation command

static const uint16_t LE_Write_RF_Path_Compensation_Cmd = 0x204d; // OGF 0x8 OCF 0x04d

typedef struct HCI_LE_Write_RF_Path_Compensation_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    RF_TX_Path_Compensation_Value;
  uint16_t                    RF_RX_Path_Compensation_Value;
} HCI_LE_Write_RF_Path_Compensation_Cmd_t;

// HCI_LE_Write_RF_Path_Compensation_Ret_t only has one member: Status.

// 7.8.35 LE Write Suggested Default Data Length command

static const uint16_t LE_Write_Suggested_Default_Data_Length_Cmd = 0x2024; // OGF 0x8 OCF 0x024

typedef struct HCI_LE_Write_Suggested_Default_Data_Length_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Suggested_Max_TX_Octets;
  uint16_t                    Suggested_Max_TX_Time;
} HCI_LE_Write_Suggested_Default_Data_Length_Cmd_t;

// HCI_LE_Write_Suggested_Default_Data_Length_Ret_t only has one member: Status.

// 7.7.24 Link Key Notification event

static const uint8_t Link_Key_Notification_Evt = 0x18;

typedef struct HCI_Link_Key_Notification_Evt_t {
  HCI_Evt_t                   Evt;
  BD_Addr_t                   Addr;
  uint8_t                     Link_Key[16];
  uint8_t                     Key_Type;
} HCI_Link_Key_Notification_Evt_t;

// 7.7.23 Link Key Request event

static const uint8_t Link_Key_Request_Evt = 0x17;

typedef struct HCI_Link_Key_Request_Evt_t {
  HCI_Evt_t                   Evt;
  BD_Addr_t                   Addr;
} HCI_Link_Key_Request_Evt_t;

// 7.1.11 Link Key Request Negative Reply command

static const uint16_t Link_Key_Request_Negative_Reply_Cmd = 0x040c; // OGF 0x1 OCF 0x00c

typedef struct HCI_Link_Key_Request_Negative_Reply_Cmd_t {
  HCI_Cmd_t                   Cmd;
  BD_Addr_t                   Addr;
} HCI_Link_Key_Request_Negative_Reply_Cmd_t;

// HCI_Link_Key_Request_Negative_Reply_Ret_t only has one member: Status_BD_Addr.

// 7.1.10 Link Key Request Reply command

static const uint16_t Link_Key_Request_Reply_Cmd = 0x040b; // OGF 0x1 OCF 0x00b

typedef struct HCI_Link_Key_Request_Reply_Cmd_t {
  HCI_Cmd_t                   Cmd;
  BD_Addr_t                   Addr;
  uint8_t                     Link_Key[16];
} HCI_Link_Key_Request_Reply_Cmd_t;

// HCI_Link_Key_Request_Reply_Ret_t only has one member: Status_BD_Addr.

// 7.1.18 Link Key Selection command

static const uint16_t Link_Key_Selection_Cmd = 0x0417; // OGF 0x1 OCF 0x017

typedef struct HCI_Link_Key_Selection_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Key_Flag;
} HCI_Link_Key_Selection_Cmd_t;

// 7.7.10 Link Key Type Changed event

static const uint8_t Link_Key_Type_Changed_Evt = 0x0a;

typedef struct HCI_Link_Key_Type_Changed_Evt_t {
  HCI_Evt_t                   Evt;
  Status_Conn_Handle_t        Status_Conn;
  uint8_t                     Key_Flag;
} HCI_Link_Key_Type_Changed_Evt_t;

// 7.7.46 Link Supervision Timeout Changed event

static const uint8_t Link_Supervision_Timeout_Changed_Evt = 0x38;

typedef struct HCI_Link_Supervision_Timeout_Changed_Evt_t {
  HCI_Evt_t                   Evt;
  uint16_t                    Conn_Handle;
  uint16_t                    Link_Supervision_Timeout;
} HCI_Link_Supervision_Timeout_Changed_Evt_t;

// 7.7.25 Loopback Command event

static const uint8_t Loopback_Command_Evt = 0x19;

typedef struct HCI_Loopback_Command_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     HCI_Command_Packet;
} HCI_Loopback_Command_Evt_t;

// 7.7.27 Max Slots Change event

static const uint8_t Max_Slots_Change_Evt = 0x1b;

typedef struct HCI_Max_Slots_Change_Evt_t {
  HCI_Evt_t                   Evt;
  uint16_t                    Conn_Handle;
  uint8_t                     LMP_Max_Slots;
} HCI_Max_Slots_Change_Evt_t;

// 7.7.20 Mode Change event

static const uint8_t Mode_Change_Evt = 0x14;

typedef struct HCI_Mode_Change_Evt_t {
  HCI_Evt_t                   Evt;
  Status_Conn_Handle_t        Status_Conn;
  uint8_t                     Current_Mode;
  uint16_t                    Interval;
} HCI_Mode_Change_Evt_t;

// 7.7.59 Number Of Completed Data Blocks event

static const uint8_t Number_Of_Completed_Data_Blocks_Evt = 0x48;

typedef struct HCI_Number_Of_Completed_Data_Blocks_Evt_t {
  HCI_Evt_t                   Evt;
  uint16_t                    Total_Num_Data_Blocks;
  uint8_t                     Num_Handles;
  struct {
    uint16_t                  Conn_Handle;
    uint16_t                  Num_Completed_Packets;
    uint16_t                  Num_Completed_Blocks;
  } Handles[0];
} HCI_Number_Of_Completed_Data_Blocks_Evt_t;

// 7.7.19 Number Of Completed Packets event

static const uint8_t Number_Of_Completed_Packets_Evt = 0x13;

typedef struct HCI_Number_Of_Completed_Packets_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Num_Handles;
  struct {
    uint16_t                  Conn_Handle;
    uint16_t                  Num_Completed_Packets;
  } Handles[0];
} HCI_Number_Of_Completed_Packets_Evt_t;

// 7.7.31 Page Scan Repetition Mode Change event

static const uint8_t Page_Scan_Repetition_Mode_Change_Evt = 0x20;

typedef struct HCI_Page_Scan_Repetition_Mode_Change_Evt_t {
  HCI_Evt_t                   Evt;
  BD_Addr_t                   Addr;
  uint8_t                     Page_Scan_Repetition_Mode;
} HCI_Page_Scan_Repetition_Mode_Change_Evt_t;

// 7.1.3 Periodic Inquiry Mode command

static const uint16_t Periodic_Inquiry_Mode_Cmd = 0x0403; // OGF 0x1 OCF 0x003

typedef struct HCI_Periodic_Inquiry_Mode_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Max_Period_Length;
  uint16_t                    Min_Period_Length;
  uint8_t                     LAP[3];
  uint8_t                     Inquiry_Length;
  uint8_t                     Num_Responses;
} HCI_Periodic_Inquiry_Mode_Cmd_t;

// HCI_Periodic_Inquiry_Mode_Ret_t only has one member: Status.

// 7.7.22 PIN Code Request event

static const uint8_t PIN_Code_Request_Evt = 0x16;

typedef struct HCI_PIN_Code_Request_Evt_t {
  HCI_Evt_t                   Evt;
  BD_Addr_t                   Addr;
} HCI_PIN_Code_Request_Evt_t;

// 7.1.13 PIN Code Request Negative Reply command

static const uint16_t PIN_Code_Request_Negative_Reply_Cmd = 0x040e; // OGF 0x1 OCF 0x00e

typedef struct HCI_PIN_Code_Request_Negative_Reply_Cmd_t {
  HCI_Cmd_t                   Cmd;
  BD_Addr_t                   Addr;
} HCI_PIN_Code_Request_Negative_Reply_Cmd_t;

// HCI_PIN_Code_Request_Negative_Reply_Ret_t only has one member: Status_BD_Addr.

// 7.1.12 PIN Code Request Reply command

static const uint16_t PIN_Code_Request_Reply_Cmd = 0x040d; // OGF 0x1 OCF 0x00d

typedef struct HCI_PIN_Code_Request_Reply_Cmd_t {
  HCI_Cmd_t                   Cmd;
  BD_Addr_t                   Addr;
  uint8_t                     PIN_Code_Length;
  uint8_t                     PIN_Code[16];
} HCI_PIN_Code_Request_Reply_Cmd_t;

// HCI_PIN_Code_Request_Reply_Ret_t only has one member: Status_BD_Addr.

// 7.2.6 QoS Setup command

typedef struct HCI_QoS_t {                     // Common
  uint8_t                     Unused;
  uint8_t                     Service_Type;
  uint32_t                    Token_Rate;
  uint32_t                    Peak_Bandwidth;
  uint32_t                    Latency;
  uint32_t                    Delay_Variation;
} HCI_QoS_t;

static const uint16_t QoS_Setup_Cmd = 0x0807; // OGF 0x2 OCF 0x007

typedef struct HCI_QoS_Setup_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Conn_Handle;
  HCI_QoS_t                   QoS;
} HCI_QoS_Setup_Cmd_t;

// 7.7.13 QoS Setup Complete event

static const uint8_t QoS_Setup_Complete_Evt = 0x0d;

typedef struct HCI_QoS_Setup_Complete_Evt_t {
  HCI_Evt_t                   Evt;
  Status_Conn_Handle_t        Status_Conn;
  HCI_QoS_t                   QoS;
} HCI_QoS_Setup_Complete_Evt_t;

// 7.7.30 QoS Violation event

static const uint8_t QoS_Violation_Evt = 0x1e;

typedef struct HCI_QoS_Violation_Evt_t {
  HCI_Evt_t                   Evt;
  uint16_t                    Handle;
} HCI_QoS_Violation_Evt_t;

// 7.3.53 Read AFH Channel Assessment Mode command

static const uint16_t Read_AFH_Channel_Assessment_Mode_Cmd = 0x0c48; // OGF 0x3 OCF 0x048

typedef struct HCI_Read_AFH_Channel_Assessment_Mode_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     AFH_Channel_Assessment_Mode;
} HCI_Read_AFH_Channel_Assessment_Mode_Ret_t;

// 7.5.5 Read AFH Channel Map command

// HCI_Read_AFH_Channel_Map_Cmd_t only has one member: Connection_Handle.

static const uint16_t Read_AFH_Channel_Map_Cmd = 0x1406; // OGF 0x5 OCF 0x006

typedef struct HCI_Read_AFH_Channel_Map_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  Status_Conn_Handle_t        Status_Conn;
  uint8_t                     AFH_Mode;
  uint8_t                     AFH_Channel_Map[10];
} HCI_Read_AFH_Channel_Map_Ret_t;

// 7.3.93 Read Authenticated Payload Timeout command

// HCI_Read_Authenticated_Payload_Timeout_Cmd_t only has one member: Connection_Handle.

static const uint16_t Read_Authenticated_Payload_Timeout_Cmd = 0x0c7b; // OGF 0x3 OCF 0x07b

typedef struct HCI_Read_Authenticated_Payload_Timeout_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  Status_Conn_Handle_t        Status_Conn;
  uint16_t                    Authenticated_Payload_Timeout;
} HCI_Read_Authenticated_Payload_Timeout_Ret_t;

// 7.3.23 Read Authentication Enable command

static const uint16_t Read_Authentication_Enable_Cmd = 0x0c1f; // OGF 0x3 OCF 0x01f

typedef struct HCI_Read_Authentication_Enable_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     Authentication_Enable;
} HCI_Read_Authentication_Enable_Ret_t;

// 7.3.29 Read Automatic Flush Timeout command

// HCI_Read_Automatic_Flush_Timeout_Cmd_t only has one member: Connection_Handle.

static const uint16_t Read_Automatic_Flush_Timeout_Cmd = 0x0c27; // OGF 0x3 OCF 0x027

typedef struct HCI_Read_Automatic_Flush_Timeout_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  Status_Conn_Handle_t        Status_Conn;
  uint16_t                    Flush_Timeout;
} HCI_Read_Automatic_Flush_Timeout_Ret_t;

// 7.4.6 Read BD_ADDR command

// HCI_Read_BD_ADDR_Ret_t only has one member: Status_BD_Addr.

static const uint16_t Read_BD_ADDR_Cmd = 0x1009; // OGF 0x4 OCF 0x009

// 7.4.5 Read Buffer Size command

static const uint16_t Read_Buffer_Size_Cmd = 0x1005; // OGF 0x4 OCF 0x005

typedef struct HCI_Read_Buffer_Size_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint16_t                    ACL_Data_Packet_Length;
  uint8_t                     Synchronous_Data_Packet_Length;
  uint16_t                    Total_Num_ACL_Data_Packets;
  uint16_t                    Total_Num_Synchronous_Data_Packets;
} HCI_Read_Buffer_Size_Ret_t;

// 7.3.25 Read Class of Device command

static const uint16_t Read_Class_Of_Device_Cmd = 0x0c23; // OGF 0x3 OCF 0x023

typedef struct HCI_Read_Class_Of_Device_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     Class_Of_Device[3];
} HCI_Read_Class_Of_Device_Ret_t;

// 7.5.6 Read Clock command

static const uint16_t Read_Clock_Cmd = 0x1407; // OGF 0x5 OCF 0x007

typedef struct HCI_Read_Clock_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Conn_Handle;
  uint8_t                     Which_Clock;
} HCI_Read_Clock_Cmd_t;

typedef struct HCI_Read_Clock_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  Status_Conn_Handle_t        Status_Conn;
  uint32_t                    Clock;
  uint16_t                    Accuracy;
} HCI_Read_Clock_Ret_t;

// 7.1.24 Read Clock Offset command

// HCI_Read_Clock_Offset_Cmd_t only has one member: Connection_Handle.

static const uint16_t Read_Clock_Offset_Cmd = 0x041f; // OGF 0x1 OCF 0x01f

// 7.7.28 Read Clock Offset Complete event

static const uint8_t Read_Clock_Offset_Complete_Evt = 0x1c;

typedef struct HCI_Read_Clock_Offset_Complete_Evt_t {
  HCI_Evt_t                   Evt;
  Status_Conn_Handle_t        Status_Conn;
  uint16_t                    Clock_Offset;
} HCI_Read_Clock_Offset_Complete_Evt_t;

// 7.3.13 Read Connection Accept Timeout command

static const uint16_t Read_Connection_Accept_Timeout_Cmd = 0x0c15; // OGF 0x3 OCF 0x015

typedef struct HCI_Read_Connection_Accept_Timeout_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint16_t                    Connection_Accept_Timeout;
} HCI_Read_Connection_Accept_Timeout_Ret_t;

// 7.3.44 Read Current IAC LAP command

static const uint16_t Read_Current_IAC_LAP_Cmd = 0x0c39; // OGF 0x3 OCF 0x039

typedef struct HCI_Read_Current_IAC_LAP_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     Num_Current_IAC;
  uint8_t                     IAC_LAP[3];
} HCI_Read_Current_IAC_LAP_Ret_t;

// 7.4.7 Read Data Block Size command

static const uint16_t Read_Data_Block_Size_Cmd = 0x100a; // OGF 0x4 OCF 0x00a

typedef struct HCI_Read_Data_Block_Size_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint16_t                    Max_ACL_Data_Packet_Length;
  uint16_t                    Data_Block_Length;
  uint16_t                    Total_Num_Data_Blocks;
} HCI_Read_Data_Block_Size_Ret_t;

// 7.3.64 Read Default Erroneous Data Reporting command

static const uint16_t Read_Default_Erroneous_Data_Reporting_Cmd = 0x0c5a; // OGF 0x3 OCF 0x05a

typedef struct HCI_Read_Default_Erroneous_Data_Reporting_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     Erroneous_Data_Reporting;
} HCI_Read_Default_Erroneous_Data_Reporting_Ret_t;

// 7.2.11 Read Default Link Policy Settings command

static const uint16_t Read_Default_Link_Policy_Settings_Cmd = 0x080e; // OGF 0x2 OCF 0x00e

typedef struct HCI_Read_Default_Link_Policy_Settings_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint16_t                    Default_Link_Policy_Settings;
} HCI_Read_Default_Link_Policy_Settings_Ret_t;

// 7.5.7 Read Encryption Key Size command

// HCI_Read_Encryption_Key_Size_Cmd_t only has one member: Connection_Handle.

static const uint16_t Read_Encryption_Key_Size_Cmd = 0x1408; // OGF 0x5 OCF 0x008

typedef struct HCI_Read_Encryption_Key_Size_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  Status_Conn_Handle_t        Status_Conn;
  uint8_t                     Key_Size;
} HCI_Read_Encryption_Key_Size_Ret_t;

// 7.3.74 Read Enhanced Transmit Power Level command

static const uint16_t Read_Enhanced_Transmit_Power_Level_Cmd = 0x0c68; // OGF 0x3 OCF 0x068

typedef struct HCI_Read_Enhanced_Transmit_Power_Level_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Conn_Handle;
  uint8_t                     Type;
} HCI_Read_Enhanced_Transmit_Power_Level_Cmd_t;

typedef struct HCI_Read_Enhanced_Transmit_Power_Level_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  Status_Conn_Handle_t        Status_Conn;
  uint8_t                     TX_Power_Level_GFSK;
  uint8_t                     TX_Power_Level_DQPSK;
  uint8_t                     TX_Power_Level_8DPSK;
} HCI_Read_Enhanced_Transmit_Power_Level_Ret_t;

// 7.3.98 Read Extended Inquiry Length command

static const uint16_t Read_Extended_Inquiry_Length_Cmd = 0x0c80; // OGF 0x3 OCF 0x080

typedef struct HCI_Read_Extended_Inquiry_Length_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint16_t                    Extended_Inquiry_Length;
} HCI_Read_Extended_Inquiry_Length_Ret_t;

// 7.3.55 Read Extended Inquiry Response command

static const uint16_t Read_Extended_Inquiry_Response_Cmd = 0x0c51; // OGF 0x3 OCF 0x051

typedef struct HCI_Read_Extended_Inquiry_Response_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     FEC_Required;
  uint8_t                     Extended_Inquiry_Response[240];
} HCI_Read_Extended_Inquiry_Response_Ret_t;

// 7.3.96 Read Extended Page Timeout command

static const uint16_t Read_Extended_Page_Timeout_Cmd = 0x0c7e; // OGF 0x3 OCF 0x07e

typedef struct HCI_Read_Extended_Page_Timeout_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint16_t                    Extended_Page_Timeout;
} HCI_Read_Extended_Page_Timeout_Ret_t;

// 7.5.1 Read Failed Contact Counter command

static const uint16_t Read_Failed_Contact_Counter_Cmd = 0x1401; // OGF 0x5 OCF 0x001

typedef struct HCI_Read_Failed_Contact_Counter_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Handle;
} HCI_Read_Failed_Contact_Counter_Cmd_t;

typedef struct HCI_Read_Failed_Contact_Counter_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint16_t                    Handle;
  uint16_t                    Failed_Contact_Counter;
} HCI_Read_Failed_Contact_Counter_Ret_t;

// 7.3.72 Read Flow Control Mode command

static const uint16_t Read_Flow_Control_Mode_Cmd = 0x0c66; // OGF 0x3 OCF 0x066

typedef struct HCI_Read_Flow_Control_Mode_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     Flow_Control_Mode;
} HCI_Read_Flow_Control_Mode_Ret_t;

// 7.3.33 Read Hold Mode Activity command

static const uint16_t Read_Hold_Mode_Activity_Cmd = 0x0c2b; // OGF 0x3 OCF 0x02b

typedef struct HCI_Read_Hold_Mode_Activity_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     Hold_Mode_Activity;
} HCI_Read_Hold_Mode_Activity_Ret_t;

// 7.3.49 Read Inquiry Mode command

static const uint16_t Read_Inquiry_Mode_Cmd = 0x0c44; // OGF 0x3 OCF 0x044

typedef struct HCI_Read_Inquiry_Mode_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     Inquiry_Mode;
} HCI_Read_Inquiry_Mode_Ret_t;

// 7.3.61 Read Inquiry Response Transmit Power Level command

static const uint16_t Read_Inquiry_Response_Transmit_Power_Level_Cmd = 0x0c58; // OGF 0x3 OCF 0x058

typedef struct HCI_Read_Inquiry_Response_Transmit_Power_Level_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     TX_Power;
} HCI_Read_Inquiry_Response_Transmit_Power_Level_Ret_t;

// 7.3.21 Read Inquiry Scan Activity command

static const uint16_t Read_Inquiry_Scan_Activity_Cmd = 0x0c1d; // OGF 0x3 OCF 0x01d

typedef struct HCI_Read_Inquiry_Scan_Activity_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint16_t                    Inquiry_Scan_Interval;
  uint16_t                    Inquiry_Scan_Window;
} HCI_Read_Inquiry_Scan_Activity_Ret_t;

// 7.3.47 Read Inquiry Scan Type command

static const uint16_t Read_Inquiry_Scan_Type_Cmd = 0x0c42; // OGF 0x3 OCF 0x042

typedef struct HCI_Read_Inquiry_Scan_Type_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     Inquiry_Scan_Type;
} HCI_Read_Inquiry_Scan_Type_Ret_t;

// 7.3.78 Read LE Host Support command

static const uint16_t Read_LE_Host_Support_Cmd = 0x0c6c; // OGF 0x3 OCF 0x06c

typedef struct HCI_Read_LE_Host_Support_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     LE_Supported_Host;
  uint8_t                     Unused;
} HCI_Read_LE_Host_Support_Ret_t;

// 7.2.9 Read Link Policy Settings command

// HCI_Read_Link_Policy_Settings_Cmd_t only has one member: Connection_Handle.

static const uint16_t Read_Link_Policy_Settings_Cmd = 0x080c; // OGF 0x2 OCF 0x00c

typedef struct HCI_Read_Link_Policy_Settings_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  Status_Conn_Handle_t        Status_Conn;
  uint16_t                    Link_Policy_Settings;
} HCI_Read_Link_Policy_Settings_Ret_t;

// 7.5.3 Read Link Quality command

static const uint16_t Read_Link_Quality_Cmd = 0x1403; // OGF 0x5 OCF 0x003

typedef struct HCI_Read_Link_Quality_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Handle;
} HCI_Read_Link_Quality_Cmd_t;

typedef struct HCI_Read_Link_Quality_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint16_t                    Handle;
  uint8_t                     Link_Quality;
} HCI_Read_Link_Quality_Ret_t;

// 7.3.41 Read Link Supervision Timeout command

static const uint16_t Read_Link_Supervision_Timeout_Cmd = 0x0c36; // OGF 0x3 OCF 0x036

typedef struct HCI_Read_Link_Supervision_Timeout_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Handle;
} HCI_Read_Link_Supervision_Timeout_Cmd_t;

typedef struct HCI_Read_Link_Supervision_Timeout_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint16_t                    Handle;
  uint16_t                    Link_Supervision_Timeout;
} HCI_Read_Link_Supervision_Timeout_Ret_t;

// 7.1.25 Read LMP Handle command

// HCI_Read_LMP_Handle_Cmd_t only has one member: Connection_Handle.

static const uint16_t Read_LMP_Handle_Cmd = 0x0420; // OGF 0x1 OCF 0x020

typedef struct HCI_Read_LMP_Handle_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  Status_Conn_Handle_t        Status_Conn;
  uint8_t                     LMP_Handle;
  uint32_t                    Reserved;
} HCI_Read_LMP_Handle_Ret_t;

// 7.4.4 Read Local Extended Features command

static const uint16_t Read_Local_Extended_Features_Cmd = 0x1004; // OGF 0x4 OCF 0x004

typedef struct HCI_Read_Local_Extended_Features_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Page_Number;
} HCI_Read_Local_Extended_Features_Cmd_t;

typedef struct HCI_Read_Local_Extended_Features_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     Page_Number;
  uint8_t                     Max_Page_Number;
  uint8_t                     Extended_LMP_Features[8];
} HCI_Read_Local_Extended_Features_Ret_t;

// 7.3.12 Read Local Name command

static const uint16_t Read_Local_Name_Cmd = 0x0c14; // OGF 0x3 OCF 0x014

typedef struct HCI_Read_Local_Name_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     Local_Name[248];
} HCI_Read_Local_Name_Ret_t;

// 7.3.60 Read Local OOB Data command

static const uint16_t Read_Local_OOB_Data_Cmd = 0x0c57; // OGF 0x3 OCF 0x057

typedef struct HCI_Read_Local_OOB_Data_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     C[16];
  uint8_t                     R[16];
} HCI_Read_Local_OOB_Data_Ret_t;

// 7.3.95 Read Local OOB Extended Data command

static const uint16_t Read_Local_OOB_Extended_Data_Cmd = 0x0c7d; // OGF 0x3 OCF 0x07d

typedef struct HCI_Read_Local_OOB_Extended_Data_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     C_192[16];
  uint8_t                     R_192[16];
  uint8_t                     C_256[16];
  uint8_t                     R_256[16];
} HCI_Read_Local_OOB_Extended_Data_Ret_t;

// 7.4.9 Read Local Simple Pairing Options command

static const uint16_t Read_Local_Simple_Pairing_Options_Cmd = 0x100c; // OGF 0x4 OCF 0x00c

typedef struct HCI_Read_Local_Simple_Pairing_Options_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     Simple_Pairing_Options;
  uint8_t                     Max_Encryption_Key_Size;
} HCI_Read_Local_Simple_Pairing_Options_Ret_t;

// 7.4.10 Read Local Supported Codec Capabilities command

static const uint16_t Read_Local_Supported_Codec_Capabilities_Cmd = 0x100e; // OGF 0x4 OCF 0x00e

typedef struct HCI_Read_Local_Supported_Codec_Capabilities_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Codec_ID[5];
  uint8_t                     Logical_Transport_Type;
  uint8_t                     Direction;
} HCI_Read_Local_Supported_Codec_Capabilities_Cmd_t;

typedef struct HCI_Read_Local_Supported_Codec_Capabilities_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     Num_Codec_Capabilities;
  struct {
    uint8_t                   Codec_Capability_Length;
    uint8_t  *                Codec_Capability;
  } Codec_Capabilities[0];
} HCI_Read_Local_Supported_Codec_Capabilities_Ret_t;

// 7.4.8 Read Local Supported Codecs command

static const uint16_t Read_Local_Supported_Codecs_v2_Cmd = 0x100d; // OGF 0x4 OCF 0x00d

typedef struct HCI_Read_Local_Supported_Codecs_v2_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     Num_Supported_Standard_Codecs;
  struct {
    uint8_t                   Standard_Codec_ID;
    uint8_t                   Standard_Codec_Transport;
  } * Supported_Standard_Codecs;
  uint8_t                     Num_Supported_Vendor_Specific_Codecs;
  uint8_t                     Vendor_Specific_Codec_ID;
  uint8_t                     Vendor_Specific_Codec_Transport;
} HCI_Read_Local_Supported_Codecs_v2_Ret_t;

static const uint16_t Read_Local_Supported_Codecs_v1_Cmd = 0x100b; // OGF 0x4 OCF 0x00b

typedef struct HCI_Read_Local_Supported_Codecs_v1_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     Num_Supported_Standard_Codecs;
  uint8_t  *                  Standard_Codec_ID;
  uint8_t                     Num_Supported_Vendor_Specific_Codecs;
  uint8_t                     Vendor_Specific_Codec_ID;
} HCI_Read_Local_Supported_Codecs_v1_Ret_t;

// 7.4.2 Read Local Supported Commands command

static const uint16_t Read_Local_Supported_Commands_Cmd = 0x1002; // OGF 0x4 OCF 0x002

typedef struct HCI_Read_Local_Supported_Commands_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     Supported_Commands[64];
} HCI_Read_Local_Supported_Commands_Ret_t;

// 7.4.11 Read Local Supported Controller Delay command

static const uint16_t Read_Local_Supported_Controller_Delay_Cmd = 0x100f; // OGF 0x4 OCF 0x00f

typedef struct HCI_Read_Local_Supported_Controller_Delay_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Codec_ID[5];
  uint8_t                     Logical_Transport_Type;
  uint8_t                     Direction;
  uint8_t                     Codec_Configuration_Length;
  uint8_t                     Codec_Configuration[0];
} HCI_Read_Local_Supported_Controller_Delay_Cmd_t;

typedef struct HCI_Read_Local_Supported_Controller_Delay_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     Min_Controller_Delay[3];
  uint8_t                     Max_Controller_Delay[3];
} HCI_Read_Local_Supported_Controller_Delay_Ret_t;

// 7.4.3 Read Local Supported Features command

static const uint16_t Read_Local_Supported_Features_Cmd = 0x1003; // OGF 0x4 OCF 0x003

typedef struct HCI_Read_Local_Supported_Features_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     LMP_Features[8];
} HCI_Read_Local_Supported_Features_Ret_t;

// 7.4.1 Read Local Version Information command

static const uint16_t Read_Local_Version_Information_Cmd = 0x1001; // OGF 0x4 OCF 0x001

typedef struct HCI_Read_Local_Version_Information_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     HCI_Version;
  uint16_t                    HCI_Subversion;
  uint8_t                     LMP_Version;
  uint16_t                    Company_Identifier;
  uint16_t                    LMP_Subversion;
} HCI_Read_Local_Version_Information_Ret_t;

// 7.6.1 Read Loopback Mode command

static const uint16_t Read_Loopback_Mode_Cmd = 0x1801; // OGF 0x6 OCF 0x001

typedef struct HCI_Read_Loopback_Mode_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     Loopback_Mode;
} HCI_Read_Loopback_Mode_Ret_t;

// 7.3.31 Read Num Broadcast Retransmissions command

static const uint16_t Read_Num_Broadcast_Cmdransmissions_Cmd = 0x0c29; // OGF 0x3 OCF 0x029

typedef struct HCI_Read_Num_Broadcast_Retransmissions_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     Num_Broadcast_Retransmissions;
} HCI_Read_Num_Broadcast_Retransmissions_Ret_t;

// 7.3.43 Read Number Of Supported IAC command

static const uint16_t Read_Number_Of_Supported_IAC_Cmd = 0x0c38; // OGF 0x3 OCF 0x038

typedef struct HCI_Read_Number_Of_Supported_IAC_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     Num_Supported_IAC;
} HCI_Read_Number_Of_Supported_IAC_Ret_t;

// 7.3.19 Read Page Scan Activity command

static const uint16_t Read_Page_Scan_Activity_Cmd = 0x0c1b; // OGF 0x3 OCF 0x01b

typedef struct HCI_Read_Page_Scan_Activity_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint16_t                    Page_Scan_Interval;
  uint16_t                    Page_Scan_Window;
} HCI_Read_Page_Scan_Activity_Ret_t;

// 7.3.51 Read Page Scan Type command

static const uint16_t Read_Page_Scan_Type_Cmd = 0x0c46; // OGF 0x3 OCF 0x046

typedef struct HCI_Read_Page_Scan_Type_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     Page_Scan_Type;
} HCI_Read_Page_Scan_Type_Ret_t;

// 7.3.15 Read Page Timeout command

static const uint16_t Read_Page_Timeout_Cmd = 0x0c17; // OGF 0x3 OCF 0x017

typedef struct HCI_Read_Page_Timeout_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint16_t                    Page_Timeout;
} HCI_Read_Page_Timeout_Ret_t;

// 7.3.5 Read PIN Type command

static const uint16_t Read_PIN_Type_Cmd = 0x0c09; // OGF 0x3 OCF 0x009

typedef struct HCI_Read_PIN_Type_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     PIN_Type;
} HCI_Read_PIN_Type_Ret_t;

// 7.1.22 Read Remote Extended Features command

static const uint16_t Read_Remote_Extended_Features_Cmd = 0x041c; // OGF 0x1 OCF 0x01c

typedef struct HCI_Read_Remote_Extended_Features_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Conn_Handle;
  uint8_t                     Page_Number;
} HCI_Read_Remote_Extended_Features_Cmd_t;

// 7.7.34 Read Remote Extended Features Complete event

static const uint8_t Read_Remote_Extended_Features_Complete_Evt = 0x23;

typedef struct HCI_Read_Remote_Extended_Features_Complete_Evt_t {
  HCI_Evt_t                   Evt;
  Status_Conn_Handle_t        Status_Conn;
  uint8_t                     Page_Number;
  uint8_t                     Max_Page_Number;
  uint8_t                     Extended_LMP_Features[8];
} HCI_Read_Remote_Extended_Features_Complete_Evt_t;

// 7.1.21 Read Remote Supported Features command

// HCI_Read_Remote_Supported_Features_Cmd_t only has one member: Connection_Handle.

static const uint16_t Read_Remote_Supported_Features_Cmd = 0x041b; // OGF 0x1 OCF 0x01b

// 7.7.11 Read Remote Supported Features Complete event

static const uint8_t Read_Remote_Supported_Features_Complete_Evt = 0x0b;

typedef struct HCI_Read_Remote_Supported_Features_Complete_Evt_t {
  HCI_Evt_t                   Evt;
  Status_Conn_Handle_t        Status_Conn;
  uint8_t                     LMP_Features[8];
} HCI_Read_Remote_Supported_Features_Complete_Evt_t;

// 7.1.23 Read Remote Version Information command

// HCI_Read_Remote_Version_Information_Cmd_t only has one member: Connection_Handle.

static const uint16_t Read_Remote_Version_Information_Cmd = 0x041d; // OGF 0x1 OCF 0x01d

// 7.7.12 Read Remote Version Information Complete event

static const uint8_t Read_Remote_Version_Information_Complete_Evt = 0x0c;

typedef struct HCI_Read_Remote_Version_Information_Complete_Evt_t {
  HCI_Evt_t                   Evt;
  Status_Conn_Handle_t        Status_Conn;
  uint8_t                     Version;
  uint16_t                    Company_Identifier;
  uint16_t                    Subversion;
} HCI_Read_Remote_Version_Information_Complete_Evt_t;

// 7.5.4 Read RSSI command

static const uint16_t Read_RSSI_Cmd = 0x1405; // OGF 0x5 OCF 0x005

typedef struct HCI_Read_RSSI_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Handle;
} HCI_Read_RSSI_Cmd_t;

typedef struct HCI_Read_RSSI_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint16_t                    Handle;
  uint8_t                     RSSI;
} HCI_Read_RSSI_Ret_t;

// 7.3.17 Read Scan Enable command

static const uint16_t Read_Scan_Enable_Cmd = 0x0c19; // OGF 0x3 OCF 0x019

typedef struct HCI_Read_Scan_Enable_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     Scan_Enable;
} HCI_Read_Scan_Enable_Ret_t;

// 7.3.91 Read Secure Connections Host Support command

static const uint16_t Read_Secure_Connections_Host_Support_Cmd = 0x0c79; // OGF 0x3 OCF 0x079

typedef struct HCI_Read_Secure_Connections_Host_Support_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     Secure_Connections_Host_Support;
} HCI_Read_Secure_Connections_Host_Support_Ret_t;

// 7.3.58 Read Simple Pairing Mode command

static const uint16_t Read_Simple_Pairing_Mode_Cmd = 0x0c55; // OGF 0x3 OCF 0x055

typedef struct HCI_Read_Simple_Pairing_Mode_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     Simple_Pairing_Mode;
} HCI_Read_Simple_Pairing_Mode_Ret_t;

// 7.3.8 Read Stored Link Key command

static const uint16_t Read_Stored_Link_Key_Cmd = 0x0c0d; // OGF 0x3 OCF 0x00d

typedef struct HCI_Read_Stored_Link_Key_Cmd_t {
  HCI_Cmd_t                   Cmd;
  BD_Addr_t                   Addr;
  uint8_t                     Read_All;
} HCI_Read_Stored_Link_Key_Cmd_t;

typedef struct HCI_Read_Stored_Link_Key_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint16_t                    Max_Num_Keys;
  uint16_t                    Num_Keys_Read;
} HCI_Read_Stored_Link_Key_Ret_t;

// 7.3.89 Read Synchronization Train Parameters command

static const uint16_t Read_Sync_Train_Parameters_Cmd = 0x0c77; // OGF 0x3 OCF 0x077

typedef struct HCI_Read_Sync_Train_Parameters_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint16_t                    Sync_Train_Interval;
  uint32_t                    Sync_Train_Timeout;
  uint8_t                     Service_Data;
} HCI_Read_Sync_Train_Parameters_Ret_t;

// 7.3.36 Read Synchronous Flow Control Enable command

static const uint16_t Read_Sync_Flow_Control_Enable_Cmd = 0x0c2e; // OGF 0x3 OCF 0x02e

typedef struct HCI_Read_Sync_Flow_Control_Enable_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     Sync_Flow_Control_Enable;
} HCI_Read_Sync_Flow_Control_Enable_Ret_t;

// 7.3.35 Read Transmit Power Level command

static const uint16_t Read_Transmit_Power_Level_Cmd = 0x0c2d; // OGF 0x3 OCF 0x02d

typedef struct HCI_Read_Transmit_Power_Level_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Conn_Handle;
  uint8_t                     Type;
} HCI_Read_Transmit_Power_Level_Cmd_t;

typedef struct HCI_Read_Transmit_Power_Level_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  Status_Conn_Handle_t        Status_Conn;
  uint8_t                     TX_Power_Level;
} HCI_Read_Transmit_Power_Level_Ret_t;

// 7.3.27 Read Voice Setting command

static const uint16_t Read_Voice_Setting_Cmd = 0x0c25; // OGF 0x3 OCF 0x025

typedef struct HCI_Read_Voice_Setting_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint16_t                    Voice_Setting;
} HCI_Read_Voice_Setting_Ret_t;

// 7.1.52 Receive Synchronization Train command

static const uint16_t Receive_Sync_Train_Cmd = 0x0444; // OGF 0x1 OCF 0x044

typedef struct HCI_Receive_Sync_Train_Cmd_t {
  HCI_Cmd_t                   Cmd;
  BD_Addr_t                   Addr;
  uint16_t                    Sync_Scan_Timeout;
  uint16_t                    Sync_Scan_Window;
  uint16_t                    Sync_Scan_Interval;
} HCI_Receive_Sync_Train_Cmd_t;

// 7.3.57 Refresh Encryption Key command

// HCI_Refresh_Encryption_Key_Cmd_t only has one member: Connection_Handle.

static const uint16_t Refresh_Encryption_Key_Cmd = 0x0c53; // OGF 0x3 OCF 0x053

// 7.1.9 Reject Connection Request command

static const uint16_t Reject_Connection_Request_Cmd = 0x040a; // OGF 0x1 OCF 0x00a

typedef struct HCI_Reject_Connection_Request_Cmd_t {
  HCI_Cmd_t                   Cmd;
  BD_Addr_t                   Addr;
  uint8_t                     Reason;
} HCI_Reject_Connection_Request_Cmd_t;

// 7.1.28 Reject Synchronous Connection Request command

static const uint16_t Reject_Sync_Conn_Request_Cmd = 0x042a; // OGF 0x1 OCF 0x02a

typedef struct HCI_Reject_Sync_Conn_Request_Cmd_t {
  HCI_Cmd_t                   Cmd;
  BD_Addr_t                   Addr;
  uint8_t                     Reason;
} HCI_Reject_Sync_Conn_Request_Cmd_t;

// 7.7.50 Remote Host Supported Features Notification event

static const uint8_t Remote_Host_Supported_Features_Notification_Evt = 0x3d;

typedef struct HCI_Remote_Host_Supported_Features_Notification_Evt_t {
  HCI_Evt_t                   Evt;
  BD_Addr_t                   Addr;
  uint8_t                     Host_Supported_Features[8];
} HCI_Remote_Host_Supported_Features_Notification_Evt_t;

// 7.1.20 Remote Name Request Cancel command

static const uint16_t Remote_Name_Request_Cancel_Cmd = 0x041a; // OGF 0x1 OCF 0x01a

typedef struct HCI_Remote_Name_Request_Cancel_Cmd_t {
  HCI_Cmd_t                   Cmd;
  BD_Addr_t                   Addr;
} HCI_Remote_Name_Request_Cancel_Cmd_t;

// HCI_Remote_Name_Request_Cancel_Ret_t only has one member: Status_BD_Addr.

// 7.1.19 Remote Name Request command

static const uint16_t Remote_Name_Request_Cmd = 0x0419; // OGF 0x1 OCF 0x019

typedef struct HCI_Remote_Name_Request_Cmd_t {
  HCI_Cmd_t                   Cmd;
  BD_Addr_t                   Addr;
  uint8_t                     Page_Scan_Repetition_Mode;
  uint8_t                     Reserved;
  uint16_t                    Clock_Offset;
} HCI_Remote_Name_Request_Cmd_t;

// 7.7.7 Remote Name Request Complete event

static const uint8_t Remote_Name_Request_Complete_Evt = 0x07;

typedef struct HCI_Remote_Name_Request_Complete_Evt_t {
  HCI_Evt_t                   Evt;
  Status_BD_Addr_t            Status_BD_Addr;
  uint8_t                     Remote_Name[248];
} HCI_Remote_Name_Request_Complete_Evt_t;

// 7.7.44 Remote OOB Data Request event

static const uint8_t Remote_OOB_Data_Request_Evt = 0x35;

typedef struct HCI_Remote_OOB_Data_Request_Evt_t {
  HCI_Evt_t                   Evt;
  BD_Addr_t                   Addr;
} HCI_Remote_OOB_Data_Request_Evt_t;

// 7.1.35 Remote OOB Data Request Negative Reply command

static const uint16_t Remote_OOB_Data_Request_Negative_Reply_Cmd = 0x0433; // OGF 0x1 OCF 0x033

typedef struct HCI_Remote_OOB_Data_Request_Negative_Reply_Cmd_t {
  HCI_Cmd_t                   Cmd;
  BD_Addr_t                   Addr;
} HCI_Remote_OOB_Data_Request_Negative_Reply_Cmd_t;

// HCI_Remote_OOB_Data_Request_Negative_Reply_Ret_t only has one member: Status_BD_Addr.

// 7.1.34 Remote OOB Data Request Reply command

static const uint16_t Remote_OOB_Data_Request_Reply_Cmd = 0x0430; // OGF 0x1 OCF 0x030

typedef struct HCI_Remote_OOB_Data_Request_Reply_Cmd_t {
  HCI_Cmd_t                   Cmd;
  BD_Addr_t                   Addr;
  uint8_t                     C[16];
  uint8_t                     R[16];
} HCI_Remote_OOB_Data_Request_Reply_Cmd_t;

// HCI_Remote_OOB_Data_Request_Reply_Ret_t only has one member: Status_BD_Addr.

// 7.1.53 Remote OOB Extended Data Request Reply command

static const uint16_t Remote_OOB_Extended_Data_Request_Reply_Cmd = 0x0445; // OGF 0x1 OCF 0x045

typedef struct HCI_Remote_OOB_Extended_Data_Request_Reply_Cmd_t {
  HCI_Cmd_t                   Cmd;
  BD_Addr_t                   Addr;
  uint8_t                     C_192[16];
  uint8_t                     R_192[16];
  uint8_t                     C_256[16];
  uint8_t                     R_256[16];
} HCI_Remote_OOB_Extended_Data_Request_Reply_Cmd_t;

// HCI_Remote_OOB_Extended_Data_Request_Reply_Ret_t only has one member: Status_BD_Addr.

// 7.3.2 Reset command

// HCI_Reset_Ret_t only has one member: Status.

static const uint16_t Reset_Cmd = 0x0c03; // OGF 0x3 OCF 0x003

// 7.5.2 Reset Failed Contact Counter command

static const uint16_t Reset_Failed_Contact_Counter_Cmd = 0x1402; // OGF 0x5 OCF 0x002

typedef struct HCI_Reset_Failed_Contact_Counter_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Handle;
} HCI_Reset_Failed_Contact_Counter_Cmd_t;

typedef struct HCI_Reset_Failed_Contact_Counter_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint16_t                    Handle;
} HCI_Reset_Failed_Contact_Counter_Ret_t;

// 7.7.21 Return Link Keys event

static const uint8_t Return_Link_Keys_Evt = 0x15;

typedef struct HCI_Return_Link_Keys_Evt_t {
  HCI_Evt_t                   Evt;
  uint8_t                     Num_Keys;
  struct {
    BD_Addr_t                 Addr;
    uint8_t                   Link_Key[16];
  } Keys[0];
} HCI_Return_Link_Keys_Evt_t;

// 7.7.18 Role Change event

static const uint8_t Role_Change_Evt = 0x12;

typedef struct HCI_Role_Change_Evt_t {
  HCI_Evt_t                   Evt;
  Status_BD_Addr_t            Status_BD_Addr;
  uint8_t                     New_Role;
} HCI_Role_Change_Evt_t;

// 7.2.7 Role Discovery command

// HCI_Role_Discovery_Cmd_t only has one member: Connection_Handle.

static const uint16_t Role_Discovery_Cmd = 0x0809; // OGF 0x2 OCF 0x009

typedef struct HCI_Role_Discovery_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  Status_Conn_Handle_t        Status_Conn;
  uint8_t                     Current_Role;
} HCI_Role_Discovery_Ret_t;

// 7.7.76 SAM Status Change event

static const uint8_t SAM_Status_Change_Evt = 0x58;

typedef struct HCI_SAM_Status_Change_Evt_t {
  HCI_Evt_t                   Evt;
  uint16_t                    Conn_Handle;
  uint8_t                     Local_SAM_Index;
  uint8_t                     Local_SAM_TX_Availability;
  uint8_t                     Local_SAM_RX_Availability;
  uint8_t                     Remote_SAM_Index;
  uint8_t                     Remote_SAM_TX_Availability;
  uint8_t                     Remote_SAM_RX_Availability;
} HCI_SAM_Status_Change_Evt_t;

// 7.3.63 Send Keypress Notification command

static const uint16_t Send_Keypress_Notification_Cmd = 0x0c60; // OGF 0x3 OCF 0x060

typedef struct HCI_Send_Keypress_Notification_Cmd_t {
  HCI_Cmd_t                   Cmd;
  BD_Addr_t                   Addr;
  uint8_t                     Notification_Type;
} HCI_Send_Keypress_Notification_Cmd_t;

// HCI_Send_Keypress_Notification_Ret_t only has one member: Status_BD_Addr.

// 7.3.46 Set AFH Host Channel Classification command

static const uint16_t Set_AFH_Host_Channel_Classification_Cmd = 0x0c3f; // OGF 0x3 OCF 0x03f

typedef struct HCI_Set_AFH_Host_Channel_Classification_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     AFH_Host_Channel_Classification[10];
} HCI_Set_AFH_Host_Channel_Classification_Cmd_t;

// HCI_Set_AFH_Host_Channel_Classification_Ret_t only has one member: Status.

// 7.1.16 Set Connection Encryption command

static const uint16_t Set_Connection_Encryption_Cmd = 0x0413; // OGF 0x1 OCF 0x013

typedef struct HCI_Set_Connection_Encryption_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Conn_Handle;
  uint8_t                     Encryption_Enable;
} HCI_Set_Connection_Encryption_Cmd_t;

// 7.1.49 Set Connectionless Peripheral Broadcast command

static const uint16_t Set_Connectionless_Peripheral_Broadcast_Cmd = 0x0441; // OGF 0x1 OCF 0x041

typedef struct HCI_Set_Connectionless_Peripheral_Broadcast_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Enable;
  uint8_t                     LT_ADDR;
  uint8_t                     LPO_Allowed;
  uint16_t                    Packet_Type;
  uint16_t                    Interval_Min;
  uint16_t                    Interval_Max;
  uint16_t                    Supervision_Timeout;
} HCI_Set_Connectionless_Peripheral_Broadcast_Cmd_t;

typedef struct HCI_Set_Connectionless_Peripheral_Broadcast_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     LT_ADDR;
  uint16_t                    Interval;
} HCI_Set_Connectionless_Peripheral_Broadcast_Ret_t;

// 7.3.88 Set Connectionless Peripheral Broadcast Data command

static const uint16_t Set_Connectionless_Peripheral_Broadcast_Data_Cmd = 0x0c76; // OGF 0x3 OCF 0x076

typedef struct HCI_Set_Connectionless_Peripheral_Broadcast_Data_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     LT_ADDR;
  uint8_t                     Fragment;
  uint8_t                     Data_Length;
  uint8_t                     Data[0];
} HCI_Set_Connectionless_Peripheral_Broadcast_Data_Cmd_t;

typedef struct HCI_Set_Connectionless_Peripheral_Broadcast_Data_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     LT_ADDR;
} HCI_Set_Connectionless_Peripheral_Broadcast_Data_Ret_t;

// 7.1.50 Set Connectionless Peripheral Broadcast Receive command

static const uint16_t Set_Connectionless_Peripheral_Broadcast_Receive_Cmd = 0x0442; // OGF 0x1 OCF 0x042

typedef struct HCI_Set_Connectionless_Peripheral_Broadcast_Receive_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Enable;
  BD_Addr_t                   Addr;
  uint8_t                     LT_ADDR;
  uint16_t                    Interval;
  uint32_t                    Clock_Offset;
  uint32_t                    Next_Connectionless_Peripheral_Broadcast_Clock;
  uint16_t                    Supervision_Timeout;
  uint8_t                     Remote_Timing_Accuracy;
  uint8_t                     Skip;
  uint16_t                    Packet_Type;
  uint8_t                     AFH_Channel_Map[10];
} HCI_Set_Connectionless_Peripheral_Broadcast_Receive_Cmd_t;

typedef struct HCI_Set_Connectionless_Peripheral_Broadcast_Receive_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  Status_BD_Addr_t            Status_BD_Addr;
  uint8_t                     LT_ADDR;
} HCI_Set_Connectionless_Peripheral_Broadcast_Receive_Ret_t;

// 7.3.38 Set Controller To Host Flow Control command

static const uint16_t Set_Controller_To_Host_Flow_Control_Cmd = 0x0c31; // OGF 0x3 OCF 0x031

typedef struct HCI_Set_Controller_To_Host_Flow_Control_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Flow_Control_Enable;
} HCI_Set_Controller_To_Host_Flow_Control_Cmd_t;

// HCI_Set_Controller_To_Host_Flow_Control_Ret_t only has one member: Status.

// 7.3.100 Set Ecosystem Base Interval command

static const uint16_t Set_Ecosystem_Base_Interval_Cmd = 0x0c82; // OGF 0x3 OCF 0x082

typedef struct HCI_Set_Ecosystem_Base_Interval_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Interval;
} HCI_Set_Ecosystem_Base_Interval_Cmd_t;

// HCI_Set_Ecosystem_Base_Interval_Ret_t only has one member: Status.

// 7.3.3 Set Event Filter command

static const uint16_t Set_Event_Filter_Cmd = 0x0c05; // OGF 0x3 OCF 0x005

typedef struct HCI_Set_Event_Filter_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Filter_Type;
  uint8_t                     Filter_Condition_Type;
  uint8_t                     Condition[7];
} HCI_Set_Event_Filter_Cmd_t;

// HCI_Set_Event_Filter_Ret_t only has one member: Status.

// 7.3.1 Set Event Mask command

static const uint16_t Set_Event_Mask_Cmd = 0x0c01; // OGF 0x3 OCF 0x001

typedef struct HCI_Set_Event_Mask_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Event_Mask[8];
} HCI_Set_Event_Mask_Cmd_t;

// HCI_Set_Event_Mask_Ret_t only has one member: Status.

// 7.3.69 Set Event Mask Page 2 command

static const uint16_t Set_Event_Mask_Page_2_Cmd = 0x0c63; // OGF 0x3 OCF 0x063

typedef struct HCI_Set_Event_Mask_Page_2_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Event_Mask_Page_2[8];
} HCI_Set_Event_Mask_Page_2_Cmd_t;

// HCI_Set_Event_Mask_Page_2_Ret_t only has one member: Status.

// 7.3.81 Set External Frame Configuration command

static const uint16_t Set_External_Frame_Configuration_Cmd = 0x0c6f; // OGF 0x3 OCF 0x06f

typedef struct HCI_Set_External_Frame_Configuration_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    MWS_Frame_Duration;
  uint16_t                    MWS_Frame_Sync_Assert_Offset;
  uint16_t                    MWS_Frame_Sync_Assert_Jitter;
  uint8_t                     MWS_Num_Periods;
  struct {
    uint16_t                  Period_Duration;
    uint8_t                   Period_Type;
  } Periods[0];
} HCI_Set_External_Frame_Configuration_Cmd_t;

// HCI_Set_External_Frame_Configuration_Ret_t only has one member: Status.

// 7.3.102 Set Min Encryption Key Size command

static const uint16_t Set_Min_Encryption_Key_Size_Cmd = 0x0c84; // OGF 0x3 OCF 0x084

typedef struct HCI_Set_Min_Encryption_Key_Size_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Min_Encryption_Key_Size;
} HCI_Set_Min_Encryption_Key_Size_Cmd_t;

// HCI_Set_Min_Encryption_Key_Size_Ret_t only has one member: Status.

// 7.3.80 Set MWS Channel Parameters command

static const uint16_t Set_MWS_Channel_Parameters_Cmd = 0x0c6e; // OGF 0x3 OCF 0x06e

typedef struct HCI_Set_MWS_Channel_Parameters_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     MWS_Channel_Enable;
  uint16_t                    MWS_RX_Center_Frequency;
  uint16_t                    MWS_TX_Center_Frequency;
  uint16_t                    MWS_RX_Channel_Bandwidth;
  uint16_t                    MWS_TX_Channel_Bandwidth;
  uint8_t                     MWS_Channel_Type;
} HCI_Set_MWS_Channel_Parameters_Cmd_t;

// HCI_Set_MWS_Channel_Parameters_Ret_t only has one member: Status.

// 7.3.84 Set MWS Scan Frequency Table command

static const uint16_t Set_MWS_Scan_Frequency_Table_Cmd = 0x0c72; // OGF 0x3 OCF 0x072

typedef struct HCI_Set_MWS_Scan_Frequency_Table_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Num_Scan_Frequencies;
  struct {
    uint16_t                  Scan_Frequency_Low;
    uint16_t                  Scan_Frequency_High;
  } Scan_Frequencies[0];
} HCI_Set_MWS_Scan_Frequency_Table_Cmd_t;

// HCI_Set_MWS_Scan_Frequency_Table_Ret_t only has one member: Status.

// 7.3.82 Set MWS Signaling command

static const uint16_t Set_MWS_Signaling_Cmd = 0x0c70; // OGF 0x3 OCF 0x070

typedef struct HCI_Set_MWS_Signaling_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    MWS_RX_Assert_Offset;
  uint16_t                    MWS_RX_Assert_Jitter;
  uint16_t                    MWS_RX_Deassert_Offset;
  uint16_t                    MWS_RX_Deassert_Jitter;
  uint16_t                    MWS_TX_Assert_Offset;
  uint16_t                    MWS_TX_Assert_Jitter;
  uint16_t                    MWS_TX_Deassert_Offset;
  uint16_t                    MWS_TX_Deassert_Jitter;
  uint16_t                    MWS_Pattern_Assert_Offset;
  uint16_t                    MWS_Pattern_Assert_Jitter;
  uint16_t                    MWS_Inactivity_Duration_Assert_Offset;
  uint16_t                    MWS_Inactivity_Duration_Assert_Jitter;
  uint16_t                    MWS_Scan_Frequency_Assert_Offset;
  uint16_t                    MWS_Scan_Frequency_Assert_Jitter;
  uint16_t                    MWS_Priority_Assert_Offset_Request;
} HCI_Set_MWS_Signaling_Cmd_t;

typedef struct HCI_Set_MWS_Signaling_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint16_t                    Bluetooth_RX_Priority_Assert_Offset;
  uint16_t                    Bluetooth_RX_Priority_Assert_Jitter;
  uint16_t                    Bluetooth_RX_Priority_Deassert_Offset;
  uint16_t                    Bluetooth_RX_Priority_Deassert_Jitter;
  uint16_t                    IEEE_802_RX_Priority_Assert_Offset;
  uint16_t                    IEEE_802_RX_Priority_Assert_Jitter;
  uint16_t                    IEEE_802_RX_Priority_Deassert_Offset;
  uint16_t                    IEEE_802_RX_Priority_Deassert_Jitter;
  uint16_t                    Bluetooth_TX_On_Assert_Offset;
  uint16_t                    Bluetooth_TX_On_Assert_Jitter;
  uint16_t                    Bluetooth_TX_On_Deassert_Offset;
  uint16_t                    Bluetooth_TX_On_Deassert_Jitter;
  uint16_t                    IEEE_802_TX_On_Assert_Offset;
  uint16_t                    IEEE_802_TX_On_Assert_Jitter;
  uint16_t                    IEEE_802_TX_On_Deassert_Offset;
  uint16_t                    IEEE_802_TX_On_Deassert_Jitter;
} HCI_Set_MWS_Signaling_Ret_t;

// 7.3.83 Set MWS Transport Layer command

static const uint16_t Set_MWS_Transport_Layer_Cmd = 0x0c71; // OGF 0x3 OCF 0x071

typedef struct HCI_Set_MWS_Transport_Layer_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Transport_Layer;
  uint32_t                    To_MWS_Baud_Rate;
  uint32_t                    From_MWS_Baud_Rate;
} HCI_Set_MWS_Transport_Layer_Cmd_t;

// HCI_Set_MWS_Transport_Layer_Ret_t only has one member: Status.

// 7.3.85 Set MWS_PATTERN Configuration command

static const uint16_t Set_MWS_PATTERN_Configuration_Cmd = 0x0c73; // OGF 0x3 OCF 0x073

typedef struct HCI_Set_MWS_PATTERN_Configuration_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     MWS_Pattern_Index;
  uint8_t                     MWS_Pattern_Num_Intervals;
  struct {
    uint8_t                   MWS_Pattern_Interval_Duration;
    uint8_t                   MWS_Pattern_Interval_Type;
  } Intervals[0];
} HCI_Set_MWS_PATTERN_Configuration_Cmd_t;

// HCI_Set_MWS_PATTERN_Configuration_Ret_t only has one member: Status.

// 7.3.86 Set Reserved LT_ADDR command

static const uint16_t Set_Reserved_LT_ADDR_Cmd = 0x0c74; // OGF 0x3 OCF 0x074

typedef struct HCI_Set_Reserved_LT_ADDR_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     LT_ADDR;
} HCI_Set_Reserved_LT_ADDR_Cmd_t;

typedef struct HCI_Set_Reserved_LT_ADDR_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     LT_ADDR;
} HCI_Set_Reserved_LT_ADDR_Ret_t;

// 7.5.12 Set Triggered Clock Capture command

static const uint16_t Set_Triggered_Clock_Capture_Cmd = 0x140d; // OGF 0x5 OCF 0x00d

typedef struct HCI_Set_Triggered_Clock_Capture_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Conn_Handle;
  uint8_t                     Enable;
  uint8_t                     Which_Clock;
  uint8_t                     LPO_Allowed;
  uint8_t                     Num_Clock_Captures_To_Filter;
} HCI_Set_Triggered_Clock_Capture_Cmd_t;

// HCI_Set_Triggered_Clock_Capture_Ret_t only has one member: Status.

// 7.1.26 Setup Synchronous Connection command

static const uint16_t Setup_Sync_Conn_Cmd = 0x0428; // OGF 0x1 OCF 0x028

typedef struct HCI_Setup_Sync_Conn_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Conn_Handle;
  uint32_t                    Transmit_Bandwidth;
  uint32_t                    Receive_Bandwidth;
  uint16_t                    Max_Latency;
  uint16_t                    Voice_Setting;
  uint8_t                     Retransmission_Effort;
  uint16_t                    Packet_Type;
} HCI_Setup_Sync_Conn_Cmd_t;

// 7.7.45 Simple Pairing Complete event

// HCI_Simple_Pairing_Complete_Evt_t only has one member: Status_BD_Addr.

static const uint8_t Simple_Pairing_Complete_Evt = 0x36;

// 7.2.2 Sniff Mode command

static const uint16_t Sniff_Mode_Cmd = 0x0803; // OGF 0x2 OCF 0x003

typedef struct HCI_Sniff_Mode_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Conn_Handle;
  uint16_t                    Sniff_Max_Interval;
  uint16_t                    Sniff_Min_Interval;
  uint16_t                    Sniff_Attempt;
  uint16_t                    Sniff_Timeout;
} HCI_Sniff_Mode_Cmd_t;

// 7.2.14 Sniff Subrating command

static const uint16_t Sniff_Subrating_Cmd = 0x0811; // OGF 0x2 OCF 0x011

typedef struct HCI_Sniff_Subrating_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Conn_Handle;
  uint16_t                    Max_Latency;
  uint16_t                    Min_Remote_Timeout;
  uint16_t                    Min_Local_Timeout;
} HCI_Sniff_Subrating_Cmd_t;

// HCI_Sniff_Subrating_Ret_t only has one member: Status_Conn.

// 7.7.37 Sniff Subrating event

static const uint8_t Sniff_Subrating_Evt = 0x2e;

typedef struct HCI_Sniff_Subrating_Evt_t {
  HCI_Evt_t                   Evt;
  Status_Conn_Handle_t        Status_Conn;
  uint16_t                    Max_TX_Latency;
  uint16_t                    Max_RX_Latency;
  uint16_t                    Min_Remote_Timeout;
  uint16_t                    Min_Local_Timeout;
} HCI_Sniff_Subrating_Evt_t;

// 7.2.8 Switch Role command

static const uint16_t Switch_Role_Cmd = 0x080b; // OGF 0x2 OCF 0x00b

typedef struct HCI_Switch_Role_Cmd_t {
  HCI_Cmd_t                   Cmd;
  BD_Addr_t                   Addr;
  uint8_t                     Role;
} HCI_Switch_Role_Cmd_t;

// 7.7.67 Synchronization Train Complete event

// HCI_Sync_Train_Complete_Evt_t only has one member: Status.

static const uint8_t Sync_Train_Complete_Evt = 0x4f;

// 7.7.68 Synchronization Train Received event

static const uint8_t Sync_Train_Received_Evt = 0x50;

typedef struct HCI_Sync_Train_Received_Evt_t {
  HCI_Evt_t                   Evt;
  Status_BD_Addr_t            Status_BD_Addr;
  uint32_t                    Clock_Offset;
  uint8_t                     AFH_Channel_Map[10];
  uint8_t                     LT_ADDR;
  uint32_t                    Next_Broadcast_Instant;
  uint16_t                    Connectionless_Peripheral_Broadcast_Interval;
  uint8_t                     Service_Data;
} HCI_Sync_Train_Received_Evt_t;

// 7.7.36 Synchronous Connection Changed event

static const uint8_t Sync_Conn_Changed_Evt = 0x2d;

typedef struct HCI_Sync_Conn_Changed_Evt_t {
  HCI_Evt_t                   Evt;
  Status_Conn_Handle_t        Status_Conn;
  uint8_t                     Transmission_Interval;
  uint8_t                     Retransmission_Window;
  uint16_t                    RX_Packet_Length;
  uint16_t                    TX_Packet_Length;
} HCI_Sync_Conn_Changed_Evt_t;

// 7.7.35 Synchronous Connection Complete event

static const uint8_t Sync_Conn_Complete_Evt = 0x2c;

typedef struct HCI_Sync_Conn_Complete_Evt_t {
  HCI_Evt_t                   Evt;
  Status_Conn_Handle_t        Status_Conn;
  BD_Addr_t                   Addr;
  uint8_t                     Link_Type;
  uint8_t                     Transmission_Interval;
  uint8_t                     Retransmission_Window;
  uint16_t                    RX_Packet_Length;
  uint16_t                    TX_Packet_Length;
  uint8_t                     Air_Mode;
} HCI_Sync_Conn_Complete_Evt_t;

// 7.7.66 Triggered Clock Capture event

static const uint8_t Triggered_Clock_Capture_Evt = 0x4e;

typedef struct HCI_Triggered_Clock_Capture_Evt_t {
  HCI_Evt_t                   Evt;
  uint16_t                    Conn_Handle;
  uint8_t                     Which_Clock;
  uint32_t                    Clock;
  uint16_t                    Slot_Offset;
} HCI_Triggered_Clock_Capture_Evt_t;

// 7.1.48 Truncated Page Cancel command

static const uint16_t Truncated_Page_Cancel_Cmd = 0x0440; // OGF 0x1 OCF 0x040

typedef struct HCI_Truncated_Page_Cancel_Cmd_t {
  HCI_Cmd_t                   Cmd;
  BD_Addr_t                   Addr;
} HCI_Truncated_Page_Cancel_Cmd_t;

// HCI_Truncated_Page_Cancel_Ret_t only has one member: Status_BD_Addr.

// 7.1.47 Truncated Page command

static const uint16_t Truncated_Page_Cmd = 0x043f; // OGF 0x1 OCF 0x03f

typedef struct HCI_Truncated_Page_Cmd_t {
  HCI_Cmd_t                   Cmd;
  BD_Addr_t                   Addr;
  uint8_t                     Page_Scan_Repetition_Mode;
  uint16_t                    Clock_Offset;
} HCI_Truncated_Page_Cmd_t;

// 7.7.71 Truncated Page Complete event

// HCI_Truncated_Page_Complete_Evt_t only has one member: Status_BD_Addr.

static const uint8_t Truncated_Page_Complete_Evt = 0x53;

// 7.7.42 User Confirmation Request event

static const uint8_t User_Confirmation_Request_Evt = 0x33;

typedef struct HCI_User_Confirmation_Request_Evt_t {
  HCI_Evt_t                   Evt;
  BD_Addr_t                   Addr;
  uint32_t                    Numeric_Value;
} HCI_User_Confirmation_Request_Evt_t;

// 7.1.31 User Confirmation Request Negative Reply command

static const uint16_t User_Confirmation_Request_Negative_Reply_Cmd = 0x042d; // OGF 0x1 OCF 0x02d

typedef struct HCI_User_Confirmation_Request_Negative_Reply_Cmd_t {
  HCI_Cmd_t                   Cmd;
  BD_Addr_t                   Addr;
} HCI_User_Confirmation_Request_Negative_Reply_Cmd_t;

// HCI_User_Confirmation_Request_Negative_Reply_Ret_t only has one member: Status_BD_Addr.

// 7.1.30 User Confirmation Request Reply command

static const uint16_t User_Confirmation_Request_Reply_Cmd = 0x042c; // OGF 0x1 OCF 0x02c

typedef struct HCI_User_Confirmation_Request_Reply_Cmd_t {
  HCI_Cmd_t                   Cmd;
  BD_Addr_t                   Addr;
} HCI_User_Confirmation_Request_Reply_Cmd_t;

// HCI_User_Confirmation_Request_Reply_Ret_t only has one member: Status_BD_Addr.

// 7.7.48 User Passkey Notification event

static const uint8_t User_Passkey_Notification_Evt = 0x3b;

typedef struct HCI_User_Passkey_Notification_Evt_t {
  HCI_Evt_t                   Evt;
  BD_Addr_t                   Addr;
  uint32_t                    Passkey;
} HCI_User_Passkey_Notification_Evt_t;

// 7.7.43 User Passkey Request event

static const uint8_t User_Passkey_Request_Evt = 0x34;

typedef struct HCI_User_Passkey_Request_Evt_t {
  HCI_Evt_t                   Evt;
  BD_Addr_t                   Addr;
} HCI_User_Passkey_Request_Evt_t;

// 7.1.33 User Passkey Request Negative Reply command

static const uint16_t User_Passkey_Request_Negative_Reply_Cmd = 0x042f; // OGF 0x1 OCF 0x02f

typedef struct HCI_User_Passkey_Request_Negative_Reply_Cmd_t {
  HCI_Cmd_t                   Cmd;
  BD_Addr_t                   Addr;
} HCI_User_Passkey_Request_Negative_Reply_Cmd_t;

// HCI_User_Passkey_Request_Negative_Reply_Ret_t only has one member: Status_BD_Addr.

// 7.1.32 User Passkey Request Reply command

static const uint16_t User_Passkey_Request_Reply_Cmd = 0x042e; // OGF 0x1 OCF 0x02e

typedef struct HCI_User_Passkey_Request_Reply_Cmd_t {
  HCI_Cmd_t                   Cmd;
  BD_Addr_t                   Addr;
  uint32_t                    Numeric_Value;
} HCI_User_Passkey_Request_Reply_Cmd_t;

// HCI_User_Passkey_Request_Reply_Ret_t only has one member: Status_BD_Addr.

// 7.3.54 Write AFH Channel Assessment Mode command

static const uint16_t Write_AFH_Channel_Assessment_Mode_Cmd = 0x0c49; // OGF 0x3 OCF 0x049

typedef struct HCI_Write_AFH_Channel_Assessment_Mode_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     AFH_Channel_Assessment_Mode;
} HCI_Write_AFH_Channel_Assessment_Mode_Cmd_t;

// HCI_Write_AFH_Channel_Assessment_Mode_Ret_t only has one member: Status.

// 7.3.94 Write Authenticated Payload Timeout command

static const uint16_t Write_Authenticated_Payload_Timeout_Cmd = 0x0c7c; // OGF 0x3 OCF 0x07c

typedef struct HCI_Write_Authenticated_Payload_Timeout_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Conn_Handle;
  uint16_t                    Authenticated_Payload_Timeout;
} HCI_Write_Authenticated_Payload_Timeout_Cmd_t;

// HCI_Write_Authenticated_Payload_Timeout_Ret_t only has one member: Status_Conn.

// 7.3.24 Write Authentication Enable command

static const uint16_t Write_Authentication_Enable_Cmd = 0x0c20; // OGF 0x3 OCF 0x020

typedef struct HCI_Write_Authentication_Enable_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Authentication_Enable;
} HCI_Write_Authentication_Enable_Cmd_t;

// HCI_Write_Authentication_Enable_Ret_t only has one member: Status.

// 7.3.30 Write Automatic Flush Timeout command

static const uint16_t Write_Automatic_Flush_Timeout_Cmd = 0x0c28; // OGF 0x3 OCF 0x028

typedef struct HCI_Write_Automatic_Flush_Timeout_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Conn_Handle;
  uint16_t                    Flush_Timeout;
} HCI_Write_Automatic_Flush_Timeout_Cmd_t;

// HCI_Write_Automatic_Flush_Timeout_Ret_t only has one member: Status_Conn.

// 7.3.26 Write Class of Device command

static const uint16_t Write_Class_Of_Device_Cmd = 0x0c24; // OGF 0x3 OCF 0x024

typedef struct HCI_Write_Class_Of_Device_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Class_Of_Device[3];
} HCI_Write_Class_Of_Device_Cmd_t;

// HCI_Write_Class_Of_Device_Ret_t only has one member: Status.

// 7.3.14 Write Connection Accept Timeout command

static const uint16_t Write_Connection_Accept_Timeout_Cmd = 0x0c16; // OGF 0x3 OCF 0x016

typedef struct HCI_Write_Connection_Accept_Timeout_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Connection_Accept_Timeout;
} HCI_Write_Connection_Accept_Timeout_Cmd_t;

// HCI_Write_Connection_Accept_Timeout_Ret_t only has one member: Status.

// 7.3.45 Write Current IAC LAP command

static const uint16_t Write_Current_IAC_LAP_Cmd = 0x0c3a; // OGF 0x3 OCF 0x03a

typedef struct HCI_Write_Current_IAC_LAP_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Num_Current_IAC;
  uint8_t                     IAC_LAP[3];
} HCI_Write_Current_IAC_LAP_Cmd_t;

// HCI_Write_Current_IAC_LAP_Ret_t only has one member: Status.

// 7.3.65 Write Default Erroneous Data Reporting command

static const uint16_t Write_Default_Erroneous_Data_Reporting_Cmd = 0x0c5b; // OGF 0x3 OCF 0x05b

typedef struct HCI_Write_Default_Erroneous_Data_Reporting_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Erroneous_Data_Reporting;
} HCI_Write_Default_Erroneous_Data_Reporting_Cmd_t;

// HCI_Write_Default_Erroneous_Data_Reporting_Ret_t only has one member: Status.

// 7.2.12 Write Default Link Policy Settings command

static const uint16_t Write_Default_Link_Policy_Settings_Cmd = 0x080f; // OGF 0x2 OCF 0x00f

typedef struct HCI_Write_Default_Link_Policy_Settings_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Default_Link_Policy_Settings;
} HCI_Write_Default_Link_Policy_Settings_Cmd_t;

// HCI_Write_Default_Link_Policy_Settings_Ret_t only has one member: Status.

// 7.3.99 Write Extended Inquiry Length command

static const uint16_t Write_Extended_Inquiry_Length_Cmd = 0x0c81; // OGF 0x3 OCF 0x081

typedef struct HCI_Write_Extended_Inquiry_Length_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Extended_Inquiry_Length;
} HCI_Write_Extended_Inquiry_Length_Cmd_t;

// HCI_Write_Extended_Inquiry_Length_Ret_t only has one member: Status.

// 7.3.56 Write Extended Inquiry Response command

static const uint16_t Write_Extended_Inquiry_Response_Cmd = 0x0c52; // OGF 0x3 OCF 0x052

typedef struct HCI_Write_Extended_Inquiry_Response_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     FEC_Required;
  uint8_t                     Extended_Inquiry_Response[240];
} HCI_Write_Extended_Inquiry_Response_Cmd_t;

// HCI_Write_Extended_Inquiry_Response_Ret_t only has one member: Status.

// 7.3.97 Write Extended Page Timeout command

static const uint16_t Write_Extended_Page_Timeout_Cmd = 0x0c7f; // OGF 0x3 OCF 0x07f

typedef struct HCI_Write_Extended_Page_Timeout_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Extended_Page_Timeout;
} HCI_Write_Extended_Page_Timeout_Cmd_t;

// HCI_Write_Extended_Page_Timeout_Ret_t only has one member: Status.

// 7.3.73 Write Flow Control Mode command

static const uint16_t Write_Flow_Control_Mode_Cmd = 0x0c67; // OGF 0x3 OCF 0x067

typedef struct HCI_Write_Flow_Control_Mode_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Flow_Control_Mode;
} HCI_Write_Flow_Control_Mode_Cmd_t;

// HCI_Write_Flow_Control_Mode_Ret_t only has one member: Status.

// 7.3.34 Write Hold Mode Activity command

static const uint16_t Write_Hold_Mode_Activity_Cmd = 0x0c2c; // OGF 0x3 OCF 0x02c

typedef struct HCI_Write_Hold_Mode_Activity_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Hold_Mode_Activity;
} HCI_Write_Hold_Mode_Activity_Cmd_t;

// HCI_Write_Hold_Mode_Activity_Ret_t only has one member: Status.

// 7.3.50 Write Inquiry Mode command

static const uint16_t Write_Inquiry_Mode_Cmd = 0x0c45; // OGF 0x3 OCF 0x045

typedef struct HCI_Write_Inquiry_Mode_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Inquiry_Mode;
} HCI_Write_Inquiry_Mode_Cmd_t;

// HCI_Write_Inquiry_Mode_Ret_t only has one member: Status.

// 7.3.22 Write Inquiry Scan Activity command

static const uint16_t Write_Inquiry_Scan_Activity_Cmd = 0x0c1e; // OGF 0x3 OCF 0x01e

typedef struct HCI_Write_Inquiry_Scan_Activity_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Inquiry_Scan_Interval;
  uint16_t                    Inquiry_Scan_Window;
} HCI_Write_Inquiry_Scan_Activity_Cmd_t;

// HCI_Write_Inquiry_Scan_Activity_Ret_t only has one member: Status.

// 7.3.48 Write Inquiry Scan Type command

static const uint16_t Write_Inquiry_Scan_Type_Cmd = 0x0c43; // OGF 0x3 OCF 0x043

typedef struct HCI_Write_Inquiry_Scan_Type_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Scan_Type;
} HCI_Write_Inquiry_Scan_Type_Cmd_t;

// HCI_Write_Inquiry_Scan_Type_Ret_t only has one member: Status.

// 7.3.62 Write Inquiry Transmit Power Level command

static const uint16_t Write_Inquiry_Transmit_Power_Level_Cmd = 0x0c59; // OGF 0x3 OCF 0x059

typedef struct HCI_Write_Inquiry_Transmit_Power_Level_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     TX_Power;
} HCI_Write_Inquiry_Transmit_Power_Level_Cmd_t;

// HCI_Write_Inquiry_Transmit_Power_Level_Ret_t only has one member: Status.

// 7.3.79 Write LE Host Support command

static const uint16_t Write_LE_Host_Support_Cmd = 0x0c6d; // OGF 0x3 OCF 0x06d

typedef struct HCI_Write_LE_Host_Support_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     LE_Supported_Host;
  uint8_t                     Unused;
} HCI_Write_LE_Host_Support_Cmd_t;

// HCI_Write_LE_Host_Support_Ret_t only has one member: Status.

// 7.2.10 Write Link Policy Settings command

static const uint16_t Write_Link_Policy_Settings_Cmd = 0x080d; // OGF 0x2 OCF 0x00d

typedef struct HCI_Write_Link_Policy_Settings_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Conn_Handle;
  uint16_t                    Link_Policy_Settings;
} HCI_Write_Link_Policy_Settings_Cmd_t;

// HCI_Write_Link_Policy_Settings_Ret_t only has one member: Status_Conn.

// 7.3.42 Write Link Supervision Timeout command

static const uint16_t Write_Link_Supervision_Timeout_Cmd = 0x0c37; // OGF 0x3 OCF 0x037

typedef struct HCI_Write_Link_Supervision_Timeout_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Conn_Handle;
  uint16_t                    Link_Supervision_Timeout;
} HCI_Write_Link_Supervision_Timeout_Cmd_t;

typedef struct HCI_Write_Link_Supervision_Timeout_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint16_t                    Handle;
} HCI_Write_Link_Supervision_Timeout_Ret_t;

// 7.3.11 Write Local Name command

static const uint16_t Write_Local_Name_Cmd = 0x0c13; // OGF 0x3 OCF 0x013

typedef struct HCI_Write_Local_Name_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Local_Name[248];
} HCI_Write_Local_Name_Cmd_t;

// HCI_Write_Local_Name_Ret_t only has one member: Status.

// 7.6.2 Write Loopback Mode command

static const uint16_t Write_Loopback_Mode_Cmd = 0x1802; // OGF 0x6 OCF 0x002

typedef struct HCI_Write_Loopback_Mode_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Loopback_Mode;
} HCI_Write_Loopback_Mode_Cmd_t;

// HCI_Write_Loopback_Mode_Ret_t only has one member: Status.

// 7.3.32 Write Num Broadcast Retransmissions command

static const uint16_t Write_Num_Broadcast_Cmdransmissions_Cmd = 0x0c2a; // OGF 0x3 OCF 0x02a

typedef struct HCI_Write_Num_Broadcast_Retransmissions_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Num_Broadcast_Retransmissions;
} HCI_Write_Num_Broadcast_Retransmissions_Cmd_t;

// HCI_Write_Num_Broadcast_Retransmissions_Ret_t only has one member: Status.

// 7.3.20 Write Page Scan Activity command

static const uint16_t Write_Page_Scan_Activity_Cmd = 0x0c1c; // OGF 0x3 OCF 0x01c

typedef struct HCI_Write_Page_Scan_Activity_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Page_Scan_Interval;
  uint16_t                    Page_Scan_Window;
} HCI_Write_Page_Scan_Activity_Cmd_t;

// HCI_Write_Page_Scan_Activity_Ret_t only has one member: Status.

// 7.3.52 Write Page Scan Type command

static const uint16_t Write_Page_Scan_Type_Cmd = 0x0c47; // OGF 0x3 OCF 0x047

typedef struct HCI_Write_Page_Scan_Type_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Page_Scan_Type;
} HCI_Write_Page_Scan_Type_Cmd_t;

// HCI_Write_Page_Scan_Type_Ret_t only has one member: Status.

// 7.3.16 Write Page Timeout command

static const uint16_t Write_Page_Timeout_Cmd = 0x0c18; // OGF 0x3 OCF 0x018

typedef struct HCI_Write_Page_Timeout_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Page_Timeout;
} HCI_Write_Page_Timeout_Cmd_t;

// HCI_Write_Page_Timeout_Ret_t only has one member: Status.

// 7.3.6 Write PIN Type command

static const uint16_t Write_PIN_Type_Cmd = 0x0c0a; // OGF 0x3 OCF 0x00a

typedef struct HCI_Write_PIN_Type_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     PIN_Type;
} HCI_Write_PIN_Type_Cmd_t;

// HCI_Write_PIN_Type_Ret_t only has one member: Status.

// 7.3.18 Write Scan Enable command

static const uint16_t Write_Scan_Enable_Cmd = 0x0c1a; // OGF 0x3 OCF 0x01a

typedef struct HCI_Write_Scan_Enable_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Scan_Enable;
} HCI_Write_Scan_Enable_Cmd_t;

// HCI_Write_Scan_Enable_Ret_t only has one member: Status.

// 7.3.92 Write Secure Connections Host Support command

static const uint16_t Write_Secure_Connections_Host_Support_Cmd = 0x0c7a; // OGF 0x3 OCF 0x07a

typedef struct HCI_Write_Secure_Connections_Host_Support_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Secure_Connections_Host_Support;
} HCI_Write_Secure_Connections_Host_Support_Cmd_t;

// HCI_Write_Secure_Connections_Host_Support_Ret_t only has one member: Status.

// 7.6.8 Write Secure Connections Test Mode command

static const uint16_t Write_Secure_Connections_Test_Mode_Cmd = 0x180a; // OGF 0x6 OCF 0x00a

typedef struct HCI_Write_Secure_Connections_Test_Mode_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Conn_Handle;
  uint8_t                     DM1_ACL_U_Mode;
  uint8_t                     eSCO_Loopback_Mode;
} HCI_Write_Secure_Connections_Test_Mode_Cmd_t;

// HCI_Write_Secure_Connections_Test_Mode_Ret_t only has one member: Status_Conn.

// 7.6.4 Write Simple Pairing Debug Mode command

static const uint16_t Write_Simple_Pairing_Debug_Mode_Cmd = 0x1804; // OGF 0x6 OCF 0x004

typedef struct HCI_Write_Simple_Pairing_Debug_Mode_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Simple_Pairing_Debug_Mode;
} HCI_Write_Simple_Pairing_Debug_Mode_Cmd_t;

// HCI_Write_Simple_Pairing_Debug_Mode_Ret_t only has one member: Status.

// 7.3.59 Write Simple Pairing Mode command

static const uint16_t Write_Simple_Pairing_Mode_Cmd = 0x0c56; // OGF 0x3 OCF 0x056

typedef struct HCI_Write_Simple_Pairing_Mode_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Simple_Pairing_Mode;
} HCI_Write_Simple_Pairing_Mode_Cmd_t;

// HCI_Write_Simple_Pairing_Mode_Ret_t only has one member: Status.

// 7.3.9 Write Stored Link Key command

static const uint16_t Write_Stored_Link_Key_Cmd = 0x0c11; // OGF 0x3 OCF 0x011

typedef struct HCI_Write_Stored_Link_Key_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Num_Keys_To_Write;
  struct {
    BD_Addr_t                 Addr;
    uint8_t                   Link_Key[16];
  } Keys_To_Write[0];
} HCI_Write_Stored_Link_Key_Cmd_t;

typedef struct HCI_Write_Stored_Link_Key_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint8_t                     Num_Keys_Written;
} HCI_Write_Stored_Link_Key_Ret_t;

// 7.3.90 Write Synchronization Train Parameters command

static const uint16_t Write_Sync_Train_Parameters_Cmd = 0x0c78; // OGF 0x3 OCF 0x078

typedef struct HCI_Write_Sync_Train_Parameters_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Interval_Min;
  uint16_t                    Interval_Max;
  uint32_t                    Sync_Train_Timeout;
  uint8_t                     Service_Data;
} HCI_Write_Sync_Train_Parameters_Cmd_t;

typedef struct HCI_Write_Sync_Train_Parameters_Ret_t {
  HCI_Command_Complete_Evt_t  CC_Evt;
  uint8_t                     Status;
  uint16_t                    Sync_Train_Interval;
} HCI_Write_Sync_Train_Parameters_Ret_t;

// 7.3.37 Write Synchronous Flow Control Enable command

static const uint16_t Write_Sync_Flow_Control_Enable_Cmd = 0x0c2f; // OGF 0x3 OCF 0x02f

typedef struct HCI_Write_Sync_Flow_Control_Enable_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint8_t                     Sync_Flow_Control_Enable;
} HCI_Write_Sync_Flow_Control_Enable_Cmd_t;

// HCI_Write_Sync_Flow_Control_Enable_Ret_t only has one member: Status.

// 7.3.28 Write Voice Setting command

static const uint16_t Write_Voice_Setting_Cmd = 0x0c26; // OGF 0x3 OCF 0x026

typedef struct HCI_Write_Voice_Setting_Cmd_t {
  HCI_Cmd_t                   Cmd;
  uint16_t                    Voice_Setting;
} HCI_Write_Voice_Setting_Cmd_t;

// HCI_Write_Voice_Setting_Ret_t only has one member: Status.

#endif // HCI_TYPES_H
