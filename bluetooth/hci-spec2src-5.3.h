#ifndef SPECS2SRC_H
#define SPECS2SRC_H

// Copyright 2023 Steven Buytaert

#include <stdint.h>
#include <stdlib.h>

typedef struct FRep_t {           // Field Name Replacement entry.
  const char *      r;
  const char *      w;
} FRep_t;

typedef struct SpecEvent_t {
  const char *      name;
} SpecEvent_t;

typedef struct Feature_t {
  const char *      name;
  uint32_t          used;         // Number of times used by a condition.
  uint32_t          enable;       // Set to non zero to enable.
} Feature_t;

typedef struct SpecFeat4Cond_t {  // Couples a feature to a condition.
  uint32_t          mand;         // 1 ==  Mandatory, 0 == Optional.
  uint32_t          index;        // Index in Features table.
} SpecFeat4Cond_t;

typedef struct SpecCond_t {
  const char *      name;
  const char *      notes;
  const uint32_t    numBREDR;
  const uint32_t    numLE;
  const uint32_t    numFeatures;
  const uint32_t *  BREDR;
  const uint32_t *  LE;
  SpecFeat4Cond_t * Features;
  uint32_t          enable;       // Set to non zero to enable.
} SpecCond_t;

typedef enum SizeMod_t {
  Size_No_Mod   = 0,
  Size_Sum      = 1,              // SUM( )
  Size_Bits_Set = 2,              // Bits set in
  Size_Depends  = 3,              // Depends on
} SizeMod_t;

typedef struct SpecField_t {
  const char *      ref2size;     // When not NULL, this field is an array field; size determined by ref2size.
  const char *      name;         // Name (without brackets in case of array field in the spec.
  uint8_t           size;         // Size in octets of (a single element of) this field.
  uint8_t           numb;         // When not null, the number of meaningful bits.
  SizeMod_t         mod;          // When size is special.
  uint8_t           VS[5];        // VS[3] = 1 -> available in [v3], ... Only relevant when HCI.numVar > 1.
} SpecField_t;

typedef struct SpecCmdVar_t {
  uint8_t           var;          // Variant number; when 0, the only variant.
  uint8_t           octet;        // Octet number for supported commands table.
  uint8_t           bit;          // Bit number for supported commands table.
  uint16_t          ocf;          // OCF or event code.
} SpecCmdVar_t;

typedef struct SpecHCI_t {
  const char *      name;         // Long command name.
  const char *      section;      // Section name with definition.
  const char *      version;      // Since this version of the spec.
  const char *      summary;
  const char *      BREDR;        // Condition code CXXX or 'M' 'O' 'E' for BR/EDR.
  const char *      LE;           // Same for LE.
  uint32_t          sub;          // Subevent code if applicable.
  uint32_t          row;          // Line number where defined in the ascii spec document.
  uint32_t          iscmd;        // Non zero for a command, zero for an event.
  uint32_t          subevent;     // Subevent code, if applicable.
  uint32_t          numVI;
  uint32_t          numCmd;
  uint32_t          numRet;
  uint32_t          numEvents;
  SpecCmdVar_t *    VI;           // Variant information.
  SpecField_t *     Cmd;          // Command parameters.
  SpecField_t *     Ret;          // Return parameters.
  SpecEvent_t *     Events;       // Events that this command may or must generate (see spec).
} SpecHCI_t;

#ifdef INCLUDE_SPECS

