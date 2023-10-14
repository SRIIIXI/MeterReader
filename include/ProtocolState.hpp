#ifndef PROTOCOL_STATE
#define PROTOCOL_STATE

const int SNRM_SEND = 10;
const int UARS_RECV = 11;
const int AARQ_SEND = 12;
const int AARE_RECV = 13;
const int FSTOC_SEND = 14;
const int FCTOS_RECV = 15;
const int DISCN_SEND = 16;
const int DISCN_RECV = 17;

const int AVIEW_RQS_SEND = 100;
const int AVIEW_RPP_RECV = 101;
const int AVIEW_RPF_RECV = 102;

const int OBIS_RQS_SEND = 1000;
const int OBIS_RSP_RECV = 1001;

const int PROTO_ERROR = -1;

const int PROTO_TX_READY = 10000;
const int PROTO_RX_READY = 10001;

#endif
