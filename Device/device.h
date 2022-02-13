#ifndef __DEVICE_H
#define __DEVICE_H

#ifdef __cplusplus
extern "C" {
#endif

class CDev {
public:
    CDev() ~CDev();
    virtual int init();
    virtual read();
    virtual write();
};

#ifdef __cplusplus
}
#endif

#endif