const SpecHCI_t HCITable[] = {
  { .name = "Accept Connection Request command", .iscmd = 1, .section = "7.1.8", .version = "1.1", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0009, .var = 0, .octet =  1, .bit = 0 },
    },
    .summary = "The HCI_Accept_Connection_Request command is used to accept a new "
               "incoming BR/EDR connection request.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "BD_ADDR",                                        .size = 6,    },
      { .name = "Role",                                           .size = 1,    },
    },
    .numEvents = 2, .Events = (SpecEvent_t[2]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_Connection_Complete" },
    },
  },

  { .name = "Accept Synchronous Connection Request command", .iscmd = 1, .section = "7.1.27", .version = "1.2", .BREDR = "C134", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0029, .var = 0, .octet = 16, .bit = 4 },
    },
    .summary = "The HCI_Accept_Synchronous_ Connection_Request command is used to "
               "accept an incoming request for a synchronous connection and to inform "
               "the local Link Manager about the acceptable parameter values for the "
               "synchronous connection.",

    .numCmd = 7, .Cmd = (SpecField_t[7]) {
      { .name = "BD_ADDR",                                        .size = 6,    },
      { .name = "Transmit_Bandwidth",                             .size = 4,    },
      { .name = "Receive_Bandwidth",                              .size = 4,    },
      { .name = "Max_Latency",                                    .size = 2,    },
      { .name = "Voice_Setting",                                  .size = 2,    .numb = 10 },
      { .name = "Retransmission_Effort",                          .size = 1,    },
      { .name = "Packet_Type",                                    .size = 2,    },
    },
    .numEvents = 5, .Events = (SpecEvent_t[5]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Synchronous_Connection_Complete" },
      { .name = "HCI_Connection_Complete" },
      { .name = "HCI_Synchronous_Connection_Changed" },
      { .name = "HCI_Connection_Request" },
    },
  },

  { .name = "Authenticated Payload Timeout Expired event", .iscmd = 0, .section = "7.7.75", .version = "4.1", .BREDR = "C155", .LE = "C155",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0057, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Authenticated_Payload_Timeout_Expired event is used to "
               "indicate that a packet containing a valid MIC on the Handle was not "
               "received within the authenticatedPayloadTO.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
  },

  { .name = "Authentication Complete event", .iscmd = 0, .section = "7.7.6", .version = "1.1", .BREDR = "C101 connection.", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0006, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Authentication_Complete event occurs when authentication has "
               "been completed for the specified",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
  },

  { .name = "Authentication Requested command", .iscmd = 1, .section = "7.1.15", .version = "1.1", .BREDR = "O", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0011, .var = 0, .octet =  1, .bit = 7 },
    },
    .summary = "The HCI_Authentication_Requested command is used to establish "
               "authentication between the two devices associated with the specified "
               "Connection_Handle.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numEvents = 2, .Events = (SpecEvent_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Connection_Packet_Type_Changed" },
    },
  },

  { .name = "Change Connection Link Key command", .iscmd = 1, .section = "7.1.17", .version = "1.1", .BREDR = "O", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0015, .var = 0, .octet =  2, .bit = 1 },
    },
    .summary = "The HCI_Change_Connection_ Link_Key command is used to force both "
               "devices of a connection associated to the Connection_Handle, to "
               "generate a new link key.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numEvents = 2, .Events = (SpecEvent_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Encryption_Change" },
    },
  },

  { .name = "Change Connection Link Key Complete event", .iscmd = 0, .section = "7.7.9", .version = "1.1", .BREDR = "C102", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0009, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Change_Connection_ Link_Key_Complete event is used to "
               "indicate that the change in the Link Key for the Connection_Handle "
               "specified by the Connection_Handle event parameter had been "
               "completed.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
  },

  { .name = "Change Connection Packet Type command", .iscmd = 1, .section = "7.1.14", .version = "1.1", .BREDR = "C133", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x000f, .var = 0, .octet =  1, .bit = 6 },
    },
    .summary = "The HCI_Change_Connection_ Packet_Type command is used to change "
               "which packet types can be used for a connection that is currently "
               "established.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Packet_Type",                                    .size = 2,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Command Complete event", .iscmd = 0, .section = "7.7.14", .version = "1.1", .BREDR = "M", .LE = "M",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x000e, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Command_Complete event is used by the Controller to pass the "
               "return status of a command and the other event parameters for each "
               "HCI command.",

    .numCmd = 3, .Cmd = (SpecField_t[3]) {
      { .name = "Num_HCI_Command_Packets",                        .size = 1,    },
      { .name = "Command_Opcode",                                 .size = 2,    },
      { .name = "Return_Parameters",                              .size = 1,    .mod = Size_Depends, },
    },
  },

  { .name = "Command Status event", .iscmd = 0, .section = "7.7.15", .version = "1.1", .BREDR = "M for", .LE = "M",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x000f, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Command_Status event is used to indicate that the command "
               "described by the Command_Opcode parameter has been received and the "
               "Controller is currently performing the task this command.",

    .numCmd = 3, .Cmd = (SpecField_t[3]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Num_HCI_Command_Packets",                        .size = 1,    },
      { .name = "Command_Opcode",                                 .size = 2,    },
    },
  },

  { .name = "Configure Data Path command", .iscmd = 1, .section = "7.3.101", .version = "5.2", .BREDR = "C156", .LE = "C156",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0083, .var = 0, .octet = 45, .bit = 5 },
    },
    .summary = "The HCI_Configure_Data_Path command is used by a Host to configure a "
               "data path to enable codec operation in the Controller.",

    .numCmd = 4, .Cmd = (SpecField_t[4]) {
      { .name = "Data_Path_Direction",                            .size = 1,    },
      { .name = "Data_Path_ID",                                   .size = 1,    },
      { .name = "Vendor_Specific_Config_Length",                  .size = 1,    },
      { .name = "Vendor_Specific_Config",                         .size = 1,   .ref2size = "Vendor_Specific_Config_Length", },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Connection Complete event", .iscmd = 0, .section = "7.7.3", .version = "1.1", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0003, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Connection_Complete event indicates to both of the Hosts "
               "forming the connection that a new BR/EDR connection has been "
               "established.",

    .numCmd = 5, .Cmd = (SpecField_t[5]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "BD_ADDR",                                        .size = 6,    },
      { .name = "Link_Type",                                      .size = 1,    },
      { .name = "Encryption_Enabled",                             .size = 1,    },
    },
  },

  { .name = "Connection Packet Type Changed event", .iscmd = 0, .section = "7.7.29", .version = "1.1", .BREDR = "C133", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x001d, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Connection_Packet_ Type_Changed event is used to indicate the "
               "completion of the process of the Link Manager changing the packet "
               "type mask used for the specified Connection_Handle.",

    .numCmd = 3, .Cmd = (SpecField_t[3]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Packet_Type",                                    .size = 2,    },
    },
  },

  { .name = "Connection Request event", .iscmd = 0, .section = "7.7.4", .version = "1.1", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0004, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Connection_Request event is used to indicate that a new "
               "incoming BR/EDR connection is trying to be established.",

    .numCmd = 3, .Cmd = (SpecField_t[3]) {
      { .name = "BD_ADDR",                                        .size = 6,    },
      { .name = "Class_Of_Device",                                .size = 3,    },
      { .name = "Link_Type",                                      .size = 1,    },
    },
  },

  { .name = "Connectionless Peripheral Broadcast Channel Map Change event", .iscmd = 0, .section = "7.7.73", .version = "CSA4", .BREDR = "C201", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0055, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Connectionless_Peripheral_Broadcast_Channel_Map_Change event "
               "indicates to the Host that the BR/EDR Controller has moved to a new "
               "AFH channel map for the PBD logical link.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Channel_Map",                                    .size = 10,   .numb = 79 },
    },
  },

  { .name = "Connectionless Peripheral Broadcast Receive event", .iscmd = 0, .section = "7.7.69", .version = "CSA4", .BREDR = "C202", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0051, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Connectionless_Peripheral_Broadcast_Receive event provides "
               "the Host with the data received from a Connectionless Peripheral "
               "Broadcast packet.",

    .numCmd = 8, .Cmd = (SpecField_t[8]) {
      { .name = "BD_ADDR",                                        .size = 6,    },
      { .name = "LT_ADDR",                                        .size = 1,    },
      { .name = "Clock",                                          .size = 4,    .numb = 28 },
      { .name = "Offset",                                         .size = 4,    .numb = 28 },
      { .name = "RX_Status",                                      .size = 1,    },
      { .name = "Fragment",                                       .size = 1,    },
      { .name = "Data_Length",                                    .size = 1,    },
      { .name = "Data",                                           .size = 1,   .ref2size = "Data_Length", },
    },
  },

  { .name = "Connectionless Peripheral Broadcast Timeout event", .iscmd = 0, .section = "7.7.70", .version = "CSA4", .BREDR = "C202", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0052, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "On the Connectionless Peripheral Broadcast Receiver, the "
               "HCI_Connectionless_Peripheral_Broadcast_Timeout event indicates to "
               "the Host that the BR/EDR Controller has lost synchronization with the "
               "Connectionless Peripheral Broadcast Transmitter. On the "
               "Connectionless Peripheral Broadcast Transmitter, the "
               "HCI_Connectionless_Peripheral_Broadcast_Timeout event indicates to "
               "the Host that the BR/EDR Controller has been unable to transmit a "
               "Connectionless Peripheral Broadcast packet for the timeout interval "
               "specified in the HCI_Set_Connectionless_Peripheral_Broadcast "
               "command.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "BD_ADDR",                                        .size = 6,    },
      { .name = "LT_ADDR",                                        .size = 1,    },
    },
  },

  { .name = "Create Connection Cancel command", .iscmd = 1, .section = "7.1.7", .version = "1.2", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0008, .var = 0, .octet =  0, .bit = 7 },
    },
    .summary = "The HCI_Create_Connection_Cancel command is used to cancel an ongoing "
               "Create Connection.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "BD_ADDR",                                        .size = 6,    },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "BD_ADDR",                                        .size = 6,    },
    },
    .numEvents = 3, .Events = (SpecEvent_t[3]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Disconnection_Complete" },
      { .name = "HCI_LE_CIS_Established" },
    },
  },

  { .name = "Create Connection command", .iscmd = 1, .section = "7.1.5", .version = "1.1", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0005, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Create_Connection command will cause the BR/EDR Link Manager "
               "to create an ACL connection to the BR/EDR Controller with the BD_ADDR "
               "specified by the command parameters.",

    .numCmd = 6, .Cmd = (SpecField_t[6]) {
      { .name = "BD_ADDR",                                        .size = 6,    },
      { .name = "Packet_Type",                                    .size = 2,    },
      { .name = "Page_Scan_Repetition_Mode",                      .size = 1,    },
      { .name = "Reserved",                                       .size = 1,    },
      { .name = "Clock_Offset",                                   .size = 2,    },
      { .name = "Allow_Role_Switch",                              .size = 1,    },
    },
    .numEvents = 2, .Events = (SpecEvent_t[2]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_Inquiry_Complete" },
    },
  },

  { .name = "Data Buffer Overflow event", .iscmd = 0, .section = "7.7.26", .version = "1.1", .BREDR = "O", .LE = "O",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x001a, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Data_Buffer_Overflow event is used to indicate that the "
               "Controller's data buffers have overflowed, because the Host has sent "
               "more packets than allowed.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Link_Type",                                      .size = 1,    },
    },
  },

  { .name = "Delete Reserved LT_ADDR command", .iscmd = 1, .section = "7.3.87", .version = "CSA4", .BREDR = "C201", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0075, .var = 0, .octet = 31, .bit = 5 },
    },
    .summary = "The HCI_Delete_Reserved_LT_ADDR command requests that the BR/EDR "
               "Controller cancel the reservation of a specific LT_ADDR reserved for "
               "the purposes of Connectionless Peripheral Broadcast.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "LT_ADDR",                                        .size = 1,    },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "LT_ADDR",                                        .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Delete Stored Link Key command", .iscmd = 1, .section = "7.3.10", .version = "1.1", .BREDR = "C121", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0012, .var = 0, .octet =  6, .bit = 7 },
    },
    .summary = "The HCI_Delete_Stored_Link_Key command provides the ability to remove "
               "one or more of the link keys stored in the Controller.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "BD_ADDR",                                        .size = 6,    },
      { .name = "Delete_All",                                     .size = 1,    },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Num_Keys_Deleted",                               .size = 2,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Disconnect command", .iscmd = 1, .section = "7.1.6", .version = "1.1", .BREDR = "M", .LE = "C3",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0006, .var = 0, .octet =  0, .bit = 5 },
    },
    .summary = "The HCI_Disconnect command is used to terminate an existing BR/ EDR "
               "or LE connection.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Reason",                                         .size = 1,    },
    },
    .numEvents = 2, .Events = (SpecEvent_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Connection_Complete" },
    },
  },

  { .name = "Disconnection Complete event", .iscmd = 0, .section = "7.7.5", .version = "1.1", .BREDR = "M", .LE = "C3",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0005, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Disconnection_Complete event occurs when a connection has "
               "been terminated.",

    .numCmd = 3, .Cmd = (SpecField_t[3]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Reason",                                         .size = 1,    },
    },
  },

  { .name = "Enable Device Under Test Mode command", .iscmd = 1, .section = "7.6.3", .version = "1.1", .BREDR = "C123", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0003, .var = 0, .octet = 16, .bit = 2 },
    },
    .summary = "The HCI_Enable_Device_Under_Test_Mode command will allow the local "
               "Controller to enter test mode via LMP test commands. The Host issues "
               "this command when it wants the local device to be the DUT for the "
               "Testing scenarios as described in the Bluetooth Test Mode document.",

    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Encryption Change event", .iscmd = 0, .section = "7.7.8", .version = "1.1 5.3", .BREDR = "[v1] M [v2] C158", .LE = "[v1] C4 [v2] C56",
    .numVI = 2, .VI = (SpecCmdVar_t[2]) {
      { .ocf = 0x0059, .var = 2, .octet =  0, .bit = 0 },
      { .ocf = 0x0008, .var = 1, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Encryption_Change event is used to indicate that the change "
               "in encryption has been completed for the specified "
               "Connection_Handle.",

    .numCmd = 4, .Cmd = (SpecField_t[4]) {
      { .name = "Status",                                         .size = 1,   .VS = { 0, 1, 1 },  },
      { .name = "Connection_Handle",                              .size = 2,   .VS = { 0, 1, 1 },  .numb = 12 },
      { .name = "Encryption_Enabled",                             .size = 1,   .VS = { 0, 1, 1 },  },
      { .name = "Encryption_Key_Size",                            .size = 1,   .VS = { 0, 0, 1 },  },
    },
  },

  { .name = "Encryption Key Refresh Complete event", .iscmd = 0, .section = "7.7.39", .version = "2.1 + EDR", .BREDR = "M", .LE = "C4",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0030, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Encryption_Key_Refresh_Complete event is used to indicate to "
               "the Host that the encryption key was refreshed on the given "
               "Connection_Handle any time encryption is paused and then resumed.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
  },

  { .name = "Enhanced Accept Synchronous Connection Request command", .iscmd = 1, .section = "7.1.46", .version = "CSA2", .BREDR = "C135", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003e, .var = 0, .octet = 29, .bit = 4 },
    },
    .summary = "The HCI_Enhanced_Accept_Synchronous_Connection_Request command is "
               "used to accept an incoming request for a synchronous connection and "
               "to inform the local Link Manager about the acceptable parameter "
               "values for the synchronous connection.",

    .numCmd = 24, .Cmd = (SpecField_t[24]) {
      { .name = "BD_ADDR",                                        .size = 6,    },
      { .name = "Transmit_Bandwidth",                             .size = 4,    },
      { .name = "Receive_Bandwidth",                              .size = 4,    },
      { .name = "Transmit_Coding_Format",                         .size = 5,    },
      { .name = "Receive_Coding_Format",                          .size = 5,    },
      { .name = "Transmit_Codec_Frame_Size",                      .size = 2,    },
      { .name = "Receive_Codec_Frame_Size",                       .size = 2,    },
      { .name = "Input_Bandwidth",                                .size = 4,    },
      { .name = "Output_Bandwidth",                               .size = 4,    },
      { .name = "Input_Coding_Format",                            .size = 5,    },
      { .name = "Output_Coding_Format",                           .size = 5,    },
      { .name = "Input_Coded_Data_Size",                          .size = 2,    },
      { .name = "Output_Coded_Data_Size",                         .size = 2,    },
      { .name = "Input_PCM_Data_Format",                          .size = 1,    },
      { .name = "Output_PCM_Data_Format",                         .size = 1,    },
      { .name = "Input_PCM_Sample_Payload_MSB_Position",          .size = 1,    },
      { .name = "Output_PCM_Sample_Payload_MSB_Position",         .size = 1,    },
      { .name = "Input_Data_Path",                                .size = 1,    },
      { .name = "Output_Data_Path",                               .size = 1,    },
      { .name = "Input_Transport_Unit_Size",                      .size = 1,    },
      { .name = "Output_Transport_Unit_Size",                     .size = 1,    },
      { .name = "Max_Latency",                                    .size = 2,    },
      { .name = "Packet_Type",                                    .size = 2,    },
      { .name = "Retransmission_Effort",                          .size = 1,    },
    },
    .numEvents = 3, .Events = (SpecEvent_t[3]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Synchronous_Connection_Complete" },
      { .name = "HCI_Connection_Complete" },
    },
  },

  { .name = "Enhanced Flush command", .iscmd = 1, .section = "7.3.66", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x005f, .var = 0, .octet = 19, .bit = 6 },
    },
    .summary = "The HCI_Enhanced_Flush command is used to discard specific packets "
               "currently pending for transmission in the Controller for the "
               "specified Handle. This command takes a parameter specifying the type "
               "of packets to be flushed.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Handle",                                         .size = 2,    .numb = 12 },
      { .name = "Packet_Type",                                    .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Enhanced Flush Complete event", .iscmd = 0, .section = "7.7.47", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0039, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Enhanced_Flush_Complete event is used to indicate that an "
               "Enhanced Flush is complete.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Handle",                                         .size = 2,    .numb = 12 },
    },
  },

  { .name = "Enhanced Setup Synchronous Connection command", .iscmd = 1, .section = "7.1.45", .version = "CSA2", .BREDR = "C135", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003d, .var = 0, .octet = 29, .bit = 3 },
    },
    .summary = "The HCI_Enhanced_Setup_Synchronous_Connection command adds a new or "
               "modifies an existing synchronous logical transport (SCO or eSCO) on a "
               "physical link depending on the Connection_Handle parameter "
               "specified.",

    .numCmd = 24, .Cmd = (SpecField_t[24]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Transmit_Bandwidth",                             .size = 4,    },
      { .name = "Receive_Bandwidth",                              .size = 4,    },
      { .name = "Transmit_Coding_Format",                         .size = 5,    },
      { .name = "Receive_Coding_Format",                          .size = 5,    },
      { .name = "Transmit_Codec_Frame_Size",                      .size = 2,    },
      { .name = "Receive_Codec_Frame_Size",                       .size = 2,    },
      { .name = "Input_Bandwidth",                                .size = 4,    },
      { .name = "Output_Bandwidth",                               .size = 4,    },
      { .name = "Input_Coding_Format",                            .size = 5,    },
      { .name = "Output_Coding_Format",                           .size = 5,    },
      { .name = "Input_Coded_Data_Size",                          .size = 2,    },
      { .name = "Output_Coded_Data_Size",                         .size = 2,    },
      { .name = "Input_PCM_Data_Format",                          .size = 1,    },
      { .name = "Output_PCM_Data_Format",                         .size = 1,    },
      { .name = "Input_PCM_Sample_Payload_MSB_Position",          .size = 1,    },
      { .name = "Output_PCM_Sample_Payload_MSB_Position",         .size = 1,    },
      { .name = "Input_Data_Path",                                .size = 1,    },
      { .name = "Output_Data_Path",                               .size = 1,    },
      { .name = "Input_Transport_Unit_Size",                      .size = 1,    },
      { .name = "Output_Transport_Unit_Size",                     .size = 1,    },
      { .name = "Max_Latency",                                    .size = 2,    },
      { .name = "Packet_Type",                                    .size = 2,    },
      { .name = "Retransmission_Effort",                          .size = 1,    },
    },
  },

  { .name = "Exit Periodic Inquiry Mode command", .iscmd = 1, .section = "7.1.4", .version = "1.1", .BREDR = "C103", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0004, .var = 0, .octet =  0, .bit = 3 },
    },
    .summary = "The HCI_Exit_Periodic_Inquiry_Mode command is used to end the "
               "Periodic Inquiry mode when the local device is in Periodic Inquiry "
               "Mode.",

    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 3, .Events = (SpecEvent_t[3]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_Extended_Inquiry_Result" },
      { .name = "HCI_Inquiry_Complete" },
    },
  },

  { .name = "Exit Sniff Mode command", .iscmd = 1, .section = "7.2.3", .version = "1.1", .BREDR = "C214", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0004, .var = 0, .octet =  4, .bit = 3 },
    },
    .summary = "The HCI_Exit_Sniff_Mode command is used to end Sniff mode for a "
               "Connection_Handle which is currently in Sniff mode.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numEvents = 2, .Events = (SpecEvent_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Mode_Change" },
    },
  },

  { .name = "Extended Inquiry Result event", .iscmd = 0, .section = "7.7.38", .version = "2.1 + EDR", .BREDR = "C147", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x002f, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Extended_Inquiry_Result event indicates that a BR/ EDR "
               "Controller has responded with an extended inquiry response during the "
               "current Inquiry process.",

    .numCmd = 8, .Cmd = (SpecField_t[8]) {
      { .name = "Num_Responses",                                  .size = 1,    },
      { .name = "BD_ADDR",                                        .size = 6,    },
      { .name = "Page_Scan_Repetition_Mode",                      .size = 1,    },
      { .name = "Reserved",                                       .size = 1,    },
      { .name = "Class_Of_Device",                                .size = 3,    },
      { .name = "Clock_Offset",                                   .size = 2,    },
      { .name = "RSSI",                                           .size = 1,    },
      { .name = "Extended_Inquiry_Response",                      .size = 240,  },
    },
  },

  { .name = "Flow Specification command", .iscmd = 1, .section = "7.2.13", .version = "1.2", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0010, .var = 0, .octet =  5, .bit = 5 },
    },
    .summary = "The HCI_Flow_Specification command is used to specify the flow "
               "parameters for the traffic carried over the ACL connection identified "
               "by the Connection_Handle.",

    .numCmd = 8, .Cmd = (SpecField_t[8]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Unused",                                         .size = 1,    },
      { .name = "Flow_Direction",                                 .size = 1,    },
      { .name = "Service_Type",                                   .size = 1,    },
      { .name = "Token_Rate",                                     .size = 4,    },
      { .name = "Token_Bucket_Size",                              .size = 4,    },
      { .name = "Peak_Bandwidth",                                 .size = 4,    },
      { .name = "Access_Latency",                                 .size = 4,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Flow Specification Complete event", .iscmd = 0, .section = "7.7.32", .version = "1.2", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0021, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Flow_Specification_Complete event is used to inform the Host "
               "about the Quality of Service for the ACL connection the Controller is "
               "able to support.",

    .numCmd = 9, .Cmd = (SpecField_t[9]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Unused",                                         .size = 1,    },
      { .name = "Flow_Direction",                                 .size = 1,    },
      { .name = "Service_Type",                                   .size = 1,    },
      { .name = "Token_Rate",                                     .size = 4,    },
      { .name = "Token_Bucket_Size",                              .size = 4,    },
      { .name = "Peak_Bandwidth",                                 .size = 4,    },
      { .name = "Access_Latency",                                 .size = 4,    },
    },
  },

  { .name = "Flush command", .iscmd = 1, .section = "7.3.4", .version = "1.1", .BREDR = "M currently", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0008, .var = 0, .octet =  6, .bit = 1 },
    },
    .summary = "The HCI_Flush command is used to discard all data that is pending for "
               "transmission in the Controller for the specified Connection_Handle.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Flush Occurred event", .iscmd = 0, .section = "7.7.17", .version = "1.1", .BREDR = "M speci-", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0011, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Flush_Occurred event is used to indicate that, for the fied "
               "Handle, the data to be transmitted has been discarded.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Handle",                                         .size = 2,    .numb = 12 },
    },
  },

  { .name = "Get MWS Transport Layer Configuration command", .iscmd = 1, .section = "7.5.11", .version = "CSA3", .BREDR = "C109", .LE = "C109",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x000c, .var = 0, .octet = 30, .bit = 3 },
    },
    .summary = "The HCI_Get_MWS_Transport_Layer_Configuration command reads the "
               "supported baud rates from the Controller.",

    .numRet = 6, .Ret = (SpecField_t[6]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Num_Transports",                                 .size = 1,    },
      { .name = "Transport_Layer",                                .size = 1,   .ref2size = "Num_Transports", },
      { .name = "Num_Baud_Rates",                                 .size = 1,   .ref2size = "Num_Transports", },
      { .name = "To_MWS_Baud_Rate",                               .size = 4,   .ref2size = "Num_Baud_Rates", .mod = Size_Sum, },
      { .name = "From_MWS_Baud_Rate",                             .size = 4,   .ref2size = "Num_Baud_Rates", .mod = Size_Sum, },
    },
  },

  { .name = "Hardware Error event", .iscmd = 0, .section = "7.7.16", .version = "1.1", .BREDR = "O Controller.", .LE = "O",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0010, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Hardware_Error event is used to indicate some type of "
               "hardware failure for the",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Hardware_Code",                                  .size = 1,    },
    },
  },

  { .name = "Hold Mode command", .iscmd = 1, .section = "7.2.1", .version = "1.1", .BREDR = "C213", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0001, .var = 0, .octet =  4, .bit = 1 },
    },
    .summary = "The HCI_Hold_Mode command is used to initiate Hold mode.",

    .numCmd = 3, .Cmd = (SpecField_t[3]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Hold_Mode_Max_Interval",                         .size = 2,    },
      { .name = "Hold_Mode_Min_Interval",                         .size = 2,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Host Buffer Size command", .iscmd = 1, .section = "7.3.39", .version = "1.1", .BREDR = "C107 sizes data", .LE = "C107",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0033, .var = 0, .octet = 10, .bit = 6 },
    },
    .summary = "The HCI_Host_Buffer_Size command is used by the Host to notify the "
               "Controller about its buffer for ACL and synchronous data. The "
               "Controller will segment the data to be transmitted from the "
               "Controller to the Host, so that contained in HCI Data packets will "
               "not exceed these sizes.",

    .numCmd = 4, .Cmd = (SpecField_t[4]) {
      { .name = "Host_ACL_Data_Packet_Length",                    .size = 2,    },
      { .name = "Host_Synchronous_Data_Packet_Length",            .size = 1,    },
      { .name = "Host_Total_Num_ACL_Data_Packets",                .size = 2,    },
      { .name = "Host_Total_Num_Synchronous_Data_Packets",        .size = 2,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Host Number Of Completed Packets command", .iscmd = 1, .section = "7.3.40", .version = "1.1", .BREDR = "C107", .LE = "C107",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0035, .var = 0, .octet = 10, .bit = 7 },
    },
    .summary = "The HCI_Host_Number_Of_Completed_Packets command is used by the Host "
               "to indicate to the Controller when the Host is ready to receive more "
               "HCI packets for any Connection_Handle.",

    .numCmd = 3, .Cmd = (SpecField_t[3]) {
      { .name = "Num_Handles",                                    .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,   .ref2size = "Num_Handles", .numb = 12 },
      { .name = "Host_Num_Completed_Packets",                     .size = 2,   .ref2size = "Num_Handles", },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Inquiry Cancel command", .iscmd = 1, .section = "7.1.2", .version = "1.1", .BREDR = "C127 if", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0002, .var = 0, .octet =  0, .bit = 1 },
    },
    .summary = "The HCI_Inquiry_Cancel command will cause the BR/EDR Controller to "
               "stop the current Inquiry the BR/EDR Controller is in Inquiry Mode.",

    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 3, .Events = (SpecEvent_t[3]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Extended_Inquiry_Result" },
      { .name = "HCI_Inquiry_Complete" },
    },
  },

  { .name = "Inquiry command", .iscmd = 1, .section = "7.1.1", .version = "1.1", .BREDR = "C127", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0001, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Inquiry command will cause the BR/EDR Controller to enter "
               "Inquiry Mode. Inquiry Mode is used to discovery other nearby BR/EDR "
               "Controllers.",

    .numCmd = 3, .Cmd = (SpecField_t[3]) {
      { .name = "LAP",                                            .size = 3,    },
      { .name = "Inquiry_Length",                                 .size = 1,    },
      { .name = "Num_Responses",                                  .size = 1,    },
    },
  },

  { .name = "Inquiry Complete event", .iscmd = 0, .section = "7.7.1", .version = "1.1", .BREDR = "C127", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0001, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Inquiry_Complete event indicates that the Inquiry is "
               "finished.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Inquiry Response Notification event", .iscmd = 0, .section = "7.7.74", .version = "CSA4", .BREDR = "C126", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0056, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Inquiry_Response_Notification event indicates to the Host "
               "that the BR/EDR Controller responded to an inquiry message.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "LAP",                                            .size = 3,    },
      { .name = "RSSI",                                           .size = 1,    },
    },
  },

  { .name = "Inquiry Result event", .iscmd = 0, .section = "7.7.2", .version = "1.1", .BREDR = "C127", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0002, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Inquiry_Result event indicates that a BR/EDR Controller or "
               "multiple BR/EDR Controllers have responded so far during the current "
               "Inquiry process.",

    .numCmd = 6, .Cmd = (SpecField_t[6]) {
      { .name = "Num_Responses",                                  .size = 1,    },
      { .name = "BD_ADDR",                                        .size = 6,   .ref2size = "Num_Responses", },
      { .name = "Page_Scan_Repetition_Mode",                      .size = 1,   .ref2size = "Num_Responses", },
      { .name = "Reserved",                                       .size = 2,   .ref2size = "Num_Responses", },
      { .name = "Class_Of_Device",                                .size = 3,   .ref2size = "Num_Responses", },
      { .name = "Clock_Offset",                                   .size = 2,   .ref2size = "Num_Responses", },
    },
  },

  { .name = "Inquiry Result with RSSI event", .iscmd = 0, .section = "7.7.33", .version = "1.2", .BREDR = "C128", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0022, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Inquiry_Result_with_RSSI event indicates that a BR/ EDR "
               "Controller or multiple BR/ EDR Controllers have responded so far "
               "during the current Inquiry process.",

    .numCmd = 7, .Cmd = (SpecField_t[7]) {
      { .name = "Num_Responses",                                  .size = 1,    },
      { .name = "BD_ADDR",                                        .size = 6,   .ref2size = "Num_Responses", },
      { .name = "Page_Scan_Repetition_Mode",                      .size = 1,   .ref2size = "Num_Responses", },
      { .name = "Reserved",                                       .size = 1,   .ref2size = "Num_Responses", },
      { .name = "Class_Of_Device",                                .size = 3,   .ref2size = "Num_Responses", },
      { .name = "Clock_Offset",                                   .size = 2,   .ref2size = "Num_Responses", },
      { .name = "RSSI",                                           .size = 1,   .ref2size = "Num_Responses", },
    },
  },

  { .name = "IO Capability Request event", .iscmd = 0, .section = "7.7.40", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0031, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_IO_Capability_Request event is used to indicate that the IO "
               "capabilities of the Host are required for a Secure Simple Pairing "
               "process.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "BD_ADDR",                                        .size = 6,    },
    },
  },

  { .name = "IO Capability Request Negative Reply command", .iscmd = 1, .section = "7.1.36", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0034, .var = 0, .octet = 20, .bit = 3 },
    },
    .summary = "The HCI_IO_Capability_Request_Negative_Reply command is used to "
               "reject a pairing attempt after an HCI_IO_Capability_Request event has "
               "been received by the Host.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "BD_ADDR",                                        .size = 6,    },
      { .name = "Reason",                                         .size = 1,    },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "BD_ADDR",                                        .size = 6,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "IO Capability Request Reply command", .iscmd = 1, .section = "7.1.29", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x002b, .var = 0, .octet = 18, .bit = 7 },
    },
    .summary = "The HCI_IO_Capability_Request_Reply command is used to reply to an "
               "HCI_IO_Capability_Request event from the Controller, and specifies "
               "the current I/O capabilities of the Host.",

    .numCmd = 4, .Cmd = (SpecField_t[4]) {
      { .name = "BD_ADDR",                                        .size = 6,    },
      { .name = "IO_Capability",                                  .size = 1,    },
      { .name = "OOB_Data_Present",                               .size = 1,    },
      { .name = "Authentication_Requirements",                    .size = 1,    },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "BD_ADDR",                                        .size = 6,    },
    },
    .numEvents = 2, .Events = (SpecEvent_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Synchronous_Connection_Complete" },
    },
  },

  { .name = "IO Capability Response event", .iscmd = 0, .section = "7.7.41", .version = "2.1 + EDR", .BREDR = "M capabilities", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0032, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_IO_Capability_Response event is used to indicate to the Host "
               "that IO from a remote device specified by BD_ADDR have been received "
               "during a Secure Simple Pairing process.",

    .numCmd = 4, .Cmd = (SpecField_t[4]) {
      { .name = "BD_ADDR",                                        .size = 6,    },
      { .name = "IO_Capability",                                  .size = 1,    },
      { .name = "OOB_Data_Present",                               .size = 1,    },
      { .name = "Authentication_Requirements",                    .size = 1,    },
    },
  },

  { .name = "Keypress Notification event", .iscmd = 0, .section = "7.7.49", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003c, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Keypress_Notification event is sent to the Host after a "
               "passkey notification has been received by the Link Manager on the "
               "given BD_ADDR.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "BD_ADDR",                                        .size = 6,    },
      { .name = "Notification_Type",                              .size = 1,    },
    },
  },

  { .name = "LE Accept CIS Request command", .iscmd = 1, .section = "7.8.101", .version = "5.2", .BREDR = "E for", .LE = "C40",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0066, .var = 0, .octet = 42, .bit = 3 },
    },
    .summary = "The HCI_LE_Accept_CIS_Request command is used by the Peripheral’s "
               "Host to inform the Controller to accept the request creating the "
               "CIS.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Add Device To Filter Accept List command", .iscmd = 1, .section = "7.8.16", .version = "4.0", .BREDR = "E", .LE = "M",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0011, .var = 0, .octet = 27, .bit = 0 },
    },
    .summary = "The HCI_LE_Add_Device_To_Filter_Accept_List command will add a device "
               "to the Filter Accept List.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Address_Type",                                   .size = 1,    },
      { .name = "Address",                                        .size = 6,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Add Device To Periodic Advertiser List command", .iscmd = 1, .section = "7.8.70", .version = "5.0", .BREDR = "E", .LE = "C21",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0047, .var = 0, .octet = 38, .bit = 3 },
    },
    .summary = "The HCI_LE_Add_Device_To_Periodic_Advertiser_List command will add a "
               "device to the Periodic Advertiser List.",

    .numCmd = 3, .Cmd = (SpecField_t[3]) {
      { .name = "Advertiser_Address_Type",                        .size = 1,    },
      { .name = "Advertiser_Address",                             .size = 6,    },
      { .name = "Advertising_SID",                                .size = 1,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Add Device To Resolving List command", .iscmd = 1, .section = "7.8.38", .version = "4.2", .BREDR = "E Pri-", .LE = "C9",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0027, .var = 0, .octet = 34, .bit = 3 },
    },
    .summary = "The HCI_LE_Add_Device_To_Resolving_List command is used to add one "
               "device to the resolving list used to resolve Resolvable vate "
               "Addresses in the Controller.",

    .numCmd = 4, .Cmd = (SpecField_t[4]) {
      { .name = "Peer_Identity_Address_Type",                     .size = 1,    },
      { .name = "Peer_Identity_Address",                          .size = 6,    },
      { .name = "Peer_IRK",                                       .size = 16,   },
      { .name = "Local_IRK",                                      .size = 16,   },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 2, .Events = (SpecEvent_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_LE_Generate_DHKey_Complete" },
    },
  },

  { .name = "LE Advertising Report event", .iscmd = 0, .subevent = 0x02, .section = "7.7.65.2", .version = "4.0", .BREDR = "E", .LE = "C98",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003e, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_LE_Advertising_Report event indicates that an advertising or "
               "scan response packet has been received.",

    .numCmd = 8, .Cmd = (SpecField_t[8]) {
      { .name = "Subevent_Code",                                  .size = 1,    },
      { .name = "Num_Reports",                                    .size = 1,    },
      { .name = "Event_Type",                                     .size = 1,   .ref2size = "Num_Reports", },
      { .name = "Address_Type",                                   .size = 1,   .ref2size = "Num_Reports", },
      { .name = "Address",                                        .size = 6,   .ref2size = "Num_Reports", },
      { .name = "Data_Length",                                    .size = 1,   .ref2size = "Num_Reports", },
      { .name = "Data",                                           .size = 1,   .ref2size = "Data_Length", .mod = Size_Sum, },
      { .name = "RSSI",                                           .size = 1,   .ref2size = "Num_Reports", },
    },
  },

  { .name = "LE Advertising Set Terminated event", .iscmd = 0, .subevent = 0x12, .section = "7.7.65.18", .version = "5.0", .BREDR = "E", .LE = "C17",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003e, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_LE_Advertising_Set_Terminated event indicates that "
               "advertising in a given advertising set has stopped.",

    .numCmd = 5, .Cmd = (SpecField_t[5]) {
      { .name = "Subevent_Code",                                  .size = 1,    },
      { .name = "Status",                                         .size = 1,    },
      { .name = "Advertising_Handle",                             .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Num_Completed_Extended_Advertising_Events",      .size = 1,    },
    },
  },

  { .name = "LE BIG Create Sync command", .iscmd = 1, .section = "7.8.106", .version = "5.2", .BREDR = "E", .LE = "C42",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x006b, .var = 0, .octet = 43, .bit = 0 },
    },
    .summary = "The HCI_LE_BIG_Create_Sync synchronizes and receives PDUs from one or "
               "more BISes.",

    .numCmd = 8, .Cmd = (SpecField_t[8]) {
      { .name = "BIG_Handle",                                     .size = 1,    },
      { .name = "Sync_Handle",                                    .size = 2,    .numb = 12 },
      { .name = "Encryption",                                     .size = 1,    },
      { .name = "Broadcast_Code",                                 .size = 16,   },
      { .name = "MSE",                                            .size = 1,    },
      { .name = "BIG_Sync_Timeout",                               .size = 2,    },
      { .name = "Num_BIS",                                        .size = 1,    },
      { .name = "BIS",                                            .size = 1,   .ref2size = "Num_BIS", },
    },
    .numEvents = 3, .Events = (SpecEvent_t[3]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_LE_Terminate_BIG_Complete" },
      { .name = "HCI_LE_Create_BIG_Complete" },
    },
  },

  { .name = "LE BIGInfo Advertising Report event", .iscmd = 0, .subevent = 0x22, .section = "7.7.65.34", .version = "5.2", .BREDR = "E", .LE = "C54",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003e, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_LE_BIGInfo_Advertising_Report event indicates that the "
               "Controller has received an Advertising PDU that contained a BIGInfo "
               "field.",

    .numCmd = 14, .Cmd = (SpecField_t[14]) {
      { .name = "Subevent_Code",                                  .size = 1,    },
      { .name = "Sync_Handle",                                    .size = 2,    .numb = 12 },
      { .name = "Num_BIS",                                        .size = 1,    },
      { .name = "NSE",                                            .size = 1,    },
      { .name = "ISO_Interval",                                   .size = 2,    },
      { .name = "BN",                                             .size = 1,    },
      { .name = "PTO",                                            .size = 1,    },
      { .name = "IRC",                                            .size = 1,    },
      { .name = "Max_PDU",                                        .size = 2,    },
      { .name = "SDU_Interval",                                   .size = 3,    },
      { .name = "Max_SDU",                                        .size = 2,    },
      { .name = "PHY",                                            .size = 1,    },
      { .name = "Framing",                                        .size = 1,    },
      { .name = "Encryption",                                     .size = 1,    },
    },
  },

  { .name = "LE BIG Sync Established event", .iscmd = 0, .subevent = 0x1d, .section = "7.7.65.29", .version = "5.2", .BREDR = "E", .LE = "C42",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003e, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_LE_BIG_Sync_Established event indicates that the Controller "
               "has completed an attempt to synchronize with the requested BISes.",

    .numCmd = 12, .Cmd = (SpecField_t[12]) {
      { .name = "Subevent_Code",                                  .size = 1,    },
      { .name = "Status",                                         .size = 1,    },
      { .name = "BIG_Handle",                                     .size = 1,    },
      { .name = "Transport_Latency_BIG",                          .size = 3,    },
      { .name = "NSE",                                            .size = 1,    },
      { .name = "BN",                                             .size = 1,    },
      { .name = "PTO",                                            .size = 1,    },
      { .name = "IRC",                                            .size = 1,    },
      { .name = "Max_PDU",                                        .size = 2,    },
      { .name = "ISO_Interval",                                   .size = 2,    },
      { .name = "Num_BIS",                                        .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,   .ref2size = "Num_BIS", .numb = 12 },
    },
  },

  { .name = "LE BIG Sync Lost event", .iscmd = 0, .subevent = 0x1e, .section = "7.7.65.30", .version = "5.2", .BREDR = "E", .LE = "C42",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003e, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_LE_BIG_Sync_Lost event indicates that the Controller stopped "
               "synchronizing with a BIG.",

    .numCmd = 3, .Cmd = (SpecField_t[3]) {
      { .name = "Subevent_Code",                                  .size = 1,    },
      { .name = "BIG_Handle",                                     .size = 1,    },
      { .name = "Reason",                                         .size = 1,    },
    },
  },

  { .name = "LE BIG Terminate Sync command", .iscmd = 1, .section = "7.8.107", .version = "5.2", .BREDR = "E", .LE = "C42",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x006c, .var = 0, .octet = 43, .bit = 1 },
    },
    .summary = "The HCI_LE_BIG_Terminate_Sync command stops or cancels synchronizing "
               "with a BIG.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "BIG_Handle",                                     .size = 1,    },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "BIG_Handle",                                     .size = 1,    },
    },
    .numEvents = 2, .Events = (SpecEvent_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_LE_BIG_Sync_Established" },
    },
  },

  { .name = "LE Channel Selection Algorithm event", .iscmd = 0, .subevent = 0x14, .section = "7.7.65.20", .version = "5.0", .BREDR = "E", .LE = "C23",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003e, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_LE_Channel_Selection_Algorithm event indicates the channel "
               "selection algorithm used on a connection.",

    .numCmd = 3, .Cmd = (SpecField_t[3]) {
      { .name = "Subevent_Code",                                  .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Channel_Selection_Algorithm",                    .size = 1,    },
    },
  },

  { .name = "LE CIS Established event", .iscmd = 0, .subevent = 0x19, .section = "7.7.65.25", .version = "5.2", .BREDR = "E", .LE = "C38",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003e, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_LE_CIS_Established event indicates that the Controller "
               "established a CIS.",

    .numCmd = 17, .Cmd = (SpecField_t[17]) {
      { .name = "Subevent_Code",                                  .size = 1,    },
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "CIG_Sync_Delay",                                 .size = 3,    },
      { .name = "CIS_Sync_Delay",                                 .size = 3,    },
      { .name = "Transport_Latency_C_To_P",                       .size = 3,    },
      { .name = "Transport_Latency_P_To_C",                       .size = 3,    },
      { .name = "PHY_C_To_P",                                     .size = 1,    },
      { .name = "PHY_P_To_C",                                     .size = 1,    },
      { .name = "NSE",                                            .size = 1,    },
      { .name = "BN_C_To_P",                                      .size = 1,    },
      { .name = "BN_P_To_C",                                      .size = 1,    },
      { .name = "FT_C_To_P",                                      .size = 1,    },
      { .name = "FT_P_To_C",                                      .size = 1,    },
      { .name = "Max_PDU_C_To_P",                                 .size = 2,    },
      { .name = "Max_PDU_P_To_C",                                 .size = 2,    },
      { .name = "ISO_Interval",                                   .size = 2,    },
    },
  },

  { .name = "LE CIS Request event", .iscmd = 0, .subevent = 0x1a, .section = "7.7.65.26", .version = "5.2", .BREDR = "E", .LE = "C40",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003e, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_LE_CIS_Request event indicates that the Peripheral’s "
               "Controller received a request from the Central to create a CIS.",

    .numCmd = 5, .Cmd = (SpecField_t[5]) {
      { .name = "Subevent_Code",                                  .size = 1,    },
      { .name = "ACL_Connection_Handle",                          .size = 2,    .numb = 12 },
      { .name = "CIS_Connection_Handle",                          .size = 2,    .numb = 12 },
      { .name = "CIG_ID",                                         .size = 1,    },
      { .name = "CIS_ID",                                         .size = 1,    },
    },
  },

  { .name = "LE Clear Advertising Sets command", .iscmd = 1, .section = "7.8.60", .version = "5.0", .BREDR = "E", .LE = "C17",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003d, .var = 0, .octet = 37, .bit = 1 },
    },
    .summary = "The HCI_LE_Clear_Advertising_Sets command will remove all existing "
               "advertising sets from the Controller.",

    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Clear Filter Accept List command", .iscmd = 1, .section = "7.8.15", .version = "4.0", .BREDR = "E", .LE = "M",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0010, .var = 0, .octet = 26, .bit = 7 },
    },
    .summary = "The HCI_LE_Clear_Filter_Accept_List command will clear the Filter "
               "Accept List.",

    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Clear Periodic Advertiser List command", .iscmd = 1, .section = "7.8.72", .version = "5.0", .BREDR = "E", .LE = "C21",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0049, .var = 0, .octet = 38, .bit = 5 },
    },
    .summary = "The HCI_LE_Clear_Periodic_Advertiser_List command will clear the "
               "Periodic Advertiser List.",

    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Clear Resolving List command", .iscmd = 1, .section = "7.8.40", .version = "4.2", .BREDR = "E", .LE = "C9",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0029, .var = 0, .octet = 34, .bit = 5 },
    },
    .summary = "The HCI_LE_Clear_Resolving_List command is used to remove all devices "
               "from the resolving list used to resolve Resolvable Private Addresses "
               "in the Controller.",

    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Connection Complete event", .iscmd = 0, .subevent = 0x01, .section = "7.7.65.1", .version = "4.0", .BREDR = "E", .LE = "C3",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003e, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_LE_Connection_Complete event indicates to the Host that a new "
               "connection has been created.",

    .numCmd = 10, .Cmd = (SpecField_t[10]) {
      { .name = "Subevent_Code",                                  .size = 1,    },
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Role",                                           .size = 1,    },
      { .name = "Peer_Address_Type",                              .size = 1,    },
      { .name = "Peer_Address",                                   .size = 6,    },
      { .name = "Connection_Interval",                            .size = 2,    },
      { .name = "Peripheral_Latency",                             .size = 2,    },
      { .name = "Supervision_Timeout",                            .size = 2,    },
      { .name = "Central_Clock_Accuracy",                         .size = 1,    },
    },
  },

  { .name = "LE Connection CTE Request Enable command", .iscmd = 1, .section = "7.8.85", .version = "5.1", .BREDR = "E", .LE = "C25",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0056, .var = 0, .octet = 40, .bit = 2 },
    },
    .summary = "The HCI_LE_Connection_CTE_Request_Enable command will request the "
               "Controller to start or stop sending of LL_CTE_REQ PDUs on a "
               "connection.",

    .numCmd = 5, .Cmd = (SpecField_t[5]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Enable",                                         .size = 1,    },
      { .name = "CTE_Request_Interval",                           .size = 2,    },
      { .name = "Requested_CTE_Length",                           .size = 1,    },
      { .name = "Requested_CTE_Type",                             .size = 1,    },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Connection CTE Response Enable command", .iscmd = 1, .section = "7.8.86", .version = "5.1", .BREDR = "E", .LE = "C26",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0057, .var = 0, .octet = 40, .bit = 3 },
    },
    .summary = "The HCI_LE_Connection_CTE_Response_Enable command will command the "
               "Controller to respond to LL_CTE_REQ PDUs with LL_CTE_RSP PDUs.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Enable",                                         .size = 1,    },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numEvents = 3, .Events = (SpecEvent_t[3]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_LE_Connection_IQ_Report" },
      { .name = "HCI_LE_CTE_Request_Failed" },
    },
  },

  { .name = "LE Connection IQ Report event", .iscmd = 0, .subevent = 0x16, .section = "7.7.65.22", .version = "5.1", .BREDR = "E", .LE = "C25",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003e, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_LE_Connection_IQ_Report event is used to report IQ samples "
               "from the Constant Tone Extension field of a received packet "
               "containing an LL_CTE_RSP PDU.",

    .numCmd = 13, .Cmd = (SpecField_t[13]) {
      { .name = "Subevent_Code",                                  .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "RX_PHY",                                         .size = 1,    },
      { .name = "Data_Channel_Index",                             .size = 1,    },
      { .name = "RSSI",                                           .size = 2,    },
      { .name = "RSSI_Antenna_ID",                                .size = 1,    },
      { .name = "CTE_Type",                                       .size = 1,    },
      { .name = "Slot_Durations",                                 .size = 1,    },
      { .name = "Packet_Status",                                  .size = 1,    },
      { .name = "Connection_Event_Counter",                       .size = 2,    },
      { .name = "Sample_Count",                                   .size = 1,    },
      { .name = "I_Sample",                                       .size = 1,   .ref2size = "Sample_Count", },
      { .name = "Q_Sample",                                       .size = 1,   .ref2size = "Sample_Count", },
    },
  },

  { .name = "LE Connection Update command", .iscmd = 1, .section = "7.8.18", .version = "4.0", .BREDR = "E", .LE = "C62",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0013, .var = 0, .octet = 27, .bit = 2 },
    },
    .summary = "The HCI_LE_Connection_Update command will be used to change the "
               "connection parameters of an existing connection.",

    .numCmd = 7, .Cmd = (SpecField_t[7]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Connection_Interval_Min",                        .size = 2,    },
      { .name = "Connection_Interval_Max",                        .size = 2,    },
      { .name = "Max_Latency",                                    .size = 2,    },
      { .name = "Supervision_Timeout",                            .size = 2,    },
      { .name = "Min_CE_Length",                                  .size = 2,    },
      { .name = "Max_CE_Length",                                  .size = 2,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Connection Update Complete event", .iscmd = 0, .subevent = 0x03, .section = "7.7.65.3", .version = "4.0", .BREDR = "E", .LE = "C3",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003e, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_LE_Connection_Update_Complete event indicates the completion "
               "of the process to change the connection parameters.",

    .numCmd = 6, .Cmd = (SpecField_t[6]) {
      { .name = "Subevent_Code",                                  .size = 1,    },
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Connection_Interval",                            .size = 2,    },
      { .name = "Peripheral_Latency",                             .size = 2,    },
      { .name = "Supervision_Timeout",                            .size = 2,    },
    },
  },

  { .name = "LE Connectionless IQ Report event", .iscmd = 0, .subevent = 0x15, .section = "7.7.65.21", .version = "5.1", .BREDR = "E", .LE = "C28",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003e, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_LE_Connectionless_IQ_Report event reports IQ information from "
               "the Constant Tone Extension of a received advertising packet.",

    .numCmd = 12, .Cmd = (SpecField_t[12]) {
      { .name = "Subevent_Code",                                  .size = 1,    },
      { .name = "Sync_Handle",                                    .size = 2,    .numb = 12 },
      { .name = "Channel_Index",                                  .size = 1,    },
      { .name = "RSSI",                                           .size = 2,    },
      { .name = "RSSI_Antenna_ID",                                .size = 1,    },
      { .name = "CTE_Type",                                       .size = 1,    },
      { .name = "Slot_Durations",                                 .size = 1,    },
      { .name = "Packet_Status",                                  .size = 1,    },
      { .name = "Periodic_Event_Counter",                         .size = 2,    },
      { .name = "Sample_Count",                                   .size = 1,    },
      { .name = "I_Sample",                                       .size = 1,   .ref2size = "Sample_Count", },
      { .name = "Q_Sample",                                       .size = 1,   .ref2size = "Sample_Count", },
    },
  },

  { .name = "LE Create BIG command", .iscmd = 1, .section = "7.8.103", .version = "5.2", .BREDR = "E", .LE = "C41",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0068, .var = 0, .octet = 42, .bit = 5 },
    },
    .summary = "The HCI_LE_Create_BIG command creates one or more BISes of a BIG.",

    .numCmd = 12, .Cmd = (SpecField_t[12]) {
      { .name = "BIG_Handle",                                     .size = 1,    },
      { .name = "Advertising_Handle",                             .size = 1,    },
      { .name = "Num_BIS",                                        .size = 1,    },
      { .name = "SDU_Interval",                                   .size = 3,    },
      { .name = "Max_SDU",                                        .size = 2,    },
      { .name = "Max_Transport_Latency",                          .size = 2,    },
      { .name = "RTN",                                            .size = 1,    },
      { .name = "PHY",                                            .size = 1,    },
      { .name = "Packing",                                        .size = 1,    },
      { .name = "Framing",                                        .size = 1,    },
      { .name = "Encryption",                                     .size = 1,    },
      { .name = "Broadcast_Code",                                 .size = 16,   },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Create BIG Complete event", .iscmd = 0, .subevent = 0x1b, .section = "7.7.65.27", .version = "5.2", .BREDR = "E", .LE = "C41",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003e, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_LE_Create_BIG_Complete event indicates that the Controller "
               "completed an attempt to create the BISes that were requested by the "
               "Host.",

    .numCmd = 14, .Cmd = (SpecField_t[14]) {
      { .name = "Subevent_Code",                                  .size = 1,    },
      { .name = "Status",                                         .size = 1,    },
      { .name = "BIG_Handle",                                     .size = 1,    },
      { .name = "BIG_Sync_Delay",                                 .size = 3,    },
      { .name = "Transport_Latency_BIG",                          .size = 3,    },
      { .name = "PHY",                                            .size = 1,    },
      { .name = "NSE",                                            .size = 1,    },
      { .name = "BN",                                             .size = 1,    },
      { .name = "PTO",                                            .size = 1,    },
      { .name = "IRC",                                            .size = 1,    },
      { .name = "Max_PDU",                                        .size = 2,    },
      { .name = "ISO_Interval",                                   .size = 2,    },
      { .name = "Num_BIS",                                        .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,   .ref2size = "Num_BIS", .numb = 12 },
    },
  },

  { .name = "LE Create BIG Test command", .iscmd = 1, .section = "7.8.104", .version = "5.2", .BREDR = "E", .LE = "C41",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0069, .var = 0, .octet = 42, .bit = 6 },
    },
    .summary = "The HCI_LE_Create_BIG_Test command is used to create one or more "
               "BISes of a BIG for testing purposes.",

    .numCmd = 16, .Cmd = (SpecField_t[16]) {
      { .name = "BIG_Handle",                                     .size = 1,    },
      { .name = "Advertising_Handle",                             .size = 1,    },
      { .name = "Num_BIS",                                        .size = 1,    },
      { .name = "SDU_Interval",                                   .size = 3,    },
      { .name = "ISO_Interval",                                   .size = 2,    },
      { .name = "NSE",                                            .size = 1,    },
      { .name = "Max_SDU",                                        .size = 2,    },
      { .name = "Max_PDU",                                        .size = 2,    },
      { .name = "PHY",                                            .size = 1,    },
      { .name = "Packing",                                        .size = 1,    },
      { .name = "Framing",                                        .size = 1,    },
      { .name = "BN",                                             .size = 1,    },
      { .name = "IRC",                                            .size = 1,    },
      { .name = "PTO",                                            .size = 1,    },
      { .name = "Encryption",                                     .size = 1,    },
      { .name = "Broadcast_Code",                                 .size = 16,   },
    },
    .numEvents = 2, .Events = (SpecEvent_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_LE_Create_BIG_Complete" },
    },
  },

  { .name = "LE Create CIS command", .iscmd = 1, .section = "7.8.99", .version = "5.2", .BREDR = "E Host", .LE = "C39",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0064, .var = 0, .octet = 42, .bit = 1 },
    },
    .summary = "The HCI_LE_Create_CIS command is used by the Central’s to create "
               "one or more CISes.",

    .numCmd = 3, .Cmd = (SpecField_t[3]) {
      { .name = "CIS_Count",                                      .size = 1,    },
      { .name = "CIS_Connection_Handle",                          .size = 2,   .ref2size = "CIS_Count", .numb = 12 },
      { .name = "ACL_Connection_Handle",                          .size = 2,   .ref2size = "CIS_Count", .numb = 12 },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Create Connection Cancel command", .iscmd = 1, .section = "7.8.13", .version = "4.0", .BREDR = "E", .LE = "C94",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x000e, .var = 0, .octet = 26, .bit = 5 },
    },
    .summary = "The HCI_LE_Create_Connection_Cancel command is used to cancel an "
               "ongoing HCI_LE_Create_Connection command.",

    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 3, .Events = (SpecEvent_t[3]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_LE_Enhanced_Connection_Complete" },
      { .name = "HCI_LE_Channel_Selection_Algorithm" },
    },
  },

  { .name = "LE Create Connection command", .iscmd = 1, .section = "7.8.12", .version = "4.0", .BREDR = "E", .LE = "C59",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x000d, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_LE_Create_Connection command is used to create a new "
               "connection.",

    .numCmd = 12, .Cmd = (SpecField_t[12]) {
      { .name = "LE_Scan_Interval",                               .size = 2,    },
      { .name = "LE_Scan_Window",                                 .size = 2,    },
      { .name = "Initiator_Filter_Policy",                        .size = 1,    },
      { .name = "Peer_Address_Type",                              .size = 1,    },
      { .name = "Peer_Address",                                   .size = 6,    },
      { .name = "Own_Address_Type",                               .size = 1,    },
      { .name = "Connection_Interval_Min",                        .size = 2,    },
      { .name = "Connection_Interval_Max",                        .size = 2,    },
      { .name = "Max_Latency",                                    .size = 2,    },
      { .name = "Supervision_Timeout",                            .size = 2,    },
      { .name = "Min_CE_Length",                                  .size = 2,    },
      { .name = "Max_CE_Length",                                  .size = 2,    },
    },
    .numEvents = 3, .Events = (SpecEvent_t[3]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_LE_Advertising_Report" },
      { .name = "HCI_LE_Directed_Advertising_Report" },
    },
  },

  { .name = "LE CTE Request Failed event", .iscmd = 0, .subevent = 0x17, .section = "7.7.65.23", .version = "5.1", .BREDR = "E", .LE = "C25",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003e, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_LE_CTE_Request_Failed event indicates a problem with a "
               "request generated by an HCI_LE_Connection_CTE_Request_Enable command "
               "for a peer device to send Constant Tone Extensions.",

    .numCmd = 3, .Cmd = (SpecField_t[3]) {
      { .name = "Subevent_Code",                                  .size = 1,    },
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
  },

  { .name = "LE Data Length Change event", .iscmd = 0, .subevent = 0x07, .section = "7.7.65.7", .version = "4.2", .BREDR = "E", .LE = "C8",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003e, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_LE_Data_Length_Change event is used to indicate a change in "
               "the maximum packet sizes by the Link Layer.",

    .numCmd = 6, .Cmd = (SpecField_t[6]) {
      { .name = "Subevent_Code",                                  .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Max_TX_Octets",                                  .size = 2,    },
      { .name = "Max_TX_Time",                                    .size = 2,    },
      { .name = "Max_RX_Octets",                                  .size = 2,    },
      { .name = "Max_RX_Time",                                    .size = 2,    },
    },
  },

  { .name = "LE Directed Advertising Report event", .iscmd = 0, .subevent = 0x0b, .section = "7.7.65.11", .version = "4.2", .BREDR = "E prisend", .LE = "C63",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003e, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_LE_Directed_Advertising_Report event indicates that directed "
               "advertisements have been received where the advertiser is using a "
               "resolvable vate address for the TargetA field in the ADV_DIRECT_IND "
               "PDU and the filter policy is set to this event to the Host.",

    .numCmd = 8, .Cmd = (SpecField_t[8]) {
      { .name = "Subevent_Code",                                  .size = 1,    },
      { .name = "Num_Reports",                                    .size = 1,    },
      { .name = "Event_Type",                                     .size = 1,   .ref2size = "Num_Reports", },
      { .name = "Address_Type",                                   .size = 1,   .ref2size = "Num_Reports", },
      { .name = "Address",                                        .size = 6,   .ref2size = "Num_Reports", },
      { .name = "Direct_Address_Type",                            .size = 1,   .ref2size = "Num_Reports", },
      { .name = "Direct_Address",                                 .size = 6,   .ref2size = "Num_Reports", },
      { .name = "RSSI",                                           .size = 1,   .ref2size = "Num_Reports", },
    },
  },

  { .name = "LE Enable Encryption command", .iscmd = 1, .section = "7.8.24", .version = "4.0", .BREDR = "E", .LE = "C60",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0019, .var = 0, .octet = 28, .bit = 0 },
    },
    .summary = "The HCI_LE_Enable_Encryption command is used to enable link level "
               "encryption.",

    .numCmd = 4, .Cmd = (SpecField_t[4]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Random_Number",                                  .size = 8,    },
      { .name = "Encrypted_Diversifier",                          .size = 2,    },
      { .name = "Long_Term_Key",                                  .size = 16,   },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Encrypt command", .iscmd = 1, .section = "7.8.22", .version = "4.0", .BREDR = "E", .LE = "C4",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0017, .var = 0, .octet = 27, .bit = 6 },
    },
    .summary = "The HCI_LE_Encrypt command will encrypt a block of unencrypted data "
               "against a key and generate a block of encrypted data.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Key",                                            .size = 16,   },
      { .name = "Plaintext_Data",                                 .size = 16,   },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Encrypted_Data",                                 .size = 16,   },
    },
    .numEvents = 3, .Events = (SpecEvent_t[3]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_LE_Read_Remote_Features_Complete" },
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Enhanced Connection Complete event", .iscmd = 0, .subevent = 0x0a, .section = "7.7.65.10", .version = "4.2", .BREDR = "E", .LE = "C24",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003e, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_LE_Enhanced_Connection_Complete event indicates to the Host "
               "that a new connection has been created. This event contains the "
               "additional parameters of the local and peer resolvable private "
               "addresses.",

    .numCmd = 12, .Cmd = (SpecField_t[12]) {
      { .name = "Subevent_Code",                                  .size = 1,    },
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Role",                                           .size = 1,    },
      { .name = "Peer_Address_Type",                              .size = 1,    },
      { .name = "Peer_Address",                                   .size = 6,    },
      { .name = "Local_Resolvable_Private_Address",               .size = 6,    },
      { .name = "Peer_Resolvable_Private_Address",                .size = 6,    },
      { .name = "Connection_Interval",                            .size = 2,    },
      { .name = "Peripheral_Latency",                             .size = 2,    },
      { .name = "Supervision_Timeout",                            .size = 2,    },
      { .name = "Central_Clock_Accuracy",                         .size = 1,    },
    },
  },

  { .name = "LE Enhanced Read Transmit Power Level command", .iscmd = 1, .section = "7.8.117", .version = "5.2", .BREDR = "E", .LE = "C51",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0076, .var = 0, .octet = 44, .bit = 3 },
    },
    .summary = "The HCI_LE_Enhanced_Read_Transmit_Power_Level command is used to read "
               "the current and maximum transmit power levels used by the local "
               "Controller on a specified PHY on an ACL connection.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "PHY",                                            .size = 1,    },
    },
    .numRet = 5, .Ret = (SpecField_t[5]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "PHY",                                            .size = 1,    },
      { .name = "Current_TX_Power_Level",                         .size = 1,    },
      { .name = "Max_TX_Power_Level",                             .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Extended Advertising Report event", .iscmd = 0, .subevent = 0x0d, .section = "7.7.65.13", .version = "5.0", .BREDR = "E", .LE = "C19",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003e, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_LE_Extended_Advertising_Report event indicates that an "
               "advertising packet has been received.",

    .numCmd = 15, .Cmd = (SpecField_t[15]) {
      { .name = "Subevent_Code",                                  .size = 1,    },
      { .name = "Num_Reports",                                    .size = 1,    },
      { .name = "Event_Type",                                     .size = 2,   .ref2size = "Num_Reports", },
      { .name = "Address_Type",                                   .size = 1,   .ref2size = "Num_Reports", },
      { .name = "Address",                                        .size = 6,   .ref2size = "Num_Reports", },
      { .name = "Primary_PHY",                                    .size = 1,   .ref2size = "Num_Reports", },
      { .name = "Secondary_PHY",                                  .size = 1,   .ref2size = "Num_Reports", },
      { .name = "Advertising_SID",                                .size = 1,   .ref2size = "Num_Reports", },
      { .name = "TX_Power",                                       .size = 1,   .ref2size = "Num_Reports", },
      { .name = "RSSI",                                           .size = 1,   .ref2size = "Num_Reports", },
      { .name = "Periodic_Advertising_Interval",                  .size = 2,   .ref2size = "Num_Reports", },
      { .name = "Direct_Address_Type",                            .size = 1,   .ref2size = "Num_Reports", },
      { .name = "Direct_Address",                                 .size = 6,   .ref2size = "Num_Reports", },
      { .name = "Data_Length",                                    .size = 1,   .ref2size = "Num_Reports", },
      { .name = "Data",                                           .size = 1,   .ref2size = "Data_Length", .mod = Size_Sum, },
    },
  },

  { .name = "LE Extended Create Connection command", .iscmd = 1, .section = "7.8.66", .version = "5.0", .BREDR = "E", .LE = "C20",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0043, .var = 0, .octet = 37, .bit = 7 },
    },
    .summary = "The LE Extended Create Connection Command is used to create a new "
               "connection supporting different initiating PHYs.",

    .numCmd = 13, .Cmd = (SpecField_t[13]) {
      { .name = "Initiator_Filter_Policy",                        .size = 1,    },
      { .name = "Own_Address_Type",                               .size = 1,    },
      { .name = "Peer_Address_Type",                              .size = 1,    },
      { .name = "Peer_Address",                                   .size = 6,    },
      { .name = "Initiating_PHYs",                                .size = 1,    },
      { .name = "Scan_Interval",                                  .size = 2,   .ref2size = "Initiating_PHYs", .mod = Size_Bits_Set, },
      { .name = "Scan_Window",                                    .size = 2,   .ref2size = "Initiating_PHYs", .mod = Size_Bits_Set, },
      { .name = "Connection_Interval_Min",                        .size = 2,   .ref2size = "Initiating_PHYs", .mod = Size_Bits_Set, },
      { .name = "Connection_Interval_Max",                        .size = 2,   .ref2size = "Initiating_PHYs", .mod = Size_Bits_Set, },
      { .name = "Max_Latency",                                    .size = 2,   .ref2size = "Initiating_PHYs", .mod = Size_Bits_Set, },
      { .name = "Supervision_Timeout",                            .size = 2,   .ref2size = "Initiating_PHYs", .mod = Size_Bits_Set, },
      { .name = "Min_CE_Length",                                  .size = 2,   .ref2size = "Initiating_PHYs", .mod = Size_Bits_Set, },
      { .name = "Max_CE_Length",                                  .size = 2,   .ref2size = "Initiating_PHYs", .mod = Size_Bits_Set, },
    },
    .numEvents = 3, .Events = (SpecEvent_t[3]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_LE_Extended_Advertising_Report" },
      { .name = "HCI_LE_Scan_Timeout" },
    },
  },

  { .name = "LE Generate DHKey command", .iscmd = 1, .section = "7.8.37", .version = "4.2 5.1", .BREDR = "E the", .LE = "[v1] C99 [v2] O",
    .numVI = 2, .VI = (SpecCmdVar_t[2]) {
      { .ocf = 0x005e, .var = 2, .octet = 41, .bit = 2 },
      { .ocf = 0x0026, .var = 1, .octet = 34, .bit = 2 },
    },
    .summary = "The HCI_LE_Generate_DHKey command is used to initiate generation of a "
               "Diffie-Hellman key in Controller for use over the LE transport.",

    .numCmd = 3, .Cmd = (SpecField_t[3]) {
      { .name = "Key_X_Coordinate",                               .size = 32,  .VS = { 0, 1, 1 },  },
      { .name = "Key_Y_Coordinate",                               .size = 32,  .VS = { 0, 1, 1 },  },
      { .name = "Key_Type",                                       .size = 1,   .VS = { 0, 0, 1 },  },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Status" },
    },
  },

  { .name = "LE Generate DHKey Complete event", .iscmd = 0, .subevent = 0x09, .section = "7.7.65.9", .version = "4.2", .BREDR = "E", .LE = "O",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003e, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_LE_Generate_DHKey_Complete event indicates that LE "
               "Diffie-Hellman key generation has been completed by the Controller.",

    .numCmd = 3, .Cmd = (SpecField_t[3]) {
      { .name = "Subevent_Code",                                  .size = 1,    },
      { .name = "Status",                                         .size = 1,    },
      { .name = "DH_Key",                                         .size = 32,   },
    },
  },

  { .name = "LE ISO Read Test Counters command", .iscmd = 1, .section = "7.8.113", .version = "5.2", .BREDR = "E", .LE = "C46",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0072, .var = 0, .octet = 43, .bit = 7 },
    },
    .summary = "The HCI_LE_ISO_Read_Test_Counter s command reads the test counters in "
               "the Controller which is configured in ISO Receive Test mode.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numRet = 5, .Ret = (SpecField_t[5]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Received_SDU_Count",                             .size = 4,    },
      { .name = "Missed_SDU_Count",                               .size = 4,    },
      { .name = "Failed_SDU_Count",                               .size = 4,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE ISO Receive Test command", .iscmd = 1, .section = "7.8.112", .version = "5.2", .BREDR = "E", .LE = "C46",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0071, .var = 0, .octet = 43, .bit = 6 },
    },
    .summary = "The HCI_LE_ISO_Receive_Test command configures a Link Layer to "
               "receive test payloads from an established CIS or a synchronized BIS.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Payload_Type",                                   .size = 1,    },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE ISO Test End command", .iscmd = 1, .section = "7.8.114", .version = "5.2", .BREDR = "E", .LE = "C47",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0073, .var = 0, .octet = 44, .bit = 0 },
    },
    .summary = "The HCI_LE_ISO_Test_End command terminates the ISO Transmit and/or "
               "Receive Test mode.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numRet = 5, .Ret = (SpecField_t[5]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Received_SDU_Count",                             .size = 4,    },
      { .name = "Missed_SDU_Count",                               .size = 4,    },
      { .name = "Failed_SDU_Count",                               .size = 4,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE ISO Transmit Test command", .iscmd = 1, .section = "7.8.111", .version = "5.2", .BREDR = "E", .LE = "C45",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0070, .var = 0, .octet = 43, .bit = 5 },
    },
    .summary = "The HCI_LE_ISO_Transmit_Test command configures an established CIS or "
               "BIS to transmit test payloads that are generated by the Controller.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Payload_Type",                                   .size = 1,    },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Long Term Key Request event", .iscmd = 0, .subevent = 0x05, .section = "7.7.65.5", .version = "4.0", .BREDR = "E", .LE = "C61",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003e, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_LE_Long_Term_Key_Request event indicates that a Long Term Key "
               "is required for a connection.",

    .numCmd = 4, .Cmd = (SpecField_t[4]) {
      { .name = "Subevent_Code",                                  .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Random_Number",                                  .size = 8,    },
      { .name = "Encrypted_Diversifier",                          .size = 2,    },
    },
  },

  { .name = "LE Long Term Key Request Negative Reply command", .iscmd = 1, .section = "7.8.26", .version = "4.0", .BREDR = "E", .LE = "C61",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x001b, .var = 0, .octet = 28, .bit = 2 },
    },
    .summary = "The HCI_LE_Long_Term_Key_Request_Negative_Reply command is used to "
               "reply to an HCI_LE_Long_Term_Key_Request event and indicates that the "
               "Host does not have a Long Term Key for that connection.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Long Term Key Request Reply command", .iscmd = 1, .section = "7.8.25", .version = "4.0", .BREDR = "E", .LE = "C61",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x001a, .var = 0, .octet = 28, .bit = 1 },
    },
    .summary = "The HCI_LE_Long_Term_Key_Request_Reply command is used to reply to an "
               "HCI_LE_Long_Term_Key_Request event and includes the Long Term Key "
               "stored in the Host for that connection.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Long_Term_Key",                                  .size = 16,   },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numEvents = 4, .Events = (SpecEvent_t[4]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Encryption_Change" },
      { .name = "HCI_Encryption_Key_Refresh_Complete" },
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Modify Sleep Clock Accuracy command", .iscmd = 1, .section = "7.8.94", .version = "5.1", .BREDR = "E", .LE = "C37",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x005f, .var = 0, .octet = 41, .bit = 4 },
    },
    .summary = "The HCI_LE_Modify_Sleep_Clock Accuracy command requests the "
               "Controller changes its sleep clock accuracy for testing purposes.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Action",                                         .size = 1,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
  },

  { .name = "LE Path Loss Threshold event", .iscmd = 0, .subevent = 0x20, .section = "7.7.65.32", .version = "5.2", .BREDR = "E", .LE = "C52",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003e, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_LE_Path_Loss_Threshold event is used to report a path loss "
               "threshold crossing on an ACL connection.",

    .numCmd = 4, .Cmd = (SpecField_t[4]) {
      { .name = "Subevent_Code",                                  .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Current_Path_Loss",                              .size = 1,    },
      { .name = "Zone_Entered",                                   .size = 1,    },
    },
  },

  { .name = "LE Periodic Advertising Create Sync Cancel command", .iscmd = 1, .section = "7.8.68", .version = "5.0", .BREDR = "E", .LE = "C16",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0045, .var = 0, .octet = 38, .bit = 1 },
    },
    .summary = "The HCI_LE_Periodic_Advertising_Create_Sync_Cancel command is used to "
               "cancel a pending HCI_LE_Periodic_Advertising_Create_Sync command.",

    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 3, .Events = (SpecEvent_t[3]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_LE_Periodic_Advertising_Sync_Established" },
      { .name = "HCI_LE_Periodic_Advertising_Report" },
    },
  },

  { .name = "LE Periodic Advertising Create Sync command", .iscmd = 1, .section = "7.8.67", .version = "5.0", .BREDR = "E", .LE = "C16",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0044, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_LE_Periodic_Advertising_Create_Sync command is used to start "
               "receiving periodic advertising packets from an advertiser.",

    .numCmd = 7, .Cmd = (SpecField_t[7]) {
      { .name = "Options",                                        .size = 1,    },
      { .name = "Advertising_SID",                                .size = 1,    },
      { .name = "Advertiser_Address_Type",                        .size = 1,    },
      { .name = "Advertiser_Address",                             .size = 6,    },
      { .name = "Skip",                                           .size = 2,    },
      { .name = "Sync_Timeout",                                   .size = 2,    },
      { .name = "Sync_CTE_Type",                                  .size = 1,    },
    },
    .numEvents = 3, .Events = (SpecEvent_t[3]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_LE_Enhanced_Connection_Complete" },
      { .name = "HCI_LE_Channel_Selection_Algorithm" },
    },
  },

  { .name = "LE Periodic Advertising Report event", .iscmd = 0, .subevent = 0x0f, .section = "7.7.65.15", .version = "5.0", .BREDR = "E", .LE = "C21",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003e, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_LE_Periodic_Advertising_Report event indicates that a "
               "periodic advertising packet has been received.",

    .numCmd = 8, .Cmd = (SpecField_t[8]) {
      { .name = "Subevent_Code",                                  .size = 1,    },
      { .name = "Sync_Handle",                                    .size = 2,    .numb = 12 },
      { .name = "TX_Power",                                       .size = 1,    },
      { .name = "RSSI",                                           .size = 1,    },
      { .name = "CTE_Type",                                       .size = 1,    },
      { .name = "Data_Status",                                    .size = 1,    },
      { .name = "Data_Length",                                    .size = 1,    },
      { .name = "Data",                                           .size = 1,   .ref2size = "Data_Length", },
    },
  },

  { .name = "LE Periodic Advertising Set Info Transfer command", .iscmd = 1, .section = "7.8.90", .version = "5.1", .BREDR = "E is", .LE = "C34",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x005b, .var = 0, .octet = 40, .bit = 7 },
    },
    .summary = "The HCI_LE_Periodic_Advertising_Set_Info_Transfer command is used to "
               "send periodic advertising synchronization information, describing "
               "periodic advertising events that the Controller transmitting, to a "
               "connected Controller.",

    .numCmd = 3, .Cmd = (SpecField_t[3]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Service_Data",                                   .size = 2,    },
      { .name = "Advertising_Handle",                             .size = 1,    },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Periodic Advertising Sync Established event", .iscmd = 0, .subevent = 0x0e, .section = "7.7.65.14", .version = "5.0", .BREDR = "E", .LE = "C16",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003e, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_LE_Periodic_Advertising_Sync_Established event indicates that "
               "the Controller has started receiving periodic advertising packets "
               "from an advertiser.",

    .numCmd = 9, .Cmd = (SpecField_t[9]) {
      { .name = "Subevent_Code",                                  .size = 1,    },
      { .name = "Status",                                         .size = 1,    },
      { .name = "Sync_Handle",                                    .size = 2,    .numb = 12 },
      { .name = "Advertising_SID",                                .size = 1,    },
      { .name = "Advertiser_Address_Type",                        .size = 1,    },
      { .name = "Advertiser_Address",                             .size = 6,    },
      { .name = "Advertiser_PHY",                                 .size = 1,    },
      { .name = "Periodic_Advertising_Interval",                  .size = 2,    },
      { .name = "Advertiser_Clock_Accuracy",                      .size = 1,    },
    },
  },

  { .name = "LE Periodic Advertising Sync Lost event", .iscmd = 0, .subevent = 0x10, .section = "7.7.65.16", .version = "5.0", .BREDR = "E", .LE = "C21",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003e, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_LE_Periodic_Advertising_Sync_Lost event indicates the "
               "Controller has ended receiving a periodic advertising train.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Subevent_Code",                                  .size = 1,    },
      { .name = "Sync_Handle",                                    .size = 2,    .numb = 12 },
    },
  },

  { .name = "LE Periodic Advertising Sync Transfer command", .iscmd = 1, .section = "7.8.89", .version = "5.1", .BREDR = "E", .LE = "C33",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x005a, .var = 0, .octet = 40, .bit = 6 },
    },
    .summary = "The HCI_LE_Periodic_Advertising_Sync_Transfer command is used to send "
               "periodic advertising synchronization information to a connected "
               "Controller.",

    .numCmd = 3, .Cmd = (SpecField_t[3]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Service_Data",                                   .size = 2,    },
      { .name = "Sync_Handle",                                    .size = 2,    .numb = 12 },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Periodic Advertising Sync Transfer Received event", .iscmd = 0, .subevent = 0x18, .section = "7.7.65.24", .version = "5.1", .BREDR = "E", .LE = "C35",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003e, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_LE_Periodic_Advertising_Sync_Transfer_Received event reports "
               "reception of periodic advertising synchronization information from a "
               "connected Controller.",

    .numCmd = 11, .Cmd = (SpecField_t[11]) {
      { .name = "Subevent_Code",                                  .size = 1,    },
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Service_Data",                                   .size = 2,    },
      { .name = "Sync_Handle",                                    .size = 2,    .numb = 12 },
      { .name = "Advertising_SID",                                .size = 1,    },
      { .name = "Advertiser_Address_Type",                        .size = 1,    },
      { .name = "Advertiser_Address",                             .size = 6,    },
      { .name = "Advertiser_PHY",                                 .size = 1,    },
      { .name = "Periodic_Advertising_Interval",                  .size = 2,    },
      { .name = "Advertiser_Clock_Accuracy",                      .size = 1,    },
    },
  },

  { .name = "LE Periodic Advertising Terminate Sync command", .iscmd = 1, .section = "7.8.69", .version = "5.0", .BREDR = "E", .LE = "C21",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0046, .var = 0, .octet = 38, .bit = 2 },
    },
    .summary = "The HCI_LE_Periodic_Advertising_Terminate_Sync command is used to end "
               "receiving of a periodic advertising train.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Sync_Handle",                                    .size = 2,    .numb = 12 },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 2, .Events = (SpecEvent_t[2]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_LE_Periodic_Advertising_Sync_Established" },
    },
  },

  { .name = "LE PHY Update Complete event", .iscmd = 0, .subevent = 0x0c, .section = "7.7.65.12", .version = "5.0", .BREDR = "E", .LE = "C11",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003e, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_LE_PHY_Update_Complete event is used to inform the Host of "
               "the current PHY.",

    .numCmd = 5, .Cmd = (SpecField_t[5]) {
      { .name = "Subevent_Code",                                  .size = 1,    },
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "TX_PHY",                                         .size = 1,    },
      { .name = "RX_PHY",                                         .size = 1,    },
    },
  },

  { .name = "LE Rand command", .iscmd = 1, .section = "7.8.23", .version = "4.0", .BREDR = "E", .LE = "C4",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0018, .var = 0, .octet = 27, .bit = 7 },
    },
    .summary = "The HCI_LE_Rand command will generate a random number.",

    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Random_Number",                                  .size = 8,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Read Advertising Physical Channel Tx Power command", .iscmd = 1, .section = "7.8.6", .version = "4.0", .BREDR = "E", .LE = "C97",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0007, .var = 0, .octet = 25, .bit = 6 },
    },
    .summary = "The HCI_LE_Read_Advertising_Physical_Channel_Tx_Power command will "
               "read the transmit power level that will be used for advertising.",

    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "TX_Power_Level",                                 .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Read Antenna Information command", .iscmd = 1, .section = "7.8.87", .version = "5.1", .BREDR = "E", .LE = "C31",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0058, .var = 0, .octet = 40, .bit = 4 },
    },
    .summary = "The HCI_LE_Read_Antenna_Information command allows the Host to read "
               "the switching rates, the sampling rates, the number of antennae, and "
               "the maximum length of the Constant Tone Extension supported by the "
               "Controller.",

    .numRet = 5, .Ret = (SpecField_t[5]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Supported_Switching_Sampling_Rates",             .size = 1,    },
      { .name = "Num_Antennae",                                   .size = 1,    },
      { .name = "Max_Switching_Pattern_Length",                   .size = 1,    },
      { .name = "Max_CTE_Length",                                 .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Read Buffer Size command", .iscmd = 1, .section = "7.8.2", .version = "4.0", .BREDR = "E", .LE = "[v1] C3 [v2] C55",
    .numVI = 2, .VI = (SpecCmdVar_t[2]) {
      { .ocf = 0x0060, .var = 2, .octet = 41, .bit = 5 },
      { .ocf = 0x0002, .var = 1, .octet = 25, .bit = 1 },
    },
    .summary = "The HCI_LE_Read_Buffer_Size command returns the size of the HCI "
               "buffers. These buffers are used by the LE Controller to buffer data "
               "that is to be transmitted.",

    .numRet = 5, .Ret = (SpecField_t[5]) {
      { .name = "Status",                                         .size = 1,   .VS = { 0, 1, 1 },  },
      { .name = "LE_ACL_Data_Packet_Length",                      .size = 2,   .VS = { 0, 1, 1 },  },
      { .name = "Total_Num_LE_ACL_Data_Packets",                  .size = 1,   .VS = { 0, 1, 1 },  },
      { .name = "ISO_Data_Packet_Length",                         .size = 2,   .VS = { 0, 0, 1 },  },
      { .name = "Total_Num_ISO_Data_Packets",                     .size = 1,   .VS = { 0, 0, 1 },  },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Read Channel Map command", .iscmd = 1, .section = "7.8.20", .version = "4.0", .BREDR = "E", .LE = "C3",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0015, .var = 0, .octet = 27, .bit = 4 },
    },
    .summary = "The HCI_LE_Read_Channel_Map command will read the current state of "
               "the channel map for a connection.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numRet = 3, .Ret = (SpecField_t[3]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Channel_Map",                                    .size = 5,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Read Filter Accept List Size command", .iscmd = 1, .section = "7.8.14", .version = "4.0", .BREDR = "E", .LE = "M",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x000f, .var = 0, .octet = 26, .bit = 6 },
    },
    .summary = "The HCI_LE_Read_Filter_Accept_List_Size command will read the maximum "
               "number of Filter Accept List entries that this Controller supports.",

    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Filter_Accept_List_Size",                        .size = 1,    },
    },
    .numEvents = 2, .Events = (SpecEvent_t[2]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_LE_Enhanced_Connection_Complete" },
    },
  },

  { .name = "LE Read ISO Link Quality command", .iscmd = 1, .section = "7.8.116", .version = "5.2", .BREDR = "E", .LE = "C50",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0075, .var = 0, .octet = 44, .bit = 2 },
    },
    .summary = "The HCI_LE_Read_ISO_Link_Quality command returns the value of various "
               "counters related to link quality on an isochronous stream.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numRet = 9, .Ret = (SpecField_t[9]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "TX_UnACKed_Packets",                             .size = 4,    },
      { .name = "TX_Flushed_Packets",                             .size = 4,    },
      { .name = "TX_Last_Subevent_Packets",                       .size = 4,    },
      { .name = "Retransmitted_Packets",                          .size = 4,    },
      { .name = "CRC_Error_Packets",                              .size = 4,    },
      { .name = "RX_Unreceived_Packets",                          .size = 4,    },
      { .name = "Duplicate_Packets",                              .size = 4,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Read ISO TX Sync command", .iscmd = 1, .section = "7.8.96", .version = "5.2", .BREDR = "E", .LE = "C45",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0061, .var = 0, .octet = 41, .bit = 6 },
    },
    .summary = "The HCI_LE_Read_ISO_TX_Sync command is used to read the Time_Stamp "
               "and Time_Offset of a transmitted SDU.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numRet = 5, .Ret = (SpecField_t[5]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Packet_Sequence_Number",                         .size = 2,    },
      { .name = "TX_Time_Stamp",                                  .size = 4,    },
      { .name = "Time_Offset",                                    .size = 3,    },
    },
  },

  { .name = "LE Read Local P-256 Public Key command", .iscmd = 1, .section = "7.8.36", .version = "4.2", .BREDR = "E key", .LE = "O",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0025, .var = 0, .octet = 34, .bit = 1 },
    },
    .summary = "The HCI_LE_Read_Local_P256_Public_Key command is used to return the "
               "local P-256 public from the Controller.",

    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Read Local P-256 Public Key Complete event", .iscmd = 0, .subevent = 0x08, .section = "7.7.65.8", .version = "4.2", .BREDR = "E", .LE = "O",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003e, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_LE_Read_Local_P256_Public_Key_Complete event is generated "
               "when local P-256 key generation is complete.",

    .numCmd = 4, .Cmd = (SpecField_t[4]) {
      { .name = "Subevent_Code",                                  .size = 1,    },
      { .name = "Status",                                         .size = 1,    },
      { .name = "Key_X_Coordinate",                               .size = 32,   },
      { .name = "Key_Y_Coordinate",                               .size = 32,   },
    },
  },

  { .name = "LE Read Local Resolvable Address command", .iscmd = 1, .section = "7.8.43", .version = "4.2", .BREDR = "E", .LE = "C10",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x002c, .var = 0, .octet = 35, .bit = 0 },
    },
    .summary = "The HCI_LE_Read_Local_Resolvable_Address command is used to get the "
               "current local Resolvable Private Address being used for the "
               "corresponding peer Identity Address.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Peer_Identity_Address_Type",                     .size = 1,    },
      { .name = "Peer_Identity_Address",                          .size = 6,    },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Local_Resolvable_Address",                       .size = 6,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Read Local Supported Features command", .iscmd = 1, .section = "7.8.3", .version = "4.0", .BREDR = "E", .LE = "M",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0003, .var = 0, .octet = 25, .bit = 2 },
    },
    .summary = "The HCI_LE_Read_Local_Supported_Features command will read the "
               "version information for the local LE Controller.",

    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "LE_Features",                                    .size = 8,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Read Maximum Advertising Data Length command", .iscmd = 1, .section = "7.8.57", .version = "5.0", .BREDR = "E a", .LE = "C17",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003a, .var = 0, .octet = 36, .bit = 6 },
    },
    .summary = "The HCI_LE_Read_Maximum_Advertising_Data_Length command will read the "
               "maximum length of advertising data that the advertising Controller "
               "supports in given advertising set.",

    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Max_Advertising_Data_Length",                    .size = 2,    },
    },
    .numEvents = 3, .Events = (SpecEvent_t[3]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_LE_Advertising_Set_Terminated" },
      { .name = "HCI_LE_Enhanced_Connection_Complete" },
    },
  },

  { .name = "LE Read Maximum Data Length command", .iscmd = 1, .section = "7.8.46", .version = "4.2", .BREDR = "E", .LE = "C8",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x002f, .var = 0, .octet = 35, .bit = 3 },
    },
    .summary = "The HCI_LE_Read_Maximum_Data_Length command allows the Host to read "
               "the Controller’s supportedMaxTxOctets, supportedMaxTxTime, "
               "supportedMaxRxOctets, and supportedMaxRxTime parameters.",

    .numRet = 5, .Ret = (SpecField_t[5]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Supported_Max_TX_Octets",                        .size = 2,    },
      { .name = "Supported_Max_TX_Time",                          .size = 2,    },
      { .name = "Supported_Max_RX_Octets",                        .size = 2,    },
      { .name = "Supported_Max_RX_Time",                          .size = 2,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Read Number of Supported Advertising Sets command", .iscmd = 1, .section = "7.8.58", .version = "5.0", .BREDR = "E", .LE = "C17",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003b, .var = 0, .octet = 36, .bit = 7 },
    },
    .summary = "The HCI_LE_Read_Number_of_Supported_Advertising_Sets command will "
               "read the maximum number of advertising sets supported by the "
               "advertising Controller at the same time.",

    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Num_Supported_Advertising_Sets",                 .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Read Peer Resolvable Address command", .iscmd = 1, .section = "7.8.42", .version = "4.2", .BREDR = "E", .LE = "C10",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x002b, .var = 0, .octet = 34, .bit = 7 },
    },
    .summary = "The HCI_LE_Read_Peer_Resolvable_Address command is used to get the "
               "current peer Resolvable Private Address being used for the "
               "corresponding peer Public and Random (static) Identity Address.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Peer_Identity_Address_Type",                     .size = 1,    },
      { .name = "Peer_Identity_Address",                          .size = 6,    },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Peer_Resolvable_Address",                        .size = 6,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Read Periodic Advertiser List Size command", .iscmd = 1, .section = "7.8.73", .version = "5.0", .BREDR = "E that", .LE = "C21",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x004a, .var = 0, .octet = 38, .bit = 6 },
    },
    .summary = "The HCI_LE_Read_Periodic_Advertiser_List_Size command will read the "
               "maximum number of Periodic Advertiser List entries the Controller "
               "supports.",

    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Periodic_Advertiser_List_Size",                  .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Read PHY command", .iscmd = 1, .section = "7.8.47", .version = "5.0", .BREDR = "E", .LE = "C11",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0030, .var = 0, .octet = 35, .bit = 4 },
    },
    .summary = "The HCI_LE_Read_PHY command will read the current PHY.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numRet = 4, .Ret = (SpecField_t[4]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "TX_PHY",                                         .size = 1,    },
      { .name = "RX_PHY",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Read Remote Features command", .iscmd = 1, .section = "7.8.21", .version = "4.0", .BREDR = "E", .LE = "C3",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0016, .var = 0, .octet = 27, .bit = 5 },
    },
    .summary = "The HCI_LE_Read_Remote_Features command is used to read the features "
               "used on a connection and the features supported by a remote LE "
               "device.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Read Remote Features Complete event", .iscmd = 0, .subevent = 0x04, .section = "7.7.65.4", .version = "4.0", .BREDR = "E", .LE = "C3",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003e, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_LE_Read_Remote_Features_Complete event indicates the "
               "completion of the process to read the features used on a connection "
               "and the features supported by a remote LE device.",

    .numCmd = 4, .Cmd = (SpecField_t[4]) {
      { .name = "Subevent_Code",                                  .size = 1,    },
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "LE_Features",                                    .size = 8,    },
    },
  },

  { .name = "LE Read Remote Transmit Power Level command", .iscmd = 1, .section = "7.8.118", .version = "5.2", .BREDR = "E", .LE = "C51",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0077, .var = 0, .octet = 44, .bit = 4 },
    },
    .summary = "The HCI_LE_Read_Remote_Transmit_Power_Level command is used to read "
               "the transmit power level used by the remote Controller on a specified "
               "PHY on an ACL connection.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "PHY",                                            .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Read Resolving List Size command", .iscmd = 1, .section = "7.8.41", .version = "4.2", .BREDR = "E stored", .LE = "C9",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x002a, .var = 0, .octet = 34, .bit = 6 },
    },
    .summary = "The HCI_LE_Read_Resolving_List_Size command is used to read the total "
               "number of entries in the resolving list that can be in the "
               "Controller.",

    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Resolving_List_Size",                            .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Read RF Path Compensation command", .iscmd = 1, .section = "7.8.75", .version = "5.0", .BREDR = "E", .LE = "C22",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x004c, .var = 0, .octet = 39, .bit = 0 },
    },
    .summary = "The HCI_LE_Read_RF_Path_Compensation command is used to read the RF "
               "Path Compensation Value.",

    .numRet = 3, .Ret = (SpecField_t[3]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "RF_TX_Path_Compensation_Value",                  .size = 2,    },
      { .name = "RF_RX_Path_Compensation_Value",                  .size = 2,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Read Suggested Default Data Length command", .iscmd = 1, .section = "7.8.34", .version = "4.2", .BREDR = "E", .LE = "C8",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0023, .var = 0, .octet = 33, .bit = 7 },
    },
    .summary = "The HCI_LE_Read_Suggested_Default_Data_Length command allows the Host "
               "to read the initial MaxTxOctets and MaxTxTime values for new "
               "connections it suggested to the Controller.",

    .numRet = 3, .Ret = (SpecField_t[3]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Suggested_Max_TX_Octets",                        .size = 2,    },
      { .name = "Suggested_Max_TX_Time",                          .size = 2,    },
    },
    .numEvents = 2, .Events = (SpecEvent_t[2]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_LE_Data_Length_Change" },
    },
  },

  { .name = "LE Read Supported States command", .iscmd = 1, .section = "7.8.27", .version = "4.0", .BREDR = "E", .LE = "M",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x001c, .var = 0, .octet = 28, .bit = 3 },
    },
    .summary = "The HCI_LE_Read_Supported_States command will read the current "
               "supported state and role combinations for the local LE Controllers.",

    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "LE_States",                                      .size = 8,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Read Transmit Power command", .iscmd = 1, .section = "7.8.74", .version = "5.0", .BREDR = "E", .LE = "C64",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x004b, .var = 0, .octet = 38, .bit = 7 },
    },
    .summary = "The HCI_LE_Read_Transmit_Power command will read the minimum and "
               "maximum transmit powers supported by the Controller.",

    .numRet = 3, .Ret = (SpecField_t[3]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Min_TX_Power",                                   .size = 1,    },
      { .name = "Max_TX_Power",                                   .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Receiver Test command", .iscmd = 1, .section = "7.8.28", .version = "4.0 5.0 5.1", .BREDR = "E", .LE = "[v1] C2 [v2] C13 [v3] C30",
    .numVI = 3, .VI = (SpecCmdVar_t[3]) {
      { .ocf = 0x004f, .var = 3, .octet = 39, .bit = 3 },
      { .ocf = 0x0033, .var = 2, .octet = 35, .bit = 7 },
      { .ocf = 0x001d, .var = 1, .octet = 28, .bit = 4 },
    },
    .summary = "The HCI_LE_Receiver_Test command will run the LE receiver test.",

    .numCmd = 8, .Cmd = (SpecField_t[8]) {
      { .name = "RX_Channel",                                     .size = 1,   .VS = { 0, 1, 1, 1 },  },
      { .name = "PHY",                                            .size = 1,   .VS = { 0, 0, 1, 1 },  },
      { .name = "Modulation_Index",                               .size = 1,   .VS = { 0, 0, 1, 1 },  },
      { .name = "Expected_CTE_Length",                            .size = 1,   .VS = { 0, 0, 0, 1 },  },
      { .name = "Expected_CTE_Type",                              .size = 1,   .VS = { 0, 0, 0, 1 },  },
      { .name = "Slot_Durations",                                 .size = 1,   .VS = { 0, 0, 0, 1 },  },
      { .name = "Switching_Pattern_Length",                       .size = 1,   .VS = { 0, 0, 0, 1 },  },
      { .name = "Antenna_IDs",                                    .size = 1,   .VS = { 0, 0, 0, 1 }, .ref2size = "Switching_Pattern_Length", },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,   .VS = { 0, 1, 1, 1 },  },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Reject CIS Request command", .iscmd = 1, .section = "7.8.102", .version = "5.2", .BREDR = "E for", .LE = "C40",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0067, .var = 0, .octet = 42, .bit = 4 },
    },
    .summary = "The HCI_LE_Reject_CIS_Request command is used by the Peripheral’s "
               "Host to inform the Controller to reject the request creating the "
               "CIS.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Reason",                                         .size = 1,    },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numEvents = 2, .Events = (SpecEvent_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_LE_CIS_Established" },
    },
  },

  { .name = "LE Remote Connection Parameter Request event", .iscmd = 0, .subevent = 0x06, .section = "7.7.65.6", .version = "4.1", .BREDR = "E", .LE = "C6",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003e, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The LE Remote Connection Parameter Request event is used to indicate "
               "to the Host that the remote device is requesting a change in the "
               "connection parameters.",

    .numCmd = 6, .Cmd = (SpecField_t[6]) {
      { .name = "Subevent_Code",                                  .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Interval_Min",                                   .size = 2,    },
      { .name = "Interval_Max",                                   .size = 2,    },
      { .name = "Max_Latency",                                    .size = 2,    },
      { .name = "Timeout",                                        .size = 2,    },
    },
  },

  { .name = "LE Remote Connection Parameter Request Negative Reply command", .iscmd = 1, .section = "7.8.32", .version = "4.1", .BREDR = "E", .LE = "C6",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0021, .var = 0, .octet = 33, .bit = 5 },
    },
    .summary = "The HCI_LE_Remote_Connection_Parameter_Request_Negative_Reply command "
               "is used to reject the remote device’s request to change the "
               "connection parameters of the LE connection.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Reason",                                         .size = 1,    },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Remote Connection Parameter Request Reply command", .iscmd = 1, .section = "7.8.31", .version = "4.1", .BREDR = "E", .LE = "C6",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0020, .var = 0, .octet = 33, .bit = 4 },
    },
    .summary = "The HCI_LE_Remote_Connection_Parameter_Request_Reply command is used "
               "to accept the remote device’s request to change the connection "
               "parameters of the LE connection.",

    .numCmd = 7, .Cmd = (SpecField_t[7]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Interval_Min",                                   .size = 2,    },
      { .name = "Interval_Max",                                   .size = 2,    },
      { .name = "Max_Latency",                                    .size = 2,    },
      { .name = "Timeout",                                        .size = 2,    },
      { .name = "Min_CE_Length",                                  .size = 2,    },
      { .name = "Max_CE_Length",                                  .size = 2,    },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Remove Advertising Set command", .iscmd = 1, .section = "7.8.59", .version = "5.0", .BREDR = "E", .LE = "C17",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003c, .var = 0, .octet = 37, .bit = 0 },
    },
    .summary = "The HCI_LE_Remove_Advertising_Set command will remove an advertising "
               "set from the Controller.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Advertising_Handle",                             .size = 1,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Remove CIG command", .iscmd = 1, .section = "7.8.100", .version = "5.2", .BREDR = "E Host", .LE = "C39",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0065, .var = 0, .octet = 42, .bit = 2 },
    },
    .summary = "The HCI_LE_Remove_CIG command is used by the Central’s to remove a "
               "CIG from the Controller.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "CIG_ID",                                         .size = 1,    },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "CIG_ID",                                         .size = 1,    },
    },
    .numEvents = 2, .Events = (SpecEvent_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_LE_CIS_Established" },
    },
  },

  { .name = "LE Remove Device From Filter Accept List command", .iscmd = 1, .section = "7.8.17", .version = "4.0", .BREDR = "E", .LE = "M",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0012, .var = 0, .octet = 27, .bit = 1 },
    },
    .summary = "The HCI_LE_Remove_Device_From_Filter_Accept_List command will remove "
               "a single device from the Filter Accept List.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Address_Type",                                   .size = 1,    },
      { .name = "Address",                                        .size = 6,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Remove Device From Periodic Advertiser List command", .iscmd = 1, .section = "7.8.71", .version = "5.0", .BREDR = "E", .LE = "C21",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0048, .var = 0, .octet = 38, .bit = 4 },
    },
    .summary = "The HCI_LE_Remove_Device_From_Periodic_Advertiser_List command will "
               "remove a single device from the Periodic Advertiser List.",

    .numCmd = 3, .Cmd = (SpecField_t[3]) {
      { .name = "Advertiser_Address_Type",                        .size = 1,    },
      { .name = "Advertiser_Address",                             .size = 6,    },
      { .name = "Advertising_SID",                                .size = 1,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Remove Device From Resolving List command", .iscmd = 1, .section = "7.8.39", .version = "4.2", .BREDR = "E", .LE = "C9",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0028, .var = 0, .octet = 34, .bit = 4 },
    },
    .summary = "The HCI_LE_Remove_Device_From_Resolving_List command is used to "
               "remove one device from the resolving list used to resolve Resolvable "
               "Private Addresses in the Controller.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Peer_Identity_Address_Type",                     .size = 1,    },
      { .name = "Peer_Identity_Address",                          .size = 6,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Remove ISO Data Path command", .iscmd = 1, .section = "7.8.110", .version = "5.2", .BREDR = "E", .LE = "C47",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x006f, .var = 0, .octet = 43, .bit = 4 },
    },
    .summary = "The HCI_LE_Remove_ISO_Data_Path command removes an isochronous data "
               "path between the Host and the Controller.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Data_Path_Direction",                            .size = 1,    },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Request Peer SCA command", .iscmd = 1, .section = "7.8.108", .version = "5.2", .BREDR = "E", .LE = "C44",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x006d, .var = 0, .octet = 43, .bit = 2 },
    },
    .summary = "The HCI_LE_Request_Peer_SCA command requests the Sleep Clock Accuracy "
               "of the peer device.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numEvents = 2, .Events = (SpecEvent_t[2]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_LE_BIG_Sync_Established" },
    },
  },

  { .name = "LE Request Peer SCA Complete event", .iscmd = 0, .subevent = 0x1f, .section = "7.7.65.31", .version = "5.2", .BREDR = "E", .LE = "C95",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003e, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_LE_Request_Peer_SCA_Complete event indicates that the "
               "Controller completed the attempt to read the Sleep Clock Accuracy "
               "(SCA) of the peer device.",

    .numCmd = 4, .Cmd = (SpecField_t[4]) {
      { .name = "Subevent_Code",                                  .size = 1,    },
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Peer_Clock_Accuracy",                            .size = 1,    },
    },
  },

  { .name = "LE Scan Request Received event", .iscmd = 0, .subevent = 0x13, .section = "7.7.65.19", .version = "5.0", .BREDR = "E", .LE = "C17",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003e, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_LE_Scan_Request_Received event indicates that a scan request "
               "has been received.",

    .numCmd = 4, .Cmd = (SpecField_t[4]) {
      { .name = "Subevent_Code",                                  .size = 1,    },
      { .name = "Advertising_Handle",                             .size = 1,    },
      { .name = "Scanner_Address_Type",                           .size = 1,    },
      { .name = "Scanner_Address",                                .size = 6,    },
    },
  },

  { .name = "LE Scan Timeout event", .iscmd = 0, .subevent = 0x11, .section = "7.7.65.17", .version = "5.0", .BREDR = "E", .LE = "C19",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003e, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_LE_Scan_Timeout event indicates that scanning has finished.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Subevent_Code",                                  .size = 1,    },
    },
  },

  { .name = "LE Set Address Resolution Enable command", .iscmd = 1, .section = "7.8.44", .version = "4.2", .BREDR = "E", .LE = "C9",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x002d, .var = 0, .octet = 35, .bit = 1 },
    },
    .summary = "The HCI_LE_Set_Address_Resolution_Enable command is used to enable "
               "resolution of Resolvable Private Addresses in the Controller.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Address_Resolution_Enable",                      .size = 1,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Set Advertising Data command", .iscmd = 1, .section = "7.8.7", .version = "4.0", .BREDR = "E", .LE = "C97",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0008, .var = 0, .octet = 25, .bit = 7 },
    },
    .summary = "The HCI_LE_Set_Advertising_Data command will set the data transmitted "
               "when advertising.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Advertising_Data_Length",                        .size = 1,    },
      { .name = "Advertising_Data",                               .size = 31,   },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Set Advertising Enable command", .iscmd = 1, .section = "7.8.9", .version = "4.0", .BREDR = "E", .LE = "C97",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x000a, .var = 0, .octet = 26, .bit = 1 },
    },
    .summary = "The HCI_LE_Set_Advertising_Enable command will enable or disable "
               "advertising.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Advertising_Enable",                             .size = 1,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Set Advertising Parameters command", .iscmd = 1, .section = "7.8.5", .version = "4.0", .BREDR = "E", .LE = "C97",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0006, .var = 0, .octet = 25, .bit = 5 },
    },
    .summary = "The HCI_LE_Set_Advertising_Parameters command will set the parameters "
               "used for advertising.",

    .numCmd = 8, .Cmd = (SpecField_t[8]) {
      { .name = "Advertising_Interval_Min",                       .size = 2,    },
      { .name = "Advertising_Interval_Max",                       .size = 2,    },
      { .name = "Advertising_Type",                               .size = 1,    },
      { .name = "Own_Address_Type",                               .size = 1,    },
      { .name = "Peer_Address_Type",                              .size = 1,    },
      { .name = "Peer_Address",                                   .size = 6,    },
      { .name = "Advertising_Channel_Map",                        .size = 1,    },
      { .name = "Advertising_Filter_Policy",                      .size = 1,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Set Advertising Set Random Address command", .iscmd = 1, .section = "7.8.52", .version = "5.0", .BREDR = "E", .LE = "C17",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0035, .var = 0, .octet = 36, .bit = 1 },
    },
    .summary = "The HCI_LE_Set_Advertising_Set_Random_Address command will set the "
               "random address used in advertising.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Advertising_Handle",                             .size = 1,    },
      { .name = "Random_Address",                                 .size = 6,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
  },

  { .name = "LE Set CIG Parameters command", .iscmd = 1, .section = "7.8.97", .version = "5.2", .BREDR = "E", .LE = "C39",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0062, .var = 0, .octet = 41, .bit = 7 },
    },
    .summary = "The HCI_LE_Set_CIG_Parameters command is used by a Central’s Host "
               "to set the parameters of one or more Connected Isochronous Streams "
               "(CISes) that are associated with a CIG in the Controller.",

    .numCmd = 16, .Cmd = (SpecField_t[16]) {
      { .name = "CIG_ID",                                         .size = 1,    },
      { .name = "SDU_Interval_C_To_P",                            .size = 3,    },
      { .name = "SDU_Interval_P_To_C",                            .size = 3,    },
      { .name = "Worst_Case_SCA",                                 .size = 1,    },
      { .name = "Packing",                                        .size = 1,    },
      { .name = "Framing",                                        .size = 1,    },
      { .name = "Max_Transport_Latency_C_To_P",                   .size = 2,    },
      { .name = "Max_Transport_Latency_P_To_C",                   .size = 2,    },
      { .name = "CIS_Count",                                      .size = 1,    },
      { .name = "CIS_ID",                                         .size = 1,   .ref2size = "CIS_Count", },
      { .name = "Max_SDU_C_To_P",                                 .size = 2,   .ref2size = "CIS_Count", },
      { .name = "Max_SDU_P_To_C",                                 .size = 2,   .ref2size = "CIS_Count", },
      { .name = "PHY_C_To_P",                                     .size = 1,   .ref2size = "CIS_Count", },
      { .name = "PHY_P_To_C",                                     .size = 1,   .ref2size = "CIS_Count", },
      { .name = "RTN_C_To_P",                                     .size = 1,   .ref2size = "CIS_Count", },
      { .name = "RTN_P_To_C",                                     .size = 1,   .ref2size = "CIS_Count", },
    },
    .numRet = 4, .Ret = (SpecField_t[4]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "CIG_ID",                                         .size = 1,    },
      { .name = "CIS_Count",                                      .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,   .ref2size = "CIS_Count", .numb = 12 },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Set CIG Parameters Test command", .iscmd = 1, .section = "7.8.98", .version = "5.2", .BREDR = "E the purposes.", .LE = "C39",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0063, .var = 0, .octet = 42, .bit = 0 },
    },
    .summary = "The HCI_LE_Set_CIG_Parameters_Test command is used by a Central’s "
               "Host to set parameters of one or more CISes that are associated with "
               "a CIG in the Controller for testing",

    .numCmd = 20, .Cmd = (SpecField_t[20]) {
      { .name = "CIG_ID",                                         .size = 1,    },
      { .name = "SDU_Interval_C_To_P",                            .size = 3,    },
      { .name = "SDU_Interval_P_To_C",                            .size = 3,    },
      { .name = "FT_C_To_P",                                      .size = 1,    },
      { .name = "FT_P_To_C",                                      .size = 1,    },
      { .name = "ISO_Interval",                                   .size = 2,    },
      { .name = "Worst_Case_SCA",                                 .size = 1,    },
      { .name = "Packing",                                        .size = 1,    },
      { .name = "Framing",                                        .size = 1,    },
      { .name = "CIS_Count",                                      .size = 1,    },
      { .name = "CIS_ID",                                         .size = 1,   .ref2size = "CIS_Count", },
      { .name = "NSE",                                            .size = 1,   .ref2size = "CIS_Count", },
      { .name = "Max_SDU_C_To_P",                                 .size = 2,   .ref2size = "CIS_Count", },
      { .name = "Max_SDU_P_To_C",                                 .size = 2,   .ref2size = "CIS_Count", },
      { .name = "Max_PDU_C_To_P",                                 .size = 2,   .ref2size = "CIS_Count", },
      { .name = "Max_PDU_P_To_C",                                 .size = 2,   .ref2size = "CIS_Count", },
      { .name = "PHY_C_To_P",                                     .size = 1,   .ref2size = "CIS_Count", },
      { .name = "PHY_P_To_C",                                     .size = 1,   .ref2size = "CIS_Count", },
      { .name = "BN_C_To_P",                                      .size = 1,   .ref2size = "CIS_Count", },
      { .name = "BN_P_To_C",                                      .size = 1,   .ref2size = "CIS_Count", },
    },
    .numRet = 4, .Ret = (SpecField_t[4]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "CIG_ID",                                         .size = 1,    },
      { .name = "CIS_Count",                                      .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,   .ref2size = "CIS_Count", .numb = 12 },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Set Connection CTE Receive Parameters command", .iscmd = 1, .section = "7.8.83", .version = "5.1", .BREDR = "E", .LE = "C25",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0054, .var = 0, .octet = 40, .bit = 0 },
    },
    .summary = "The HCI_LE_Set_Connection_CTE_Receive_Parameters command will set the "
               "antennaswitching pattern, switching and sampling slot durations for "
               "receiving the Constant Tone Extension on a connection.",

    .numCmd = 5, .Cmd = (SpecField_t[5]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Sampling_Enable",                                .size = 1,    },
      { .name = "Slot_Durations",                                 .size = 1,    },
      { .name = "Switching_Pattern_Length",                       .size = 1,    },
      { .name = "Antenna_IDs",                                    .size = 1,   .ref2size = "Switching_Pattern_Length", },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numEvents = 2, .Events = (SpecEvent_t[2]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_LE_Connectionless_IQ_Report" },
    },
  },

  { .name = "LE Set Connection CTE Transmit Parameters command", .iscmd = 1, .section = "7.8.84", .version = "5.1", .BREDR = "E", .LE = "C26",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0055, .var = 0, .octet = 40, .bit = 1 },
    },
    .summary = "The HCI_LE_Set_Connection_CTE_Transmit_Parameters command will set "
               "the antennaswitching pattern, switching and sampling slot durations "
               "for transmitting the Constant Tone Extension on a connection.",

    .numCmd = 4, .Cmd = (SpecField_t[4]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "CTE_Types",                                      .size = 1,    },
      { .name = "Switching_Pattern_Length",                       .size = 1,    },
      { .name = "Antenna_IDs",                                    .size = 1,   .ref2size = "Switching_Pattern_Length", },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Set Connectionless CTE Transmit Enable command", .iscmd = 1, .section = "7.8.81", .version = "5.1", .BREDR = "E", .LE = "C27",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0052, .var = 0, .octet = 39, .bit = 6 },
    },
    .summary = "The HCI_LE_Set_Connectionless_CTE_Transmit_Enable command will "
               "request the Controller to enable or disable sending packets "
               "containing a Constant Tone Extension.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Advertising_Handle",                             .size = 1,    },
      { .name = "CTE_Enable",                                     .size = 1,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Set Connectionless CTE Transmit Parameters command", .iscmd = 1, .section = "7.8.80", .version = "5.1", .BREDR = "E", .LE = "C27",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0051, .var = 0, .octet = 39, .bit = 5 },
    },
    .summary = "The HCI_LE_Set_Connectionless_CTE_Transmit_Parameters command will "
               "set the antenna-switching pattern and switching and sampling slot "
               "durations for the transmission of Constant Tone Extensions.",

    .numCmd = 6, .Cmd = (SpecField_t[6]) {
      { .name = "Advertising_Handle",                             .size = 1,    },
      { .name = "CTE_Length",                                     .size = 1,    },
      { .name = "CTE_Type",                                       .size = 1,    },
      { .name = "CTE_Count",                                      .size = 1,    },
      { .name = "Switching_Pattern_Length",                       .size = 1,    },
      { .name = "Antenna_IDs",                                    .size = 1,   .ref2size = "Switching_Pattern_Length", },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
  },

  { .name = "LE Set Connectionless IQ Sampling Enable command", .iscmd = 1, .section = "7.8.82", .version = "5.1", .BREDR = "E tak-", .LE = "C28",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0053, .var = 0, .octet = 39, .bit = 7 },
    },
    .summary = "The HCI_LE_Set_Connectionless_IQ_Sampling_Enable command will request "
               "the Controller to enable or disable ing IQ samples from the Constant "
               "Tone Extension of advertising packets.",

    .numCmd = 6, .Cmd = (SpecField_t[6]) {
      { .name = "Sync_Handle",                                    .size = 2,    .numb = 12 },
      { .name = "Sampling_Enable",                                .size = 1,    },
      { .name = "Slot_Durations",                                 .size = 1,    },
      { .name = "Max_Sampled_CTEs",                               .size = 1,    },
      { .name = "Switching_Pattern_Length",                       .size = 1,    },
      { .name = "Antenna_IDs",                                    .size = 1,   .ref2size = "Switching_Pattern_Length", },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Sync_Handle",                                    .size = 2,    .numb = 12 },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Set Data Length command", .iscmd = 1, .section = "7.8.33", .version = "4.2", .BREDR = "E", .LE = "C8",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0022, .var = 0, .octet = 33, .bit = 6 },
    },
    .summary = "The HCI_LE_Set_Data_Length command is used to suggest maximum packet "
               "sizes to the Controller.",

    .numCmd = 3, .Cmd = (SpecField_t[3]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "TX_Octets",                                      .size = 2,    },
      { .name = "TX_Time",                                        .size = 2,    },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Set Data Related Address Changes command", .iscmd = 1, .section = "7.8.122", .version = "5.3", .BREDR = "E", .LE = "C10",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x007c, .var = 0, .octet = 45, .bit = 6 },
    },
    .summary = "The HCI_LE_Set_Data_Related_Address_Changes command specifies "
               "circumstances when the Controller shall refresh any Resolvable "
               "Private Address used by an advertising set, whether or not the "
               "address timeout period has been reached.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Advertising_Handle",                             .size = 1,    },
      { .name = "Change_Reasons",                                 .size = 1,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 2, .Events = (SpecEvent_t[2]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_LE_Transmit_Power_Reporting" },
    },
  },

  { .name = "LE Set Default Periodic Advertising Sync Transfer Parameters command", .iscmd = 1, .section = "7.8.92", .version = "5.1", .BREDR = "E", .LE = "C35",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x005d, .var = 0, .octet = 41, .bit = 1 },
    },
    .summary = "The HCI_LE_Set_Default_Periodic_Advertising_Sync_Transfer_Parameters "
               "command is used to specify the default behavior of the Controller "
               "when periodic advertising synchronization information is received "
               "from a connected Controller.",

    .numCmd = 4, .Cmd = (SpecField_t[4]) {
      { .name = "Mode",                                           .size = 1,    },
      { .name = "Skip",                                           .size = 2,    },
      { .name = "Sync_Timeout",                                   .size = 2,    },
      { .name = "CTE_Type",                                       .size = 1,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Set Default PHY command", .iscmd = 1, .section = "7.8.48", .version = "5.0", .BREDR = "E", .LE = "C11",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0031, .var = 0, .octet = 35, .bit = 5 },
    },
    .summary = "The HCI_LE_Set_Default_PHY command is used to configure preferred "
               "PHYs for new connections for the local device.",

    .numCmd = 3, .Cmd = (SpecField_t[3]) {
      { .name = "All_PHYs",                                       .size = 1,    },
      { .name = "TX_PHYs",                                        .size = 1,    },
      { .name = "RX_PHYs",                                        .size = 1,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Set Default Subrate command", .iscmd = 1, .section = "7.8.123", .version = "5.3", .BREDR = "E", .LE = "C57",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x007d, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_LE_Set_Default_Subrate command sets the range of the min and "
               "max subrates and other subrate parameters on a Central that may be "
               "requested by a Peripheral.",

    .numCmd = 5, .Cmd = (SpecField_t[5]) {
      { .name = "Subrate_Min",                                    .size = 2,    },
      { .name = "Subrate_Max",                                    .size = 2,    },
      { .name = "Max_Latency",                                    .size = 2,    },
      { .name = "Continuation_Number",                            .size = 2,    },
      { .name = "Supervision_Timeout",                            .size = 2,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Set Event Mask command", .iscmd = 1, .section = "7.8.1", .version = "4.0", .BREDR = "E", .LE = "M",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0001, .var = 0, .octet = 25, .bit = 0 },
    },
    .summary = "The HCI_LE_Set_Event_Mask command is used to control which events are "
               "generated by the HCI for the Host.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "LE_Event_Mask",                                  .size = 8,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
  },

  { .name = "LE Set Extended Advertising Data command", .iscmd = 1, .section = "7.8.54", .version = "5.0", .BREDR = "E", .LE = "C17",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0037, .var = 0, .octet = 36, .bit = 3 },
    },
    .summary = "The HCI_LE_Set_Extended_Advertising_Data command will set the "
               "advertising data transmitted when advertising.",

    .numCmd = 5, .Cmd = (SpecField_t[5]) {
      { .name = "Advertising_Handle",                             .size = 1,    },
      { .name = "Operation",                                      .size = 1,    },
      { .name = "Fragment_Preference",                            .size = 1,    },
      { .name = "Advertising_Data_Length",                        .size = 1,    },
      { .name = "Advertising_Data",                               .size = 1,   .ref2size = "Advertising_Data_Length", },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Set Extended Advertising Enable command", .iscmd = 1, .section = "7.8.56", .version = "5.0", .BREDR = "E", .LE = "C17",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0039, .var = 0, .octet = 36, .bit = 5 },
    },
    .summary = "The HCI_LE_Set_Extended_Advertising_Enable command will enable or "
               "disable advertising.",

    .numCmd = 5, .Cmd = (SpecField_t[5]) {
      { .name = "Enable",                                         .size = 1,    },
      { .name = "Num_Sets",                                       .size = 1,    },
      { .name = "Advertising_Handle",                             .size = 1,   .ref2size = "Num_Sets", },
      { .name = "Duration",                                       .size = 2,   .ref2size = "Num_Sets", },
      { .name = "Max_Extended_Advertising_Events",                .size = 1,   .ref2size = "Num_Sets", },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Set Extended Advertising Parameters command", .iscmd = 1, .section = "7.8.53", .version = "5.0", .BREDR = "E", .LE = "C17",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0036, .var = 0, .octet = 36, .bit = 2 },
    },
    .summary = "The HCI_LE_Set_Extended_Advertising_Parameters command will set the "
               "parameters used for advertising.",

    .numCmd = 15, .Cmd = (SpecField_t[15]) {
      { .name = "Advertising_Handle",                             .size = 1,    },
      { .name = "Advertising_Event_Properties",                   .size = 2,    },
      { .name = "Primary_Advertising_Interval_Min",               .size = 3,    },
      { .name = "Primary_Advertising_Interval_Max",               .size = 3,    },
      { .name = "Primary_Advertising_Channel_Map",                .size = 1,    },
      { .name = "Own_Address_Type",                               .size = 1,    },
      { .name = "Peer_Address_Type",                              .size = 1,    },
      { .name = "Peer_Address",                                   .size = 6,    },
      { .name = "Advertising_Filter_Policy",                      .size = 1,    },
      { .name = "Advertising_TX_Power",                           .size = 1,    },
      { .name = "Primary_Advertising_PHY",                        .size = 1,    },
      { .name = "Secondary_Advertising_Max_Skip",                 .size = 1,    },
      { .name = "Secondary_Advertising_PHY",                      .size = 1,    },
      { .name = "Advertising_SID",                                .size = 1,    },
      { .name = "Scan_Request_Notification_Enable",               .size = 1,    },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Selected_TX_Power",                              .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Set Extended Scan Enable command", .iscmd = 1, .section = "7.8.65", .version = "5.0", .BREDR = "E", .LE = "C19",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0042, .var = 0, .octet = 37, .bit = 6 },
    },
    .summary = "The HCI_LE_Set_Extended_Scan_Enable command will enable or disable "
               "scanning on the primary advertising physical channels.",

    .numCmd = 4, .Cmd = (SpecField_t[4]) {
      { .name = "Enable",                                         .size = 1,    },
      { .name = "Filter_Duplicates",                              .size = 1,    },
      { .name = "Duration",                                       .size = 2,    },
      { .name = "Period",                                         .size = 2,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Set Extended Scan Parameters command", .iscmd = 1, .section = "7.8.64", .version = "5.0", .BREDR = "E", .LE = "C19",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0041, .var = 0, .octet = 37, .bit = 5 },
    },
    .summary = "The HCI_LE_Set_Extended_Scan_Parameters command will set the "
               "parameters used for scanning on the primary advertising physical "
               "channel.",

    .numCmd = 6, .Cmd = (SpecField_t[6]) {
      { .name = "Own_Address_Type",                               .size = 1,    },
      { .name = "Scanning_Filter_Policy",                         .size = 1,    },
      { .name = "Scanning_PHYs",                                  .size = 1,    },
      { .name = "Scan_Type",                                      .size = 1,   .ref2size = "Scanning_PHYs", .mod = Size_Bits_Set, },
      { .name = "Scan_Interval",                                  .size = 2,   .ref2size = "Scanning_PHYs", .mod = Size_Bits_Set, },
      { .name = "Scan_Window",                                    .size = 2,   .ref2size = "Scanning_PHYs", .mod = Size_Bits_Set, },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Set Extended Scan Response Data command", .iscmd = 1, .section = "7.8.55", .version = "5.0", .BREDR = "E", .LE = "C17",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0038, .var = 0, .octet = 36, .bit = 4 },
    },
    .summary = "The HCI_LE_Set_Extended_Scan_Response_Data command will set the data "
               "transmitted in a scan response.",

    .numCmd = 5, .Cmd = (SpecField_t[5]) {
      { .name = "Advertising_Handle",                             .size = 1,    },
      { .name = "Operation",                                      .size = 1,    },
      { .name = "Fragment_Preference",                            .size = 1,    },
      { .name = "Scan_Response_Data_Length",                      .size = 1,    },
      { .name = "Scan_Response_Data",                             .size = 1,   .ref2size = "Scan_Response_Data_Length", },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Set Host Channel Classification command", .iscmd = 1, .section = "7.8.19", .version = "4.0", .BREDR = "E informa-", .LE = "C36",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0014, .var = 0, .octet = 27, .bit = 3 },
    },
    .summary = "The HCI_LE_Set_Host_Channel_Classification command allows the Host to "
               "specify a channel classification based on its “local tion”.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Channel_Map",                                    .size = 5,    .numb = 37 },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 3, .Events = (SpecEvent_t[3]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_LE_Connection_Update_Complete" },
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Set Host Feature command", .iscmd = 1, .section = "7.8.115", .version = "5.2", .BREDR = "E", .LE = "C49",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0074, .var = 0, .octet = 44, .bit = 1 },
    },
    .summary = "The HCI_LE_Set_Host_Feature command is used to set or clear a bit "
               "controlled by the Host in the Link Layer FeatureSet stored in the "
               "Controller.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Bit_Number",                                     .size = 1,    },
      { .name = "Bit_Value",                                      .size = 1,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Set Path Loss Reporting Enable command", .iscmd = 1, .section = "7.8.120", .version = "5.2", .BREDR = "E", .LE = "C52",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0079, .var = 0, .octet = 44, .bit = 6 },
    },
    .summary = "The HCI_LE_Set_Path_Loss_Reporting_Enable command is used to enable "
               "or disable path loss reporting events for an ACL connection.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Enable",                                         .size = 1,    },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numEvents = 2, .Events = (SpecEvent_t[2]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_LE_Path_Loss_Threshold" },
    },
  },

  { .name = "LE Set Path Loss Reporting Parameters command", .iscmd = 1, .section = "7.8.119", .version = "5.2", .BREDR = "E thresh-", .LE = "C52",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0078, .var = 0, .octet = 44, .bit = 5 },
    },
    .summary = "The HCI_LE_Set_Path_Loss_Reporting_Parameters command is used to set "
               "the path loss old and related parameters used to trigger reports for "
               "an ACL connection.",

    .numCmd = 6, .Cmd = (SpecField_t[6]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "High_Threshold",                                 .size = 1,    },
      { .name = "High_Hysteresis",                                .size = 1,    },
      { .name = "Low_Threshold",                                  .size = 1,    },
      { .name = "Low_Hysteresis",                                 .size = 1,    },
      { .name = "Min_Time_Spent",                                 .size = 2,    },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numEvents = 3, .Events = (SpecEvent_t[3]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_LE_Transmit_Power_Reporting" },
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Set Periodic Advertising Data command", .iscmd = 1, .section = "7.8.62", .version = "5.0", .BREDR = "E", .LE = "C18",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003f, .var = 0, .octet = 37, .bit = 3 },
    },
    .summary = "The HCI_LE_Set_Periodic_Advertising_Data command will set the "
               "periodic advertising data transmitted when advertising.",

    .numCmd = 4, .Cmd = (SpecField_t[4]) {
      { .name = "Advertising_Handle",                             .size = 1,    },
      { .name = "Operation",                                      .size = 1,    },
      { .name = "Advertising_Data_Length",                        .size = 1,    },
      { .name = "Advertising_Data",                               .size = 1,   .ref2size = "Advertising_Data_Length", },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Set Periodic Advertising Enable command", .iscmd = 1, .section = "7.8.63", .version = "5.0", .BREDR = "E", .LE = "C18",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0040, .var = 0, .octet = 37, .bit = 4 },
    },
    .summary = "The HCI_LE_Set_Periodic_Advertising_Enable command will enable or "
               "disable periodic advertising.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Enable",                                         .size = 1,    },
      { .name = "Advertising_Handle",                             .size = 1,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Set Periodic Advertising Parameters command", .iscmd = 1, .section = "7.8.61", .version = "5.0", .BREDR = "E", .LE = "C18",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003e, .var = 0, .octet = 37, .bit = 2 },
    },
    .summary = "The HCI_LE_Set_Periodic_Advertising_Parameters command will set the "
               "parameters used for periodic advertising.",

    .numCmd = 4, .Cmd = (SpecField_t[4]) {
      { .name = "Advertising_Handle",                             .size = 1,    },
      { .name = "Periodic_Advertising_Interval_Min",              .size = 2,    },
      { .name = "Periodic_Advertising_Interval_Max",              .size = 2,    },
      { .name = "Periodic_Advertising_Properties",                .size = 2,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Set Periodic Advertising Receive Enable command", .iscmd = 1, .section = "7.8.88", .version = "5.1", .BREDR = "E", .LE = "C32",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0059, .var = 0, .octet = 40, .bit = 5 },
    },
    .summary = "The HCI_LE_Set_Periodic_Advertising_Receive_Enable command will "
               "enable or disable periodic advertising reports once synchronized.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Sync_Handle",                                    .size = 2,    .numb = 12 },
      { .name = "Enable",                                         .size = 1,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Set Periodic Advertising Sync Transfer Parameters command", .iscmd = 1, .section = "7.8.91", .version = "5.1", .BREDR = "E", .LE = "C35",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x005c, .var = 0, .octet = 41, .bit = 0 },
    },
    .summary = "The HCI_LE_Set_Periodic_Advertising_Sync_Transfer_Parameters command "
               "is used to allow the Host to specify the behavior of the Controller "
               "when periodic advertising synchronization information is received "
               "from a connected Controller.",

    .numCmd = 5, .Cmd = (SpecField_t[5]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Mode",                                           .size = 1,    },
      { .name = "Skip",                                           .size = 2,    },
      { .name = "Sync_Timeout",                                   .size = 2,    },
      { .name = "CTE_Type",                                       .size = 1,    },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Set PHY command", .iscmd = 1, .section = "7.8.49", .version = "5.0", .BREDR = "E", .LE = "C11",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0032, .var = 0, .octet = 35, .bit = 6 },
    },
    .summary = "The HCI_LE_Set_PHY command is used to request a change of the PHY for "
               "a Connection_Handle.",

    .numCmd = 5, .Cmd = (SpecField_t[5]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "All_PHYs",                                       .size = 1,    },
      { .name = "TX_PHYs",                                        .size = 1,    },
      { .name = "RX_PHYs",                                        .size = 1,    },
      { .name = "PHY_Options",                                    .size = 2,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Set Privacy Mode command", .iscmd = 1, .section = "7.8.77", .version = "5.0", .BREDR = "E", .LE = "C9",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x004e, .var = 0, .octet = 39, .bit = 2 },
    },
    .summary = "The HCI_LE_Set_Privacy_Mode command is used to allow the Host to "
               "specify the privacy mode for an entry on the resolving list.",

    .numCmd = 3, .Cmd = (SpecField_t[3]) {
      { .name = "Peer_Identity_Address_Type",                     .size = 1,    },
      { .name = "Peer_Identity_Address",                          .size = 6,    },
      { .name = "Privacy_Mode",                                   .size = 1,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 2, .Events = (SpecEvent_t[2]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_LE_Transmit_Power_Reporting" },
    },
  },

  { .name = "LE Set Random Address command", .iscmd = 1, .section = "7.8.4", .version = "4.0", .BREDR = "E", .LE = "C1",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0005, .var = 0, .octet = 25, .bit = 4 },
    },
    .summary = "The HCI_LE_Set_Random_Address command will set the Random Device "
               "Address that may be used in a packet sent on the advertising physical "
               "channel.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Random_Address",                                 .size = 6,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Set Resolvable Private Address Timeout command", .iscmd = 1, .section = "7.8.45", .version = "4.2", .BREDR = "E a", .LE = "C9",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x002e, .var = 0, .octet = 35, .bit = 2 },
    },
    .summary = "The HCI_LE_Set_Resolvable_Private_Address_Timeout sets the length of "
               "time the Controller uses random private address before a new random "
               "private address is generated and starts being used.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "RPA_Timeout",                                    .size = 2,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Set Scan Enable command", .iscmd = 1, .section = "7.8.11", .version = "4.0", .BREDR = "E", .LE = "C98",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x000c, .var = 0, .octet = 26, .bit = 3 },
    },
    .summary = "The HCI_LE_Set_Scan_Enable command will enable or disable scanning.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "LE_Scan_Enable",                                 .size = 1,    },
      { .name = "Filter_Duplicates",                              .size = 1,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Set Scan Parameters command", .iscmd = 1, .section = "7.8.10", .version = "4.0", .BREDR = "E", .LE = "C98",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x000b, .var = 0, .octet = 26, .bit = 2 },
    },
    .summary = "The HCI_LE_Set_Scan_Parameters command will set the parameters used "
               "for scanning.",

    .numCmd = 5, .Cmd = (SpecField_t[5]) {
      { .name = "LE_Scan_Type",                                   .size = 1,    },
      { .name = "LE_Scan_Interval",                               .size = 2,    },
      { .name = "LE_Scan_Window",                                 .size = 2,    },
      { .name = "Own_Address_Type",                               .size = 1,    },
      { .name = "Scanning_Filter_Policy",                         .size = 1,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 3, .Events = (SpecEvent_t[3]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_LE_Enhanced_Connection_Complete" },
      { .name = "HCI_LE_Connection_Complete" },
    },
  },

  { .name = "LE Set Scan Response Data command", .iscmd = 1, .section = "7.8.8", .version = "4.0", .BREDR = "E", .LE = "C15",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0009, .var = 0, .octet = 26, .bit = 0 },
    },
    .summary = "The HCI_LE_Set_Scan_Response_Data command will set the data "
               "transmitted in a scan response.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Scan_Response_Data_Length",                      .size = 1,    },
      { .name = "Scan_Response_Data",                             .size = 31,   },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Set Transmit Power Reporting Enable command", .iscmd = 1, .section = "7.8.121", .version = "5.2", .BREDR = "E trans-", .LE = "C51",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x007a, .var = 0, .octet = 44, .bit = 7 },
    },
    .summary = "The HCI_LE_Set_Transmit_Power_Reporting_Enable command is used to "
               "enable or disable reporting to the local Host of mit power level "
               "changes on an ACL connection.",

    .numCmd = 3, .Cmd = (SpecField_t[3]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Local_Enable",                                   .size = 1,    },
      { .name = "Remote_Enable",                                  .size = 1,    },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numEvents = 2, .Events = (SpecEvent_t[2]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_LE_Path_Loss_Threshold" },
    },
  },

  { .name = "LE Setup ISO Data Path command", .iscmd = 1, .section = "7.8.109", .version = "5.2", .BREDR = "E", .LE = "C47",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x006e, .var = 0, .octet = 43, .bit = 3 },
    },
    .summary = "The HCI_LE_Setup_ISO_Data_Path command identifies and creates the "
               "isochronous data path between the Host and the Controller and "
               "optionally configures the codec in the Controller.",

    .numCmd = 7, .Cmd = (SpecField_t[7]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Data_Path_Direction",                            .size = 1,    },
      { .name = "Data_Path_ID",                                   .size = 1,    },
      { .name = "Codec_ID",                                       .size = 5,    },
      { .name = "Controller_Delay",                               .size = 3,    },
      { .name = "Codec_Configuration_Length",                     .size = 1,    },
      { .name = "Codec_Configuration",                            .size = 1,   .ref2size = "Codec_Configuration_Length", },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numEvents = 2, .Events = (SpecEvent_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_LE_Request_Peer_SCA_Complete" },
    },
  },

  { .name = "LE Subrate Change event", .iscmd = 0, .subevent = 0x23, .section = "7.7.65.35", .version = "5.3", .BREDR = "E", .LE = "C57",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003e, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_LE_Subrate_Change event indicates that a new subrate factor "
               "has been applied to an existing ACL connection.",

    .numCmd = 7, .Cmd = (SpecField_t[7]) {
      { .name = "Subevent_Code",                                  .size = 1,    },
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Subrate_Factor",                                 .size = 2,    },
      { .name = "Peripheral_Latency",                             .size = 2,    },
      { .name = "Continuation_Number",                            .size = 2,    },
      { .name = "Supervision_Timeout",                            .size = 2,    },
    },
  },

  { .name = "LE Subrate Request command", .iscmd = 1, .section = "7.8.124", .version = "5.3", .BREDR = "E", .LE = "C57",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x007e, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_LE_Subrate_Request command modifies an existing ACL "
               "connection by applying a subrate factor.",

    .numCmd = 6, .Cmd = (SpecField_t[6]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Subrate_Min",                                    .size = 2,    },
      { .name = "Subrate_Max",                                    .size = 2,    },
      { .name = "Max_Latency",                                    .size = 2,    },
      { .name = "Continuation_Number",                            .size = 2,    },
      { .name = "Supervision_Timeout",                            .size = 2,    },
    },
  },

  { .name = "LE Terminate BIG command", .iscmd = 1, .section = "7.8.105", .version = "5.2", .BREDR = "E the", .LE = "C41",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x006a, .var = 0, .octet = 42, .bit = 7 },
    },
    .summary = "The HCI_LE_Terminate_BIG command terminates the transmission of all "
               "BISes of a BIG or cancels process of creating a BIG.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "BIG_Handle",                                     .size = 1,    },
      { .name = "Reason",                                         .size = 1,    },
    },
    .numEvents = 2, .Events = (SpecEvent_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_LE_Create_BIG_Complete" },
    },
  },

  { .name = "LE Terminate BIG Complete event", .iscmd = 0, .subevent = 0x1c, .section = "7.7.65.28", .version = "5.2", .BREDR = "E", .LE = "C41",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003e, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_LE_Terminate_BIG_Complete event indicates that the "
               "transmission of all the BISes in the BIG have been terminated.",

    .numCmd = 3, .Cmd = (SpecField_t[3]) {
      { .name = "Subevent_Code",                                  .size = 1,    },
      { .name = "BIG_Handle",                                     .size = 1,    },
      { .name = "Reason",                                         .size = 1,    },
    },
  },

  { .name = "LE Test End command", .iscmd = 1, .section = "7.8.30", .version = "4.0", .BREDR = "E or", .LE = "M",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x001f, .var = 0, .octet = 28, .bit = 6 },
    },
    .summary = "The HCI_LE_Test_End command will end the current the receiver "
               "transmitter test.",

    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Num_Packets",                                    .size = 2,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Transmit Power Reporting event", .iscmd = 0, .subevent = 0x21, .section = "7.7.65.33", .version = "5.2", .BREDR = "E", .LE = "C51",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003e, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_LE_Transmit_Power_Reporting event is used to report the "
               "transmit power level on the ACL connection.",

    .numCmd = 8, .Cmd = (SpecField_t[8]) {
      { .name = "Subevent_Code",                                  .size = 1,    },
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Reason",                                         .size = 1,    },
      { .name = "PHY",                                            .size = 1,    },
      { .name = "TX_Power_Level",                                 .size = 1,    },
      { .name = "TX_Power_Level_Flag",                            .size = 1,    },
      { .name = "Delta",                                          .size = 1,    },
    },
  },

  { .name = "LE Transmitter Test command", .iscmd = 1, .section = "7.8.29", .version = "4.0 5.0 5.1 5.2", .BREDR = "E", .LE = "[v1] C1 [v2] C12 [v3] C29 [v4] C53",
    .numVI = 4, .VI = (SpecCmdVar_t[4]) {
      { .ocf = 0x007b, .var = 4, .octet = 45, .bit = 0 },
      { .ocf = 0x0050, .var = 3, .octet = 39, .bit = 4 },
      { .ocf = 0x0034, .var = 2, .octet = 36, .bit = 0 },
      { .ocf = 0x001e, .var = 1, .octet = 28, .bit = 5 },
    },
    .summary = "The HCI_LE_Transmitter_Test command will run the LE transmitter "
               "test.",

    .numCmd = 9, .Cmd = (SpecField_t[9]) {
      { .name = "TX_Channel",                                     .size = 1,   .VS = { 0, 1, 1, 1, 1 },  },
      { .name = "Test_Data_Length",                               .size = 1,   .VS = { 0, 1, 1, 1, 1 },  },
      { .name = "Packet_Payload",                                 .size = 1,   .VS = { 0, 1, 1, 1, 1 },  },
      { .name = "PHY",                                            .size = 1,   .VS = { 0, 0, 1, 1, 1 },  },
      { .name = "CTE_Length",                                     .size = 1,   .VS = { 0, 0, 0, 1, 1 },  },
      { .name = "CTE_Type",                                       .size = 1,   .VS = { 0, 0, 0, 1, 1 },  },
      { .name = "Switching_Pattern_Length",                       .size = 1,   .VS = { 0, 0, 0, 1, 1 },  },
      { .name = "Antenna_IDs",                                    .size = 1,   .VS = { 0, 0, 0, 1, 1 }, .ref2size = "Switching_Pattern_Length", },
      { .name = "TX_Power_Level",                                 .size = 1,   .VS = { 0, 0, 0, 0, 1 },  },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,   .VS = { 0, 1, 1, 1, 1 },  },
    },
    .numEvents = 2, .Events = (SpecEvent_t[2]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_LE_Connectionless_IQ_Report" },
    },
  },

  { .name = "LE Write RF Path Compensation command", .iscmd = 1, .section = "7.8.76", .version = "5.0", .BREDR = "E loss", .LE = "C22",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x004d, .var = 0, .octet = 39, .bit = 1 },
    },
    .summary = "The HCI_LE_Write_RF_Path_Compensation command is used to indicate the "
               "RF path gain or from the RF transceiver output to the antenna output "
               "contributed by intermediate components.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "RF_TX_Path_Compensation_Value",                  .size = 2,    },
      { .name = "RF_RX_Path_Compensation_Value",                  .size = 2,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "LE Write Suggested Default Data Length command", .iscmd = 1, .section = "7.8.35", .version = "4.2", .BREDR = "E", .LE = "C8",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0024, .var = 0, .octet = 34, .bit = 0 },
    },
    .summary = "The HCI_LE_Write_Suggested_Default_Data_Length command allows the "
               "Host to suggest initial MaxTxOctets and MaxTxTime values for new "
               "connections.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Suggested_Max_TX_Octets",                        .size = 2,    },
      { .name = "Suggested_Max_TX_Time",                          .size = 2,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Link Key Notification event", .iscmd = 0, .section = "7.7.24", .version = "1.1", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0018, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Link_Key_Notification event is used to indicate to the Host "
               "that a new Link Key has been created for the connection with the "
               "BR/EDR Controller specified in BD_ADDR.",

    .numCmd = 3, .Cmd = (SpecField_t[3]) {
      { .name = "BD_ADDR",                                        .size = 6,    },
      { .name = "Link_Key",                                       .size = 16,   },
      { .name = "Key_Type",                                       .size = 1,    },
    },
  },

  { .name = "Link Key Request event", .iscmd = 0, .section = "7.7.23", .version = "1.1", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0017, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Link_Key_Request event is used to indicate that a Link Key is "
               "required for the connection with the device specified in BD_ADDR.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "BD_ADDR",                                        .size = 6,    },
    },
  },

  { .name = "Link Key Request Negative Reply command", .iscmd = 1, .section = "7.1.11", .version = "1.1", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x000c, .var = 0, .octet =  1, .bit = 3 },
    },
    .summary = "The HCI_Link_Key_Request_Negative_Reply command is used to reply to "
               "an HCI_Link_Key_Request event from the BR/EDR Controller if the Host "
               "does not have a stored Link Key for the connection with the other "
               "BR/EDR Controller specified by BD_ADDR.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "BD_ADDR",                                        .size = 6,    },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "BD_ADDR",                                        .size = 6,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Link Key Request Reply command", .iscmd = 1, .section = "7.1.10", .version = "1.1", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x000b, .var = 0, .octet =  1, .bit = 2 },
    },
    .summary = "The HCI_Link_Key_Request_Reply command is used to reply to an "
               "HCI_Link_Key_Request event from the BR/EDR Controller, and specifies "
               "the Link Key stored on the Host to be used as the link key for the "
               "connection with the other BR/EDR Controller specified by BD_ADDR.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "BD_ADDR",                                        .size = 6,    },
      { .name = "Link_Key",                                       .size = 16,   },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "BD_ADDR",                                        .size = 6,    },
    },
    .numEvents = 3, .Events = (SpecEvent_t[3]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Connection_Complete" },
      { .name = "HCI_Synchronous_Connection_Complete" },
    },
  },

  { .name = "Link Key Selection command", .iscmd = 1, .section = "7.1.18", .version = "1.1", .BREDR = "C215 regu-", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0017, .var = 0, .octet =  2, .bit = 2 },
    },
    .summary = "The HCI_Link_Key_Selection command is used to force both BR/EDR "
               "Controllers of a connection associated to the Connection_Handle to "
               "use the temporary link key of the Central or the lar link keys.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Key_Flag",                                       .size = 1,    },
    },
    .numEvents = 3, .Events = (SpecEvent_t[3]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Link_Key_Notification" },
      { .name = "HCI_Change_Connection_Link_Key_Complete" },
    },
  },

  { .name = "Link Key Type Changed event", .iscmd = 0, .section = "7.7.10", .version = "1.1", .BREDR = "C215", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x000a, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Link_Key_Type_Changed event is used to indicate that the "
               "change in the temporary Link Key or in the semi-permanent link keys "
               "on the Bluetooth Central side has been completed.",

    .numCmd = 3, .Cmd = (SpecField_t[3]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Key_Flag",                                       .size = 1,    },
    },
  },

  { .name = "Link Supervision Timeout Changed event", .iscmd = 0, .section = "7.7.46", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0038, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Link_Supervision_Timeout_Changed event indicates that the "
               "remote device changed the Link Supervision Timeout.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Link_Supervision_Timeout",                       .size = 2,    },
    },
  },

  { .name = "Loopback Command event", .iscmd = 0, .section = "7.7.25", .version = "1.1", .BREDR = "C123", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0019, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Loopback_Command event is used to loop back all commands that "
               "the Host sends to the BR/EDR Controller with some exceptions.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "HCI_Command_Packet",                             .size = 1,    .mod = Size_Depends, },
    },
  },

  { .name = "Max Slots Change event", .iscmd = 0, .section = "7.7.27", .version = "1.1", .BREDR = "C132", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x001b, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Max_Slots_Change event is used to indicate a change in the "
               "max slots by the LM.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "LMP_Max_Slots",                                  .size = 1,    },
    },
  },

  { .name = "Mode Change event", .iscmd = 0, .section = "7.7.20", .version = "1.1", .BREDR = "C144", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0014, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Mode_Change event is used to indicate that the current mode "
               "has changed.",

    .numCmd = 4, .Cmd = (SpecField_t[4]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Current_Mode",                                   .size = 1,    },
      { .name = "Interval",                                       .size = 2,    },
    },
  },

  { .name = "Number Of Completed Data Blocks event", .iscmd = 0, .section = "7.7.59", .version = "3.0 + HS", .BREDR = "C124", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0048, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Number_Of_Completed_Data_Blocks event is used by the "
               "Controller to indicate to the Host how many HCI ACL Data packets have "
               "been completed and how many data block buffers have been freed for "
               "each Handle since the previous HCI_Number_Of_Completed_Data_Blocks "
               "event was sent.",

    .numCmd = 5, .Cmd = (SpecField_t[5]) {
      { .name = "Total_Num_Data_Blocks",                          .size = 2,    },
      { .name = "Num_Handles",                                    .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,   .ref2size = "Num_Handles", .numb = 12 },
      { .name = "Num_Completed_Packets",                          .size = 2,   .ref2size = "Num_Handles", },
      { .name = "Num_Completed_Blocks",                           .size = 2,   .ref2size = "Num_Handles", },
    },
  },

  { .name = "Number Of Completed Packets event", .iscmd = 0, .section = "7.7.19", .version = "1.1", .BREDR = "M the", .LE = "C3",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0013, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Number_Of_Completed_Packets event is used by the Controller "
               "to indicate to Host how many HCI Data packets have been completed for "
               "each Connection_Handle since the previous "
               "HCI_Number_Of_Completed_Packets event was sent.",

    .numCmd = 3, .Cmd = (SpecField_t[3]) {
      { .name = "Num_Handles",                                    .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,   .ref2size = "Num_Handles", .numb = 12 },
      { .name = "Num_Completed_Packets",                          .size = 2,   .ref2size = "Num_Handles", },
    },
  },

  { .name = "Page Scan Repetition Mode Change event", .iscmd = 0, .section = "7.7.31", .version = "1.1", .BREDR = "O", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x000c, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Page_Scan_Repetition_Mode_Change event indicates that the "
               "connected remote BR/EDR Controller with the specified "
               "Connection_Handle has successfully changed the Page Scan Repetition "
               "Mode (SR).",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "BD_ADDR",                                        .size = 6,    },
      { .name = "Page_Scan_Repetition_Mode",                      .size = 1,    },
    },
  },

  { .name = "Periodic Inquiry Mode command", .iscmd = 1, .section = "7.1.3", .version = "1.1", .BREDR = "C128", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0003, .var = 0, .octet =  0, .bit = 2 },
    },
    .summary = "The HCI_Periodic_Inquiry_Mode command is used to configure the BR/EDR "
               "Controller to perform an automatic Inquiry based on a specified "
               "period range.",

    .numCmd = 5, .Cmd = (SpecField_t[5]) {
      { .name = "Max_Period_Length",                              .size = 2,    },
      { .name = "Min_Period_Length",                              .size = 2,    },
      { .name = "LAP",                                            .size = 3,    },
      { .name = "Inquiry_Length",                                 .size = 1,    },
      { .name = "Num_Responses",                                  .size = 1,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 2, .Events = (SpecEvent_t[2]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_Inquiry_Complete" },
    },
  },

  { .name = "Peripheral Page Response Timeout event", .iscmd = 0, .section = "7.7.72", .version = "CSA4", .BREDR = "O", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0054, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Peripheral_Page_Response_Timeout event indicates to the Host "
               "that the pagerespTO has been exceeded on the BR/EDR Controller after "
               "the Controller responded to an ID packet.",

  },

  { .name = "PIN Code Request event", .iscmd = 0, .section = "7.7.22", .version = "1.1", .BREDR = "M PIN", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0016, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_PIN_Code_Request event is used to indicate that a code is "
               "required to create a new link key for a connection.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "BD_ADDR",                                        .size = 6,    },
    },
  },

  { .name = "PIN Code Request Negative Reply command", .iscmd = 1, .section = "7.1.13", .version = "1.1", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x000e, .var = 0, .octet =  1, .bit = 5 },
    },
    .summary = "The HCI_PIN_Code_Request_Negative_Reply command is used to reply to "
               "an HCI_PIN_Code_Request event from the Controller when the Host "
               "cannot specify a PIN code to use for a connection.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "BD_ADDR",                                        .size = 6,    },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "BD_ADDR",                                        .size = 6,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "PIN Code Request Reply command", .iscmd = 1, .section = "7.1.12", .version = "1.1", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x000d, .var = 0, .octet =  1, .bit = 4 },
    },
    .summary = "The HCI_PIN_Code_Request_Reply command is used to reply to an "
               "HCI_PIN_Code_Request event from the Controller and specifies the PIN "
               "code to use for a connection.",

    .numCmd = 3, .Cmd = (SpecField_t[3]) {
      { .name = "BD_ADDR",                                        .size = 6,    },
      { .name = "PIN_Code_Length",                                .size = 1,    },
      { .name = "PIN_Code",                                       .size = 16,   },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "BD_ADDR",                                        .size = 6,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "QoS Setup command", .iscmd = 1, .section = "7.2.6", .version = "1.1", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0007, .var = 0, .octet =  4, .bit = 6 },
    },
    .summary = "The HCI_QoS_Setup command is used to specify Quality of Service "
               "parameters for a Connection_Handle.",

    .numCmd = 7, .Cmd = (SpecField_t[7]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Unused",                                         .size = 1,    },
      { .name = "Service_Type",                                   .size = 1,    },
      { .name = "Token_Rate",                                     .size = 4,    },
      { .name = "Peak_Bandwidth",                                 .size = 4,    },
      { .name = "Latency",                                        .size = 4,    },
      { .name = "Delay_Variation",                                .size = 4,    },
    },
  },

  { .name = "QoS Setup Complete event", .iscmd = 0, .section = "7.7.13", .version = "1.1", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x000d, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_QoS_Setup_Complete event is used to indicate that QoS is set "
               "up.",

    .numCmd = 8, .Cmd = (SpecField_t[8]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Unused",                                         .size = 1,    },
      { .name = "Service_Type",                                   .size = 1,    },
      { .name = "Token_Rate",                                     .size = 4,    },
      { .name = "Peak_Bandwidth",                                 .size = 4,    },
      { .name = "Latency",                                        .size = 4,    },
      { .name = "Delay_Variation",                                .size = 4,    },
    },
  },

  { .name = "QoS Violation event", .iscmd = 0, .section = "7.7.30", .version = "1.1", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x001e, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_QoS_Violation event is used to indicate the Controller’s "
               "Link Manager is unable to provide the current QoS requirement for the "
               "Handle.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Handle",                                         .size = 2,    .numb = 12 },
    },
  },

  { .name = "Read AFH Channel Assessment Mode command", .iscmd = 1, .section = "7.3.53", .version = "1.2", .BREDR = "C140 dis-", .LE = "C58",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0048, .var = 0, .octet = 13, .bit = 2 },
    },
    .summary = "The HCI_Read_AFH_Channel_Assessment_Mode command will read the value "
               "for the AFH Channel Classification Mode parameter. This value is used "
               "to enable or able the Controller’s channel assessment scheme.",

    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "AFH_Channel_Assessment_Mode",                    .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read AFH Channel Map command", .iscmd = 1, .section = "7.5.5", .version = "1.2", .BREDR = "C139", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0006, .var = 0, .octet = 15, .bit = 6 },
    },
    .summary = "The HCI_Read_AFH_Channel_Map command will read the current state of "
               "the channel map for a connection.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numRet = 4, .Ret = (SpecField_t[4]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "AFH_Mode",                                       .size = 1,    },
      { .name = "AFH_Channel_Map",                                .size = 10,   .numb = 79 },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read Authenticated Payload Timeout command", .iscmd = 1, .section = "7.3.93", .version = "4.1", .BREDR = "C155", .LE = "C155",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x007b, .var = 0, .octet = 32, .bit = 4 },
    },
    .summary = "The HCI_Read_Authenticated_Payload_Timeout command is used to read "
               "the Authenticated Payload Timeout parameter, which is used to set the "
               "maximum time between packets being received from the remote device "
               "without a valid MIC.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numRet = 3, .Ret = (SpecField_t[3]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Authenticated_Payload_Timeout",                  .size = 2,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read Authentication Enable command", .iscmd = 1, .section = "7.3.23", .version = "1.1", .BREDR = "C111", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x001f, .var = 0, .octet =  8, .bit = 4 },
    },
    .summary = "The HCI_Read_Authentication_Enable command will read the value for "
               "the Authentication Enable parameter, which controls whether the "
               "Bluetooth device will require authentication for each connection with "
               "other Bluetooth devices.",

    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Authentication_Enable",                          .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read Automatic Flush Timeout command", .iscmd = 1, .section = "7.3.29", .version = "1.1", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0027, .var = 0, .octet =  9, .bit = 4 },
    },
    .summary = "The HCI_Read_Automatic_Flush_Timeout command will read the value for "
               "the Flush Timeout configuration parameter for the specified "
               "Connection_Handle. The Flush Timeout parameter is only used for ACL "
               "connections.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numRet = 3, .Ret = (SpecField_t[3]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Flush_Timeout",                                  .size = 2,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read BD_ADDR command", .iscmd = 1, .section = "7.4.6", .version = "1.1", .BREDR = "M", .LE = "M",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0009, .var = 0, .octet = 15, .bit = 1 },
    },
    .summary = "The HCI_Read_BD_ADDR command will read the value for the BD_ADDR "
               "parameter.",

    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "BD_ADDR",                                        .size = 6,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read Buffer Size command", .iscmd = 1, .section = "7.4.5", .version = "1.1", .BREDR = "M is", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0005, .var = 0, .octet = 14, .bit = 7 },
    },
    .summary = "The HCI_Read_Buffer_Size command returns the size of the HCI buffers. "
               "These buffers are used by the Controller to buffer data that to be "
               "transmitted.",

    .numRet = 5, .Ret = (SpecField_t[5]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "ACL_Data_Packet_Length",                         .size = 2,    },
      { .name = "Synchronous_Data_Packet_Length",                 .size = 1,    },
      { .name = "Total_Num_ACL_Data_Packets",                     .size = 2,    },
      { .name = "Total_Num_Synchronous_Data_Packets",             .size = 2,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read Class of Device command", .iscmd = 1, .section = "7.3.25", .version = "1.1", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0023, .var = 0, .octet =  9, .bit = 0 },
    },
    .summary = "The HCI_Read_Class_of_Device command will read the value for the "
               "Class of Device configuration parameter, which is used to indicate "
               "its capabilities to other devices.",

    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Class_Of_Device",                                .size = 3,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read Clock command", .iscmd = 1, .section = "7.5.6", .version = "1.2", .BREDR = "O", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0007, .var = 0, .octet = 15, .bit = 7 },
    },
    .summary = "The HCI_Read_Clock command will read an estimate of a piconet or the "
               "local Bluetooth Clock.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Which_Clock",                                    .size = 1,    },
    },
    .numRet = 4, .Ret = (SpecField_t[4]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Clock",                                          .size = 4,    .numb = 28 },
      { .name = "Accuracy",                                       .size = 2,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read Clock Offset command", .iscmd = 1, .section = "7.1.24", .version = "1.1", .BREDR = "O", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x001f, .var = 0, .octet =  3, .bit = 0 },
    },
    .summary = "The HCI_Read_Clock_Offset command allows the Host to read the clock "
               "offset of remote BR/EDR Controllers.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numEvents = 2, .Events = (SpecEvent_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Read_Remote_Version_Information_Complete" },
    },
  },

  { .name = "Read Clock Offset Complete event", .iscmd = 0, .section = "7.7.28", .version = "1.1", .BREDR = "C104", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x001c, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Read_Clock_Offset_Complete event is used to indicate the "
               "completion of the process of the LM obtaining the Clock offset "
               "information.",

    .numCmd = 3, .Cmd = (SpecField_t[3]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Clock_Offset",                                   .size = 2,    },
    },
  },

  { .name = "Read Connection Accept Timeout command", .iscmd = 1, .section = "7.3.13", .version = "1.1", .BREDR = "M con-", .LE = "C40",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0015, .var = 0, .octet =  7, .bit = 2 },
    },
    .summary = "The HCI_Read_Connection_Accept_Timeout command will read the value "
               "for the Connection Accept Timeout configuration parameter, which "
               "allows the Controller to automatically deny a nection request after a "
               "specified period has occurred, and to refuse a new connection.",

    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Accept_Timeout",                      .size = 2,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read Current IAC LAP command", .iscmd = 1, .section = "7.3.44", .version = "1.1", .BREDR = "C125", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0039, .var = 0, .octet = 11, .bit = 3 },
    },
    .summary = "The HCI_Read_Current_IAC LAP command will read the LAP(s) used to "
               "create the Inquiry Access Codes (IAC) that the local BR/ EDR "
               "Controller is simultaneously scanning for during Inquiry Scans.",

    .numRet = 3, .Ret = (SpecField_t[3]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Num_Current_IAC",                                .size = 1,    },
      { .name = "IAC_LAP",                                        .size = 3,   .ref2size = "Num_Current_IAC", },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read Data Block Size command", .iscmd = 1, .section = "7.4.7", .version = "3.0 + HS", .BREDR = "C124", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x000a, .var = 0, .octet = 23, .bit = 2 },
    },
    .summary = "The HCI_Read_Data_Block_Size command returns the maximum size of the "
               "HCI buffers. These buffers are used by the Controller to buffer data "
               "that is to be transmitted.",

    .numRet = 4, .Ret = (SpecField_t[4]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Max_ACL_Data_Packet_Length",                     .size = 2,    },
      { .name = "Data_Block_Length",                              .size = 2,    },
      { .name = "Total_Num_Data_Blocks",                          .size = 2,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read Default Erroneous Data Reporting command", .iscmd = 1, .section = "7.3.64", .version = "2.1 + EDR", .BREDR = "C112", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x005a, .var = 0, .octet = 18, .bit = 2 },
    },
    .summary = "The HCI_Read_Default_Erroneous_Data_Reporting command will read the "
               "value for the Erroneous Data Reporting configuration parameter, which "
               "controls whether the BR/EDR Controller will provide data for every "
               "(e)SCO interval, with the Packet_Status_Flag in HCI Synchronous Data "
               "packets set according to HCI Synchronous Data packets.",

    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Erroneous_Data_Reporting",                       .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read Default Link Policy Settings command", .iscmd = 1, .section = "7.2.11", .version = "1.2", .BREDR = "C141 config-", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x000e, .var = 0, .octet =  5, .bit = 3 },
    },
    .summary = "The HCI_Read_Default_Link_Policy_Settings command will read the "
               "Default Link Policy uration parameter for all new connections.",

    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Default_Link_Policy_Settings",                   .size = 2,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read Encryption Key Size command", .iscmd = 1, .section = "7.5.7", .version = "3.0 + HS", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0008, .var = 0, .octet = 20, .bit = 4 },
    },
    .summary = "The HCI_Read_Encryption_Key_Size command is used to read the "
               "encryption key size on a given Connection_Handle.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numRet = 3, .Ret = (SpecField_t[3]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Key_Size",                                       .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read Enhanced Transmit Power Level command", .iscmd = 1, .section = "7.3.74", .version = "3.0 + HS", .BREDR = "C217", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0068, .var = 0, .octet = 24, .bit = 0 },
    },
    .summary = "The HCI_Read_Enhanced_Transmit_Power_Level command will read the "
               "values for the GFSK, π/4-DQPSK and 8DPSK Transmit Power Level "
               "parameters for the specified Connection_Handle.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Type",                                           .size = 1,    },
    },
    .numRet = 4, .Ret = (SpecField_t[4]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "TX_Power_Level_GFSK",                            .size = 1,    },
      { .name = "TX_Power_Level_DQPSK",                           .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read Extended Inquiry Length command", .iscmd = 1, .section = "7.3.98", .version = "4.1", .BREDR = "C113", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0080, .var = 0, .octet = 33, .bit = 2 },
    },
    .summary = "The HCI_Read_Extended_Inquiry_Length command is used to read the "
               "Extended Inquiry Length parameter from the Controller.",

    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Extended_Inquiry_Length",                        .size = 2,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read Extended Inquiry Response command", .iscmd = 1, .section = "7.3.55", .version = "2.1 + EDR", .BREDR = "C205", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0051, .var = 0, .octet = 17, .bit = 0 },
    },
    .summary = "The HCI_Read_Extended_Inquiry_Response command will read the data "
               "that the BR/EDR Controller sends in the extended inquiry response "
               "packet during inquiry response.",

    .numRet = 3, .Ret = (SpecField_t[3]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "FEC_Required",                                   .size = 1,    },
      { .name = "Extended_Inquiry_Response",                      .size = 240,  },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read Extended Page Timeout command", .iscmd = 1, .section = "7.3.96", .version = "4.1", .BREDR = "C114", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x007e, .var = 0, .octet = 33, .bit = 0 },
    },
    .summary = "The HCI_Read_Extended_Page_Timeout command is used to read the "
               "Extended Page Timeout parameter from the Controller.",

    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Extended_Page_Timeout",                          .size = 2,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read Failed Contact Counter command", .iscmd = 1, .section = "7.5.1", .version = "1.1", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0001, .var = 0, .octet = 15, .bit = 2 },
    },
    .summary = "The HCI_Read_Failed_Contact_Counter command will read the value for "
               "the Failed Contact Counter configuration parameter for a particular "
               "connection to another device.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Handle",                                         .size = 2,    .numb = 12 },
    },
    .numRet = 3, .Ret = (SpecField_t[3]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Handle",                                         .size = 2,    .numb = 12 },
      { .name = "Failed_Contact_Counter",                         .size = 2,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read Flow Control Mode command", .iscmd = 1, .section = "7.3.72", .version = "3.0 + HS", .BREDR = "C124", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0066, .var = 0, .octet = 23, .bit = 0 },
    },
    .summary = "The HCI_Read_Flow_Control_Mode command returns the value of the "
               "Flow_Control_Mode configuration parameter supported by this "
               "Controller.",

    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Flow_Control_Mode",                              .size = 1,    },
    },
  },

  { .name = "Read Hold Mode Activity command", .iscmd = 1, .section = "7.3.33", .version = "1.1", .BREDR = "C213", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x002b, .var = 0, .octet = 10, .bit = 0 },
    },
    .summary = "The HCI_Read_Hold_Mode_ Activity command is used to read which "
               "activities should be suspended when the BR/EDR Controller is in Hold "
               "mode.",

    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Hold_Mode_Activity",                             .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read Inquiry Mode command", .iscmd = 1, .section = "7.3.49", .version = "1.2", .BREDR = "C115", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0044, .var = 0, .octet = 12, .bit = 6 },
    },
    .summary = "The HCI_Read_Inquiry_Mode command is used to read the Inquiry Mode "
               "configuration parameter of the local BR/EDR Controller.",

    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Inquiry_Mode",                                   .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read Inquiry Response Transmit Power Level command", .iscmd = 1, .section = "7.3.61", .version = "2.1 + EDR", .BREDR = "C125", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0058, .var = 0, .octet = 18, .bit = 0 },
    },
    .summary = "The HCI_Read_Inquiry_Response_Transmit_Power_Level command will read "
               "the inquiry response Transmit Power level used to transmit the FHS "
               "and EIR data packets. This can be used directly in the Tx Power Level "
               "EIR data type.",

    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "TX_Power",                                       .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read Inquiry Scan Activity command", .iscmd = 1, .section = "7.3.21", .version = "1.1", .BREDR = "C125", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x001d, .var = 0, .octet =  8, .bit = 2 },
    },
    .summary = "The HCI_Read_Inquiry_Scan_Activity command will read the value for "
               "Inquiry Scan Interval and Inquiry Scan Window configuration "
               "parameters. Inquiry Scan Interval defines the amount of time between "
               "consecutive inquiry scans. Inquiry Scan Window defines the amount of "
               "time for the duration of the inquiry scan.",

    .numRet = 3, .Ret = (SpecField_t[3]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Inquiry_Scan_Interval",                          .size = 2,    },
      { .name = "Inquiry_Scan_Window",                            .size = 2,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read Inquiry Scan Type command", .iscmd = 1, .section = "7.3.47", .version = "1.2", .BREDR = "C125 or", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0042, .var = 0, .octet = 12, .bit = 4 },
    },
    .summary = "The HCI_Read_Inquiry_Scan_Type command is used to read the Inquiry "
               "Scan Type configuration parameter of the local BR/EDR Controller. The "
               "Inquiry Scan Type configuration parameter can set the inquiry scan to "
               "either normal interlaced scan.",

    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Inquiry_Scan_Type",                              .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read LE Host Support command", .iscmd = 1, .section = "7.3.78", .version = "4.0", .BREDR = "C116", .LE = "C116",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x006c, .var = 0, .octet = 24, .bit = 5 },
    },
    .summary = "The HCI_Read_LE_Host_Support command reads the LE Supported Host "
               "setting from the BR/ EDR Controller.",

    .numRet = 3, .Ret = (SpecField_t[3]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "LE_Supported_Host",                              .size = 1,    },
      { .name = "Unused",                                         .size = 1,    },
    },
  },

  { .name = "Read Link Policy Settings command", .iscmd = 1, .section = "7.2.9", .version = "1.1", .BREDR = "C141", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x000c, .var = 0, .octet =  5, .bit = 1 },
    },
    .summary = "The HCI_Read_Link_Policy_Settings command will read the Link Policy "
               "configuration parameter for the specified Connection_Handle. The Link "
               "Policy settings allow the Host to specify which Link Modes the Link "
               "Manager can use for the specified Connection_Handle.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numRet = 3, .Ret = (SpecField_t[3]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Link_Policy_Settings",                           .size = 2,    },
    },
    .numEvents = 2, .Events = (SpecEvent_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Role_Change" },
    },
  },

  { .name = "Read Link Quality command", .iscmd = 1, .section = "7.5.3", .version = "1.1", .BREDR = "O", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0003, .var = 0, .octet = 15, .bit = 4 },
    },
    .summary = "The HCI_Read_Link_Quality command will read the value for the Link "
               "Quality for the specified Connection_Handle.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Handle",                                         .size = 2,    .numb = 12 },
    },
    .numRet = 3, .Ret = (SpecField_t[3]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Handle",                                         .size = 2,    .numb = 12 },
      { .name = "Link_Quality",                                   .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read Link Supervision Timeout command", .iscmd = 1, .section = "7.3.41", .version = "1.1", .BREDR = "C117", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0036, .var = 0, .octet = 11, .bit = 0 },
    },
    .summary = "The HCI_Read_Link_Supervision_Timeout command will read the value for "
               "the Link Supervision Timeout configuration parameter for the device. "
               "This parameter is used by the Controller to determine link loss.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Handle",                                         .size = 2,    .numb = 12 },
    },
    .numRet = 3, .Ret = (SpecField_t[3]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Handle",                                         .size = 2,    .numb = 12 },
      { .name = "Link_Supervision_Timeout",                       .size = 2,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read LMP Handle command", .iscmd = 1, .section = "7.1.25", .version = "1.2", .BREDR = "C134", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0020, .var = 0, .octet =  3, .bit = 1 },
    },
    .summary = "The HCI_Read_LMP_Handle command will read the current LMP Handle "
               "associated with the Connection_Handle.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numRet = 4, .Ret = (SpecField_t[4]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "LMP_Handle",                                     .size = 1,    },
      { .name = "Reserved",                                       .size = 4,    },
    },
    .numEvents = 2, .Events = (SpecEvent_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Read_Clock_Offset_Complete" },
    },
  },

  { .name = "Read Local Extended Features command", .iscmd = 1, .section = "7.4.4", .version = "1.2", .BREDR = "C220", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0004, .var = 0, .octet = 14, .bit = 6 },
    },
    .summary = "The HCI_Read_Local_Extended_Features command requests a list of the "
               "supported extended features for the local device.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Page_Number",                                    .size = 1,    },
    },
    .numRet = 4, .Ret = (SpecField_t[4]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Page_Number",                                    .size = 1,    },
      { .name = "Max_Page_Number",                                .size = 1,    },
      { .name = "Extended_LMP_Features",                          .size = 8,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read Local Name command", .iscmd = 1, .section = "7.3.12", .version = "1.1", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0014, .var = 0, .octet =  7, .bit = 1 },
    },
    .summary = "The HCI_Read_Local_Name command provides the ability to read the "
               "stored user-friendly name for the BR/EDR Controller.",

    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Local_Name",                                     .size = 248,  },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read Local OOB Data command", .iscmd = 1, .section = "7.3.60", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0057, .var = 0, .octet = 17, .bit = 7 },
    },
    .summary = "The HCI_Read_Local_OOB_Data command is used to obtain a Secure Simple "
               "Pairing Hash C and Randomizer R which are intended to be transferred "
               "to a remote device using an OOB mechanism.",

    .numRet = 3, .Ret = (SpecField_t[3]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "C",                                              .size = 16,   },
      { .name = "R",                                              .size = 16,   },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read Local OOB Extended Data command", .iscmd = 1, .section = "7.3.95", .version = "4.1", .BREDR = "C142", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x007d, .var = 0, .octet = 32, .bit = 6 },
    },
    .summary = "The HCI_Read_Local_OOB_Extended_Data command is used to obtain a "
               "Secure Simple Pairing Hash C and Randomizer R associated with both "
               "P-192 and P-256 public keys, which are intended to be transferred to "
               "a remote device using an OOB mechanism.",

    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read Local Simple Pairing Options command", .iscmd = 1, .section = "7.4.9", .version = "Erratum 10734", .BREDR = "O", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x000c, .var = 0, .octet = 41, .bit = 3 },
    },
    .summary = "The HCI_Read_Local_Simple_Pairing_Options command is used to read the "
               "Secure Simple Pairing options and the maximum encryption key size "
               "supported.",

    .numRet = 3, .Ret = (SpecField_t[3]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Simple_Pairing_Options",                         .size = 1,    },
      { .name = "Max_Encryption_Key_Size",                        .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read Local Supported Codec Capabilities command", .iscmd = 1, .section = "7.4.10", .version = "5.2", .BREDR = "C156", .LE = "C156",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x000e, .var = 0, .octet = 45, .bit = 3 },
    },
    .summary = "The HCI_Read_Local_Supported_Codec_Capabilities command is used by a "
               "Host to query codec capabilities.",

    .numCmd = 3, .Cmd = (SpecField_t[3]) {
      { .name = "Codec_ID",                                       .size = 5,    },
      { .name = "Logical_Transport_Type",                         .size = 1,    },
      { .name = "Direction",                                      .size = 1,    },
    },
    .numRet = 4, .Ret = (SpecField_t[4]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Num_Codec_Capabilities",                         .size = 1,    },
      { .name = "Codec_Capability_Length",                        .size = 1,   .ref2size = "Num_Codec_Capabilities", },
      { .name = "Codec_Capability",                               .size = 1,   .ref2size = "Codec_Capability_Length", .mod = Size_Sum, },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read Local Supported Codecs command", .iscmd = 1, .section = "7.4.8", .version = "CSA2 5.2", .BREDR = "[v1] C157 [v2] O", .LE = "[v1] E [v2] O",
    .numVI = 2, .VI = (SpecCmdVar_t[2]) {
      { .ocf = 0x000d, .var = 2, .octet = 45, .bit = 2 },
      { .ocf = 0x000b, .var = 1, .octet = 29, .bit = 5 },
    },
    .summary = "The HCI_Read_Local_Supported_Codecs command is used by a Host to "
               "query a Controller’s supported codecs.",

    .numRet = 7, .Ret = (SpecField_t[7]) {
      { .name = "Status",                                         .size = 1,   .VS = { 0, 1, 1 },  },
      { .name = "Num_Supported_Standard_Codecs",                  .size = 1,   .VS = { 0, 1, 1 },  },
      { .name = "Standard_Codec_ID",                              .size = 1,   .VS = { 0, 1, 1 }, .ref2size = "Num_Supported_Standard_Codecs", },
      { .name = "Standard_Codec_Transport",                       .size = 1,   .VS = { 0, 0, 1 }, .ref2size = "Num_Supported_Standard_Codecs", },
      { .name = "Num_Supported_Vendor_Specific_Codecs",           .size = 1,   .VS = { 0, 1, 1 },  },
      { .name = "Vendor_Specific_Codec_ID",                       .size = 4,   .VS = { 0, 1, 1 }, .ref2size = "Num_Supported_Vendor_Specific_Codecs", },
      { .name = "Vendor_Specific_Codec_Transport",                .size = 1,   .VS = { 0, 0, 1 }, .ref2size = "Num_Supported_Vendor_Specific_Codecs", },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read Local Supported Commands command", .iscmd = 1, .section = "7.4.2", .version = "1.2", .BREDR = "M", .LE = "M",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0002, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Read_Local_Supported_Commands command requests a list of the "
               "supported HCI commands for the local device.",

    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Supported_Commands",                             .size = 64,   },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read Local Supported Controller Delay command", .iscmd = 1, .section = "7.4.11", .version = "5.2", .BREDR = "C156", .LE = "C156",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x000f, .var = 0, .octet = 45, .bit = 4 },
    },
    .summary = "The HCI_Read_Local_Supported_Controller_Delay command is used by a "
               "Host to query a range of supported Controller delays for a given "
               "codec configuration.",

    .numCmd = 5, .Cmd = (SpecField_t[5]) {
      { .name = "Codec_ID",                                       .size = 5,    },
      { .name = "Logical_Transport_Type",                         .size = 1,    },
      { .name = "Direction",                                      .size = 1,    },
      { .name = "Codec_Configuration_Length",                     .size = 1,    },
      { .name = "Codec_Configuration",                            .size = 1,   .ref2size = "Codec_Configuration_Length", },
    },
    .numRet = 3, .Ret = (SpecField_t[3]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Min_Controller_Delay",                           .size = 3,    },
      { .name = "Max_Controller_Delay",                           .size = 3,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read Local Supported Features command", .iscmd = 1, .section = "7.4.3", .version = "1.1", .BREDR = "M", .LE = "M",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0003, .var = 0, .octet = 14, .bit = 5 },
    },
    .summary = "The HCI_Read_Local_Supported_Features command requests a list of the "
               "supported features for the local device.",

    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "LMP_Features",                                   .size = 8,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read Local Version Information command", .iscmd = 1, .section = "7.4.1", .version = "1.1", .BREDR = "M", .LE = "M",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0001, .var = 0, .octet = 14, .bit = 3 },
    },
    .summary = "The HCI_Read_Local_Version_Information command will read the version "
               "information for the local Controller.",

    .numRet = 6, .Ret = (SpecField_t[6]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "HCI_Version",                                    .size = 1,    },
      { .name = "HCI_Subversion",                                 .size = 2,    },
      { .name = "LMP_Version",                                    .size = 1,    },
      { .name = "Company_Identifier",                             .size = 2,    },
      { .name = "LMP_Subversion",                                 .size = 2,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read Loopback Mode command", .iscmd = 1, .section = "7.6.1", .version = "1.1", .BREDR = "C123", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0001, .var = 0, .octet = 16, .bit = 0 },
    },
    .summary = "The HCI_Read_Loopback_Mode command will read the value for the "
               "setting of the BR/EDR Controller’s Loopback Mode. The setting of "
               "the Loopback Mode will determine the path of information.",

    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Loopback_Mode",                                  .size = 1,    },
    },
    .numEvents = 2, .Events = (SpecEvent_t[2]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_Triggered_Clock_Capture" },
    },
  },

  { .name = "Read Num Broadcast Retransmissions command", .iscmd = 1, .section = "7.3.31", .version = "1.1", .BREDR = "C118", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0029, .var = 0, .octet =  9, .bit = 6 },
    },
    .summary = "The HCI_Read_Num_Broadcast_Retransmissions command will read the "
               "parameter value for the Number of Broadcast Retransmissions for the "
               "BR/EDR Controller.",

    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Num_Broadcast_Retransmissions",                  .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read Number Of Supported IAC command", .iscmd = 1, .section = "7.3.43", .version = "1.1", .BREDR = "C125", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0038, .var = 0, .octet = 11, .bit = 2 },
    },
    .summary = "The HCI_Read_Number_Of_Supported_IAC command will read the value for "
               "the number of Inquiry Access Codes (IAC) that the local BR/EDR "
               "Controller can simultaneously listen for during an Inquiry Scan.",

    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Num_Supported_IAC",                              .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read Page Scan Activity command", .iscmd = 1, .section = "7.3.19", .version = "1.1", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x001b, .var = 0, .octet =  8, .bit = 0 },
    },
    .summary = "The HCI_Read_Page_Scan_Activity command will read the values for the "
               "Page Scan Interval and Page Scan Window configuration parameters. "
               "Page Scan Interval defines the amount of time between consecutive "
               "page scans. Page Scan Window defines the duration of the page scan.",

    .numRet = 3, .Ret = (SpecField_t[3]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Page_Scan_Interval",                             .size = 2,    },
      { .name = "Page_Scan_Window",                               .size = 2,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read Page Scan Type command", .iscmd = 1, .section = "7.3.51", .version = "1.2", .BREDR = "C119", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0046, .var = 0, .octet = 13, .bit = 0 },
    },
    .summary = "The HCI_Read_Page_Scan_Type command is used to read the page scan "
               "type of the local BR/EDR Controller. The Page Scan Type configuration "
               "parameter can set the page scan to either normal or interlaced scan.",

    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Page_Scan_Type",                                 .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read Page Timeout command", .iscmd = 1, .section = "7.3.15", .version = "1.1", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0017, .var = 0, .octet =  7, .bit = 4 },
    },
    .summary = "The HCI_Read_Page_Timeout command will read the value for the Page "
               "Reply Timeout configuration parameter, which determines the time the "
               "BR/EDR Controller will wait for the remote device to respond to a "
               "connection request before the local device returns a connection "
               "failure.",

    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Page_Timeout",                                   .size = 2,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read PIN Type command", .iscmd = 1, .section = "7.3.5", .version = "1.1", .BREDR = "C120 indi-", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0009, .var = 0, .octet =  6, .bit = 2 },
    },
    .summary = "The HCI_Read_PIN_Type command is used for the Host to read the value "
               "that is specified to cate whether the Host supports variable PINs or "
               "only fixed PINs.",

    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "PIN_Type",                                       .size = 1,    },
    },
    .numEvents = 2, .Events = (SpecEvent_t[2]) {
      { .name = "HCI_Flush_Occurred" },
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read Remote Extended Features command", .iscmd = 1, .section = "7.1.22", .version = "1.2", .BREDR = "C220", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x001c, .var = 0, .octet =  2, .bit = 6 },
    },
    .summary = "The HCI_Read_Remote_Extended_Features command requests a list of the "
               "supported extended features of a remote device.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Page_Number",                                    .size = 1,    },
    },
    .numEvents = 2, .Events = (SpecEvent_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Read_Remote_Supported_Features_Complete" },
    },
  },

  { .name = "Read Remote Extended Features Complete event", .iscmd = 0, .section = "7.7.34", .version = "1.2", .BREDR = "C220", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0023, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Read_Remote_Extended_Features_Complete event is used to "
               "indicate the completion of the process of the Link Manager obtaining "
               "the supported Extended features of the remote BR/EDR Controller "
               "specified by the Connection_Handle event parameter.",

    .numCmd = 5, .Cmd = (SpecField_t[5]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Page_Number",                                    .size = 1,    },
      { .name = "Max_Page_Number",                                .size = 1,    },
      { .name = "Extended_LMP_Features",                          .size = 8,    },
    },
  },

  { .name = "Read Remote Supported Features command", .iscmd = 1, .section = "7.1.21", .version = "1.1", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x001b, .var = 0, .octet =  2, .bit = 5 },
    },
    .summary = "The HCI_Read_Remote_Supported_Features command requests a list of the "
               "supported features of a remote device.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numEvents = 2, .Events = (SpecEvent_t[2]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_Remote_Name_Request_Complete" },
    },
  },

  { .name = "Read Remote Supported Features Complete event", .iscmd = 0, .section = "7.7.11", .version = "1.1", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x000b, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Read_Remote_Supported_Features_Complete event is used to "
               "indicate the completion of the process of the Link Manager obtaining "
               "the supported features of the remote BR/EDR Controller specified by "
               "the Connection_Handle event parameter.",

    .numCmd = 3, .Cmd = (SpecField_t[3]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "LMP_Features",                                   .size = 8,    },
    },
  },

  { .name = "Read Remote Version Information command", .iscmd = 1, .section = "7.1.23", .version = "1.1", .BREDR = "O", .LE = "C3",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x001d, .var = 0, .octet =  2, .bit = 7 },
    },
    .summary = "The HCI_Read_Remote_Version_Information command will read the values "
               "for the version information for the remote device associated with the "
               "Connection_Handle.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numEvents = 2, .Events = (SpecEvent_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Read_Remote_Extended_Features_Complete" },
    },
  },

  { .name = "Read Remote Version Information Complete event", .iscmd = 0, .section = "7.7.12", .version = "1.1", .BREDR = "C105", .LE = "C3",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x000c, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Read_Remote_Version_Information_Complete event is used to "
               "indicate the completion of the process of the Link Manager obtaining "
               "the version information of the remote device associated with the "
               "Connection_Handle event parameter.",

    .numCmd = 5, .Cmd = (SpecField_t[5]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Version",                                        .size = 1,    },
      { .name = "Company_Identifier",                             .size = 2,    },
      { .name = "Subversion",                                     .size = 2,    },
    },
  },

  { .name = "Read RSSI command", .iscmd = 1, .section = "7.5.4", .version = "1.1", .BREDR = "O", .LE = "C3",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0005, .var = 0, .octet = 15, .bit = 5 },
    },
    .summary = "The HCI_Read_RSSI command will read the value for the Received Signal "
               "Strength Indication (RSSI) for a Connection_Handle to another "
               "Controller.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Handle",                                         .size = 2,    .numb = 12 },
    },
    .numRet = 3, .Ret = (SpecField_t[3]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Handle",                                         .size = 2,    .numb = 12 },
      { .name = "RSSI",                                           .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read Scan Enable command", .iscmd = 1, .section = "7.3.17", .version = "1.1", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0019, .var = 0, .octet =  7, .bit = 6 },
    },
    .summary = "The HCI_Read_Scan_Enable command will read the value for the Scan "
               "Enable configuration parameter, which controls whether or not the "
               "BR/EDR Controller will periodically scan for page attempts and/or "
               "inquiry requests from other BR/EDR Controllers.",

    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Scan_Enable",                                    .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read Secure Connections Host Support command", .iscmd = 1, .section = "7.3.91", .version = "4.1", .BREDR = "C218", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0079, .var = 0, .octet = 32, .bit = 2 },
    },
    .summary = "The HCI_Read_Secure_Connections_Host_Support command is used to read "
               "the Secure Connections Host Supports parameter from the Controller.",

    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Secure_Connections_Host_Support",                .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read Simple Pairing Mode command", .iscmd = 1, .section = "7.3.58", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0055, .var = 0, .octet = 17, .bit = 5 },
    },
    .summary = "The HCI_Read_Simple_Pairing_Mode command reads the Secure Simple "
               "Pairing mode setting in the BR/EDR Controller.",

    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Simple_Pairing_Mode",                            .size = 1,    },
    },
    .numEvents = 2, .Events = (SpecEvent_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Encryption_Key_Refresh_Complete" },
    },
  },

  { .name = "Read Stored Link Key command", .iscmd = 1, .section = "7.3.8", .version = "1.1", .BREDR = "C121", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x000d, .var = 0, .octet =  6, .bit = 5 },
    },
    .summary = "The HCI_Read_Stored_Link_Key command provides the ability to read "
               "whether one or more link keys are stored in the Controller.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "BD_ADDR",                                        .size = 6,    },
      { .name = "Read_All",                                       .size = 1,    },
    },
    .numRet = 3, .Ret = (SpecField_t[3]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Max_Num_Keys",                                   .size = 2,    },
      { .name = "Num_Keys_Read",                                  .size = 2,    },
    },
  },

  { .name = "Read Synchronization Train Parameters command", .iscmd = 1, .section = "7.3.89", .version = "CSA4", .BREDR = "C203", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0077, .var = 0, .octet = 31, .bit = 7 },
    },
    .summary = "The HCI_Read_Synchronization_Train_Parameters command returns the "
               "currently configured values for the Synchronization Train "
               "functionality in the BR/EDR Controller.",

    .numRet = 4, .Ret = (SpecField_t[4]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Sync_Train_Interval",                            .size = 2,    },
      { .name = "Sync_Train_Timeout",                             .size = 4,    },
      { .name = "Service_Data",                                   .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read Synchronous Flow Control Enable command", .iscmd = 1, .section = "7.3.36", .version = "1.1", .BREDR = "C122", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x002e, .var = 0, .octet = 10, .bit = 3 },
    },
    .summary = "The HCI_Read_Synchronous_Flow_Control_Enable command provides the "
               "ability to read the Synchronous Flow Control Enable setting. By using "
               "this setting, the Host can decide if the Controller will send "
               "HCI_Number_Of_Completed_Packets events for synchronous "
               "Connection_Handles.",

    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Synchronous_Flow_Control_Enable",                .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read Transmit Power Level command", .iscmd = 1, .section = "7.3.35", .version = "1.1", .BREDR = "C152", .LE = "C3",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x002d, .var = 0, .octet = 10, .bit = 2 },
    },
    .summary = "The HCI_Read_Transmit_Power_Level command will read the values for "
               "the Transmit Power Level parameter for the specified "
               "Connection_Handle.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Type",                                           .size = 1,    },
    },
    .numRet = 3, .Ret = (SpecField_t[3]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "TX_Power_Level",                                 .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Read Voice Setting command", .iscmd = 1, .section = "7.3.27", .version = "1.1", .BREDR = "C134", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0025, .var = 0, .octet =  9, .bit = 2 },
    },
    .summary = "The HCI_Read_Voice_Setting command will read the values for the Voice "
               "Setting configuration parameter, which controls all the various "
               "settings for the voice connections.",

    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Voice_Setting",                                  .size = 2,    .numb = 10 },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Receive Synchronization Train command", .iscmd = 1, .section = "7.1.52", .version = "CSA4", .BREDR = "C204", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0044, .var = 0, .octet = 31, .bit = 3 },
    },
    .summary = "The HCI_Receive_Synchronization_Train command requests "
               "synchronization with the specified Connectionless Peripheral "
               "Broadcast transmitter.",

    .numCmd = 4, .Cmd = (SpecField_t[4]) {
      { .name = "BD_ADDR",                                        .size = 6,    },
      { .name = "Sync_Scan_Timeout",                              .size = 2,    },
      { .name = "Sync_Scan_Window",                               .size = 2,    },
      { .name = "Sync_Scan_Interval",                             .size = 2,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Status" },
    },
  },

  { .name = "Refresh Encryption Key command", .iscmd = 1, .section = "7.3.57", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0053, .var = 0, .octet = 17, .bit = 2 },
    },
    .summary = "The HCI_Refresh_Encryption_Key command is used by the Host to cause "
               "the Controller to refresh the encryption key by pausing and resuming "
               "encryption",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Reject Connection Request command", .iscmd = 1, .section = "7.1.9", .version = "1.1", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x000a, .var = 0, .octet =  1, .bit = 1 },
    },
    .summary = "The HCI_Reject_Connection_Request command is used to decline a new "
               "incoming BR/EDR connection request.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "BD_ADDR",                                        .size = 6,    },
      { .name = "Reason",                                         .size = 1,    },
    },
    .numEvents = 3, .Events = (SpecEvent_t[3]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Connection_Complete" },
      { .name = "HCI_Synchronous_Connection_Complete" },
    },
  },

  { .name = "Reject Synchronous Connection Request command", .iscmd = 1, .section = "7.1.28", .version = "1.2", .BREDR = "C134", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x002a, .var = 0, .octet = 16, .bit = 5 },
    },
    .summary = "The HCI_Reject_Synchronous_Connection_Request command is used to "
               "decline an incoming request for a synchronous link.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "BD_ADDR",                                        .size = 6,    },
      { .name = "Reason",                                         .size = 1,    },
    },
    .numEvents = 2, .Events = (SpecEvent_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Synchronous_Connection_Complete" },
    },
  },

  { .name = "Remote Host Supported Features Notification event", .iscmd = 0, .section = "7.7.50", .version = "2.1 + EDR", .BREDR = "C106", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003d, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Remote_Host_Supported_Features_Notification event is used to "
               "return the LMP extended features page containing the Host features.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "BD_ADDR",                                        .size = 6,    },
      { .name = "Host_Supported_Features",                        .size = 8,    },
    },
  },

  { .name = "Remote Name Request Cancel command", .iscmd = 1, .section = "7.1.20", .version = "1.2", .BREDR = "C106", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x001a, .var = 0, .octet =  2, .bit = 4 },
    },
    .summary = "The HCI_Remote_Name_Request_Cancel command is used to cancel an "
               "ongoing Remote Name Request.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "BD_ADDR",                                        .size = 6,    },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "BD_ADDR",                                        .size = 6,    },
    },
    .numEvents = 3, .Events = (SpecEvent_t[3]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Remote_Host_Supported_Features_Notification" },
      { .name = "HCI_Remote_Name_Request_Complete" },
    },
  },

  { .name = "Remote Name Request command", .iscmd = 1, .section = "7.1.19", .version = "1.1", .BREDR = "O", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0019, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Remote_Name_Request command is used to obtain the "
               "user-friendly name of another BR/EDR Controller.",

    .numCmd = 4, .Cmd = (SpecField_t[4]) {
      { .name = "BD_ADDR",                                        .size = 6,    },
      { .name = "Page_Scan_Repetition_Mode",                      .size = 1,    },
      { .name = "Reserved",                                       .size = 1,    },
      { .name = "Clock_Offset",                                   .size = 2,    },
    },
    .numEvents = 2, .Events = (SpecEvent_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Link_Key_Type_Changed" },
    },
  },

  { .name = "Remote Name Request Complete event", .iscmd = 0, .section = "7.7.7", .version = "1.1", .BREDR = "C106", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0007, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Remote_Name_Request_Complete event is used to indicate a "
               "remote name request has been completed.",

    .numCmd = 3, .Cmd = (SpecField_t[3]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "BD_ADDR",                                        .size = 6,    },
      { .name = "Remote_Name",                                    .size = 248,  },
    },
  },

  { .name = "Remote OOB Data Request event", .iscmd = 0, .section = "7.7.44", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0035, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Remote_OOB_Data_Request event is used to indicate that the "
               "Secure Simple Pairing Hash C and Randomizer R is required for the "
               "Secure Simple Pairing process involving the device identified by "
               "BD_ADDR.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "BD_ADDR",                                        .size = 6,    },
    },
  },

  { .name = "Remote OOB Data Request Negative Reply command", .iscmd = 1, .section = "7.1.35", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0033, .var = 0, .octet = 19, .bit = 7 },
    },
    .summary = "The HCI_Remote_OOB_Data_Request_Negative_Reply command is used to "
               "reply to an HCI_Remote_OOB_Data_Request event that the Host does not "
               "have the C and R",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "BD_ADDR",                                        .size = 6,    },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "BD_ADDR",                                        .size = 6,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Remote OOB Data Request Reply command", .iscmd = 1, .section = "7.1.34", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0030, .var = 0, .octet = 19, .bit = 4 },
    },
    .summary = "The HCI_Remote_OOB_Data_Request_Reply command is used to reply to an "
               "HCI_Remote_OOB_Data_Request event with the C and R values received "
               "via an OOB transfer from a remote BR/ EDR Controller identified by "
               "BD_ADDR.",

    .numCmd = 3, .Cmd = (SpecField_t[3]) {
      { .name = "BD_ADDR",                                        .size = 6,    },
      { .name = "C",                                              .size = 16,   },
      { .name = "R",                                              .size = 16,   },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "BD_ADDR",                                        .size = 6,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Remote OOB Extended Data Request Reply command", .iscmd = 1, .section = "7.1.53", .version = "4.1", .BREDR = "C143", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0045, .var = 0, .octet = 32, .bit = 1 },
    },
    .summary = "The HCI_Remote_OOB_Extended_Data_Request_Reply command is used to "
               "reply to an HCI_Remote_OOB_Data_Request event with the C and R values "
               "received via an OOB transfer from a remote BR/EDR Controller "
               "identified by the BD_ADDR.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "BD_ADDR",                                        .size = 6,    },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "BD_ADDR",                                        .size = 6,    },
    },
    .numEvents = 2, .Events = (SpecEvent_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Synchronization_Train_Received" },
    },
  },

  { .name = "Reset command", .iscmd = 1, .section = "7.3.2", .version = "1.1", .BREDR = "M", .LE = "M",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0003, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "For a BR/EDR Controller, the HCI_Reset command resets HCI, the Link "
               "Manager, and the Bluetooth radio. For an LE Controller, the HCI_Reset "
               "command resets HCI, the Link Layer, and LE PHY.",

    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Reset Failed Contact Counter command", .iscmd = 1, .section = "7.5.2", .version = "1.1", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0002, .var = 0, .octet = 15, .bit = 3 },
    },
    .summary = "The HCI_Reset_Failed_Contact_Counter command will reset the value for "
               "the Failed Contact Counter configuration parameter for a particular "
               "connection to another device.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Handle",                                         .size = 2,    .numb = 12 },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Handle",                                         .size = 2,    .numb = 12 },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Return Link Keys event", .iscmd = 0, .section = "7.7.21", .version = "1.1", .BREDR = "C121", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0015, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Return_Link_Keys event is used to return stored link keys "
               "after an HCI_Read_Stored_Link_Key command is used.",

    .numCmd = 3, .Cmd = (SpecField_t[3]) {
      { .name = "Num_Keys",                                       .size = 1,    },
      { .name = "BD_ADDR",                                        .size = 6,   .ref2size = "Num_Keys", },
      { .name = "Link_Key",                                       .size = 16,  .ref2size = "Num_Keys", },
    },
  },

  { .name = "Role Change event", .iscmd = 0, .section = "7.7.18", .version = "1.1", .BREDR = "C212", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0012, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Role_Change event is used to indicate that the current BR/EDR "
               "Controller role related to the particular connection has been "
               "changed.",

    .numCmd = 3, .Cmd = (SpecField_t[3]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "BD_ADDR",                                        .size = 6,    },
      { .name = "New_Role",                                       .size = 1,    },
    },
  },

  { .name = "Role Discovery command", .iscmd = 1, .section = "7.2.7", .version = "1.1", .BREDR = "O", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0009, .var = 0, .octet =  4, .bit = 7 },
    },
    .summary = "The HCI_Role_Discovery command is used for a BR/EDR Controller to "
               "determine which role the device is performing for a particular "
               "Connection_Handle.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numRet = 3, .Ret = (SpecField_t[3]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Current_Role",                                   .size = 1,    },
    },
    .numEvents = 3, .Events = (SpecEvent_t[3]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_QoS_Setup_Complete" },
      { .name = "HCI_Flow_Specification_Complete" },
    },
  },

  { .name = "SAM Status Change event", .iscmd = 0, .section = "7.7.76", .version = "5.0", .BREDR = "C219 either", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0058, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_SAM_Status_Change event is used to indicate that the local or "
               "remote SAM slot map on a particular connection has been changed.",

    .numCmd = 7, .Cmd = (SpecField_t[7]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Local_SAM_Index",                                .size = 1,    },
      { .name = "Local_SAM_TX_Availability",                      .size = 1,    },
      { .name = "Local_SAM_RX_Availability",                      .size = 1,    },
      { .name = "Remote_SAM_Index",                               .size = 1,    },
      { .name = "Remote_SAM_TX_Availability",                     .size = 1,    },
      { .name = "Remote_SAM_RX_Availability",                     .size = 1,    },
    },
  },

  { .name = "Send Keypress Notification command", .iscmd = 1, .section = "7.3.63", .version = "2.1 + EDR", .BREDR = "M to", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0060, .var = 0, .octet = 20, .bit = 2 },
    },
    .summary = "The HCI_Send_Keypress_Notification command is used during the Passkey "
               "Entry protocol by a device with KeyboardOnly IO capabilities. It is "
               "used by a Host inform the remote device when keys have been entered "
               "or erased.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "BD_ADDR",                                        .size = 6,    },
      { .name = "Notification_Type",                              .size = 1,    },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "BD_ADDR",                                        .size = 6,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Set AFH Host Channel Classification command", .iscmd = 1, .section = "7.3.46", .version = "1.2", .BREDR = "C140", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003f, .var = 0, .octet = 12, .bit = 1 },
    },
    .summary = "The HCI_Set_AFH_Host_Channel_Classification command allows the Host "
               "to specify a channel classification based on its “local "
               "information”.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "AFH_Host_Channel_Classification",                .size = 10,   .numb = 79 },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Set Connection Encryption command", .iscmd = 1, .section = "7.1.16", .version = "1.1", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0013, .var = 0, .octet =  2, .bit = 0 },
    },
    .summary = "The HCI_Set_Connection_Encryption command is used to enable and "
               "disable the link level encryption.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Encryption_Enable",                              .size = 1,    },
    },
    .numEvents = 4, .Events = (SpecEvent_t[4]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Link_Key_Request" },
      { .name = "HCI_PIN_Code_Request" },
      { .name = "HCI_Authentication_Complete" },
    },
  },

  { .name = "Set Connectionless Peripheral Broadcast command", .iscmd = 1, .section = "7.1.49", .version = "CSA4", .BREDR = "C201 transmission)", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0041, .var = 0, .octet = 31, .bit = 0 },
    },
    .summary = "The HCI_Set_Connectionless_Peripheral_Broadcast command controls "
               "Connectionless Peripheral Broadcast functionality (for in the BR/EDR "
               "Controller including enabling and disabling the broadcast.",

    .numCmd = 7, .Cmd = (SpecField_t[7]) {
      { .name = "Enable",                                         .size = 1,    },
      { .name = "LT_ADDR",                                        .size = 1,    },
      { .name = "LPO_Allowed",                                    .size = 1,    },
      { .name = "Packet_Type",                                    .size = 2,    },
      { .name = "Interval_Min",                                   .size = 2,    },
      { .name = "Interval_Max",                                   .size = 2,    },
      { .name = "Supervision_Timeout",                            .size = 2,    },
    },
    .numRet = 3, .Ret = (SpecField_t[3]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "LT_ADDR",                                        .size = 1,    },
      { .name = "Interval",                                       .size = 2,    },
    },
    .numEvents = 2, .Events = (SpecEvent_t[2]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_Truncated_Page_Complete" },
    },
  },

  { .name = "Set Connectionless Peripheral Broadcast Data command", .iscmd = 1, .section = "7.3.88", .version = "CSA4", .BREDR = "C201", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0076, .var = 0, .octet = 31, .bit = 6 },
    },
    .summary = "The HCI_Set_Connectionless_Peripheral_Broadcast_Data command is used "
               "by the Host to set Connectionless Peripheral Broadcast data in the "
               "BR/EDR Controller.",

    .numCmd = 4, .Cmd = (SpecField_t[4]) {
      { .name = "LT_ADDR",                                        .size = 1,    },
      { .name = "Fragment",                                       .size = 1,    },
      { .name = "Data_Length",                                    .size = 1,    },
      { .name = "Data",                                           .size = 1,   .ref2size = "Data_Length", },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "LT_ADDR",                                        .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Set Connectionless Peripheral Broadcast Receive command", .iscmd = 1, .section = "7.1.50", .version = "CSA4", .BREDR = "C202", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0042, .var = 0, .octet = 31, .bit = 1 },
    },
    .summary = "The HCI_Set_Connectionless_Peripheral_Broadcast_Receive command "
               "enables and disables Connectionless Peripheral Broadcast reception in "
               "the BR/EDR Controller.",

    .numCmd = 11, .Cmd = (SpecField_t[11]) {
      { .name = "Enable",                                         .size = 1,    },
      { .name = "BD_ADDR",                                        .size = 6,    },
      { .name = "LT_ADDR",                                        .size = 1,    },
      { .name = "Interval",                                       .size = 2,    },
      { .name = "Clock_Offset",                                   .size = 4,    .numb = 28 },
      { .name = "Broadcast_Clock",                                .size = 4,    .numb = 28 },
      { .name = "Supervision_Timeout",                            .size = 2,    },
      { .name = "Remote_Timing_Accuracy",                         .size = 1,    },
      { .name = "Skip",                                           .size = 1,    },
      { .name = "Packet_Type",                                    .size = 2,    },
      { .name = "AFH_Channel_Map",                                .size = 10,   .numb = 79 },
    },
    .numRet = 3, .Ret = (SpecField_t[3]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "BD_ADDR",                                        .size = 6,    },
      { .name = "LT_ADDR",                                        .size = 1,    },
    },
    .numEvents = 2, .Events = (SpecEvent_t[2]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_Connectionless_Peripheral_Broadcast_Timeout" },
    },
  },

  { .name = "Set Controller To Host Flow Control command", .iscmd = 1, .section = "7.3.38", .version = "1.1", .BREDR = "O from", .LE = "C96",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0031, .var = 0, .octet = 10, .bit = 5 },
    },
    .summary = "The HCI_Set_Controller_To_Host_Flow_Control command is used by the "
               "Host to turn flow control on or off in the direction the Controller "
               "to the Host.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Flow_Control_Enable",                            .size = 1,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Set Ecosystem Base Interval command", .iscmd = 1, .section = "7.3.100", .version = "5.2", .BREDR = "O", .LE = "O",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0082, .var = 0, .octet = 45, .bit = 1 },
    },
    .summary = "The HCI_Set_Ecosystem_Base_Interval command indicates to the "
               "Controller the base interval of the ecosystem.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Interval",                                       .size = 2,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Set Event Filter command", .iscmd = 1, .section = "7.3.3", .version = "1.1", .BREDR = "C148 Host fil-", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0005, .var = 0, .octet =  6, .bit = 0 },
    },
    .summary = "The HCI_Set_Event_Filter command is used by the Host to specify "
               "different event filters. The may issue this command multiple times to "
               "request various conditions for the same type of event filter and for "
               "different types of event ters.",

    .numCmd = 17, .Cmd = (SpecField_t[17]) {
      { .name = "Filter_Type",                                    .size = 1,    },
      { .name = "Inquiry_Result_Filter_Condition_Type",           .size = 1,    },
      { .name = "Connection_Setup_Filter_Condition_Type",         .size = 1,    },
      { .name = "Condition",                                      .size = 6,    },
      { .name = "Class_Of_Device",                                .size = 3,    },
      { .name = "Class_Of_Device_Mask",                           .size = 3,    },
      { .name = "Condition",                                      .size = 6,    },
      { .name = "BD_ADDR",                                        .size = 6,    },
      { .name = "Condition",                                      .size = 1,    },
      { .name = "Auto_Accept_Flag",                               .size = 1,    },
      { .name = "Condition",                                      .size = 7,    },
      { .name = "Class_Of_Device",                                .size = 3,    },
      { .name = "Class_Of_Device_Mask",                           .size = 3,    },
      { .name = "Auto_Accept_Flag",                               .size = 1,    },
      { .name = "Condition",                                      .size = 7,    },
      { .name = "BD_ADDR",                                        .size = 6,    },
      { .name = "Auto_Accept_Flag",                               .size = 1,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Set Event Mask command", .iscmd = 1, .section = "7.3.1", .version = "1.1", .BREDR = "M", .LE = "M",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0001, .var = 0, .octet =  5, .bit = 6 },
    },
    .summary = "The HCI_Set_Event_Mask command is used to control which events are "
               "generated by the HCI for the Host.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Event_Mask",                                     .size = 8,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 2, .Events = (SpecEvent_t[2]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_Sniff_Subrating" },
    },
  },

  { .name = "Set Event Mask Page 2 command", .iscmd = 1, .section = "7.3.69", .version = "3.0 + HS", .BREDR = "C145", .LE = "C145",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0006, .var = 0, .octet = 22, .bit = 2 },
    },
    .summary = "The HCI_Set_Event_Mask_Page 2 command is used to control which events "
               "are generated by the HCI for the Host.",

    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
  },

  { .name = "Set External Frame Configuration command", .iscmd = 1, .section = "7.3.81", .version = "CSA3", .BREDR = "C108", .LE = "O",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x006f, .var = 0, .octet = 29, .bit = 7 },
    },
    .summary = "The HCI_Set_External_Frame_Configuration command enables an external "
               "device to describe a frame structure to the Controller.",

    .numCmd = 6, .Cmd = (SpecField_t[6]) {
      { .name = "MWS_Frame_Duration",                             .size = 2,    },
      { .name = "MWS_Frame_Sync_Assert_Offset",                   .size = 2,    },
      { .name = "MWS_Frame_Sync_Assert_Jitter",                   .size = 2,    },
      { .name = "MWS_Num_Periods",                                .size = 1,    },
      { .name = "Period_Duration",                                .size = 2,   .ref2size = "MWS_Num_Periods", },
      { .name = "Period_Type",                                    .size = 1,   .ref2size = "MWS_Num_Periods", },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Set Min Encryption Key Size command", .iscmd = 1, .section = "7.3.102", .version = "5.3", .BREDR = "O", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0084, .var = 0, .octet = 45, .bit = 7 },
    },
    .summary = "The HCI_Set_Min_Encryption_Key_Size command is used to modify the "
               "minimum encryption key size that may be negotiated by the "
               "Controller.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Min_Encryption_Key_Size",                        .size = 1,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Set MWS Channel Parameters command", .iscmd = 1, .section = "7.3.80", .version = "CSA3", .BREDR = "O", .LE = "O",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x006e, .var = 0, .octet = 29, .bit = 6 },
    },
    .summary = "The HCI_Set_MWS_Channel_Parameters command enables an MWS device to "
               "inform the Controller about the MWS channel configuration.",

    .numCmd = 6, .Cmd = (SpecField_t[6]) {
      { .name = "MWS_Channel_Enable",                             .size = 1,    },
      { .name = "MWS_RX_Center_Frequency",                        .size = 2,    },
      { .name = "MWS_TX_Center_Frequency",                        .size = 2,    },
      { .name = "MWS_RX_Channel_Bandwidth",                       .size = 2,    },
      { .name = "MWS_TX_Channel_Bandwidth",                       .size = 2,    },
      { .name = "MWS_Channel_Type",                               .size = 1,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Set MWS Scan Frequency Table command", .iscmd = 1, .section = "7.3.84", .version = "CSA3", .BREDR = "O", .LE = "O",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0072, .var = 0, .octet = 30, .bit = 2 },
    },
    .summary = "The HCI_Set_MWS_Scan_Frequency_Table command specifies the "
               "frequencies represented by the frequency index supplied by the "
               "MWS_SCAN_FREQUENCY signal.",

    .numCmd = 3, .Cmd = (SpecField_t[3]) {
      { .name = "Num_Scan_Frequencies",                           .size = 1,    },
      { .name = "Scan_Frequency_Low",                             .size = 2,   .ref2size = "Num_Scan_Frequencies", },
      { .name = "Scan_Frequency_High",                            .size = 2,   .ref2size = "Num_Scan_Frequencies", },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Set MWS Signaling command", .iscmd = 1, .section = "7.3.82", .version = "CSA3", .BREDR = "O", .LE = "O",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0070, .var = 0, .octet = 30, .bit = 0 },
    },
    .summary = "The HCI_Set_MWS_Signaling command enables an MWS device to inform the "
               "Controller about the timing parameters for the MWS coexistence "
               "interface.",

    .numCmd = 15, .Cmd = (SpecField_t[15]) {
      { .name = "MWS_RX_Assert_Offset",                           .size = 2,    },
      { .name = "MWS_RX_Assert_Jitter",                           .size = 2,    },
      { .name = "MWS_RX_Deassert_Offset",                         .size = 2,    },
      { .name = "MWS_RX_Deassert_Jitter",                         .size = 2,    },
      { .name = "MWS_TX_Assert_Offset",                           .size = 2,    },
      { .name = "MWS_TX_Assert_Jitter",                           .size = 2,    },
      { .name = "MWS_TX_Deassert_Offset",                         .size = 2,    },
      { .name = "MWS_TX_Deassert_Jitter",                         .size = 2,    },
      { .name = "MWS_Pattern_Assert_Offset",                      .size = 2,    },
      { .name = "MWS_Pattern_Assert_Jitter",                      .size = 2,    },
      { .name = "MWS_Inactivity_Duration_Assert_Offset",          .size = 2,    },
      { .name = "MWS_Inactivity_Duration_Assert_Jitter",          .size = 2,    },
      { .name = "MWS_Scan_Frequency_Assert_Offset",               .size = 2,    },
      { .name = "MWS_Scan_Frequency_Assert_Jitter",               .size = 2,    },
      { .name = "MWS_Priority_Assert_Offset_Request",             .size = 2,    },
    },
    .numRet = 9, .Ret = (SpecField_t[9]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Bluetooth_RX_Priority_Assert_Offset",            .size = 2,    },
      { .name = "Bluetooth_RX_Priority_Assert_Jitter",            .size = 2,    },
      { .name = "Bluetooth_RX_Priority_Deassert_Offset",          .size = 2,    },
      { .name = "Bluetooth_RX_Priority_Deassert_Jitter",          .size = 2,    },
      { .name = "Bluetooth_TX_On_Assert_Offset",                  .size = 2,    },
      { .name = "Bluetooth_TX_On_Assert_Jitter",                  .size = 2,    },
      { .name = "Bluetooth_TX_On_Deassert_Offset",                .size = 2,    },
      { .name = "Bluetooth_TX_On_Deassert_Jitter",                .size = 2,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Set MWS Transport Layer command", .iscmd = 1, .section = "7.3.83", .version = "CSA3", .BREDR = "C109", .LE = "C109",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0071, .var = 0, .octet = 30, .bit = 1 },
    },
    .summary = "The HCI_Set_MWS_Transport_Layer command selects the MWS coexistence "
               "signaling transport layer in the Controller.",

    .numCmd = 3, .Cmd = (SpecField_t[3]) {
      { .name = "Transport_Layer",                                .size = 1,    },
      { .name = "To_MWS_Baud_Rate",                               .size = 4,    },
      { .name = "From_MWS_Baud_Rate",                             .size = 4,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Set MWS_PATTERN Configuration command", .iscmd = 1, .section = "7.3.85", .version = "CSA3", .BREDR = "C136", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0073, .var = 0, .octet = 30, .bit = 4 },
    },
    .summary = "The HCI_Set_MWS_PATTERN_Configuration command specifies the "
               "configuration of the pattern indicated over the MWS Coexistence "
               "Transport Layer.",

    .numCmd = 4, .Cmd = (SpecField_t[4]) {
      { .name = "MWS_Pattern_Index",                              .size = 1,    },
      { .name = "MWS_Pattern_Num_Intervals",                      .size = 1,    },
      { .name = "MWS_Pattern_Interval_Duration",                  .size = 2,   .ref2size = "MWS_Pattern_Num_Intervals", },
      { .name = "MWS_Pattern_Interval_Type",                      .size = 1,   .ref2size = "MWS_Pattern_Num_Intervals", },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Set Reserved LT_ADDR command", .iscmd = 1, .section = "7.3.86", .version = "CSA4", .BREDR = "C201", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0074, .var = 0, .octet = 31, .bit = 4 },
    },
    .summary = "The HCI_Set_Reserved_LT_ADDR command requests that the BR/EDR "
               "Controller reserve a specific LT_ADDR for the purposes of "
               "Connectionless Peripheral Broadcast.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "LT_ADDR",                                        .size = 1,    },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "LT_ADDR",                                        .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Set Triggered Clock Capture command", .iscmd = 1, .section = "7.5.12", .version = "CSA4", .BREDR = "O", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x000d, .var = 0, .octet = 30, .bit = 5 },
    },
    .summary = "The HCI_Set_Triggered_Clock_Capture command is used to configure the "
               "Controller to return events containing an estimate of a piconet or "
               "the local Bluetooth clock.",

    .numCmd = 5, .Cmd = (SpecField_t[5]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Enable",                                         .size = 1,    },
      { .name = "Which_Clock",                                    .size = 1,    },
      { .name = "LPO_Allowed",                                    .size = 1,    },
      { .name = "Num_Clock_Captures_To_Filter",                   .size = 1,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Setup Synchronous Connection command", .iscmd = 1, .section = "7.1.26", .version = "1.2", .BREDR = "C134", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0028, .var = 0, .octet = 16, .bit = 3 },
    },
    .summary = "The HCI_Setup_Synchronous_Connection command adds a new or modifies "
               "an existing synchronous logical transport (SCO or eSCO) on a physical "
               "link depending on the Connection_Handle parameter specified.",

    .numCmd = 7, .Cmd = (SpecField_t[7]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Transmit_Bandwidth",                             .size = 4,    },
      { .name = "Receive_Bandwidth",                              .size = 4,    },
      { .name = "Max_Latency",                                    .size = 2,    },
      { .name = "Voice_Setting",                                  .size = 2,    .numb = 10 },
      { .name = "Retransmission_Effort",                          .size = 1,    },
      { .name = "Packet_Type",                                    .size = 2,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Simple Pairing Complete event", .iscmd = 0, .section = "7.7.45", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0036, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Simple_Pairing_Complete event is used to indicate that the "
               "Secure Simple Pairing process has completed.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "BD_ADDR",                                        .size = 6,    },
    },
  },

  { .name = "Sniff Mode command", .iscmd = 1, .section = "7.2.2", .version = "1.1", .BREDR = "C214", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0003, .var = 0, .octet =  4, .bit = 2 },
    },
    .summary = "The HCI_Sniff_Mode command is used to alter the behavior of the LM "
               "and have the LM place the local or remote device into Sniff mode.",

    .numCmd = 5, .Cmd = (SpecField_t[5]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Sniff_Max_Interval",                             .size = 2,    },
      { .name = "Sniff_Min_Interval",                             .size = 2,    },
      { .name = "Sniff_Attempt",                                  .size = 2,    },
      { .name = "Sniff_Timeout",                                  .size = 2,    },
    },
    .numEvents = 2, .Events = (SpecEvent_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Mode_Change" },
    },
  },

  { .name = "Sniff Subrating command", .iscmd = 1, .section = "7.2.14", .version = "2.1 + EDR", .BREDR = "C221", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0011, .var = 0, .octet = 17, .bit = 4 },
    },
    .summary = "The HCI_Sniff_Subrating command is used to configure the sniff "
               "subrating parameters in the local device.",

    .numCmd = 4, .Cmd = (SpecField_t[4]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Max_Latency",                                    .size = 2,    },
      { .name = "Min_Remote_Timeout",                             .size = 2,    },
      { .name = "Min_Local_Timeout",                              .size = 2,    },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numEvents = 3, .Events = (SpecEvent_t[3]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Flow_Specification_Complete" },
      { .name = "HCI_QoS_Setup_Complete" },
    },
  },

  { .name = "Sniff Subrating event", .iscmd = 0, .section = "7.7.37", .version = "2.1 + EDR", .BREDR = "C221 local", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x002e, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Sniff_Subrating event is used to inform the Host of the and "
               "remote transmit and receive latencies.",

    .numCmd = 6, .Cmd = (SpecField_t[6]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Max_TX_Latency",                                 .size = 2,    },
      { .name = "Max_RX_Latency",                                 .size = 2,    },
      { .name = "Min_Remote_Timeout",                             .size = 2,    },
      { .name = "Min_Local_Timeout",                              .size = 2,    },
    },
  },

  { .name = "Start Synchronization Train command", .iscmd = 1, .section = "7.1.51", .version = "CSA4", .BREDR = "C203", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0043, .var = 0, .octet = 31, .bit = 2 },
    },
    .summary = "The HCI_Start_Synchronization_Train command enables the "
               "Synchronization Train on the BR/EDR Controller using the currently "
               "configured Synchronization Train parameters.",

    .numEvents = 2, .Events = (SpecEvent_t[2]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_Connectionless_Peripheral_Broadcast_Timeout" },
    },
  },

  { .name = "Switch Role command", .iscmd = 1, .section = "7.2.8", .version = "1.1", .BREDR = "C212", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x000b, .var = 0, .octet =  5, .bit = 0 },
    },
    .summary = "The HCI_Switch_Role command is used to switch Central and Peripheral "
               "roles of the devices on either side of a connection.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "BD_ADDR",                                        .size = 6,    },
      { .name = "Role",                                           .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Synchronization Train Complete event", .iscmd = 0, .section = "7.7.67", .version = "CSA4", .BREDR = "C203", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x004f, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Synchronization_Train_Complete event indicates that the "
               "Synchronization Train has completed.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
  },

  { .name = "Synchronization Train Received event", .iscmd = 0, .section = "7.7.68", .version = "CSA4", .BREDR = "C204", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0050, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Synchronization_Train_Received event provides the status of "
               "Synchronization Train packets received from the device with the given "
               "BD_ADDR.",

    .numCmd = 8, .Cmd = (SpecField_t[8]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "BD_ADDR",                                        .size = 6,    },
      { .name = "Clock_Offset",                                   .size = 4,    .numb = 28 },
      { .name = "AFH_Channel_Map",                                .size = 10,   .numb = 79 },
      { .name = "LT_ADDR",                                        .size = 1,    },
      { .name = "Next_Broadcast_Instant",                         .size = 4,    .numb = 28 },
      { .name = "Connectionless_Peripheral_Broadcast_Interval",   .size = 2,    },
      { .name = "Service_Data",                                   .size = 1,    },
    },
  },

  { .name = "Synchronous Connection Changed event", .iscmd = 0, .section = "7.7.36", .version = "1.2", .BREDR = "C134", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x002d, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Synchronous_Connection_Changed event indicates to the Host "
               "that an existing synchronous connection has been reconfigured.",

    .numCmd = 6, .Cmd = (SpecField_t[6]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Transmission_Interval",                          .size = 1,    },
      { .name = "Retransmission_Window",                          .size = 1,    },
      { .name = "RX_Packet_Length",                               .size = 2,    },
      { .name = "TX_Packet_Length",                               .size = 2,    },
    },
  },

  { .name = "Synchronous Connection Complete event", .iscmd = 0, .section = "7.7.35", .version = "1.2", .BREDR = "C134", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x002c, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Synchronous_Connection_Complete event indicates to both the "
               "Hosts that a new synchronous connection has been established.",

    .numCmd = 9, .Cmd = (SpecField_t[9]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "BD_ADDR",                                        .size = 6,    },
      { .name = "Link_Type",                                      .size = 1,    },
      { .name = "Transmission_Interval",                          .size = 1,    },
      { .name = "Retransmission_Window",                          .size = 1,    },
      { .name = "RX_Packet_Length",                               .size = 2,    },
      { .name = "TX_Packet_Length",                               .size = 2,    },
      { .name = "Air_Mode",                                       .size = 1,    },
    },
  },

  { .name = "Triggered Clock Capture event", .iscmd = 0, .section = "7.7.66", .version = "CSA4", .BREDR = "C110", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x004e, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Triggered_Clock_Capture event reports the Bluetooth clock "
               "when an external trigger occurred.",

    .numCmd = 4, .Cmd = (SpecField_t[4]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Which_Clock",                                    .size = 1,    },
      { .name = "Clock",                                          .size = 4,    .numb = 28 },
      { .name = "Slot_Offset",                                    .size = 2,    },
    },
  },

  { .name = "Truncated Page Cancel command", .iscmd = 1, .section = "7.1.48", .version = "CSA4", .BREDR = "C129", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0040, .var = 0, .octet = 30, .bit = 7 },
    },
    .summary = "The HCI_Truncated_Page_Cancel command is used to cancel an ongoing "
               "Truncated Page.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "BD_ADDR",                                        .size = 6,    },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "BD_ADDR",                                        .size = 6,    },
    },
    .numEvents = 2, .Events = (SpecEvent_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Truncated_Page_Complete" },
    },
  },

  { .name = "Truncated Page command", .iscmd = 1, .section = "7.1.47", .version = "CSA4", .BREDR = "C129", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003f, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Truncated_Page command will cause the BR/EDR Controller to "
               "page the BR/EDR Controller with the BD_ADDR specified by the command "
               "parameters and abort the page sequence after receiving the ID "
               "response packet.",

    .numCmd = 3, .Cmd = (SpecField_t[3]) {
      { .name = "BD_ADDR",                                        .size = 6,    },
      { .name = "Page_Scan_Repetition_Mode",                      .size = 1,    },
      { .name = "Clock_Offset",                                   .size = 2,    },
    },
    .numEvents = 2, .Events = (SpecEvent_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Synchronous_Connection_Complete" },
    },
  },

  { .name = "Truncated Page Complete event", .iscmd = 0, .section = "7.7.71", .version = "CSA4", .BREDR = "C129", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0053, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_Truncated_Page_Complete event indicates to the Host that a "
               "Truncated Page has completed.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "BD_ADDR",                                        .size = 6,    },
    },
  },

  { .name = "User Confirmation Request event", .iscmd = 0, .section = "7.7.42", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0033, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_User_Confirmation_Request event is used to indicate that user "
               "confirmation of a numeric value is required.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "BD_ADDR",                                        .size = 6,    },
      { .name = "Numeric_Value",                                  .size = 4,    },
    },
  },

  { .name = "User Confirmation Request Negative Reply command", .iscmd = 1, .section = "7.1.31", .version = "2.1 + EDR", .BREDR = "M will", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x002d, .var = 0, .octet = 19, .bit = 1 },
    },
    .summary = "The HCI_User_Confirmation_Request_Negative_Reply command is used to "
               "reply to an HCI_User_Confirmation_Request event and indicates that "
               "the user selected “no”. This command terminate Secure Simple "
               "Pairing.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "BD_ADDR",                                        .size = 6,    },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "BD_ADDR",                                        .size = 6,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "User Confirmation Request Reply command", .iscmd = 1, .section = "7.1.30", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x002c, .var = 0, .octet = 19, .bit = 0 },
    },
    .summary = "The HCI_User_Confirmation_Request_Reply command is used to reply to "
               "an HCI_User_Confirmation_Request event and indicates that the user "
               "selected “yes”. It is also used when the Host has no input and no "
               "output capabilities.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "BD_ADDR",                                        .size = 6,    },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "BD_ADDR",                                        .size = 6,    },
    },
    .numEvents = 3, .Events = (SpecEvent_t[3]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_IO_Capability_Response" },
      { .name = "HCI_Remote_OOB_Data_Request" },
    },
  },

  { .name = "User Passkey Notification event", .iscmd = 0, .section = "7.7.48", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003b, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_User_Passkey_Notification event is used to provide a passkey "
               "for the Host to display to the user as required as part of a Secure "
               "Simple Pairing process.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "BD_ADDR",                                        .size = 6,    },
      { .name = "Passkey",                                        .size = 4,    },
    },
  },

  { .name = "User Passkey Request event", .iscmd = 0, .section = "7.7.43", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0034, .var = 0, .octet =  0, .bit = 0 },
    },
    .summary = "The HCI_User_Passkey_Request event is used to indicate that a passkey "
               "is required as part of a Secure Simple Pairing process.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "BD_ADDR",                                        .size = 6,    },
    },
  },

  { .name = "User Passkey Request Negative Reply command", .iscmd = 1, .section = "7.1.33", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x002f, .var = 0, .octet = 19, .bit = 3 },
    },
    .summary = "The HCI_User_Passkey_Request_Negative_Reply command is used to reply "
               "to an HCI_User_Passkey_Request event and indicates the Host could not "
               "provide a passkey. This command will terminate Secure Simple "
               "Pairing.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "BD_ADDR",                                        .size = 6,    },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "BD_ADDR",                                        .size = 6,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "User Passkey Request Reply command", .iscmd = 1, .section = "7.1.32", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x002e, .var = 0, .octet = 19, .bit = 2 },
    },
    .summary = "The HCI_User_Passkey_Request_Reply command is used to reply to an "
               "HCI_User_Passkey_Request event and specifies the Numeric_Value "
               "(passkey) entered by the user to be used in the Secure Simple Pairing "
               "process.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "BD_ADDR",                                        .size = 6,    },
      { .name = "Numeric_Value",                                  .size = 4,    },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "BD_ADDR",                                        .size = 6,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Write AFH Channel Assessment Mode command", .iscmd = 1, .section = "7.3.54", .version = "1.2", .BREDR = "C140 Controller’s", .LE = "C58",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0049, .var = 0, .octet = 13, .bit = 3 },
    },
    .summary = "The HCI_Write_AFH_Channel_Assessment_Mode command will write the "
               "value for the Channel Classification Mode configuration parameter. "
               "This value is used to enable or disable the channel assessment "
               "scheme.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "AFH_Channel_Assessment_Mode",                    .size = 1,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Write Authenticated Payload Timeout command", .iscmd = 1, .section = "7.3.94", .version = "4.1", .BREDR = "C151", .LE = "C7",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x007c, .var = 0, .octet = 32, .bit = 5 },
    },
    .summary = "The HCI_Write_Authenticated_Payload_Timeout command is used to write "
               "the Authenticated Payload Timeout parameter, which is used to set the "
               "maximum time between packets being received from the remote device "
               "without a valid MIC.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Authenticated_Payload_Timeout",                  .size = 2,    },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Write Authentication Enable command", .iscmd = 1, .section = "7.3.24", .version = "1.1", .BREDR = "O", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0020, .var = 0, .octet =  8, .bit = 5 },
    },
    .summary = "The HCI_Write_Authentication_Enable command will write the value for "
               "the Authentication Enable parameter, which controls whether the "
               "Bluetooth device will require authentication for each connection with "
               "other Bluetooth devices.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Authentication_Enable",                          .size = 1,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Write Automatic Flush Timeout command", .iscmd = 1, .section = "7.3.30", .version = "1.1", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0028, .var = 0, .octet =  9, .bit = 5 },
    },
    .summary = "The HCI_Write_Automatic_Flush_Timeout command will write the value "
               "for the Flush Timeout configuration parameter for the specified "
               "Connection_Handle. The Flush Timeout parameter is only used for ACL "
               "connections.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Flush_Timeout",                                  .size = 2,    },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Write Class of Device command", .iscmd = 1, .section = "7.3.26", .version = "1.1", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0024, .var = 0, .octet =  9, .bit = 1 },
    },
    .summary = "The HCI_Write_Class_of_Device command will write the value for the "
               "Class_of_Device configuration parameter, which is used to indicate "
               "its capabilities to other devices.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Class_Of_Device",                                .size = 3,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Write Connection Accept Timeout command", .iscmd = 1, .section = "7.3.14", .version = "1.1", .BREDR = "M con-", .LE = "C40",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0016, .var = 0, .octet =  7, .bit = 3 },
    },
    .summary = "The HCI_Write_Connection_Accept_Timeout command will write the value "
               "for the Connection Accept Timeout configuration parameter, which "
               "allows the Controller to automatically deny a nection request after a "
               "specified period has occurred, and to refuse a new connection.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Connection_Accept_Timeout",                      .size = 2,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Write Current IAC LAP command", .iscmd = 1, .section = "7.3.45", .version = "1.1", .BREDR = "C125", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x003a, .var = 0, .octet = 11, .bit = 4 },
    },
    .summary = "The HCI_Write_Current_IAC LAP command will write the LAP(s) used to "
               "create the Inquiry Access Codes (IAC) that the local BR/ EDR "
               "Controller is simultaneously scanning for during Inquiry Scans.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Num_Current_IAC",                                .size = 1,    },
      { .name = "IAC_LAP",                                        .size = 3,   .ref2size = "Num_Current_IAC", },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Write Default Erroneous Data Reporting command", .iscmd = 1, .section = "7.3.65", .version = "2.1 + EDR", .BREDR = "C135", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x005b, .var = 0, .octet = 18, .bit = 3 },
    },
    .summary = "The HCI_Write_Default_Erroneous_Data_Reporting command will write the "
               "value for the Erroneous Data Reporting configuration parameter, which "
               "controls whether the Bluetooth Controller will provide data for every "
               "(e)SCO interval, with the Packet_Status_Flag in HCI Synchronous Data "
               "packets set according to HCI Synchronous Data packets.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Erroneous_Data_Reporting",                       .size = 1,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Write Default Link Policy Settings command", .iscmd = 1, .section = "7.2.12", .version = "1.2", .BREDR = "C141", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x000f, .var = 0, .octet =  5, .bit = 4 },
    },
    .summary = "The HCI_Write_Default_Link_Policy_Settings command will write the "
               "Default Link Policy configuration parameter for all new connections.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Default_Link_Policy_Settings",                   .size = 2,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Write Extended Inquiry Length command", .iscmd = 1, .section = "7.3.99", .version = "4.1", .BREDR = "C128", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0081, .var = 0, .octet = 33, .bit = 3 },
    },
    .summary = "The HCI_Write_Extended_Inquiry_Length command is used to write the "
               "Extended Inquiry Length parameter to the Controller.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Extended_Inquiry_Length",                        .size = 2,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Write Extended Inquiry Response command", .iscmd = 1, .section = "7.3.56", .version = "2.1 + EDR", .BREDR = "C205", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0052, .var = 0, .octet = 17, .bit = 1 },
    },
    .summary = "The HCI_Write_Extended_Inquiry_Response command will write the data "
               "that the BR/EDR Controller sends in the extended inquiry response "
               "packet during inquiry response.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "FEC_Required",                                   .size = 1,    },
      { .name = "Extended_Inquiry_Response",                      .size = 240,  },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Write Extended Page Timeout command", .iscmd = 1, .section = "7.3.97", .version = "4.1", .BREDR = "O", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x007f, .var = 0, .octet = 33, .bit = 1 },
    },
    .summary = "The HCI_Write_Extended_Page_Timeout command is used to write the "
               "Extended Page Timeout parameter to the Controller.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Extended_Page_Timeout",                          .size = 2,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Write Flow Control Mode command", .iscmd = 1, .section = "7.3.73", .version = "3.0 + HS", .BREDR = "C124", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0067, .var = 0, .octet = 23, .bit = 1 },
    },
    .summary = "The HCI_Write_Flow_Control_Mode command sets the value of the "
               "Flow_Control_Mode configuration parameter for this Controller.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Flow_Control_Mode",                              .size = 1,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Write Hold Mode Activity command", .iscmd = 1, .section = "7.3.34", .version = "1.1", .BREDR = "C213", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x002c, .var = 0, .octet = 10, .bit = 1 },
    },
    .summary = "The HCI_Write_Hold_Mode_Activity command is used to write which "
               "activities should be suspended when the BR/EDR Controller is in Hold "
               "mode.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Hold_Mode_Activity",                             .size = 1,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Write Inquiry Mode command", .iscmd = 1, .section = "7.3.50", .version = "1.2", .BREDR = "C146", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0045, .var = 0, .octet = 12, .bit = 7 },
    },
    .summary = "The HCI_Write_Inquiry_Mode command is used to write the Inquiry Mode "
               "configuration parameter of the local BR/EDR Controller.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Inquiry_Mode",                                   .size = 1,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Write Inquiry Scan Activity command", .iscmd = 1, .section = "7.3.22", .version = "1.1", .BREDR = "C125", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x001e, .var = 0, .octet =  8, .bit = 3 },
    },
    .summary = "The HCI_Write_Inquiry_Scan_Activity command will write the value for "
               "Inquiry Scan Interval and Inquiry Scan Window configuration "
               "parameters. Inquiry Scan Interval defines the amount of time between "
               "consecutive inquiry scans. Inquiry Scan Window defines the amount of "
               "time for the duration of the inquiry scan.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Inquiry_Scan_Interval",                          .size = 2,    },
      { .name = "Inquiry_Scan_Window",                            .size = 2,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Write Inquiry Scan Type command", .iscmd = 1, .section = "7.3.48", .version = "1.2", .BREDR = "C125 or", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0043, .var = 0, .octet = 12, .bit = 5 },
    },
    .summary = "The HCI_Write_Inquiry_Scan_Type command is used to write the Inquiry "
               "Scan Type configuration parameter of the local BR/EDR Controller. The "
               "Inquiry Scan Type configuration parameter can set the inquiry scan to "
               "either normal interlaced scan.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Scan_Type",                                      .size = 1,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Write Inquiry Transmit Power Level command", .iscmd = 1, .section = "7.3.62", .version = "2.1 + EDR", .BREDR = "C127", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0059, .var = 0, .octet = 18, .bit = 1 },
    },
    .summary = "The HCI_Write_Inquiry_Transmit_Power_Level command is used to write "
               "the transmit power level used to transmit the inquiry (ID) data "
               "packets.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "TX_Power",                                       .size = 1,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Write LE Host Support command", .iscmd = 1, .section = "7.3.79", .version = "4.0", .BREDR = "C153", .LE = "O",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0006, .var = 0, .octet = 24, .bit = 6 },
    },
    .summary = "The HCI_Write_LE_Host_Support command writes the LE Supported Host "
               "setting to the BR/EDR Controller.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "LE_Supported_Host",                              .size = 1,    },
      { .name = "Unused",                                         .size = 1,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Write Link Policy Settings command", .iscmd = 1, .section = "7.2.10", .version = "1.1", .BREDR = "C141", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x000d, .var = 0, .octet =  5, .bit = 2 },
    },
    .summary = "The HCI_Write_Link_Policy_Settings command will write the Link Policy "
               "configuration parameter for the specified Connection_Handle. The Link "
               "Policy settings allow the Host to specify which Link Modes the Link "
               "Manager can use for the specified Connection_Handle.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "Link_Policy_Settings",                           .size = 2,    },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Write Link Supervision Timeout command", .iscmd = 1, .section = "7.3.42", .version = "1.1", .BREDR = "O", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0037, .var = 0, .octet = 11, .bit = 1 },
    },
    .summary = "The HCI_Write_Link_Supervision_Timeout command will write the value "
               "for the Link Supervision Timeout configuration parameter for the "
               "device. This parameter is used by the Controller to determine link "
               "loss.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Handle",                                         .size = 2,    .numb = 12 },
      { .name = "Link_Supervision_Timeout",                       .size = 2,    },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Handle",                                         .size = 2,    .numb = 12 },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Write Local Name command", .iscmd = 1, .section = "7.3.11", .version = "1.1", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0013, .var = 0, .octet =  7, .bit = 0 },
    },
    .summary = "The HCI_Write_Local_Name command provides the ability to modify the "
               "user-friendly name for the BR/ EDR Controller.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Local_Name",                                     .size = 248,  },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Write Loopback Mode command", .iscmd = 1, .section = "7.6.2", .version = "1.1", .BREDR = "C123", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0002, .var = 0, .octet = 16, .bit = 1 },
    },
    .summary = "The HCI_Write_Loopback_Mode command will write the value for the "
               "setting of the BR/EDR Controllers Loopback Mode. The setting of the "
               "Loopback Mode will determine the path of information.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Loopback_Mode",                                  .size = 1,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Write Num Broadcast Retransmissions command", .iscmd = 1, .section = "7.3.32", .version = "1.1", .BREDR = "O", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x002a, .var = 0, .octet =  9, .bit = 7 },
    },
    .summary = "The HCI_Write_Num_Broadcast_Retransmissions command will write the "
               "parameter value for the Number of Broadcast Retransmissions for the "
               "BR/EDR Controller.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Num_Broadcast_Retransmissions",                  .size = 1,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Write Page Scan Activity command", .iscmd = 1, .section = "7.3.20", .version = "1.1", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x001c, .var = 0, .octet =  8, .bit = 1 },
    },
    .summary = "The HCI_Write_Page_Scan_Activity command will write the value for "
               "Page Scan Interval and Page Scan Window configuration parameters. "
               "Page Scan Interval defines the amount of time between consecutive "
               "page scans. Page Scan Window defines the duration of the page scan.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Page_Scan_Interval",                             .size = 2,    },
      { .name = "Page_Scan_Window",                               .size = 2,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Write Page Scan Type command", .iscmd = 1, .section = "7.3.52", .version = "1.2", .BREDR = "C154", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0047, .var = 0, .octet = 13, .bit = 1 },
    },
    .summary = "The HCI_Write_Page_Scan_Type command is used to write the page scan "
               "type of the local BR/EDR Controller. The Page Scan Type configuration "
               "parameter can set the page scan to either normal or interlaced scan.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Page_Scan_Type",                                 .size = 1,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Write Page Timeout command", .iscmd = 1, .section = "7.3.16", .version = "1.1", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0018, .var = 0, .octet =  7, .bit = 5 },
    },
    .summary = "The HCI_Write_Page_Timeout command will write the value for the Page "
               "Reply Timeout configuration parameter, which allows the BR/EDR "
               "Controller to define the amount of time a connection request will "
               "wait for the remote device to respond before the local device returns "
               "a connection failure.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Page_Timeout",                                   .size = 2,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Write PIN Type command", .iscmd = 1, .section = "7.3.6", .version = "1.1", .BREDR = "O", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x000a, .var = 0, .octet =  6, .bit = 3 },
    },
    .summary = "The HCI_Write_PIN_Type command is used for the Host to specify "
               "whether the Host supports variable PIN or only fixed PINs.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "PIN_Type",                                       .size = 1,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Write Scan Enable command", .iscmd = 1, .section = "7.3.18", .version = "1.1", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x001a, .var = 0, .octet =  7, .bit = 7 },
    },
    .summary = "The HCI_Write_Scan_Enable command will write the value for the Scan "
               "Enable configuration parameter, which controls whether or not the "
               "BR/EDR Controller will periodically scan for page attempts and/or "
               "inquiry requests from other BR/EDR Controllers.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Scan_Enable",                                    .size = 1,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Write Secure Connections Host Support command", .iscmd = 1, .section = "7.3.92", .version = "4.1", .BREDR = "C218", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x007a, .var = 0, .octet = 32, .bit = 3 },
    },
    .summary = "The HCI_Write_Secure_Connections_Host_Support command is used to "
               "write the Secure Connections Host Supports parameter to the "
               "Controller.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Secure_Connections_Host_Support",                .size = 1,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Write Secure Connections Test Mode command", .iscmd = 1, .section = "7.6.8", .version = "4.1", .BREDR = "C138", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x000a, .var = 0, .octet = 32, .bit = 7 },
    },
    .summary = "The HCI_Write_Secure_Connections_Test_Mode command is used to put the "
               "Controller in a test mode where DM1 packets are not allowed to be "
               "used for ACL-U traffic and/or the contents of eSCO payloads can be "
               "looped back.",

    .numCmd = 2, .Cmd = (SpecField_t[2]) {
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
      { .name = "eSCO_Loopback_Mode",                             .size = 1,    },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Connection_Handle",                              .size = 2,    .numb = 12 },
    },
  },

  { .name = "Write Simple Pairing Debug Mode command", .iscmd = 1, .section = "7.6.4", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0004, .var = 0, .octet = 19, .bit = 5 },
    },
    .summary = "The HCI_Write_Simple_Pairing_Debug_Mode command configures the BR/EDR "
               "Controller to use a predefined Diffie Hellman private key for Secure "
               "Simple Pairing to enable debug equipment to monitor the encrypted "
               "connection.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Simple_Pairing_Debug_Mode",                      .size = 1,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Write Simple Pairing Mode command", .iscmd = 1, .section = "7.3.59", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0056, .var = 0, .octet = 17, .bit = 6 },
    },
    .summary = "The HCI_Write_Simple_Pairing_Mode command writes the Secure Simple "
               "Pairing mode setting in the BR/EDR Controller.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Simple_Pairing_Mode",                            .size = 1,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Write Stored Link Key command", .iscmd = 1, .section = "7.3.9", .version = "1.1", .BREDR = "O", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0011, .var = 0, .octet =  6, .bit = 6 },
    },
    .summary = "The HCI_Write_Stored_Link_Key command provides the ability to write "
               "one or more link keys to be stored in the Controller.",

    .numCmd = 3, .Cmd = (SpecField_t[3]) {
      { .name = "Num_Keys_To_Write",                              .size = 1,    },
      { .name = "BD_ADDR",                                        .size = 6,   .ref2size = "Num_Keys_To_Write", },
      { .name = "Link_Key",                                       .size = 16,  .ref2size = "Num_Keys_To_Write", },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Num_Keys_Written",                               .size = 1,    },
    },
    .numEvents = 2, .Events = (SpecEvent_t[2]) {
      { .name = "HCI_Return_Link_Keys" },
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Write Synchronization Train Parameters command", .iscmd = 1, .section = "7.3.90", .version = "CSA4", .BREDR = "C203", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0078, .var = 0, .octet = 32, .bit = 0 },
    },
    .summary = "The HCI_Write_Synchronization_Train_Parameters command configures the "
               "Synchronization Train functionality in the BR/EDR Controller.",

    .numCmd = 4, .Cmd = (SpecField_t[4]) {
      { .name = "Interval_Min",                                   .size = 2,    },
      { .name = "Interval_Max",                                   .size = 2,    },
      { .name = "Sync_Train_Timeout",                             .size = 4,    },
      { .name = "Service_Data",                                   .size = 1,    },
    },
    .numRet = 2, .Ret = (SpecField_t[2]) {
      { .name = "Status",                                         .size = 1,    },
      { .name = "Sync_Train_Interval",                            .size = 2,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Write Synchronous Flow Control Enable command", .iscmd = 1, .section = "7.3.37", .version = "1.1", .BREDR = "C135", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x002f, .var = 0, .octet = 10, .bit = 4 },
    },
    .summary = "The HCI_Write_Synchronous_Flow_Control_Enable command provides the "
               "ability to write the Synchronous Flow Control Enable setting. By "
               "using this setting, the Host can decide if the Controller will send "
               "HCI_Number_Of_Completed_Packets events for synchronous "
               "Connection_Handles.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Synchronous_Flow_Control_Enable",                .size = 1,    },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .name = "Write Voice Setting command", .iscmd = 1, .section = "7.3.28", .version = "1.1", .BREDR = "C134", .LE = "E",
    .numVI = 1, .VI = (SpecCmdVar_t[1]) {
      { .ocf = 0x0026, .var = 0, .octet =  9, .bit = 3 },
    },
    .summary = "The HCI_Write_Voice_Setting command will write the values for the "
               "Voice Setting configuration parameter, which controls all the various "
               "settings for the voice connections.",

    .numCmd = 1, .Cmd = (SpecField_t[1]) {
      { .name = "Voice_Setting",                                  .size = 2,    .numb = 10 },
    },
    .numRet = 1, .Ret = (SpecField_t[1]) {
      { .name = "Status",                                         .size = 1,    },
    },
    .numEvents = 1, .Events = (SpecEvent_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

};

