//
// Created on 12/23/17.
//

#ifndef RSOCK_RCAP_H
#define RSOCK_RCAP_H

#include <string>

#include <pcap.h>
#include <uv.h>
#include "cap_util.h"
#include "../util/RPortList.h"

// todo: move icap construction to irawconn
class RCap {
public:
    struct CapThreadArgs {
        RCap *instance;
        pcap_handler handler;
        u_char *args;
    };

    // todo: remove for testing
    RCap(const std::string &dev, const std::string &selfIp, const RPortList &selfPorts, const RPortList &srcPorts,
         const std::string &srcIp = "", int timeout_ms = 20);

    virtual ~RCap() = default;

    virtual int Init();

    virtual int Close();

    // run in a seperate thread.
    uv_thread_t Start(pcap_handler handler, u_char *args);

    // run in the calling thread.
    void Run(pcap_handler handler, u_char *args);

    int Datalink();

    virtual void SetCapHandler(pcap_handler handler, u_char *args);

    virtual void HandlePkt(u_char *args, const struct pcap_pkthdr *hdr, const u_char *pkt);

protected:
    static void threadCb(void *threadArg);
    static void capHandler(u_char *, const struct pcap_pkthdr *, const u_char *);

private:

    int initDevAndIp();

    std::string mSrcIp;
    std::string mDstIp;
    std::string mDev;
    pcap_t *mCap = nullptr;
    RPortList mSrc;
    RPortList mDest;
    bool mDone = false;
    const int TIMEOUT = 20;

    bool mInited = false;
    u_char *mArgs = nullptr;
    pcap_handler mHandler = nullptr;
};


#endif //RSOCK_RCAP_H
