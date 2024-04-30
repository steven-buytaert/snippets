#ifndef BT_HCI_SPEC_H
#define BT_HCI_SPEC_H

// Copyright 2024 Steven Buytaert

#include <stdint.h>

typedef struct HCI_Feature_t {
  const char *      name;
  uint32_t          used;         // Number of times it is used by a condition.
  uint32_t          enable;       // Set to non zero to enable.
#if defined(EXTRA4FEAT)
  EXTRA4FEAT                      // Define extra members if needed.
#endif
} HCI_Feature_t;

typedef struct HCI_Event_t {
  const char *      name;
#if defined(EXTRA4EVENT)
  EXTRA4EVENT                     // Define extra members if needed.
#endif
} HCI_Event_t;

typedef struct HCI_Feat4Cond_t {  // Couples a feature to a condition.
  uint32_t          mand;         // 1 ==  Mandatory, 0 == Optional.
  uint32_t          index;        // Index in Features table.
} HCI_Feat4Cond_t;

typedef struct HCI_Cond_t {
  const char *      name;
  const char *      notes;
  const uint32_t    numBREDR;
  const uint32_t    numLE;
  const uint32_t    numFeatures;
  const uint32_t *  BREDR;
  const uint32_t *  LE;
  HCI_Feat4Cond_t * Features;
  uint32_t          enable;       // Set to non zero to enable.
#if defined(EXTRA4COND)
  EXTRA4COND                      // Define extra members if needed.
#endif
} HCI_Cond_t;

typedef struct HCI_Field_t {
  const char *      name;
  const char *      ref2size;
  uint8_t           octets;
#if defined(EXTRA4FIELD)
  EXTRA4FIELD                     // Define extra members if needed.
#endif
} HCI_Field_t;

typedef struct HCI_Inst_t {       // HCI Command or Event instance; at least 1 for each HCI.
  const char *      name;         // Command or event name.
  union {
    uint16_t        OCF;          // For a command.
    uint16_t        code;         // For an event.
  };
  uint8_t           numcoe;       // Number of command or event parameters.
  uint8_t           numret;       // Number of return parameters.
  uint8_t           bit;          // Bit number and ...
  uint8_t           octet;        // ... octet number as found in '6.27 SUPPORTED COMMANDS'.
  struct {
    const char *    BR;
    const char *    LE;
  } Cond;
  HCI_Field_t *     CoE;          // Command or Event parameter fields.
  HCI_Field_t *     Ret;          // Return parameter fields, if applicable.
#if defined(EXTRA4INST)
  EXTRA4INST                      // Define extra members if needed.
#endif
} HCI_Inst_t;

typedef struct HCI_t {            // HCI Command or Event.
  uint8_t           iscmd;        // Non zero for a command, zero for an event.
  uint8_t           OGF;          // OGF; when set implies iscmd non zero.
  uint8_t           subevent;     // Subevent code (only non zero for meta events).
  uint8_t           numinst;      // Number of instances.
  HCI_Inst_t *      Inst;
  const char *      section;      // Section number.
  const char *      title;        // Section title.
  const char *      summary;      // Summary as found in '3 OVERVIEW OF COMMANDS AND EVENTS'.
  const char *      version;
  const char *      BREDR;
  const char *      LE;
  uint32_t          numevts;
  HCI_Event_t *     Events;       // Events generated.
#if defined(EXTRA4HCI)
  EXTRA4HCI                       // Define extra members if needed.
#endif
} HCI_t;

extern HCI_t HCITable[396];

#ifdef INCLUDE_SPECS

HCI_t HCITable[] = {
  { .title = "Accept Connection Request command", .iscmd = 1, .OGF = 0x01, .section = "7.1.8", .version = "1.1", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Accept_Connection_Request command is used to accept a new "
               "incoming BR/EDR connection request.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Accept_Connection_Request", .OCF = 0x0009, .numcoe = 2, .numret = 0, .octet =  1, .bit = 0, .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "BD_ADDR",                                        .octets =   6 },
          { .name = "Role",                                           .octets =   1 },
        },
      },
    },
    .numevts = 3, .Events = (HCI_Event_t[3]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Connection_Complete" },
      { .name = "HCI_Synchronous_Connection_Complete" },
    },
  },

  { .title = "Accept Synchronous Connection Request command", .iscmd = 1, .OGF = 0x01, .section = "7.1.27", .version = "1.2", .BREDR = "C.134", .LE = "E",
    .summary = "The HCI_Accept_Synchronous_Connection_Request command is used to "
               "accept an incoming request for a synchronous connection and to inform "
               "the local Link Manager about the acceptable parameter values for the "
               "synchronous connection.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Accept_Synchronous_Connection_Request", .OCF = 0x0029, .numcoe = 7, .numret = 0, .octet = 16, .bit = 4, .Cond = { "C.134", "E" },
        .CoE = (HCI_Field_t[7]) {
          { .name = "BD_ADDR",                                        .octets =   6 },
          { .name = "Transmit_Bandwidth",                             .octets =   4 },
          { .name = "Receive_Bandwidth",                              .octets =   4 },
          { .name = "Max_Latency",                                    .octets =   2 },
          { .name = "Voice_Setting",                                  .octets =   2 },
          { .name = "Retransmission_Effort",                          .octets =   1 },
          { .name = "Packet_Type",                                    .octets =   2 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Synchronous_Connection_Complete" },
    },
  },

  { .title = "Authenticated Payload Timeout Expired event", .iscmd = 0, .section = "7.7.75", .version = "4.1", .BREDR = "C.155", .LE = "C.155",
    .summary = "The HCI_Authenticated_Payload_Timeout_Expired event is used to "
               "indicate that a packet containing a valid MIC on the Handle was not "
               "received within the authenticatedPayloadTO.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Authenticated_Payload_Timeout_Expired", .code = 0x0057, .numcoe = 1, .numret = 0,  .Cond = { "C.155", "C.155" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Connection_Handle",                              .octets =   2 },
        },
      },
    },
  },

  { .title = "Authentication Complete event", .iscmd = 0, .section = "7.7.6", .version = "1.1", .BREDR = "C.101", .LE = "E",
    .summary = "The HCI_Authentication_Complete event occurs when authentication has "
               "been completed for the specified connection.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Authentication_Complete", .code = 0x0006, .numcoe = 2, .numret = 0,  .Cond = { "C.101", "E" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
        },
      },
    },
  },

  { .title = "Authentication Requested command", .iscmd = 1, .OGF = 0x01, .section = "7.1.15", .version = "1.1", .BREDR = "O", .LE = "E",
    .summary = "The HCI_Authentication_Requested command is used to establish "
               "authentication between the two devices associated with the specified "
               "Connection_Handle.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Authentication_Requested", .OCF = 0x0011, .numcoe = 1, .numret = 0, .octet =  1, .bit = 7, .Cond = { "O", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Connection_Handle",                              .octets =   2 },
        },
      },
    },
    .numevts = 4, .Events = (HCI_Event_t[4]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Link_Key_Request" },
      { .name = "HCI_PIN_Code_Request" },
      { .name = "HCI_Authentication_Complete" },
    },
  },

  { .title = "Change Connection Link Key command", .iscmd = 1, .OGF = 0x01, .section = "7.1.17", .version = "1.1", .BREDR = "O", .LE = "E",
    .summary = "The HCI_Change_Connection_Link_Key command is used to force both "
               "devices of a connection associated to the Connection_Handle, to "
               "generate a new link key.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Change_Connection_Link_Key", .OCF = 0x0015, .numcoe = 1, .numret = 0, .octet =  2, .bit = 1, .Cond = { "O", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Connection_Handle",                              .octets =   2 },
        },
      },
    },
    .numevts = 3, .Events = (HCI_Event_t[3]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Link_Key_Notification" },
      { .name = "HCI_Change_Connection_Link_Key_Complete" },
    },
  },

  { .title = "Change Connection Link Key Complete event", .iscmd = 0, .section = "7.7.9", .version = "1.1", .BREDR = "C.102", .LE = "E",
    .summary = "The HCI_Change_Connection_Link_Key_Complete event is used to indicate "
               "that the change in the Link Key for the Connection_Handle specified "
               "by the Connection_Handle event parameter had been completed.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Change_Connection_Link_Key_Complete", .code = 0x0009, .numcoe = 2, .numret = 0,  .Cond = { "C.102", "E" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
        },
      },
    },
  },

  { .title = "Change Connection Packet Type command", .iscmd = 1, .OGF = 0x01, .section = "7.1.14", .version = "1.1", .BREDR = "C.133", .LE = "E",
    .summary = "The HCI_Change_Connection_Packet_Type command is used to change which "
               "packet types can be used for a connection that is currently "
               "established.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Change_Connection_Packet_Type", .OCF = 0x000f, .numcoe = 2, .numret = 0, .octet =  1, .bit = 6, .Cond = { "C.133", "E" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Packet_Type",                                    .octets =   2 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Connection_Packet_Type_Changed" },
    },
  },

  { .title = "Command Complete event", .iscmd = 0, .section = "7.7.14", .version = "1.1", .BREDR = "M", .LE = "M",
    .summary = "The HCI_Command_Complete event is used by the Controller to pass the "
               "return status of a command and the other event parameters for each "
               "HCI command.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Command_Complete", .code = 0x000e, .numcoe = 3, .numret = 0,  .Cond = { "M", "M" },
        .CoE = (HCI_Field_t[3]) {
          { .name = "Num_HCI_Command_Packets",                        .octets =   1 },
          { .name = "Command_Opcode",                                 .octets =   2 },
          { .name = "Return_Parameters",                                            },
        },
      },
    },
  },

  { .title = "Command Status event", .iscmd = 0, .section = "7.7.15", .version = "1.1", .BREDR = "M", .LE = "M",
    .summary = "The HCI_Command_Status event is used to indicate that the command "
               "described by the Command_Opcode parameter has been received and the "
               "Controller is currently performing the task for this command.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Command_Status", .code = 0x000f, .numcoe = 3, .numret = 0,  .Cond = { "M", "M" },
        .CoE = (HCI_Field_t[3]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Num_HCI_Command_Packets",                        .octets =   1 },
          { .name = "Command_Opcode",                                 .octets =   2 },
        },
      },
    },
  },

  { .title = "Configure Data Path command", .iscmd = 1, .OGF = 0x03, .section = "7.3.101", .version = "5.2", .BREDR = "C.156", .LE = "C.156",
    .summary = "The HCI_Configure_Data_Path command is used by a Host to configure a "
               "data path to enable codec operation in the Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Configure_Data_Path", .OCF = 0x0083, .numcoe = 4, .numret = 1, .octet = 45, .bit = 5, .Cond = { "C.156", "C.156" },
        .CoE = (HCI_Field_t[4]) {
          { .name = "Data_Path_Direction",                            .octets =   1 },
          { .name = "Data_Path_ID",                                   .octets =   1 },
          { .name = "Vendor_Specific_Config_Length",                  .octets =   1 },
          { .name = "Vendor_Specific_Config",                                       .ref2size = "Vendor_Specific_Config_Length" },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Connection Complete event", .iscmd = 0, .section = "7.7.3", .version = "1.1", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Connection_Complete event indicates to both of the Hosts "
               "forming the connection that a new BR/EDR connection has been "
               "established.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Connection_Complete", .code = 0x0003, .numcoe = 5, .numret = 0,  .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[5]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "BD_ADDR",                                        .octets =   6 },
          { .name = "Link_Type",                                      .octets =   1 },
          { .name = "Encryption_Enabled",                             .octets =   1 },
        },
      },
    },
  },

  { .title = "Connection Packet Type Changed event", .iscmd = 0, .section = "7.7.29", .version = "1.1", .BREDR = "C.133", .LE = "E",
    .summary = "The HCI_Connection_Packet_Type_Changed event is used to indicate the "
               "completion of the process of the Link Manager changing the packet "
               "type mask used for the specified Connection_Handle.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Connection_Packet_Type_Changed", .code = 0x001d, .numcoe = 3, .numret = 0,  .Cond = { "C.133", "E" },
        .CoE = (HCI_Field_t[3]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Packet_Type",                                    .octets =   2 },
        },
      },
    },
  },

  { .title = "Connection Request event", .iscmd = 0, .section = "7.7.4", .version = "1.1", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Connection_Request event is used to indicate that a new "
               "incoming BR/EDR connection is trying to be established.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Connection_Request", .code = 0x0004, .numcoe = 3, .numret = 0,  .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[3]) {
          { .name = "BD_ADDR",                                        .octets =   6 },
          { .name = "Class_Of_Device",                                .octets =   3 },
          { .name = "Link_Type",                                      .octets =   1 },
        },
      },
    },
  },

  { .title = "Connectionless Peripheral Broadcast Channel Map Change event", .iscmd = 0, .section = "7.7.73", .version = "CSA4", .BREDR = "C.201", .LE = "E",
    .summary = "The HCI_Connectionless_Peripheral_Broadcast_Channel_Map_Change event "
               "indicates to the Host that the BR/EDR Controller has moved to a new "
               "AFH channel map for the PBD logical link.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Connectionless_Peripheral_Broadcast_Channel_Map_Change", .code = 0x0055, .numcoe = 1, .numret = 0,  .Cond = { "C.201", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Channel_Map",                                    .octets =  10 },
        },
      },
    },
  },

  { .title = "Connectionless Peripheral Broadcast Receive event", .iscmd = 0, .section = "7.7.69", .version = "CSA4", .BREDR = "C.202", .LE = "E",
    .summary = "The HCI_Connectionless_Peripheral_Broadcast_Receive event provides "
               "the Host with the data received from a Connectionless Peripheral "
               "Broadcast packet.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Connectionless_Peripheral_Broadcast_Receive", .code = 0x0051, .numcoe = 8, .numret = 0,  .Cond = { "C.202", "E" },
        .CoE = (HCI_Field_t[8]) {
          { .name = "BD_ADDR",                                        .octets =   6 },
          { .name = "LT_ADDR",                                        .octets =   1 },
          { .name = "Clock",                                          .octets =   4 },
          { .name = "Offset",                                         .octets =   4 },
          { .name = "RX_Status",                                      .octets =   1 },
          { .name = "Fragment",                                       .octets =   1 },
          { .name = "Data_Length",                                    .octets =   1 },
          { .name = "Data",                                                         .ref2size = "Data_Length" },
        },
      },
    },
  },

  { .title = "Connectionless Peripheral Broadcast Timeout event", .iscmd = 0, .section = "7.7.70", .version = "CSA4", .BREDR = "C.202", .LE = "E",
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

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Connectionless_Peripheral_Broadcast_Timeout", .code = 0x0052, .numcoe = 2, .numret = 0,  .Cond = { "C.202", "E" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "BD_ADDR",                                        .octets =   6 },
          { .name = "LT_ADDR",                                        .octets =   1 },
        },
      },
    },
  },

  { .title = "Create Connection Cancel command", .iscmd = 1, .OGF = 0x01, .section = "7.1.7", .version = "1.2", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Create_Connection_Cancel command is used to cancel an ongoing "
               "Create Connection.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Create_Connection_Cancel", .OCF = 0x0008, .numcoe = 1, .numret = 2, .octet =  0, .bit = 7, .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "BD_ADDR",                                        .octets =   6 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "BD_ADDR",                                        .octets =   6 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_Connection_Complete" },
    },
  },

  { .title = "Create Connection command", .iscmd = 1, .OGF = 0x01, .section = "7.1.5", .version = "1.1", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Create_Connection command will cause the BR/EDR Link Manager "
               "to create an ACL connection to the BR/EDR Controller with the BD_ADDR "
               "specified by the command parameters.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Create_Connection", .OCF = 0x0005, .numcoe = 6, .numret = 0, .octet =  0, .bit = 4, .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[6]) {
          { .name = "BD_ADDR",                                        .octets =   6 },
          { .name = "Packet_Type",                                    .octets =   2 },
          { .name = "Page_Scan_Repetition_Mode",                      .octets =   1 },
          { .name = "Reserved",                                       .octets =   1 },
          { .name = "Clock_Offset",                                   .octets =   2 },
          { .name = "Allow_Role_Switch",                              .octets =   1 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Connection_Complete" },
    },
  },

  { .title = "Data Buffer Overflow event", .iscmd = 0, .section = "7.7.26", .version = "1.1", .BREDR = "O", .LE = "O",
    .summary = "The HCI_Data_Buffer_Overflow event is used to indicate that the "
               "Controller's data buffers have overflowed, because the Host has sent "
               "more packets than allowed.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Data_Buffer_Overflow", .code = 0x001a, .numcoe = 1, .numret = 0,  .Cond = { "O", "O" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Link_Type",                                      .octets =   1 },
        },
      },
    },
  },

  { .title = "Delete Reserved LT_ADDR command", .iscmd = 1, .OGF = 0x03, .section = "7.3.87", .version = "CSA4", .BREDR = "C.201", .LE = "E",
    .summary = "The HCI_Delete_Reserved_LT_ADDR command requests that the BR/EDR "
               "Controller cancel the reservation of a specific LT_ADDR reserved for "
               "the purposes of Connectionless Peripheral Broadcast.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Delete_Reserved_LT_ADDR", .OCF = 0x0075, .numcoe = 1, .numret = 2, .octet = 31, .bit = 5, .Cond = { "C.201", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "LT_ADDR",                                        .octets =   1 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "LT_ADDR",                                        .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Delete Stored Link Key command", .iscmd = 1, .OGF = 0x03, .section = "7.3.10", .version = "1.1", .BREDR = "C.121", .LE = "E",
    .summary = "The HCI_Delete_Stored_Link_Key command provides the ability to remove "
               "one or more of the link keys stored in the Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Delete_Stored_Link_Key", .OCF = 0x0012, .numcoe = 2, .numret = 2, .octet =  6, .bit = 7, .Cond = { "C.121", "E" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "BD_ADDR",                                        .octets =   6 },
          { .name = "Delete_All",                                     .octets =   1 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Num_Keys_Deleted",                               .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Disconnect command", .iscmd = 1, .OGF = 0x01, .section = "7.1.6", .version = "1.1", .BREDR = "M", .LE = "C.3",
    .summary = "The HCI_Disconnect command is used to terminate an existing BR/ EDR "
               "or LE connection.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Disconnect", .OCF = 0x0006, .numcoe = 2, .numret = 0, .octet =  0, .bit = 5, .Cond = { "M", "C.3" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Reason",                                         .octets =   1 },
        },
      },
    },
    .numevts = 3, .Events = (HCI_Event_t[3]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Disconnection_Complete" },
      { .name = "HCI_LE_CIS_Established" },
    },
  },

  { .title = "Disconnection Complete event", .iscmd = 0, .section = "7.7.5", .version = "1.1", .BREDR = "M", .LE = "C.3",
    .summary = "The HCI_Disconnection_Complete event occurs when a connection has "
               "been terminated.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Disconnection_Complete", .code = 0x0005, .numcoe = 3, .numret = 0,  .Cond = { "M", "C.3" },
        .CoE = (HCI_Field_t[3]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Reason",                                         .octets =   1 },
        },
      },
    },
  },

  { .title = "Enable Device Under Test Mode command", .iscmd = 1, .OGF = 0x06, .section = "7.6.3", .version = "1.1", .BREDR = "C.123", .LE = "E",
    .summary = "The HCI_Enable_Device_Under_Test_Mode command will allow the local "
               "Controller to enter test mode via LMP test commands. The Host issues "
               "this command when it wants the local device to be the DUT for the "
               "Testing scenarios as described in the Bluetooth Test Mode document.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Enable_Device_Under_Test_Mode", .OCF = 0x0003, .numcoe = 0, .numret = 1, .octet = 16, .bit = 2, .Cond = { "C.123", "E" },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Encryption Change event", .iscmd = 0, .section = "7.7.8", .version = "1.1 5.3", .BREDR = "[v1] M [v2] C.158", .LE = "[v1] C.4 [v2] C.56",
    .summary = "The HCI_Encryption_Change event is used to indicate that the change "
               "in encryption has been completed for the specified "
               "Connection_Handle.",

    .numinst = 2, .Inst = (HCI_Inst_t[2]) {
      { .name = "HCI_Encryption_Change [v2]", .code = 0x0059, .numcoe = 4, .numret = 0,  .Cond = { "C.158", "C.56" },
        .CoE = (HCI_Field_t[4]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Encryption_Enabled",                             .octets =   1 },
          { .name = "Encryption_Key_Size",                            .octets =   1 },
        },
      },
      { .name = "HCI_Encryption_Change [v1]", .code = 0x0008, .numcoe = 3, .numret = 0,  .Cond = { "M", "C.4" },
        .CoE = (HCI_Field_t[3]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Encryption_Enabled",                             .octets =   1 },
        },
      },
    },
  },

  { .title = "Encryption Key Refresh Complete event", .iscmd = 0, .section = "7.7.39", .version = "2.1 + EDR", .BREDR = "M", .LE = "C.4",
    .summary = "The HCI_Encryption_Key_Refresh_Complete event is used to indicate to "
               "the Host that the encryption key was refreshed on the given "
               "Connection_Handle any time encryption is paused and then resumed.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Encryption_Key_Refresh_Complete", .code = 0x0030, .numcoe = 2, .numret = 0,  .Cond = { "M", "C.4" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
        },
      },
    },
  },

  { .title = "Enhanced Accept Synchronous Connection Request command", .iscmd = 1, .OGF = 0x01, .section = "7.1.46", .version = "CSA2", .BREDR = "C.135", .LE = "E",
    .summary = "The HCI_Enhanced_Accept_Synchronous_Connection_Request command is "
               "used to accept an incoming request for a synchronous connection and "
               "to inform the local Link Manager about the acceptable parameter "
               "values for the synchronous connection.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Enhanced_Accept_Synchronous_Connection_Request", .OCF = 0x003e, .numcoe = 24, .numret = 0, .octet = 29, .bit = 4, .Cond = { "C.135", "E" },
        .CoE = (HCI_Field_t[24]) {
          { .name = "BD_ADDR",                                        .octets =   6 },
          { .name = "Transmit_Bandwidth",                             .octets =   4 },
          { .name = "Receive_Bandwidth",                              .octets =   4 },
          { .name = "Transmit_Coding_Format",                         .octets =   5 },
          { .name = "Receive_Coding_Format",                          .octets =   5 },
          { .name = "Transmit_Codec_Frame_Size",                      .octets =   2 },
          { .name = "Receive_Codec_Frame_Size",                       .octets =   2 },
          { .name = "Input_Bandwidth",                                .octets =   4 },
          { .name = "Output_Bandwidth",                               .octets =   4 },
          { .name = "Input_Coding_Format",                            .octets =   5 },
          { .name = "Output_Coding_Format",                           .octets =   5 },
          { .name = "Input_Coded_Data_Size",                          .octets =   2 },
          { .name = "Output_Coded_Data_Size",                         .octets =   2 },
          { .name = "Input_PCM_Data_Format",                          .octets =   1 },
          { .name = "Output_PCM_Data_Format",                         .octets =   1 },
          { .name = "Input_PCM_Sample_Payload_MSB_Position",          .octets =   1 },
          { .name = "Output_PCM_Sample_Payload_MSB_Position",         .octets =   1 },
          { .name = "Input_Data_Path",                                .octets =   1 },
          { .name = "Output_Data_Path",                               .octets =   1 },
          { .name = "Input_Transport_Unit_Size",                      .octets =   1 },
          { .name = "Output_Transport_Unit_Size",                     .octets =   1 },
          { .name = "Max_Latency",                                    .octets =   2 },
          { .name = "Packet_Type",                                    .octets =   2 },
          { .name = "Retransmission_Effort",                          .octets =   1 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Synchronous_Connection_Complete" },
    },
  },

  { .title = "Enhanced Flush command", .iscmd = 1, .OGF = 0x03, .section = "7.3.66", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Enhanced_Flush command is used to discard specific packets "
               "currently pending for transmission in the Controller for the "
               "specified Handle. This command takes a parameter specifying the type "
               "of packets to be flushed.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Enhanced_Flush", .OCF = 0x005f, .numcoe = 2, .numret = 0, .octet = 19, .bit = 6, .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Handle",                                         .octets =   2 },
          { .name = "Packet_Type",                                    .octets =   1 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Enhanced_Flush_Complete" },
    },
  },

  { .title = "Enhanced Flush Complete event", .iscmd = 0, .section = "7.7.47", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Enhanced_Flush_Complete event is used to indicate that an "
               "Enhanced Flush is complete.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Enhanced_Flush_Complete", .code = 0x0039, .numcoe = 1, .numret = 0,  .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Handle",                                         .octets =   2 },
        },
      },
    },
  },

  { .title = "Enhanced Setup Synchronous Connection command", .iscmd = 1, .OGF = 0x01, .section = "7.1.45", .version = "CSA2", .BREDR = "C.135", .LE = "E",
    .summary = "The HCI_Enhanced_Setup_Synchronous_Connection command adds a new or "
               "modifies an existing synchronous logical transport (SCO or eSCO) on a "
               "physical link depending on the Connection_Handle parameter "
               "specified.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Enhanced_Setup_Synchronous_Connection", .OCF = 0x003d, .numcoe = 24, .numret = 0, .octet = 29, .bit = 3, .Cond = { "C.135", "E" },
        .CoE = (HCI_Field_t[24]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Transmit_Bandwidth",                             .octets =   4 },
          { .name = "Receive_Bandwidth",                              .octets =   4 },
          { .name = "Transmit_Coding_Format",                         .octets =   5 },
          { .name = "Receive_Coding_Format",                          .octets =   5 },
          { .name = "Transmit_Codec_Frame_Size",                      .octets =   2 },
          { .name = "Receive_Codec_Frame_Size",                       .octets =   2 },
          { .name = "Input_Bandwidth",                                .octets =   4 },
          { .name = "Output_Bandwidth",                               .octets =   4 },
          { .name = "Input_Coding_Format",                            .octets =   5 },
          { .name = "Output_Coding_Format",                           .octets =   5 },
          { .name = "Input_Coded_Data_Size",                          .octets =   2 },
          { .name = "Output_Coded_Data_Size",                         .octets =   2 },
          { .name = "Input_PCM_Data_Format",                          .octets =   1 },
          { .name = "Output_PCM_Data_Format",                         .octets =   1 },
          { .name = "Input_PCM_Sample_Payload_MSB_Position",          .octets =   1 },
          { .name = "Output_PCM_Sample_Payload_MSB_Position",         .octets =   1 },
          { .name = "Input_Data_Path",                                .octets =   1 },
          { .name = "Output_Data_Path",                               .octets =   1 },
          { .name = "Input_Transport_Unit_Size",                      .octets =   1 },
          { .name = "Output_Transport_Unit_Size",                     .octets =   1 },
          { .name = "Max_Latency",                                    .octets =   2 },
          { .name = "Packet_Type",                                    .octets =   2 },
          { .name = "Retransmission_Effort",                          .octets =   1 },
        },
      },
    },
    .numevts = 3, .Events = (HCI_Event_t[3]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Synchronous_Connection_Complete" },
      { .name = "HCI_Connection_Complete" },
    },
  },

  { .title = "Exit Periodic Inquiry Mode command", .iscmd = 1, .OGF = 0x01, .section = "7.1.4", .version = "1.1", .BREDR = "C.103", .LE = "E",
    .summary = "The HCI_Exit_Periodic_Inquiry_Mode command is used to end the "
               "Periodic Inquiry mode when the local device is in Periodic Inquiry "
               "Mode.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Exit_Periodic_Inquiry_Mode", .OCF = 0x0004, .numcoe = 0, .numret = 1, .octet =  0, .bit = 3, .Cond = { "C.103", "E" },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_Inquiry_Complete" },
    },
  },

  { .title = "Exit Sniff Mode command", .iscmd = 1, .OGF = 0x02, .section = "7.2.3", .version = "1.1", .BREDR = "C.214", .LE = "E",
    .summary = "The HCI_Exit_Sniff_Mode command is used to end Sniff mode for a "
               "Connection_Handle which is currently in Sniff mode.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Exit_Sniff_Mode", .OCF = 0x0004, .numcoe = 1, .numret = 0, .octet =  4, .bit = 3, .Cond = { "C.214", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Connection_Handle",                              .octets =   2 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Mode_Change" },
    },
  },

  { .title = "Extended Inquiry Result event", .iscmd = 0, .section = "7.7.38", .version = "2.1 + EDR", .BREDR = "C.147", .LE = "E",
    .summary = "The HCI_Extended_Inquiry_Result event indicates that a BR/ EDR "
               "Controller has responded with an extended inquiry response during the "
               "current Inquiry process.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Extended_Inquiry_Result", .code = 0x002f, .numcoe = 8, .numret = 0,  .Cond = { "C.147", "E" },
        .CoE = (HCI_Field_t[8]) {
          { .name = "Num_Responses",                                  .octets =   1 },
          { .name = "BD_ADDR",                                        .octets =   6 },
          { .name = "Page_Scan_Repetition_Mode",                      .octets =   1 },
          { .name = "Reserved",                                       .octets =   1 },
          { .name = "Class_Of_Device",                                .octets =   3 },
          { .name = "Clock_Offset",                                   .octets =   2 },
          { .name = "RSSI",                                           .octets =   1 },
          { .name = "Extended_Inquiry_Response",                      .octets = 240 },
        },
      },
    },
  },

  { .title = "Flow Specification command", .iscmd = 1, .OGF = 0x02, .section = "7.2.13", .version = "1.2", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Flow_Specification command is used to specify the flow "
               "parameters for the traffic carried over the ACL connection identified "
               "by the Connection_Handle.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Flow_Specification", .OCF = 0x0010, .numcoe = 8, .numret = 0, .octet =  5, .bit = 5, .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[8]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Unused",                                         .octets =   1 },
          { .name = "Flow_Direction",                                 .octets =   1 },
          { .name = "Service_Type",                                   .octets =   1 },
          { .name = "Token_Rate",                                     .octets =   4 },
          { .name = "Token_Bucket_Size",                              .octets =   4 },
          { .name = "Peak_Bandwidth",                                 .octets =   4 },
          { .name = "Access_Latency",                                 .octets =   4 },
        },
      },
    },
    .numevts = 3, .Events = (HCI_Event_t[3]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Flow_Specification_Complete" },
      { .name = "HCI_QoS_Setup_Complete" },
    },
  },

  { .title = "Flow Specification Complete event", .iscmd = 0, .section = "7.7.32", .version = "1.2", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Flow_Specification_Complete event is used to inform the Host "
               "about the Quality of Service for the ACL connection the Controller is "
               "able to support.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Flow_Specification_Complete", .code = 0x0021, .numcoe = 9, .numret = 0,  .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[9]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Unused",                                         .octets =   1 },
          { .name = "Flow_Direction",                                 .octets =   1 },
          { .name = "Service_Type",                                   .octets =   1 },
          { .name = "Token_Rate",                                     .octets =   4 },
          { .name = "Token_Bucket_Size",                              .octets =   4 },
          { .name = "Peak_Bandwidth",                                 .octets =   4 },
          { .name = "Access_Latency",                                 .octets =   4 },
        },
      },
    },
  },

  { .title = "Flush command", .iscmd = 1, .OGF = 0x03, .section = "7.3.4", .version = "1.1", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Flush command is used to discard all data that is currently "
               "pending for transmission in the Controller for the specified "
               "Connection_Handle.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Flush", .OCF = 0x0008, .numcoe = 1, .numret = 2, .octet =  6, .bit = 1, .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Connection_Handle",                              .octets =   2 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Flush_Occurred" },
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Flush Occurred event", .iscmd = 0, .section = "7.7.17", .version = "1.1", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Flush_Occurred event is used to indicate that, for the "
               "specified Handle, the data to be transmitted has been discarded.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Flush_Occurred", .code = 0x0011, .numcoe = 1, .numret = 0,  .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Handle",                                         .octets =   2 },
        },
      },
    },
  },

  { .title = "Get MWS Transport Layer Configuration command", .iscmd = 1, .OGF = 0x05, .section = "7.5.11", .version = "CSA3", .BREDR = "C.109", .LE = "C.109",
    .summary = "The HCI_Get_MWS_Transport_Layer_Configuration command reads the "
               "supported baud rates from the Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Get_MWS_Transport_Layer_Configuration", .OCF = 0x000c, .numcoe = 0, .numret = 6, .octet = 30, .bit = 3, .Cond = { "C.109", "C.109" },
        .Ret = (HCI_Field_t[6]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Num_Transports",                                 .octets =   1 },
          { .name = "Transport_Layer",                                .octets =   1, .ref2size = "Num_Transports" },
          { .name = "Num_Baud_Rates",                                 .octets =   1, .ref2size = "Num_Transports" },
          { .name = "To_MWS_Baud_Rate",                               .octets =   4, .ref2size = "Num_Baud_Rates" },
          { .name = "From_MWS_Baud_Rate",                             .octets =   4, .ref2size = "Num_Baud_Rates" },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Hardware Error event", .iscmd = 0, .section = "7.7.16", .version = "1.1", .BREDR = "O", .LE = "O",
    .summary = "The HCI_Hardware_Error event is used to indicate some type of "
               "hardware failure for the Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Hardware_Error", .code = 0x0010, .numcoe = 1, .numret = 0,  .Cond = { "O", "O" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Hardware_Code",                                  .octets =   1 },
        },
      },
    },
  },

  { .title = "Hold Mode command", .iscmd = 1, .OGF = 0x02, .section = "7.2.1", .version = "1.1", .BREDR = "C.213", .LE = "E",
    .summary = "The HCI_Hold_Mode command is used to initiate Hold mode.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Hold_Mode", .OCF = 0x0001, .numcoe = 3, .numret = 0, .octet =  4, .bit = 1, .Cond = { "C.213", "E" },
        .CoE = (HCI_Field_t[3]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Hold_Mode_Max_Interval",                         .octets =   2 },
          { .name = "Hold_Mode_Min_Interval",                         .octets =   2 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Mode_Change" },
    },
  },

  { .title = "Host Buffer Size command", .iscmd = 1, .OGF = 0x03, .section = "7.3.39", .version = "1.1", .BREDR = "C.107", .LE = "C.107",
    .summary = "The HCI_Host_Buffer_Size command is used by the Host to notify the "
               "Controller about its buffer sizes for ACL and synchronous data. The "
               "Controller will segment the data to be transmitted from the "
               "Controller to the Host, so that data contained in HCI Data packets "
               "will not exceed these sizes.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Host_Buffer_Size", .OCF = 0x0033, .numcoe = 4, .numret = 1, .octet = 10, .bit = 6, .Cond = { "C.107", "C.107" },
        .CoE = (HCI_Field_t[4]) {
          { .name = "Host_ACL_Data_Packet_Length",                    .octets =   2 },
          { .name = "Host_Synchronous_Data_Packet_Length",            .octets =   1 },
          { .name = "Host_Total_Num_ACL_Data_Packets",                .octets =   2 },
          { .name = "Host_Total_Num_Synchronous_Data_Packets",        .octets =   2 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Host Number Of Completed Packets command", .iscmd = 1, .OGF = 0x03, .section = "7.3.40", .version = "1.1", .BREDR = "C.107", .LE = "C.107",
    .summary = "The HCI_Host_Number_Of_Completed_Packets command is used by the Host "
               "to indicate to the Controller when the Host is ready to receive more "
               "HCI packets for any Connection_Handle.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Host_Number_Of_Completed_Packets", .OCF = 0x0035, .numcoe = 3, .numret = 0, .octet = 10, .bit = 7, .Cond = { "C.107", "C.107" },
        .CoE = (HCI_Field_t[3]) {
          { .name = "Num_Handles",                                    .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2, .ref2size = "Num_Handles" },
          { .name = "Host_Num_Completed_Packets",                     .octets =   2, .ref2size = "Num_Handles" },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Inquiry Cancel command", .iscmd = 1, .OGF = 0x01, .section = "7.1.2", .version = "1.1", .BREDR = "C.127", .LE = "E",
    .summary = "The HCI_Inquiry_Cancel command will cause the BR/EDR Controller to "
               "stop the current Inquiry if the BR/EDR Controller is in Inquiry "
               "Mode.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Inquiry_Cancel", .OCF = 0x0002, .numcoe = 0, .numret = 1, .octet =  0, .bit = 1, .Cond = { "C.127", "E" },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_Inquiry_Complete" },
    },
  },

  { .title = "Inquiry command", .iscmd = 1, .OGF = 0x01, .section = "7.1.1", .version = "1.1", .BREDR = "C.127", .LE = "E",
    .summary = "The HCI_Inquiry command will cause the BR/EDR Controller to enter "
               "Inquiry Mode. Inquiry Mode is used to discovery other nearby BR/EDR "
               "Controllers.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Inquiry", .OCF = 0x0001, .numcoe = 3, .numret = 0, .octet =  0, .bit = 0, .Cond = { "C.127", "E" },
        .CoE = (HCI_Field_t[3]) {
          { .name = "LAP",                                            .octets =   3 },
          { .name = "Inquiry_Length",                                 .octets =   1 },
          { .name = "Num_Responses",                                  .octets =   1 },
        },
      },
    },
    .numevts = 3, .Events = (HCI_Event_t[3]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Extended_Inquiry_Result" },
      { .name = "HCI_Inquiry_Complete" },
    },
  },

  { .title = "Inquiry Complete event", .iscmd = 0, .section = "7.7.1", .version = "1.1", .BREDR = "C.127", .LE = "E",
    .summary = "The HCI_Inquiry_Complete event indicates that the Inquiry is "
               "finished.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Inquiry_Complete", .code = 0x0001, .numcoe = 1, .numret = 0,  .Cond = { "C.127", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
  },

  { .title = "Inquiry Response Notification event", .iscmd = 0, .section = "7.7.74", .version = "CSA4", .BREDR = "C.126", .LE = "E",
    .summary = "The HCI_Inquiry_Response_Notification event indicates to the Host "
               "that the BR/EDR Controller responded to an inquiry message.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Inquiry_Response_Notification", .code = 0x0056, .numcoe = 2, .numret = 0,  .Cond = { "C.126", "E" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "LAP",                                            .octets =   3 },
          { .name = "RSSI",                                           .octets =   1 },
        },
      },
    },
  },

  { .title = "Inquiry Result event", .iscmd = 0, .section = "7.7.2", .version = "1.1", .BREDR = "C.127", .LE = "E",
    .summary = "The HCI_Inquiry_Result event indicates that a BR/EDR Controller or "
               "multiple BR/EDR Controllers have responded so far during the current "
               "Inquiry process.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Inquiry_Result", .code = 0x0002, .numcoe = 6, .numret = 0,  .Cond = { "C.127", "E" },
        .CoE = (HCI_Field_t[6]) {
          { .name = "Num_Responses",                                  .octets =   1 },
          { .name = "BD_ADDR",                                        .octets =   6, .ref2size = "Num_Responses" },
          { .name = "Page_Scan_Repetition_Mode",                      .octets =   1, .ref2size = "Num_Responses" },
          { .name = "Reserved",                                       .octets =   2, .ref2size = "Num_Responses" },
          { .name = "Class_Of_Device",                                .octets =   3, .ref2size = "Num_Responses" },
          { .name = "Clock_Offset",                                   .octets =   2, .ref2size = "Num_Responses" },
        },
      },
    },
  },

  { .title = "Inquiry Result with RSSI event", .iscmd = 0, .section = "7.7.33", .version = "1.2", .BREDR = "C.128", .LE = "E",
    .summary = "The HCI_Inquiry_Result_with_RSSI event indicates that a BR/ EDR "
               "Controller or multiple BR/ EDR Controllers have responded so far "
               "during the current Inquiry process.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Inquiry_Result_with_RSSI", .code = 0x0022, .numcoe = 7, .numret = 0,  .Cond = { "C.128", "E" },
        .CoE = (HCI_Field_t[7]) {
          { .name = "Num_Responses",                                  .octets =   1 },
          { .name = "BD_ADDR",                                        .octets =   6, .ref2size = "Num_Responses" },
          { .name = "Page_Scan_Repetition_Mode",                      .octets =   1, .ref2size = "Num_Responses" },
          { .name = "Reserved",                                       .octets =   1, .ref2size = "Num_Responses" },
          { .name = "Class_Of_Device",                                .octets =   3, .ref2size = "Num_Responses" },
          { .name = "Clock_Offset",                                   .octets =   2, .ref2size = "Num_Responses" },
          { .name = "RSSI",                                           .octets =   1, .ref2size = "Num_Responses" },
        },
      },
    },
  },

  { .title = "IO Capability Request event", .iscmd = 0, .section = "7.7.40", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .summary = "The HCI_IO_Capability_Request event is used to indicate that the IO "
               "capabilities of the Host are required for a Secure Simple Pairing "
               "process.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_IO_Capability_Request", .code = 0x0031, .numcoe = 1, .numret = 0,  .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "BD_ADDR",                                        .octets =   6 },
        },
      },
    },
  },

  { .title = "IO Capability Request Negative Reply command", .iscmd = 1, .OGF = 0x01, .section = "7.1.36", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .summary = "The HCI_IO_Capability_Request_Negative_Reply command is used to "
               "reject a pairing attempt after an HCI_IO_Capability_Request event has "
               "been received by the Host.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_IO_Capability_Request_Negative_Reply", .OCF = 0x0034, .numcoe = 2, .numret = 2, .octet = 20, .bit = 3, .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "BD_ADDR",                                        .octets =   6 },
          { .name = "Reason",                                         .octets =   1 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "BD_ADDR",                                        .octets =   6 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "IO Capability Request Reply command", .iscmd = 1, .OGF = 0x01, .section = "7.1.29", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .summary = "The HCI_IO_Capability_Request_Reply command is used to reply to an "
               "HCI_IO_Capability_Request event from the Controller, and specifies "
               "the current I/O capabilities of the Host.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_IO_Capability_Request_Reply", .OCF = 0x002b, .numcoe = 4, .numret = 2, .octet = 18, .bit = 7, .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[4]) {
          { .name = "BD_ADDR",                                        .octets =   6 },
          { .name = "IO_Capability",                                  .octets =   1 },
          { .name = "OOB_Data_Present",                               .octets =   1 },
          { .name = "Authentication_Requirements",                    .octets =   1 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "BD_ADDR",                                        .octets =   6 },
        },
      },
    },
    .numevts = 3, .Events = (HCI_Event_t[3]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_IO_Capability_Response" },
      { .name = "HCI_Remote_OOB_Data_Request" },
    },
  },

  { .title = "IO Capability Response event", .iscmd = 0, .section = "7.7.41", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .summary = "The HCI_IO_Capability_Response event is used to indicate to the Host "
               "that IO capabilities from a remote device specified by BD_ADDR have "
               "been received during a Secure Simple Pairing process.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_IO_Capability_Response", .code = 0x0032, .numcoe = 4, .numret = 0,  .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[4]) {
          { .name = "BD_ADDR",                                        .octets =   6 },
          { .name = "IO_Capability",                                  .octets =   1 },
          { .name = "OOB_Data_Present",                               .octets =   1 },
          { .name = "Authentication_Requirements",                    .octets =   1 },
        },
      },
    },
  },

  { .title = "Keypress Notification event", .iscmd = 0, .section = "7.7.49", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Keypress_Notification event is sent to the Host after a "
               "passkey notification has been received by the Link Manager on the "
               "given BD_ADDR.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Keypress_Notification", .code = 0x003c, .numcoe = 2, .numret = 0,  .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "BD_ADDR",                                        .octets =   6 },
          { .name = "Notification_Type",                              .octets =   1 },
        },
      },
    },
  },

  { .title = "LE Accept CIS Request command", .iscmd = 1, .OGF = 0x08, .section = "7.8.101", .version = "5.2", .BREDR = "E", .LE = "C.40",
    .summary = "The HCI_LE_Accept_CIS_Request command is used by the Peripheral's "
               "Host to inform the Controller to accept the request for creating the "
               "CIS.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Accept_CIS_Request", .OCF = 0x0066, .numcoe = 1, .numret = 0, .octet = 42, .bit = 3, .Cond = { "E", "C.40" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Connection_Handle",                              .octets =   2 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_LE_CIS_Established" },
    },
  },

  { .title = "LE Add Device To Filter Accept List command", .iscmd = 1, .OGF = 0x08, .section = "7.8.16", .version = "4.0", .BREDR = "E", .LE = "M",
    .summary = "The HCI_LE_Add_Device_To_Filter_Accept_List command will add a device "
               "to the Filter Accept List.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Add_Device_To_Filter_Accept_List", .OCF = 0x0011, .numcoe = 2, .numret = 1, .octet = 27, .bit = 0, .Cond = { "E", "M" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Address_Type",                                   .octets =   1 },
          { .name = "Address",                                        .octets =   6 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Add Device To Periodic Advertiser List command", .iscmd = 1, .OGF = 0x08, .section = "7.8.70", .version = "5.0", .BREDR = "E", .LE = "C.21",
    .summary = "The HCI_LE_Add_Device_To_Periodic_Advertiser_List command will add a "
               "device to the Periodic Advertiser List.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Add_Device_To_Periodic_Advertiser_List", .OCF = 0x0047, .numcoe = 3, .numret = 1, .octet = 38, .bit = 3, .Cond = { "E", "C.21" },
        .CoE = (HCI_Field_t[3]) {
          { .name = "Advertiser_Address_Type",                        .octets =   1 },
          { .name = "Advertiser_Address",                             .octets =   6 },
          { .name = "Advertising_SID",                                .octets =   1 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Add Device To Resolving List command", .iscmd = 1, .OGF = 0x08, .section = "7.8.38", .version = "4.2", .BREDR = "E", .LE = "C.9",
    .summary = "The HCI_LE_Add_Device_To_Resolving_List command is used to add one "
               "device to the resolving list used to resolve Resolvable Private "
               "Addresses in the Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Add_Device_To_Resolving_List", .OCF = 0x0027, .numcoe = 4, .numret = 1, .octet = 34, .bit = 3, .Cond = { "E", "C.9" },
        .CoE = (HCI_Field_t[4]) {
          { .name = "Peer_Identity_Address_Type",                     .octets =   1 },
          { .name = "Peer_Identity_Address",                          .octets =   6 },
          { .name = "Peer_IRK",                                       .octets =  16 },
          { .name = "Local_IRK",                                      .octets =  16 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Advertising Report event", .iscmd = 0, .section = "7.7.65.2", .version = "4.0", .BREDR = "E", .LE = "C.98",
    .summary = "The HCI_LE_Advertising_Report event indicates that an advertising or "
               "scan response packet has been received.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Advertising_Report", .code = 0x003e, .subevent = 0x02, .numcoe = 8, .numret = 0,  .Cond = { "E", "C.98" },
        .CoE = (HCI_Field_t[8]) {
          { .name = "Subevent_Code",                                  .octets =   1 },
          { .name = "Num_Reports",                                    .octets =   1 },
          { .name = "Event_Type",                                     .octets =   1, .ref2size = "Num_Reports" },
          { .name = "Address_Type",                                   .octets =   1, .ref2size = "Num_Reports" },
          { .name = "Address",                                        .octets =   6, .ref2size = "Num_Reports" },
          { .name = "Data_Length",                                    .octets =   1, .ref2size = "Num_Reports" },
          { .name = "Data",                                                         .ref2size = "Data_Length" },
          { .name = "RSSI",                                           .octets =   1, .ref2size = "Num_Reports" },
        },
      },
    },
  },

  { .title = "LE Advertising Set Terminated event", .iscmd = 0, .section = "7.7.65.18", .version = "5.0", .BREDR = "E", .LE = "C.17",
    .summary = "The HCI_LE_Advertising_Set_Terminated event indicates that "
               "advertising in a given advertising set has stopped.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Advertising_Set_Terminated", .code = 0x003e, .subevent = 0x12, .numcoe = 5, .numret = 0,  .Cond = { "E", "C.17" },
        .CoE = (HCI_Field_t[5]) {
          { .name = "Subevent_Code",                                  .octets =   1 },
          { .name = "Status",                                         .octets =   1 },
          { .name = "Advertising_Handle",                             .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Num_Completed_Extended_Advertising_Events",      .octets =   1 },
        },
      },
    },
  },

  { .title = "LE BIG Create Sync command", .iscmd = 1, .OGF = 0x08, .section = "7.8.106", .version = "5.2", .BREDR = "E", .LE = "C.42",
    .summary = "The HCI_LE_BIG_Create_Sync synchronizes and receives PDUs from one or "
               "more BISes.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_BIG_Create_Sync", .OCF = 0x006b, .numcoe = 8, .numret = 0, .octet = 43, .bit = 0, .Cond = { "E", "C.42" },
        .CoE = (HCI_Field_t[8]) {
          { .name = "BIG_Handle",                                     .octets =   1 },
          { .name = "Sync_Handle",                                    .octets =   2 },
          { .name = "Encryption",                                     .octets =   1 },
          { .name = "Broadcast_Code",                                 .octets =  16 },
          { .name = "MSE",                                            .octets =   1 },
          { .name = "BIG_Sync_Timeout",                               .octets =   2 },
          { .name = "Num_BIS",                                        .octets =   1 },
          { .name = "BIS",                                            .octets =   1, .ref2size = "Num_BIS" },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_LE_BIG_Sync_Established" },
    },
  },

  { .title = "LE BIG Sync Established event", .iscmd = 0, .section = "7.7.65.29", .version = "5.2", .BREDR = "E", .LE = "C.42",
    .summary = "The HCI_LE_BIG_Sync_Established event indicates that the Controller "
               "has completed an attempt to synchronize with the requested BISes.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_BIG_Sync_Established", .code = 0x003e, .subevent = 0x1d, .numcoe = 12, .numret = 0,  .Cond = { "E", "C.42" },
        .CoE = (HCI_Field_t[12]) {
          { .name = "Subevent_Code",                                  .octets =   1 },
          { .name = "Status",                                         .octets =   1 },
          { .name = "BIG_Handle",                                     .octets =   1 },
          { .name = "Transport_Latency_BIG",                          .octets =   3 },
          { .name = "NSE",                                            .octets =   1 },
          { .name = "BN",                                             .octets =   1 },
          { .name = "PTO",                                            .octets =   1 },
          { .name = "IRC",                                            .octets =   1 },
          { .name = "Max_PDU",                                        .octets =   2 },
          { .name = "ISO_Interval",                                   .octets =   2 },
          { .name = "Num_BIS",                                        .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2, .ref2size = "Num_BIS" },
        },
      },
    },
  },

  { .title = "LE BIG Sync Lost event", .iscmd = 0, .section = "7.7.65.30", .version = "5.2", .BREDR = "E", .LE = "C.42",
    .summary = "The HCI_LE_BIG_Sync_Lost event indicates that the Controller stopped "
               "synchronizing with a BIG.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_BIG_Sync_Lost", .code = 0x003e, .subevent = 0x1e, .numcoe = 3, .numret = 0,  .Cond = { "E", "C.42" },
        .CoE = (HCI_Field_t[3]) {
          { .name = "Subevent_Code",                                  .octets =   1 },
          { .name = "BIG_Handle",                                     .octets =   1 },
          { .name = "Reason",                                         .octets =   1 },
        },
      },
    },
  },

  { .title = "LE BIG Terminate Sync command", .iscmd = 1, .OGF = 0x08, .section = "7.8.107", .version = "5.2", .BREDR = "E", .LE = "C.42",
    .summary = "The HCI_LE_BIG_Terminate_Sync command stops or cancels synchronizing "
               "with a BIG.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_BIG_Terminate_Sync", .OCF = 0x006c, .numcoe = 1, .numret = 2, .octet = 43, .bit = 1, .Cond = { "E", "C.42" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "BIG_Handle",                                     .octets =   1 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "BIG_Handle",                                     .octets =   1 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_LE_BIG_Sync_Established" },
    },
  },

  { .title = "LE BIGInfo Advertising Report event", .iscmd = 0, .section = "7.7.65.34", .version = "5.2", .BREDR = "E", .LE = "C.54",
    .summary = "The HCI_LE_BIGInfo_Advertising_Report event indicates that the "
               "Controller has received an Advertising PDU that contained a BIGInfo "
               "field.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_BIGInfo_Advertising_Report", .code = 0x003e, .subevent = 0x22, .numcoe = 14, .numret = 0,  .Cond = { "E", "C.54" },
        .CoE = (HCI_Field_t[14]) {
          { .name = "Subevent_Code",                                  .octets =   1 },
          { .name = "Sync_Handle",                                    .octets =   2 },
          { .name = "Num_BIS",                                        .octets =   1 },
          { .name = "NSE",                                            .octets =   1 },
          { .name = "ISO_Interval",                                   .octets =   2 },
          { .name = "BN",                                             .octets =   1 },
          { .name = "PTO",                                            .octets =   1 },
          { .name = "IRC",                                            .octets =   1 },
          { .name = "Max_PDU",                                        .octets =   2 },
          { .name = "SDU_Interval",                                   .octets =   3 },
          { .name = "Max_SDU",                                        .octets =   2 },
          { .name = "PHY",                                            .octets =   1 },
          { .name = "Framing",                                        .octets =   1 },
          { .name = "Encryption",                                     .octets =   1 },
        },
      },
    },
  },

  { .title = "LE Channel Selection Algorithm event", .iscmd = 0, .section = "7.7.65.20", .version = "5.0", .BREDR = "E", .LE = "C.23",
    .summary = "The HCI_LE_Channel_Selection_Algorithm event indicates the channel "
               "selection algorithm used on a connection.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Channel_Selection_Algorithm", .code = 0x003e, .subevent = 0x14, .numcoe = 3, .numret = 0,  .Cond = { "E", "C.23" },
        .CoE = (HCI_Field_t[3]) {
          { .name = "Subevent_Code",                                  .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Channel_Selection_Algorithm",                    .octets =   1 },
        },
      },
    },
  },

  { .title = "LE CIS Established event", .iscmd = 0, .section = "7.7.65.25", .version = "5.2", .BREDR = "E", .LE = "C.38",
    .summary = "The HCI_LE_CIS_Established event indicates that the Controller "
               "established a CIS.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_CIS_Established", .code = 0x003e, .subevent = 0x19, .numcoe = 17, .numret = 0,  .Cond = { "E", "C.38" },
        .CoE = (HCI_Field_t[17]) {
          { .name = "Subevent_Code",                                  .octets =   1 },
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "CIG_Sync_Delay",                                 .octets =   3 },
          { .name = "CIS_Sync_Delay",                                 .octets =   3 },
          { .name = "Transport_Latency_C_To_P",                       .octets =   3 },
          { .name = "Transport_Latency_P_To_C",                       .octets =   3 },
          { .name = "PHY_C_To_P",                                     .octets =   1 },
          { .name = "PHY_P_To_C",                                     .octets =   1 },
          { .name = "NSE",                                            .octets =   1 },
          { .name = "BN_C_To_P",                                      .octets =   1 },
          { .name = "BN_P_To_C",                                      .octets =   1 },
          { .name = "FT_C_To_P",                                      .octets =   1 },
          { .name = "FT_P_To_C",                                      .octets =   1 },
          { .name = "Max_PDU_C_To_P",                                 .octets =   2 },
          { .name = "Max_PDU_P_To_C",                                 .octets =   2 },
          { .name = "ISO_Interval",                                   .octets =   2 },
        },
      },
    },
  },

  { .title = "LE CIS Request event", .iscmd = 0, .section = "7.7.65.26", .version = "5.2", .BREDR = "E", .LE = "C.40",
    .summary = "The HCI_LE_CIS_Request event indicates that the Peripheral's "
               "Controller received a request from the Central to create a CIS.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_CIS_Request", .code = 0x003e, .subevent = 0x1a, .numcoe = 5, .numret = 0,  .Cond = { "E", "C.40" },
        .CoE = (HCI_Field_t[5]) {
          { .name = "Subevent_Code",                                  .octets =   1 },
          { .name = "ACL_Connection_Handle",                          .octets =   2 },
          { .name = "CIS_Connection_Handle",                          .octets =   2 },
          { .name = "CIG_ID",                                         .octets =   1 },
          { .name = "CIS_ID",                                         .octets =   1 },
        },
      },
    },
  },

  { .title = "LE Clear Advertising Sets command", .iscmd = 1, .OGF = 0x08, .section = "7.8.60", .version = "5.0", .BREDR = "E", .LE = "C.17",
    .summary = "The HCI_LE_Clear_Advertising_Sets command will remove all existing "
               "advertising sets from the Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Clear_Advertising_Sets", .OCF = 0x003d, .numcoe = 0, .numret = 1, .octet = 37, .bit = 1, .Cond = { "E", "C.17" },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Clear Filter Accept List command", .iscmd = 1, .OGF = 0x08, .section = "7.8.15", .version = "4.0", .BREDR = "E", .LE = "M",
    .summary = "The HCI_LE_Clear_Filter_Accept_List command will clear the Filter "
               "Accept List.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Clear_Filter_Accept_List", .OCF = 0x0010, .numcoe = 0, .numret = 1, .octet = 26, .bit = 7, .Cond = { "E", "M" },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Clear Periodic Advertiser List command", .iscmd = 1, .OGF = 0x08, .section = "7.8.72", .version = "5.0", .BREDR = "E", .LE = "C.21",
    .summary = "The HCI_LE_Clear_Periodic_Advertiser_List command will clear the "
               "Periodic Advertiser List.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Clear_Periodic_Advertiser_List", .OCF = 0x0049, .numcoe = 0, .numret = 1, .octet = 38, .bit = 5, .Cond = { "E", "C.21" },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Clear Resolving List command", .iscmd = 1, .OGF = 0x08, .section = "7.8.40", .version = "4.2", .BREDR = "E", .LE = "C.9",
    .summary = "The HCI_LE_Clear_Resolving_List command is used to remove all devices "
               "from the resolving list used to resolve Resolvable Private Addresses "
               "in the Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Clear_Resolving_List", .OCF = 0x0029, .numcoe = 0, .numret = 1, .octet = 34, .bit = 5, .Cond = { "E", "C.9" },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Connection Complete event", .iscmd = 0, .section = "7.7.65.1", .version = "4.0", .BREDR = "E", .LE = "C.3",
    .summary = "The HCI_LE_Connection_Complete event indicates to the Host that a new "
               "connection has been created.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Connection_Complete", .code = 0x003e, .subevent = 0x01, .numcoe = 10, .numret = 0,  .Cond = { "E", "C.3" },
        .CoE = (HCI_Field_t[10]) {
          { .name = "Subevent_Code",                                  .octets =   1 },
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Role",                                           .octets =   1 },
          { .name = "Peer_Address_Type",                              .octets =   1 },
          { .name = "Peer_Address",                                   .octets =   6 },
          { .name = "Connection_Interval",                            .octets =   2 },
          { .name = "Peripheral_Latency",                             .octets =   2 },
          { .name = "Supervision_Timeout",                            .octets =   2 },
          { .name = "Central_Clock_Accuracy",                         .octets =   1 },
        },
      },
    },
  },

  { .title = "LE Connection CTE Request Enable command", .iscmd = 1, .OGF = 0x08, .section = "7.8.85", .version = "5.1", .BREDR = "E", .LE = "C.25",
    .summary = "The HCI_LE_Connection_CTE_Request_Enable command will request the "
               "Controller to start or stop sending of LL_CTE_REQ PDUs on a "
               "connection.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Connection_CTE_Request_Enable", .OCF = 0x0056, .numcoe = 5, .numret = 2, .octet = 40, .bit = 2, .Cond = { "E", "C.25" },
        .CoE = (HCI_Field_t[5]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Enable",                                         .octets =   1 },
          { .name = "CTE_Request_Interval",                           .octets =   2 },
          { .name = "Requested_CTE_Length",                           .octets =   1 },
          { .name = "Requested_CTE_Type",                             .octets =   1 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
        },
      },
    },
    .numevts = 3, .Events = (HCI_Event_t[3]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_LE_Connection_IQ_Report" },
      { .name = "HCI_LE_CTE_Request_Failed" },
    },
  },

  { .title = "LE Connection CTE Response Enable command", .iscmd = 1, .OGF = 0x08, .section = "7.8.86", .version = "5.1", .BREDR = "E", .LE = "C.26",
    .summary = "The HCI_LE_Connection_CTE_Response_Enable command will command the "
               "Controller to respond to LL_CTE_REQ PDUs with LL_CTE_RSP PDUs.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Connection_CTE_Response_Enable", .OCF = 0x0057, .numcoe = 2, .numret = 2, .octet = 40, .bit = 3, .Cond = { "E", "C.26" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Enable",                                         .octets =   1 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Connection IQ Report event", .iscmd = 0, .section = "7.7.65.22", .version = "5.1", .BREDR = "E", .LE = "C.25",
    .summary = "The HCI_LE_Connection_IQ_Report event is used to report IQ samples "
               "from the Constant Tone Extension field of a received packet "
               "containing an LL_CTE_RSP PDU.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Connection_IQ_Report", .code = 0x003e, .subevent = 0x16, .numcoe = 13, .numret = 0,  .Cond = { "E", "C.25" },
        .CoE = (HCI_Field_t[13]) {
          { .name = "Subevent_Code",                                  .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "RX_PHY",                                         .octets =   1 },
          { .name = "Data_Channel_Index",                             .octets =   1 },
          { .name = "RSSI",                                           .octets =   2 },
          { .name = "RSSI_Antenna_ID",                                .octets =   1 },
          { .name = "CTE_Type",                                       .octets =   1 },
          { .name = "Slot_Durations",                                 .octets =   1 },
          { .name = "Packet_Status",                                  .octets =   1 },
          { .name = "Connection_Event_Counter",                       .octets =   2 },
          { .name = "Sample_Count",                                   .octets =   1 },
          { .name = "I_Sample",                                       .octets =   1, .ref2size = "Sample_Count" },
          { .name = "Q_Sample",                                       .octets =   1, .ref2size = "Sample_Count" },
        },
      },
    },
  },

  { .title = "LE Connection Update command", .iscmd = 1, .OGF = 0x08, .section = "7.8.18", .version = "4.0", .BREDR = "E", .LE = "C.62",
    .summary = "The HCI_LE_Connection_Update command will be used to change the "
               "connection parameters of an existing connection.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Connection_Update", .OCF = 0x0013, .numcoe = 7, .numret = 0, .octet = 27, .bit = 2, .Cond = { "E", "C.62" },
        .CoE = (HCI_Field_t[7]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Connection_Interval_Min",                        .octets =   2 },
          { .name = "Connection_Interval_Max",                        .octets =   2 },
          { .name = "Max_Latency",                                    .octets =   2 },
          { .name = "Supervision_Timeout",                            .octets =   2 },
          { .name = "Min_CE_Length",                                  .octets =   2 },
          { .name = "Max_CE_Length",                                  .octets =   2 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_LE_Connection_Update_Complete" },
    },
  },

  { .title = "LE Connection Update Complete event", .iscmd = 0, .section = "7.7.65.3", .version = "4.0", .BREDR = "E", .LE = "C.3",
    .summary = "The HCI_LE_Connection_Update_Complete event indicates the completion "
               "of the process to change the connection parameters.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Connection_Update_Complete", .code = 0x003e, .subevent = 0x03, .numcoe = 6, .numret = 0,  .Cond = { "E", "C.3" },
        .CoE = (HCI_Field_t[6]) {
          { .name = "Subevent_Code",                                  .octets =   1 },
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Connection_Interval",                            .octets =   2 },
          { .name = "Peripheral_Latency",                             .octets =   2 },
          { .name = "Supervision_Timeout",                            .octets =   2 },
        },
      },
    },
  },

  { .title = "LE Connectionless IQ Report event", .iscmd = 0, .section = "7.7.65.21", .version = "5.1", .BREDR = "E", .LE = "C.28",
    .summary = "The HCI_LE_Connectionless_IQ_Report event reports IQ information from "
               "the Constant Tone Extension of a received advertising packet.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Connectionless_IQ_Report", .code = 0x003e, .subevent = 0x15, .numcoe = 12, .numret = 0,  .Cond = { "E", "C.28" },
        .CoE = (HCI_Field_t[12]) {
          { .name = "Subevent_Code",                                  .octets =   1 },
          { .name = "Sync_Handle",                                    .octets =   2 },
          { .name = "Channel_Index",                                  .octets =   1 },
          { .name = "RSSI",                                           .octets =   2 },
          { .name = "RSSI_Antenna_ID",                                .octets =   1 },
          { .name = "CTE_Type",                                       .octets =   1 },
          { .name = "Slot_Durations",                                 .octets =   1 },
          { .name = "Packet_Status",                                  .octets =   1 },
          { .name = "Periodic_Event_Counter",                         .octets =   2 },
          { .name = "Sample_Count",                                   .octets =   1 },
          { .name = "I_Sample",                                       .octets =   1, .ref2size = "Sample_Count" },
          { .name = "Q_Sample",                                       .octets =   1, .ref2size = "Sample_Count" },
        },
      },
    },
  },

  { .title = "LE Create BIG command", .iscmd = 1, .OGF = 0x08, .section = "7.8.103", .version = "5.2", .BREDR = "E", .LE = "C.41",
    .summary = "The HCI_LE_Create_BIG command creates one or more BISes of a BIG.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Create_BIG", .OCF = 0x0068, .numcoe = 12, .numret = 0, .octet = 42, .bit = 5, .Cond = { "E", "C.41" },
        .CoE = (HCI_Field_t[12]) {
          { .name = "BIG_Handle",                                     .octets =   1 },
          { .name = "Advertising_Handle",                             .octets =   1 },
          { .name = "Num_BIS",                                        .octets =   1 },
          { .name = "SDU_Interval",                                   .octets =   3 },
          { .name = "Max_SDU",                                        .octets =   2 },
          { .name = "Max_Transport_Latency",                          .octets =   2 },
          { .name = "RTN",                                            .octets =   1 },
          { .name = "PHY",                                            .octets =   1 },
          { .name = "Packing",                                        .octets =   1 },
          { .name = "Framing",                                        .octets =   1 },
          { .name = "Encryption",                                     .octets =   1 },
          { .name = "Broadcast_Code",                                 .octets =  16 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_LE_Create_BIG_Complete" },
    },
  },

  { .title = "LE Create BIG Complete event", .iscmd = 0, .section = "7.7.65.27", .version = "5.2", .BREDR = "E", .LE = "C.41",
    .summary = "The HCI_LE_Create_BIG_Complete event indicates that the Controller "
               "completed an attempt to create the BISes that were requested by the "
               "Host.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Create_BIG_Complete", .code = 0x003e, .subevent = 0x1b, .numcoe = 14, .numret = 0,  .Cond = { "E", "C.41" },
        .CoE = (HCI_Field_t[14]) {
          { .name = "Subevent_Code",                                  .octets =   1 },
          { .name = "Status",                                         .octets =   1 },
          { .name = "BIG_Handle",                                     .octets =   1 },
          { .name = "BIG_Sync_Delay",                                 .octets =   3 },
          { .name = "Transport_Latency_BIG",                          .octets =   3 },
          { .name = "PHY",                                            .octets =   1 },
          { .name = "NSE",                                            .octets =   1 },
          { .name = "BN",                                             .octets =   1 },
          { .name = "PTO",                                            .octets =   1 },
          { .name = "IRC",                                            .octets =   1 },
          { .name = "Max_PDU",                                        .octets =   2 },
          { .name = "ISO_Interval",                                   .octets =   2 },
          { .name = "Num_BIS",                                        .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2, .ref2size = "Num_BIS" },
        },
      },
    },
  },

  { .title = "LE Create BIG Test command", .iscmd = 1, .OGF = 0x08, .section = "7.8.104", .version = "5.2", .BREDR = "E", .LE = "C.41",
    .summary = "The HCI_LE_Create_BIG_Test command is used to create one or more "
               "BISes of a BIG for testing purposes.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Create_BIG_Test", .OCF = 0x0069, .numcoe = 16, .numret = 0, .octet = 42, .bit = 6, .Cond = { "E", "C.41" },
        .CoE = (HCI_Field_t[16]) {
          { .name = "BIG_Handle",                                     .octets =   1 },
          { .name = "Advertising_Handle",                             .octets =   1 },
          { .name = "Num_BIS",                                        .octets =   1 },
          { .name = "SDU_Interval",                                   .octets =   3 },
          { .name = "ISO_Interval",                                   .octets =   2 },
          { .name = "NSE",                                            .octets =   1 },
          { .name = "Max_SDU",                                        .octets =   2 },
          { .name = "Max_PDU",                                        .octets =   2 },
          { .name = "PHY",                                            .octets =   1 },
          { .name = "Packing",                                        .octets =   1 },
          { .name = "Framing",                                        .octets =   1 },
          { .name = "BN",                                             .octets =   1 },
          { .name = "IRC",                                            .octets =   1 },
          { .name = "PTO",                                            .octets =   1 },
          { .name = "Encryption",                                     .octets =   1 },
          { .name = "Broadcast_Code",                                 .octets =  16 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_LE_Create_BIG_Complete" },
    },
  },

  { .title = "LE Create CIS command", .iscmd = 1, .OGF = 0x08, .section = "7.8.99", .version = "5.2", .BREDR = "E", .LE = "C.39",
    .summary = "The HCI_LE_Create_CIS command is used by the Central's Host to create "
               "one or more CISes.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Create_CIS", .OCF = 0x0064, .numcoe = 3, .numret = 0, .octet = 42, .bit = 1, .Cond = { "E", "C.39" },
        .CoE = (HCI_Field_t[3]) {
          { .name = "CIS_Count",                                      .octets =   1 },
          { .name = "CIS_Connection_Handle",                          .octets =   2, .ref2size = "CIS_Count" },
          { .name = "ACL_Connection_Handle",                          .octets =   2, .ref2size = "CIS_Count" },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_LE_CIS_Established" },
    },
  },

  { .title = "LE Create Connection Cancel command", .iscmd = 1, .OGF = 0x08, .section = "7.8.13", .version = "4.0", .BREDR = "E", .LE = "C.94",
    .summary = "The HCI_LE_Create_Connection_Cancel command is used to cancel an "
               "ongoing HCI_LE_Create_Connection command.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Create_Connection_Cancel", .OCF = 0x000e, .numcoe = 0, .numret = 1, .octet = 26, .bit = 5, .Cond = { "E", "C.94" },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 3, .Events = (HCI_Event_t[3]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_LE_Connection_Complete" },
      { .name = "HCI_LE_Enhanced_Connection_Complete [v2]" },
    },
  },

  { .title = "LE Create Connection command", .iscmd = 1, .OGF = 0x08, .section = "7.8.12", .version = "4.0", .BREDR = "E", .LE = "C.59",
    .summary = "The HCI_LE_Create_Connection command is used to create a new "
               "connection.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Create_Connection", .OCF = 0x000d, .numcoe = 12, .numret = 0, .octet = 26, .bit = 4, .Cond = { "E", "C.59" },
        .CoE = (HCI_Field_t[12]) {
          { .name = "LE_Scan_Interval",                               .octets =   2 },
          { .name = "LE_Scan_Window",                                 .octets =   2 },
          { .name = "Initiator_Filter_Policy",                        .octets =   1 },
          { .name = "Peer_Address_Type",                              .octets =   1 },
          { .name = "Peer_Address",                                   .octets =   6 },
          { .name = "Own_Address_Type",                               .octets =   1 },
          { .name = "Connection_Interval_Min",                        .octets =   2 },
          { .name = "Connection_Interval_Max",                        .octets =   2 },
          { .name = "Max_Latency",                                    .octets =   2 },
          { .name = "Supervision_Timeout",                            .octets =   2 },
          { .name = "Min_CE_Length",                                  .octets =   2 },
          { .name = "Max_CE_Length",                                  .octets =   2 },
        },
      },
    },
    .numevts = 4, .Events = (HCI_Event_t[4]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_LE_Connection_Complete" },
      { .name = "HCI_LE_Enhanced_Connection_Complete [v2]" },
      { .name = "HCI_LE_Channel_Selection_Algorithm" },
    },
  },

  { .title = "LE CTE Request Failed event", .iscmd = 0, .section = "7.7.65.23", .version = "5.1", .BREDR = "E", .LE = "C.25",
    .summary = "The HCI_LE_CTE_Request_Failed event indicates a problem with a "
               "request generated by an HCI_LE_Connection_CTE_Request_Enable command "
               "for a peer device to send Constant Tone Extensions.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_CTE_Request_Failed", .code = 0x003e, .subevent = 0x17, .numcoe = 3, .numret = 0,  .Cond = { "E", "C.25" },
        .CoE = (HCI_Field_t[3]) {
          { .name = "Subevent_Code",                                  .octets =   1 },
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
        },
      },
    },
  },

  { .title = "LE Data Length Change event", .iscmd = 0, .section = "7.7.65.7", .version = "4.2", .BREDR = "E", .LE = "C.8",
    .summary = "The HCI_LE_Data_Length_Change event is used to indicate a change in "
               "the maximum packet sizes by the Link Layer.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Data_Length_Change", .code = 0x003e, .subevent = 0x07, .numcoe = 6, .numret = 0,  .Cond = { "E", "C.8" },
        .CoE = (HCI_Field_t[6]) {
          { .name = "Subevent_Code",                                  .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Max_TX_Octets",                                  .octets =   2 },
          { .name = "Max_TX_Time",                                    .octets =   2 },
          { .name = "Max_RX_Octets",                                  .octets =   2 },
          { .name = "Max_RX_Time",                                    .octets =   2 },
        },
      },
    },
  },

  { .title = "LE Directed Advertising Report event", .iscmd = 0, .section = "7.7.65.11", .version = "4.2", .BREDR = "E", .LE = "C.63",
    .summary = "The HCI_LE_Directed_Advertising_Report event indicates that directed "
               "advertisements have been received where the advertiser is using a "
               "resolvable private address for the TargetA field in the "
               "ADV_DIRECT_IND PDU and the scanning filter policy is set to send this "
               "event to the Host.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Directed_Advertising_Report", .code = 0x003e, .subevent = 0x0b, .numcoe = 8, .numret = 0,  .Cond = { "E", "C.63" },
        .CoE = (HCI_Field_t[8]) {
          { .name = "Subevent_Code",                                  .octets =   1 },
          { .name = "Num_Reports",                                    .octets =   1 },
          { .name = "Event_Type",                                     .octets =   1, .ref2size = "Num_Reports" },
          { .name = "Address_Type",                                   .octets =   1, .ref2size = "Num_Reports" },
          { .name = "Address",                                        .octets =   6, .ref2size = "Num_Reports" },
          { .name = "Direct_Address_Type",                            .octets =   1, .ref2size = "Num_Reports" },
          { .name = "Direct_Address",                                 .octets =   6, .ref2size = "Num_Reports" },
          { .name = "RSSI",                                           .octets =   1, .ref2size = "Num_Reports" },
        },
      },
    },
  },

  { .title = "LE Enable Encryption command", .iscmd = 1, .OGF = 0x08, .section = "7.8.24", .version = "4.0", .BREDR = "E", .LE = "C.60",
    .summary = "The HCI_LE_Enable_Encryption command is used to enable link level "
               "encryption.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Enable_Encryption", .OCF = 0x0019, .numcoe = 4, .numret = 0, .octet = 28, .bit = 0, .Cond = { "E", "C.60" },
        .CoE = (HCI_Field_t[4]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Random_Number",                                  .octets =   8 },
          { .name = "Encrypted_Diversifier",                          .octets =   2 },
          { .name = "Long_Term_Key",                                  .octets =  16 },
        },
      },
    },
    .numevts = 3, .Events = (HCI_Event_t[3]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Encryption_Change [v2]" },
      { .name = "HCI_Encryption_Key_Refresh_Complete" },
    },
  },

  { .title = "LE Encrypt command", .iscmd = 1, .OGF = 0x08, .section = "7.8.22", .version = "4.0", .BREDR = "E", .LE = "C.4",
    .summary = "The HCI_LE_Encrypt command will encrypt a block of unencrypted data "
               "against a key and generate a block of encrypted data.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Encrypt", .OCF = 0x0017, .numcoe = 2, .numret = 2, .octet = 27, .bit = 6, .Cond = { "E", "C.4" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Key",                                            .octets =  16 },
          { .name = "Plaintext_Data",                                 .octets =  16 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Encrypted_Data",                                 .octets =  16 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Enhanced Connection Complete event", .iscmd = 0, .section = "7.7.65.10", .version = "4.2 5.4", .BREDR = "E", .LE = "[v1] C.24 [v2] C.69",
    .summary = "The HCI_LE_Enhanced_Connection_Complete event indicates to the Host "
               "that a new connection has been created. This event contains the "
               "additional parameters of the local and peer resolvable private "
               "addresses.",

    .numinst = 2, .Inst = (HCI_Inst_t[2]) {
      { .name = "HCI_LE_Enhanced_Connection_Complete [v2]", .code = 0x003e, .subevent = 0x29, .numcoe = 14, .numret = 0,  .Cond = { "E", "C.69" },
        .CoE = (HCI_Field_t[14]) {
          { .name = "Subevent_Code",                                  .octets =   1 },
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Role",                                           .octets =   1 },
          { .name = "Peer_Address_Type",                              .octets =   1 },
          { .name = "Peer_Address",                                   .octets =   6 },
          { .name = "Local_Resolvable_Private_Address",               .octets =   6 },
          { .name = "Peer_Resolvable_Private_Address",                .octets =   6 },
          { .name = "Connection_Interval",                            .octets =   2 },
          { .name = "Peripheral_Latency",                             .octets =   2 },
          { .name = "Supervision_Timeout",                            .octets =   2 },
          { .name = "Central_Clock_Accuracy",                         .octets =   1 },
          { .name = "Advertising_Handle",                             .octets =   1 },
          { .name = "Sync_Handle",                                    .octets =   2 },
        },
      },
      { .name = "HCI_LE_Enhanced_Connection_Complete [v1]", .code = 0x003e, .subevent = 0x0a, .numcoe = 12, .numret = 0,  .Cond = { "E", "C.24" },
        .CoE = (HCI_Field_t[12]) {
          { .name = "Subevent_Code",                                  .octets =   1 },
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Role",                                           .octets =   1 },
          { .name = "Peer_Address_Type",                              .octets =   1 },
          { .name = "Peer_Address",                                   .octets =   6 },
          { .name = "Local_Resolvable_Private_Address",               .octets =   6 },
          { .name = "Peer_Resolvable_Private_Address",                .octets =   6 },
          { .name = "Connection_Interval",                            .octets =   2 },
          { .name = "Peripheral_Latency",                             .octets =   2 },
          { .name = "Supervision_Timeout",                            .octets =   2 },
          { .name = "Central_Clock_Accuracy",                         .octets =   1 },
        },
      },
    },
  },

  { .title = "LE Enhanced Read Transmit Power Level command", .iscmd = 1, .OGF = 0x08, .section = "7.8.117", .version = "5.2", .BREDR = "E", .LE = "C.51",
    .summary = "The HCI_LE_Enhanced_Read_Transmit_Power_Level command is used to read "
               "the current and maximum transmit power levels used by the local "
               "Controller on a specified PHY on an ACL connection.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Enhanced_Read_Transmit_Power_Level", .OCF = 0x0076, .numcoe = 2, .numret = 5, .octet = 44, .bit = 3, .Cond = { "E", "C.51" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "PHY",                                            .octets =   1 },
        },
        .Ret = (HCI_Field_t[5]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "PHY",                                            .octets =   1 },
          { .name = "Current_TX_Power_Level",                         .octets =   1 },
          { .name = "Max_TX_Power_Level",                             .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Extended Advertising Report event", .iscmd = 0, .section = "7.7.65.13", .version = "5.0", .BREDR = "E", .LE = "C.19",
    .summary = "The HCI_LE_Extended_Advertising_Report event indicates that an "
               "advertising packet has been received.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Extended_Advertising_Report", .code = 0x003e, .subevent = 0x0d, .numcoe = 15, .numret = 0,  .Cond = { "E", "C.19" },
        .CoE = (HCI_Field_t[15]) {
          { .name = "Subevent_Code",                                  .octets =   1 },
          { .name = "Num_Reports",                                    .octets =   1 },
          { .name = "Event_Type",                                     .octets =   2, .ref2size = "Num_Reports" },
          { .name = "Address_Type",                                   .octets =   1, .ref2size = "Num_Reports" },
          { .name = "Address",                                        .octets =   6, .ref2size = "Num_Reports" },
          { .name = "Primary_PHY",                                    .octets =   1, .ref2size = "Num_Reports" },
          { .name = "Secondary_PHY",                                  .octets =   1, .ref2size = "Num_Reports" },
          { .name = "Advertising_SID",                                .octets =   1, .ref2size = "Num_Reports" },
          { .name = "TX_Power",                                       .octets =   1, .ref2size = "Num_Reports" },
          { .name = "RSSI",                                           .octets =   1, .ref2size = "Num_Reports" },
          { .name = "Periodic_Advertising_Interval",                  .octets =   2, .ref2size = "Num_Reports" },
          { .name = "Direct_Address_Type",                            .octets =   1, .ref2size = "Num_Reports" },
          { .name = "Direct_Address",                                 .octets =   6, .ref2size = "Num_Reports" },
          { .name = "Data_Length",                                    .octets =   1, .ref2size = "Num_Reports" },
          { .name = "Data",                                                         .ref2size = "Data_Length" },
        },
      },
    },
  },

  { .title = "LE Extended Create Connection command", .iscmd = 1, .OGF = 0x08, .section = "7.8.66", .version = "5.0 5.4", .BREDR = "E", .LE = "[v1] C.20 [v2] C.67",
    .summary = "The LE Extended Create Connection Command is used to create a new "
               "connection supporting different initiating PHYs and to initiate a "
               "connection with a synchronized device.",

    .numinst = 2, .Inst = (HCI_Inst_t[2]) {
      { .name = "HCI_LE_Extended_Create_Connection [v2]", .OCF = 0x0085, .numcoe = 15, .numret = 0, .octet = 47, .bit = 0, .Cond = { "E", "C.67" },
        .CoE = (HCI_Field_t[15]) {
          { .name = "Advertising_Handle",                             .octets =   1 },
          { .name = "Subevent",                                       .octets =   1 },
          { .name = "Initiator_Filter_Policy",                        .octets =   1 },
          { .name = "Own_Address_Type",                               .octets =   1 },
          { .name = "Peer_Address_Type",                              .octets =   1 },
          { .name = "Peer_Address",                                   .octets =   6 },
          { .name = "Initiating_PHYs",                                .octets =   1 },
          { .name = "Scan_Interval",                                                .ref2size = "Initiating_PHYs" },
          { .name = "Scan_Window",                                                  .ref2size = "Initiating_PHYs" },
          { .name = "Connection_Interval_Min",                                      .ref2size = "Initiating_PHYs" },
          { .name = "Connection_Interval_Max",                                      .ref2size = "Initiating_PHYs" },
          { .name = "Max_Latency",                                                  .ref2size = "Initiating_PHYs" },
          { .name = "Supervision_Timeout",                                          .ref2size = "Initiating_PHYs" },
          { .name = "Min_CE_Length",                                                .ref2size = "Initiating_PHYs" },
          { .name = "Max_CE_Length",                                                .ref2size = "Initiating_PHYs" },
        },
      },
      { .name = "HCI_LE_Extended_Create_Connection [v1]", .OCF = 0x0043, .numcoe = 13, .numret = 0, .octet = 37, .bit = 7, .Cond = { "E", "C.20" },
        .CoE = (HCI_Field_t[13]) {
          { .name = "Initiator_Filter_Policy",                        .octets =   1 },
          { .name = "Own_Address_Type",                               .octets =   1 },
          { .name = "Peer_Address_Type",                              .octets =   1 },
          { .name = "Peer_Address",                                   .octets =   6 },
          { .name = "Initiating_PHYs",                                .octets =   1 },
          { .name = "Scan_Interval",                                                .ref2size = "Initiating_PHYs" },
          { .name = "Scan_Window",                                                  .ref2size = "Initiating_PHYs" },
          { .name = "Connection_Interval_Min",                                      .ref2size = "Initiating_PHYs" },
          { .name = "Connection_Interval_Max",                                      .ref2size = "Initiating_PHYs" },
          { .name = "Max_Latency",                                                  .ref2size = "Initiating_PHYs" },
          { .name = "Supervision_Timeout",                                          .ref2size = "Initiating_PHYs" },
          { .name = "Min_CE_Length",                                                .ref2size = "Initiating_PHYs" },
          { .name = "Max_CE_Length",                                                .ref2size = "Initiating_PHYs" },
        },
      },
    },
    .numevts = 3, .Events = (HCI_Event_t[3]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_LE_Enhanced_Connection_Complete [v2]" },
      { .name = "HCI_LE_Channel_Selection_Algorithm" },
    },
  },

  { .title = "LE Generate DHKey command", .iscmd = 1, .OGF = 0x08, .section = "7.8.37", .version = "4.2 5.1", .BREDR = "E", .LE = "[v1] C.99 [v2] O",
    .summary = "The HCI_LE_Generate_DHKey command is used to initiate generation of a "
               "Diffie-Hellman key in the Controller for use over the LE transport.",

    .numinst = 2, .Inst = (HCI_Inst_t[2]) {
      { .name = "HCI_LE_Generate_DHKey [v2]", .OCF = 0x005e, .numcoe = 3, .numret = 0, .octet = 41, .bit = 2, .Cond = { "E", "O" },
        .CoE = (HCI_Field_t[3]) {
          { .name = "Key_X_Coordinate",                               .octets =  32 },
          { .name = "Key_Y_Coordinate",                               .octets =  32 },
          { .name = "Key_Type",                                       .octets =   1 },
        },
      },
      { .name = "HCI_LE_Generate_DHKey [v1]", .OCF = 0x0026, .numcoe = 2, .numret = 0, .octet = 34, .bit = 2, .Cond = { "E", "C.99" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Key_X_Coordinate",                               .octets =  32 },
          { .name = "Key_Y_Coordinate",                               .octets =  32 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_LE_Generate_DHKey_Complete" },
    },
  },

  { .title = "LE Generate DHKey Complete event", .iscmd = 0, .section = "7.7.65.9", .version = "4.2", .BREDR = "E", .LE = "O",
    .summary = "The HCI_LE_Generate_DHKey_Complete event indicates that LE "
               "Diffie-Hellman key generation has been completed by the Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Generate_DHKey_Complete", .code = 0x003e, .subevent = 0x09, .numcoe = 3, .numret = 0,  .Cond = { "E", "O" },
        .CoE = (HCI_Field_t[3]) {
          { .name = "Subevent_Code",                                  .octets =   1 },
          { .name = "Status",                                         .octets =   1 },
          { .name = "DH_Key",                                         .octets =  32 },
        },
      },
    },
  },

  { .title = "LE ISO Read Test Counters command", .iscmd = 1, .OGF = 0x08, .section = "7.8.113", .version = "5.2", .BREDR = "E", .LE = "C.46",
    .summary = "The HCI_LE_ISO_Read_Test_Counters command reads the test counters in "
               "the Controller which is configured in ISO Receive Test mode.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_ISO_Read_Test_Counters", .OCF = 0x0072, .numcoe = 1, .numret = 5, .octet = 43, .bit = 7, .Cond = { "E", "C.46" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Connection_Handle",                              .octets =   2 },
        },
        .Ret = (HCI_Field_t[5]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Received_SDU_Count",                             .octets =   4 },
          { .name = "Missed_SDU_Count",                               .octets =   4 },
          { .name = "Failed_SDU_Count",                               .octets =   4 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE ISO Receive Test command", .iscmd = 1, .OGF = 0x08, .section = "7.8.112", .version = "5.2", .BREDR = "E", .LE = "C.46",
    .summary = "The HCI_LE_ISO_Receive_Test command configures a Link Layer to "
               "receive test payloads from an established CIS or a synchronized BIS.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_ISO_Receive_Test", .OCF = 0x0071, .numcoe = 2, .numret = 2, .octet = 43, .bit = 6, .Cond = { "E", "C.46" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Payload_Type",                                   .octets =   1 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE ISO Test End command", .iscmd = 1, .OGF = 0x08, .section = "7.8.114", .version = "5.2", .BREDR = "E", .LE = "C.47",
    .summary = "The HCI_LE_ISO_Test_End command terminates the ISO Transmit and/or "
               "Receive Test mode.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_ISO_Test_End", .OCF = 0x0073, .numcoe = 1, .numret = 5, .octet = 44, .bit = 0, .Cond = { "E", "C.47" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Connection_Handle",                              .octets =   2 },
        },
        .Ret = (HCI_Field_t[5]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Received_SDU_Count",                             .octets =   4 },
          { .name = "Missed_SDU_Count",                               .octets =   4 },
          { .name = "Failed_SDU_Count",                               .octets =   4 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE ISO Transmit Test command", .iscmd = 1, .OGF = 0x08, .section = "7.8.111", .version = "5.2", .BREDR = "E", .LE = "C.45",
    .summary = "The HCI_LE_ISO_Transmit_Test command configures an established CIS or "
               "BIS to transmit test payloads that are generated by the Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_ISO_Transmit_Test", .OCF = 0x0070, .numcoe = 2, .numret = 2, .octet = 43, .bit = 5, .Cond = { "E", "C.45" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Payload_Type",                                   .octets =   1 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Long Term Key Request event", .iscmd = 0, .section = "7.7.65.5", .version = "4.0", .BREDR = "E", .LE = "C.61",
    .summary = "The HCI_LE_Long_Term_Key_Request event indicates that a Long Term Key "
               "is required for a connection.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Long_Term_Key_Request", .code = 0x003e, .subevent = 0x05, .numcoe = 4, .numret = 0,  .Cond = { "E", "C.61" },
        .CoE = (HCI_Field_t[4]) {
          { .name = "Subevent_Code",                                  .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Random_Number",                                  .octets =   8 },
          { .name = "Encrypted_Diversifier",                          .octets =   2 },
        },
      },
    },
  },

  { .title = "LE Long Term Key Request Negative Reply command", .iscmd = 1, .OGF = 0x08, .section = "7.8.26", .version = "4.0", .BREDR = "E", .LE = "C.61",
    .summary = "The HCI_LE_Long_Term_Key_Request_Negative_Reply command is used to "
               "reply to an HCI_LE_Long_Term_Key_Request event and indicates that the "
               "Host does not have a Long Term Key for that connection.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Long_Term_Key_Request_Negative_Reply", .OCF = 0x001b, .numcoe = 1, .numret = 2, .octet = 28, .bit = 2, .Cond = { "E", "C.61" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Connection_Handle",                              .octets =   2 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Long Term Key Request Reply command", .iscmd = 1, .OGF = 0x08, .section = "7.8.25", .version = "4.0", .BREDR = "E", .LE = "C.61",
    .summary = "The HCI_LE_Long_Term_Key_Request_Reply command is used to reply to an "
               "HCI_LE_Long_Term_Key_Request event and includes the Long Term Key "
               "stored in the Host for that connection.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Long_Term_Key_Request_Reply", .OCF = 0x001a, .numcoe = 2, .numret = 2, .octet = 28, .bit = 1, .Cond = { "E", "C.61" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Long_Term_Key",                                  .octets =  16 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Modify Sleep Clock Accuracy command", .iscmd = 1, .OGF = 0x08, .section = "7.8.94", .version = "5.1", .BREDR = "E", .LE = "C.37",
    .summary = "The HCI_LE_Modify_Sleep_Clock_Accuracy command requests the "
               "Controller changes its sleep clock accuracy for testing purposes.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Modify_Sleep_Clock_Accuracy", .OCF = 0x005f, .numcoe = 1, .numret = 1, .octet = 41, .bit = 4, .Cond = { "E", "C.37" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Action",                                         .octets =   1 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Path Loss Threshold event", .iscmd = 0, .section = "7.7.65.32", .version = "5.2", .BREDR = "E", .LE = "C.52",
    .summary = "The HCI_LE_Path_Loss_Threshold event is used to report a path loss "
               "threshold crossing on an ACL connection.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Path_Loss_Threshold", .code = 0x003e, .subevent = 0x20, .numcoe = 4, .numret = 0,  .Cond = { "E", "C.52" },
        .CoE = (HCI_Field_t[4]) {
          { .name = "Subevent_Code",                                  .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Current_Path_Loss",                              .octets =   1 },
          { .name = "Zone_Entered",                                   .octets =   1 },
        },
      },
    },
  },

  { .title = "LE Periodic Advertising Create Sync Cancel command", .iscmd = 1, .OGF = 0x08, .section = "7.8.68", .version = "5.0", .BREDR = "E", .LE = "C.16",
    .summary = "The HCI_LE_Periodic_Advertising_Create_Sync_Cancel command is used to "
               "cancel a pending HCI_LE_Periodic_Advertising_Create_Sync command.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Periodic_Advertising_Create_Sync_Cancel", .OCF = 0x0045, .numcoe = 0, .numret = 1, .octet = 38, .bit = 1, .Cond = { "E", "C.16" },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_LE_Periodic_Advertising_Sync_Established [v2]" },
    },
  },

  { .title = "LE Periodic Advertising Create Sync command", .iscmd = 1, .OGF = 0x08, .section = "7.8.67", .version = "5.0", .BREDR = "E", .LE = "C.16",
    .summary = "The HCI_LE_Periodic_Advertising_Create_Sync command is used to start "
               "receiving periodic advertising packets from an advertiser.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Periodic_Advertising_Create_Sync", .OCF = 0x0044, .numcoe = 7, .numret = 0, .octet = 38, .bit = 0, .Cond = { "E", "C.16" },
        .CoE = (HCI_Field_t[7]) {
          { .name = "Options",                                        .octets =   1 },
          { .name = "Advertising_SID",                                .octets =   1 },
          { .name = "Advertiser_Address_Type",                        .octets =   1 },
          { .name = "Advertiser_Address",                             .octets =   6 },
          { .name = "Skip",                                           .octets =   2 },
          { .name = "Sync_Timeout",                                   .octets =   2 },
          { .name = "Sync_CTE_Type",                                  .octets =   1 },
        },
      },
    },
    .numevts = 3, .Events = (HCI_Event_t[3]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_LE_Periodic_Advertising_Sync_Established [v2]" },
      { .name = "HCI_LE_Periodic_Advertising_Report [v2]" },
    },
  },

  { .title = "LE Periodic Advertising Report event", .iscmd = 0, .section = "7.7.65.15", .version = "5.0 5.4", .BREDR = "E", .LE = "[v1] C.21 [v2] C.68",
    .summary = "The HCI_LE_Periodic_Advertising_Report event indicates that a "
               "periodic advertising packet has been received.",

    .numinst = 2, .Inst = (HCI_Inst_t[2]) {
      { .name = "HCI_LE_Periodic_Advertising_Report [v2]", .code = 0x003e, .subevent = 0x25, .numcoe = 10, .numret = 0,  .Cond = { "E", "C.68" },
        .CoE = (HCI_Field_t[10]) {
          { .name = "Subevent_Code",                                  .octets =   1 },
          { .name = "Sync_Handle",                                    .octets =   2 },
          { .name = "TX_Power",                                       .octets =   1 },
          { .name = "RSSI",                                           .octets =   1 },
          { .name = "CTE_Type",                                       .octets =   1 },
          { .name = "Periodic_Event_Counter",                         .octets =   2 },
          { .name = "Subevent",                                       .octets =   1 },
          { .name = "Data_Status",                                    .octets =   1 },
          { .name = "Data_Length",                                    .octets =   1 },
          { .name = "Data",                                                         .ref2size = "Data_Length" },
        },
      },
      { .name = "HCI_LE_Periodic_Advertising_Report [v1]", .code = 0x003e, .subevent = 0x0f, .numcoe = 8, .numret = 0,  .Cond = { "E", "C.21" },
        .CoE = (HCI_Field_t[8]) {
          { .name = "Subevent_Code",                                  .octets =   1 },
          { .name = "Sync_Handle",                                    .octets =   2 },
          { .name = "TX_Power",                                       .octets =   1 },
          { .name = "RSSI",                                           .octets =   1 },
          { .name = "CTE_Type",                                       .octets =   1 },
          { .name = "Data_Status",                                    .octets =   1 },
          { .name = "Data_Length",                                    .octets =   1 },
          { .name = "Data",                                                         .ref2size = "Data_Length" },
        },
      },
    },
  },

  { .title = "LE Periodic Advertising Set Info Transfer command", .iscmd = 1, .OGF = 0x08, .section = "7.8.90", .version = "5.1", .BREDR = "E", .LE = "C.34",
    .summary = "The HCI_LE_Periodic_Advertising_Set_Info_Transfer command is used to "
               "send periodic advertising synchronization information, describing "
               "periodic advertising events that the Controller is transmitting, to a "
               "connected Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Periodic_Advertising_Set_Info_Transfer", .OCF = 0x005b, .numcoe = 3, .numret = 2, .octet = 40, .bit = 7, .Cond = { "E", "C.34" },
        .CoE = (HCI_Field_t[3]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Service_Data",                                   .octets =   2 },
          { .name = "Advertising_Handle",                             .octets =   1 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Periodic Advertising Response Report event", .iscmd = 0, .section = "7.7.65.37", .version = "5.4", .BREDR = "E", .LE = "C.67",
    .summary = "The HCI_LE_Periodic_Advertising_Response_Report event is used to "
               "report response data to a Host.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Periodic_Advertising_Response_Report", .code = 0x003e, .numcoe = 12, .numret = 0,  .Cond = { "E", "C.67" },
        .CoE = (HCI_Field_t[12]) {
          { .name = "Subevent_Code",                                  .octets =   1 },
          { .name = "Advertising_Handle",                             .octets =   1 },
          { .name = "Subevent",                                       .octets =   1 },
          { .name = "Tx_Status",                                      .octets =   1 },
          { .name = "Num_Responses",                                  .octets =   1 },
          { .name = "Tx_Power",                                       .octets =   1, .ref2size = "Num_Responses" },
          { .name = "RSSI",                                           .octets =   1, .ref2size = "Num_Responses" },
          { .name = "CTE_Type",                                       .octets =   1, .ref2size = "Num_Responses" },
          { .name = "Response_Slot",                                  .octets =   1, .ref2size = "Num_Responses" },
          { .name = "Data_Status",                                    .octets =   1, .ref2size = "Num_Responses" },
          { .name = "Data_Length",                                    .octets =   1, .ref2size = "Num_Responses" },
          { .name = "Data",                                                         .ref2size = "Data_Length" },
        },
      },
    },
  },

  { .title = "LE Periodic Advertising Subevent Data Request event", .iscmd = 0, .section = "7.7.65.36", .version = "5.4", .BREDR = "E", .LE = "C.67",
    .summary = "The HCI_LE_Periodic_Advertising_Subevent_Data_Request event is used "
               "to request subevent data from a Host.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Periodic_Advertising_Subevent_Data_Request", .code = 0x003e, .subevent = 0x27, .numcoe = 4, .numret = 0,  .Cond = { "E", "C.67" },
        .CoE = (HCI_Field_t[4]) {
          { .name = "Subevent_Code",                                  .octets =   1 },
          { .name = "Advertising_Handle",                             .octets =   1 },
          { .name = "Subevent_Start",                                 .octets =   1 },
          { .name = "Subevent_Data_Count",                            .octets =   1 },
        },
      },
    },
  },

  { .title = "LE Periodic Advertising Sync Established event", .iscmd = 0, .section = "7.7.65.14", .version = "5.0 5.4", .BREDR = "E", .LE = "[v1] C.16 [v2] C.68",
    .summary = "The HCI_LE_Periodic_Advertising_Sync_Established event indicates that "
               "the Controller has started receiving periodic advertising packets "
               "from an advertiser.",

    .numinst = 2, .Inst = (HCI_Inst_t[2]) {
      { .name = "HCI_LE_Periodic_Advertising_Sync_Established [v2]", .code = 0x003e, .subevent = 0x24, .numcoe = 13, .numret = 0,  .Cond = { "E", "C.68" },
        .CoE = (HCI_Field_t[13]) {
          { .name = "Subevent_Code",                                  .octets =   1 },
          { .name = "Status",                                         .octets =   1 },
          { .name = "Sync_Handle",                                    .octets =   2 },
          { .name = "Advertising_SID",                                .octets =   1 },
          { .name = "Advertiser_Address_Type",                        .octets =   1 },
          { .name = "Advertiser_Address",                             .octets =   6 },
          { .name = "Advertiser_PHY",                                 .octets =   1 },
          { .name = "Periodic_Advertising_Interval",                  .octets =   2 },
          { .name = "Advertiser_Clock_Accuracy",                      .octets =   1 },
          { .name = "Num_Subevents",                                  .octets =   1 },
          { .name = "Subevent_Interval",                              .octets =   1 },
          { .name = "Response_Slot_Delay",                            .octets =   1 },
          { .name = "Response_Slot_Spacing",                          .octets =   1 },
        },
      },
      { .name = "HCI_LE_Periodic_Advertising_Sync_Established [v1]", .code = 0x003e, .subevent = 0x0e, .numcoe = 9, .numret = 0,  .Cond = { "E", "C.16" },
        .CoE = (HCI_Field_t[9]) {
          { .name = "Subevent_Code",                                  .octets =   1 },
          { .name = "Status",                                         .octets =   1 },
          { .name = "Sync_Handle",                                    .octets =   2 },
          { .name = "Advertising_SID",                                .octets =   1 },
          { .name = "Advertiser_Address_Type",                        .octets =   1 },
          { .name = "Advertiser_Address",                             .octets =   6 },
          { .name = "Advertiser_PHY",                                 .octets =   1 },
          { .name = "Periodic_Advertising_Interval",                  .octets =   2 },
          { .name = "Advertiser_Clock_Accuracy",                      .octets =   1 },
        },
      },
    },
  },

  { .title = "LE Periodic Advertising Sync Lost event", .iscmd = 0, .section = "7.7.65.16", .version = "5.0", .BREDR = "E", .LE = "C.21",
    .summary = "The HCI_LE_Periodic_Advertising_Sync_Lost event indicates the "
               "Controller has ended receiving a periodic advertising train.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Periodic_Advertising_Sync_Lost", .code = 0x003e, .subevent = 0x10, .numcoe = 2, .numret = 0,  .Cond = { "E", "C.21" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Subevent_Code",                                  .octets =   1 },
          { .name = "Sync_Handle",                                    .octets =   2 },
        },
      },
    },
  },

  { .title = "LE Periodic Advertising Sync Transfer command", .iscmd = 1, .OGF = 0x08, .section = "7.8.89", .version = "5.1", .BREDR = "E", .LE = "C.33",
    .summary = "The HCI_LE_Periodic_Advertising_Sync_Transfer command is used to send "
               "periodic advertising synchronization information to a connected "
               "Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Periodic_Advertising_Sync_Transfer", .OCF = 0x005a, .numcoe = 3, .numret = 2, .octet = 40, .bit = 6, .Cond = { "E", "C.33" },
        .CoE = (HCI_Field_t[3]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Service_Data",                                   .octets =   2 },
          { .name = "Sync_Handle",                                    .octets =   2 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Periodic Advertising Sync Transfer Received event", .iscmd = 0, .section = "7.7.65.24", .version = "5.1 5.4", .BREDR = "E", .LE = "[v1] C.35 [v2] C.68",
    .summary = "The HCI_LE_Periodic_Advertising_Sync_Transfer_Received event reports "
               "reception of periodic advertising synchronization information from a "
               "connected Controller.",

    .numinst = 2, .Inst = (HCI_Inst_t[2]) {
      { .name = "HCI_LE_Periodic_Advertising_Sync_Transfer_Received [v2]", .code = 0x003e, .subevent = 0x26, .numcoe = 15, .numret = 0,  .Cond = { "E", "C.68" },
        .CoE = (HCI_Field_t[15]) {
          { .name = "Subevent_Code",                                  .octets =   1 },
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Service_Data",                                   .octets =   2 },
          { .name = "Sync_Handle",                                    .octets =   2 },
          { .name = "Advertising_SID",                                .octets =   1 },
          { .name = "Advertiser_Address_Type",                        .octets =   1 },
          { .name = "Advertiser_Address",                             .octets =   6 },
          { .name = "Advertiser_PHY",                                 .octets =   1 },
          { .name = "Periodic_Advertising_Interval",                  .octets =   2 },
          { .name = "Advertiser_Clock_Accuracy",                      .octets =   1 },
          { .name = "Num_Subevents",                                  .octets =   1 },
          { .name = "Subevent_Interval",                              .octets =   1 },
          { .name = "Response_Slot_Delay",                            .octets =   1 },
          { .name = "Response_Slot_Spacing",                          .octets =   1 },
        },
      },
      { .name = "HCI_LE_Periodic_Advertising_Sync_Transfer_Received [v1]", .code = 0x003e, .subevent = 0x18, .numcoe = 11, .numret = 0,  .Cond = { "E", "C.35" },
        .CoE = (HCI_Field_t[11]) {
          { .name = "Subevent_Code",                                  .octets =   1 },
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Service_Data",                                   .octets =   2 },
          { .name = "Sync_Handle",                                    .octets =   2 },
          { .name = "Advertising_SID",                                .octets =   1 },
          { .name = "Advertiser_Address_Type",                        .octets =   1 },
          { .name = "Advertiser_Address",                             .octets =   6 },
          { .name = "Advertiser_PHY",                                 .octets =   1 },
          { .name = "Periodic_Advertising_Interval",                  .octets =   2 },
          { .name = "Advertiser_Clock_Accuracy",                      .octets =   1 },
        },
      },
    },
  },

  { .title = "LE Periodic Advertising Terminate Sync command", .iscmd = 1, .OGF = 0x08, .section = "7.8.69", .version = "5.0", .BREDR = "E", .LE = "C.21",
    .summary = "The HCI_LE_Periodic_Advertising_Terminate_Sync command is used to end "
               "receiving of a periodic advertising train.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Periodic_Advertising_Terminate_Sync", .OCF = 0x0046, .numcoe = 1, .numret = 1, .octet = 38, .bit = 2, .Cond = { "E", "C.21" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Sync_Handle",                                    .octets =   2 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE PHY Update Complete event", .iscmd = 0, .section = "7.7.65.12", .version = "5.0", .BREDR = "E", .LE = "C.11",
    .summary = "The HCI_LE_PHY_Update_Complete event is used to inform the Host of "
               "the current PHY.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_PHY_Update_Complete", .code = 0x003e, .subevent = 0x0c, .numcoe = 5, .numret = 0,  .Cond = { "E", "C.11" },
        .CoE = (HCI_Field_t[5]) {
          { .name = "Subevent_Code",                                  .octets =   1 },
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "TX_PHY",                                         .octets =   1 },
          { .name = "RX_PHY",                                         .octets =   1 },
        },
      },
    },
  },

  { .title = "LE Rand command", .iscmd = 1, .OGF = 0x08, .section = "7.8.23", .version = "4.0", .BREDR = "E", .LE = "C.4",
    .summary = "The HCI_LE_Rand command will generate a random number.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Rand", .OCF = 0x0018, .numcoe = 0, .numret = 2, .octet = 27, .bit = 7, .Cond = { "E", "C.4" },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Random_Number",                                  .octets =   8 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Read Advertising Physical Channel Tx Power command", .iscmd = 1, .OGF = 0x08, .section = "7.8.6", .version = "4.0", .BREDR = "E", .LE = "C.97",
    .summary = "The HCI_LE_Read_Advertising_Physical_Channel_Tx_Power command will "
               "read the transmit power level that will be used for advertising.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Read_Advertising_Channel_Tx_Power", .OCF = 0x0007, .numcoe = 0, .numret = 2, .octet = 25, .bit = 6, .Cond = { "E", "C.97" },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "TX_Power_Level",                                 .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Read Antenna Information command", .iscmd = 1, .OGF = 0x08, .section = "7.8.87", .version = "5.1", .BREDR = "E", .LE = "C.31",
    .summary = "The HCI_LE_Read_Antenna_Information command allows the Host to read "
               "the switching rates, the sampling rates, the number of antennae, and "
               "the maximum length of the Constant Tone Extension supported by the "
               "Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Read_Antenna_Information", .OCF = 0x0058, .numcoe = 0, .numret = 5, .octet = 40, .bit = 4, .Cond = { "E", "C.31" },
        .Ret = (HCI_Field_t[5]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Supported_Switching_Sampling_Rates",             .octets =   1 },
          { .name = "Num_Antennae",                                   .octets =   1 },
          { .name = "Max_Switching_Pattern_Length",                   .octets =   1 },
          { .name = "Max_CTE_Length",                                 .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Read Buffer Size command", .iscmd = 1, .OGF = 0x08, .section = "7.8.2", .version = "4.0", .BREDR = "E", .LE = "[v1] C.3 [v2] C.55",
    .summary = "The HCI_LE_Read_Buffer_Size command returns the size of the HCI "
               "buffers. These buffers are used by the LE Controller to buffer data "
               "that is to be transmitted.",

    .numinst = 2, .Inst = (HCI_Inst_t[2]) {
      { .name = "HCI_LE_Read_Buffer_Size [v2]", .OCF = 0x0060, .numcoe = 0, .numret = 5, .octet = 41, .bit = 5, .Cond = { "E", "C.55" },
        .Ret = (HCI_Field_t[5]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "LE_ACL_Data_Packet_Length",                      .octets =   2 },
          { .name = "Total_Num_LE_ACL_Data_Packets",                  .octets =   1 },
          { .name = "ISO_Data_Packet_Length",                         .octets =   2 },
          { .name = "Total_Num_ISO_Data_Packets",                     .octets =   1 },
        },
      },
      { .name = "HCI_LE_Read_Buffer_Size [v1]", .OCF = 0x0002, .numcoe = 0, .numret = 3, .octet = 25, .bit = 1, .Cond = { "E", "C.3" },
        .Ret = (HCI_Field_t[3]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "LE_ACL_Data_Packet_Length",                      .octets =   2 },
          { .name = "Total_Num_LE_ACL_Data_Packets",                  .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Read Channel Map command", .iscmd = 1, .OGF = 0x08, .section = "7.8.20", .version = "4.0", .BREDR = "E", .LE = "C.3",
    .summary = "The HCI_LE_Read_Channel_Map command will read the current state of "
               "the channel map for a connection.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Read_Channel_Map", .OCF = 0x0015, .numcoe = 1, .numret = 3, .octet = 27, .bit = 4, .Cond = { "E", "C.3" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Connection_Handle",                              .octets =   2 },
        },
        .Ret = (HCI_Field_t[3]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Channel_Map",                                    .octets =   5 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Read Filter Accept List Size command", .iscmd = 1, .OGF = 0x08, .section = "7.8.14", .version = "4.0", .BREDR = "E", .LE = "M",
    .summary = "The HCI_LE_Read_Filter_Accept_List_Size command will read the maximum "
               "number of Filter Accept List entries that this Controller supports.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Read_Filter_Accept_List_Size", .OCF = 0x000f, .numcoe = 0, .numret = 2, .octet = 26, .bit = 6, .Cond = { "E", "M" },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Filter_Accept_List_Size",                        .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Read ISO Link Quality command", .iscmd = 1, .OGF = 0x08, .section = "7.8.116", .version = "5.2", .BREDR = "E", .LE = "C.50",
    .summary = "The HCI_LE_Read_ISO_Link_Quality command returns the value of various "
               "counters related to link quality on an isochronous stream.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Read_ISO_Link_Quality", .OCF = 0x0075, .numcoe = 1, .numret = 9, .octet = 44, .bit = 2, .Cond = { "E", "C.50" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Connection_Handle",                              .octets =   2 },
        },
        .Ret = (HCI_Field_t[9]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "TX_UnACKed_Packets",                             .octets =   4 },
          { .name = "TX_Flushed_Packets",                             .octets =   4 },
          { .name = "TX_Last_Subevent_Packets",                       .octets =   4 },
          { .name = "Retransmitted_Packets",                          .octets =   4 },
          { .name = "CRC_Error_Packets",                              .octets =   4 },
          { .name = "RX_Unreceived_Packets",                          .octets =   4 },
          { .name = "Duplicate_Packets",                              .octets =   4 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Read ISO TX Sync command", .iscmd = 1, .OGF = 0x08, .section = "7.8.96", .version = "5.2", .BREDR = "E", .LE = "C.45",
    .summary = "The HCI_LE_Read_ISO_TX_Sync command is used to read the Time_Stamp "
               "and Time_Offset of a transmitted SDU.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Read_ISO_TX_Sync", .OCF = 0x0061, .numcoe = 1, .numret = 5, .octet = 41, .bit = 6, .Cond = { "E", "C.45" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Connection_Handle",                              .octets =   2 },
        },
        .Ret = (HCI_Field_t[5]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Packet_Sequence_Number",                         .octets =   2 },
          { .name = "TX_Time_Stamp",                                  .octets =   4 },
          { .name = "Time_Offset",                                    .octets =   3 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Read Local P-256 Public Key command", .iscmd = 1, .OGF = 0x08, .section = "7.8.36", .version = "4.2", .BREDR = "E", .LE = "O",
    .summary = "The HCI_LE_Read_Local_P-256_Public_Key command is used to return the "
               "local P-256 public key from the Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Read_Local_P-256_Public_Key", .OCF = 0x0025, .numcoe = 0, .numret = 0, .octet = 34, .bit = 1, .Cond = { "E", "O" },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Status" },
    },
  },

  { .title = "LE Read Local P-256 Public Key Complete event", .iscmd = 0, .section = "7.7.65.8", .version = "4.2", .BREDR = "E", .LE = "O",
    .summary = "The HCI_LE_Read_Local_P-256_Public_Key_Complete event is generated "
               "when local P-256 key generation is complete.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Read_Local_P-256_Public_Key_Complete", .code = 0x003e, .subevent = 0x08, .numcoe = 4, .numret = 0,  .Cond = { "E", "O" },
        .CoE = (HCI_Field_t[4]) {
          { .name = "Subevent_Code",                                  .octets =   1 },
          { .name = "Status",                                         .octets =   1 },
          { .name = "Key_X_Coordinate",                               .octets =  32 },
          { .name = "Key_Y_Coordinate",                               .octets =  32 },
        },
      },
    },
  },

  { .title = "LE Read Local Resolvable Address command", .iscmd = 1, .OGF = 0x08, .section = "7.8.43", .version = "4.2", .BREDR = "E", .LE = "C.10",
    .summary = "The HCI_LE_Read_Local_Resolvable_Address command is used to get the "
               "current local Resolvable Private Address being used for the "
               "corresponding peer Identity Address.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Read_Local_Resolvable_Address", .OCF = 0x002c, .numcoe = 2, .numret = 2, .octet = 35, .bit = 0, .Cond = { "E", "C.10" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Peer_Identity_Address_Type",                     .octets =   1 },
          { .name = "Peer_Identity_Address",                          .octets =   6 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Local_Resolvable_Address",                       .octets =   6 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Read Local Supported Features command", .iscmd = 1, .OGF = 0x08, .section = "7.8.3", .version = "4.0", .BREDR = "E", .LE = "M",
    .summary = "The HCI_LE_Read_Local_Supported_Features command will read the "
               "version information for the local LE Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Read_Local_Supported_Features", .OCF = 0x0003, .numcoe = 0, .numret = 2, .octet = 25, .bit = 2, .Cond = { "E", "M" },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "LE_Features",                                    .octets =   8 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Read Maximum Advertising Data Length command", .iscmd = 1, .OGF = 0x08, .section = "7.8.57", .version = "5.0", .BREDR = "E", .LE = "C.17",
    .summary = "The HCI_LE_Read_Maximum_Advertising_Data_Length command will read the "
               "maximum length of advertising data that the advertising Controller "
               "supports in a given advertising set.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Read_Maximum_Advertising_Data_Length", .OCF = 0x003a, .numcoe = 0, .numret = 2, .octet = 36, .bit = 6, .Cond = { "E", "C.17" },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Max_Advertising_Data_Length",                    .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Read Maximum Data Length command", .iscmd = 1, .OGF = 0x08, .section = "7.8.46", .version = "4.2", .BREDR = "E", .LE = "C.8",
    .summary = "The HCI_LE_Read_Maximum_Data_Length command allows the Host to read "
               "the Controller's supportedMaxTxOctets, supportedMaxTxTime, "
               "supportedMaxRxOctets, and supportedMaxRxTime parameters.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Read_Maximum_Data_Length", .OCF = 0x002f, .numcoe = 0, .numret = 5, .octet = 35, .bit = 3, .Cond = { "E", "C.8" },
        .Ret = (HCI_Field_t[5]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Supported_Max_TX_Octets",                        .octets =   2 },
          { .name = "Supported_Max_TX_Time",                          .octets =   2 },
          { .name = "Supported_Max_RX_Octets",                        .octets =   2 },
          { .name = "Supported_Max_RX_Time",                          .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Read Number of Supported Advertising Sets command", .iscmd = 1, .OGF = 0x08, .section = "7.8.58", .version = "5.0", .BREDR = "E", .LE = "C.17",
    .summary = "The HCI_LE_Read_Number_of_Supported_Advertising_Sets command will "
               "read the maximum number of advertising sets supported by the "
               "advertising Controller at the same time.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Read_Number_of_Supported_Advertising_Sets", .OCF = 0x003b, .numcoe = 0, .numret = 2, .octet = 36, .bit = 7, .Cond = { "E", "C.17" },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Num_Supported_Advertising_Sets",                 .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Read Peer Resolvable Address command", .iscmd = 1, .OGF = 0x08, .section = "7.8.42", .version = "4.2", .BREDR = "E", .LE = "C.10",
    .summary = "The HCI_LE_Read_Peer_Resolvable_Address command is used to get the "
               "current peer Resolvable Private Address being used for the "
               "corresponding peer Public and Random (static) Identity Address.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Read_Peer_Resolvable_Address", .OCF = 0x002b, .numcoe = 2, .numret = 2, .octet = 34, .bit = 7, .Cond = { "E", "C.10" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Peer_Identity_Address_Type",                     .octets =   1 },
          { .name = "Peer_Identity_Address",                          .octets =   6 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Peer_Resolvable_Address",                        .octets =   6 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Read Periodic Advertiser List Size command", .iscmd = 1, .OGF = 0x08, .section = "7.8.73", .version = "5.0", .BREDR = "E", .LE = "C.21",
    .summary = "The HCI_LE_Read_Periodic_Advertiser_List_Size command will read the "
               "maximum number of Periodic Advertiser List entries that the "
               "Controller supports.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Read_Periodic_Advertiser_List_Size", .OCF = 0x004a, .numcoe = 0, .numret = 2, .octet = 38, .bit = 6, .Cond = { "E", "C.21" },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Periodic_Advertiser_List_Size",                  .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Read PHY command", .iscmd = 1, .OGF = 0x08, .section = "7.8.47", .version = "5.0", .BREDR = "E", .LE = "C.11",
    .summary = "The HCI_LE_Read_PHY command will read the current PHY.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Read_PHY", .OCF = 0x0030, .numcoe = 1, .numret = 4, .octet = 35, .bit = 4, .Cond = { "E", "C.11" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Connection_Handle",                              .octets =   2 },
        },
        .Ret = (HCI_Field_t[4]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "TX_PHY",                                         .octets =   1 },
          { .name = "RX_PHY",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Read Remote Features command", .iscmd = 1, .OGF = 0x08, .section = "7.8.21", .version = "4.0", .BREDR = "E", .LE = "C.3",
    .summary = "The HCI_LE_Read_Remote_Features command is used to read the features "
               "used on a connection and the features supported by a remote LE "
               "device.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Read_Remote_Features", .OCF = 0x0016, .numcoe = 1, .numret = 0, .octet = 27, .bit = 5, .Cond = { "E", "C.3" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Connection_Handle",                              .octets =   2 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_LE_Read_Remote_Features_Complete" },
    },
  },

  { .title = "LE Read Remote Features Complete event", .iscmd = 0, .section = "7.7.65.4", .version = "4.0", .BREDR = "E", .LE = "C.3",
    .summary = "The HCI_LE_Read_Remote_Features_Complete event indicates the "
               "completion of the process to read the features used on a connection "
               "and the features supported by a remote LE device.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Read_Remote_Features_Complete", .code = 0x003e, .subevent = 0x04, .numcoe = 4, .numret = 0,  .Cond = { "E", "C.3" },
        .CoE = (HCI_Field_t[4]) {
          { .name = "Subevent_Code",                                  .octets =   1 },
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "LE_Features",                                    .octets =   8 },
        },
      },
    },
  },

  { .title = "LE Read Remote Transmit Power Level command", .iscmd = 1, .OGF = 0x08, .section = "7.8.118", .version = "5.2", .BREDR = "E", .LE = "C.51",
    .summary = "The HCI_LE_Read_Remote_Transmit_Power_Level command is used to read "
               "the transmit power level used by the remote Controller on a specified "
               "PHY on an ACL connection.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Read_Remote_Transmit_Power_Level", .OCF = 0x0077, .numcoe = 2, .numret = 0, .octet = 44, .bit = 4, .Cond = { "E", "C.51" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "PHY",                                            .octets =   1 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_LE_Transmit_Power_Reporting" },
    },
  },

  { .title = "LE Read Resolving List Size command", .iscmd = 1, .OGF = 0x08, .section = "7.8.41", .version = "4.2", .BREDR = "E", .LE = "C.9",
    .summary = "The HCI_LE_Read_Resolving_List_Size command is used to read the total "
               "number of entries in the resolving list that can be stored in the "
               "Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Read_Resolving_List_Size", .OCF = 0x002a, .numcoe = 0, .numret = 2, .octet = 34, .bit = 6, .Cond = { "E", "C.9" },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Resolving_List_Size",                            .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Read RF Path Compensation command", .iscmd = 1, .OGF = 0x08, .section = "7.8.75", .version = "5.0", .BREDR = "E", .LE = "C.22",
    .summary = "The HCI_LE_Read_RF_Path_Compensation command is used to read the RF "
               "Path Compensation Value.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Read_RF_Path_Compensation", .OCF = 0x004c, .numcoe = 0, .numret = 3, .octet = 39, .bit = 0, .Cond = { "E", "C.22" },
        .Ret = (HCI_Field_t[3]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "RF_TX_Path_Compensation_Value",                  .octets =   2 },
          { .name = "RF_RX_Path_Compensation_Value",                  .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Read Suggested Default Data Length command", .iscmd = 1, .OGF = 0x08, .section = "7.8.34", .version = "4.2", .BREDR = "E", .LE = "C.8",
    .summary = "The HCI_LE_Read_Suggested_Default_Data_Length command allows the Host "
               "to read the initial MaxTxOctets and MaxTxTime values for new "
               "connections it suggested to the Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Read_Suggested_Default_Data_Length", .OCF = 0x0023, .numcoe = 0, .numret = 3, .octet = 33, .bit = 7, .Cond = { "E", "C.8" },
        .Ret = (HCI_Field_t[3]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Suggested_Max_TX_Octets",                        .octets =   2 },
          { .name = "Suggested_Max_TX_Time",                          .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Read Supported States command", .iscmd = 1, .OGF = 0x08, .section = "7.8.27", .version = "4.0", .BREDR = "E", .LE = "M",
    .summary = "The HCI_LE_Read_Supported_States command will read the current "
               "supported state and role combinations for the local LE Controllers.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Read_Supported_States", .OCF = 0x001c, .numcoe = 0, .numret = 2, .octet = 28, .bit = 3, .Cond = { "E", "M" },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "LE_States",                                                    },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Read Transmit Power command", .iscmd = 1, .OGF = 0x08, .section = "7.8.74", .version = "5.0", .BREDR = "E", .LE = "C.64",
    .summary = "The HCI_LE_Read_Transmit_Power command will read the minimum and "
               "maximum transmit powers supported by the Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Read_Transmit_Power", .OCF = 0x004b, .numcoe = 0, .numret = 3, .octet = 38, .bit = 7, .Cond = { "E", "C.64" },
        .Ret = (HCI_Field_t[3]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Min_TX_Power",                                   .octets =   1 },
          { .name = "Max_TX_Power",                                   .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Receiver Test command", .iscmd = 1, .OGF = 0x08, .section = "7.8.28", .version = "4.0 5.0 5.1", .BREDR = "E", .LE = "[v1] C.2 [v2] C.13 [v3] C.30",
    .summary = "The HCI_LE_Receiver_Test command will run the LE receiver test.",

    .numinst = 3, .Inst = (HCI_Inst_t[3]) {
      { .name = "HCI_LE_Receiver_Test [v3]", .OCF = 0x004f, .numcoe = 8, .numret = 1, .octet = 39, .bit = 3, .Cond = { "E", "C.30" },
        .CoE = (HCI_Field_t[8]) {
          { .name = "RX_Channel",                                     .octets =   1 },
          { .name = "PHY",                                            .octets =   1 },
          { .name = "Modulation_Index",                               .octets =   1 },
          { .name = "Expected_CTE_Length",                            .octets =   1 },
          { .name = "Expected_CTE_Type",                              .octets =   1 },
          { .name = "Slot_Durations",                                 .octets =   1 },
          { .name = "Switching_Pattern_Length",                       .octets =   1 },
          { .name = "Antenna_IDs",                                    .octets =   1, .ref2size = "Switching_Pattern_Length" },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
      { .name = "HCI_LE_Receiver_Test [v2]", .OCF = 0x0033, .numcoe = 3, .numret = 1, .octet = 35, .bit = 7, .Cond = { "E", "C.13" },
        .CoE = (HCI_Field_t[3]) {
          { .name = "RX_Channel",                                     .octets =   1 },
          { .name = "PHY",                                            .octets =   1 },
          { .name = "Modulation_Index",                               .octets =   1 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
      { .name = "HCI_LE_Receiver_Test [v1]", .OCF = 0x001d, .numcoe = 1, .numret = 1, .octet = 28, .bit = 4, .Cond = { "E", "C.2" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "RX_Channel",                                     .octets =   1 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_LE_Connectionless_IQ_Report" },
    },
  },

  { .title = "LE Reject CIS Request command", .iscmd = 1, .OGF = 0x08, .section = "7.8.102", .version = "5.2", .BREDR = "E", .LE = "C.40",
    .summary = "The HCI_LE_Reject_CIS_Request command is used by the Peripheral's "
               "Host to inform the Controller to reject the request for creating the "
               "CIS.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Reject_CIS_Request", .OCF = 0x0067, .numcoe = 2, .numret = 2, .octet = 42, .bit = 4, .Cond = { "E", "C.40" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Reason",                                         .octets =   1 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Remote Connection Parameter Request event", .iscmd = 0, .section = "7.7.65.6", .version = "4.1", .BREDR = "E", .LE = "C.6",
    .summary = "The LE Remote Connection Parameter Request event is used to indicate "
               "to the Host that the remote device is requesting a change in the "
               "connection parameters.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Remote_Connection_Parameter_Request", .code = 0x003e, .subevent = 0x06, .numcoe = 6, .numret = 0,  .Cond = { "E", "C.6" },
        .CoE = (HCI_Field_t[6]) {
          { .name = "Subevent_Code",                                  .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Interval_Min",                                   .octets =   2 },
          { .name = "Interval_Max",                                   .octets =   2 },
          { .name = "Max_Latency",                                    .octets =   2 },
          { .name = "Timeout",                                        .octets =   2 },
        },
      },
    },
  },

  { .title = "LE Remote Connection Parameter Request Negative Reply command", .iscmd = 1, .OGF = 0x08, .section = "7.8.32", .version = "4.1", .BREDR = "E", .LE = "C.6",
    .summary = "The HCI_LE_Remote_Connection_Parameter_Request_Negative_Reply command "
               "is used to reject the remote device's request to change the "
               "connection parameters of the LE connection.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Remote_Connection_Parameter_Request_Negative_Reply", .OCF = 0x0021, .numcoe = 2, .numret = 2, .octet = 33, .bit = 5, .Cond = { "E", "C.6" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Reason",                                         .octets =   1 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Remote Connection Parameter Request Reply command", .iscmd = 1, .OGF = 0x08, .section = "7.8.31", .version = "4.1", .BREDR = "E", .LE = "C.6",
    .summary = "The HCI_LE_Remote_Connection_Parameter_Request_Reply command is used "
               "to accept the remote device's request to change the connection "
               "parameters of the LE connection.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Remote_Connection_Parameter_Request_Reply", .OCF = 0x0020, .numcoe = 7, .numret = 2, .octet = 33, .bit = 4, .Cond = { "E", "C.6" },
        .CoE = (HCI_Field_t[7]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Interval_Min",                                   .octets =   2 },
          { .name = "Interval_Max",                                   .octets =   2 },
          { .name = "Max_Latency",                                    .octets =   2 },
          { .name = "Timeout",                                        .octets =   2 },
          { .name = "Min_CE_Length",                                  .octets =   2 },
          { .name = "Max_CE_Length",                                  .octets =   2 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Remove Advertising Set command", .iscmd = 1, .OGF = 0x08, .section = "7.8.59", .version = "5.0", .BREDR = "E", .LE = "C.17",
    .summary = "The HCI_LE_Remove_Advertising_Set command will remove an advertising "
               "set from the Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Remove_Advertising_Set", .OCF = 0x003c, .numcoe = 1, .numret = 1, .octet = 37, .bit = 0, .Cond = { "E", "C.17" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Advertising_Handle",                             .octets =   1 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Remove CIG command", .iscmd = 1, .OGF = 0x08, .section = "7.8.100", .version = "5.2", .BREDR = "E", .LE = "C.39",
    .summary = "The HCI_LE_Remove_CIG command is used by the Central's Host to remove "
               "a CIG from the Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Remove_CIG", .OCF = 0x0065, .numcoe = 1, .numret = 2, .octet = 42, .bit = 2, .Cond = { "E", "C.39" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "CIG_ID",                                         .octets =   1 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "CIG_ID",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Remove Device From Filter Accept List command", .iscmd = 1, .OGF = 0x08, .section = "7.8.17", .version = "4.0", .BREDR = "E", .LE = "M",
    .summary = "The HCI_LE_Remove_Device_From_Filter_Accept_List command will remove "
               "a single device from the Filter Accept List.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Remove_Device_From_Filter_Accept_List", .OCF = 0x0012, .numcoe = 2, .numret = 1, .octet = 27, .bit = 1, .Cond = { "E", "M" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Address_Type",                                   .octets =   1 },
          { .name = "Address",                                        .octets =   6 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Remove Device From Periodic Advertiser List command", .iscmd = 1, .OGF = 0x08, .section = "7.8.71", .version = "5.0", .BREDR = "E", .LE = "C.21",
    .summary = "The HCI_LE_Remove_Device_From_Periodic_Advertiser_List command will "
               "remove a single device from the Periodic Advertiser List.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Remove_Device_From_Periodic_Advertiser_List", .OCF = 0x0048, .numcoe = 3, .numret = 1, .octet = 38, .bit = 4, .Cond = { "E", "C.21" },
        .CoE = (HCI_Field_t[3]) {
          { .name = "Advertiser_Address_Type",                        .octets =   1 },
          { .name = "Advertiser_Address",                             .octets =   6 },
          { .name = "Advertising_SID",                                .octets =   1 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Remove Device From Resolving List command", .iscmd = 1, .OGF = 0x08, .section = "7.8.39", .version = "4.2", .BREDR = "E", .LE = "C.9",
    .summary = "The HCI_LE_Remove_Device_From_Resolving_List command is used to "
               "remove one device from the resolving list used to resolve Resolvable "
               "Private Addresses in the Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Remove_Device_From_Resolving_List", .OCF = 0x0028, .numcoe = 2, .numret = 1, .octet = 34, .bit = 4, .Cond = { "E", "C.9" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Peer_Identity_Address_Type",                     .octets =   1 },
          { .name = "Peer_Identity_Address",                          .octets =   6 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Remove ISO Data Path command", .iscmd = 1, .OGF = 0x08, .section = "7.8.110", .version = "5.2", .BREDR = "E", .LE = "C.47",
    .summary = "The HCI_LE_Remove_ISO_Data_Path command removes an isochronous data "
               "path between the Host and the Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Remove_ISO_Data_Path", .OCF = 0x006f, .numcoe = 2, .numret = 2, .octet = 43, .bit = 4, .Cond = { "E", "C.47" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Data_Path_Direction",                            .octets =   1 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Request Peer SCA command", .iscmd = 1, .OGF = 0x08, .section = "7.8.108", .version = "5.2", .BREDR = "E", .LE = "C.44",
    .summary = "The HCI_LE_Request_Peer_SCA command requests the Sleep Clock Accuracy "
               "of the peer device.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Request_Peer_SCA", .OCF = 0x006d, .numcoe = 1, .numret = 0, .octet = 43, .bit = 2, .Cond = { "E", "C.44" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Connection_Handle",                              .octets =   2 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_LE_Request_Peer_SCA_Complete" },
    },
  },

  { .title = "LE Request Peer SCA Complete event", .iscmd = 0, .section = "7.7.65.31", .version = "5.2", .BREDR = "E", .LE = "C.95",
    .summary = "The HCI_LE_Request_Peer_SCA_Complete event indicates that the "
               "Controller completed the attempt to read the Sleep Clock Accuracy "
               "(SCA) of the peer device.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Request_Peer_SCA_Complete", .code = 0x003e, .subevent = 0x1f, .numcoe = 4, .numret = 0,  .Cond = { "E", "C.95" },
        .CoE = (HCI_Field_t[4]) {
          { .name = "Subevent_Code",                                  .octets =   1 },
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Peer_Clock_Accuracy",                            .octets =   1 },
        },
      },
    },
  },

  { .title = "LE Scan Request Received event", .iscmd = 0, .section = "7.7.65.19", .version = "5.0", .BREDR = "E", .LE = "C.17",
    .summary = "The HCI_LE_Scan_Request_Received event indicates that a scan request "
               "has been received.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Scan_Request_Received", .code = 0x003e, .subevent = 0x13, .numcoe = 4, .numret = 0,  .Cond = { "E", "C.17" },
        .CoE = (HCI_Field_t[4]) {
          { .name = "Subevent_Code",                                  .octets =   1 },
          { .name = "Advertising_Handle",                             .octets =   1 },
          { .name = "Scanner_Address_Type",                           .octets =   1 },
          { .name = "Scanner_Address",                                .octets =   6 },
        },
      },
    },
  },

  { .title = "LE Scan Timeout event", .iscmd = 0, .section = "7.7.65.17", .version = "5.0", .BREDR = "E", .LE = "C.19",
    .summary = "The HCI_LE_Scan_Timeout event indicates that scanning has finished.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Scan_Timeout", .code = 0x003e, .subevent = 0x11, .numcoe = 1, .numret = 0,  .Cond = { "E", "C.19" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Subevent_Code",                                  .octets =   1 },
        },
      },
    },
  },

  { .title = "LE Set Address Resolution Enable command", .iscmd = 1, .OGF = 0x08, .section = "7.8.44", .version = "4.2", .BREDR = "E", .LE = "C.9",
    .summary = "The HCI_LE_Set_Address_Resolution_Enable command is used to enable "
               "resolution of Resolvable Private Addresses in the Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Set_Address_Resolution_Enable", .OCF = 0x002d, .numcoe = 1, .numret = 1, .octet = 35, .bit = 1, .Cond = { "E", "C.9" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Address_Resolution_Enable",                      .octets =   1 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Set Advertising Data command", .iscmd = 1, .OGF = 0x08, .section = "7.8.7", .version = "4.0", .BREDR = "E", .LE = "C.97",
    .summary = "The HCI_LE_Set_Advertising_Data command will set the data transmitted "
               "when advertising.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Set_Advertising_Data", .OCF = 0x0008, .numcoe = 2, .numret = 1, .octet = 25, .bit = 7, .Cond = { "E", "C.97" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Advertising_Data_Length",                        .octets =   1 },
          { .name = "Advertising_Data",                               .octets =  31 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Set Advertising Enable command", .iscmd = 1, .OGF = 0x08, .section = "7.8.9", .version = "4.0", .BREDR = "E", .LE = "C.97",
    .summary = "The HCI_LE_Set_Advertising_Enable command will enable or disable "
               "advertising.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Set_Advertising_Enable", .OCF = 0x000a, .numcoe = 1, .numret = 1, .octet = 26, .bit = 1, .Cond = { "E", "C.97" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Advertising_Enable",                             .octets =   1 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 3, .Events = (HCI_Event_t[3]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_LE_Connection_Complete" },
      { .name = "HCI_LE_Enhanced_Connection_Complete [v2]" },
    },
  },

  { .title = "LE Set Advertising Parameters command", .iscmd = 1, .OGF = 0x08, .section = "7.8.5", .version = "4.0", .BREDR = "E", .LE = "C.97",
    .summary = "The HCI_LE_Set_Advertising_Parameters command will set the parameters "
               "used for advertising.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Set_Advertising_Parameters", .OCF = 0x0006, .numcoe = 8, .numret = 1, .octet = 25, .bit = 5, .Cond = { "E", "C.97" },
        .CoE = (HCI_Field_t[8]) {
          { .name = "Advertising_Interval_Min",                       .octets =   2 },
          { .name = "Advertising_Interval_Max",                       .octets =   2 },
          { .name = "Advertising_Type",                               .octets =   1 },
          { .name = "Own_Address_Type",                               .octets =   1 },
          { .name = "Peer_Address_Type",                              .octets =   1 },
          { .name = "Peer_Address",                                   .octets =   6 },
          { .name = "Advertising_Channel_Map",                        .octets =   1 },
          { .name = "Advertising_Filter_Policy",                      .octets =   1 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Set Advertising Set Random Address command", .iscmd = 1, .OGF = 0x08, .section = "7.8.52", .version = "5.0", .BREDR = "E", .LE = "C.17",
    .summary = "The HCI_LE_Set_Advertising_Set_Random_Address command will set the "
               "random address used in advertising.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Set_Advertising_Set_Random_Address", .OCF = 0x0035, .numcoe = 2, .numret = 1, .octet = 36, .bit = 1, .Cond = { "E", "C.17" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Advertising_Handle",                             .octets =   1 },
          { .name = "Random_Address",                                 .octets =   6 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Set CIG Parameters command", .iscmd = 1, .OGF = 0x08, .section = "7.8.97", .version = "5.2", .BREDR = "E", .LE = "C.39",
    .summary = "The HCI_LE_Set_CIG_Parameters command is used by a Central's Host to "
               "set the parameters of one or more Connected Isochronous Streams "
               "(CISes) that are associated with a CIG in the Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Set_CIG_Parameters", .OCF = 0x0062, .numcoe = 16, .numret = 4, .octet = 41, .bit = 7, .Cond = { "E", "C.39" },
        .CoE = (HCI_Field_t[16]) {
          { .name = "CIG_ID",                                         .octets =   1 },
          { .name = "SDU_Interval_C_To_P",                            .octets =   3 },
          { .name = "SDU_Interval_P_To_C",                            .octets =   3 },
          { .name = "Worst_Case_SCA",                                 .octets =   1 },
          { .name = "Packing",                                        .octets =   1 },
          { .name = "Framing",                                        .octets =   1 },
          { .name = "Max_Transport_Latency_C_To_P",                   .octets =   2 },
          { .name = "Max_Transport_Latency_P_To_C",                   .octets =   2 },
          { .name = "CIS_Count",                                      .octets =   1 },
          { .name = "CIS_ID",                                         .octets =   1, .ref2size = "CIS_Count" },
          { .name = "Max_SDU_C_To_P",                                 .octets =   2, .ref2size = "CIS_Count" },
          { .name = "Max_SDU_P_To_C",                                 .octets =   2, .ref2size = "CIS_Count" },
          { .name = "PHY_C_To_P",                                     .octets =   1, .ref2size = "CIS_Count" },
          { .name = "PHY_P_To_C",                                     .octets =   1, .ref2size = "CIS_Count" },
          { .name = "RTN_C_To_P",                                     .octets =   1, .ref2size = "CIS_Count" },
          { .name = "RTN_P_To_C",                                     .octets =   1, .ref2size = "CIS_Count" },
        },
        .Ret = (HCI_Field_t[4]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "CIG_ID",                                         .octets =   1 },
          { .name = "CIS_Count",                                      .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2, .ref2size = "CIS_Count" },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Set CIG Parameters Test command", .iscmd = 1, .OGF = 0x08, .section = "7.8.98", .version = "5.2", .BREDR = "E", .LE = "C.39",
    .summary = "The HCI_LE_Set_CIG_Parameters_Test command is used by a Central's "
               "Host to set the parameters of one or more CISes that are associated "
               "with a CIG in the Controller for testing purposes.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Set_CIG_Parameters_Test", .OCF = 0x0063, .numcoe = 20, .numret = 4, .octet = 42, .bit = 0, .Cond = { "E", "C.39" },
        .CoE = (HCI_Field_t[20]) {
          { .name = "CIG_ID",                                         .octets =   1 },
          { .name = "SDU_Interval_C_To_P",                            .octets =   3 },
          { .name = "SDU_Interval_P_To_C",                            .octets =   3 },
          { .name = "FT_C_To_P",                                      .octets =   1 },
          { .name = "FT_P_To_C",                                      .octets =   1 },
          { .name = "ISO_Interval",                                   .octets =   2 },
          { .name = "Worst_Case_SCA",                                 .octets =   1 },
          { .name = "Packing",                                        .octets =   1 },
          { .name = "Framing",                                        .octets =   1 },
          { .name = "CIS_Count",                                      .octets =   1 },
          { .name = "CIS_ID",                                         .octets =   1, .ref2size = "CIS_Count" },
          { .name = "NSE",                                            .octets =   1, .ref2size = "CIS_Count" },
          { .name = "Max_SDU_C_To_P",                                 .octets =   2, .ref2size = "CIS_Count" },
          { .name = "Max_SDU_P_To_C",                                 .octets =   2, .ref2size = "CIS_Count" },
          { .name = "Max_PDU_C_To_P",                                 .octets =   2, .ref2size = "CIS_Count" },
          { .name = "Max_PDU_P_To_C",                                 .octets =   2, .ref2size = "CIS_Count" },
          { .name = "PHY_C_To_P",                                     .octets =   1, .ref2size = "CIS_Count" },
          { .name = "PHY_P_To_C",                                     .octets =   1, .ref2size = "CIS_Count" },
          { .name = "BN_C_To_P",                                      .octets =   1, .ref2size = "CIS_Count" },
          { .name = "BN_P_To_C",                                      .octets =   1, .ref2size = "CIS_Count" },
        },
        .Ret = (HCI_Field_t[4]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "CIG_ID",                                         .octets =   1 },
          { .name = "CIS_Count",                                      .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2, .ref2size = "CIS_Count" },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Set Connection CTE Receive Parameters command", .iscmd = 1, .OGF = 0x08, .section = "7.8.83", .version = "5.1", .BREDR = "E", .LE = "C.25",
    .summary = "The HCI_LE_Set_Connection_CTE_Receive_Parameters command will set the "
               "antennaswitching pattern, switching and sampling slot durations for "
               "receiving the Constant Tone Extension on a connection.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Set_Connection_CTE_Receive_Parameters", .OCF = 0x0054, .numcoe = 5, .numret = 2, .octet = 40, .bit = 0, .Cond = { "E", "C.25" },
        .CoE = (HCI_Field_t[5]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Sampling_Enable",                                .octets =   1 },
          { .name = "Slot_Durations",                                 .octets =   1 },
          { .name = "Switching_Pattern_Length",                       .octets =   1 },
          { .name = "Antenna_IDs",                                    .octets =   1, .ref2size = "Switching_Pattern_Length" },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Set Connection CTE Transmit Parameters command", .iscmd = 1, .OGF = 0x08, .section = "7.8.84", .version = "5.1", .BREDR = "E", .LE = "C.26",
    .summary = "The HCI_LE_Set_Connection_CTE_Transmit_Parameters command will set "
               "the antennaswitching pattern, switching and sampling slot durations "
               "for transmitting the Constant Tone Extension on a connection.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Set_Connection_CTE_Transmit_Parameters", .OCF = 0x0055, .numcoe = 4, .numret = 2, .octet = 40, .bit = 1, .Cond = { "E", "C.26" },
        .CoE = (HCI_Field_t[4]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "CTE_Types",                                      .octets =   1 },
          { .name = "Switching_Pattern_Length",                       .octets =   1 },
          { .name = "Antenna_IDs",                                    .octets =   1, .ref2size = "Switching_Pattern_Length" },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Set Connectionless CTE Transmit Enable command", .iscmd = 1, .OGF = 0x08, .section = "7.8.81", .version = "5.1", .BREDR = "E", .LE = "C.27",
    .summary = "The HCI_LE_Set_Connectionless_CTE_Transmit_Enable command will "
               "request the Controller to enable or disable sending packets "
               "containing a Constant Tone Extension.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Set_Connectionless_CTE_Transmit_Enable", .OCF = 0x0052, .numcoe = 2, .numret = 1, .octet = 39, .bit = 6, .Cond = { "E", "C.27" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Advertising_Handle",                             .octets =   1 },
          { .name = "CTE_Enable",                                     .octets =   1 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Set Connectionless CTE Transmit Parameters command", .iscmd = 1, .OGF = 0x08, .section = "7.8.80", .version = "5.1", .BREDR = "E", .LE = "C.27",
    .summary = "The HCI_LE_Set_Connectionless_CTE_Transmit_Parameters command will "
               "set the antenna-switching pattern and switching and sampling slot "
               "durations for the transmission of Constant Tone Extensions.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Set_Connectionless_CTE_Transmit_Parameters", .OCF = 0x0051, .numcoe = 6, .numret = 1, .octet = 39, .bit = 5, .Cond = { "E", "C.27" },
        .CoE = (HCI_Field_t[6]) {
          { .name = "Advertising_Handle",                             .octets =   1 },
          { .name = "CTE_Length",                                     .octets =   1 },
          { .name = "CTE_Type",                                       .octets =   1 },
          { .name = "CTE_Count",                                      .octets =   1 },
          { .name = "Switching_Pattern_Length",                       .octets =   1 },
          { .name = "Antenna_IDs",                                    .octets =   1, .ref2size = "Switching_Pattern_Length" },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Set Connectionless IQ Sampling Enable command", .iscmd = 1, .OGF = 0x08, .section = "7.8.82", .version = "5.1", .BREDR = "E", .LE = "C.28",
    .summary = "The HCI_LE_Set_Connectionless_IQ_Sampling_Enable command will request "
               "the Controller to enable or disable taking IQ samples from the "
               "Constant Tone Extension of advertising packets.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Set_Connectionless_IQ_Sampling_Enable", .OCF = 0x0053, .numcoe = 6, .numret = 2, .octet = 39, .bit = 7, .Cond = { "E", "C.28" },
        .CoE = (HCI_Field_t[6]) {
          { .name = "Sync_Handle",                                    .octets =   2 },
          { .name = "Sampling_Enable",                                .octets =   1 },
          { .name = "Slot_Durations",                                 .octets =   1 },
          { .name = "Max_Sampled_CTEs",                               .octets =   1 },
          { .name = "Switching_Pattern_Length",                       .octets =   1 },
          { .name = "Antenna_IDs",                                    .octets =   1, .ref2size = "Switching_Pattern_Length" },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Sync_Handle",                                    .octets =   2 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_LE_Connectionless_IQ_Report" },
    },
  },

  { .title = "LE Set Data Length command", .iscmd = 1, .OGF = 0x08, .section = "7.8.33", .version = "4.2", .BREDR = "E", .LE = "C.8",
    .summary = "The HCI_LE_Set_Data_Length command is used to suggest maximum packet "
               "sizes to the Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Set_Data_Length", .OCF = 0x0022, .numcoe = 3, .numret = 2, .octet = 33, .bit = 6, .Cond = { "E", "C.8" },
        .CoE = (HCI_Field_t[3]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "TX_Octets",                                      .octets =   2 },
          { .name = "TX_Time",                                        .octets =   2 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_LE_Data_Length_Change" },
    },
  },

  { .title = "LE Set Data Related Address Changes command", .iscmd = 1, .OGF = 0x08, .section = "7.8.122", .version = "5.3", .BREDR = "E", .LE = "C.10",
    .summary = "The HCI_LE_Set_Data_Related_Address_Changes command specifies "
               "circumstances when the Controller shall refresh any Resolvable "
               "Private Address used by an advertising set, whether or not the "
               "address timeout period has been reached.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Set_Data_Related_Address_Changes", .OCF = 0x007c, .numcoe = 2, .numret = 1, .octet = 45, .bit = 6, .Cond = { "E", "C.10" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Advertising_Handle",                             .octets =   1 },
          { .name = "Change_Reasons",                                 .octets =   1 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Set Default Periodic Advertising Sync Transfer Parameters command", .iscmd = 1, .OGF = 0x08, .section = "7.8.92", .version = "5.1", .BREDR = "E", .LE = "C.35",
    .summary = "The HCI_LE_Set_Default_Periodic_Advertising_Sync_Transfer_Parameters "
               "command is used to specify the default behavior of the Controller "
               "when periodic advertising synchronization information is received "
               "from a connected Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Set_Default_Periodic_Advertising_Sync_Transfer_Parameters", .OCF = 0x005d, .numcoe = 4, .numret = 1, .octet = 41, .bit = 1, .Cond = { "E", "C.35" },
        .CoE = (HCI_Field_t[4]) {
          { .name = "Mode",                                           .octets =   1 },
          { .name = "Skip",                                           .octets =   2 },
          { .name = "Sync_Timeout",                                   .octets =   2 },
          { .name = "CTE_Type",                                       .octets =   1 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Set Default PHY command", .iscmd = 1, .OGF = 0x08, .section = "7.8.48", .version = "5.0", .BREDR = "E", .LE = "C.11",
    .summary = "The HCI_LE_Set_Default_PHY command is used to configure preferred "
               "PHYs for new connections for the local device.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Set_Default_PHY", .OCF = 0x0031, .numcoe = 3, .numret = 1, .octet = 35, .bit = 5, .Cond = { "E", "C.11" },
        .CoE = (HCI_Field_t[3]) {
          { .name = "All_PHYs",                                       .octets =   1 },
          { .name = "TX_PHYs",                                        .octets =   1 },
          { .name = "RX_PHYs",                                        .octets =   1 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Set Default Subrate command", .iscmd = 1, .OGF = 0x08, .section = "7.8.123", .version = "5.3", .BREDR = "E", .LE = "C.57",
    .summary = "The HCI_LE_Set_Default_Subrate command sets the range of the min and "
               "max subrates and other subrate parameters on a Central that may be "
               "requested by a Peripheral.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Set_Default_Subrate", .OCF = 0x007d, .numcoe = 5, .numret = 1, .octet = 46, .bit = 0, .Cond = { "E", "C.57" },
        .CoE = (HCI_Field_t[5]) {
          { .name = "Subrate_Min",                                    .octets =   2 },
          { .name = "Subrate_Max",                                    .octets =   2 },
          { .name = "Max_Latency",                                    .octets =   2 },
          { .name = "Continuation_Number",                            .octets =   2 },
          { .name = "Supervision_Timeout",                            .octets =   2 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Set Event Mask command", .iscmd = 1, .OGF = 0x08, .section = "7.8.1", .version = "4.0", .BREDR = "E", .LE = "M",
    .summary = "The HCI_LE_Set_Event_Mask command is used to control which events are "
               "generated by the HCI for the Host.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Set_Event_Mask", .OCF = 0x0001, .numcoe = 1, .numret = 1, .octet = 25, .bit = 0, .Cond = { "E", "M" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "LE_Event_Mask",                                  .octets =   8 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Set Extended Advertising Data command", .iscmd = 1, .OGF = 0x08, .section = "7.8.54", .version = "5.0", .BREDR = "E", .LE = "C.17",
    .summary = "The HCI_LE_Set_Extended_Advertising_Data command will set the "
               "advertising data transmitted when advertising.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Set_Extended_Advertising_Data", .OCF = 0x0037, .numcoe = 5, .numret = 1, .octet = 36, .bit = 3, .Cond = { "E", "C.17" },
        .CoE = (HCI_Field_t[5]) {
          { .name = "Advertising_Handle",                             .octets =   1 },
          { .name = "Operation",                                      .octets =   1 },
          { .name = "Fragment_Preference",                            .octets =   1 },
          { .name = "Advertising_Data_Length",                        .octets =   1 },
          { .name = "Advertising_Data",                                             .ref2size = "Advertising_Data_Length" },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Set Extended Advertising Enable command", .iscmd = 1, .OGF = 0x08, .section = "7.8.56", .version = "5.0", .BREDR = "E", .LE = "C.17",
    .summary = "The HCI_LE_Set_Extended_Advertising_Enable command will enable or "
               "disable advertising.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Set_Extended_Advertising_Enable", .OCF = 0x0039, .numcoe = 5, .numret = 1, .octet = 36, .bit = 5, .Cond = { "E", "C.17" },
        .CoE = (HCI_Field_t[5]) {
          { .name = "Enable",                                         .octets =   1 },
          { .name = "Num_Sets",                                       .octets =   1 },
          { .name = "Advertising_Handle",                             .octets =   1, .ref2size = "Num_Sets" },
          { .name = "Duration",                                       .octets =   2, .ref2size = "Num_Sets" },
          { .name = "Max_Extended_Advertising_Events",                .octets =   1, .ref2size = "Num_Sets" },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 5, .Events = (HCI_Event_t[5]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_LE_Advertising_Set_Terminated" },
      { .name = "HCI_LE_Connection_Complete" },
      { .name = "HCI_LE_Enhanced_Connection_Complete [v2]" },
      { .name = "HCI_LE_Channel_Selection_Algorithm" },
    },
  },

  { .title = "LE Set Extended Advertising Parameters command", .iscmd = 1, .OGF = 0x08, .section = "7.8.53", .version = "5.0 5.4", .BREDR = "E", .LE = "[v1] C.65 [v2] C.66",
    .summary = "The HCI_LE_Set_Extended_Advertising_Parameters command will set the "
               "parameters used for advertising.",

    .numinst = 2, .Inst = (HCI_Inst_t[2]) {
      { .name = "HCI_LE_Set_Extended_Advertising_Parameters [v2]", .OCF = 0x007f, .numcoe = 17, .numret = 2, .octet = 46, .bit = 2, .Cond = { "E", "C.66" },
        .CoE = (HCI_Field_t[17]) {
          { .name = "Advertising_Handle",                             .octets =   1 },
          { .name = "Advertising_Event_Properties",                   .octets =   2 },
          { .name = "Primary_Advertising_Interval_Min",               .octets =   3 },
          { .name = "Primary_Advertising_Interval_Max",               .octets =   3 },
          { .name = "Primary_Advertising_Channel_Map",                .octets =   1 },
          { .name = "Own_Address_Type",                               .octets =   1 },
          { .name = "Peer_Address_Type",                              .octets =   1 },
          { .name = "Peer_Address",                                   .octets =   6 },
          { .name = "Advertising_Filter_Policy",                      .octets =   1 },
          { .name = "Advertising_TX_Power",                           .octets =   1 },
          { .name = "Primary_Advertising_PHY",                        .octets =   1 },
          { .name = "Secondary_Advertising_Max_Skip",                 .octets =   1 },
          { .name = "Secondary_Advertising_PHY",                      .octets =   1 },
          { .name = "Advertising_SID",                                .octets =   1 },
          { .name = "Scan_Request_Notification_Enable",               .octets =   1 },
          { .name = "Primary_Advertising_PHY_Options",                .octets =   1 },
          { .name = "Secondary_Advertising_PHY_Options",              .octets =   1 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Selected_TX_Power",                              .octets =   1 },
        },
      },
      { .name = "HCI_LE_Set_Extended_Advertising_Parameters [v1]", .OCF = 0x0036, .numcoe = 15, .numret = 2, .octet = 36, .bit = 2, .Cond = { "E", "C.65" },
        .CoE = (HCI_Field_t[15]) {
          { .name = "Advertising_Handle",                             .octets =   1 },
          { .name = "Advertising_Event_Properties",                   .octets =   2 },
          { .name = "Primary_Advertising_Interval_Min",               .octets =   3 },
          { .name = "Primary_Advertising_Interval_Max",               .octets =   3 },
          { .name = "Primary_Advertising_Channel_Map",                .octets =   1 },
          { .name = "Own_Address_Type",                               .octets =   1 },
          { .name = "Peer_Address_Type",                              .octets =   1 },
          { .name = "Peer_Address",                                   .octets =   6 },
          { .name = "Advertising_Filter_Policy",                      .octets =   1 },
          { .name = "Advertising_TX_Power",                           .octets =   1 },
          { .name = "Primary_Advertising_PHY",                        .octets =   1 },
          { .name = "Secondary_Advertising_Max_Skip",                 .octets =   1 },
          { .name = "Secondary_Advertising_PHY",                      .octets =   1 },
          { .name = "Advertising_SID",                                .octets =   1 },
          { .name = "Scan_Request_Notification_Enable",               .octets =   1 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Selected_TX_Power",                              .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Set Extended Scan Enable command", .iscmd = 1, .OGF = 0x08, .section = "7.8.65", .version = "5.0", .BREDR = "E", .LE = "C.19",
    .summary = "The HCI_LE_Set_Extended_Scan_Enable command will enable or disable "
               "scanning on the primary advertising physical channels.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Set_Extended_Scan_Enable", .OCF = 0x0042, .numcoe = 4, .numret = 1, .octet = 37, .bit = 6, .Cond = { "E", "C.19" },
        .CoE = (HCI_Field_t[4]) {
          { .name = "Enable",                                         .octets =   1 },
          { .name = "Filter_Duplicates",                              .octets =   1 },
          { .name = "Duration",                                       .octets =   2 },
          { .name = "Period",                                         .octets =   2 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 3, .Events = (HCI_Event_t[3]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_LE_Extended_Advertising_Report" },
      { .name = "HCI_LE_Scan_Timeout" },
    },
  },

  { .title = "LE Set Extended Scan Parameters command", .iscmd = 1, .OGF = 0x08, .section = "7.8.64", .version = "5.0", .BREDR = "E", .LE = "C.19",
    .summary = "The HCI_LE_Set_Extended_Scan_Parameters command will set the "
               "parameters used for scanning on the primary advertising physical "
               "channel.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Set_Extended_Scan_Parameters", .OCF = 0x0041, .numcoe = 6, .numret = 1, .octet = 37, .bit = 5, .Cond = { "E", "C.19" },
        .CoE = (HCI_Field_t[6]) {
          { .name = "Own_Address_Type",                               .octets =   1 },
          { .name = "Scanning_Filter_Policy",                         .octets =   1 },
          { .name = "Scanning_PHYs",                                  .octets =   1 },
          { .name = "Scan_Type",                                                    .ref2size = "Scanning_PHYs" },
          { .name = "Scan_Interval",                                                .ref2size = "Scanning_PHYs" },
          { .name = "Scan_Window",                                                  .ref2size = "Scanning_PHYs" },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Set Extended Scan Response Data command", .iscmd = 1, .OGF = 0x08, .section = "7.8.55", .version = "5.0", .BREDR = "E", .LE = "C.17",
    .summary = "The HCI_LE_Set_Extended_Scan_Response_Data command will set the data "
               "transmitted in a scan response.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Set_Extended_Scan_Response_Data", .OCF = 0x0038, .numcoe = 5, .numret = 1, .octet = 36, .bit = 4, .Cond = { "E", "C.17" },
        .CoE = (HCI_Field_t[5]) {
          { .name = "Advertising_Handle",                             .octets =   1 },
          { .name = "Operation",                                      .octets =   1 },
          { .name = "Fragment_Preference",                            .octets =   1 },
          { .name = "Scan_Response_Data_Length",                      .octets =   1 },
          { .name = "Scan_Response_Data",                                           .ref2size = "Scan_Response_Data_Length" },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Set Host Channel Classification command", .iscmd = 1, .OGF = 0x08, .section = "7.8.19", .version = "4.0", .BREDR = "E", .LE = "C.36",
    .summary = "The HCI_LE_Set_Host_Channel_Classification command allows the Host to "
               "specify a channel classification based on its 'local information'.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Set_Host_Channel_Classification", .OCF = 0x0014, .numcoe = 1, .numret = 1, .octet = 27, .bit = 3, .Cond = { "E", "C.36" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Channel_Map",                                    .octets =   5 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Set Host Feature command", .iscmd = 1, .OGF = 0x08, .section = "7.8.115", .version = "5.2", .BREDR = "E", .LE = "C.49",
    .summary = "The HCI_LE_Set_Host_Feature command is used to set or clear a bit "
               "controlled by the Host in the Link Layer FeatureSet stored in the "
               "Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Set_Host_Feature", .OCF = 0x0074, .numcoe = 2, .numret = 1, .octet = 44, .bit = 1, .Cond = { "E", "C.49" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Bit_Number",                                     .octets =   1 },
          { .name = "Bit_Value",                                      .octets =   1 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Set Path Loss Reporting Enable command", .iscmd = 1, .OGF = 0x08, .section = "7.8.120", .version = "5.2", .BREDR = "E", .LE = "C.52",
    .summary = "The HCI_LE_Set_Path_Loss_Reporting_Enable command is used to enable "
               "or disable path loss reporting events for an ACL connection.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Set_Path_Loss_Reporting_Enable", .OCF = 0x0079, .numcoe = 2, .numret = 2, .octet = 44, .bit = 6, .Cond = { "E", "C.52" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Enable",                                         .octets =   1 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_LE_Path_Loss_Threshold" },
    },
  },

  { .title = "LE Set Path Loss Reporting Parameters command", .iscmd = 1, .OGF = 0x08, .section = "7.8.119", .version = "5.2", .BREDR = "E", .LE = "C.52",
    .summary = "The HCI_LE_Set_Path_Loss_Reporting_Parameters command is used to set "
               "the path loss threshold and related parameters used to trigger "
               "reports for an ACL connection.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Set_Path_Loss_Reporting_Parameters", .OCF = 0x0078, .numcoe = 6, .numret = 2, .octet = 44, .bit = 5, .Cond = { "E", "C.52" },
        .CoE = (HCI_Field_t[6]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "High_Threshold",                                 .octets =   1 },
          { .name = "High_Hysteresis",                                .octets =   1 },
          { .name = "Low_Threshold",                                  .octets =   1 },
          { .name = "Low_Hysteresis",                                 .octets =   1 },
          { .name = "Min_Time_Spent",                                 .octets =   2 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_LE_Path_Loss_Threshold" },
    },
  },

  { .title = "LE Set Periodic Advertising Data command", .iscmd = 1, .OGF = 0x08, .section = "7.8.62", .version = "5.0", .BREDR = "E", .LE = "C.18",
    .summary = "The HCI_LE_Set_Periodic_Advertising_Data command will set the "
               "periodic advertising data transmitted when advertising.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Set_Periodic_Advertising_Data", .OCF = 0x003f, .numcoe = 4, .numret = 1, .octet = 37, .bit = 3, .Cond = { "E", "C.18" },
        .CoE = (HCI_Field_t[4]) {
          { .name = "Advertising_Handle",                             .octets =   1 },
          { .name = "Operation",                                      .octets =   1 },
          { .name = "Advertising_Data_Length",                        .octets =   1 },
          { .name = "Advertising_Data",                                             .ref2size = "Advertising_Data_Length" },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Set Periodic Advertising Enable command", .iscmd = 1, .OGF = 0x08, .section = "7.8.63", .version = "5.0", .BREDR = "E", .LE = "C.18",
    .summary = "The HCI_LE_Set_Periodic_Advertising_Enable command will enable or "
               "disable periodic advertising.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Set_Periodic_Advertising_Enable", .OCF = 0x0040, .numcoe = 2, .numret = 1, .octet = 37, .bit = 4, .Cond = { "E", "C.18" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Enable",                                         .octets =   1 },
          { .name = "Advertising_Handle",                             .octets =   1 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Set Periodic Advertising Parameters command", .iscmd = 1, .OGF = 0x08, .section = "7.8.61", .version = "5.0 5.4", .BREDR = "E", .LE = "[v1] C.18 [v2] C.67",
    .summary = "The HCI_LE_Set_Periodic_Advertising_Parameters command will set the "
               "parameters used for periodic advertising.",

    .numinst = 2, .Inst = (HCI_Inst_t[2]) {
      { .name = "HCI_LE_Set_Periodic_Advertising_Parameters [v2]", .OCF = 0x0086, .numcoe = 9, .numret = 2, .octet = 47, .bit = 1, .Cond = { "E", "C.67" },
        .CoE = (HCI_Field_t[9]) {
          { .name = "Advertising_Handle",                             .octets =   1 },
          { .name = "Periodic_Advertising_Interval_Min",              .octets =   2 },
          { .name = "Periodic_Advertising_Interval_Max",              .octets =   2 },
          { .name = "Periodic_Advertising_Properties",                .octets =   2 },
          { .name = "Num_Subevents",                                  .octets =   1 },
          { .name = "Subevent_Interval",                              .octets =   1 },
          { .name = "Response_Slot_Delay",                            .octets =   1 },
          { .name = "Response_Slot_Spacing",                          .octets =   1 },
          { .name = "Num_Response_Slots",                             .octets =   1 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Advertising_Handle",                             .octets =   1 },
        },
      },
      { .name = "HCI_LE_Set_Periodic_Advertising_Parameters [v1]", .OCF = 0x003e, .numcoe = 4, .numret = 1, .octet = 37, .bit = 2, .Cond = { "E", "C.18" },
        .CoE = (HCI_Field_t[4]) {
          { .name = "Advertising_Handle",                             .octets =   1 },
          { .name = "Periodic_Advertising_Interval_Min",              .octets =   2 },
          { .name = "Periodic_Advertising_Interval_Max",              .octets =   2 },
          { .name = "Periodic_Advertising_Properties",                .octets =   2 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Set Periodic Advertising Receive Enable command", .iscmd = 1, .OGF = 0x08, .section = "7.8.88", .version = "5.1", .BREDR = "E", .LE = "C.32",
    .summary = "The HCI_LE_Set_Periodic_Advertising_Receive_Enable command will "
               "enable or disable periodic advertising reports once synchronized.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Set_Periodic_Advertising_Receive_Enable", .OCF = 0x0059, .numcoe = 2, .numret = 1, .octet = 40, .bit = 5, .Cond = { "E", "C.32" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Sync_Handle",                                    .octets =   2 },
          { .name = "Enable",                                         .octets =   1 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Set Periodic Advertising Response Data command", .iscmd = 1, .OGF = 0x08, .section = "7.8.126", .version = "5.4", .BREDR = "E", .LE = "C.68",
    .summary = "The HCI_LE_Set_Periodic_Advertising_Response_Data command is used to "
               "set the data for a response slot.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Set_Periodic_Advertising_Response_Data", .OCF = 0x0083, .numcoe = 7, .numret = 2, .octet = 46, .bit = 6, .Cond = { "E", "C.68" },
        .CoE = (HCI_Field_t[7]) {
          { .name = "Sync_Handle",                                    .octets =   2 },
          { .name = "Request_Event",                                  .octets =   2 },
          { .name = "Request_Subevent",                               .octets =   1 },
          { .name = "Response_Subevent",                              .octets =   1 },
          { .name = "Response_Slot",                                  .octets =   1 },
          { .name = "Response_Data_Length",                           .octets =   1 },
          { .name = "Response_Data",                                                .ref2size = "Response_Data_Length" },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Sync_Handle",                                    .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Set Periodic Advertising Subevent Data command", .iscmd = 1, .OGF = 0x08, .section = "7.8.125", .version = "5.4", .BREDR = "E", .LE = "C.67",
    .summary = "The HCI_LE_Set_Periodic_Advertising_Subevent_Data command is used to "
               "send subevent data for one or more subevents.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Set_Periodic_Advertising_Subevent_Data", .OCF = 0x0082, .numcoe = 7, .numret = 2, .octet = 46, .bit = 5, .Cond = { "E", "C.67" },
        .CoE = (HCI_Field_t[7]) {
          { .name = "Advertising_Handle",                             .octets =   1 },
          { .name = "Num_Subevents",                                  .octets =   1 },
          { .name = "Subevent",                                       .octets =   1, .ref2size = "Num_Subevents" },
          { .name = "Response_Slot_Start",                            .octets =   1, .ref2size = "Num_Subevents" },
          { .name = "Response_Slot_Count",                            .octets =   1, .ref2size = "Num_Subevents" },
          { .name = "Subevent_Data_Length",                           .octets =   1, .ref2size = "Num_Subevents" },
          { .name = "Subevent_Data",                                                .ref2size = "Subevent_Data_Length" },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Advertising_Handle",                             .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Set Periodic Sync Subevent command", .iscmd = 1, .OGF = 0x08, .section = "7.8.127", .version = "5.4", .BREDR = "E", .LE = "C.68",
    .summary = "The HCI_LE_Set_Periodic_Sync_Subevent command is used to configure "
               "the subset of subevents a device will synchronize with.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Set_Periodic_Sync_Subevent", .OCF = 0x0084, .numcoe = 4, .numret = 2, .octet = 46, .bit = 7, .Cond = { "E", "C.68" },
        .CoE = (HCI_Field_t[4]) {
          { .name = "Sync_Handle",                                    .octets =   2 },
          { .name = "Periodic_Advertising_Properties",                .octets =   2 },
          { .name = "Num_Subevents",                                  .octets =   1 },
          { .name = "Subevent",                                       .octets =   1, .ref2size = "Num_Subevents" },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Sync_Handle",                                    .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Set Periodic Advertising Sync Transfer Parameters command", .iscmd = 1, .OGF = 0x08, .section = "7.8.91", .version = "5.1", .BREDR = "E", .LE = "C.35",
    .summary = "The HCI_LE_Set_Periodic_Advertising_Sync_Transfer_Parameters command "
               "is used to allow the Host to specify the behavior of the Controller "
               "when periodic advertising synchronization information is received "
               "from a connected Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Set_Periodic_Advertising_Sync_Transfer_Parameters", .OCF = 0x005c, .numcoe = 5, .numret = 2, .octet = 41, .bit = 0, .Cond = { "E", "C.35" },
        .CoE = (HCI_Field_t[5]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Mode",                                           .octets =   1 },
          { .name = "Skip",                                           .octets =   2 },
          { .name = "Sync_Timeout",                                   .octets =   2 },
          { .name = "CTE_Type",                                       .octets =   1 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Set PHY command", .iscmd = 1, .OGF = 0x08, .section = "7.8.49", .version = "5.0", .BREDR = "E", .LE = "C.11",
    .summary = "The HCI_LE_Set_PHY command is used to request a change of the PHY for "
               "a Connection_Handle.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Set_PHY", .OCF = 0x0032, .numcoe = 5, .numret = 0, .octet = 35, .bit = 6, .Cond = { "E", "C.11" },
        .CoE = (HCI_Field_t[5]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "All_PHYs",                                       .octets =   1 },
          { .name = "TX_PHYs",                                        .octets =   1 },
          { .name = "RX_PHYs",                                        .octets =   1 },
          { .name = "PHY_Options",                                    .octets =   2 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_LE_PHY_Update_Complete" },
    },
  },

  { .title = "LE Set Privacy Mode command", .iscmd = 1, .OGF = 0x08, .section = "7.8.77", .version = "5.0", .BREDR = "E", .LE = "C.9",
    .summary = "The HCI_LE_Set_Privacy_Mode command is used to allow the Host to "
               "specify the privacy mode for an entry on the resolving list.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Set_Privacy_Mode", .OCF = 0x004e, .numcoe = 3, .numret = 1, .octet = 39, .bit = 2, .Cond = { "E", "C.9" },
        .CoE = (HCI_Field_t[3]) {
          { .name = "Peer_Identity_Address_Type",                     .octets =   1 },
          { .name = "Peer_Identity_Address",                          .octets =   6 },
          { .name = "Privacy_Mode",                                   .octets =   1 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Set Random Address command", .iscmd = 1, .OGF = 0x08, .section = "7.8.4", .version = "4.0", .BREDR = "E", .LE = "C.1",
    .summary = "The HCI_LE_Set_Random_Address command will set the Random Device "
               "Address that may be used in a packet sent on the advertising physical "
               "channel.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Set_Random_Address", .OCF = 0x0005, .numcoe = 1, .numret = 1, .octet = 25, .bit = 4, .Cond = { "E", "C.1" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Random_Address",                                 .octets =   6 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Set Resolvable Private Address Timeout command", .iscmd = 1, .OGF = 0x08, .section = "7.8.45", .version = "4.2", .BREDR = "E", .LE = "C.9",
    .summary = "The HCI_LE_Set_Resolvable_Private_Address_Timeout sets the length of "
               "time the Controller uses a random private address before a new random "
               "private address is generated and starts being used.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Set_Resolvable_Private_Address_Timeout", .OCF = 0x002e, .numcoe = 1, .numret = 1, .octet = 35, .bit = 2, .Cond = { "E", "C.9" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "RPA_Timeout",                                    .octets =   2 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Set Scan Enable command", .iscmd = 1, .OGF = 0x08, .section = "7.8.11", .version = "4.0", .BREDR = "E", .LE = "C.98",
    .summary = "The HCI_LE_Set_Scan_Enable command will enable or disable scanning.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Set_Scan_Enable", .OCF = 0x000c, .numcoe = 2, .numret = 1, .octet = 26, .bit = 3, .Cond = { "E", "C.98" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "LE_Scan_Enable",                                 .octets =   1 },
          { .name = "Filter_Duplicates",                              .octets =   1 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 3, .Events = (HCI_Event_t[3]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_LE_Advertising_Report" },
      { .name = "HCI_LE_Directed_Advertising_Report" },
    },
  },

  { .title = "LE Set Scan Parameters command", .iscmd = 1, .OGF = 0x08, .section = "7.8.10", .version = "4.0", .BREDR = "E", .LE = "C.98",
    .summary = "The HCI_LE_Set_Scan_Parameters command will set the parameters used "
               "for scanning.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Set_Scan_Parameters", .OCF = 0x000b, .numcoe = 5, .numret = 1, .octet = 26, .bit = 2, .Cond = { "E", "C.98" },
        .CoE = (HCI_Field_t[5]) {
          { .name = "LE_Scan_Type",                                   .octets =   1 },
          { .name = "LE_Scan_Interval",                               .octets =   2 },
          { .name = "LE_Scan_Window",                                 .octets =   2 },
          { .name = "Own_Address_Type",                               .octets =   1 },
          { .name = "Scanning_Filter_Policy",                         .octets =   1 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Set Scan Response Data command", .iscmd = 1, .OGF = 0x08, .section = "7.8.8", .version = "4.0", .BREDR = "E", .LE = "C.15",
    .summary = "The HCI_LE_Set_Scan_Response_Data command will set the data "
               "transmitted in a scan response.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Set_Scan_Response_Data", .OCF = 0x0009, .numcoe = 2, .numret = 1, .octet = 26, .bit = 0, .Cond = { "E", "C.15" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Scan_Response_Data_Length",                      .octets =   1 },
          { .name = "Scan_Response_Data",                             .octets =  31 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Set Transmit Power Reporting Enable command", .iscmd = 1, .OGF = 0x08, .section = "7.8.121", .version = "5.2", .BREDR = "E", .LE = "C.51",
    .summary = "The HCI_LE_Set_Transmit_Power_Reporting_Enable command is used to "
               "enable or disable reporting to the local Host of transmit power level "
               "changes on an ACL connection.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Set_Transmit_Power_Reporting_Enable", .OCF = 0x007a, .numcoe = 3, .numret = 2, .octet = 44, .bit = 7, .Cond = { "E", "C.51" },
        .CoE = (HCI_Field_t[3]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Local_Enable",                                   .octets =   1 },
          { .name = "Remote_Enable",                                  .octets =   1 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_LE_Transmit_Power_Reporting" },
    },
  },

  { .title = "LE Setup ISO Data Path command", .iscmd = 1, .OGF = 0x08, .section = "7.8.109", .version = "5.2", .BREDR = "E", .LE = "C.47",
    .summary = "The HCI_LE_Setup_ISO_Data_Path command identifies and creates the "
               "isochronous data path between the Host and the Controller and "
               "optionally configures the codec in the Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Setup_ISO_Data_Path", .OCF = 0x006e, .numcoe = 7, .numret = 2, .octet = 43, .bit = 3, .Cond = { "E", "C.47" },
        .CoE = (HCI_Field_t[7]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Data_Path_Direction",                            .octets =   1 },
          { .name = "Data_Path_ID",                                   .octets =   1 },
          { .name = "Codec_ID",                                       .octets =   5 },
          { .name = "Controller_Delay",                               .octets =   3 },
          { .name = "Codec_Configuration_Length",                     .octets =   1 },
          { .name = "Codec_Configuration",                                          .ref2size = "Codec_Configuration_Length" },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Subrate Change event", .iscmd = 0, .section = "7.7.65.35", .version = "5.3", .BREDR = "E", .LE = "C.57",
    .summary = "The HCI_LE_Subrate_Change event indicates that a new subrate factor "
               "has been applied to an existing ACL connection.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Subrate_Change", .code = 0x003e, .subevent = 0x23, .numcoe = 7, .numret = 0,  .Cond = { "E", "C.57" },
        .CoE = (HCI_Field_t[7]) {
          { .name = "Subevent_Code",                                  .octets =   1 },
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Subrate_Factor",                                 .octets =   2 },
          { .name = "Peripheral_Latency",                             .octets =   2 },
          { .name = "Continuation_Number",                            .octets =   2 },
          { .name = "Supervision_Timeout",                            .octets =   2 },
        },
      },
    },
  },

  { .title = "LE Subrate Request command", .iscmd = 1, .OGF = 0x08, .section = "7.8.124", .version = "5.3", .BREDR = "E", .LE = "C.57",
    .summary = "The HCI_LE_Subrate_Request command modifies an existing ACL "
               "connection by applying a subrate factor.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Subrate_Request", .OCF = 0x007e, .numcoe = 6, .numret = 0, .octet = 46, .bit = 1, .Cond = { "E", "C.57" },
        .CoE = (HCI_Field_t[6]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Subrate_Min",                                    .octets =   2 },
          { .name = "Subrate_Max",                                    .octets =   2 },
          { .name = "Max_Latency",                                    .octets =   2 },
          { .name = "Continuation_Number",                            .octets =   2 },
          { .name = "Supervision_Timeout",                            .octets =   2 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_LE_Subrate_Change" },
    },
  },

  { .title = "LE Terminate BIG command", .iscmd = 1, .OGF = 0x08, .section = "7.8.105", .version = "5.2", .BREDR = "E", .LE = "C.41",
    .summary = "The HCI_LE_Terminate_BIG command terminates the transmission of all "
               "BISes of a BIG or cancels the process of creating a BIG.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Terminate_BIG", .OCF = 0x006a, .numcoe = 2, .numret = 0, .octet = 42, .bit = 7, .Cond = { "E", "C.41" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "BIG_Handle",                                     .octets =   1 },
          { .name = "Reason",                                         .octets =   1 },
        },
      },
    },
    .numevts = 3, .Events = (HCI_Event_t[3]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_LE_Terminate_BIG_Complete" },
      { .name = "HCI_LE_Create_BIG_Complete" },
    },
  },

  { .title = "LE Terminate BIG Complete event", .iscmd = 0, .section = "7.7.65.28", .version = "5.2", .BREDR = "E", .LE = "C.41",
    .summary = "The HCI_LE_Terminate_BIG_Complete event indicates that the "
               "transmission of all the BISes in the BIG have been terminated.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Terminate_BIG_Complete", .code = 0x003e, .subevent = 0x1c, .numcoe = 3, .numret = 0,  .Cond = { "E", "C.41" },
        .CoE = (HCI_Field_t[3]) {
          { .name = "Subevent_Code",                                  .octets =   1 },
          { .name = "BIG_Handle",                                     .octets =   1 },
          { .name = "Reason",                                         .octets =   1 },
        },
      },
    },
  },

  { .title = "LE Test End command", .iscmd = 1, .OGF = 0x08, .section = "7.8.30", .version = "4.0", .BREDR = "E", .LE = "M",
    .summary = "The HCI_LE_Test_End command will end the current the receiver or "
               "transmitter test.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Test_End", .OCF = 0x001f, .numcoe = 0, .numret = 2, .octet = 28, .bit = 6, .Cond = { "E", "M" },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Num_Packets",                                    .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Transmit Power Reporting event", .iscmd = 0, .section = "7.7.65.33", .version = "5.2", .BREDR = "E", .LE = "C.51",
    .summary = "The HCI_LE_Transmit_Power_Reporting event is used to report the "
               "transmit power level on the ACL connection.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Transmit_Power_Reporting", .code = 0x003e, .subevent = 0x21, .numcoe = 8, .numret = 0,  .Cond = { "E", "C.51" },
        .CoE = (HCI_Field_t[8]) {
          { .name = "Subevent_Code",                                  .octets =   1 },
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Reason",                                         .octets =   1 },
          { .name = "PHY",                                            .octets =   1 },
          { .name = "TX_Power_Level",                                 .octets =   1 },
          { .name = "TX_Power_Level_Flag",                            .octets =   1 },
          { .name = "Delta",                                          .octets =   1 },
        },
      },
    },
  },

  { .title = "LE Transmitter Test command", .iscmd = 1, .OGF = 0x08, .section = "7.8.29", .version = "4.0 5.0 5.1 5.2", .BREDR = "E", .LE = "[v1] C.1 [v2] C.12 [v3] C.29 [v4] C.53",
    .summary = "The HCI_LE_Transmitter_Test command will run the LE transmitter "
               "test.",

    .numinst = 4, .Inst = (HCI_Inst_t[4]) {
      { .name = "HCI_LE_Transmitter_Test [v4]", .OCF = 0x007b, .numcoe = 9, .numret = 1, .octet = 45, .bit = 0, .Cond = { "E", "C.53" },
        .CoE = (HCI_Field_t[9]) {
          { .name = "TX_Channel",                                     .octets =   1 },
          { .name = "Test_Data_Length",                               .octets =   1 },
          { .name = "Packet_Payload",                                 .octets =   1 },
          { .name = "PHY",                                            .octets =   1 },
          { .name = "CTE_Length",                                     .octets =   1 },
          { .name = "CTE_Type",                                       .octets =   1 },
          { .name = "Switching_Pattern_Length",                       .octets =   1 },
          { .name = "Antenna_IDs",                                    .octets =   1, .ref2size = "Switching_Pattern_Length" },
          { .name = "TX_Power_Level",                                 .octets =   1 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
      { .name = "HCI_LE_Transmitter_Test [v3]", .OCF = 0x0050, .numcoe = 8, .numret = 1, .octet = 39, .bit = 4, .Cond = { "E", "C.29" },
        .CoE = (HCI_Field_t[8]) {
          { .name = "TX_Channel",                                     .octets =   1 },
          { .name = "Test_Data_Length",                               .octets =   1 },
          { .name = "Packet_Payload",                                 .octets =   1 },
          { .name = "PHY",                                            .octets =   1 },
          { .name = "CTE_Length",                                     .octets =   1 },
          { .name = "CTE_Type",                                       .octets =   1 },
          { .name = "Switching_Pattern_Length",                       .octets =   1 },
          { .name = "Antenna_IDs",                                    .octets =   1, .ref2size = "Switching_Pattern_Length" },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
      { .name = "HCI_LE_Transmitter_Test [v2]", .OCF = 0x0034, .numcoe = 4, .numret = 1, .octet = 36, .bit = 0, .Cond = { "E", "C.12" },
        .CoE = (HCI_Field_t[4]) {
          { .name = "TX_Channel",                                     .octets =   1 },
          { .name = "Test_Data_Length",                               .octets =   1 },
          { .name = "Packet_Payload",                                 .octets =   1 },
          { .name = "PHY",                                            .octets =   1 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
      { .name = "HCI_LE_Transmitter_Test [v1]", .OCF = 0x001e, .numcoe = 3, .numret = 1, .octet = 28, .bit = 5, .Cond = { "E", "C.1" },
        .CoE = (HCI_Field_t[3]) {
          { .name = "TX_Channel",                                     .octets =   1 },
          { .name = "Test_Data_Length",                               .octets =   1 },
          { .name = "Packet_Payload",                                 .octets =   1 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "LE Write RF Path Compensation command", .iscmd = 1, .OGF = 0x08, .section = "7.8.76", .version = "5.0", .BREDR = "E", .LE = "C.22",
    .summary = "The HCI_LE_Write_RF_Path_Compensation command is used to indicate the "
               "RF path gain or loss from the RF transceiver output to the antenna "
               "output contributed by intermediate components.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Write_RF_Path_Compensation", .OCF = 0x004d, .numcoe = 2, .numret = 1, .octet = 39, .bit = 1, .Cond = { "E", "C.22" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "RF_TX_Path_Compensation_Value",                  .octets =   2 },
          { .name = "RF_RX_Path_Compensation_Value",                  .octets =   2 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_LE_Transmit_Power_Reporting" },
    },
  },

  { .title = "LE Write Suggested Default Data Length command", .iscmd = 1, .OGF = 0x08, .section = "7.8.35", .version = "4.2", .BREDR = "E", .LE = "C.8",
    .summary = "The HCI_LE_Write_Suggested_Default_Data_Length command allows the "
               "Host to suggest initial MaxTxOctets and MaxTxTime values for new "
               "connections.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_LE_Write_Suggested_Default_Data_Length", .OCF = 0x0024, .numcoe = 2, .numret = 1, .octet = 34, .bit = 0, .Cond = { "E", "C.8" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Suggested_Max_TX_Octets",                        .octets =   2 },
          { .name = "Suggested_Max_TX_Time",                          .octets =   2 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Link Key Notification event", .iscmd = 0, .section = "7.7.24", .version = "1.1", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Link_Key_Notification event is used to indicate to the Host "
               "that a new Link Key has been created for the connection with the "
               "BR/EDR Controller specified in BD_ADDR.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Link_Key_Notification", .code = 0x0018, .numcoe = 3, .numret = 0,  .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[3]) {
          { .name = "BD_ADDR",                                        .octets =   6 },
          { .name = "Link_Key",                                       .octets =  16 },
          { .name = "Key_Type",                                       .octets =   1 },
        },
      },
    },
  },

  { .title = "Link Key Request event", .iscmd = 0, .section = "7.7.23", .version = "1.1", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Link_Key_Request event is used to indicate that a Link Key is "
               "required for the connection with the device specified in BD_ADDR.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Link_Key_Request", .code = 0x0017, .numcoe = 1, .numret = 0,  .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "BD_ADDR",                                        .octets =   6 },
        },
      },
    },
  },

  { .title = "Link Key Request Negative Reply command", .iscmd = 1, .OGF = 0x01, .section = "7.1.11", .version = "1.1", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Link_Key_Request_Negative_Reply command is used to reply to "
               "an HCI_Link_Key_Request event from the BR/EDR Controller if the Host "
               "does not have a stored Link Key for the connection with the other "
               "BR/EDR Controller specified by BD_ADDR.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Link_Key_Request_Negative_Reply", .OCF = 0x000c, .numcoe = 1, .numret = 2, .octet =  1, .bit = 3, .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "BD_ADDR",                                        .octets =   6 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "BD_ADDR",                                        .octets =   6 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Link Key Request Reply command", .iscmd = 1, .OGF = 0x01, .section = "7.1.10", .version = "1.1", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Link_Key_Request_Reply command is used to reply to an "
               "HCI_Link_Key_Request event from the BR/EDR Controller, and specifies "
               "the Link Key stored on the Host to be used as the link key for the "
               "connection with the other BR/EDR Controller specified by BD_ADDR.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Link_Key_Request_Reply", .OCF = 0x000b, .numcoe = 2, .numret = 2, .octet =  1, .bit = 2, .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "BD_ADDR",                                        .octets =   6 },
          { .name = "Link_Key",                                       .octets =  16 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "BD_ADDR",                                        .octets =   6 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Link Key Selection command", .iscmd = 1, .OGF = 0x01, .section = "7.1.18", .version = "1.1", .BREDR = "C.215", .LE = "E",
    .summary = "The HCI_Link_Key_Selection command is used to force both BR/EDR "
               "Controllers of a connection associated to the Connection_Handle to "
               "use the temporary link key of the Central or the regular link keys.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Link_Key_Selection", .OCF = 0x0017, .numcoe = 1, .numret = 0, .octet =  2, .bit = 2, .Cond = { "C.215", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Key_Flag",                                       .octets =   1 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Link_Key_Type_Changed" },
    },
  },

  { .title = "Link Key Type Changed event", .iscmd = 0, .section = "7.7.10", .version = "1.1", .BREDR = "C.215", .LE = "E",
    .summary = "The HCI_Link_Key_Type_Changed event is used to indicate that the "
               "change in the temporary Link Key or in the semi-permanent link keys "
               "on the Bluetooth Central side has been completed.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Link_Key_Type_Changed", .code = 0x000a, .numcoe = 3, .numret = 0,  .Cond = { "C.215", "E" },
        .CoE = (HCI_Field_t[3]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Key_Flag",                                       .octets =   1 },
        },
      },
    },
  },

  { .title = "Link Supervision Timeout Changed event", .iscmd = 0, .section = "7.7.46", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Link_Supervision_Timeout_Changed event indicates that the "
               "remote device changed the Link Supervision Timeout.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Link_Supervision_Timeout_Changed", .code = 0x0038, .numcoe = 2, .numret = 0,  .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Link_Supervision_Timeout",                       .octets =   2 },
        },
      },
    },
  },

  { .title = "Loopback Command event", .iscmd = 0, .section = "7.7.25", .version = "1.1", .BREDR = "C.123", .LE = "E",
    .summary = "The HCI_Loopback_Command event is used to loop back all commands that "
               "the Host sends to the BR/EDR Controller with some exceptions.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Loopback_Command", .code = 0x0019, .numcoe = 1, .numret = 0,  .Cond = { "C.123", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "HCI_Command_Packet",                                           },
        },
      },
    },
  },

  { .title = "Max Slots Change event", .iscmd = 0, .section = "7.7.27", .version = "1.1", .BREDR = "C.132", .LE = "E",
    .summary = "The HCI_Max_Slots_Change event is used to indicate a change in the "
               "max slots by the LM.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Max_Slots_Change", .code = 0x001b, .numcoe = 2, .numret = 0,  .Cond = { "C.132", "E" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "LMP_Max_Slots",                                  .octets =   1 },
        },
      },
    },
  },

  { .title = "Mode Change event", .iscmd = 0, .section = "7.7.20", .version = "1.1", .BREDR = "C.144", .LE = "E",
    .summary = "The HCI_Mode_Change event is used to indicate that the current mode "
               "has changed.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Mode_Change", .code = 0x0014, .numcoe = 4, .numret = 0,  .Cond = { "C.144", "E" },
        .CoE = (HCI_Field_t[4]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Current_Mode",                                   .octets =   1 },
          { .name = "Interval",                                       .octets =   2 },
        },
      },
    },
  },

  { .title = "Number Of Completed Data Blocks event", .iscmd = 0, .section = "7.7.59", .version = "3.0 + HS", .BREDR = "C.124", .LE = "E",
    .summary = "The HCI_Number_Of_Completed_Data_Blocks event is used by the "
               "Controller to indicate to the Host how many HCI ACL Data packets have "
               "been completed and how many data block buffers have been freed for "
               "each Handle since the previous HCI_Number_Of_Completed_Data_Blocks "
               "event was sent.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Number_Of_Completed_Data_Blocks", .code = 0x0048, .numcoe = 5, .numret = 0,  .Cond = { "C.124", "E" },
        .CoE = (HCI_Field_t[5]) {
          { .name = "Total_Num_Data_Blocks",                          .octets =   2 },
          { .name = "Num_Handles",                                    .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2, .ref2size = "Num_Handles" },
          { .name = "Num_Completed_Packets",                          .octets =   2, .ref2size = "Num_Handles" },
          { .name = "Num_Completed_Blocks",                           .octets =   2, .ref2size = "Num_Handles" },
        },
      },
    },
  },

  { .title = "Number Of Completed Packets event", .iscmd = 0, .section = "7.7.19", .version = "1.1", .BREDR = "M", .LE = "C.3",
    .summary = "The HCI_Number_Of_Completed_Packets event is used by the Controller "
               "to indicate to the Host how many HCI Data packets have been completed "
               "for each Connection_Handle since the previous "
               "HCI_Number_Of_Completed_Packets event was sent.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Number_Of_Completed_Packets", .code = 0x0013, .numcoe = 3, .numret = 0,  .Cond = { "M", "C.3" },
        .CoE = (HCI_Field_t[3]) {
          { .name = "Num_Handles",                                    .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2, .ref2size = "Num_Handles" },
          { .name = "Num_Completed_Packets",                          .octets =   2, .ref2size = "Num_Handles" },
        },
      },
    },
  },

  { .title = "Page Scan Repetition Mode Change event", .iscmd = 0, .section = "7.7.31", .version = "1.1", .BREDR = "O", .LE = "E",
    .summary = "The HCI_Page_Scan_Repetition_Mode_Change event indicates that the "
               "connected remote BR/EDR Controller with the specified "
               "Connection_Handle has successfully changed the Page Scan Repetition "
               "Mode (SR).",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Page_Scan_Repetition_Mode_Change", .code = 0x0020, .numcoe = 2, .numret = 0,  .Cond = { "O", "E" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "BD_ADDR",                                        .octets =   6 },
          { .name = "Page_Scan_Repetition_Mode",                      .octets =   1 },
        },
      },
    },
  },

  { .title = "Periodic Inquiry Mode command", .iscmd = 1, .OGF = 0x01, .section = "7.1.3", .version = "1.1", .BREDR = "C.128", .LE = "E",
    .summary = "The HCI_Periodic_Inquiry_Mode command is used to configure the BR/EDR "
               "Controller to perform an automatic Inquiry based on a specified "
               "period range.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Periodic_Inquiry_Mode", .OCF = 0x0003, .numcoe = 5, .numret = 1, .octet =  0, .bit = 2, .Cond = { "C.128", "E" },
        .CoE = (HCI_Field_t[5]) {
          { .name = "Max_Period_Length",                              .octets =   2 },
          { .name = "Min_Period_Length",                              .octets =   2 },
          { .name = "LAP",                                            .octets =   3 },
          { .name = "Inquiry_Length",                                 .octets =   1 },
          { .name = "Num_Responses",                                  .octets =   1 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 3, .Events = (HCI_Event_t[3]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_Extended_Inquiry_Result" },
      { .name = "HCI_Inquiry_Complete" },
    },
  },

  { .title = "Peripheral Page Response Timeout event", .iscmd = 0, .section = "7.7.72", .version = "CSA4", .BREDR = "O", .LE = "E",
    .summary = "The HCI_Peripheral_Page_Response_Timeout event indicates to the Host "
               "that the pagerespTO has been exceeded on the BR/EDR Controller after "
               "the Controller responded to an ID packet.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Peripheral_Page_Response_Timeout", .code = 0x0054, .numcoe = 0, .numret = 0,  .Cond = { "O", "E" },
      },
    },
  },

  { .title = "PIN Code Request event", .iscmd = 0, .section = "7.7.22", .version = "1.1", .BREDR = "M", .LE = "E",
    .summary = "The HCI_PIN_Code_Request event is used to indicate that a PIN code is "
               "required to create a new link key for a connection.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_PIN_Code_Request", .code = 0x0016, .numcoe = 1, .numret = 0,  .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "BD_ADDR",                                        .octets =   6 },
        },
      },
    },
  },

  { .title = "PIN Code Request Negative Reply command", .iscmd = 1, .OGF = 0x01, .section = "7.1.13", .version = "1.1", .BREDR = "M", .LE = "E",
    .summary = "The HCI_PIN_Code_Request_Negative_Reply command is used to reply to "
               "an HCI_PIN_Code_Request event from the Controller when the Host "
               "cannot specify a PIN code to use for a connection.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_PIN_Code_Request_Negative_Reply", .OCF = 0x000e, .numcoe = 1, .numret = 2, .octet =  1, .bit = 5, .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "BD_ADDR",                                        .octets =   6 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "BD_ADDR",                                        .octets =   6 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "PIN Code Request Reply command", .iscmd = 1, .OGF = 0x01, .section = "7.1.12", .version = "1.1", .BREDR = "M", .LE = "E",
    .summary = "The HCI_PIN_Code_Request_Reply command is used to reply to an "
               "HCI_PIN_Code_Request event from the Controller and specifies the PIN "
               "code to use for a connection.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_PIN_Code_Request_Reply", .OCF = 0x000d, .numcoe = 3, .numret = 2, .octet =  1, .bit = 4, .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[3]) {
          { .name = "BD_ADDR",                                        .octets =   6 },
          { .name = "PIN_Code_Length",                                .octets =   1 },
          { .name = "PIN_Code",                                       .octets =  16 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "BD_ADDR",                                        .octets =   6 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "QoS Setup command", .iscmd = 1, .OGF = 0x02, .section = "7.2.6", .version = "1.1", .BREDR = "M", .LE = "E",
    .summary = "The HCI_QoS_Setup command is used to specify Quality of Service "
               "parameters for a Connection_Handle.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_QoS_Setup", .OCF = 0x0007, .numcoe = 7, .numret = 0, .octet =  4, .bit = 6, .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[7]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Unused",                                         .octets =   1 },
          { .name = "Service_Type",                                   .octets =   1 },
          { .name = "Token_Rate",                                     .octets =   4 },
          { .name = "Peak_Bandwidth",                                 .octets =   4 },
          { .name = "Latency",                                        .octets =   4 },
          { .name = "Delay_Variation",                                .octets =   4 },
        },
      },
    },
    .numevts = 3, .Events = (HCI_Event_t[3]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_QoS_Setup_Complete" },
      { .name = "HCI_Flow_Specification_Complete" },
    },
  },

  { .title = "QoS Setup Complete event", .iscmd = 0, .section = "7.7.13", .version = "1.1", .BREDR = "M", .LE = "E",
    .summary = "The HCI_QoS_Setup_Complete event is used to indicate that QoS is set "
               "up.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_QoS_Setup_Complete", .code = 0x000d, .numcoe = 8, .numret = 0,  .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[8]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Unused",                                         .octets =   1 },
          { .name = "Service_Type",                                   .octets =   1 },
          { .name = "Token_Rate",                                     .octets =   4 },
          { .name = "Peak_Bandwidth",                                 .octets =   4 },
          { .name = "Latency",                                        .octets =   4 },
          { .name = "Delay_Variation",                                .octets =   4 },
        },
      },
    },
  },

  { .title = "QoS Violation event", .iscmd = 0, .section = "7.7.30", .version = "1.1", .BREDR = "M", .LE = "E",
    .summary = "The HCI_QoS_Violation event is used to indicate the Controller's Link "
               "Manager is unable to provide the current QoS requirement for the "
               "Handle.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_QoS_Violation", .code = 0x001e, .numcoe = 1, .numret = 0,  .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Handle",                                         .octets =   2 },
        },
      },
    },
  },

  { .title = "Read AFH Channel Assessment Mode command", .iscmd = 1, .OGF = 0x03, .section = "7.3.53", .version = "1.2", .BREDR = "C.140", .LE = "C.58",
    .summary = "The HCI_Read_AFH_Channel_Assessment_Mode command will read the value "
               "for the AFH Channel Classification Mode parameter. This value is used "
               "to enable or disable the Controller's channel assessment scheme.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_AFH_Channel_Assessment_Mode", .OCF = 0x0048, .numcoe = 0, .numret = 2, .octet = 13, .bit = 2, .Cond = { "C.140", "C.58" },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "AFH_Channel_Assessment_Mode",                    .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read AFH Channel Map command", .iscmd = 1, .OGF = 0x05, .section = "7.5.5", .version = "1.2", .BREDR = "C.139", .LE = "E",
    .summary = "The HCI_Read_AFH_Channel_Map command will read the current state of "
               "the channel map for a connection.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_AFH_Channel_Map", .OCF = 0x0006, .numcoe = 1, .numret = 4, .octet = 15, .bit = 6, .Cond = { "C.139", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Connection_Handle",                              .octets =   2 },
        },
        .Ret = (HCI_Field_t[4]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "AFH_Mode",                                       .octets =   1 },
          { .name = "AFH_Channel_Map",                                .octets =  10 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Authenticated Payload Timeout command", .iscmd = 1, .OGF = 0x03, .section = "7.3.93", .version = "4.1", .BREDR = "C.155", .LE = "C.155",
    .summary = "The HCI_Read_Authenticated_Payload_Timeout command is used to read "
               "the Authenticated Payload Timeout parameter, which is used to set the "
               "maximum time between packets being received from the remote device "
               "without a valid MIC.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Authenticated_Payload_Timeout", .OCF = 0x007b, .numcoe = 1, .numret = 3, .octet = 32, .bit = 4, .Cond = { "C.155", "C.155" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Connection_Handle",                              .octets =   2 },
        },
        .Ret = (HCI_Field_t[3]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Authenticated_Payload_Timeout",                  .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Authentication Enable command", .iscmd = 1, .OGF = 0x03, .section = "7.3.23", .version = "1.1", .BREDR = "C.111", .LE = "E",
    .summary = "The HCI_Read_Authentication_Enable command will read the value for "
               "the Authentication Enable parameter, which controls whether the "
               "Bluetooth device will require authentication for each connection with "
               "other Bluetooth devices.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Authentication_Enable", .OCF = 0x001f, .numcoe = 0, .numret = 2, .octet =  8, .bit = 4, .Cond = { "C.111", "E" },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Authentication_Enable",                          .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Automatic Flush Timeout command", .iscmd = 1, .OGF = 0x03, .section = "7.3.29", .version = "1.1", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Read_Automatic_Flush_Timeout command will read the value for "
               "the Flush Timeout configuration parameter for the specified "
               "Connection_Handle. The Flush Timeout parameter is only used for ACL "
               "connections.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Automatic_Flush_Timeout", .OCF = 0x0027, .numcoe = 1, .numret = 3, .octet =  9, .bit = 4, .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Connection_Handle",                              .octets =   2 },
        },
        .Ret = (HCI_Field_t[3]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Flush_Timeout",                                  .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read BD_ADDR command", .iscmd = 1, .OGF = 0x04, .section = "7.4.6", .version = "1.1", .BREDR = "M", .LE = "M",
    .summary = "The HCI_Read_BD_ADDR command will read the value for the BD_ADDR "
               "parameter.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_BD_ADDR", .OCF = 0x0009, .numcoe = 0, .numret = 2, .octet = 15, .bit = 1, .Cond = { "M", "M" },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "BD_ADDR",                                        .octets =   6 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Buffer Size command", .iscmd = 1, .OGF = 0x04, .section = "7.4.5", .version = "1.1", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Read_Buffer_Size command returns the size of the HCI buffers. "
               "These buffers are used by the Controller to buffer data that is to be "
               "transmitted.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Buffer_Size", .OCF = 0x0005, .numcoe = 0, .numret = 5, .octet = 14, .bit = 7, .Cond = { "M", "E" },
        .Ret = (HCI_Field_t[5]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "ACL_Data_Packet_Length",                         .octets =   2 },
          { .name = "Synchronous_Data_Packet_Length",                 .octets =   1 },
          { .name = "Total_Num_ACL_Data_Packets",                     .octets =   2 },
          { .name = "Total_Num_Synchronous_Data_Packets",             .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Class of Device command", .iscmd = 1, .OGF = 0x03, .section = "7.3.25", .version = "1.1", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Read_Class_of_Device command will read the value for the "
               "Class of Device configuration parameter, which is used to indicate "
               "its capabilities to other devices.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Class_Of_Device", .OCF = 0x0023, .numcoe = 0, .numret = 2, .octet =  9, .bit = 0, .Cond = { "M", "E" },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Class_Of_Device",                                .octets =   3 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Clock command", .iscmd = 1, .OGF = 0x05, .section = "7.5.6", .version = "1.2", .BREDR = "O", .LE = "E",
    .summary = "The HCI_Read_Clock command will read an estimate of a piconet or the "
               "local Bluetooth Clock.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Clock", .OCF = 0x0007, .numcoe = 2, .numret = 4, .octet = 15, .bit = 7, .Cond = { "O", "E" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Which_Clock",                                                  },
        },
        .Ret = (HCI_Field_t[4]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Clock",                                          .octets =   4 },
          { .name = "Accuracy",                                       .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Clock Offset command", .iscmd = 1, .OGF = 0x01, .section = "7.1.24", .version = "1.1", .BREDR = "O", .LE = "E",
    .summary = "The HCI_Read_Clock_Offset command allows the Host to read the clock "
               "offset of remote BR/EDR Controllers.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Clock_Offset", .OCF = 0x001f, .numcoe = 1, .numret = 0, .octet =  3, .bit = 0, .Cond = { "O", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Connection_Handle",                              .octets =   2 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Read_Clock_Offset_Complete" },
    },
  },

  { .title = "Read Clock Offset Complete event", .iscmd = 0, .section = "7.7.28", .version = "1.1", .BREDR = "C.104", .LE = "E",
    .summary = "The HCI_Read_Clock_Offset_Complete event is used to indicate the "
               "completion of the process of the LM obtaining the Clock offset "
               "information.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Clock_Offset_Complete", .code = 0x001c, .numcoe = 3, .numret = 0,  .Cond = { "C.104", "E" },
        .CoE = (HCI_Field_t[3]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Clock_Offset",                                   .octets =   2 },
        },
      },
    },
  },

  { .title = "Read Connection Accept Timeout command", .iscmd = 1, .OGF = 0x03, .section = "7.3.13", .version = "1.1", .BREDR = "M", .LE = "C.40",
    .summary = "The HCI_Read_Connection_Accept_Timeout command will read the value "
               "for the Connection Accept Timeout configuration parameter, which "
               "allows the Controller to automatically deny a connection request "
               "after a specified period has occurred, and to refuse a new "
               "connection.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Connection_Accept_Timeout", .OCF = 0x0015, .numcoe = 0, .numret = 2, .octet =  7, .bit = 2, .Cond = { "M", "C.40" },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Accept_Timeout",                      .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Current IAC LAP command", .iscmd = 1, .OGF = 0x03, .section = "7.3.44", .version = "1.1", .BREDR = "C.125", .LE = "E",
    .summary = "The HCI_Read_Current_IAC_LAP command will read the LAP(s) used to "
               "create the Inquiry Access Codes (IAC) that the local BR/EDR "
               "Controller is simultaneously scanning for during Inquiry Scans.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Current_IAC_LAP", .OCF = 0x0039, .numcoe = 0, .numret = 3, .octet = 11, .bit = 3, .Cond = { "C.125", "E" },
        .Ret = (HCI_Field_t[3]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Num_Current_IAC",                                .octets =   1 },
          { .name = "IAC_LAP",                                        .octets =   3, .ref2size = "Num_Current_IAC" },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Data Block Size command", .iscmd = 1, .OGF = 0x04, .section = "7.4.7", .version = "3.0 + HS", .BREDR = "C.124", .LE = "E",
    .summary = "The HCI_Read_Data_Block_Size command returns the maximum size of the "
               "HCI buffers. These buffers are used by the Controller to buffer data "
               "that is to be transmitted.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Data_Block_Size", .OCF = 0x000a, .numcoe = 0, .numret = 4, .octet = 23, .bit = 2, .Cond = { "C.124", "E" },
        .Ret = (HCI_Field_t[4]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Max_ACL_Data_Packet_Length",                     .octets =   2 },
          { .name = "Data_Block_Length",                              .octets =   2 },
          { .name = "Total_Num_Data_Blocks",                          .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Default Erroneous Data Reporting command", .iscmd = 1, .OGF = 0x03, .section = "7.3.64", .version = "2.1 + EDR", .BREDR = "C.112", .LE = "E",
    .summary = "The HCI_Read_Default_Erroneous_Data_Reporting command will read the "
               "value for the Erroneous Data Reporting configuration parameter, which "
               "controls whether the BR/EDR Controller will provide data for every "
               "(e)SCO interval, with the Packet_Status_Flag in HCI Synchronous Data "
               "packets set according to HCI Synchronous Data packets.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Default_Erroneous_Data_Reporting", .OCF = 0x005a, .numcoe = 0, .numret = 2, .octet = 18, .bit = 2, .Cond = { "C.112", "E" },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Erroneous_Data_Reporting",                       .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Default Link Policy Settings command", .iscmd = 1, .OGF = 0x02, .section = "7.2.11", .version = "1.2", .BREDR = "C.141", .LE = "E",
    .summary = "The HCI_Read_Default_Link_Policy_Settings command will read the "
               "Default Link Policy configuration parameter for all new connections.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Default_Link_Policy_Settings", .OCF = 0x000e, .numcoe = 0, .numret = 2, .octet =  5, .bit = 3, .Cond = { "C.141", "E" },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Default_Link_Policy_Settings",                   .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Encryption Key Size command", .iscmd = 1, .OGF = 0x05, .section = "7.5.7", .version = "3.0 + HS", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Read_Encryption_Key_Size command is used to read the "
               "encryption key size on a given Connection_Handle.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Encryption_Key_Size", .OCF = 0x0008, .numcoe = 1, .numret = 3, .octet = 20, .bit = 4, .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Connection_Handle",                              .octets =   2 },
        },
        .Ret = (HCI_Field_t[3]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Key_Size",                                       .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Enhanced Transmit Power Level command", .iscmd = 1, .OGF = 0x03, .section = "7.3.74", .version = "3.0 + HS", .BREDR = "C.217", .LE = "E",
    .summary = "The HCI_Read_Enhanced_Transmit_Power_Level command will read the "
               "values for the GFSK, /4-DQPSK and 8DPSK Transmit Power Level "
               "parameters for the specified Connection_Handle.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Enhanced_Transmit_Power_Level", .OCF = 0x0068, .numcoe = 2, .numret = 5, .octet = 24, .bit = 0, .Cond = { "C.217", "E" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Type",                                           .octets =   1 },
        },
        .Ret = (HCI_Field_t[5]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "TX_Power_Level_GFSK",                            .octets =   1 },
          { .name = "TX_Power_Level_DQPSK",                           .octets =   1 },
          { .name = "TX_Power_Level_8DPSK",                           .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Extended Inquiry Length command", .iscmd = 1, .OGF = 0x03, .section = "7.3.98", .version = "4.1", .BREDR = "C.113", .LE = "E",
    .summary = "The HCI_Read_Extended_Inquiry_Length command is used to read the "
               "Extended Inquiry Length parameter from the Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Extended_Inquiry_Length", .OCF = 0x0080, .numcoe = 0, .numret = 2, .octet = 33, .bit = 2, .Cond = { "C.113", "E" },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Extended_Inquiry_Length",                        .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Extended Inquiry Response command", .iscmd = 1, .OGF = 0x03, .section = "7.3.55", .version = "2.1 + EDR", .BREDR = "C.205", .LE = "E",
    .summary = "The HCI_Read_Extended_Inquiry_Response command will read the data "
               "that the BR/EDR Controller sends in the extended inquiry response "
               "packet during inquiry response.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Extended_Inquiry_Response", .OCF = 0x0051, .numcoe = 0, .numret = 3, .octet = 17, .bit = 0, .Cond = { "C.205", "E" },
        .Ret = (HCI_Field_t[3]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "FEC_Required",                                   .octets =   1 },
          { .name = "Extended_Inquiry_Response",                      .octets = 240 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Extended Page Timeout command", .iscmd = 1, .OGF = 0x03, .section = "7.3.96", .version = "4.1", .BREDR = "C.114", .LE = "E",
    .summary = "The HCI_Read_Extended_Page_Timeout command is used to read the "
               "Extended Page Timeout parameter from the Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Extended_Page_Timeout", .OCF = 0x007e, .numcoe = 0, .numret = 2, .octet = 33, .bit = 0, .Cond = { "C.114", "E" },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Extended_Page_Timeout",                          .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Failed Contact Counter command", .iscmd = 1, .OGF = 0x05, .section = "7.5.1", .version = "1.1", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Read_Failed_Contact_Counter command will read the value for "
               "the Failed Contact Counter configuration parameter for a particular "
               "connection to another device.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Failed_Contact_Counter", .OCF = 0x0001, .numcoe = 1, .numret = 3, .octet = 15, .bit = 2, .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Handle",                                         .octets =   2 },
        },
        .Ret = (HCI_Field_t[3]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Handle",                                         .octets =   2 },
          { .name = "Failed_Contact_Counter",                         .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Flow Control Mode command", .iscmd = 1, .OGF = 0x03, .section = "7.3.72", .version = "3.0 + HS", .BREDR = "C.124", .LE = "E",
    .summary = "The HCI_Read_Flow_Control_Mode command returns the value of the "
               "Flow_Control_Mode configuration parameter supported by this "
               "Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Flow_Control_Mode", .OCF = 0x0066, .numcoe = 0, .numret = 2, .octet = 23, .bit = 0, .Cond = { "C.124", "E" },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Flow_Control_Mode",                              .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Hold Mode Activity command", .iscmd = 1, .OGF = 0x03, .section = "7.3.33", .version = "1.1", .BREDR = "C.213", .LE = "E",
    .summary = "The HCI_Read_Hold_Mode_ Activity command is used to read which "
               "activities should be suspended when the BR/EDR Controller is in Hold "
               "mode.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Hold_Mode_Activity", .OCF = 0x002b, .numcoe = 0, .numret = 2, .octet = 10, .bit = 0, .Cond = { "C.213", "E" },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Hold_Mode_Activity",                             .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Inquiry Mode command", .iscmd = 1, .OGF = 0x03, .section = "7.3.49", .version = "1.2", .BREDR = "C.115", .LE = "E",
    .summary = "The HCI_Read_Inquiry_Mode command is used to read the Inquiry Mode "
               "configuration parameter of the local BR/EDR Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Inquiry_Mode", .OCF = 0x0044, .numcoe = 0, .numret = 2, .octet = 12, .bit = 6, .Cond = { "C.115", "E" },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Inquiry_Mode",                                   .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Inquiry Response Transmit Power Level command", .iscmd = 1, .OGF = 0x03, .section = "7.3.61", .version = "2.1 + EDR", .BREDR = "C.125", .LE = "E",
    .summary = "The HCI_Read_Inquiry_Response_Transmit_Power_Level command will read "
               "the inquiry response Transmit Power level used to transmit the FHS "
               "and EIR data packets. This can be used directly in the Tx Power Level "
               "EIR data type.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Inquiry_Response_Transmit_Power_Level", .OCF = 0x0058, .numcoe = 0, .numret = 2, .octet = 18, .bit = 0, .Cond = { "C.125", "E" },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "TX_Power",                                       .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Inquiry Scan Activity command", .iscmd = 1, .OGF = 0x03, .section = "7.3.21", .version = "1.1", .BREDR = "C.125", .LE = "E",
    .summary = "The HCI_Read_Inquiry_Scan_Activity command will read the value for "
               "Inquiry Scan Interval and Inquiry Scan Window configuration "
               "parameters. Inquiry Scan Interval defines the amount of time between "
               "consecutive inquiry scans. Inquiry Scan Window defines the amount of "
               "time for the duration of the inquiry scan.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Inquiry_Scan_Activity", .OCF = 0x001d, .numcoe = 0, .numret = 3, .octet =  8, .bit = 2, .Cond = { "C.125", "E" },
        .Ret = (HCI_Field_t[3]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Inquiry_Scan_Interval",                          .octets =   2 },
          { .name = "Inquiry_Scan_Window",                            .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Inquiry Scan Type command", .iscmd = 1, .OGF = 0x03, .section = "7.3.47", .version = "1.2", .BREDR = "C.125", .LE = "E",
    .summary = "The HCI_Read_Inquiry_Scan_Type command is used to read the Inquiry "
               "Scan Type configuration parameter of the local BR/EDR Controller. The "
               "Inquiry Scan Type configuration parameter can set the inquiry scan to "
               "either normal or interlaced scan.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Inquiry_Scan_Type", .OCF = 0x0042, .numcoe = 0, .numret = 2, .octet = 12, .bit = 4, .Cond = { "C.125", "E" },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Inquiry_Scan_Type",                              .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read LE Host Support command", .iscmd = 1, .OGF = 0x03, .section = "7.3.78", .version = "4.0", .BREDR = "C.116", .LE = "E",
    .summary = "The HCI_Read_LE_Host_Support command reads the LE Supported Host "
               "setting from the BR/ EDR Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_LE_Host_Support", .OCF = 0x006c, .numcoe = 0, .numret = 3, .octet = 24, .bit = 5, .Cond = { "C.116", "E" },
        .Ret = (HCI_Field_t[3]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "LE_Supported_Host",                              .octets =   1 },
          { .name = "Unused",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Link Policy Settings command", .iscmd = 1, .OGF = 0x02, .section = "7.2.9", .version = "1.1", .BREDR = "C.141", .LE = "E",
    .summary = "The HCI_Read_Link_Policy_Settings command will read the Link Policy "
               "configuration parameter for the specified Connection_Handle. The Link "
               "Policy settings allow the Host to specify which Link Modes the Link "
               "Manager can use for the specified Connection_Handle.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Link_Policy_Settings", .OCF = 0x000c, .numcoe = 1, .numret = 3, .octet =  5, .bit = 1, .Cond = { "C.141", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Connection_Handle",                              .octets =   2 },
        },
        .Ret = (HCI_Field_t[3]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Link_Policy_Settings",                           .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Link Quality command", .iscmd = 1, .OGF = 0x05, .section = "7.5.3", .version = "1.1", .BREDR = "O", .LE = "E",
    .summary = "The HCI_Read_Link_Quality command will read the value for the Link "
               "Quality for the specified Connection_Handle.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Link_Quality", .OCF = 0x0003, .numcoe = 1, .numret = 3, .octet = 15, .bit = 4, .Cond = { "O", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Handle",                                         .octets =   2 },
        },
        .Ret = (HCI_Field_t[3]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Handle",                                         .octets =   2 },
          { .name = "Link_Quality",                                   .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Link Supervision Timeout command", .iscmd = 1, .OGF = 0x03, .section = "7.3.41", .version = "1.1", .BREDR = "C.117", .LE = "E",
    .summary = "The HCI_Read_Link_Supervision_Timeout command will read the value for "
               "the Link Supervision Timeout configuration parameter for the device. "
               "This parameter is used by the Controller to determine link loss.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Link_Supervision_Timeout", .OCF = 0x0036, .numcoe = 1, .numret = 3, .octet = 11, .bit = 0, .Cond = { "C.117", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Handle",                                         .octets =   2 },
        },
        .Ret = (HCI_Field_t[3]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Handle",                                         .octets =   2 },
          { .name = "Link_Supervision_Timeout",                       .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read LMP Handle command", .iscmd = 1, .OGF = 0x01, .section = "7.1.25", .version = "1.2", .BREDR = "C.134", .LE = "E",
    .summary = "The HCI_Read_LMP_Handle command will read the current LMP Handle "
               "associated with the Connection_Handle.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_LMP_Handle", .OCF = 0x0020, .numcoe = 1, .numret = 4, .octet =  3, .bit = 1, .Cond = { "C.134", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Connection_Handle",                              .octets =   2 },
        },
        .Ret = (HCI_Field_t[4]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "LMP_Handle",                                     .octets =   1 },
          { .name = "Reserved",                                       .octets =   4 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Local Extended Features command", .iscmd = 1, .OGF = 0x04, .section = "7.4.4", .version = "1.2", .BREDR = "C.220", .LE = "E",
    .summary = "The HCI_Read_Local_Extended_Features command requests a list of the "
               "supported extended features for the local device.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Local_Extended_Features", .OCF = 0x0004, .numcoe = 1, .numret = 4, .octet = 14, .bit = 6, .Cond = { "C.220", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Page_Number",                                    .octets =   1 },
        },
        .Ret = (HCI_Field_t[4]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Page_Number",                                    .octets =   1 },
          { .name = "Max_Page_Number",                                .octets =   1 },
          { .name = "Extended_LMP_Features",                          .octets =   8 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Local Name command", .iscmd = 1, .OGF = 0x03, .section = "7.3.12", .version = "1.1", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Read_Local_Name command provides the ability to read the "
               "stored user-friendly name for the BR/EDR Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Local_Name", .OCF = 0x0014, .numcoe = 0, .numret = 2, .octet =  7, .bit = 1, .Cond = { "M", "E" },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Local_Name",                                     .octets = 248 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Local OOB Data command", .iscmd = 1, .OGF = 0x03, .section = "7.3.60", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Read_Local_OOB_Data command is used to obtain a Secure Simple "
               "Pairing Hash C and Randomizer R which are intended to be transferred "
               "to a remote device using an OOB mechanism.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Local_OOB_Data", .OCF = 0x0057, .numcoe = 0, .numret = 3, .octet = 17, .bit = 7, .Cond = { "M", "E" },
        .Ret = (HCI_Field_t[3]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "C",                                              .octets =  16 },
          { .name = "R",                                              .octets =  16 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Local OOB Extended Data command", .iscmd = 1, .OGF = 0x03, .section = "7.3.95", .version = "4.1", .BREDR = "C.142", .LE = "E",
    .summary = "The HCI_Read_Local_OOB_Extended_Data command is used to obtain a "
               "Secure Simple Pairing Hash C and Randomizer R associated with both "
               "P-192 and P-256 public keys, which are intended to be transferred to "
               "a remote device using an OOB mechanism.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Local_OOB_Extended_Data", .OCF = 0x007d, .numcoe = 0, .numret = 5, .octet = 32, .bit = 6, .Cond = { "C.142", "E" },
        .Ret = (HCI_Field_t[5]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "C_192",                                          .octets =  16 },
          { .name = "R_192",                                          .octets =  16 },
          { .name = "C_256",                                          .octets =  16 },
          { .name = "R_256",                                          .octets =  16 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Local Simple Pairing Options command", .iscmd = 1, .OGF = 0x04, .section = "7.4.9", .version = "Erratum 10734", .BREDR = "O", .LE = "E",
    .summary = "The HCI_Read_Local_Simple_Pairing_Options command is used to read the "
               "Secure Simple Pairing options and the maximum encryption key size "
               "supported.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Local_Simple_Pairing_Options", .OCF = 0x000c, .numcoe = 0, .numret = 3, .octet = 41, .bit = 3, .Cond = { "O", "E" },
        .Ret = (HCI_Field_t[3]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Simple_Pairing_Options",                         .octets =   1 },
          { .name = "Max_Encryption_Key_Size",                        .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Local Supported Codec Capabilities command", .iscmd = 1, .OGF = 0x04, .section = "7.4.10", .version = "5.2", .BREDR = "C.156", .LE = "C.156",
    .summary = "The HCI_Read_Local_Supported_Codec_Capabilities command is used by a "
               "Host to query codec capabilities.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Local_Supported_Codec_Capabilities", .OCF = 0x000e, .numcoe = 3, .numret = 4, .octet = 45, .bit = 3, .Cond = { "C.156", "C.156" },
        .CoE = (HCI_Field_t[3]) {
          { .name = "Codec_ID",                                       .octets =   5 },
          { .name = "Logical_Transport_Type",                         .octets =   1 },
          { .name = "Direction",                                      .octets =   1 },
        },
        .Ret = (HCI_Field_t[4]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Num_Codec_Capabilities",                         .octets =   1 },
          { .name = "Codec_Capability_Length",                        .octets =   1, .ref2size = "Num_Codec_Capabilities" },
          { .name = "Codec_Capability",                                             .ref2size = "Codec_Capability_Length" },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Local Supported Codecs command", .iscmd = 1, .OGF = 0x04, .section = "7.4.8", .version = "CSA2 5.2", .BREDR = "[v1] C.157 [v2] O", .LE = "[v1] E [v2] O",
    .summary = "The HCI_Read_Local_Supported_Codecs command is used by a Host to "
               "query a Controller's supported codecs.",

    .numinst = 2, .Inst = (HCI_Inst_t[2]) {
      { .name = "HCI_Read_Local_Supported_Codecs [v2]", .OCF = 0x000d, .numcoe = 0, .numret = 7, .octet = 45, .bit = 2, .Cond = { "O", "O" },
        .Ret = (HCI_Field_t[7]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Num_Supported_Standard_Codecs",                  .octets =   1 },
          { .name = "Standard_Codec_ID",                              .octets =   1, .ref2size = "Num_Supported_Standard_Codecs" },
          { .name = "Standard_Codec_Transport",                                     .ref2size = "Num_Supported_Standard_Codecs" },
          { .name = "Num_Supported_Vendor_Specific_Codecs",           .octets =   1 },
          { .name = "Vendor_Specific_Codec_ID",                                     },
          { .name = "Vendor_Specific_Codec_Transport",                              },
        },
      },
      { .name = "HCI_Read_Local_Supported_Codecs [v1]", .OCF = 0x000b, .numcoe = 0, .numret = 5, .octet = 29, .bit = 5, .Cond = { "C.157", "E" },
        .Ret = (HCI_Field_t[5]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Num_Supported_Standard_Codecs",                  .octets =   1 },
          { .name = "Standard_Codec_ID",                              .octets =   1, .ref2size = "Num_Supported_Standard_Codecs" },
          { .name = "Num_Supported_Vendor_Specific_Codecs",           .octets =   1 },
          { .name = "Vendor_Specific_Codec_ID",                                     },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Local Supported Commands command", .iscmd = 1, .OGF = 0x04, .section = "7.4.2", .version = "1.2", .BREDR = "M", .LE = "M",
    .summary = "The HCI_Read_Local_Supported_Commands command requests a list of the "
               "supported HCI commands for the local device.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Local_Supported_Commands", .OCF = 0x0002, .numcoe = 0, .numret = 2, .octet =  0, .bit = 0, .Cond = { "M", "M" },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Supported_Commands",                             .octets =  64 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Local Supported Controller Delay command", .iscmd = 1, .OGF = 0x04, .section = "7.4.11", .version = "5.2", .BREDR = "C.156", .LE = "C.156",
    .summary = "The HCI_Read_Local_Supported_Controller_Delay command is used by a "
               "Host to query a range of supported Controller delays for a given "
               "codec configuration.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Local_Supported_Controller_Delay", .OCF = 0x000f, .numcoe = 5, .numret = 3, .octet = 45, .bit = 4, .Cond = { "C.156", "C.156" },
        .CoE = (HCI_Field_t[5]) {
          { .name = "Codec_ID",                                       .octets =   5 },
          { .name = "Logical_Transport_Type",                         .octets =   1 },
          { .name = "Direction",                                      .octets =   1 },
          { .name = "Codec_Configuration_Length",                     .octets =   1 },
          { .name = "Codec_Configuration",                                          .ref2size = "Codec_Configuration_Length" },
        },
        .Ret = (HCI_Field_t[3]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Min_Controller_Delay",                           .octets =   3 },
          { .name = "Max_Controller_Delay",                           .octets =   3 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Local Supported Features command", .iscmd = 1, .OGF = 0x04, .section = "7.4.3", .version = "1.1", .BREDR = "M", .LE = "M",
    .summary = "The HCI_Read_Local_Supported_Features command requests a list of the "
               "supported features for the local device.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Local_Supported_Features", .OCF = 0x0003, .numcoe = 0, .numret = 2, .octet = 14, .bit = 5, .Cond = { "M", "M" },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "LMP_Features",                                   .octets =   8 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Local Version Information command", .iscmd = 1, .OGF = 0x04, .section = "7.4.1", .version = "1.1", .BREDR = "M", .LE = "M",
    .summary = "The HCI_Read_Local_Version_Information command will read the version "
               "information for the local Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Local_Version_Information", .OCF = 0x0001, .numcoe = 0, .numret = 6, .octet = 14, .bit = 3, .Cond = { "M", "M" },
        .Ret = (HCI_Field_t[6]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "HCI_Version",                                    .octets =   1 },
          { .name = "HCI_Subversion",                                 .octets =   2 },
          { .name = "LMP_Version",                                    .octets =   1 },
          { .name = "Company_Identifier",                             .octets =   2 },
          { .name = "LMP_Subversion",                                 .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Loopback Mode command", .iscmd = 1, .OGF = 0x06, .section = "7.6.1", .version = "1.1", .BREDR = "C.123", .LE = "E",
    .summary = "The HCI_Read_Loopback_Mode command will read the value for the "
               "setting of the BR/EDR Controller's Loopback Mode. The setting of the "
               "Loopback Mode will determine the path of information.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Loopback_Mode", .OCF = 0x0001, .numcoe = 0, .numret = 2, .octet = 16, .bit = 0, .Cond = { "C.123", "E" },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Loopback_Mode",                                  .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Num Broadcast Retransmissions command", .iscmd = 1, .OGF = 0x03, .section = "7.3.31", .version = "1.1", .BREDR = "C.118", .LE = "E",
    .summary = "The HCI_Read_Num_Broadcast_Retransmissions command will read the "
               "parameter value for the Number of Broadcast Retransmissions for the "
               "BR/EDR Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Num_Broadcast_Retransmissions", .OCF = 0x0029, .numcoe = 0, .numret = 2, .octet =  9, .bit = 6, .Cond = { "C.118", "E" },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Num_Broadcast_Retransmissions",                  .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Number Of Supported IAC command", .iscmd = 1, .OGF = 0x03, .section = "7.3.43", .version = "1.1", .BREDR = "C.125", .LE = "E",
    .summary = "The HCI_Read_Number_Of_Supported_IAC command will read the value for "
               "the number of Inquiry Access Codes (IAC) that the local BR/EDR "
               "Controller can simultaneously listen for during an Inquiry Scan.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Number_Of_Supported_IAC", .OCF = 0x0038, .numcoe = 0, .numret = 2, .octet = 11, .bit = 2, .Cond = { "C.125", "E" },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Num_Supported_IAC",                              .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Page Scan Activity command", .iscmd = 1, .OGF = 0x03, .section = "7.3.19", .version = "1.1", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Read_Page_Scan_Activity command will read the values for the "
               "Page Scan Interval and Page Scan Window configuration parameters. "
               "Page Scan Interval defines the amount of time between consecutive "
               "page scans. Page Scan Window defines the duration of the page scan.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Page_Scan_Activity", .OCF = 0x001b, .numcoe = 0, .numret = 3, .octet =  8, .bit = 0, .Cond = { "M", "E" },
        .Ret = (HCI_Field_t[3]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Page_Scan_Interval",                             .octets =   2 },
          { .name = "Page_Scan_Window",                               .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Page Scan Type command", .iscmd = 1, .OGF = 0x03, .section = "7.3.51", .version = "1.2", .BREDR = "C.119", .LE = "E",
    .summary = "The HCI_Read_Page_Scan_Type command is used to read the page scan "
               "type of the local BR/EDR Controller. The Page Scan Type configuration "
               "parameter can set the page scan to either normal or interlaced scan.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Page_Scan_Type", .OCF = 0x0046, .numcoe = 0, .numret = 2, .octet = 13, .bit = 0, .Cond = { "C.119", "E" },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Page_Scan_Type",                                 .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Page Timeout command", .iscmd = 1, .OGF = 0x03, .section = "7.3.15", .version = "1.1", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Read_Page_Timeout command will read the value for the Page "
               "Reply Timeout configuration parameter, which determines the time the "
               "BR/EDR Controller will wait for the remote device to respond to a "
               "connection request before the local device returns a connection "
               "failure.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Page_Timeout", .OCF = 0x0017, .numcoe = 0, .numret = 2, .octet =  7, .bit = 4, .Cond = { "M", "E" },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Page_Timeout",                                   .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read PIN Type command", .iscmd = 1, .OGF = 0x03, .section = "7.3.5", .version = "1.1", .BREDR = "C.120", .LE = "E",
    .summary = "The HCI_Read_PIN_Type command is used for the Host to read the value "
               "that is specified to indicate whether the Host supports variable PINs "
               "or only fixed PINs.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_PIN_Type", .OCF = 0x0009, .numcoe = 0, .numret = 2, .octet =  6, .bit = 2, .Cond = { "C.120", "E" },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "PIN_Type",                                       .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Remote Extended Features command", .iscmd = 1, .OGF = 0x01, .section = "7.1.22", .version = "1.2", .BREDR = "C.220", .LE = "E",
    .summary = "The HCI_Read_Remote_Extended_Features command requests a list of the "
               "supported extended features of a remote device.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Remote_Extended_Features", .OCF = 0x001c, .numcoe = 2, .numret = 0, .octet =  2, .bit = 6, .Cond = { "C.220", "E" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Page_Number",                                    .octets =   1 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Read_Remote_Extended_Features_Complete" },
    },
  },

  { .title = "Read Remote Extended Features Complete event", .iscmd = 0, .section = "7.7.34", .version = "1.2", .BREDR = "C.220", .LE = "E",
    .summary = "The HCI_Read_Remote_Extended_Features_Complete event is used to "
               "indicate the completion of the process of the Link Manager obtaining "
               "the supported Extended features of the remote BR/EDR Controller "
               "specified by the Connection_Handle event parameter.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Remote_Extended_Features_Complete", .code = 0x0023, .numcoe = 5, .numret = 0,  .Cond = { "C.220", "E" },
        .CoE = (HCI_Field_t[5]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Page_Number",                                    .octets =   1 },
          { .name = "Max_Page_Number",                                .octets =   1 },
          { .name = "Extended_LMP_Features",                          .octets =   8 },
        },
      },
    },
  },

  { .title = "Read Remote Supported Features command", .iscmd = 1, .OGF = 0x01, .section = "7.1.21", .version = "1.1", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Read_Remote_Supported_Features command requests a list of the "
               "supported features of a remote device.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Remote_Supported_Features", .OCF = 0x001b, .numcoe = 1, .numret = 0, .octet =  2, .bit = 5, .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Connection_Handle",                              .octets =   2 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Read_Remote_Supported_Features_Complete" },
    },
  },

  { .title = "Read Remote Supported Features Complete event", .iscmd = 0, .section = "7.7.11", .version = "1.1", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Read_Remote_Supported_Features_Complete event is used to "
               "indicate the completion of the process of the Link Manager obtaining "
               "the supported features of the remote BR/EDR Controller specified by "
               "the Connection_Handle event parameter.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Remote_Supported_Features_Complete", .code = 0x000b, .numcoe = 3, .numret = 0,  .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[3]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "LMP_Features",                                   .octets =   8 },
        },
      },
    },
  },

  { .title = "Read Remote Version Information command", .iscmd = 1, .OGF = 0x01, .section = "7.1.23", .version = "1.1", .BREDR = "O", .LE = "C.3",
    .summary = "The HCI_Read_Remote_Version_Information command will read the values "
               "for the version information for the remote device associated with the "
               "Connection_Handle.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Remote_Version_Information", .OCF = 0x001d, .numcoe = 1, .numret = 0, .octet =  2, .bit = 7, .Cond = { "O", "C.3" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Connection_Handle",                              .octets =   2 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Read_Remote_Version_Information_Complete" },
    },
  },

  { .title = "Read Remote Version Information Complete event", .iscmd = 0, .section = "7.7.12", .version = "1.1", .BREDR = "C.105", .LE = "C.3",
    .summary = "The HCI_Read_Remote_Version_Information_Complete event is used to "
               "indicate the completion of the process of the Link Manager obtaining "
               "the version information of the remote device associated with the "
               "Connection_Handle event parameter.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Remote_Version_Information_Complete", .code = 0x000c, .numcoe = 5, .numret = 0,  .Cond = { "C.105", "C.3" },
        .CoE = (HCI_Field_t[5]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Version",                                        .octets =   1 },
          { .name = "Company_Identifier",                             .octets =   2 },
          { .name = "Subversion",                                     .octets =   2 },
        },
      },
    },
  },

  { .title = "Read RSSI command", .iscmd = 1, .OGF = 0x05, .section = "7.5.4", .version = "1.1", .BREDR = "O", .LE = "C.3",
    .summary = "The HCI_Read_RSSI command will read the value for the Received Signal "
               "Strength Indication (RSSI) for a Connection_Handle to another "
               "Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_RSSI", .OCF = 0x0005, .numcoe = 1, .numret = 3, .octet = 15, .bit = 5, .Cond = { "O", "C.3" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Handle",                                         .octets =   2 },
        },
        .Ret = (HCI_Field_t[3]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Handle",                                         .octets =   2 },
          { .name = "RSSI",                                           .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Scan Enable command", .iscmd = 1, .OGF = 0x03, .section = "7.3.17", .version = "1.1", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Read_Scan_Enable command will read the value for the Scan "
               "Enable configuration parameter, which controls whether or not the "
               "BR/EDR Controller will periodically scan for page attempts and/or "
               "inquiry requests from other BR/EDR Controllers.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Scan_Enable", .OCF = 0x0019, .numcoe = 0, .numret = 2, .octet =  7, .bit = 6, .Cond = { "M", "E" },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Scan_Enable",                                    .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Secure Connections Host Support command", .iscmd = 1, .OGF = 0x03, .section = "7.3.91", .version = "4.1", .BREDR = "C.218", .LE = "E",
    .summary = "The HCI_Read_Secure_Connections_Host_Support command is used to read "
               "the Secure Connections Host Supports parameter from the Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Secure_Connections_Host_Support", .OCF = 0x0079, .numcoe = 0, .numret = 2, .octet = 32, .bit = 2, .Cond = { "C.218", "E" },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Secure_Connections_Host_Support",                .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Simple Pairing Mode command", .iscmd = 1, .OGF = 0x03, .section = "7.3.58", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Read_Simple_Pairing_Mode command reads the Secure Simple "
               "Pairing mode setting in the BR/EDR Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Simple_Pairing_Mode", .OCF = 0x0055, .numcoe = 0, .numret = 2, .octet = 17, .bit = 5, .Cond = { "M", "E" },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Simple_Pairing_Mode",                            .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Stored Link Key command", .iscmd = 1, .OGF = 0x03, .section = "7.3.8", .version = "1.1", .BREDR = "C.121", .LE = "E",
    .summary = "The HCI_Read_Stored_Link_Key command provides the ability to read "
               "whether one or more link keys are stored in the Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Stored_Link_Key", .OCF = 0x000d, .numcoe = 2, .numret = 3, .octet =  6, .bit = 5, .Cond = { "C.121", "E" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "BD_ADDR",                                        .octets =   6 },
          { .name = "Read_All",                                       .octets =   1 },
        },
        .Ret = (HCI_Field_t[3]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Max_Num_Keys",                                   .octets =   2 },
          { .name = "Num_Keys_Read",                                  .octets =   2 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Return_Link_Keys" },
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Synchronization Train Parameters command", .iscmd = 1, .OGF = 0x03, .section = "7.3.89", .version = "CSA4", .BREDR = "C.201", .LE = "E",
    .summary = "The HCI_Read_Synchronization_Train_Parameters command returns the "
               "currently configured values for the Synchronization Train "
               "functionality in the BR/EDR Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Synchronization_Train_Parameters", .OCF = 0x0077, .numcoe = 0, .numret = 4, .octet = 31, .bit = 7, .Cond = { "C.201", "E" },
        .Ret = (HCI_Field_t[4]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Sync_Train_Interval",                            .octets =   2 },
          { .name = "Sync_Train_Timeout",                             .octets =   4 },
          { .name = "Service_Data",                                   .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Synchronous Flow Control Enable command", .iscmd = 1, .OGF = 0x03, .section = "7.3.36", .version = "1.1", .BREDR = "C.122", .LE = "E",
    .summary = "The HCI_Read_Synchronous_Flow_Control_Enable command provides the "
               "ability to read the Synchronous Flow Control Enable setting. By using "
               "this setting, the Host can decide if the Controller will send "
               "HCI_Number_Of_Completed_Packets events for synchronous "
               "Connection_Handles.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Synchronous_Flow_Control_Enable", .OCF = 0x002e, .numcoe = 0, .numret = 2, .octet = 10, .bit = 3, .Cond = { "C.122", "E" },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Synchronous_Flow_Control_Enable",                .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Transmit Power Level command", .iscmd = 1, .OGF = 0x03, .section = "7.3.35", .version = "1.1", .BREDR = "C.152", .LE = "C.3",
    .summary = "The HCI_Read_Transmit_Power_Level command will read the values for "
               "the Transmit Power Level parameter for the specified "
               "Connection_Handle.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Transmit_Power_Level", .OCF = 0x002d, .numcoe = 2, .numret = 3, .octet = 10, .bit = 2, .Cond = { "C.152", "C.3" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Type",                                           .octets =   1 },
        },
        .Ret = (HCI_Field_t[3]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "TX_Power_Level",                                 .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Read Voice Setting command", .iscmd = 1, .OGF = 0x03, .section = "7.3.27", .version = "1.1", .BREDR = "C.134", .LE = "E",
    .summary = "The HCI_Read_Voice_Setting command will read the values for the Voice "
               "Setting configuration parameter, which controls all the various "
               "settings for the voice connections.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Read_Voice_Setting", .OCF = 0x0025, .numcoe = 0, .numret = 2, .octet =  9, .bit = 2, .Cond = { "C.134", "E" },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Voice_Setting",                                  .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Receive Synchronization Train command", .iscmd = 1, .OGF = 0x01, .section = "7.1.52", .version = "CSA4", .BREDR = "C.202", .LE = "E",
    .summary = "The HCI_Receive_Synchronization_Train command requests "
               "synchronization with the specified Connectionless Peripheral "
               "Broadcast transmitter.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Receive_Synchronization_Train", .OCF = 0x0044, .numcoe = 4, .numret = 0, .octet = 31, .bit = 3, .Cond = { "C.202", "E" },
        .CoE = (HCI_Field_t[4]) {
          { .name = "BD_ADDR",                                        .octets =   6 },
          { .name = "Sync_Scan_Timeout",                              .octets =   2 },
          { .name = "Sync_Scan_Window",                               .octets =   2 },
          { .name = "Sync_Scan_Interval",                             .octets =   2 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Synchronization_Train_Received" },
    },
  },

  { .title = "Refresh Encryption Key command", .iscmd = 1, .OGF = 0x03, .section = "7.3.57", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Refresh_Encryption_Key command is used by the Host to cause "
               "the Controller to refresh the encryption key by pausing and resuming "
               "encryption",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Refresh_Encryption_Key", .OCF = 0x0053, .numcoe = 1, .numret = 0, .octet = 17, .bit = 2, .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Connection_Handle",                              .octets =   2 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Encryption_Key_Refresh_Complete" },
    },
  },

  { .title = "Reject Connection Request command", .iscmd = 1, .OGF = 0x01, .section = "7.1.9", .version = "1.1", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Reject_Connection_Request command is used to decline a new "
               "incoming BR/EDR connection request.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Reject_Connection_Request", .OCF = 0x000a, .numcoe = 2, .numret = 0, .octet =  1, .bit = 1, .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "BD_ADDR",                                        .octets =   6 },
          { .name = "Reason",                                         .octets =   1 },
        },
      },
    },
    .numevts = 3, .Events = (HCI_Event_t[3]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Connection_Complete" },
      { .name = "HCI_Synchronous_Connection_Complete" },
    },
  },

  { .title = "Reject Synchronous Connection Request command", .iscmd = 1, .OGF = 0x01, .section = "7.1.28", .version = "1.2", .BREDR = "C.134", .LE = "E",
    .summary = "The HCI_Reject_Synchronous_Connection_Request command is used to "
               "decline an incoming request for a synchronous link.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Reject_Synchronous_Connection_Request", .OCF = 0x002a, .numcoe = 2, .numret = 0, .octet = 16, .bit = 5, .Cond = { "C.134", "E" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "BD_ADDR",                                        .octets =   6 },
          { .name = "Reason",                                         .octets =   1 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Synchronous_Connection_Complete" },
    },
  },

  { .title = "Remote Host Supported Features Notification event", .iscmd = 0, .section = "7.7.50", .version = "2.1 + EDR", .BREDR = "C.106", .LE = "E",
    .summary = "The HCI_Remote_Host_Supported_Features_Notification event is used to "
               "return the LMP extended features page containing the Host features.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Remote_Host_Supported_Features_Notification", .code = 0x003d, .numcoe = 2, .numret = 0,  .Cond = { "C.106", "E" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "BD_ADDR",                                        .octets =   6 },
          { .name = "Host_Supported_Features",                        .octets =   8 },
        },
      },
    },
  },

  { .title = "Remote Name Request Cancel command", .iscmd = 1, .OGF = 0x01, .section = "7.1.20", .version = "1.2", .BREDR = "C.106", .LE = "E",
    .summary = "The HCI_Remote_Name_Request_Cancel command is used to cancel an "
               "ongoing Remote Name Request.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Remote_Name_Request_Cancel", .OCF = 0x001a, .numcoe = 1, .numret = 2, .octet =  2, .bit = 4, .Cond = { "C.106", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "BD_ADDR",                                        .octets =   6 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "BD_ADDR",                                        .octets =   6 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_Remote_Name_Request_Complete" },
    },
  },

  { .title = "Remote Name Request command", .iscmd = 1, .OGF = 0x01, .section = "7.1.19", .version = "1.1", .BREDR = "O", .LE = "E",
    .summary = "The HCI_Remote_Name_Request command is used to obtain the "
               "user-friendly name of another BR/EDR Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Remote_Name_Request", .OCF = 0x0019, .numcoe = 4, .numret = 0, .octet =  2, .bit = 3, .Cond = { "O", "E" },
        .CoE = (HCI_Field_t[4]) {
          { .name = "BD_ADDR",                                        .octets =   6 },
          { .name = "Page_Scan_Repetition_Mode",                      .octets =   1 },
          { .name = "Reserved",                                       .octets =   1 },
          { .name = "Clock_Offset",                                   .octets =   2 },
        },
      },
    },
    .numevts = 3, .Events = (HCI_Event_t[3]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Remote_Host_Supported_Features_Notification" },
      { .name = "HCI_Remote_Name_Request_Complete" },
    },
  },

  { .title = "Remote Name Request Complete event", .iscmd = 0, .section = "7.7.7", .version = "1.1", .BREDR = "C.106", .LE = "E",
    .summary = "The HCI_Remote_Name_Request_Complete event is used to indicate a "
               "remote name request has been completed.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Remote_Name_Request_Complete", .code = 0x0007, .numcoe = 3, .numret = 0,  .Cond = { "C.106", "E" },
        .CoE = (HCI_Field_t[3]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "BD_ADDR",                                        .octets =   6 },
          { .name = "Remote_Name",                                    .octets = 248 },
        },
      },
    },
  },

  { .title = "Remote OOB Data Request event", .iscmd = 0, .section = "7.7.44", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Remote_OOB_Data_Request event is used to indicate that the "
               "Secure Simple Pairing Hash C and Randomizer R is required for the "
               "Secure Simple Pairing process involving the device identified by "
               "BD_ADDR.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Remote_OOB_Data_Request", .code = 0x0035, .numcoe = 1, .numret = 0,  .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "BD_ADDR",                                        .octets =   6 },
        },
      },
    },
  },

  { .title = "Remote OOB Data Request Negative Reply command", .iscmd = 1, .OGF = 0x01, .section = "7.1.35", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Remote_OOB_Data_Request_Negative_Reply command is used to "
               "reply to an HCI_Remote_OOB_Data_Request event that the Host does not "
               "have the C and R",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Remote_OOB_Data_Request_Negative_Reply", .OCF = 0x0033, .numcoe = 1, .numret = 2, .octet = 19, .bit = 7, .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "BD_ADDR",                                        .octets =   6 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "BD_ADDR",                                        .octets =   6 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Remote OOB Data Request Reply command", .iscmd = 1, .OGF = 0x01, .section = "7.1.34", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Remote_OOB_Data_Request_Reply command is used to reply to an "
               "HCI_Remote_OOB_Data_Request event with the C and R values received "
               "via an OOB transfer from a remote BR/ EDR Controller identified by "
               "BD_ADDR.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Remote_OOB_Data_Request_Reply", .OCF = 0x0030, .numcoe = 3, .numret = 2, .octet = 19, .bit = 4, .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[3]) {
          { .name = "BD_ADDR",                                        .octets =   6 },
          { .name = "C",                                              .octets =  16 },
          { .name = "R",                                              .octets =  16 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "BD_ADDR",                                        .octets =   6 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Remote OOB Extended Data Request Reply command", .iscmd = 1, .OGF = 0x01, .section = "7.1.53", .version = "4.1", .BREDR = "C.142", .LE = "E",
    .summary = "The HCI_Remote_OOB_Extended_Data_Request_Reply command is used to "
               "reply to an HCI_Remote_OOB_Data_Request event with the C and R values "
               "received via an OOB transfer from a remote BR/EDR Controller "
               "identified by the BD_ADDR.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Remote_OOB_Extended_Data_Request_Reply", .OCF = 0x0045, .numcoe = 5, .numret = 2, .octet = 32, .bit = 1, .Cond = { "C.142", "E" },
        .CoE = (HCI_Field_t[5]) {
          { .name = "BD_ADDR",                                        .octets =   6 },
          { .name = "C_192",                                          .octets =  16 },
          { .name = "R_192",                                          .octets =  16 },
          { .name = "C_256",                                          .octets =  16 },
          { .name = "R_256",                                          .octets =  16 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "BD_ADDR",                                        .octets =   6 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Reset command", .iscmd = 1, .OGF = 0x03, .section = "7.3.2", .version = "1.1", .BREDR = "M", .LE = "M",
    .summary = "For a BR/EDR Controller, the HCI_Reset command resets HCI, the Link "
               "Manager, and the Bluetooth radio. For an LE Controller, the HCI_Reset "
               "command resets HCI, the Link Layer, and LE PHY.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Reset", .OCF = 0x0003, .numcoe = 0, .numret = 1, .octet =  5, .bit = 7, .Cond = { "M", "M" },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Reset Failed Contact Counter command", .iscmd = 1, .OGF = 0x05, .section = "7.5.2", .version = "1.1", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Reset_Failed_Contact_Counter command will reset the value for "
               "the Failed Contact Counter configuration parameter for a particular "
               "connection to another device.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Reset_Failed_Contact_Counter", .OCF = 0x0002, .numcoe = 1, .numret = 2, .octet = 15, .bit = 3, .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Handle",                                         .octets =   2 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Handle",                                         .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Return Link Keys event", .iscmd = 0, .section = "7.7.21", .version = "1.1", .BREDR = "C.121", .LE = "E",
    .summary = "The HCI_Return_Link_Keys event is used to return stored link keys "
               "after an HCI_Read_Stored_Link_Key command is used.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Return_Link_Keys", .code = 0x0015, .numcoe = 3, .numret = 0,  .Cond = { "C.121", "E" },
        .CoE = (HCI_Field_t[3]) {
          { .name = "Num_Keys",                                       .octets =   1 },
          { .name = "BD_ADDR",                                        .octets =   6, .ref2size = "Num_Keys" },
          { .name = "Link_Key",                                       .octets =  16, .ref2size = "Num_Keys" },
        },
      },
    },
  },

  { .title = "Role Change event", .iscmd = 0, .section = "7.7.18", .version = "1.1", .BREDR = "C.212", .LE = "E",
    .summary = "The HCI_Role_Change event is used to indicate that the current BR/EDR "
               "Controller role related to the particular connection has been "
               "changed.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Role_Change", .code = 0x0012, .numcoe = 3, .numret = 0,  .Cond = { "C.212", "E" },
        .CoE = (HCI_Field_t[3]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "BD_ADDR",                                        .octets =   6 },
          { .name = "New_Role",                                       .octets =   1 },
        },
      },
    },
  },

  { .title = "Role Discovery command", .iscmd = 1, .OGF = 0x02, .section = "7.2.7", .version = "1.1", .BREDR = "O", .LE = "E",
    .summary = "The HCI_Role_Discovery command is used for a BR/EDR Controller to "
               "determine which role the device is performing for a particular "
               "Connection_Handle.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Role_Discovery", .OCF = 0x0009, .numcoe = 1, .numret = 3, .octet =  4, .bit = 7, .Cond = { "O", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Connection_Handle",                              .octets =   2 },
        },
        .Ret = (HCI_Field_t[3]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Current_Role",                                   .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "SAM Status Change event", .iscmd = 0, .section = "7.7.76", .version = "5.0", .BREDR = "C.219", .LE = "E",
    .summary = "The HCI_SAM_Status_Change event is used to indicate that either the "
               "local or remote SAM slot map on a particular connection has been "
               "changed.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_SAM_Status_Change", .code = 0x0058, .numcoe = 7, .numret = 0,  .Cond = { "C.219", "E" },
        .CoE = (HCI_Field_t[7]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Local_SAM_Index",                                .octets =   1 },
          { .name = "Local_SAM_TX_Availability",                      .octets =   1 },
          { .name = "Local_SAM_RX_Availability",                      .octets =   1 },
          { .name = "Remote_SAM_Index",                               .octets =   1 },
          { .name = "Remote_SAM_TX_Availability",                     .octets =   1 },
          { .name = "Remote_SAM_RX_Availability",                     .octets =   1 },
        },
      },
    },
  },

  { .title = "Send Keypress Notification command", .iscmd = 1, .OGF = 0x03, .section = "7.3.63", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Send_Keypress_Notification command is used during the Passkey "
               "Entry protocol by a device with KeyboardOnly IO capabilities. It is "
               "used by a Host to inform the remote device when keys have been "
               "entered or erased.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Send_Keypress_Notification", .OCF = 0x0060, .numcoe = 2, .numret = 2, .octet = 20, .bit = 2, .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "BD_ADDR",                                        .octets =   6 },
          { .name = "Notification_Type",                              .octets =   1 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "BD_ADDR",                                        .octets =   6 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Set AFH Host Channel Classification command", .iscmd = 1, .OGF = 0x03, .section = "7.3.46", .version = "1.2", .BREDR = "C.140", .LE = "E",
    .summary = "The HCI_Set_AFH_Host_Channel_Classification command allows the Host "
               "to specify a channel classification based on its 'local "
               "information'.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Set_AFH_Host_Channel_Classification", .OCF = 0x003f, .numcoe = 1, .numret = 1, .octet = 12, .bit = 1, .Cond = { "C.140", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "AFH_Host_Channel_Classification",                .octets =  10 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Set Connection Encryption command", .iscmd = 1, .OGF = 0x01, .section = "7.1.16", .version = "1.1", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Set_Connection_Encryption command is used to enable and "
               "disable the link level encryption.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Set_Connection_Encryption", .OCF = 0x0013, .numcoe = 2, .numret = 0, .octet =  2, .bit = 0, .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Encryption_Enable",                              .octets =   1 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Encryption_Change [v2]" },
    },
  },

  { .title = "Set Connectionless Peripheral Broadcast command", .iscmd = 1, .OGF = 0x01, .section = "7.1.49", .version = "CSA4", .BREDR = "C.201", .LE = "E",
    .summary = "The HCI_Set_Connectionless_Peripheral_Broadcast command controls "
               "Connectionless Peripheral Broadcast functionality (for transmission) "
               "in the BR/EDR Controller including enabling and disabling the "
               "broadcast.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Set_Connectionless_Peripheral_Broadcast", .OCF = 0x0041, .numcoe = 7, .numret = 3, .octet = 31, .bit = 0, .Cond = { "C.201", "E" },
        .CoE = (HCI_Field_t[7]) {
          { .name = "Enable",                                         .octets =   1 },
          { .name = "LT_ADDR",                                        .octets =   1 },
          { .name = "LPO_Allowed",                                    .octets =   1 },
          { .name = "Packet_Type",                                    .octets =   2 },
          { .name = "Interval_Min",                                   .octets =   2 },
          { .name = "Interval_Max",                                   .octets =   2 },
          { .name = "Supervision_Timeout",                            .octets =   2 },
        },
        .Ret = (HCI_Field_t[3]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "LT_ADDR",                                        .octets =   1 },
          { .name = "Interval",                                       .octets =   2 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_Connectionless_Peripheral_Broadcast_Timeout" },
    },
  },

  { .title = "Set Connectionless Peripheral Broadcast Data command", .iscmd = 1, .OGF = 0x03, .section = "7.3.88", .version = "CSA4", .BREDR = "C.201", .LE = "E",
    .summary = "The HCI_Set_Connectionless_Peripheral_Broadcast_Data command is used "
               "by the Host to set Connectionless Peripheral Broadcast data in the "
               "BR/EDR Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Set_Connectionless_Peripheral_Broadcast_Data", .OCF = 0x0076, .numcoe = 4, .numret = 2, .octet = 31, .bit = 6, .Cond = { "C.201", "E" },
        .CoE = (HCI_Field_t[4]) {
          { .name = "LT_ADDR",                                        .octets =   1 },
          { .name = "Fragment",                                       .octets =   1 },
          { .name = "Data_Length",                                    .octets =   1 },
          { .name = "Data",                                                         .ref2size = "Data_Length" },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "LT_ADDR",                                        .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Set Connectionless Peripheral Broadcast Receive command", .iscmd = 1, .OGF = 0x01, .section = "7.1.50", .version = "CSA4", .BREDR = "C.202", .LE = "E",
    .summary = "The HCI_Set_Connectionless_Peripheral_Broadcast_Receive command "
               "enables and disables Connectionless Peripheral Broadcast reception in "
               "the BR/EDR Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Set_Connectionless_Peripheral_Broadcast_Receive", .OCF = 0x0042, .numcoe = 11, .numret = 3, .octet = 31, .bit = 1, .Cond = { "C.202", "E" },
        .CoE = (HCI_Field_t[11]) {
          { .name = "Enable",                                         .octets =   1 },
          { .name = "BD_ADDR",                                        .octets =   6 },
          { .name = "LT_ADDR",                                        .octets =   1 },
          { .name = "Interval",                                       .octets =   2 },
          { .name = "Clock_Offset",                                   .octets =   4 },
          { .name = "Next_Connectionless_Peripheral_Broadcast_Clock",  .octets =   4 },
          { .name = "Supervision_Timeout",                            .octets =   2 },
          { .name = "Remote_Timing_Accuracy",                         .octets =   1 },
          { .name = "Skip",                                           .octets =   1 },
          { .name = "Packet_Type",                                    .octets =   2 },
          { .name = "AFH_Channel_Map",                                .octets =  10 },
        },
        .Ret = (HCI_Field_t[3]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "BD_ADDR",                                        .octets =   6 },
          { .name = "LT_ADDR",                                        .octets =   1 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_Connectionless_Peripheral_Broadcast_Timeout" },
    },
  },

  { .title = "Set Controller To Host Flow Control command", .iscmd = 1, .OGF = 0x03, .section = "7.3.38", .version = "1.1", .BREDR = "O", .LE = "C.96",
    .summary = "The HCI_Set_Controller_To_Host_Flow_Control command is used by the "
               "Host to turn flow control on or off in the direction from the "
               "Controller to the Host.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Set_Controller_To_Host_Flow_Control", .OCF = 0x0031, .numcoe = 1, .numret = 1, .octet = 10, .bit = 5, .Cond = { "O", "C.96" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Flow_Control_Enable",                            .octets =   1 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Set Ecosystem Base Interval command", .iscmd = 1, .OGF = 0x03, .section = "7.3.100", .version = "5.2", .BREDR = "O", .LE = "O",
    .summary = "The HCI_Set_Ecosystem_Base_Interval command indicates to the "
               "Controller the base interval of the ecosystem.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Set_Ecosystem_Base_Interval", .OCF = 0x0082, .numcoe = 1, .numret = 1, .octet = 45, .bit = 1, .Cond = { "O", "O" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Interval",                                       .octets =   2 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Set Event Filter command", .iscmd = 1, .OGF = 0x03, .section = "7.3.3", .version = "1.1", .BREDR = "C.148", .LE = "E",
    .summary = "The HCI_Set_Event_Filter command is used by the Host to specify "
               "different event filters. The Host may issue this command multiple "
               "times to request various conditions for the same type of event filter "
               "and for different types of event filters.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Set_Event_Filter", .OCF = 0x0005, .numcoe = 3, .numret = 1, .octet =  6, .bit = 0, .Cond = { "C.148", "E" },
        .CoE = (HCI_Field_t[3]) {
          { .name = "Filter_Type",                                    .octets =   1 },
          { .name = "Filter_Condition_Type",                                        },
          { .name = "Condition",                                      .octets =   7 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Set Event Mask command", .iscmd = 1, .OGF = 0x03, .section = "7.3.1", .version = "1.1", .BREDR = "M", .LE = "M",
    .summary = "The HCI_Set_Event_Mask command is used to control which events are "
               "generated by the HCI for the Host.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Set_Event_Mask", .OCF = 0x0001, .numcoe = 1, .numret = 1, .octet =  5, .bit = 6, .Cond = { "M", "M" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Event_Mask",                                     .octets =   8 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Set Event Mask Page 2 command", .iscmd = 1, .OGF = 0x03, .section = "7.3.69", .version = "3.0 + HS", .BREDR = "C.145", .LE = "C.145",
    .summary = "The HCI_Set_Event_Mask_Page 2 command is used to control which events "
               "are generated by the HCI for the Host.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Set_Event_Mask_Page_2", .OCF = 0x0063, .numcoe = 1, .numret = 1, .octet = 22, .bit = 2, .Cond = { "C.145", "C.145" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Event_Mask_Page_2",                              .octets =   8 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Set External Frame Configuration command", .iscmd = 1, .OGF = 0x03, .section = "7.3.81", .version = "CSA3", .BREDR = "C.108", .LE = "O",
    .summary = "The HCI_Set_External_Frame_Configuration command enables an external "
               "device to describe a frame structure to the Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Set_External_Frame_Configuration", .OCF = 0x006f, .numcoe = 6, .numret = 1, .octet = 29, .bit = 7, .Cond = { "C.108", "O" },
        .CoE = (HCI_Field_t[6]) {
          { .name = "MWS_Frame_Duration",                             .octets =   2 },
          { .name = "MWS_Frame_Sync_Assert_Offset",                   .octets =   2 },
          { .name = "MWS_Frame_Sync_Assert_Jitter",                   .octets =   2 },
          { .name = "MWS_Num_Periods",                                .octets =   1 },
          { .name = "Period_Duration",                                .octets =   2, .ref2size = "MWS_Num_Periods" },
          { .name = "Period_Type",                                    .octets =   1, .ref2size = "MWS_Num_Periods" },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Set Min Encryption Key Size command", .iscmd = 1, .OGF = 0x03, .section = "7.3.102", .version = "5.3", .BREDR = "O", .LE = "E",
    .summary = "The HCI_Set_Min_Encryption_Key_Size command is used to modify the "
               "minimum encryption key size that may be negotiated by the "
               "Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Set_Min_Encryption_Key_Size", .OCF = 0x0084, .numcoe = 1, .numret = 1, .octet = 45, .bit = 7, .Cond = { "O", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Min_Encryption_Key_Size",                        .octets =   1 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Set MWS Channel Parameters command", .iscmd = 1, .OGF = 0x03, .section = "7.3.80", .version = "CSA3", .BREDR = "O", .LE = "O",
    .summary = "The HCI_Set_MWS_Channel_Parameters command enables an MWS device to "
               "inform the Controller about the MWS channel configuration.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Set_MWS_Channel_Parameters", .OCF = 0x006e, .numcoe = 6, .numret = 1, .octet = 29, .bit = 6, .Cond = { "O", "O" },
        .CoE = (HCI_Field_t[6]) {
          { .name = "MWS_Channel_Enable",                             .octets =   1 },
          { .name = "MWS_RX_Center_Frequency",                        .octets =   2 },
          { .name = "MWS_TX_Center_Frequency",                        .octets =   2 },
          { .name = "MWS_RX_Channel_Bandwidth",                       .octets =   2 },
          { .name = "MWS_TX_Channel_Bandwidth",                       .octets =   2 },
          { .name = "MWS_Channel_Type",                               .octets =   1 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Set MWS Scan Frequency Table command", .iscmd = 1, .OGF = 0x03, .section = "7.3.84", .version = "CSA3", .BREDR = "O", .LE = "O",
    .summary = "The HCI_Set_MWS_Scan_Frequency_Table command specifies the "
               "frequencies represented by the frequency index supplied by the "
               "MWS_SCAN_FREQUENCY signal.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Set_MWS_Scan_Frequency_Table", .OCF = 0x0072, .numcoe = 3, .numret = 1, .octet = 30, .bit = 2, .Cond = { "O", "O" },
        .CoE = (HCI_Field_t[3]) {
          { .name = "Num_Scan_Frequencies",                           .octets =   1 },
          { .name = "Scan_Frequency_Low",                             .octets =   2, .ref2size = "Num_Scan_Frequencies" },
          { .name = "Scan_Frequency_High",                            .octets =   2, .ref2size = "Num_Scan_Frequencies" },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Set MWS Signaling command", .iscmd = 1, .OGF = 0x03, .section = "7.3.82", .version = "CSA3", .BREDR = "O", .LE = "O",
    .summary = "The HCI_Set_MWS_Signaling command enables an MWS device to inform the "
               "Controller about the timing parameters for the MWS coexistence "
               "interface.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Set_MWS_Signaling", .OCF = 0x0070, .numcoe = 15, .numret = 17, .octet = 30, .bit = 0, .Cond = { "O", "O" },
        .CoE = (HCI_Field_t[15]) {
          { .name = "MWS_RX_Assert_Offset",                           .octets =   2 },
          { .name = "MWS_RX_Assert_Jitter",                           .octets =   2 },
          { .name = "MWS_RX_Deassert_Offset",                         .octets =   2 },
          { .name = "MWS_RX_Deassert_Jitter",                         .octets =   2 },
          { .name = "MWS_TX_Assert_Offset",                           .octets =   2 },
          { .name = "MWS_TX_Assert_Jitter",                           .octets =   2 },
          { .name = "MWS_TX_Deassert_Offset",                         .octets =   2 },
          { .name = "MWS_TX_Deassert_Jitter",                         .octets =   2 },
          { .name = "MWS_Pattern_Assert_Offset",                      .octets =   2 },
          { .name = "MWS_Pattern_Assert_Jitter",                      .octets =   2 },
          { .name = "MWS_Inactivity_Duration_Assert_Offset",          .octets =   2 },
          { .name = "MWS_Inactivity_Duration_Assert_Jitter",          .octets =   2 },
          { .name = "MWS_Scan_Frequency_Assert_Offset",               .octets =   2 },
          { .name = "MWS_Scan_Frequency_Assert_Jitter",               .octets =   2 },
          { .name = "MWS_Priority_Assert_Offset_Request",             .octets =   2 },
        },
        .Ret = (HCI_Field_t[17]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Bluetooth_RX_Priority_Assert_Offset",            .octets =   2 },
          { .name = "Bluetooth_RX_Priority_Assert_Jitter",            .octets =   2 },
          { .name = "Bluetooth_RX_Priority_Deassert_Offset",          .octets =   2 },
          { .name = "Bluetooth_RX_Priority_Deassert_Jitter",          .octets =   2 },
          { .name = "802_RX_Priority_Assert_Offset",                  .octets =   2 },
          { .name = "802_RX_Priority_Assert_Jitter",                  .octets =   2 },
          { .name = "802_RX_Priority_Deassert_Offset",                .octets =   2 },
          { .name = "802_RX_Priority_Deassert_Jitter",                .octets =   2 },
          { .name = "Bluetooth_TX_On_Assert_Offset",                  .octets =   2 },
          { .name = "Bluetooth_TX_On_Assert_Jitter",                  .octets =   2 },
          { .name = "Bluetooth_TX_On_Deassert_Offset",                .octets =   2 },
          { .name = "Bluetooth_TX_On_Deassert_Jitter",                .octets =   2 },
          { .name = "802_TX_On_Assert_Offset",                        .octets =   2 },
          { .name = "802_TX_On_Assert_Jitter",                        .octets =   2 },
          { .name = "802_TX_On_Deassert_Offset",                      .octets =   2 },
          { .name = "802_TX_On_Deassert_Jitter",                      .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Set MWS Transport Layer command", .iscmd = 1, .OGF = 0x03, .section = "7.3.83", .version = "CSA3", .BREDR = "C.109", .LE = "C.109",
    .summary = "The HCI_Set_MWS_Transport_Layer command selects the MWS coexistence "
               "signaling transport layer in the Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Set_MWS_Transport_Layer", .OCF = 0x0071, .numcoe = 3, .numret = 1, .octet = 30, .bit = 1, .Cond = { "C.109", "C.109" },
        .CoE = (HCI_Field_t[3]) {
          { .name = "Transport_Layer",                                .octets =   1 },
          { .name = "To_MWS_Baud_Rate",                               .octets =   4 },
          { .name = "From_MWS_Baud_Rate",                             .octets =   4 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Set MWS_PATTERN Configuration command", .iscmd = 1, .OGF = 0x03, .section = "7.3.85", .version = "CSA3", .BREDR = "C.136", .LE = "E",
    .summary = "The HCI_Set_MWS_PATTERN_Configuration command specifies the "
               "configuration of the pattern indicated over the MWS Coexistence "
               "Transport Layer.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Set_MWS_PATTERN_Configuration", .OCF = 0x0073, .numcoe = 4, .numret = 1, .octet = 30, .bit = 4, .Cond = { "C.136", "E" },
        .CoE = (HCI_Field_t[4]) {
          { .name = "MWS_Pattern_Index",                              .octets =   1 },
          { .name = "MWS_Pattern_Num_Intervals",                      .octets =   1 },
          { .name = "MWS_Pattern_Interval_Duration",                                .ref2size = "MWS_Pattern_Num_Intervals" },
          { .name = "MWS_Pattern_Interval_Type",                      .octets =   1, .ref2size = "MWS_Pattern_Num_Intervals" },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Set Reserved LT_ADDR command", .iscmd = 1, .OGF = 0x03, .section = "7.3.86", .version = "CSA4", .BREDR = "C.201", .LE = "E",
    .summary = "The HCI_Set_Reserved_LT_ADDR command requests that the BR/EDR "
               "Controller reserve a specific LT_ADDR for the purposes of "
               "Connectionless Peripheral Broadcast.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Set_Reserved_LT_ADDR", .OCF = 0x0074, .numcoe = 1, .numret = 2, .octet = 31, .bit = 4, .Cond = { "C.201", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "LT_ADDR",                                        .octets =   1 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "LT_ADDR",                                        .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Set Triggered Clock Capture command", .iscmd = 1, .OGF = 0x05, .section = "7.5.12", .version = "CSA4", .BREDR = "O", .LE = "E",
    .summary = "The HCI_Set_Triggered_Clock_Capture command is used to configure the "
               "Controller to return events containing an estimate of a piconet or "
               "the local Bluetooth clock.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Set_Triggered_Clock_Capture", .OCF = 0x000d, .numcoe = 5, .numret = 1, .octet = 30, .bit = 5, .Cond = { "O", "E" },
        .CoE = (HCI_Field_t[5]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Enable",                                         .octets =   1 },
          { .name = "Which_Clock",                                    .octets =   1 },
          { .name = "LPO_Allowed",                                    .octets =   1 },
          { .name = "Num_Clock_Captures_To_Filter",                   .octets =   1 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_Triggered_Clock_Capture" },
    },
  },

  { .title = "Setup Synchronous Connection command", .iscmd = 1, .OGF = 0x01, .section = "7.1.26", .version = "1.2", .BREDR = "C.134", .LE = "E",
    .summary = "The HCI_Setup_Synchronous_Connection command adds a new or modifies "
               "an existing synchronous logical transport (SCO or eSCO) on a physical "
               "link depending on the Connection_Handle parameter specified.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Setup_Synchronous_Connection", .OCF = 0x0028, .numcoe = 7, .numret = 0, .octet = 16, .bit = 3, .Cond = { "C.134", "E" },
        .CoE = (HCI_Field_t[7]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Transmit_Bandwidth",                             .octets =   4 },
          { .name = "Receive_Bandwidth",                              .octets =   4 },
          { .name = "Max_Latency",                                    .octets =   2 },
          { .name = "Voice_Setting",                                  .octets =   2 },
          { .name = "Retransmission_Effort",                          .octets =   1 },
          { .name = "Packet_Type",                                    .octets =   2 },
        },
      },
    },
    .numevts = 5, .Events = (HCI_Event_t[5]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Synchronous_Connection_Complete" },
      { .name = "HCI_Connection_Complete" },
      { .name = "HCI_Synchronous_Connection_Changed" },
      { .name = "HCI_Connection_Request" },
    },
  },

  { .title = "Simple Pairing Complete event", .iscmd = 0, .section = "7.7.45", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Simple_Pairing_Complete event is used to indicate that the "
               "Secure Simple Pairing process has completed.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Simple_Pairing_Complete", .code = 0x0036, .numcoe = 2, .numret = 0,  .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "BD_ADDR",                                        .octets =   6 },
        },
      },
    },
  },

  { .title = "Sniff Mode command", .iscmd = 1, .OGF = 0x02, .section = "7.2.2", .version = "1.1", .BREDR = "C.214", .LE = "E",
    .summary = "The HCI_Sniff_Mode command is used to alter the behavior of the LM "
               "and have the LM place the local or remote device into Sniff mode.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Sniff_Mode", .OCF = 0x0003, .numcoe = 5, .numret = 0, .octet =  4, .bit = 2, .Cond = { "C.214", "E" },
        .CoE = (HCI_Field_t[5]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Sniff_Max_Interval",                             .octets =   2 },
          { .name = "Sniff_Min_Interval",                             .octets =   2 },
          { .name = "Sniff_Attempt",                                  .octets =   2 },
          { .name = "Sniff_Timeout",                                  .octets =   2 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Mode_Change" },
    },
  },

  { .title = "Sniff Subrating command", .iscmd = 1, .OGF = 0x02, .section = "7.2.14", .version = "2.1 + EDR", .BREDR = "C.221", .LE = "E",
    .summary = "The HCI_Sniff_Subrating command is used to configure the sniff "
               "subrating parameters in the local device.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Sniff_Subrating", .OCF = 0x0011, .numcoe = 4, .numret = 2, .octet = 17, .bit = 4, .Cond = { "C.221", "E" },
        .CoE = (HCI_Field_t[4]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Max_Latency",                                    .octets =   2 },
          { .name = "Min_Remote_Timeout",                             .octets =   2 },
          { .name = "Min_Local_Timeout",                              .octets =   2 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Sniff Subrating event", .iscmd = 0, .section = "7.7.37", .version = "2.1 + EDR", .BREDR = "C.221", .LE = "E",
    .summary = "The HCI_Sniff_Subrating event is used to inform the Host of the local "
               "and remote transmit and receive latencies.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Sniff_Subrating", .code = 0x002e, .numcoe = 6, .numret = 0,  .Cond = { "C.221", "E" },
        .CoE = (HCI_Field_t[6]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Max_TX_Latency",                                 .octets =   2 },
          { .name = "Max_RX_Latency",                                 .octets =   2 },
          { .name = "Min_Remote_Timeout",                             .octets =   2 },
          { .name = "Min_Local_Timeout",                              .octets =   2 },
        },
      },
    },
  },

  { .title = "Start Synchronization Train command", .iscmd = 1, .OGF = 0x01, .section = "7.1.51", .version = "CSA4", .BREDR = "C.201", .LE = "E",
    .summary = "The HCI_Start_Synchronization_Train command enables the "
               "Synchronization Train on the BR/EDR Controller using the currently "
               "configured Synchronization Train parameters.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Start_Synchronization_Train", .OCF = 0x0043, .numcoe = 0, .numret = 0, .octet = 31, .bit = 2, .Cond = { "C.201", "E" },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Status" },
    },
  },

  { .title = "Switch Role command", .iscmd = 1, .OGF = 0x02, .section = "7.2.8", .version = "1.1", .BREDR = "C.212", .LE = "E",
    .summary = "The HCI_Switch_Role command is used to switch Central and Peripheral "
               "roles of the devices on either side of a connection.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Switch_Role", .OCF = 0x000b, .numcoe = 2, .numret = 0, .octet =  5, .bit = 0, .Cond = { "C.212", "E" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "BD_ADDR",                                        .octets =   6 },
          { .name = "Role",                                           .octets =   1 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Role_Change" },
    },
  },

  { .title = "Synchronization Train Complete event", .iscmd = 0, .section = "7.7.67", .version = "CSA4", .BREDR = "C.201", .LE = "E",
    .summary = "The HCI_Synchronization_Train_Complete event indicates that the "
               "Synchronization Train has completed.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Synchronization_Train_Complete", .code = 0x004f, .numcoe = 1, .numret = 0,  .Cond = { "C.201", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
  },

  { .title = "Synchronization Train Received event", .iscmd = 0, .section = "7.7.68", .version = "CSA4", .BREDR = "C.202", .LE = "E",
    .summary = "The HCI_Synchronization_Train_Received event provides the status of "
               "Synchronization Train packets received from the device with the given "
               "BD_ADDR.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Synchronization_Train_Received", .code = 0x0050, .numcoe = 8, .numret = 0,  .Cond = { "C.202", "E" },
        .CoE = (HCI_Field_t[8]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "BD_ADDR",                                        .octets =   6 },
          { .name = "Clock_Offset",                                   .octets =   4 },
          { .name = "AFH_Channel_Map",                                .octets =  10 },
          { .name = "LT_ADDR",                                        .octets =   1 },
          { .name = "Next_Broadcast_Instant",                         .octets =   4 },
          { .name = "Connectionless_Peripheral_Broadcast_Interval",   .octets =   2 },
          { .name = "Service_Data",                                   .octets =   1 },
        },
      },
    },
  },

  { .title = "Synchronous Connection Changed event", .iscmd = 0, .section = "7.7.36", .version = "1.2", .BREDR = "C.134", .LE = "E",
    .summary = "The HCI_Synchronous_Connection_Changed event indicates to the Host "
               "that an existing synchronous connection has been reconfigured.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Synchronous_Connection_Changed", .code = 0x002d, .numcoe = 6, .numret = 0,  .Cond = { "C.134", "E" },
        .CoE = (HCI_Field_t[6]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Transmission_Interval",                          .octets =   1 },
          { .name = "Retransmission_Window",                          .octets =   1 },
          { .name = "RX_Packet_Length",                               .octets =   2 },
          { .name = "TX_Packet_Length",                               .octets =   2 },
        },
      },
    },
  },

  { .title = "Synchronous Connection Complete event", .iscmd = 0, .section = "7.7.35", .version = "1.2", .BREDR = "C.134", .LE = "E",
    .summary = "The HCI_Synchronous_Connection_Complete event indicates to both the "
               "Hosts that a new synchronous connection has been established.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Synchronous_Connection_Complete", .code = 0x002c, .numcoe = 9, .numret = 0,  .Cond = { "C.134", "E" },
        .CoE = (HCI_Field_t[9]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "BD_ADDR",                                        .octets =   6 },
          { .name = "Link_Type",                                      .octets =   1 },
          { .name = "Transmission_Interval",                          .octets =   1 },
          { .name = "Retransmission_Window",                          .octets =   1 },
          { .name = "RX_Packet_Length",                               .octets =   2 },
          { .name = "TX_Packet_Length",                               .octets =   2 },
          { .name = "Air_Mode",                                       .octets =   1 },
        },
      },
    },
  },

  { .title = "Triggered Clock Capture event", .iscmd = 0, .section = "7.7.66", .version = "CSA4", .BREDR = "C.110", .LE = "E",
    .summary = "The HCI_Triggered_Clock_Capture event reports the Bluetooth clock "
               "when an external trigger occurred.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Triggered_Clock_Capture", .code = 0x004e, .numcoe = 4, .numret = 0,  .Cond = { "C.110", "E" },
        .CoE = (HCI_Field_t[4]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Which_Clock",                                    .octets =   1 },
          { .name = "Clock",                                          .octets =   4 },
          { .name = "Slot_Offset",                                    .octets =   2 },
        },
      },
    },
  },

  { .title = "Truncated Page Cancel command", .iscmd = 1, .OGF = 0x01, .section = "7.1.48", .version = "CSA4", .BREDR = "C.129", .LE = "E",
    .summary = "The HCI_Truncated_Page_Cancel command is used to cancel an ongoing "
               "Truncated Page.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Truncated_Page_Cancel", .OCF = 0x0040, .numcoe = 1, .numret = 2, .octet = 30, .bit = 7, .Cond = { "C.129", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "BD_ADDR",                                        .octets =   6 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "BD_ADDR",                                        .octets =   6 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Complete" },
      { .name = "HCI_Truncated_Page_Complete" },
    },
  },

  { .title = "Truncated Page command", .iscmd = 1, .OGF = 0x01, .section = "7.1.47", .version = "CSA4", .BREDR = "C.129", .LE = "E",
    .summary = "The HCI_Truncated_Page command will cause the BR/EDR Controller to "
               "page the BR/EDR Controller with the BD_ADDR specified by the command "
               "parameters and abort the page sequence after receiving the ID "
               "response packet.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Truncated_Page", .OCF = 0x003f, .numcoe = 3, .numret = 0, .octet = 30, .bit = 6, .Cond = { "C.129", "E" },
        .CoE = (HCI_Field_t[3]) {
          { .name = "BD_ADDR",                                        .octets =   6 },
          { .name = "Page_Scan_Repetition_Mode",                      .octets =   1 },
          { .name = "Clock_Offset",                                   .octets =   2 },
        },
      },
    },
    .numevts = 2, .Events = (HCI_Event_t[2]) {
      { .name = "HCI_Command_Status" },
      { .name = "HCI_Truncated_Page_Complete" },
    },
  },

  { .title = "Truncated Page Complete event", .iscmd = 0, .section = "7.7.71", .version = "CSA4", .BREDR = "C.129", .LE = "E",
    .summary = "The HCI_Truncated_Page_Complete event indicates to the Host that a "
               "Truncated Page has completed.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Truncated_Page_Complete", .code = 0x0053, .numcoe = 2, .numret = 0,  .Cond = { "C.129", "E" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "BD_ADDR",                                        .octets =   6 },
        },
      },
    },
  },

  { .title = "User Confirmation Request event", .iscmd = 0, .section = "7.7.42", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .summary = "The HCI_User_Confirmation_Request event is used to indicate that user "
               "confirmation of a numeric value is required.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_User_Confirmation_Request", .code = 0x0033, .numcoe = 2, .numret = 0,  .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "BD_ADDR",                                        .octets =   6 },
          { .name = "Numeric_Value",                                  .octets =   4 },
        },
      },
    },
  },

  { .title = "User Confirmation Request Negative Reply command", .iscmd = 1, .OGF = 0x01, .section = "7.1.31", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .summary = "The HCI_User_Confirmation_Request_Negative_Reply command is used to "
               "reply to an HCI_User_Confirmation_Request event and indicates that "
               "the user selected 'no'. This command will terminate Secure Simple "
               "Pairing.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_User_Confirmation_Request_Negative_Reply", .OCF = 0x002d, .numcoe = 1, .numret = 2, .octet = 19, .bit = 1, .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "BD_ADDR",                                        .octets =   6 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "BD_ADDR",                                        .octets =   6 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "User Confirmation Request Reply command", .iscmd = 1, .OGF = 0x01, .section = "7.1.30", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .summary = "The HCI_User_Confirmation_Request_Reply command is used to reply to "
               "an HCI_User_Confirmation_Request event and indicates that the user "
               "selected 'yes'. It is also used when the Host has no input and no "
               "output capabilities.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_User_Confirmation_Request_Reply", .OCF = 0x002c, .numcoe = 1, .numret = 2, .octet = 19, .bit = 0, .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "BD_ADDR",                                        .octets =   6 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "BD_ADDR",                                        .octets =   6 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "User Passkey Notification event", .iscmd = 0, .section = "7.7.48", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .summary = "The HCI_User_Passkey_Notification event is used to provide a passkey "
               "for the Host to display to the user as required as part of a Secure "
               "Simple Pairing process.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_User_Passkey_Notification", .code = 0x003b, .numcoe = 2, .numret = 0,  .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "BD_ADDR",                                        .octets =   6 },
          { .name = "Passkey",                                        .octets =   4 },
        },
      },
    },
  },

  { .title = "User Passkey Request event", .iscmd = 0, .section = "7.7.43", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .summary = "The HCI_User_Passkey_Request event is used to indicate that a passkey "
               "is required as part of a Secure Simple Pairing process.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_User_Passkey_Request", .code = 0x0034, .numcoe = 1, .numret = 0,  .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "BD_ADDR",                                        .octets =   6 },
        },
      },
    },
  },

  { .title = "User Passkey Request Negative Reply command", .iscmd = 1, .OGF = 0x01, .section = "7.1.33", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .summary = "The HCI_User_Passkey_Request_Negative_Reply command is used to reply "
               "to an HCI_User_Passkey_Request event and indicates the Host could not "
               "provide a passkey. This command will terminate Secure Simple "
               "Pairing.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_User_Passkey_Request_Negative_Reply", .OCF = 0x002f, .numcoe = 1, .numret = 2, .octet = 19, .bit = 3, .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "BD_ADDR",                                        .octets =   6 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "BD_ADDR",                                        .octets =   6 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "User Passkey Request Reply command", .iscmd = 1, .OGF = 0x01, .section = "7.1.32", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .summary = "The HCI_User_Passkey_Request_Reply command is used to reply to an "
               "HCI_User_Passkey_Request event and specifies the Numeric_Value "
               "(passkey) entered by the user to be used in the Secure Simple Pairing "
               "process.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_User_Passkey_Request_Reply", .OCF = 0x002e, .numcoe = 2, .numret = 2, .octet = 19, .bit = 2, .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "BD_ADDR",                                        .octets =   6 },
          { .name = "Numeric_Value",                                  .octets =   4 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "BD_ADDR",                                        .octets =   6 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Write AFH Channel Assessment Mode command", .iscmd = 1, .OGF = 0x03, .section = "7.3.54", .version = "1.2", .BREDR = "C.140", .LE = "C.58",
    .summary = "The HCI_Write_AFH_Channel_Assessment_Mode command will write the "
               "value for the Channel Classification Mode configuration parameter. "
               "This value is used to enable or disable the Controller's channel "
               "assessment scheme.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Write_AFH_Channel_Assessment_Mode", .OCF = 0x0049, .numcoe = 1, .numret = 1, .octet = 13, .bit = 3, .Cond = { "C.140", "C.58" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "AFH_Channel_Assessment_Mode",                    .octets =   1 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Write Authenticated Payload Timeout command", .iscmd = 1, .OGF = 0x03, .section = "7.3.94", .version = "4.1", .BREDR = "C.151", .LE = "C.7",
    .summary = "The HCI_Write_Authenticated_Payload_Timeout command is used to write "
               "the Authenticated Payload Timeout parameter, which is used to set the "
               "maximum time between packets being received from the remote device "
               "without a valid MIC.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Write_Authenticated_Payload_Timeout", .OCF = 0x007c, .numcoe = 2, .numret = 2, .octet = 32, .bit = 5, .Cond = { "C.151", "C.7" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Authenticated_Payload_Timeout",                  .octets =   2 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Write Authentication Enable command", .iscmd = 1, .OGF = 0x03, .section = "7.3.24", .version = "1.1", .BREDR = "O", .LE = "E",
    .summary = "The HCI_Write_Authentication_Enable command will write the value for "
               "the Authentication Enable parameter, which controls whether the "
               "Bluetooth device will require authentication for each connection with "
               "other Bluetooth devices.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Write_Authentication_Enable", .OCF = 0x0020, .numcoe = 1, .numret = 1, .octet =  8, .bit = 5, .Cond = { "O", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Authentication_Enable",                          .octets =   1 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Write Automatic Flush Timeout command", .iscmd = 1, .OGF = 0x03, .section = "7.3.30", .version = "1.1", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Write_Automatic_Flush_Timeout command will write the value "
               "for the Flush Timeout configuration parameter for the specified "
               "Connection_Handle. The Flush Timeout parameter is only used for ACL "
               "connections.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Write_Automatic_Flush_Timeout", .OCF = 0x0028, .numcoe = 2, .numret = 2, .octet =  9, .bit = 5, .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Flush_Timeout",                                  .octets =   2 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Write Class of Device command", .iscmd = 1, .OGF = 0x03, .section = "7.3.26", .version = "1.1", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Write_Class_of_Device command will write the value for the "
               "Class_of_Device configuration parameter, which is used to indicate "
               "its capabilities to other devices.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Write_Class_Of_Device", .OCF = 0x0024, .numcoe = 1, .numret = 1, .octet =  9, .bit = 1, .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Class_Of_Device",                                .octets =   3 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Write Connection Accept Timeout command", .iscmd = 1, .OGF = 0x03, .section = "7.3.14", .version = "1.1", .BREDR = "M", .LE = "C.40",
    .summary = "The HCI_Write_Connection_Accept_Timeout command will write the value "
               "for the Connection Accept Timeout configuration parameter, which "
               "allows the Controller to automatically deny a connection request "
               "after a specified period has occurred, and to refuse a new "
               "connection.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Write_Connection_Accept_Timeout", .OCF = 0x0016, .numcoe = 1, .numret = 1, .octet =  7, .bit = 3, .Cond = { "M", "C.40" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Connection_Accept_Timeout",                      .octets =   2 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Write Current IAC LAP command", .iscmd = 1, .OGF = 0x03, .section = "7.3.45", .version = "1.1", .BREDR = "C.125", .LE = "E",
    .summary = "The HCI_Write_Current_IAC_LAP command will write the LAP(s) used to "
               "create the Inquiry Access Codes (IAC) that the local BR/EDR "
               "Controller is simultaneously scanning for during Inquiry Scans.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Write_Current_IAC_LAP", .OCF = 0x003a, .numcoe = 2, .numret = 1, .octet = 11, .bit = 4, .Cond = { "C.125", "E" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Num_Current_IAC",                                .octets =   1 },
          { .name = "IAC_LAP",                                        .octets =   3, .ref2size = "Num_Current_IAC" },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Write Default Erroneous Data Reporting command", .iscmd = 1, .OGF = 0x03, .section = "7.3.65", .version = "2.1 + EDR", .BREDR = "C.135", .LE = "E",
    .summary = "The HCI_Write_Default_Erroneous_Data_Reporting command will write the "
               "value for the Erroneous Data Reporting configuration parameter, which "
               "controls whether the Bluetooth Controller will provide data for every "
               "(e)SCO interval, with the Packet_Status_Flag in HCI Synchronous Data "
               "packets set according to HCI Synchronous Data packets.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Write_Default_Erroneous_Data_Reporting", .OCF = 0x005b, .numcoe = 1, .numret = 1, .octet = 18, .bit = 3, .Cond = { "C.135", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Erroneous_Data_Reporting",                       .octets =   1 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Write Default Link Policy Settings command", .iscmd = 1, .OGF = 0x02, .section = "7.2.12", .version = "1.2", .BREDR = "C.141", .LE = "E",
    .summary = "The HCI_Write_Default_Link_Policy_Settings command will write the "
               "Default Link Policy configuration parameter for all new connections.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Write_Default_Link_Policy_Settings", .OCF = 0x000f, .numcoe = 1, .numret = 1, .octet =  5, .bit = 4, .Cond = { "C.141", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Default_Link_Policy_Settings",                   .octets =   2 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Write Extended Inquiry Length command", .iscmd = 1, .OGF = 0x03, .section = "7.3.99", .version = "4.1", .BREDR = "C.128", .LE = "E",
    .summary = "The HCI_Write_Extended_Inquiry_Length command is used to write the "
               "Extended Inquiry Length parameter to the Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Write_Extended_Inquiry_Length", .OCF = 0x0081, .numcoe = 1, .numret = 1, .octet = 33, .bit = 3, .Cond = { "C.128", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Extended_Inquiry_Length",                        .octets =   2 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Write Extended Inquiry Response command", .iscmd = 1, .OGF = 0x03, .section = "7.3.56", .version = "2.1 + EDR", .BREDR = "C.205", .LE = "E",
    .summary = "The HCI_Write_Extended_Inquiry_Response command will write the data "
               "that the BR/EDR Controller sends in the extended inquiry response "
               "packet during inquiry response.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Write_Extended_Inquiry_Response", .OCF = 0x0052, .numcoe = 2, .numret = 1, .octet = 17, .bit = 1, .Cond = { "C.205", "E" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "FEC_Required",                                   .octets =   1 },
          { .name = "Extended_Inquiry_Response",                      .octets = 240 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Write Extended Page Timeout command", .iscmd = 1, .OGF = 0x03, .section = "7.3.97", .version = "4.1", .BREDR = "O", .LE = "E",
    .summary = "The HCI_Write_Extended_Page_Timeout command is used to write the "
               "Extended Page Timeout parameter to the Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Write_Extended_Page_Timeout", .OCF = 0x007f, .numcoe = 1, .numret = 1, .octet = 33, .bit = 1, .Cond = { "O", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Extended_Page_Timeout",                          .octets =   2 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Write Flow Control Mode command", .iscmd = 1, .OGF = 0x03, .section = "7.3.73", .version = "3.0 + HS", .BREDR = "C.124", .LE = "E",
    .summary = "The HCI_Write_Flow_Control_Mode command sets the value of the "
               "Flow_Control_Mode configuration parameter for this Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Write_Flow_Control_Mode", .OCF = 0x0067, .numcoe = 1, .numret = 1, .octet = 23, .bit = 1, .Cond = { "C.124", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Flow_Control_Mode",                              .octets =   1 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Write Hold Mode Activity command", .iscmd = 1, .OGF = 0x03, .section = "7.3.34", .version = "1.1", .BREDR = "C.213", .LE = "E",
    .summary = "The HCI_Write_Hold_Mode_Activity command is used to write which "
               "activities should be suspended when the BR/EDR Controller is in Hold "
               "mode.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Write_Hold_Mode_Activity", .OCF = 0x002c, .numcoe = 1, .numret = 1, .octet = 10, .bit = 1, .Cond = { "C.213", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Hold_Mode_Activity",                             .octets =   1 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Write Inquiry Mode command", .iscmd = 1, .OGF = 0x03, .section = "7.3.50", .version = "1.2", .BREDR = "C.146", .LE = "E",
    .summary = "The HCI_Write_Inquiry_Mode command is used to write the Inquiry Mode "
               "configuration parameter of the local BR/EDR Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Write_Inquiry_Mode", .OCF = 0x0045, .numcoe = 1, .numret = 1, .octet = 12, .bit = 7, .Cond = { "C.146", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Inquiry_Mode",                                   .octets =   1 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Write Inquiry Scan Activity command", .iscmd = 1, .OGF = 0x03, .section = "7.3.22", .version = "1.1", .BREDR = "C.125", .LE = "E",
    .summary = "The HCI_Write_Inquiry_Scan_Activity command will write the value for "
               "Inquiry Scan Interval and Inquiry Scan Window configuration "
               "parameters. Inquiry Scan Interval defines the amount of time between "
               "consecutive inquiry scans. Inquiry Scan Window defines the amount of "
               "time for the duration of the inquiry scan.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Write_Inquiry_Scan_Activity", .OCF = 0x001e, .numcoe = 2, .numret = 1, .octet =  8, .bit = 3, .Cond = { "C.125", "E" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Inquiry_Scan_Interval",                          .octets =   2 },
          { .name = "Inquiry_Scan_Window",                            .octets =   2 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Write Inquiry Scan Type command", .iscmd = 1, .OGF = 0x03, .section = "7.3.48", .version = "1.2", .BREDR = "C.125", .LE = "E",
    .summary = "The HCI_Write_Inquiry_Scan_Type command is used to write the Inquiry "
               "Scan Type configuration parameter of the local BR/EDR Controller. The "
               "Inquiry Scan Type configuration parameter can set the inquiry scan to "
               "either normal or interlaced scan.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Write_Inquiry_Scan_Type", .OCF = 0x0043, .numcoe = 1, .numret = 1, .octet = 12, .bit = 5, .Cond = { "C.125", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Scan_Type",                                      .octets =   1 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Write Inquiry Transmit Power Level command", .iscmd = 1, .OGF = 0x03, .section = "7.3.62", .version = "2.1 + EDR", .BREDR = "C.127", .LE = "E",
    .summary = "The HCI_Write_Inquiry_Transmit_Power_Level command is used to write "
               "the transmit power level used to transmit the inquiry (ID) data "
               "packets.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Write_Inquiry_Transmit_Power_Level", .OCF = 0x0059, .numcoe = 1, .numret = 1, .octet = 18, .bit = 1, .Cond = { "C.127", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "TX_Power",                                       .octets =   1 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Write LE Host Support command", .iscmd = 1, .OGF = 0x03, .section = "7.3.79", .version = "4.0", .BREDR = "C.153", .LE = "E",
    .summary = "The HCI_Write_LE_Host_Support command writes the LE Supported Host "
               "setting to the BR/EDR Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Write_LE_Host_Support", .OCF = 0x006d, .numcoe = 2, .numret = 1, .octet = 24, .bit = 6, .Cond = { "C.153", "E" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "LE_Supported_Host",                              .octets =   1 },
          { .name = "Unused",                                         .octets =   1 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Write Link Policy Settings command", .iscmd = 1, .OGF = 0x02, .section = "7.2.10", .version = "1.1", .BREDR = "C.141", .LE = "E",
    .summary = "The HCI_Write_Link_Policy_Settings command will write the Link Policy "
               "configuration parameter for the specified Connection_Handle. The Link "
               "Policy settings allow the Host to specify which Link Modes the Link "
               "Manager can use for the specified Connection_Handle.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Write_Link_Policy_Settings", .OCF = 0x000d, .numcoe = 2, .numret = 2, .octet =  5, .bit = 2, .Cond = { "C.141", "E" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Link_Policy_Settings",                           .octets =   2 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Write Link Supervision Timeout command", .iscmd = 1, .OGF = 0x03, .section = "7.3.42", .version = "1.1", .BREDR = "O", .LE = "E",
    .summary = "The HCI_Write_Link_Supervision_Timeout command will write the value "
               "for the Link Supervision Timeout configuration parameter for the "
               "device. This parameter is used by the Controller to determine link "
               "loss.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Write_Link_Supervision_Timeout", .OCF = 0x0037, .numcoe = 2, .numret = 2, .octet = 11, .bit = 1, .Cond = { "O", "E" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "Link_Supervision_Timeout",                       .octets =   2 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Handle",                                         .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Write Local Name command", .iscmd = 1, .OGF = 0x03, .section = "7.3.11", .version = "1.1", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Write_Local_Name command provides the ability to modify the "
               "user-friendly name for the BR/ EDR Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Write_Local_Name", .OCF = 0x0013, .numcoe = 1, .numret = 1, .octet =  7, .bit = 0, .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Local_Name",                                     .octets = 248 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Write Loopback Mode command", .iscmd = 1, .OGF = 0x06, .section = "7.6.2", .version = "1.1", .BREDR = "C.123", .LE = "E",
    .summary = "The HCI_Write_Loopback_Mode command will write the value for the "
               "setting of the BR/EDR Controllers Loopback Mode. The setting of the "
               "Loopback Mode will determine the path of information.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Write_Loopback_Mode", .OCF = 0x0002, .numcoe = 1, .numret = 1, .octet = 16, .bit = 1, .Cond = { "C.123", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Loopback_Mode",                                  .octets =   1 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Write Num Broadcast Retransmissions command", .iscmd = 1, .OGF = 0x03, .section = "7.3.32", .version = "1.1", .BREDR = "O", .LE = "E",
    .summary = "The HCI_Write_Num_Broadcast_Retransmissions command will write the "
               "parameter value for the Number of Broadcast Retransmissions for the "
               "BR/EDR Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Write_Num_Broadcast_Retransmissions", .OCF = 0x002a, .numcoe = 1, .numret = 1, .octet =  9, .bit = 7, .Cond = { "O", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Num_Broadcast_Retransmissions",                  .octets =   1 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Write Page Scan Activity command", .iscmd = 1, .OGF = 0x03, .section = "7.3.20", .version = "1.1", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Write_Page_Scan_Activity command will write the value for "
               "Page Scan Interval and Page Scan Window configuration parameters. "
               "Page Scan Interval defines the amount of time between consecutive "
               "page scans. Page Scan Window defines the duration of the page scan.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Write_Page_Scan_Activity", .OCF = 0x001c, .numcoe = 2, .numret = 1, .octet =  8, .bit = 1, .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[2]) {
          { .name = "Page_Scan_Interval",                             .octets =   2 },
          { .name = "Page_Scan_Window",                               .octets =   2 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Write Page Scan Type command", .iscmd = 1, .OGF = 0x03, .section = "7.3.52", .version = "1.2", .BREDR = "C.154", .LE = "E",
    .summary = "The HCI_Write_Page_Scan_Type command is used to write the page scan "
               "type of the local BR/EDR Controller. The Page Scan Type configuration "
               "parameter can set the page scan to either normal or interlaced scan.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Write_Page_Scan_Type", .OCF = 0x0047, .numcoe = 1, .numret = 1, .octet = 13, .bit = 1, .Cond = { "C.154", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Page_Scan_Type",                                 .octets =   1 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Write Page Timeout command", .iscmd = 1, .OGF = 0x03, .section = "7.3.16", .version = "1.1", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Write_Page_Timeout command will write the value for the Page "
               "Reply Timeout configuration parameter, which allows the BR/EDR "
               "Controller to define the amount of time a connection request will "
               "wait for the remote device to respond before the local device returns "
               "a connection failure.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Write_Page_Timeout", .OCF = 0x0018, .numcoe = 1, .numret = 1, .octet =  7, .bit = 5, .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Page_Timeout",                                   .octets =   2 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Write PIN Type command", .iscmd = 1, .OGF = 0x03, .section = "7.3.6", .version = "1.1", .BREDR = "O", .LE = "E",
    .summary = "The HCI_Write_PIN_Type command is used for the Host to specify "
               "whether the Host supports variable PIN or only fixed PINs.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Write_PIN_Type", .OCF = 0x000a, .numcoe = 1, .numret = 1, .octet =  6, .bit = 3, .Cond = { "O", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "PIN_Type",                                       .octets =   1 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Write Scan Enable command", .iscmd = 1, .OGF = 0x03, .section = "7.3.18", .version = "1.1", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Write_Scan_Enable command will write the value for the Scan "
               "Enable configuration parameter, which controls whether or not the "
               "BR/EDR Controller will periodically scan for page attempts and/or "
               "inquiry requests from other BR/EDR Controllers.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Write_Scan_Enable", .OCF = 0x001a, .numcoe = 1, .numret = 1, .octet =  7, .bit = 7, .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Scan_Enable",                                    .octets =   1 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Write Secure Connections Host Support command", .iscmd = 1, .OGF = 0x03, .section = "7.3.92", .version = "4.1", .BREDR = "C.218", .LE = "E",
    .summary = "The HCI_Write_Secure_Connections_Host_Support command is used to "
               "write the Secure Connections Host Supports parameter to the "
               "Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Write_Secure_Connections_Host_Support", .OCF = 0x007a, .numcoe = 1, .numret = 1, .octet = 32, .bit = 3, .Cond = { "C.218", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Secure_Connections_Host_Support",                .octets =   1 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Write Secure Connections Test Mode command", .iscmd = 1, .OGF = 0x06, .section = "7.6.8", .version = "4.1", .BREDR = "C.138", .LE = "E",
    .summary = "The HCI_Write_Secure_Connections_Test_Mode command is used to put the "
               "Controller in a test mode where DM1 packets are not allowed to be "
               "used for ACL-U traffic and/or the contents of eSCO payloads can be "
               "looped back.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Write_Secure_Connections_Test_Mode", .OCF = 0x000a, .numcoe = 3, .numret = 2, .octet = 32, .bit = 7, .Cond = { "C.138", "E" },
        .CoE = (HCI_Field_t[3]) {
          { .name = "Connection_Handle",                              .octets =   2 },
          { .name = "DM1_ACL-U_Mode",                                 .octets =   1 },
          { .name = "eSCO_Loopback_Mode",                             .octets =   1 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Connection_Handle",                              .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Write Simple Pairing Debug Mode command", .iscmd = 1, .OGF = 0x06, .section = "7.6.4", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Write_Simple_Pairing_Debug_Mode command configures the BR/EDR "
               "Controller to use a predefined Diffie Hellman private key for Secure "
               "Simple Pairing to enable debug equipment to monitor the encrypted "
               "connection.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Write_Simple_Pairing_Debug_Mode", .OCF = 0x0004, .numcoe = 1, .numret = 1, .octet = 19, .bit = 5, .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Simple_Pairing_Debug_Mode",                      .octets =   1 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Write Simple Pairing Mode command", .iscmd = 1, .OGF = 0x03, .section = "7.3.59", .version = "2.1 + EDR", .BREDR = "M", .LE = "E",
    .summary = "The HCI_Write_Simple_Pairing_Mode command writes the Secure Simple "
               "Pairing mode setting in the BR/EDR Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Write_Simple_Pairing_Mode", .OCF = 0x0056, .numcoe = 1, .numret = 1, .octet = 17, .bit = 6, .Cond = { "M", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Simple_Pairing_Mode",                            .octets =   1 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Write Stored Link Key command", .iscmd = 1, .OGF = 0x03, .section = "7.3.9", .version = "1.1", .BREDR = "O", .LE = "E",
    .summary = "The HCI_Write_Stored_Link_Key command provides the ability to write "
               "one or more link keys to be stored in the Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Write_Stored_Link_Key", .OCF = 0x0011, .numcoe = 3, .numret = 2, .octet =  6, .bit = 6, .Cond = { "O", "E" },
        .CoE = (HCI_Field_t[3]) {
          { .name = "Num_Keys_To_Write",                              .octets =   1 },
          { .name = "BD_ADDR",                                        .octets =   6, .ref2size = "Num_Keys_To_Write" },
          { .name = "Link_Key",                                       .octets =  16, .ref2size = "Num_Keys_To_Write" },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Num_Keys_Written",                               .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Write Synchronization Train Parameters command", .iscmd = 1, .OGF = 0x03, .section = "7.3.90", .version = "CSA4", .BREDR = "C.201", .LE = "E",
    .summary = "The HCI_Write_Synchronization_Train_Parameters command configures the "
               "Synchronization Train functionality in the BR/EDR Controller.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Write_Synchronization_Train_Parameters", .OCF = 0x0078, .numcoe = 4, .numret = 2, .octet = 32, .bit = 0, .Cond = { "C.201", "E" },
        .CoE = (HCI_Field_t[4]) {
          { .name = "Interval_Min",                                   .octets =   2 },
          { .name = "Interval_Max",                                   .octets =   2 },
          { .name = "Sync_Train_Timeout",                             .octets =   4 },
          { .name = "Service_Data",                                   .octets =   1 },
        },
        .Ret = (HCI_Field_t[2]) {
          { .name = "Status",                                         .octets =   1 },
          { .name = "Sync_Train_Interval",                            .octets =   2 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Write Synchronous Flow Control Enable command", .iscmd = 1, .OGF = 0x03, .section = "7.3.37", .version = "1.1", .BREDR = "C.135", .LE = "E",
    .summary = "The HCI_Write_Synchronous_Flow_Control_Enable command provides the "
               "ability to write the Synchronous Flow Control Enable setting. By "
               "using this setting, the Host can decide if the Controller will send "
               "HCI_Number_Of_Completed_Packets events for synchronous "
               "Connection_Handles.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Write_Synchronous_Flow_Control_Enable", .OCF = 0x002f, .numcoe = 1, .numret = 1, .octet = 10, .bit = 4, .Cond = { "C.135", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Synchronous_Flow_Control_Enable",                .octets =   1 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

  { .title = "Write Voice Setting command", .iscmd = 1, .OGF = 0x03, .section = "7.3.28", .version = "1.1", .BREDR = "C.134", .LE = "E",
    .summary = "The HCI_Write_Voice_Setting command will write the values for the "
               "Voice Setting configuration parameter, which controls all the various "
               "settings for the voice connections.",

    .numinst = 1, .Inst = (HCI_Inst_t[1]) {
      { .name = "HCI_Write_Voice_Setting", .OCF = 0x0026, .numcoe = 1, .numret = 1, .octet =  9, .bit = 3, .Cond = { "C.134", "E" },
        .CoE = (HCI_Field_t[1]) {
          { .name = "Voice_Setting",                                  .octets =   2 },
        },
        .Ret = (HCI_Field_t[1]) {
          { .name = "Status",                                         .octets =   1 },
        },
      },
    },
    .numevts = 1, .Events = (HCI_Event_t[1]) {
      { .name = "HCI_Command_Complete" },
    },
  },

};

HCI_Feature_t Features[] = {
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
    .used   = 7, // 10
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

  { .name   = "Periodic Advertising Sync Transfer - Recipient",
    .used   = 2, // 17
    .enable = 1 },

  { .name   = "Periodic Advertising Sync Transfer - Sender",
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

  { .name   = "Advertising Coding Selection",
    .used   = 1, // 29
    .enable = 1 },

  { .name   = "Periodic Advertising with Responses - Advertiser",
    .used   = 2, // 30
    .enable = 1 },

  { .name   = "Periodic Advertising with Responses - Scanner",
    .used   = 2, // 31
    .enable = 1 },

};

HCI_Cond_t Conditions[] = {
  { .name   = "C.1", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if the LE Controller supports transmitting packets, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.2", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if the LE Controller supports receiving packets, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C.3", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if the LE Controller supports Connection State, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C.4", .numBREDR = 0, .numLE = 0, .numFeatures = 1,
    .Features = (HCI_Feat4Cond_t[1]) { { .mand = 1, .index =  0 }, },
    .notes  = "Mandatory if LE Feature (LE Encryption) is supported, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C.6", .numBREDR = 0, .numLE = 0, .numFeatures = 1,
    .Features = (HCI_Feat4Cond_t[1]) { { .mand = 1, .index =  1 }, },
    .notes  = "Mandatory if LE Feature (Connection Parameters Request procedure) is "
              "supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.7", .numBREDR = 0, .numLE = 0, .numFeatures = 2,
    .Features = (HCI_Feat4Cond_t[2]) { { .mand = 1, .index =  0 }, { .mand = 1, .index =  2 }, },
    .notes  = "Mandatory if LE Feature (LE Encryption) and LE Feature (LE Ping) are "
              "supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.8", .numBREDR = 0, .numLE = 0, .numFeatures = 1,
    .Features = (HCI_Feat4Cond_t[1]) { { .mand = 1, .index =  3 }, },
    .notes  = "Mandatory if LE Feature (LE Data Packet Length Extension) is "
              "supported, otherwise optional.",
    .enable = 0 
  },

  { .name   = "C.9", .numBREDR = 0, .numLE = 0, .numFeatures = 1,
    .Features = (HCI_Feat4Cond_t[1]) { { .mand = 1, .index =  4 }, },
    .notes  = "Mandatory if LE Feature (LL Privacy) is supported, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C.10", .numBREDR = 0, .numLE = 0, .numFeatures = 1,
    .Features = (HCI_Feat4Cond_t[1]) { { .mand = 0, .index =  4 }, },
    .notes  = "Optional if LE Feature (LL Privacy) is supported, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C.11", .numBREDR = 0, .numLE = 0, .numFeatures = 2,
    .Features = (HCI_Feat4Cond_t[2]) { { .mand = 1, .index =  5 }, { .mand = 1, .index =  6 }, },
    .notes  = "Mandatory if LE Feature (LE 2M PHY) or LE Feature (LE Coded PHY) is "
              "supported, otherwise optional.",
    .enable = 0 
  },

  { .name   = "C.12", .numBREDR = 0, .numLE = 0, .numFeatures = 3,
    .Features = (HCI_Feat4Cond_t[3]) { { .mand = 1, .index =  5 }, { .mand = 1, .index =  6 }, { .mand = 1, .index =  7 }, },
    .notes  = "Mandatory if LE Feature (LE 2M PHY) or LE Feature (LE Coded PHY) or "
              "LE Feature (Stable Modulation Index - Transmitter) is supported, "
              "otherwise optional if the LE Controller supports transmitting "
              "packets, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.13", .numBREDR = 0, .numLE = 0, .numFeatures = 3,
    .Features = (HCI_Feat4Cond_t[3]) { { .mand = 1, .index =  5 }, { .mand = 1, .index =  6 }, { .mand = 1, .index =  8 }, },
    .notes  = "Mandatory if LE Feature (LE 2M PHY) or LE Feature (LE Coded PHY) or "
              "LE Feature (Stable Modulation Index - Receiver) is supported, "
              "otherwise optional if the LE Controller supports receiving packets, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.15", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if LE Controller supports transmitting scannable "
              "advertisements, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.16", .numBREDR = 0, .numLE = 0, .numFeatures = 1,
    .Features = (HCI_Feat4Cond_t[1]) { { .mand = 1, .index =  9 }, },
    .notes  = "Mandatory if LE Feature (Periodic Advertising) is supported and the "
              "LE Controller supports both Scanning State and Synchronization State, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.17", .numBREDR = 0, .numLE = 0, .numFeatures = 1,
    .Features = (HCI_Feat4Cond_t[1]) { { .mand = 1, .index = 10 }, },
    .notes  = "Mandatory if LE Feature (Extended Advertising) is supported and the "
              "LE Controller supports Advertising State, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.18", .numBREDR = 0, .numLE = 0, .numFeatures = 1,
    .Features = (HCI_Feat4Cond_t[1]) { { .mand = 1, .index =  9 }, },
    .notes  = "Mandatory if LE Feature (Periodic Advertising) is supported and the "
              "LE Controller supports Advertising State, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.19", .numBREDR = 0, .numLE = 0, .numFeatures = 1,
    .Features = (HCI_Feat4Cond_t[1]) { { .mand = 1, .index = 10 }, },
    .notes  = "Mandatory if LE Feature (Extended Advertising) is supported and the "
              "LE Controller supports Scanning State, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.20", .numBREDR = 0, .numLE = 0, .numFeatures = 1,
    .Features = (HCI_Feat4Cond_t[1]) { { .mand = 1, .index = 10 }, },
    .notes  = "Mandatory if LE Feature (Extended Advertising) is supported and the "
              "LE Controller supports Initiating State, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.21", .numBREDR = 0, .numLE = 0, .numFeatures = 1,
    .Features = (HCI_Feat4Cond_t[1]) { { .mand = 1, .index =  9 }, },
    .notes  = "Mandatory if LE Feature (Periodic Advertising) is supported and the "
              "LE Controller supports Synchronization State, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.22", .numBREDR = 0, .numLE = 0, .numFeatures = 1,
    .Features = (HCI_Feat4Cond_t[1]) { { .mand = 1, .index = 11 }, },
    .notes  = "Mandatory if the LE Controller supports sending Transmit Power in "
              "advertisements or if LE Feature (LE Power Control Request) is "
              "supported, otherwise optional.",
    .enable = 0 
  },

  { .name   = "C.23", .numBREDR = 0, .numLE = 0, .numFeatures = 1,
    .Features = (HCI_Feat4Cond_t[1]) { { .mand = 1, .index = 12 }, },
    .notes  = "Mandatory if LE Feature (LE Channel Selection Algorithm #2) is "
              "supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.24", .numBREDR = 0, .numLE = 0, .numFeatures = 2,
    .Features = (HCI_Feat4Cond_t[2]) { { .mand = 1, .index =  4 }, { .mand = 1, .index = 10 }, },
    .notes  = "Mandatory if the LE Controller supports Connection State and either "
              "LE Feature (LL Privacy) or LE Feature (Extended Advertising) is "
              "supported, otherwise optional if the LE Controller supports "
              "Connection State, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.25", .numBREDR = 0, .numLE = 0, .numFeatures = 1,
    .Features = (HCI_Feat4Cond_t[1]) { { .mand = 1, .index = 13 }, },
    .notes  = "Mandatory if LE Feature (Connection CTE Request) is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.26", .numBREDR = 0, .numLE = 0, .numFeatures = 1,
    .Features = (HCI_Feat4Cond_t[1]) { { .mand = 1, .index = 14 }, },
    .notes  = "Mandatory if LE Feature (Connection CTE Response) is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.27", .numBREDR = 0, .numLE = 0, .numFeatures = 1,
    .Features = (HCI_Feat4Cond_t[1]) { { .mand = 1, .index = 15 }, },
    .notes  = "Mandatory if LE Feature (Connectionless CTE Transmitter) is "
              "supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.28", .numBREDR = 0, .numLE = 0, .numFeatures = 1,
    .Features = (HCI_Feat4Cond_t[1]) { { .mand = 1, .index = 16 }, },
    .notes  = "Mandatory if LE Feature (Connectionless CTE Receiver) is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.29", .numBREDR = 0, .numLE = 0, .numFeatures = 2,
    .Features = (HCI_Feat4Cond_t[2]) { { .mand = 1, .index = 14 }, { .mand = 1, .index = 15 }, },
    .notes  = "Mandatory if LE Feature (Connection CTE Response) or LE Feature "
              "(Connectionless CTE Transmitter) is supported, otherwise optional if "
              "the LE Controller supports transmitting packets, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.30", .numBREDR = 0, .numLE = 0, .numFeatures = 2,
    .Features = (HCI_Feat4Cond_t[2]) { { .mand = 1, .index = 13 }, { .mand = 1, .index = 16 }, },
    .notes  = "Mandatory if LE Feature (Connection CTE Request) or LE Feature "
              "(Connectionless CTE Receiver) is supported, otherwise optional if the "
              "LE Controller supports receiving packets, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.31", .numBREDR = 0, .numLE = 0, .numFeatures = 4,
    .Features = (HCI_Feat4Cond_t[4]) { { .mand = 1, .index = 13 }, { .mand = 1, .index = 14 }, { .mand = 1, .index = 15 }, { .mand = 1, .index = 16 }, },
    .notes  = "Mandatory if LE Feature (Connection CTE Request) or LE Feature "
              "(Connection CTE Response) or LE Feature (Connectionless CTE "
              "Transmitter) or LE Feature (Connectionless CTE Receiver) is "
              "supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.32", .numBREDR = 0, .numLE = 0, .numFeatures = 2,
    .Features = (HCI_Feat4Cond_t[2]) { { .mand = 1, .index = 17 }, { .mand = 1, .index =  9 }, },
    .notes  = "Mandatory if LE Feature (Periodic Advertising Sync Transfer - "
              "Recipient) is supported, otherwise optional if LE Feature (Periodic "
              "Advertising) is supported and the LE Controller supports "
              "Synchronization State, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.33", .numBREDR = 0, .numLE = 0, .numFeatures = 1,
    .Features = (HCI_Feat4Cond_t[1]) { { .mand = 1, .index = 18 }, },
    .notes  = "Mandatory if LE Feature (Periodic Advertising Sync Transfer - Sender) "
              "is supported and the LE Controller supports Scanning State, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C.34", .numBREDR = 0, .numLE = 0, .numFeatures = 1,
    .Features = (HCI_Feat4Cond_t[1]) { { .mand = 1, .index = 18 }, },
    .notes  = "Mandatory if LE Feature (Periodic Advertising Sync Transfer - Sender) "
              "is supported and the LE Controller supports Advertising State, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.35", .numBREDR = 0, .numLE = 0, .numFeatures = 1,
    .Features = (HCI_Feat4Cond_t[1]) { { .mand = 1, .index = 17 }, },
    .notes  = "Mandatory if LE Feature (Periodic Advertising Sync Transfer - "
              "Recipient) is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.36", .numBREDR = 0, .numLE = 0, .numFeatures = 3,
    .Features = (HCI_Feat4Cond_t[3]) { { .mand = 1, .index = 19 }, { .mand = 1, .index = 10 }, { .mand = 1, .index = 20 }, },
    .notes  = "Mandatory if the LE Controller supports Central role or supports both "
              "Peripheral role and LE Feature (Channel Classification), otherwise "
              "optional if LE Feature (Extended Advertising) is supported and the LE "
              "Controller supports Advertising State or if LE Feature (Isochronous "
              "Broadcaster) is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.37", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if the LE Controller can change its sleep clock accuracy, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.38", .numBREDR = 0, .numLE = 0, .numFeatures = 2,
    .Features = (HCI_Feat4Cond_t[2]) { { .mand = 1, .index = 21 }, { .mand = 1, .index = 22 }, },
    .notes  = "Mandatory if LE Feature (Connected Isochronous Stream - Central) or "
              "LE Feature (Connected Isochronous Stream - Peripheral) is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.39", .numBREDR = 0, .numLE = 0, .numFeatures = 1,
    .Features = (HCI_Feat4Cond_t[1]) { { .mand = 1, .index = 21 }, },
    .notes  = "Mandatory if LE Feature (Connected Isochronous Stream - Central) is "
              "supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.40", .numBREDR = 0, .numLE = 0, .numFeatures = 1,
    .Features = (HCI_Feat4Cond_t[1]) { { .mand = 1, .index = 22 }, },
    .notes  = "Mandatory if LE Feature (Connected Isochronous Stream - Peripheral) "
              "is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.41", .numBREDR = 0, .numLE = 0, .numFeatures = 1,
    .Features = (HCI_Feat4Cond_t[1]) { { .mand = 1, .index = 20 }, },
    .notes  = "Mandatory if LE Feature (Isochronous Broadcaster) is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.42", .numBREDR = 0, .numLE = 0, .numFeatures = 1,
    .Features = (HCI_Feat4Cond_t[1]) { { .mand = 1, .index = 23 }, },
    .notes  = "Mandatory if LE Feature (Synchronized Receiver role) is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.44", .numBREDR = 0, .numLE = 0, .numFeatures = 4,
    .Features = (HCI_Feat4Cond_t[4]) { { .mand = 1, .index = 24 }, { .mand = 1, .index = 21 }, { .mand = 1, .index = 22 }, { .mand = 1, .index = 24 }, },
    .notes  = "Mandatory if LE Feature (Sleep Clock Accuracy Updates) and either LE "
              "Feature (Connected Isochronous Stream - Central) or LE Feature "
              "(Connected Isochronous Stream - Peripheral) are supported, otherwise "
              "optional if LE Feature (Sleep Clock Accuracy Updates) is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.45", .numBREDR = 0, .numLE = 0, .numFeatures = 3,
    .Features = (HCI_Feat4Cond_t[3]) { { .mand = 1, .index = 21 }, { .mand = 1, .index = 22 }, { .mand = 1, .index = 20 }, },
    .notes  = "Mandatory if LE Feature (Connected Isochronous Stream - Central), or "
              "LE Feature (Connected Isochronous Stream - Peripheral), or LE Feature "
              "(Isochronous Broadcaster) is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.46", .numBREDR = 0, .numLE = 0, .numFeatures = 3,
    .Features = (HCI_Feat4Cond_t[3]) { { .mand = 1, .index = 21 }, { .mand = 1, .index = 22 }, { .mand = 1, .index = 23 }, },
    .notes  = "Mandatory if LE Feature (Connected Isochronous Stream - Central), or "
              "LE Feature (Connected Isochronous Stream - Peripheral), or LE Feature "
              "(Synchronized Receiver role) is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.47", .numBREDR = 0, .numLE = 0, .numFeatures = 4,
    .Features = (HCI_Feat4Cond_t[4]) { { .mand = 1, .index = 21 }, { .mand = 1, .index = 22 }, { .mand = 1, .index = 20 }, { .mand = 1, .index = 23 }, },
    .notes  = "Mandatory if LE Feature (Connected Isochronous Stream - Central), or "
              "LE Feature (Connected Isochronous Stream - Peripheral), or LE Feature "
              "(Isochronous Broadcaster), or LE Feature (Synchronized Receiver role) "
              "is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.49", .numBREDR = 0, .numLE = 0, .numFeatures = 3,
    .Features = (HCI_Feat4Cond_t[3]) { { .mand = 1, .index = 21 }, { .mand = 1, .index = 22 }, { .mand = 1, .index = 25 }, },
    .notes  = "Mandatory if LE Feature (Connected Isochronous Stream - Central) or "
              "LE Feature (Connected Isochronous Stream - Peripheral) or LE Feature "
              "(Connection Subrating) is supported, otherwise optional.",
    .enable = 0 
  },

  { .name   = "C.50", .numBREDR = 0, .numLE = 0, .numFeatures = 3,
    .Features = (HCI_Feat4Cond_t[3]) { { .mand = 0, .index = 21 }, { .mand = 0, .index = 22 }, { .mand = 0, .index = 23 }, },
    .notes  = "Optional if LE Feature (Connected Isochronous Stream - Central), or "
              "LE Feature (Connected Isochronous Stream - Peripheral), or LE Feature "
              "(Synchronized Receiver role) is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.51", .numBREDR = 0, .numLE = 0, .numFeatures = 1,
    .Features = (HCI_Feat4Cond_t[1]) { { .mand = 1, .index = 11 }, },
    .notes  = "Mandatory if LE Feature (LE Power Control Request) is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.52", .numBREDR = 0, .numLE = 0, .numFeatures = 1,
    .Features = (HCI_Feat4Cond_t[1]) { { .mand = 1, .index = 26 }, },
    .notes  = "Mandatory if LE Feature (LE Path Loss Monitoring) is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.53", .numBREDR = 0, .numLE = 0, .numFeatures = 1,
    .Features = (HCI_Feat4Cond_t[1]) { { .mand = 1, .index = 11 }, },
    .notes  = "Mandatory if LE Feature (LE Power Control Request) is supported, "
              "otherwise optional if the LE Controller supports transmitting "
              "packets, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.54", .numBREDR = 0, .numLE = 0, .numFeatures = 1,
    .Features = (HCI_Feat4Cond_t[1]) { { .mand = 1, .index = 27 }, },
    .notes  = "Mandatory if LE Feature (Synchronized Receiver) is supported, "
              "otherwise optional.",
    .enable = 0 
  },

  { .name   = "C.55", .numBREDR = 0, .numLE = 0, .numFeatures = 3,
    .Features = (HCI_Feat4Cond_t[3]) { { .mand = 1, .index = 21 }, { .mand = 1, .index = 22 }, { .mand = 1, .index = 20 }, },
    .notes  = "Mandatory if LE Feature (Connected Isochronous Stream - Central), or "
              "LE Feature (Connected Isochronous Stream - Peripheral), or LE Feature "
              "(Isochronous Broadcaster) is supported, otherwise optional if the LE "
              "Controller supports Connection State, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.56", .numBREDR = 0, .numLE = 0, .numFeatures = 1,
    .Features = (HCI_Feat4Cond_t[1]) { { .mand = 0, .index =  0 }, },
    .notes  = "Optional if LE Feature (LE Encryption) is supported, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C.57", .numBREDR = 0, .numLE = 0, .numFeatures = 1,
    .Features = (HCI_Feat4Cond_t[1]) { { .mand = 1, .index = 25 }, },
    .notes  = "Mandatory if LE Feature (Connection Subrating) is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.58", .numBREDR = 0, .numLE = 0, .numFeatures = 1,
    .Features = (HCI_Feat4Cond_t[1]) { { .mand = 1, .index = 19 }, },
    .notes  = "Mandatory if LE Feature (Channel Classification) is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.59", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if the LE Controller supports Central role, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C.60", .numBREDR = 0, .numLE = 0, .numFeatures = 1,
    .Features = (HCI_Feat4Cond_t[1]) { { .mand = 1, .index =  0 }, },
    .notes  = "Mandatory if the LE Controller supports Central role and LE Feature "
              "(LE Encryption), otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.61", .numBREDR = 0, .numLE = 0, .numFeatures = 1,
    .Features = (HCI_Feat4Cond_t[1]) { { .mand = 1, .index =  0 }, },
    .notes  = "Mandatory if the LE Controller supports Peripheral role and LE "
              "Feature (LE Encryption), otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.62", .numBREDR = 0, .numLE = 0, .numFeatures = 1,
    .Features = (HCI_Feat4Cond_t[1]) { { .mand = 1, .index = 28 }, },
    .notes  = "Mandatory if the LE Controller supports Central role or supports both "
              "Peripheral role and LE Feature (Connection Parameters Request "
              "Procedure), otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.63", .numBREDR = 0, .numLE = 0, .numFeatures = 1,
    .Features = (HCI_Feat4Cond_t[1]) { { .mand = 1, .index =  4 }, },
    .notes  = "Mandatory if the LE Controller supports Scanning state and LE Feature "
              "(LL Privacy), otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.64", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Optional if the Controller supports transmitting packets, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C.65", .numBREDR = 0, .numLE = 0, .numFeatures = 1,
    .Features = (HCI_Feat4Cond_t[1]) { { .mand = 1, .index = 10 }, },
    .notes  = "Mandatory if LE Set Extended Advertising Parameters command [v2] is "
              "supported, otherwise mandatory if LE Feature (Extended Advertising) "
              "is supported and the LE Controller supports Advertising State, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.66", .numBREDR = 0, .numLE = 0, .numFeatures = 2,
    .Features = (HCI_Feat4Cond_t[2]) { { .mand = 1, .index = 29 }, { .mand = 1, .index = 10 }, },
    .notes  = "Mandatory if LE Feature (Advertising Coding Selection) is supported, "
              "otherwise optional if LE Feature (Extended Advertising) is supported "
              "and the LE Controller supports Advertising state, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C.67", .numBREDR = 0, .numLE = 0, .numFeatures = 1,
    .Features = (HCI_Feat4Cond_t[1]) { { .mand = 1, .index = 30 }, },
    .notes  = "Mandatory if LE Feature (Periodic Advertising with Responses - "
              "Advertiser) is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.68", .numBREDR = 0, .numLE = 0, .numFeatures = 1,
    .Features = (HCI_Feat4Cond_t[1]) { { .mand = 1, .index = 31 }, },
    .notes  = "Mandatory if LE Feature (Periodic Advertising with Responses - "
              "Scanner) is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.69", .numBREDR = 0, .numLE = 0, .numFeatures = 2,
    .Features = (HCI_Feat4Cond_t[2]) { { .mand = 1, .index = 30 }, { .mand = 1, .index = 31 }, },
    .notes  = "Mandatory if LE Feature (Periodic Advertising with Responses - "
              "Advertiser) or LE Feature (Periodic Advertising with Responses - "
              "Scanner) is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.94", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if the LE Create Connection or LE Extended Create "
              "Connection command is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.95", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if the LE Request Peer SCA command is supported, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C.96", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Optional if the LE Controller supports Connection State, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C.97", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if Advertising State is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.98", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if Scanning State is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.99", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if LE Generate DHKey command [v2] is supported, otherwise "
              "optional.",
    .enable = 0 
  },

  { .name   = "C.101", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if the Authentication Requested command is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.102", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if the Change Connection Link Key command is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.103", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if the Periodic Inquiry Mode command is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.104", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if the Read Clock Offset command is supported, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C.105", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if the Read Remote Version Information command is "
              "supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.106", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if the Remote Name Request command is supported, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C.107", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if the Set Controller To Host Flow Control command is "
              "supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.108", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if the Set MWS_PATTERN Configuration command is supported, "
              "otherwise optional.",
    .enable = 0 
  },

  { .name   = "C.109", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if the Set MWS Signaling command is supported, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C.110", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if the Set Triggered Clock Capture command is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.111", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if the Write Authentication Enable command is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.112", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if the Write Default Erroneous Data Reporting command is "
              "supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.113", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if the Write Extended Inquiry Length command is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.114", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if the Write Extended Page Timeout command is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.115", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if the Write Inquiry Mode command is supported, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C.116", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if the Write LE Host Support command is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.117", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if the Write Link Supervision Timeout command is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.118", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if the Write Num Broadcast Retransmissions command is "
              "supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.119", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if the Write Page Scan Type command is supported, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C.120", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if the Write PIN Type command is supported, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C.121", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if the Write Stored Link Key command is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.122", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if the Write Synchronous Flow Control Enable command is "
              "supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.123", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if BR/EDR test mode is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.124", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if Data block based flow control is supported, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C.125", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if Inquiry Scan is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.126", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Optional if Inquiry Scan is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.127", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if Inquiry is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.128", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Optional if Inquiry is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.129", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if Truncated page state is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.132", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if multi-slot ACL packets are is supported, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C.133", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if HV2, HV3, or multi-slot or EDR ACL packets are is "
              "supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.134", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if SCO or eSCO is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.135", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Optional if SCO or eSCO is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.136", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Optional if Slot Availability Mask is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.138", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if Secure Connections (Controller) is supported, otherwise "
              "optional if eSCO is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.139", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if the Controller is AFH capable in either role, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C.140", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if the Controller supports AFH classification in either "
              "role or is an AFH capable Central, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.141", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if Role Switch, Hold mode, or Sniff mode is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.142", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if Secure Connections (Host) is supported, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C.144", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if Hold Mode or Sniff Mode is supported, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C.145", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if any event in event mask page 2 is supported, otherwise "
              "optional.",
    .enable = 0 
  },

  { .name   = "C.146", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if the Extended Inquiry Result event or the IO Capability "
              "Request event is supported, otherwise optional if Inquiry is "
              "supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.147", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Optional if the Inquiry Result with RSSI event is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.148", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Optional if any of the Connection Complete, Connection Request, "
              "Extended Inquiry Result, Inquiry Result with RSSI, IO Capability "
              "Request, or Synchronous Connection Complete events is supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.151", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if Secure Connections (Controller) and Ping are supported, "
              "otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.152", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if Power Control is supported, otherwise optional.",
    .enable = 0 
  },

  { .name   = "C.153", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if LE supported in the Controller, otherwise optional.",
    .enable = 0 
  },

  { .name   = "C.154", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if Interlaced Page Scan is supported, otherwise optional.",
    .enable = 0 
  },

  { .name   = "C.155", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if the Write Authenticated Payload Timeout command is "
              "supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.156", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if the Read Local Supported Codecs command [v2] is "
              "supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.157", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if the Read Local Supported Codecs command [v2] is "
              "supported, otherwise optional.",
    .enable = 0 
  },

  { .name   = "C.158", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if the Set Min Encryption Key Size command is supported, "
              "otherwise optional.",
    .enable = 0 
  },

  { .name   = "C.201", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if Connectionless Peripheral Broadcast - Transmitter is "
              "supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.202", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if Connectionless Peripheral Broadcast - Receiver is "
              "supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.205", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if Extended Inquiry Response is supported, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C.212", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if Role Switch is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.213", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if Hold mode is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.214", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if Sniff mode is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.215", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if Broadcast Encryption is supported, otherwise excluded.",
    .enable = 0 
  },

  { .name   = "C.217", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if BR/EDR Enhanced Power Control is supported, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C.218", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if Secure Connections (Controller) is supported, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C.219", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if Slot Availability Mask is supported, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C.220", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if LMP Extended Features mask is supported, otherwise "
              "excluded.",
    .enable = 0 
  },

  { .name   = "C.221", .numBREDR = 0, .numLE = 0, .numFeatures = 0,
    .notes  = "Mandatory if Sniff subrating is supported, otherwise excluded.",
    .enable = 0 
  },

};

#endif // INCLUDE_SPECS
#endif // BT_HCI_SPEC_H