Feature_t Features[] = {
  { .name   = "LE Encryption",
    .used   = 5, // 0
    .enable = 1 },

  { .name   = "Connection Parameters Request procedure",
    .used   = 1, // 1
    .enable = 1 },

  { .name   = "LE Ping",
    .used   = 1, // 2
    .enable = 1 },

  { .name   = "LE Data Packet Length Extension",
    .used   = 1, // 3
    .enable = 1 },

  { .name   = "LL Privacy",
    .used   = 4, // 4
    .enable = 1 },

  { .name   = "LE 2M PHY",
    .used   = 3, // 5
    .enable = 1 },

  { .name   = "LE Coded PHY",
    .used   = 3, // 6
    .enable = 1 },

  { .name   = "Stable Modulation Index - Transmitter",
    .used   = 1, // 7
    .enable = 1 },

  { .name   = "Stable Modulation Index - Receiver",
    .used   = 1, // 8
    .enable = 1 },

  { .name   = "Periodic Advertising",
    .used   = 4, // 9
    .enable = 1 },

  { .name   = "Extended Advertising",
    .used   = 5, // 10
    .enable = 1 },

  { .name   = "LE Power Control Request",
    .used   = 3, // 11
    .enable = 1 },

  { .name   = "LE Channel Selection Algorithm #2",
    .used   = 1, // 12
    .enable = 1 },

  { .name   = "Connection CTE Request",
    .used   = 3, // 13
    .enable = 1 },

  { .name   = "Connection CTE Response",
    .used   = 3, // 14
    .enable = 1 },

  { .name   = "Connectionless CTE Transmitter",
    .used   = 3, // 15
    .enable = 1 },

  { .name   = "Connectionless CTE Receiver",
    .used   = 3, // 16
    .enable = 1 },

  { .name   = "Periodic Advertising Sync Transfer – Recipient",
    .used   = 2, // 17
    .enable = 1 },

  { .name   = "Periodic Advertising Sync Transfer – Sender",
    .used   = 2, // 18
    .enable = 1 },

  { .name   = "Channel Classification",
    .used   = 2, // 19
    .enable = 1 },

  { .name   = "Isochronous Broadcaster",
    .used   = 5, // 20
    .enable = 1 },

  { .name   = "Connected Isochronous Stream - Central",
    .used   = 9, // 21
    .enable = 1 },

  { .name   = "Connected Isochronous Stream - Peripheral",
    .used   = 9, // 22
    .enable = 1 },

  { .name   = "Synchronized Receiver role",
    .used   = 4, // 23
    .enable = 1 },

  { .name   = "Sleep Clock Accuracy Updates",
    .used   = 2, // 24
    .enable = 1 },

  { .name   = "Connection Subrating",
    .used   = 2, // 25
    .enable = 1 },

  { .name   = "LE Path Loss Monitoring",
    .used   = 1, // 26
    .enable = 1 },

  { .name   = "Synchronized Receiver",
    .used   = 1, // 27
    .enable = 1 },

  { .name   = "Connection Parameters Request Procedure",
    .used   = 1, // 28
    .enable = 1 },

};

SpecCond_t Conditions[] = {
  { .name   = "C1", .numBREDR = 0, .numLE = 2, .numFeatures = 0,
    .LE     = (uint32_t[2]) { 191, 204, },
    .notes  = "Mandatory if the LE Controller supports transmitting packets, "
              "otherwise excluded",
    .enable = 1  // Not feature controlled therefore enabled by default.
  },

  { .name   = "C2", .numBREDR = 0, .numLE = 1, .numFeatures = 0,
    .LE     = (uint32_t[1]) { 141, },
    .notes  = "Mandatory if the LE Controller supports receiving packets, otherwise "
              "excluded.",
    .enable = 1  // Not feature controlled therefore enabled by default.
  },

  { .name   = "C3", .numBREDR = 0, .numLE = 13, .numFeatures = 0,
    .LE     = (uint32_t[13]) { 22, 23, 72, 77, 118, 119, 133, 134, 218, 283, 284, 285, 292, },
    .notes  = "Mandatory if the LE Controller supports Connection State, otherwise "
              "excluded.",
    .enable = 1  // Not feature controlled therefore enabled by default.
  },

  { .name   = "C4", .numBREDR = 0, .numLE = 4, .numFeatures = 1,
    .LE     = (uint32_t[4]) { 25, 26, 89, 115, },
    .Features = (SpecFeat4Cond_t[1]) { { .mand = 1, .index =  0 }, },
    .notes  = "Mandatory if LE Feature (LE Encryption) is supported, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C6", .numBREDR = 0, .numLE = 3, .numFeatures = 1,
    .LE     = (uint32_t[3]) { 143, 144, 145, },
    .Features = (SpecFeat4Cond_t[1]) { { .mand = 1, .index =  1 }, },
    .notes  = "Mandatory if LE Feature (Connection Parameters Request procedure) is "
              "supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C7", .numBREDR = 0, .numLE = 1, .numFeatures = 2,
    .LE     = (uint32_t[1]) { 355, },
    .Features = (SpecFeat4Cond_t[2]) { { .mand = 1, .index =  0 }, { .mand = 1, .index =  2 }, },
    .notes  = "Mandatory if LE Feature (LE Encryption) and LE Feature (LE Ping) are "
              "supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C8", .numBREDR = 0, .numLE = 5, .numFeatures = 1,
    .LE     = (uint32_t[5]) { 86, 128, 138, 168, 206, },
    .Features = (SpecFeat4Cond_t[1]) { { .mand = 1, .index =  3 }, },
    .notes  = "Mandatory if LE Feature (LE Data Packet Length Extension) is "
              "supported, otherwise optional.",
    .enable = 0 
  },

  { .name   = "C9", .numBREDR = 0, .numLE = 7, .numFeatures = 1,
    .LE     = (uint32_t[7]) { 57, 71, 136, 150, 156, 190, 192, },
    .Features = (SpecFeat4Cond_t[1]) { { .mand = 1, .index =  4 }, },
    .notes  = "Mandatory if LE Feature (LL Privacy) is supported, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C10", .numBREDR = 0, .numLE = 3, .numFeatures = 1,
    .LE     = (uint32_t[3]) { 125, 130, 169, },
    .Features = (SpecFeat4Cond_t[1]) { { .mand = 0, .index =  4 }, },
    .notes  = "Optional if LE Feature (LL Privacy) is supported, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C11", .numBREDR = 0, .numLE = 4, .numFeatures = 2,
    .LE     = (uint32_t[4]) { 114, 132, 171, 189, },
    .Features = (SpecFeat4Cond_t[2]) { { .mand = 1, .index =  5 }, { .mand = 1, .index =  6 }, },
    .notes  = "Mandatory if LE Feature (LE 2M PHY) or LE Feature (LE Coded PHY) is "
              "supported, otherwise optional.",
    .enable = 0 
  },

  { .name   = "C12", .numBREDR = 0, .numLE = 1, .numFeatures = 3,
    .LE     = (uint32_t[1]) { 204, },
    .Features = (SpecFeat4Cond_t[3]) { { .mand = 1, .index =  5 }, { .mand = 1, .index =  6 }, { .mand = 1, .index =  7 }, },
    .notes  = "Mandatory if LE Feature (LE 2M PHY) or LE Feature (LE Coded PHY) or "
              "LE Feature (Stable Modulation Index - Transmitter) is supported, "
              "otherwise optional if the LE Controller supports transmitting "
              "packets, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C13", .numBREDR = 0, .numLE = 1, .numFeatures = 3,
    .LE     = (uint32_t[1]) { 141, },
    .Features = (SpecFeat4Cond_t[3]) { { .mand = 1, .index =  5 }, { .mand = 1, .index =  6 }, { .mand = 1, .index =  8 }, },
    .notes  = "Mandatory if LE Feature (LE 2M PHY) or LE Feature (LE Coded PHY) or "
              "LE Feature (Stable Modulation Index - Receiver) is supported, "
              "otherwise optional if the LE Controller supports receiving packets, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C15", .numBREDR = 0, .numLE = 1, .numFeatures = 0,
    .LE     = (uint32_t[1]) { 195, },
    .notes  = "Mandatory if LE Controller supports transmitting scannable "
              "advertisements, otherwise excluded.",
    .enable = 1  // Not feature controlled therefore enabled by default.
  },

  { .name   = "C16", .numBREDR = 0, .numLE = 3, .numFeatures = 1,
    .LE     = (uint32_t[3]) { 105, 106, 109, },
    .Features = (SpecFeat4Cond_t[1]) { { .mand = 1, .index =  9 }, },
    .notes  = "Mandatory if LE Feature (Periodic Advertising) is supported and the "
              "LE Controller supports both Scanning State and Synchronization State, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C17", .numBREDR = 0, .numLE = 11, .numFeatures = 1,
    .LE     = (uint32_t[11]) { 59, 68, 127, 129, 146, 154, 160, 174, 175, 176, 179, },
    .Features = (SpecFeat4Cond_t[1]) { { .mand = 1, .index = 10 }, },
    .notes  = "Mandatory if LE Feature (Extended Advertising) is supported and the "
              "LE Controller supports Advertising State, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C18", .numBREDR = 0, .numLE = 3, .numFeatures = 1,
    .LE     = (uint32_t[3]) { 184, 185, 186, },
    .Features = (SpecFeat4Cond_t[1]) { { .mand = 1, .index =  9 }, },
    .notes  = "Mandatory if LE Feature (Periodic Advertising) is supported and the "
              "LE Controller supports Advertising State, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C19", .numBREDR = 0, .numLE = 4, .numFeatures = 1,
    .LE     = (uint32_t[4]) { 92, 155, 177, 178, },
    .Features = (SpecFeat4Cond_t[1]) { { .mand = 1, .index = 10 }, },
    .notes  = "Mandatory if LE Feature (Extended Advertising) is supported and the "
              "LE Controller supports Scanning State, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C20", .numBREDR = 0, .numLE = 1, .numFeatures = 1,
    .LE     = (uint32_t[1]) { 93, },
    .Features = (SpecFeat4Cond_t[1]) { { .mand = 1, .index = 10 }, },
    .notes  = "Mandatory if LE Feature (Extended Advertising) is supported and the "
              "LE Controller supports Initiating State, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C21", .numBREDR = 0, .numLE = 7, .numFeatures = 1,
    .LE     = (uint32_t[7]) { 56, 70, 107, 110, 113, 131, 149, },
    .Features = (SpecFeat4Cond_t[1]) { { .mand = 1, .index =  9 }, },
    .notes  = "Mandatory if LE Feature (Periodic Advertising) is supported and the "
              "LE Controller supports Synchronization State, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C22", .numBREDR = 0, .numLE = 2, .numFeatures = 1,
    .LE     = (uint32_t[2]) { 137, 205, },
    .Features = (SpecFeat4Cond_t[1]) { { .mand = 1, .index = 11 }, },
    .notes  = "Mandatory if the LE Controller supports sending Transmit Power in "
              "advertisements or if LE Feature (LE Power Control Request) is "
              "supported, otherwise optional.",
    .enable = 0 
  },

  { .name   = "C23", .numBREDR = 0, .numLE = 1, .numFeatures = 1,
    .LE     = (uint32_t[1]) { 65, },
    .Features = (SpecFeat4Cond_t[1]) { { .mand = 1, .index = 12 }, },
    .notes  = "Mandatory if LE Feature (LE Channel Selection Algorithm #2) is "
              "supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C24", .numBREDR = 0, .numLE = 1, .numFeatures = 2,
    .LE     = (uint32_t[1]) { 90, },
    .Features = (SpecFeat4Cond_t[2]) { { .mand = 1, .index =  4 }, { .mand = 1, .index = 10 }, },
    .notes  = "Mandatory if the LE Controller supports Connection State and either "
              "LE Feature (LL Privacy) or LE Feature (Extended Advertising) is "
              "supported, otherwise optional if the LE Controller supports "
              "Connection State, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C25", .numBREDR = 0, .numLE = 4, .numFeatures = 1,
    .LE     = (uint32_t[4]) { 73, 75, 85, 163, },
    .Features = (SpecFeat4Cond_t[1]) { { .mand = 1, .index = 13 }, },
    .notes  = "Mandatory if LE Feature (Connection CTE Request) is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C26", .numBREDR = 0, .numLE = 2, .numFeatures = 1,
    .LE     = (uint32_t[2]) { 74, 164, },
    .Features = (SpecFeat4Cond_t[1]) { { .mand = 1, .index = 14 }, },
    .notes  = "Mandatory if LE Feature (Connection CTE Response) is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C27", .numBREDR = 0, .numLE = 2, .numFeatures = 1,
    .LE     = (uint32_t[2]) { 165, 166, },
    .Features = (SpecFeat4Cond_t[1]) { { .mand = 1, .index = 15 }, },
    .notes  = "Mandatory if LE Feature (Connectionless CTE Transmitter) is "
              "supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C28", .numBREDR = 0, .numLE = 2, .numFeatures = 1,
    .LE     = (uint32_t[2]) { 78, 167, },
    .Features = (SpecFeat4Cond_t[1]) { { .mand = 1, .index = 16 }, },
    .notes  = "Mandatory if LE Feature (Connectionless CTE Receiver) is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C29", .numBREDR = 0, .numLE = 1, .numFeatures = 2,
    .LE     = (uint32_t[1]) { 204, },
    .Features = (SpecFeat4Cond_t[2]) { { .mand = 1, .index = 14 }, { .mand = 1, .index = 15 }, },
    .notes  = "Mandatory if LE Feature (Connection CTE Response) or LE Feature "
              "(Connectionless CTE Transmitter) is supported, otherwise optional if "
              "the LE Controller supports transmitting packets, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C30", .numBREDR = 0, .numLE = 1, .numFeatures = 2,
    .LE     = (uint32_t[1]) { 141, },
    .Features = (SpecFeat4Cond_t[2]) { { .mand = 1, .index = 13 }, { .mand = 1, .index = 16 }, },
    .notes  = "Mandatory if LE Feature (Connection CTE Request) or LE Feature "
              "(Connectionless CTE Receiver) is supported, otherwise optional if the "
              "LE Controller supports receiving packets, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C31", .numBREDR = 0, .numLE = 1, .numFeatures = 4,
    .LE     = (uint32_t[1]) { 117, },
    .Features = (SpecFeat4Cond_t[4]) { { .mand = 1, .index = 13 }, { .mand = 1, .index = 14 }, { .mand = 1, .index = 15 }, { .mand = 1, .index = 16 }, },
    .notes  = "Mandatory if LE Feature (Connection CTE Request) or LE Feature "
              "(Connection CTE Response) or LE Feature (Connectionless CTE "
              "Transmitter) or LE Feature (Connectionless CTE Receiver) is "
              "supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C32", .numBREDR = 0, .numLE = 1, .numFeatures = 2,
    .LE     = (uint32_t[1]) { 187, },
    .Features = (SpecFeat4Cond_t[2]) { { .mand = 1, .index = 17 }, { .mand = 1, .index =  9 }, },
    .notes  = "Mandatory if LE Feature (Periodic Advertising Sync Transfer – "
              "Recipient) is supported, otherwise optional if LE Feature (Periodic "
              "Advertising) is supported and the LE Controller supports "
              "Synchronization State, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C33", .numBREDR = 0, .numLE = 1, .numFeatures = 1,
    .LE     = (uint32_t[1]) { 111, },
    .Features = (SpecFeat4Cond_t[1]) { { .mand = 1, .index = 18 }, },
    .notes  = "Mandatory if LE Feature (Periodic Advertising Sync Transfer – "
              "Sender) is supported and the LE Controller supports Scanning State, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C34", .numBREDR = 0, .numLE = 1, .numFeatures = 1,
    .LE     = (uint32_t[1]) { 108, },
    .Features = (SpecFeat4Cond_t[1]) { { .mand = 1, .index = 18 }, },
    .notes  = "Mandatory if LE Feature (Periodic Advertising Sync Transfer – "
              "Sender) is supported and the LE Controller supports Advertising "
              "State, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C35", .numBREDR = 0, .numLE = 3, .numFeatures = 1,
    .LE     = (uint32_t[3]) { 112, 170, 188, },
    .Features = (SpecFeat4Cond_t[1]) { { .mand = 1, .index = 17 }, },
    .notes  = "Mandatory if LE Feature (Periodic Advertising Sync Transfer – "
              "Recipient) is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C36", .numBREDR = 0, .numLE = 1, .numFeatures = 3,
    .LE     = (uint32_t[1]) { 180, },
    .Features = (SpecFeat4Cond_t[3]) { { .mand = 1, .index = 19 }, { .mand = 1, .index = 10 }, { .mand = 1, .index = 20 }, },
    .notes  = "Mandatory if the LE Controller supports Central role or supports both "
              "Peripheral role and LE Feature (Channel Classification), otherwise "
              "optional if LE Feature (Extended Advertising) is supported and the LE "
              "Controller supports Advertising State or if LE Feature (Isochronous "
              "Broadcaster) is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C37", .numBREDR = 0, .numLE = 1, .numFeatures = 0,
    .LE     = (uint32_t[1]) { 103, },
    .notes  = "Mandatory if the LE Controller can change its sleep clock accuracy, "
              "otherwise excluded.",
    .enable = 1  // Not feature controlled therefore enabled by default.
  },

  { .name   = "C38", .numBREDR = 0, .numLE = 1, .numFeatures = 2,
    .LE     = (uint32_t[1]) { 66, },
    .Features = (SpecFeat4Cond_t[2]) { { .mand = 1, .index = 21 }, { .mand = 1, .index = 22 }, },
    .notes  = "Mandatory if LE Feature (Connected Isochronous Stream - Central) or "
              "LE Feature (Connected Isochronous Stream - Peripheral) is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C39", .numBREDR = 0, .numLE = 4, .numFeatures = 1,
    .LE     = (uint32_t[4]) { 82, 147, 161, 162, },
    .Features = (SpecFeat4Cond_t[1]) { { .mand = 1, .index = 21 }, },
    .notes  = "Mandatory if LE Feature (Connected Isochronous Stream - Central) is "
              "supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C40", .numBREDR = 0, .numLE = 5, .numFeatures = 1,
    .LE     = (uint32_t[5]) { 54, 67, 142, 239, 359, },
    .Features = (SpecFeat4Cond_t[1]) { { .mand = 1, .index = 22 }, },
    .notes  = "Mandatory if LE Feature (Connected Isochronous Stream - Peripheral) "
              "is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C41", .numBREDR = 0, .numLE = 5, .numFeatures = 1,
    .LE     = (uint32_t[5]) { 79, 80, 81, 200, 201, },
    .Features = (SpecFeat4Cond_t[1]) { { .mand = 1, .index = 20 }, },
    .notes  = "Mandatory if LE Feature (Isochronous Broadcaster) is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C42", .numBREDR = 0, .numLE = 4, .numFeatures = 1,
    .LE     = (uint32_t[4]) { 60, 62, 63, 64, },
    .Features = (SpecFeat4Cond_t[1]) { { .mand = 1, .index = 23 }, },
    .notes  = "Mandatory if LE Feature (Synchronized Receiver role) is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C44", .numBREDR = 0, .numLE = 1, .numFeatures = 4,
    .LE     = (uint32_t[1]) { 152, },
    .Features = (SpecFeat4Cond_t[4]) { { .mand = 1, .index = 24 }, { .mand = 1, .index = 21 }, { .mand = 1, .index = 22 }, { .mand = 1, .index = 24 }, },
    .notes  = "Mandatory if LE Feature (Sleep Clock Accuracy Updates) and either LE "
              "Feature (Connected Isochronous Stream - Central) or LE Feature "
              "(Connected Isochronous Stream - Peripheral) are supported, otherwise "
              "optional if LE Feature (Sleep Clock Accuracy Updates) is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C45", .numBREDR = 0, .numLE = 2, .numFeatures = 3,
    .LE     = (uint32_t[2]) { 99, 122, },
    .Features = (SpecFeat4Cond_t[3]) { { .mand = 1, .index = 21 }, { .mand = 1, .index = 22 }, { .mand = 1, .index = 20 }, },
    .notes  = "Mandatory if LE Feature (Connected Isochronous Stream - Central), or "
              "LE Feature (Connected Isochronous Stream - Peripheral), or LE Feature "
              "(Isochronous Broadcaster) is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C46", .numBREDR = 0, .numLE = 2, .numFeatures = 3,
    .LE     = (uint32_t[2]) { 96, 97, },
    .Features = (SpecFeat4Cond_t[3]) { { .mand = 1, .index = 21 }, { .mand = 1, .index = 22 }, { .mand = 1, .index = 23 }, },
    .notes  = "Mandatory if LE Feature (Connected Isochronous Stream - Central), or "
              "LE Feature (Connected Isochronous Stream - Peripheral), or LE Feature "
              "(Synchronized Receiver role) is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C47", .numBREDR = 0, .numLE = 3, .numFeatures = 4,
    .LE     = (uint32_t[3]) { 98, 151, 197, },
    .Features = (SpecFeat4Cond_t[4]) { { .mand = 1, .index = 21 }, { .mand = 1, .index = 22 }, { .mand = 1, .index = 20 }, { .mand = 1, .index = 23 }, },
    .notes  = "Mandatory if LE Feature (Connected Isochronous Stream - Central), or "
              "LE Feature (Connected Isochronous Stream - Peripheral), or LE Feature "
              "(Isochronous Broadcaster), or LE Feature (Synchronized Receiver role) "
              "is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C49", .numBREDR = 0, .numLE = 1, .numFeatures = 3,
    .LE     = (uint32_t[1]) { 181, },
    .Features = (SpecFeat4Cond_t[3]) { { .mand = 1, .index = 21 }, { .mand = 1, .index = 22 }, { .mand = 1, .index = 25 }, },
    .notes  = "Mandatory if LE Feature (Connected Isochronous Stream - Central) or "
              "LE Feature (Connected Isochronous Stream - Peripheral) or LE Feature "
              "(Connection Subrating) is supported, otherwise optional.",
    .enable = 0 
  },

  { .name   = "C50", .numBREDR = 0, .numLE = 1, .numFeatures = 3,
    .LE     = (uint32_t[1]) { 121, },
    .Features = (SpecFeat4Cond_t[3]) { { .mand = 0, .index = 21 }, { .mand = 0, .index = 22 }, { .mand = 0, .index = 23 }, },
    .notes  = "Optional if LE Feature (Connected Isochronous Stream - Central), or "
              "LE Feature (Connected Isochronous Stream - Peripheral), or LE Feature "
              "(Synchronized Receiver role) is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C51", .numBREDR = 0, .numLE = 4, .numFeatures = 1,
    .LE     = (uint32_t[4]) { 91, 135, 196, 203, },
    .Features = (SpecFeat4Cond_t[1]) { { .mand = 1, .index = 11 }, },
    .notes  = "Mandatory if LE Feature (LE Power Control Request) is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C52", .numBREDR = 0, .numLE = 3, .numFeatures = 1,
    .LE     = (uint32_t[3]) { 104, 182, 183, },
    .Features = (SpecFeat4Cond_t[1]) { { .mand = 1, .index = 26 }, },
    .notes  = "Mandatory if LE Feature (LE Path Loss Monitoring) is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C53", .numBREDR = 0, .numLE = 1, .numFeatures = 1,
    .LE     = (uint32_t[1]) { 204, },
    .Features = (SpecFeat4Cond_t[1]) { { .mand = 1, .index = 11 }, },
    .notes  = "Mandatory if LE Feature (LE Power Control Request) is supported, "
              "otherwise optional if the LE Controller supports transmitting "
              "packets, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C54", .numBREDR = 0, .numLE = 1, .numFeatures = 4,
    .LE     = (uint32_t[1]) { 61, },
    .Features = (SpecFeat4Cond_t[4]) { { .mand = 1, .index = 27 }, { .mand = 1, .index = 21 }, { .mand = 1, .index = 22 }, { .mand = 1, .index = 20 }, },
    .notes  = "Mandatory if LE Feature (Synchronized Receiver) is supported, "
              "otherwise optional. C55:Mandatory if LE Feature (Connected "
              "Isochronous Stream - Central), or LE Feature (Connected Isochronous "
              "Stream - Peripheral), or LE Feature (Isochronous Broadcaster) is "
              "supported, otherwise optional if the LE Controller supports "
              "Connection State, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C56", .numBREDR = 0, .numLE = 1, .numFeatures = 1,
    .LE     = (uint32_t[1]) { 25, },
    .Features = (SpecFeat4Cond_t[1]) { { .mand = 0, .index =  0 }, },
    .notes  = "Optional if LE Feature (LE Encryption) is supported, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C57", .numBREDR = 0, .numLE = 3, .numFeatures = 1,
    .LE     = (uint32_t[3]) { 172, 198, 199, },
    .Features = (SpecFeat4Cond_t[1]) { { .mand = 1, .index = 25 }, },
    .notes  = "Mandatory if LE Feature (Connection Subrating) is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C58", .numBREDR = 0, .numLE = 2, .numFeatures = 1,
    .LE     = (uint32_t[2]) { 228, 354, },
    .Features = (SpecFeat4Cond_t[1]) { { .mand = 1, .index = 19 }, },
    .notes  = "Mandatory if LE Feature (Channel Classification) is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C59", .numBREDR = 0, .numLE = 1, .numFeatures = 0,
    .LE     = (uint32_t[1]) { 84, },
    .notes  = "Mandatory if the LE Controller supports Central role, otherwise "
              "excluded.",
    .enable = 1  // Not feature controlled therefore enabled by default.
  },

  { .name   = "C60", .numBREDR = 0, .numLE = 1, .numFeatures = 1,
    .LE     = (uint32_t[1]) { 88, },
    .Features = (SpecFeat4Cond_t[1]) { { .mand = 1, .index =  0 }, },
    .notes  = "Mandatory if the LE Controller supports Central role and LE Feature "
              "(LE Encryption), otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C61", .numBREDR = 0, .numLE = 3, .numFeatures = 1,
    .LE     = (uint32_t[3]) { 100, 101, 102, },
    .Features = (SpecFeat4Cond_t[1]) { { .mand = 1, .index =  0 }, },
    .notes  = "Mandatory if the LE Controller supports Peripheral role and LE "
              "Feature (LE Encryption), otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C62", .numBREDR = 0, .numLE = 1, .numFeatures = 1,
    .LE     = (uint32_t[1]) { 76, },
    .Features = (SpecFeat4Cond_t[1]) { { .mand = 1, .index = 28 }, },
    .notes  = "Mandatory if the LE Controller supports Central role or supports both "
              "Peripheral role and LE Feature (Connection Parameters Request "
              "Procedure), otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C63", .numBREDR = 0, .numLE = 1, .numFeatures = 1,
    .LE     = (uint32_t[1]) { 87, },
    .Features = (SpecFeat4Cond_t[1]) { { .mand = 1, .index =  4 }, },
    .notes  = "Mandatory if the LE Controller supports Scanning state and LE Feature "
              "(LL Privacy), otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C64", .numBREDR = 0, .numLE = 1, .numFeatures = 0,
    .LE     = (uint32_t[1]) { 140, },
    .notes  = "Optional if the Controller supports transmitting packets, otherwise "
              "excluded.",
    .enable = 1  // Not feature controlled therefore enabled by default.
  },

  { .name   = "C94", .numBREDR = 0, .numLE = 1, .numFeatures = 0,
    .LE     = (uint32_t[1]) { 83, },
    .notes  = "Mandatory if the LE Create Connection or LE Extended Create "
              "Connection command is supported, otherwise excluded.",
    .enable = 1  // Not feature controlled therefore enabled by default.
  },

  { .name   = "C95", .numBREDR = 0, .numLE = 1, .numFeatures = 0,
    .LE     = (uint32_t[1]) { 153, },
    .notes  = "Mandatory if the LE Request Peer SCA command is supported, otherwise "
              "excluded.",
    .enable = 1  // Not feature controlled therefore enabled by default.
  },

  { .name   = "C96", .numBREDR = 0, .numLE = 1, .numFeatures = 0,
    .LE     = (uint32_t[1]) { 318, },
    .notes  = "Optional if the LE Controller supports Connection State, otherwise "
              "excluded.",
    .enable = 1  // Not feature controlled therefore enabled by default.
  },

  { .name   = "C97", .numBREDR = 0, .numLE = 4, .numFeatures = 0,
    .LE     = (uint32_t[4]) { 116, 157, 158, 159, },
    .notes  = "Mandatory if Advertising State is supported, otherwise excluded.",
    .enable = 1  // Not feature controlled therefore enabled by default.
  },

  { .name   = "C98", .numBREDR = 0, .numLE = 3, .numFeatures = 0,
    .LE     = (uint32_t[3]) { 58, 193, 194, },
    .notes  = "Mandatory if Scanning State is supported, otherwise excluded.",
    .enable = 1  // Not feature controlled therefore enabled by default.
  },

  { .name   = "C99", .numBREDR = 0, .numLE = 1, .numFeatures = 0,
    .LE     = (uint32_t[1]) { 94, },
    .notes  = "Mandatory if LE Generate DHKey command [v2] is supported, otherwise "
              "optional.",
    .enable = 1  // Not feature controlled therefore enabled by default.
  },

  { .name   = "C101", .numBREDR = 1, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[1]) { 3, },
    .notes  = "Mandatory if the Authentication Requested command is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C102", .numBREDR = 1, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[1]) { 6, },
    .notes  = "Mandatory if the Change Connection Link Key command is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C103", .numBREDR = 1, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[1]) { 31, },
    .notes  = "Mandatory if the Periodic Inquiry Mode command is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C104", .numBREDR = 1, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[1]) { 238, },
    .notes  = "Mandatory if the Read Clock Offset command is supported, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C105", .numBREDR = 1, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[1]) { 284, },
    .notes  = "Mandatory if the Read Remote Version Information command is "
              "supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C106", .numBREDR = 3, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[3]) { 298, 299, 301, },
    .notes  = "Mandatory if the Remote Name Request command is supported, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C107", .numBREDR = 2, .numLE = 2, .numFeatures = 0,
    .BREDR = (uint32_t[2]) { 41, 42, },
    .LE     = (uint32_t[2]) { 41, 42, },
    .notes  = "Mandatory if the Set Controller To Host Flow Control command is "
              "supported, otherwise excluded.",
    .enable = 1  // Not feature controlled therefore enabled by default.
  },

  { .name   = "C108", .numBREDR = 1, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[1]) { 323, },
    .notes  = "Mandatory if the Set MWS_PATTERN Configuration command is supported, "
              "otherwise optional.",
    .enable = 0 
  },

  { .name   = "C109", .numBREDR = 2, .numLE = 2, .numFeatures = 0,
    .BREDR = (uint32_t[2]) { 38, 328, },
    .LE     = (uint32_t[2]) { 38, 328, },
    .notes  = "Mandatory if the Set MWS Signaling command is supported, otherwise "
              "excluded.",
    .enable = 1  // Not feature controlled therefore enabled by default.
  },

  { .name   = "C110", .numBREDR = 1, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[1]) { 343, },
    .notes  = "Mandatory if the Set Triggered Clock Capture command is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C111", .numBREDR = 1, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[1]) { 231, },
    .notes  = "Mandatory if the Write Authentication Enable command is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C112", .numBREDR = 1, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[1]) { 242, },
    .notes  = "Mandatory if the Write Default Erroneous Data Reporting command is "
              "supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C113", .numBREDR = 1, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[1]) { 246, },
    .notes  = "Mandatory if the Write Extended Inquiry Length command is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C114", .numBREDR = 1, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[1]) { 248, },
    .notes  = "Mandatory if the Write Extended Page Timeout command is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C115", .numBREDR = 1, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[1]) { 252, },
    .notes  = "Mandatory if the Write Inquiry Mode command is supported, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C116", .numBREDR = 1, .numLE = 1, .numFeatures = 0,
    .BREDR = (uint32_t[1]) { 256, },
    .LE     = (uint32_t[1]) { 256, },
    .notes  = "Mandatory if the Write LE Host Support command is supported, "
              "otherwise excluded.",
    .enable = 1  // Not feature controlled therefore enabled by default.
  },

  { .name   = "C117", .numBREDR = 1, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[1]) { 259, },
    .notes  = "Mandatory if the Write Link Supervision Timeout command is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C118", .numBREDR = 1, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[1]) { 273, },
    .notes  = "Mandatory if the Write Num Broadcast Retransmissions command is "
              "supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C119", .numBREDR = 1, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[1]) { 276, },
    .notes  = "Mandatory if the Write Page Scan Type command is supported, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C120", .numBREDR = 1, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[1]) { 278, },
    .notes  = "Mandatory if the Write PIN Type command is supported, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C121", .numBREDR = 3, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[3]) { 21, 289, 308, },
    .notes  = "Mandatory if the Write Stored Link Key command is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C122", .numBREDR = 1, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[1]) { 291, },
    .notes  = "Mandatory if the Write Synchronous Flow Control Enable command is "
              "supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C123", .numBREDR = 4, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[4]) { 24, 214, 272, 376, },
    .notes  = "Mandatory if BR/EDR test mode is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C124", .numBREDR = 4, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[4]) { 217, 241, 250, 366, },
    .notes  = "Mandatory if Data block based flow control is supported, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C125", .numBREDR = 8, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[8]) { 240, 253, 254, 255, 274, 360, 369, 370, },
    .notes  = "Mandatory if Inquiry Scan is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C126", .numBREDR = 1, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[1]) { 46, },
    .notes  = "Optional if Inquiry Scan is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C127", .numBREDR = 5, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[5]) { 43, 44, 45, 47, 371, },
    .notes  = "Mandatory if Inquiry is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C128", .numBREDR = 3, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[3]) { 48, 220, 363, },
    .notes  = "Optional if Inquiry is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C129", .numBREDR = 3, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[3]) { 344, 345, 346, },
    .notes  = "Mandatory if Truncated page state is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C132", .numBREDR = 1, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[1]) { 215, },
    .notes  = "Mandatory if multi-slot ACL packets are is supported, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C133", .numBREDR = 2, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[2]) { 7, 12, },
    .notes  = "Mandatory if HV2, HV3, or multi-slot or EDR ACL packets are is "
              "supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C134", .numBREDR = 8, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[8]) { 1, 260, 293, 297, 332, 341, 342, 390, },
    .notes  = "Mandatory if SCO or eSCO is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C135", .numBREDR = 4, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[4]) { 27, 30, 361, 389, },
    .notes  = "Optional if SCO or eSCO is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C136", .numBREDR = 1, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[1]) { 329, },
    .notes  = "Optional if Slot Availability Mask is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C138", .numBREDR = 1, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[1]) { 384, },
    .notes  = "Mandatory if Secure Connections (Controller) is supported, otherwise "
              "optional if eSCO is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C139", .numBREDR = 1, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[1]) { 229, },
    .notes  = "Mandatory if the Controller is AFH capable in either role, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C140", .numBREDR = 3, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[3]) { 228, 313, 354, },
    .notes  = "Mandatory if the Controller supports AFH classification in either "
              "role or is an AFH capable Central, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C141", .numBREDR = 4, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[4]) { 243, 257, 362, 373, },
    .notes  = "Mandatory if Role Switch, Hold mode, or Sniff mode is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C142", .numBREDR = 1, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[1]) { 264, },
    .notes  = "Mandatory if Secure Connections (Host) is supported, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C143", .numBREDR = 1, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[1]) { 305, },
    .notes  = "Mandatory if Secure Connections (both Host and Controller) is "
              "supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C144", .numBREDR = 1, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[1]) { 216, },
    .notes  = "Mandatory if Hold Mode or Sniff Mode is supported, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C145", .numBREDR = 1, .numLE = 1, .numFeatures = 0,
    .BREDR = (uint32_t[1]) { 322, },
    .LE     = (uint32_t[1]) { 322, },
    .notes  = "Mandatory if any event in event mask page 2 is supported, otherwise "
              "optional.",
    .enable = 1  // Not feature controlled therefore enabled by default.
  },

  { .name   = "C146", .numBREDR = 1, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[1]) { 368, },
    .notes  = "Mandatory if the Extended Inquiry Result event or the IO Capability "
              "Request event is supported, otherwise optional if Inquiry is "
              "supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C147", .numBREDR = 1, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[1]) { 33, },
    .notes  = "Optional if the Inquiry Result with RSSI event is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C148", .numBREDR = 1, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[1]) { 320, },
    .notes  = "Optional if any of the Connection Complete, Connection Request, "
              "Extended Inquiry Result, Inquiry Result with RSSI, IO Capability "
              "Request, or Synchronous Connection Complete events is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C151", .numBREDR = 1, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[1]) { 355, },
    .notes  = "Mandatory if Secure Connections (Controller) and Ping are supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C152", .numBREDR = 1, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[1]) { 292, },
    .notes  = "Mandatory if Power Control is supported, otherwise optional.",
    .enable = 0 
  },

  { .name   = "C153", .numBREDR = 1, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[1]) { 372, },
    .notes  = "Mandatory if LE supported in the Controller, otherwise optional.",
    .enable = 0 
  },

  { .name   = "C154", .numBREDR = 1, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[1]) { 379, },
    .notes  = "Mandatory if Interlaced Page Scan is supported, otherwise optional.",
    .enable = 0 
  },

  { .name   = "C155", .numBREDR = 2, .numLE = 2, .numFeatures = 0,
    .BREDR = (uint32_t[2]) { 2, 230, },
    .LE     = (uint32_t[2]) { 2, 230, },
    .notes  = "Mandatory if the Write Authenticated Payload Timeout command is "
              "supported, otherwise excluded.",
    .enable = 1  // Not feature controlled therefore enabled by default.
  },

  { .name   = "C156", .numBREDR = 3, .numLE = 3, .numFeatures = 0,
    .BREDR = (uint32_t[3]) { 10, 266, 269, },
    .LE     = (uint32_t[3]) { 10, 266, 269, },
    .notes  = "Mandatory if the Read Local Supported Codecs command [v2] is "
              "supported, otherwise excluded.",
    .enable = 1  // Not feature controlled therefore enabled by default.
  },

  { .name   = "C157", .numBREDR = 1, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[1]) { 267, },
    .notes  = "Mandatory if the Read Local Supported Codecs command [v2] is "
              "supported, otherwise optional.",
    .enable = 0 
  },

  { .name   = "C158", .numBREDR = 1, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[1]) { 25, },
    .notes  = "Mandatory if the Set Min Encryption Key Size command is supported, "
              "otherwise optional.",
    .enable = 0 
  },

  { .name   = "C201", .numBREDR = 5, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[5]) { 14, 20, 315, 316, 330, },
    .notes  = "Mandatory if Connectionless Peripheral Broadcast - Transmitter is "
              "supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C202", .numBREDR = 3, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[3]) { 15, 16, 317, },
    .notes  = "Mandatory if Connectionless Peripheral Broadcast - Receiver is "
              "supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C203", .numBREDR = 4, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[4]) { 290, 337, 339, 388, },
    .notes  = "Mandatory if Synchronization Train is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C204", .numBREDR = 2, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[2]) { 294, 340, },
    .notes  = "Mandatory if Synchronization Scan is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C205", .numBREDR = 2, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[2]) { 247, 364, },
    .notes  = "Mandatory if Extended Inquiry Response is supported, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C212", .numBREDR = 2, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[2]) { 309, 338, },
    .notes  = "Mandatory if Role Switch is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C213", .numBREDR = 3, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[3]) { 40, 251, 367, },
    .notes  = "Mandatory if Hold mode is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C214", .numBREDR = 2, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[2]) { 32, 334, },
    .notes  = "Mandatory if Sniff mode is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C215", .numBREDR = 2, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[2]) { 211, 212, },
    .notes  = "Mandatory if Broadcast Encryption is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C217", .numBREDR = 1, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[1]) { 245, },
    .notes  = "Mandatory if BR/EDR Enhanced Power Control is supported, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C218", .numBREDR = 2, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[2]) { 287, 383, },
    .notes  = "Mandatory if Secure Connections (Controller) is supported, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C219", .numBREDR = 1, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[1]) { 311, },
    .notes  = "Mandatory if Slot Availability Mask is supported, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C220", .numBREDR = 3, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[3]) { 261, 279, 280, },
    .notes  = "Mandatory if LMP Extended Features mask is supported, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C221", .numBREDR = 2, .numLE = 0, .numFeatures = 0,
    .BREDR = (uint32_t[2]) { 335, 336, },
    .notes  = "Mandatory if Sniff subrating is supported, otherwise excluded.",
    .enable = 0 
  },

};

#endif // INCLUDE_SPECS
#endif // SPECS2SRC_H
